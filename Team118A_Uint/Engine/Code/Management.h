#pragma once
#ifndef __Management_H__
#define __Management_H__

#include "Scene.h"
#include "Base.h"
#include "GraphicDev.h"

// Obj ///////////////////
#include "DynamicCamera.h"
#include "QuatCamera.h"
#include "Camera_TPS.h"
#include "Unit.h"
#include "Monster.h"
#include "Character.h"
#include "Obstacle.h"
#include "Obstacle_Combat.h"
#include "CombatTile.h"
#include "Mascot.h"
#include "TileField.h"
#include "BackBoard.h"
#include "WorldUI.h"
#include "StaticObject.h"
#include "SkyDome.h"

// Mgr ///////////////////
#include "GameManager.h"
#include "KeyManager.h"
#include "FogManager.h"
#include "CollisionManager.h"
#include "DataManager.h"
#include "TimeScale_Manager.h"
#include "PrefabManager.h"
#include "Sound_Manager.h"

#include "FontMgr.h"
#include "LightMgr.h"
#include "ProtoMgr.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "ActionCam_Mgr.h"
#include "RenderTargetMgr.h"
#include "XL_ParsingMgr.h"
#include "CharacterMgr.h"
#include "Dialog_Manager.h"
#include "UI_Manager.h"
#include "Skill_Manager.h"
#include "Warning_Mgr.h"
#include "LoadingManager.h"

#include "Line.h"
#include "Wall512.h"

// Com ///////////////////
#include "RectBuffer.h"
#include "SphereTex2.h"
#include "CrossRectBuffer.h"
#include "RectTest.h"
#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Renderer.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "Light.h"
#include "SMesh.h"
#include "DMesh.h"
#include "EMesh.h"

// Obj ////////////////////
#include "UI_Object.h"
#include "UI_Slider_Vert.h"
#include "UI_Scroll_View.h"
#include "UI_Damage.h"

#include "ParticleObject.h"
#include "EffectObject.h"
#include "SkillObject.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	virtual void Free() override;

public:
	HRESULT					Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev);
	_int					Update_Scene(const _float& fTimeDelta);
	_int					LateUpdate_Scene(const _float& fTimeDelta);
	void					Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

	virtual void			Debugging_Console();

	HRESULT					Set_Scene(CScene* pScene);
	CScene*					Get_Scene();

public:
	CComponent*				Get_Component(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx, const wstring& pComponentTag);
	CGameObject*			Get_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx = 0);
	vector<CGameObject*>*	Get_vecGameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag);
	map<E_OBJTAG, vector<CGameObject*>>*	Get_LayerMapObject(E_LAYERTAG eLayerTag);

	void					Add_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject* pGameObject);

public:
	void					Set_Dead_AllObject(E_LAYERTAG eLayerTag);
	void					Set_Dead_CafeTile(E_LAYERTAG eLayerTag);
	void					Set_Dead_CafeWall_L(E_LAYERTAG eLayerTag);
	void					Set_Dead_CafeWall_R(E_LAYERTAG eLayerTag);

public:
	void					Ready_Managers(HWND* pHwnd);
	void					Release_Managers();
	HRESULT					Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);

	//////////////////////////////////////////////////////////////////////////						GraphicMgr
	LPDIRECT3DDEVICE9		GetDevice();
	HRESULT					Ready_GraphicDev(HWND hWnd, E_WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
	void					Render_Begin(D3DXCOLOR Color);
	void					Render_End();
	void					Render_End(HWND hWnd);

	//////////////////////////////////////////////////////////////////////////						TimeMgr
	_float					Get_TimeDelta(const wstring& pTimerTag);
	void					Set_TimeDelta(const wstring& pTimerTag);
	HRESULT					Ready_Timer(const wstring& pTimerTag);

	//////////////////////////////////////////////////////////////////////////						FrameMgr
	_bool					IsPermit_Call(const wstring& pFrameTag, const _float& fTimeDelta);
	HRESULT					Ready_Frame(const wstring& pFrameTag, const _float& fCallLimit);
	_float					Get_AccTimeDelta(const wstring& pFrameTag);

	//////////////////////////////////////////////////////////////////////////						TimeScaleMgr
	float					Update_TimeScale(const _float& fTimeDelta);
	void					Reset_TimeScale();
	void					Set_TimeScale(float fTimeScale);
	void					Set_TimeScale_Duration(float fTimeScale, float fTimeDuration);
	void					Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch = FALSE);

	float					Get_TimeScale();
	float					Get_TimeReal();
	float					Get_TimeDelta();

	//////////////////////////////////////////////////////////////////////////						FontMgr
	HRESULT					Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& pFontTag,
		const T_FONT& pStFont);

	void					Render_Font(const wstring& pFontTag,
		const wstring& pString,
		const _vec2* pPos,
		D3DXCOLOR Color);
	//////////////////////////////////////////////////////////////////////////						KeyMgr
	void					UpdateKeyManager(HWND hWnd);
	bool					Key_Up(long long llKey);
	bool					Key_Down(long long llKey);
	bool					Key_Pressing(long long llKey);
	_vec2					Get_MouseMovementFigure();

	//////////////////////////////////////////////////////////////////////////						ProtoMgr
	HRESULT					Ready_Prototype(const wstring& pProtoTag, CComponent* pInstance);
	CComponent*				Clone_Prototype(const wstring& pProtoTag);
	HRESULT					Get_Texture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFullPath, LPDIRECT3DTEXTURE9* ppOut);
	HRESULT					Delete_Prototype(const wstring& pProtoTag);

	//////////////////////////////////////////////////////////////////////////					    PrefabManager
	_bool					ReadPrefab(wstring wstrFileName, void* pOut, size_t sizeSize, DWORD* pDwordByte);
	void					FileReset();
	void					PrefabReload();

	///////////////////////////////////////////////////////////////////////							Renderer
	HRESULT					Ready_Renderer(LPDIRECT3DDEVICE9&	pGraphicDev);
	CRenderer*				Get_Renderer();
	void					Add_RenderGroup(E_RENDER_ID eRenderID, CGameObject* pGameObject);
	void					Add_InstanceGroup(E_INSTANCE_RENDER_ID eInstanceRenderID, CGameObject* pGameObject);
	void					Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Clear_RenderGroup();
	void					Clear_Instance_RenderGroup();
	void					Set_Render_Defferd();
	void					Set_Render_Defferd(_bool bState);
	_bool					Get_Debug();
	void					Set_Debug();

	void					Set_RendererOption(T_RenderOption tRenderOption);
	void					Reset_RendererOption();
	T_RenderOption			Get_RendererOption();

	///////////////////////////////////////////////////////////////////////							LightMgr
	HRESULT					Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
		const D3DLIGHT9* pLightInfo,
		const _uint& iIndex);
	void					Render_Light(LPD3DXEFFECT& pEffect);
	D3DLIGHT9*				Get_Light(const _uint& iIndex = 0);

	_bool					IsLightExist();

	void					Set_LightTarget(_int iIndex);

	_vec3					Get_LightPos();
	void					Set_LightPos(_vec3 vPos);

	_vec3					Get_LightDir();
	void					Set_LightDir(_vec3 vDir);

	D3DLIGHTTYPE			Get_LightType();
	void					Set_LightType(D3DLIGHTTYPE eLightType);

	D3DXCOLOR				Get_LightDiffuse();
	void					Set_LightDiffuse(D3DXCOLOR Diffuse);

	D3DXCOLOR				Get_LightSpecular();
	void					Set_LightSpecular(D3DXCOLOR Specular);

	D3DXCOLOR				Get_LightAmbient();
	void					Set_LightAmbient(D3DXCOLOR Ambient);

	D3DXCOLOR				Get_LightBackBufferColor();
	void					Set_LightBackBufferColor(D3DXCOLOR BackBufferColor);

	///////////////////////////////////////////////////////////////////////							FogMgr
	_float					Get_FogStart();
	void					Set_FogStart(_float fFogStart);

	_float					Get_FogEnd();
	void					Set_FogEnd(_float fFogEnd);

	_vec4					Get_FogColor();
	void					Set_FogColor(_vec4 FogColor);

	void					Reset_Fog();

	//////////////////////////////////////////////////////////////////////////						RenderTarget

	HRESULT					Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
											   const wstring& pTargetTag,
											   const _uint& iWidth,
											   const _uint& iHeight,
											   D3DFORMAT Format,
											   D3DXCOLOR Color);
	HRESULT					Ready_MRT(const wstring& pMRTTag, const wstring& pTargetTag);
	HRESULT					Begin_MRT(const wstring& pMRTTag);
	HRESULT					End_MRT(const wstring& pMRTTag);

	HRESULT					Ready_DebugBuffer(const wstring& pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void					Render_DebugBuffer(const wstring& pMRTTag);
	void					Set_RenderTargetTexture(LPD3DXEFFECT& pEffect, const wstring& pTargetTag, const char* pConstantTable);

	//////////////////////////////////////////////////////////////////////							ColliderManager

	//충돌체 컴포넌트 끼리 충돌시키는 함수
	_bool	Collision_CircleToCircle(CSphereCollider* pDestSphereCol, E_LAYERTAG eLayerTag);

	//충돌체 컴포넌트 없이 충돌시키는 함수
	_bool   Collision_FanToCircle(CGameObject* pDestObject, _vec3 vStartPos, _vec3 vDir, _float fFanDegree, _float fFanDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool	Collision_StraightToCircle(CGameObject* pDestObject, _vec3 vStartPos, _vec3 vDir, _float fWidth, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool   Collision_SpotToCircle(CGameObject* pDestObject, _vec3 vStartPos, _float fRadius, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	
	//////////////////////////////////////////////////////////////////////////						XL_Parsing Manager

	//////////////////////////////////////////////////////////////////////////						Dialog Manager
	void				Start_DialogEvent(const wstring& wstrEventName);
	void				Render_DialogFont();

	//////////////////////////////////////////////////////////////////////////						Skill Manager
	CSkillObject*		SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale = VEC_ONE);
	

	//////////////////////////////////////////////////////////////////////////						ActionCam_Mgr
	void				Start_ActionCamEvent(const wstring& wstrFileName, const wstring& wstrAction, _vec3 vPos, _float fYAngle, CActionCam_Mgr::E_ActionCam_Mode eMode = CActionCam_Mgr::ActionCam_Subject);

	//////////////////////////////////////////////////////////////////////////						Wall 512
	_uint				GetValue();
	_uint				GetValue(unsigned int nMinValue, unsigned int nMaxValue);
	_uint				GetValue(unsigned int nMaxValue);
	_bool				GetRandBoolean();
	_float				GetFloatValue();
	_float				GetFloatValue(float fMinValue, float fMaxValue);
	_float				GetFloatValue(float fMaxValue);

private:
	CScene*			m_pScene = nullptr;
};

END

#endif // !__Management_H__