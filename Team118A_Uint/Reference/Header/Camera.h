#pragma once

#ifndef __Camera_H__
#define __Camera_H__

#include "GameObject.h"
#include "KeyManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "ProtoMgr.h"

// Camera 구조체 
typedef struct tagCameraInfo
{
	tagCameraInfo() { }
	tagCameraInfo(_vec3 _vEye, _vec3 _vAt, _vec3 _vUp, _float _fFov, _float _fAspect, _float _fNear, _float _fFar)
		: vEye(_vEye)
		, vAt(_vAt)
		, vUp(_vUp)
		, fFov(_fFov)
		, fAspect(_fAspect)
		, fNear(_fNear)
		, fFar(_fFar)
	{
		D3DXMatrixIdentity(&matView);
		D3DXMatrixIdentity(&matProj);
	}
	_vec3			vEye;
	_vec3			vAt;
	_vec3			vUp;
	_vec3			vRight;
	_vec3			vForward; 

	_float			fFov;
	_float			fAspect;
	_float			fNear;
	_float			fFar;

	_matrix			matView;
	_matrix			matProj;

} T_CAMINFO;

// 

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum E_CAMMODE
	{
		CAM_FREE, 
		CAM_Debug_LOOKAT, CAM_Debug_MOVETOTARGET,
		CAM_MOVEPOS, 
		CAM_ACTION, 
		CAM_END
	};

protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual void		Free() override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;

public:
	_vec3				Get_CamLook()				{ return m_tCamInfo.vAt - m_tCamInfo.vEye;										}
	_vec3				Get_CamLookNormalize()		{ return *D3DXVec3Normalize(&m_vTemp, &(m_tCamInfo.vAt - m_tCamInfo.vEye));		}
	_vec3				Get_CamRight();

	const T_CAMINFO&	Get_CamInfo()				{ return m_tCamInfo;			}
	const _vec3&		Get_CamEye()				{ return m_tCamInfo.vEye;		}
	const _vec3&		Get_CamAt()					{ return m_tCamInfo.vAt;		}
	const _vec3&		Get_CamUp()					{ return m_tCamInfo.vUp;		}
	const _float&		Get_CamFov()				{ return m_tCamInfo.fFov;		}
	const _float&		Get_CamAspect()				{ return m_tCamInfo.fAspect;	}
	const _float&		Get_CamNear()				{ return m_tCamInfo.fNear;		}
	const _float&		Get_CamFar()				{ return m_tCamInfo.fFar;		}
	const _float&		Get_CamSpeed()				{ return m_fSpeed;				}
	const _matrix&		Get_CamView()				{ return m_tCamInfo.matView;	}
	const _matrix&		Get_CamProj()				{ return m_tCamInfo.matProj;	}
	const E_CAMMODE&	Get_CamMode()				{ return m_eCamMode;			}

public:
	void				Set_CamEye(_vec3& vEye) { m_tCamInfo.vEye = vEye; }
	void				Set_CamAt(_vec3& vAt) { m_tCamInfo.vAt = vAt; }
	void				Set_Update_CamAt(const _vec3& vAt); 
	void				Set_CamFov(_float& fFov) { m_tCamInfo.fFov = fFov; }
	void				Set_MovePos_Eye(const _vec3& vTargetPos, const _float& fSpeed);
	void				Set_CamOriginSettings();
	void				Set_Mode(E_CAMMODE eMode) { m_eCamMode = eMode; }
	void				Set_FixMode(const _bool& bValue) { m_bCameraToolMode = bValue; m_bFix = bValue; }
	void				Set_CamSpeed(const _float& fSpeed) { m_fSpeed = fSpeed; }
	void				Set_ShakeCamera(const _float& fPower, const _float& fEndTime);

	//	대상을 바라봄 (SpeedInterpol로 속도 조절)
	void				Set_Debug_LookAt(const _vec3& vTargetPos, const _float& fSpeedInterpol = 1.f);
	void				Set_Debug_LookAt(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex = 0, const _float& fSpeedInterpol = 1.f);
	//	TargetPos쪽으로 향하는 함수 
	void				Set_Debug_MoveToTarget(const _vec3& vTargetPos);
	void				Set_Debug_MoveToTarget(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex = 0);

	// 메인 캠 (매니지먼트에서 가져옴)
public:
	//////////////////////////////////////////////////////////////////////////						Camera
	static const E_MAINCAM&	Get_MainCamType();

	//  (카메라들의 부모 클래스)Camera 클래스의 Set_Look() 함수와 동일 
	//	Engine->Dynamic Camera가 매개변수로 받는 대상을 바라봄. (SpeedInterpol은 속도 조절)
	static void				DebugCam_LookAt(const _vec3& vTargetPos, const _float& fSpeedInterpol = 1.f);
	static void				DebugCam_LookAt(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex = 0, const _float& fSpeedInterpol = 1.f);
	//	Camera 클래스의 Set_MoveToTarget() 함수와 동일 
	//	Engine->Dynamic Camera가 대상으로 향한다.  
	static void				DebugCam_MoveToTarget(const _vec3& vTargetPos);
	static void				DebugCam_MoveToTarget(E_LAYERTAG eLayerTag, E_OBJTAG eObjTag, _uint iIndex = 0);

	//////////////////////////////////////////////////////////////////////////						Main Camera
	static void				Swap_MainCamera(E_MAINCAM eMainCamType);
	// 메인 캠 (매니지먼트에서 가져옴)

protected:
	void				Camera_Function(const _float& fTimeDelta);
	void				Mode_Change();
	void				MovePos_Eye(const _float& fTimeDelta);
	virtual void		Debug_LookAt(const _float& fTimeDelta);
	virtual	void		Debug_MoveToTarget(const _float& fTimeDelta);
	void				Shake_Camera(const _float& fTimeDelta); 

protected:
	_bool		m_bCameraToolMode = FALSE; 
	_bool		m_bFix = TRUE;
	_bool		m_bClick = FALSE;

	_float		m_fAngle = 0.f;
	_float		m_fSpeed = 20.f;
	_float		m_fMovePosSpeed = 0.f;
	_float		m_fLookAtSpeed = 0.f;
	_float		m_fOriginAtLength = 0.f;

	// Camera Shake 
	_float		m_fShakeTime = 0.f; 
	_float		m_fFXProgress = 0.f;
	_float		m_fShakeWaveX = 0.f;
	_float		m_fFYProgress = 0.f;
	_float		m_fShakeWaveY = 0.f;
	_float		m_fShakePower = 0.025f;
	_float		m_fWaveFxProgressive = 1.5f;
	_float		m_fWaveFyProgressive = 0.9f; 

	_vec3		m_vShakeInterpol = VEC_ZERO;
	_vec3		m_vPreShakeInterpol = VEC_ZERO;
	_vec3		m_vActionCamInterpol = VEC_ZERO; 

	_vec3		m_vTemp = VEC_ZERO;
	_vec3		m_vTargetPos = VEC_ZERO;
	_vec3		m_vMoveTargetDir = VEC_ZERO;

	E_SCENETAG	m_eSceneTag = E_SCENETAG::Scene_END; 
	E_SCENETAG	m_ePreSceneTag = E_SCENETAG::Scene_END; 

	T_CAMINFO	m_tCamInfo;

	E_CAMMODE	m_eCamMode = E_CAMMODE::CAM_FREE;
	E_CAMMODE	m_ePreCamMode = E_CAMMODE::CAM_FREE;

	E_MAINCAM	m_eMyCamType = E_MAINCAM::MAINCAM_END; 

public:
	static CCamera*			m_pMainCam;
	static E_MAINCAM		m_eMainCamType;
};

END

#endif // __Camera_H__