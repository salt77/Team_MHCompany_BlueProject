#include "DynamicCamera.h" 

#include "Run_Map.h" 

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag)
	: CCamera(pGraphicDev, eObjTag)
	, m_Hwnd(hWnd)
{
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
	: CCamera(rhs)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
									   HWND hWnd,
									   T_CAMINFO tCamInfo,
									   E_OBJTAG eObjTag)
{
	CDynamicCamera*		pInstance = new CDynamicCamera(pGraphicDev, hWnd, eObjTag);

	if (FAILED(pInstance->Ready_Object(tCamInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free()
{
	CCamera::Free();
}

// ī�޶� �ʱ�ȭ 
HRESULT CDynamicCamera::Ready_Object(T_CAMINFO tCamInfo)
{
	m_tCamInfo.vEye = tCamInfo.vEye;
	m_tCamInfo.vAt = tCamInfo.vAt;
	m_tCamInfo.vUp = tCamInfo.vUp;

	m_tCamInfo.fFov = tCamInfo.fFov;
	m_tCamInfo.fAspect = tCamInfo.fAspect;
	m_tCamInfo.fNear = tCamInfo.fNear;
	m_tCamInfo.fFar = tCamInfo.fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	m_fSpeed = 10.f; 

	return S_OK;
}

// Update 
Engine::_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::Update_Object(fTimeDelta);

	Key_Input(fTimeDelta);
	Update_MousePos(); 

	if (true == m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}

	return iExit;
}

_int CDynamicCamera::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::LateUpdate_Object(fTimeDelta);

	return iExit;
}

// �⺻ �̵� �� ���콺 Lock & Unlock 
void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_tCamInfo.matView);

	// �Ϲ����� �̵� 
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_W))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_tCamInfo.vEye += vLength;
		m_tCamInfo.vAt += vLength;

		m_eCamMode = CCamera::CAM_FREE;
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_S))
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_tCamInfo.vEye -= vLength;
		m_tCamInfo.vAt -= vLength;

		m_eCamMode = CCamera::CAM_FREE;
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_D))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_tCamInfo.vEye += vLength;
		m_tCamInfo.vAt += vLength;

		m_eCamMode = CCamera::CAM_FREE;
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_A))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_tCamInfo.vEye -= vLength;
		m_tCamInfo.vAt -= vLength;

		m_eCamMode = CCamera::CAM_FREE;
	}

	// ���콺 Lock & Unlock 
	if (!m_bCameraToolMode)
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_C))
		{
			if (!m_bFix)
			{
				Mouse_Fix();
			}

			m_bFix = true;
		}
		else
		{
			m_bFix = false;
		}
	}
	else
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_C))
		{
			m_bFix = !m_bFix; 
		}
	}
	

	// ī�޶� �ӵ� ���� (�ӽ� ���ǿ�, �ּ� 0 �ִ� 200����)
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_PLUS) || (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) && CKeyManager::GetInstance()->Key_Pressing(KEY_F)))
	{
		m_fSpeed += 20.f * fTimeDelta;
	}
	else if (CKeyManager::GetInstance()->Key_Pressing(KEY_MINUS) || (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) && CKeyManager::GetInstance()->Key_Pressing(KEY_D)))
	{
		m_fSpeed -= 20.f * fTimeDelta;
	}

	m_fSpeed = CUtility::Clamp_float(m_fSpeed, 0.f, 200.f);
}

// ���콺 ������ ������Ʈ 
void CDynamicCamera::Update_MousePos()
{
	POINT pPoint = CUtility::Get_MousePoint(m_Hwnd);
	ClientToScreen(m_Hwnd, &pPoint);

	m_vMovePos.x = pPoint.x - m_vCenterPos.x;
	m_vMovePos.y = pPoint.y - m_vCenterPos.y;

	m_vPtMousePos.x = (_float)pPoint.x;
	m_vPtMousePos.y = (_float)pPoint.y;
}

// ���콺 �����ӿ� ���� �þ� ȸ�� 
void CDynamicCamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_tCamInfo.matView);

	if (m_vMovePos.y != 0.f)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = Get_CamLook();

		_matrix		matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_vMovePos.y / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		// ���� 180�� ���� 
		if (Can_Angle(m_tCamInfo.vEye, m_tCamInfo.vEye + vLook))
		{
			m_tCamInfo.vAt = m_tCamInfo.vEye + vLook;
		}
	}

	if (m_vMovePos.x != 0.f)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vUp = *D3DXVec3Cross(&m_vTemp, &Get_CamLookNormalize(), &vRight);
		D3DXVec3Normalize(&vUp, &vUp);

		_vec3	vLook = Get_CamLook();

		_matrix		matRot;
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationAxis(&matRot, &VEC_UP, D3DXToRadian(m_vMovePos.x / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_tCamInfo.vAt = m_tCamInfo.vEye + vLook;
	}
}

// Ŀ�� �߾� ���� 
void CDynamicCamera::Mouse_Fix()
{
	POINT		pPoint = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(m_Hwnd, &pPoint);
	SetCursorPos(pPoint.x, pPoint.y);

	m_vCenterPos.x = (_float)pPoint.x;
	m_vCenterPos.y = (_float)pPoint.y;
}

void CDynamicCamera::Update_ShakeCamera(const _float & fTimeDelta)
{
	if (VEC_ZERO != m_vActionCamInterpol)
	{
		m_tCamInfo.vAt = m_vActionCamInterpol + m_vShakeInterpol - m_vPreShakeInterpol;
	}
	else
	{
		m_tCamInfo.vAt = m_tCamInfo.vAt + m_vShakeInterpol - m_vPreShakeInterpol;
	}
	
	m_vPreShakeInterpol = m_vShakeInterpol;
}

// ī�޶� ��, �Ʒ� ȸ���� 180���� �����ϴ� �Լ� 
_bool CDynamicCamera::Can_Angle(_vec3 vPos, _vec3 vTargetPos)
{
	// vPos = m_vEye;  
	// vTargetPos = ����� ������ m_vAt; 

	_bool		bFront = true;

	_vec3	vDir = vTargetPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3	vFrontDir = Get_CamLook();
	vFrontDir.y = 0.f;
	D3DXVec3Normalize(&vFrontDir, &vFrontDir);

	vDir.y = 0.f;

	if (0.f > D3DXVec3Dot(&vDir, &vFrontDir))
	{
		bFront = false;
	}

	if (bFront)
	{
		return true;
	}

	return false;
}
