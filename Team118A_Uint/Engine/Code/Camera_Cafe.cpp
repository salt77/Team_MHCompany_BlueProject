#include "Camera_Cafe.h"

CCamera_Cafe::CCamera_Cafe(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag)
	: CCamera(pGraphicDev, eObjTag)
	, m_Hwnd(hWnd)
{
}

CCamera_Cafe::CCamera_Cafe(const CCamera_Cafe& rhs)
	: CCamera(rhs)
{
}

CCamera_Cafe::~CCamera_Cafe()
{
}

CCamera_Cafe* CCamera_Cafe::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	HWND hWnd,
	T_CAMINFO tCamInfo,
	E_OBJTAG eObjTag)
{
	CCamera_Cafe*		pInstance = new CCamera_Cafe(pGraphicDev, hWnd, eObjTag);

	if (FAILED(pInstance->Ready_Object(tCamInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCamera_Cafe::Free()
{
	CCamera::Free();
}

//카메라 구조체 정보를 받아서 대입해준다.
HRESULT CCamera_Cafe::Ready_Object(T_CAMINFO tCamInfo)
{
	//피킹 마우스 좌표 초기화
	m_pPickZoom.x = 0;
	m_pPickZoom.y = 0;
	m_pPickMove.x = 0;
	m_pPickMove.y = 0;

	m_tCamInfo.vEye = tCamInfo.vEye;
	m_tCamInfo.vAt = tCamInfo.vAt;
	m_tCamInfo.vUp = tCamInfo.vUp;

	m_tCamInfo.fFov = tCamInfo.fFov;
	m_tCamInfo.fAspect = tCamInfo.fAspect;
	m_tCamInfo.fNear = tCamInfo.fNear;
	m_tCamInfo.fFar = tCamInfo.fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	m_fForwardMoveDis = 0.f;
	m_fHorizonMoveDis = 0.f;

	m_vVerticalVirtualPos = m_vHorizonVirtualPos = m_vForwardVirtualPos = m_tCamInfo.vEye;

	return S_OK;
}

//실시간으로 마우스 위치 업데이트 및 키 조작
Engine::_int CCamera_Cafe::Update_Object(const _float& fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::Update_Object(fTimeDelta);

	PointMove(fTimeDelta);
	Update_MousePos();

	return iExit;
}

//카메라 타입이 다른 경우 예외처리
_int CCamera_Cafe::LateUpdate_Object(const _float & fTimeDelta)
{
	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::LateUpdate_Object(fTimeDelta);

	return iExit;
}

//마우스 위치 업데이트
void CCamera_Cafe::Update_MousePos()
{
	POINT pPoint = CUtility::Get_MousePoint(m_Hwnd);
	ClientToScreen(m_Hwnd, &pPoint);

	m_vMovePos.x = pPoint.x - m_vCenterPos.x;
	m_vMovePos.y = pPoint.y - m_vCenterPos.y;

	m_vPtMousePos.x = (_float)pPoint.x;
	m_vPtMousePos.y = (_float)pPoint.y;
}

//좌,우,상,하 일정 거리 이상 못나가게 제한
void CCamera_Cafe::Compute_Distance(const _vec3& vFrontDir)
{
	m_fForwardMoveDis = D3DXVec3Length(&(m_vForwardVirtualPos - m_tCamInfo.vEye));
	m_fVerticalMoveDis = D3DXVec3Length(&(m_vVerticalVirtualPos - m_tCamInfo.vEye));
	m_fHorizonMoveDis = D3DXVec3Length(&(m_vHorizonVirtualPos - m_tCamInfo.vEye));
	m_fForwardMoveDis = CUtility::Clamp_float(m_fForwardMoveDis, -m_fForwardLimit, m_fForwardLimit);
	m_fVerticalMoveDis = CUtility::Clamp_float(m_fVerticalMoveDis, -m_fVerticalLimit, m_fVerticalLimit);
	m_fHorizonMoveDis = CUtility::Clamp_float(m_fHorizonMoveDis, -m_fHorizonLimit, m_fHorizonLimit);

	if (0.f < m_fHorizonMoveDis)
	{
		if (FALSE == CUtility::CheckRight_vec3(vFrontDir, m_vHorizonVirtualPos - m_tCamInfo.vEye))
		{
			m_fHorizonMoveDis = -m_fHorizonMoveDis;
		}
	}

	if (0.f < m_fForwardMoveDis)
	{
		if (m_tCamInfo.vEye.y < m_vForwardVirtualPos.y)
		{
			m_fForwardMoveDis = -m_fForwardMoveDis;
		}
	}

	if (0.f < m_fVerticalMoveDis)
	{
		if (m_tCamInfo.vEye.y < m_vVerticalVirtualPos.y)
		{
			m_fVerticalMoveDis = -m_fVerticalMoveDis;
		}
	}
}

//카메라를 마우스로 조작
void CCamera_Cafe::PointMove(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_tCamInfo.matView);

	_vec3	vLookDir = m_tCamInfo.vAt - m_tCamInfo.vEye;
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	_vec3	vRight = *D3DXVec3Cross(&m_vTemp, &m_tCamInfo.vUp, &vLookDir);
	D3DXVec3Normalize(&vRight, &vRight);

	if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
	{
		GetCursorPos(&m_pPickMove);
		ClientToScreen(m_Hwnd, &m_pPickMove);
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON) && CKeyManager::GetInstance()->Key_Pressing(KEY_RBUTTON))
	{
		GetCursorPos(&m_pPickZoom);
		ClientToScreen(m_Hwnd, &m_pPickZoom);
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_LBUTTON) && CKeyManager::GetInstance()->Key_Pressing(KEY_RBUTTON))
	{
		if (m_pPickZoom.x != 0 && m_pPickZoom.y != 0)
		{
			POINT pZoom;
			GetCursorPos(&pZoom);
			ClientToScreen(m_Hwnd, &pZoom);

			m_fZoomTime += fTimeDelta;

			if (m_fZoomTime >= 0.05f)
			{
				GetCursorPos(&m_pPickZoom);
				ClientToScreen(m_Hwnd, &m_pPickZoom);

				m_fZoomTime = 0.f;
			}

			if (m_pPickZoom.y > pZoom.y)
			{
				if (-m_fForwardLimit < m_fForwardMoveDis &&
					1.f < m_tCamInfo.vEye.y)
				{
					m_tCamInfo.vEye += vLookDir * m_fSpeed * fTimeDelta;
					m_tCamInfo.vAt += vLookDir * m_fSpeed * fTimeDelta;

					m_vVerticalVirtualPos += vLookDir * m_fSpeed * fTimeDelta;
					m_vHorizonVirtualPos += vLookDir * m_fSpeed * fTimeDelta;
				}
			}

			if (m_pPickZoom.y < pZoom.y)
			{
				if (m_fForwardLimit > m_fForwardMoveDis)
				{
					m_tCamInfo.vEye -= vLookDir * m_fSpeed * fTimeDelta;
					m_tCamInfo.vAt -= vLookDir * m_fSpeed * fTimeDelta;

					m_vVerticalVirtualPos -= vLookDir * m_fSpeed * fTimeDelta;
					m_vHorizonVirtualPos -= vLookDir * m_fSpeed * fTimeDelta;
				}
			}
		}
	}
	else if (CKeyManager::GetInstance()->Key_Pressing(KEY_LBUTTON))
	{
		if (m_pPickMove.x != 0 && m_pPickMove.y != 0)
		{
			POINT pMove;
			GetCursorPos(&pMove);
			ClientToScreen(m_Hwnd, &pMove);

			m_fMoveTime += fTimeDelta;

			if (m_fMoveTime >= 0.05f)
			{
				GetCursorPos(&m_pPickMove);
				ClientToScreen(m_Hwnd, &m_pPickMove);

				m_fMoveTime = 0.f;
			}

			if (m_pPickMove.x > pMove.x)
			{
				if (m_fHorizonLimit > m_fHorizonMoveDis)
				{
					m_tCamInfo.vEye += vRight * m_fRLSpeed * fTimeDelta;
					m_tCamInfo.vAt += vRight * m_fRLSpeed * fTimeDelta;

					m_vForwardVirtualPos += vRight * m_fRLSpeed * fTimeDelta;
					m_vVerticalVirtualPos += vRight * m_fRLSpeed * fTimeDelta;
				}
			}

			if (m_pPickMove.x < pMove.x)
			{
				if (-m_fHorizonLimit < m_fHorizonMoveDis)
				{
					m_tCamInfo.vEye -= vRight * m_fRLSpeed * fTimeDelta;
					m_tCamInfo.vAt -= vRight * m_fRLSpeed * fTimeDelta;

					m_vForwardVirtualPos -= vRight * m_fRLSpeed * fTimeDelta;
					m_vVerticalVirtualPos -= vRight * m_fRLSpeed * fTimeDelta;
				}
			}

			if (m_pPickMove.y < pMove.y)
			{
				if (m_fVerticalLimit > m_fVerticalMoveDis)
				{
					_vec3	vCamUp = *D3DXVec3Cross(&m_vTemp, &-vRight, &vLookDir);
					D3DXVec3Normalize(&vCamUp, &vCamUp);

					m_tCamInfo.vEye += vCamUp * m_fUpSpeed * fTimeDelta;
					m_tCamInfo.vAt += vCamUp * m_fUpSpeed * fTimeDelta;

					m_vForwardVirtualPos += vCamUp * m_fUpSpeed * fTimeDelta;
					m_vHorizonVirtualPos += vCamUp * m_fUpSpeed * fTimeDelta;
				}
			}

			if (m_pPickMove.y > pMove.y)
			{
				if (-m_fVerticalLimit < m_fVerticalMoveDis &&
					1.f < m_tCamInfo.vEye.y)
				{
					_vec3	vCamUp = *D3DXVec3Cross(&m_vTemp, &-vRight, &vLookDir);
					D3DXVec3Normalize(&vCamUp, &vCamUp);

					m_tCamInfo.vEye -= vCamUp * m_fUpSpeed * fTimeDelta;
					m_tCamInfo.vAt -= vCamUp * m_fUpSpeed * fTimeDelta;

					m_vForwardVirtualPos -= vCamUp * m_fUpSpeed * fTimeDelta;
					m_vHorizonVirtualPos -= vCamUp * m_fUpSpeed * fTimeDelta;
				}
			}
		}
	}

	Compute_Distance(vLookDir);

}