#include "Camera_TPS.h"

USING(Engine)

CCamera_TPS::CCamera_TPS(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag)
	: CCamera(pGraphicDev, eObjTag)
	, m_Hwnd(hWnd)
{
}

CCamera_TPS::CCamera_TPS(const CCamera_TPS& rhs)
	: CCamera(rhs)
{
}

CCamera_TPS::~CCamera_TPS()
{
}

CCamera_TPS* CCamera_TPS::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								 HWND hWnd,
								 T_CAMINFO tCamInfo,
								 E_OBJTAG eObjTag)
{
	CCamera_TPS*		pInstance = new CCamera_TPS(pGraphicDev, hWnd, eObjTag);

	if (FAILED(pInstance->Ready_Object(tCamInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCamera_TPS::Free()
{
	CCamera::Free();
}


HRESULT CCamera_TPS::Ready_Object(T_CAMINFO tCamInfo)
{
	m_tCamInfo.vEye = tCamInfo.vEye;
	m_tCamInfo.vAt = tCamInfo.vAt;
	m_tCamInfo.vUp = tCamInfo.vUp;

	m_tCamInfo.fFov = tCamInfo.fFov;
	m_tCamInfo.fAspect = tCamInfo.fAspect;
	m_tCamInfo.fNear = tCamInfo.fNear;
	m_tCamInfo.fFar = tCamInfo.fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	return S_OK;
}

Engine::_int CCamera_TPS::Update_Object(const _float& fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::Update_Object(fTimeDelta);

	TPS_CameraPosition();

	Key_Input(fTimeDelta);
	Update_MousePos();

	if (m_bFix)
	{
		Mouse_Fix();
		Mouse_Move();
	}

	return iExit;
}

_int CCamera_TPS::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CCamera_TPS::Key_Input(const _float& fTimeDelta)
{
	//_matrix		matCamWorld;
	//D3DXMatrixInverse(&matCamWorld, NULL, &m_tCamInfo.matView);

	if (CKeyManager::GetInstance()->Key_Down(KEY_C))
	{
		m_bFix = !m_bFix;
	}
}

// 말 그대로 마우스 포지션 업데이트 
void CCamera_TPS::Update_MousePos()
{
	POINT pPoint = CUtility::Get_MousePoint(m_Hwnd);
	ClientToScreen(m_Hwnd, &pPoint);

	m_vMovePos.x = pPoint.x - m_vCenterPos.x;
	m_vMovePos.y = pPoint.y - m_vCenterPos.y;

	m_vPtMousePos.x = (_float)pPoint.x;
	m_vPtMousePos.y = (_float)pPoint.y;
}

void CCamera_TPS::Mouse_Move()
{
	//_matrix		matCamWorld;
	//D3DXMatrixInverse(&matCamWorld, NULL, &m_tCamInfo.matView);
}

void CCamera_TPS::Mouse_Fix()
{
	POINT		pPoint = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(m_Hwnd, &pPoint);
	SetCursorPos(pPoint.x, pPoint.y);

	m_vCenterPos.x = (_float)pPoint.x;
	m_vCenterPos.y = (_float)pPoint.y;
}

void CCamera_TPS::TPS_CameraPosition()
{
	m_pPlayer = static_cast<CCharacter*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Player));
	if (!m_pPlayer)
		return; 

	_vec3	vTargetPos = VEC_ZERO; 
	_vec3	vMoveDir = VEC_ZERO; 
	_vec3	vPlayerPos = m_pPlayer->Get_Info(E_INFO::INFO_POS);
	_vec3	vPlayerBackDir = -m_pPlayer->Get_Info(E_INFO::INFO_LOOK);
	_vec3	vPlayerRightDir = m_pPlayer->Get_Info(E_INFO::INFO_RIGHT); 
	_vec3	vPlayerScale = m_pPlayer->Get_Scale(); 

	D3DXVec3Normalize(&vPlayerBackDir, &vPlayerBackDir);
	D3DXVec3Normalize(&vPlayerRightDir, &vPlayerRightDir); 

	switch (m_eTpsCamMode)
	{
	case CCamera_TPS::TPS_Normal:
		vTargetPos = vPlayerPos + vPlayerBackDir * (vPlayerScale.x * m_fPlayerToCamDis);
		vTargetPos.y = vPlayerScale.x * m_fDistanceFromGround;
		m_tCamInfo.vAt = vPlayerPos + _vec3(0.f, m_fDistanceFromGround, 0.f);

		if (0.1f <= D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)))
		{
			vMoveDir = *D3DXVec3Normalize(&m_vTemp, &(vTargetPos - m_tCamInfo.vEye));
			m_tCamInfo.vEye += vMoveDir * D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)) * 0.03f;
		}
		break;

	case CCamera_TPS::TPS_Aiming:
		vTargetPos = vPlayerPos + (vPlayerBackDir + vPlayerRightDir * 0.75f) * ((vPlayerScale.x * m_fPlayerToCamDis) - 0.75f);
		vTargetPos.y = vPlayerScale.x * m_fDistanceFromGround - 0.3f;

		if (0.1f <= D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)))
		{
			vMoveDir = *D3DXVec3Normalize(&m_vTemp, &(vTargetPos - m_tCamInfo.vEye)); 
			m_tCamInfo.vEye += vMoveDir * D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)) * 0.01f;
		}

		m_tCamInfo.vAt = vPlayerPos - vPlayerBackDir * 10.f;
		break;

	case CCamera_TPS::TPS_Stand:
		vTargetPos = vPlayerPos + (vPlayerBackDir + vPlayerRightDir * 0.75f) * ((vPlayerScale.x * m_fPlayerToCamDis) - 0.75f);
		vTargetPos.y = vPlayerScale.x * m_fDistanceFromGround;

		if (0.1f <= D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)))
		{
			vMoveDir = *D3DXVec3Normalize(&m_vTemp, &(vTargetPos - m_tCamInfo.vEye));
			m_tCamInfo.vEye += vMoveDir * D3DXVec3Length(&(m_tCamInfo.vEye - vTargetPos)) * 0.01f;
		}

		m_tCamInfo.vAt = vPlayerPos - vPlayerBackDir;
		break;

	default:
		assert(0); 
		break;
	}
}

// 카메라 위, 아래 회전각 180도로 고정하는 함수 
_bool CCamera_TPS::Can_Angle(_vec3 vPos, _vec3 vTargetPos)
{
	// vPos = m_vEye;  
	// vTargetPos = 변경된 이후의 m_vAt; 

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
