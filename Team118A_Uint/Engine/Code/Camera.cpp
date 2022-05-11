#include "Camera.h"

USING(Engine)

E_MAINCAM CCamera::m_eMainCamType = E_MAINCAM::MAINCAM_END;
CCamera*	CCamera::m_pMainCam = nullptr;

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
	ZeroMemory(&m_tCamInfo, sizeof(T_CAMINFO));
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_tCamInfo(rhs.m_tCamInfo)
{
}

CCamera::~CCamera()
{
}

void CCamera::Free()
{
	CGameObject::Free();
}

// 씬 진입 시 현재 카메라 타입 기록 
HRESULT CCamera::Ready_Object()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	switch (m_eObjTag)
	{
	case Engine::E_OBJTAG::OBJ_DynamicCamera:
		m_eMyCamType = E_MAINCAM::MAINCAM_DYNAMIC; 
		break;

	case Engine::E_OBJTAG::OBJ_Camera_Cafe:
		m_eMyCamType = E_MAINCAM::MAINCAM_CAFE; 
		break;

	case Engine::E_OBJTAG::OBJ_Camera_TPS:
		m_eMyCamType = E_MAINCAM::MAINCAM_TPS; 
		break;

	case Engine::E_OBJTAG::OBJ_Camera_Quat:
		m_eMyCamType = E_MAINCAM::MAINCAM_QUAT; 
		break; 

	default:
		MSG_BOX("Camera->Ready_Object() : 추가되지 않은 카메라 타입"); 
		break;
	}

	
	return S_OK;
}

// 기존의 Eye와 At사이의 거리 기록 
_int CCamera::Start_Object(const _float & fTimeDelta)
{
	FAILED_CHECK(CGameObject::Start_Object(fTimeDelta));

	m_fOriginAtLength = D3DXVec3Length(&(m_tCamInfo.vAt - m_tCamInfo.vEye));

	return RETURN_OK;
}

_int CCamera::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	if (0.01f > D3DXToDegree(acos(D3DXVec3Dot(&Get_CamLookNormalize(), &-VEC_UP))))
	{
		++m_tCamInfo.vAt.x;
	}

	Camera_Function(fTimeDelta); 
	Shake_Camera(fTimeDelta); 

	D3DXMatrixLookAtLH(&m_tCamInfo.matView, &m_tCamInfo.vEye, &m_tCamInfo.vAt, &m_tCamInfo.vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_tCamInfo.matView);

	D3DXMatrixPerspectiveFovLH(&m_tCamInfo.matProj, m_tCamInfo.fFov, m_tCamInfo.fAspect, m_tCamInfo.fNear, m_tCamInfo.fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_tCamInfo.matProj);

	return RETURN_OK;
}

_int CCamera::LateUpdate_Object(const _float & fTimeDelta)
{
	CGameObject::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CCamera::Render_Object(_int iPass)
{
}

_vec3 CCamera::Get_CamRight()
{
	_vec3	vRight;
	D3DXVec3Cross(&vRight, &VEC_UP, &Get_CamLookNormalize());

	return vRight;
}

void CCamera::Set_Update_CamAt(const _vec3 & vAt)
{
	m_vActionCamInterpol = vAt; 
}

// 타겟 위치로 이동 
void CCamera::Set_MovePos_Eye(const _vec3 & vTargetPos, const _float & fSpeed)
{
	m_eCamMode = CAM_MOVEPOS;
	m_vTargetPos = vTargetPos;
	m_fMovePosSpeed = fSpeed;
	m_vMoveTargetDir = *D3DXVec3Normalize(&m_vTemp, &(vTargetPos - m_tCamInfo.vEye));
}

// 생성 당시의 상태 값으로 되돌림(코드 계속 추가될 수 있음)
void CCamera::Set_CamOriginSettings()
{
	m_fShakeWaveX = 0.f;
	m_fShakeWaveY = 0.f;
	m_fFXProgress = 0.f;
	m_fFYProgress = 0.f;

	m_fMovePosSpeed = 0.f;

	m_vTargetPos = VEC_ZERO;
	m_vMoveTargetDir = VEC_ZERO;

	_vec3	vDir = *D3DXVec3Normalize(&m_vTemp, &(m_tCamInfo.vAt - m_tCamInfo.vEye));
	m_tCamInfo.vAt = m_tCamInfo.vEye + vDir * m_fOriginAtLength;
}

void CCamera::Set_ShakeCamera(const _float & fPower, const _float & fEndTime)
{
	m_fShakeWaveX = 0.f; 
	m_fShakeWaveY = 0.f; 
	m_fFXProgress = 0.f; 
	m_fFYProgress = 0.f; 

	m_fShakePower = fPower; 
	m_fShakeTime = fEndTime; 
}

// 타겟 위치쪽으로 At 이동(SpeedInterpol에 입력한 값의 배수의 속도로 이동)
void CCamera::Set_Debug_LookAt(const _vec3 & vTargetPos, const _float& fSpeedInterpol)
{
	m_eCamMode = CAM_Debug_LOOKAT;
	m_vTargetPos = vTargetPos;
	m_fLookAtSpeed = fSpeedInterpol;

	CMyMath::Clamp_float(m_fLookAtSpeed, 0.1f, 10.f);
}

// 타겟 오브젝트쪽으로 At 이동(SpeedInterpol에 입력한 값의 배수의 속도로 이동)
void CCamera::Set_Debug_LookAt(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex, const _float & fSpeedInterpol)
{
	m_eCamMode = CAM_Debug_LOOKAT;
	m_vTargetPos = static_cast<CTransform*>(CScene::Get_MainScene()->Get_Component(eLayerTag, eObjTag, iIndex, ComTag_Transform))->Get_Info(E_INFO::INFO_POS);
	m_fLookAtSpeed = fSpeedInterpol;
}

// 타겟 위치로 이동 
void CCamera::Set_Debug_MoveToTarget(const _vec3 & vTargetPos)
{
	m_eCamMode = CAM_Debug_MOVETOTARGET;
	m_vTargetPos = vTargetPos;
	m_vMoveTargetDir = m_tCamInfo.vEye - m_tCamInfo.vAt;
}

// 타겟 오브젝트쪽으로 이동 
void CCamera::Set_Debug_MoveToTarget(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex)
{
	m_eCamMode = CAM_Debug_MOVETOTARGET;
	m_vTargetPos = static_cast<CTransform*>(CScene::Get_MainScene()->Get_Component(eLayerTag, eObjTag, iIndex, ComTag_Transform))->Get_Info(E_INFO::INFO_POS);
	m_vMoveTargetDir = m_tCamInfo.vEye - m_tCamInfo.vAt;
}

// 현재 카메라 모드에 따른 실행 함수 분기 
void CCamera::Camera_Function(const _float & fTimeDelta)
{
	switch (m_eCamMode)
	{
	case CAM_FREE:
		break;

	case CAM_Debug_LOOKAT:
		Debug_LookAt(fTimeDelta);
		break;

	case CAM_Debug_MOVETOTARGET:
		Debug_MoveToTarget(fTimeDelta);
		break;

	case CAM_MOVEPOS:
		MovePos_Eye(fTimeDelta);
		break;

	case CAM_ACTION:
		break; 

	default:
		assert(0);
		break;
	}

	Mode_Change();
}

// 카메라 모드가 변경되는지 체크
void CCamera::Mode_Change()
{
	if (m_ePreCamMode != m_eCamMode)
	{
		_vec3	vDir = VEC_ZERO;

		switch (m_eCamMode)
		{
		case CAM_FREE:
			m_fMovePosSpeed = 0.f;

			m_vTargetPos = VEC_ZERO;
			m_vMoveTargetDir = VEC_ZERO;

			vDir = *D3DXVec3Normalize(&m_vTemp, &(m_tCamInfo.vAt - m_tCamInfo.vEye));
			m_tCamInfo.vAt = m_tCamInfo.vEye + vDir * m_fOriginAtLength;
			break;

		case CAM_Debug_LOOKAT:
			break; 

		case CAM_Debug_MOVETOTARGET:
			break; 

		case CAM_MOVEPOS:
			break; 

		case CAM_ACTION:
			break;

		default:
			assert(0);
			break;
		}

		m_ePreCamMode = m_eCamMode;
	}
}

// 설정된 타겟으로 이동 
void CCamera::MovePos_Eye(const _float & fTimeDelta)
{
	_float	fLength = D3DXVec3Length(&(m_vTargetPos - m_tCamInfo.vEye));

	m_tCamInfo.vEye += m_vMoveTargetDir * m_fMovePosSpeed * fTimeDelta;

	if (0.1f >= fLength)
	{
		m_eCamMode = CAM_FREE; 
	}
}

// 타겟 위치쪽으로 At만 이동(SpeedInterpol에 입력한 값의 배수의 속도로 이동)
void CCamera::Debug_LookAt(const _float & fTimeDelta)
{
	_float	fAtLength = D3DXVec3Length(&(m_tCamInfo.vAt - m_tCamInfo.vEye));

	_vec3	vToTargetDir = *D3DXVec3Normalize(&m_vTemp, &(m_vTargetPos - m_tCamInfo.vEye));
	_vec3	vNewTargetPos = m_tCamInfo.vEye + vToTargetDir * fAtLength;
	_vec3	vDir = *D3DXVec3Normalize(&m_vTemp, &(vNewTargetPos - m_tCamInfo.vAt));

	_float	fLength = D3DXVec3Length(&(m_tCamInfo.vAt - vNewTargetPos));

	m_tCamInfo.vAt += vDir * fLength * 10.f * m_fLookAtSpeed * fTimeDelta;

	if (0.1f >= fLength)
	{
		m_eCamMode = CAM_FREE;
	}
}

// 타겟 위치쪽으로 이동(SpeedInterpol에 입력한 값의 배수의 속도로 이동)
void CCamera::Debug_MoveToTarget(const _float & fTimeDelta)
{
	_vec3	vAtDir = *D3DXVec3Normalize(&m_vTemp, &(m_vTargetPos - m_tCamInfo.vAt));
	_vec3	vNewTargetPos = m_vTargetPos + m_vMoveTargetDir;
	_vec3	vEyeDir = *D3DXVec3Normalize(&m_vTemp, &(vNewTargetPos - m_tCamInfo.vEye));
	_float	fLength = D3DXVec3Length(&(vNewTargetPos - m_tCamInfo.vEye));

	m_tCamInfo.vAt += vAtDir * fLength * 5.f * fTimeDelta;
	m_tCamInfo.vEye += vEyeDir * fLength * 5.f * fTimeDelta;

	if (0.1f >= fLength)
	{
		m_eCamMode = CAM_FREE;
	}
}

void CCamera::Shake_Camera(const _float & fTimeDelta)
{
	m_fShakeTime -= fTimeDelta; 

	if (0.f < m_fShakeTime)
	{
		m_fFXProgress += m_fWaveFxProgressive * fTimeDelta;
		m_fShakeWaveX += m_fFXProgress;
		m_fFYProgress += m_fWaveFyProgressive * fTimeDelta;
		m_fShakeWaveY += m_fFYProgress;
		m_vShakeInterpol.x = sin(m_fShakeWaveX * 10.f) * powf(Half, m_fShakeWaveX) * m_fShakePower;
		m_vShakeInterpol.y = sin(m_fShakeWaveY * 5.f) * powf(Half, m_fShakeWaveY) * m_fShakePower;
		m_vShakeInterpol.z = sin(m_fShakeWaveX * 10.f) * powf(Half, m_fShakeWaveX) * m_fShakePower;

		m_vActionCamInterpol = VEC_ZERO;
	}
	else
	{
		m_fShakePower = 0.025f;
		m_fShakeWaveX = 0.f;
		m_fShakeWaveY = 0.f;
		m_fFXProgress = 0.f;
		m_fFYProgress = 0.f;
		m_vShakeInterpol = VEC_ZERO;
	}
}

const E_MAINCAM & CCamera::Get_MainCamType()
{
	return m_eMainCamType;
}

void CCamera::DebugCam_LookAt(const _vec3 & vTargetPos, const _float & fSpeedInterpol)
{
	if (nullptr == m_pMainCam)
		return;

	switch (m_eMainCamType)
	{
	case Engine::E_MAINCAM::MAINCAM_DYNAMIC:
		m_pMainCam->Set_Debug_LookAt(vTargetPos, fSpeedInterpol);
		break;
	case Engine::E_MAINCAM::MAINCAM_TPS:
		break;
	case Engine::E_MAINCAM::MAINCAM_QUAT:
		break;
	case Engine::E_MAINCAM::MAINCAM_END:
		break;
	default:
		break;
	}
}

void CCamera::DebugCam_LookAt(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex, const _float & fSpeedInterpol)
{
	if (nullptr == m_pMainCam)
		return;

	switch (m_eMainCamType)
	{
	case Engine::E_MAINCAM::MAINCAM_DYNAMIC:
		m_pMainCam->Set_Debug_LookAt(eLayerTag, eObjTag, iIndex, fSpeedInterpol);
		break;
	case Engine::E_MAINCAM::MAINCAM_TPS:
		break;
	case Engine::E_MAINCAM::MAINCAM_QUAT:
		break;
	case Engine::E_MAINCAM::MAINCAM_END:
		break;
	default:
		break;
	}
}

void CCamera::DebugCam_MoveToTarget(const _vec3 & vTargetPos)
{
	if (nullptr == m_pMainCam)
		return;

	switch (m_eMainCamType)
	{
	case Engine::E_MAINCAM::MAINCAM_DYNAMIC:
		m_pMainCam->Set_Debug_MoveToTarget(vTargetPos);
		break;
	case Engine::E_MAINCAM::MAINCAM_TPS:
		break;
	case Engine::E_MAINCAM::MAINCAM_QUAT:
		break;
	case Engine::E_MAINCAM::MAINCAM_END:
		break;
	default:
		break;
	}
}

void CCamera::DebugCam_MoveToTarget(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex)
{
	if (nullptr == m_pMainCam)
		return;

	switch (m_eMainCamType)
	{
	case Engine::E_MAINCAM::MAINCAM_DYNAMIC:
		m_pMainCam->Set_Debug_MoveToTarget(eLayerTag, eObjTag, iIndex);
		break;
	case Engine::E_MAINCAM::MAINCAM_TPS:
		break;
	case Engine::E_MAINCAM::MAINCAM_QUAT:
		break;
	case Engine::E_MAINCAM::MAINCAM_END:
		break;
	default:
		break;
	}
}

void CCamera::Swap_MainCamera(E_MAINCAM eMainCamType)
{
	m_eMainCamType = eMainCamType;

	switch (eMainCamType)
	{
	case Engine::E_MAINCAM::MAINCAM_DYNAMIC:
		if (static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera)))
		{
			m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera));
		}
		break;

	case Engine::E_MAINCAM::MAINCAM_TPS:
		if (static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_TPS)))
		{
			m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_TPS));
		}
		break;

	case Engine::E_MAINCAM::MAINCAM_CAFE:
		if (static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Cafe)))
		{
			m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Cafe));
		}
		break;

	case Engine::E_MAINCAM::MAINCAM_QUAT:
		if (static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Quat)))
		{
			m_pMainCam = static_cast<CCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Quat));
		}
		break;

	default:
		assert(0);
		break;
	}

	assert(m_pMainCam);
}