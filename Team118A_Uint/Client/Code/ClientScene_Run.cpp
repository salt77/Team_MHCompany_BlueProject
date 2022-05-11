#include "stdafx.h"
#include "../Include/stdafx.h"
#include "ClientScene_Run.h"

#include "StageMap.h"
#include "Run_Player.h"
#include "Run_Chesed.h"
#include "Run_Drone.h"
#include "Run_Map.h"
#include "QuatCamera.h"

#include "ClientScene_Manager.h"
#include "ActionCam_Mgr.h"
#include "KeyManager.h"

USING(Engine)

CClientScene_Run::CClientScene_Run(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CClientScene_Run::~CClientScene_Run()
{
}

CClientScene_Run * CClientScene_Run::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClientScene_Run*	pInstance = new CClientScene_Run(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CClientScene_Run::Free()
{
	CScene::Free();
}

HRESULT CClientScene_Run::Ready_Scene()
{
	CScene::Ready_Scene();
	CUI_Manager::GetInstance()->m_fpComVtory_End_Button = bind(&CClientScene_Run::End_Battle, this, E_SCENETAG::Scene_Lobby);
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();

	return S_OK;
}

_int CClientScene_Run::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta);

	Set_SkySphere(L"../../Reference/Resource/Texture/Test/fullmoon.jpg");

	return iExit;
}

_int CClientScene_Run::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	return	iExit;
}

_int CClientScene_Run::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CClientScene_Run::Render_Scene()
{
}

HRESULT CClientScene_Run::Ready_Prototype()
{
	return S_OK;
}

HRESULT CClientScene_Run::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// ºû Á¤º¸
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	// ºû À§Ä¡	
	tLightInfo.Position = _vec3(0.f, 20.f, -4.f);

	// ºû ¹æÇâ¹æÇâ
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);

	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0) < 0) return E_FAIL;

	return S_OK;
}

HRESULT CClientScene_Run::Ready_SceneObject()
{
	CGameObject* pGameObject = nullptr;

	// Dynamic Camera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(CAM_DEFAULT, VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 3000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	// Quat Camera
	pGameObject = CQuatCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(CAM_DEFAULT, VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_Camera_Quat);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	// Run_Map
	pGameObject = CRun_Map::Create(m_pGraphicDev, E_OBJTAG::OBJ_Run_Map);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Environment, pGameObject->Get_ObjTag(), pGameObject);

	// Run_Player
	pGameObject = CRun_Player::Create(m_pGraphicDev, L"Momoi", E_OBJTAG::OBJ_Run_Player);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player, pGameObject);

	//// Run_Chesed
	//pGameObject = CRun_Chesed::Create(m_pGraphicDev, L"Chesed", E_OBJTAG::OBJ_Run_BossMonster);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster, pGameObject);

	return S_OK;
}

void CClientScene_Run::End_Battle(E_SCENETAG eTag)
{
	CGameManager::GetInstance()->Set_NextWorld(nullptr);

	if (CUI_Manager::GetInstance()->m_pComVtory_Animation)
	{
		CUI_Manager::GetInstance()->m_pComVtory_Animation->Set_Dead(TRUE);
	}

	CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
}
