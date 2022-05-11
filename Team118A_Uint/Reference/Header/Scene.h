#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Engine_Define.h"
#include "Base.h"
#include "Layer.h"
#include "StageMap.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class CGameObject;
class CUISkillObject;
class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	virtual void Free() override;

public:
	void									Add_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject* pGameObject);
	void									Add_GameObjectNextFrame_ForLoading(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject* pGameObject);
	void									Add_Layer(E_LAYERTAG eLayerTag, CLayer* pLayer);

public:
	CLayer*									Get_Layer(E_LAYERTAG eLayerTag);
	CComponent*								Get_Component(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx, const wstring& pComponentTag);
	CGameObject*							Get_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx = 0);
	vector<CGameObject*>*					Get_vecGameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag);
	map<E_OBJTAG, vector<CGameObject*>>*	Get_LayerMapObject(E_LAYERTAG eLayerTag);
	LPDIRECT3DDEVICE9						Get_GraphicDev() { return m_pGraphicDev; } 
	const map<E_LAYERTAG, CLayer*>&			Get_mapAllLayer() { return m_mapLayer; }

public:
	void									Set_Dead_AllObject(E_LAYERTAG eLayerTag);
	void									Set_Dead_CafeTile(E_LAYERTAG eLayerTag);
	void									Set_Dead_CafeWall_L(E_LAYERTAG eLayerTag);
	void									Set_Dead_CafeWall_R(E_LAYERTAG eLayerTag);

public:
	_bool									Get_Binah_Cam() { return m_bBinha_Cam; }
	void									Set_Binah_Cam(_bool _BinahCam) { m_bBinha_Cam = _BinahCam; }

public:
	virtual HRESULT							Ready_Scene();
	virtual _int							Start_Scene(const _float& fTimeDelta);
	virtual _int							Update_Scene(const _float& fTimeDelta);
	virtual _int							LateUpdate_Scene(const _float& fTimeDelta);
	virtual void							Render_Scene() PURE;

public:
	virtual void							Set_FogEvent();
	virtual void							Update_FogEvent(const _float& fTimeDelta);

public:
	virtual void							Active_AutoCam();
	void									OnOff_AutoCam(_bool bState)					{ m_bAutoCam = bState; }
	virtual void							Update_Fit_Camera(const _float& fTimeDelta);

	virtual void							Set_Fit_Camera();
	void									Set_Fit_Camera_Option(_float _fBaseY_At, _float _fBaseY_Eye, _float _fFarX, _float _fFarY, _float _fFarZ);
	void									Set_Fit_Camera_Interval(_float _fMinEye_X, _float _fMaxEye_X, _float _fMinEye_Y, _float _fMaxEye_Y, _float _fMinEye_Z, _float _fMaxEye_Z);

	virtual void							Set_AutoCam_Ratio(const _float& fCam_Ratio) { m_fAutoCam_Ratio = fCam_Ratio; }

	void									Set_SkySphere(wstring wstrSkyTexFullPath);

	_float									Get_BaseY_At()		{ return m_fBaseY_At;	}
	_float									Get_BaseY_Eye()		{ return m_fBaseY_Eye;	}
	_float									Get_FarX()			{ return m_fFarX;		}
	_float									Get_FarY()			{ return m_fFarY;		}
	_float									Get_FarZ()			{ return m_fFarZ;		}
	_float									Get_MinEye_X()		{ return m_fMinEye_X;	}
	_float									Get_MaxEye_X()		{ return m_fMaxEye_X;	}
	_float									Get_MinEye_Y()		{ return m_fMinEye_Y;	}
	_float									Get_MaxEye_Y()		{ return m_fMaxEye_Y;	}
	_float									Get_MinEye_Z()		{ return m_fMinEye_Z;	}
	_float									Get_MaxEye_Z()		{ return m_fMaxEye_Z;	}
	_vec3									Get_ShakeCam()		{ return m_vCamShake;	}

public:
	virtual void							Active_ShakeCam(const _float& fShakeTime, const _float& fShakePower);
	virtual void							Update_ShakeCam(const _float& fTimeDelta);

public:
	_bool									IsExist_Obj(const E_LAYERTAG& eLayerTag, const E_OBJTAG& eObjTag); 

protected:
	virtual HRESULT							Ready_SceneObject();


public:
#ifdef _DEBUG
	virtual void							Debugging_Console();
#endif

public:
	static CScene*							Get_MainScene()								{ return m_pMainScene;			}
	static _float							Get_PlayTime()								{ return m_fPlayTime;			}
	static void								Add_PlayTime(const _float& fTimeDelta)		{ m_fPlayTime += fTimeDelta;	}

protected:
	void									Set_ClickEffect();
protected:
	CUISkillObject*							m_pMouseDragObj = nullptr;
	_bool									m_bKeyDown = FALSE;

	static	_float							m_fPlayTime;
	static CScene*							m_pMainScene;
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	map<E_LAYERTAG, CLayer*>				m_mapLayer;

	_bool									m_bStart = TRUE;
	vector<pair<E_LAYERTAG, pair<E_OBJTAG, CGameObject*>>> m_vecNextFrame;
	
protected:
	_float									m_fFogNowTime		= 0.f;
	_float									m_fFogTotalTime		= 20.f;
	D3DXCOLOR								m_tFogColor[2];
	_float									m_fFogStart[2];
	_float									m_fFogEnd[2];

	T_CustomFog								m_tCustomFog[2];

protected:
	_bool									m_bAutoCam			= FALSE;
	_vec3									m_vLastCam_Eye		= VEC_ZERO;
	_vec3									m_vLastCam_At		= VEC_ZERO;
	_float									m_fCamSpeed			= 1.f;
	_float									m_fMax_At_Z_Diff	= 5.f;

	_float									m_fMinEye_X			= -6.f;
	_float									m_fMaxEye_X			= -3.f;
	_float									m_fMinEye_Y			= 5.f;
	_float									m_fMaxEye_Y			= 8.f;
	_float									m_fMinEye_Z			= 4.f;
	_float									m_fMaxEye_Z			= 2.f;

	_float									m_fAutoCam_Ratio	= 1.f;

protected:
	_float									m_fBaseY_At			= 1.f;
	_float									m_fBaseY_Eye		= 5.f;
	_float									m_fFarX				= -0.8f;
	_float									m_fFarY				= -0.05f;
	_float									m_fFarZ				= -0.4f;

protected:
	_float									m_fCamShake_Power	= 1.f;
	_float									m_fCamShake_Time	= 0.f;
	_vec3									m_vCamShake			= VEC_ZERO;
	_vec3									m_vLastShake		= VEC_ZERO;

protected:
	_bool									m_bBinha_Cam = false;

	CGameObject*							m_pSkySphere = nullptr;
};

END

#endif // !__SCENE_H__