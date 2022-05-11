#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_RunTest.h"

#include "StageMap.h"
#include "Run_Player.h"
#include "Run_Chesed.h"
#include "Run_Drone.h"
#include "Run_Map.h"
#include "QuatCamera.h"

#include "Scene_Manager.h"
#include "ActionCam_Mgr.h"
#include "KeyManager.h"

USING(Engine)

CScene_RunTest::CScene_RunTest(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_RunTest::~CScene_RunTest()
{
}

CScene_RunTest * CScene_RunTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_RunTest*	pInstance = new CScene_RunTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_RunTest::Free()
{
	CScene::Free();
}

HRESULT CScene_RunTest::Ready_Scene()
{
	AfxGetApp()->GetMainWnd()->SetWindowTextW(L"Scene_RunTest");
	CScene::Ready_Scene();
	CUI_Manager::GetInstance()->m_fpComVtory_End_Button = bind(&CScene_RunTest::End_Battle, this, E_SCENETAG::Scene_Lobby);
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();
	
	return S_OK;
}

_int CScene_RunTest::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 

	Set_SkySphere(L"../../Reference/Resource/Texture/Test/fullmoon.jpg"); 

	return iExit;
}

_int CScene_RunTest::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta); 

	if (Management Key_Down(KEY_F2))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
	}

	if (Management Key_Down(KEY_F3))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_DYNAMIC); 
	}
	else if (Management Key_Down(KEY_F4))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_QUAT);
	}

	return	iExit;
}

_int CScene_RunTest::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CScene_RunTest::Render_Scene()
{
}

HRESULT CScene_RunTest::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_RunTest::Ready_LightInfo()
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

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0)< 0) return E_FAIL;

	return S_OK;
}

HRESULT CScene_RunTest::Ready_SceneObject()
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

void CScene_RunTest::End_Battle(E_SCENETAG eTag)
{
	CGameManager::GetInstance()->Set_NextWorld(nullptr);

	if (CUI_Manager::GetInstance()->m_pComVtory_Animation)
	{
		CUI_Manager::GetInstance()->m_pComVtory_Animation->Set_Dead(TRUE);
	}

	CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
}
