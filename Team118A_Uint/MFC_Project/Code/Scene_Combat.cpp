#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_Lobby.h"
#include "Scene_Manager.h"
#include "BackGround.h"
#include "SMesh.h"
#include "CharacterMgr.h"
#include "StageMap.h"
#include "Scene_Combat.h"
#include "WorldCreator.h"
#include "Sweeper_Decagram_Taser.h" 
#include "ActionCam_Mgr.h"
#include "Kuro.h"
#include "Binah.h"

USING(Engine)

CScene_Combat::CScene_Combat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_Combat::~CScene_Combat()
{
}

CScene_Combat * CScene_Combat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Combat*	pInstance = new CScene_Combat(pGraphicDev);
	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_Combat::Free()
{
	if (m_pBattleStartUI)
	{
		m_pBattleStartUI->Set_Dead(TRUE);
		m_pBattleStartUI = nullptr;
	}
	if (m_pBattleStartOthersUI)
	{
		m_pBattleStartOthersUI->Set_Dead(TRUE);
		m_pBattleStartOthersUI = nullptr;
	}
	if (m_pBattleUI_Stage)
	{
		m_pBattleUI_Stage->Set_Dead(TRUE);
		m_pBattleUI_Stage = nullptr;
	}

	if (m_pWorld2)
	{
		Safe_Release(m_pWorld2);
	}

	CScene::Free();
	CGameManager::GetInstance()->Set_BattleIng(FALSE);
}

HRESULT CScene_Combat::Ready_Scene()
{
	//CLoadingManager::GetInstance()->Set_Loading(TRUE);

	CGameManager::GetInstance()->Reset_Squad(CGameManager::GetInstance()->Get_CurrentSquadEnum());
	CUI_Manager::GetInstance()->m_fpComVtory_End_Button = bind(&CScene_Combat::End_Battle, this, E_SCENETAG::Scene_TileField);
	AfxGetApp()->GetMainWnd()->SetWindowTextW(L"전투");
	CScene::Ready_Scene();

	Ready_Prototype();
	
	Ready_SceneObject();
	Set_FogEvent();
	m_pWorld = CGameManager::GetInstance()->Get_NextWorld();
	if (nullptr == m_pWorld)
	{
		//auto iter = (*CGameManager::GetInstance()->Get_Worlds())[E_WORLD_MOD::MOD_TOTAL].begin();
		auto iter = (*CGameManager::GetInstance()->Get_Worlds())[E_WORLD_MOD::MOD_TOTAL].begin();

		m_pWorld = *iter;
		m_pWorld->Reset_World();
	}
	else
	{
		m_pWorld->Reset_World();
	}

	Ready_LightInfo();

	if (E_WORLD_MOD::MOD_MISSION == m_pWorld->Get_WorldMod())
	{
		m_pBattleStartOthersUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]배틀스타트_나머지.uidat");
		assert(m_pBattleStartOthersUI);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pBattleStartOthersUI);

		m_pBattleStartUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]배틀스타트_1.uidat");
		assert(m_pBattleStartUI);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pBattleStartUI);

		m_pBattleUI_Stage = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Battle_Normal_Stage.uidat");
		assert(m_pBattleUI_Stage);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pBattleUI_Stage);
		static_cast<CUI_Object*>(m_pBattleUI_Stage)->Set_Ui_Text(m_pWorld->Get_WorldName().substr(0, m_pWorld->Get_WorldName().find(L".")));
	}
	
	m_fBattleStartDelay = m_fBattleStartOriginDelay;

	Set_SkySphere(L"../../Reference/Resource/Texture/Test/SkyTest2.jpg");
	return S_OK;
}

_int CScene_Combat::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta);

	Active_AutoCam();

	m_pWorld->Spawn_World();
	m_pWorld->Spawn_Frontline();

	if (m_pWorld->Get_WorldMod() == E_WORLD_MOD::MOD_TOTAL)
	{
		Spawn_Special();
		Spawn_Striker();
		Play_Game();
		Set_Fit_Camera();
		m_bAutoCam = TRUE;
	}
	else if (E_WORLD_MOD::MOD_MISSION == m_pWorld->Get_WorldMod())
	{
		if (L"임무 2 - 1.world" == m_pWorld->Get_WorldName())
		{
			CSound_Manager::GetInstance()->PlayBGM(L"BGM_일반전투_기계_메카니컬정글.mp3", 0.1f); 
		}
		else if (L"임무 2 - 2.world" == m_pWorld->Get_WorldName())
		{
			CSound_Manager::GetInstance()->PlayBGM(L"BGM_일반전투_기계_메카니컬정글.mp3", 0.1f);
		}
		else if (L"임무 2 - 3.world" == m_pWorld->Get_WorldName())
		{
			CSound_Manager::GetInstance()->PlayBGM(L"BGM_일반전투_기계_스노우.mp3", 0.1f);
		}
		
		m_fMax_At_Z_Diff = 3.f;
		m_fMinEye_X = -8.5f;
		m_fMaxEye_X = -5.f;
		m_fMinEye_Y = 5.f;
		m_fMaxEye_Y = 8.f;
		m_fMinEye_Z = 1.f;
		m_fMaxEye_Z = 3.f;

		m_fAutoCam_Ratio = 0.85f;

		m_fBaseY_At = 1.f;
		m_fBaseY_Eye = 4.f;

		m_fFarX = -8.5f;
		m_fFarY = -0.05f;
		m_fFarZ = 1.f; 

		m_fCamSpeed = 2.f * CTimeScale_Manager::GetInstance()->Get_TimeScale();
	}
	else if (E_WORLD_MOD::MOD_WANTED == m_pWorld->Get_WorldMod()) 
	{
		CSound_Manager::GetInstance()->PlayBGM(L"BGM_스시레인저.mp3", 0.1f);

		//if (L"몰?루" == m_pWorld->Get_WorldName())
		//{
		//	CSound_Manager::GetInstance()->PlayBGM(L"BGM_스시레인저.mp3", 0.1f);
		//}
	}

	//CLoadingManager::GetInstance()->Set_Loading(FALSE);
	
	
	return iExit;
}

_int CScene_Combat::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);
	

	CGameManager::GetInstance()->Add_BattleTotalTime(fTimeDelta);
	TimeEvent(fTimeDelta);
	Update_FogEvent(fTimeDelta);
	Update_ShakeCam(fTimeDelta);
	Update_Fit_Camera(fTimeDelta);
	Update_BattleStartUI(fTimeDelta);

	if (Management Key_Down(KEY_K)) 
	{
		CCamera::m_pMainCam->Set_ShakeCamera(3.5f, 5.f); 
	}

	if (Management Key_Pressing(KEY_G) && Management Key_Down(KEY_L))
	{
		Active_AutoCam();
	}

	if (Management Key_Down(KEY_F2))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
	}

	if (Management Key_Down(KEY_F3))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

	if (Management Key_Down(KEY_F4))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Cafe);
	}

	if (Management Key_Down(KEY_F7))
	{
		if (CGameManager::GetInstance()->Get_TileField())
		{
			PopupTool_PickObject_Clear();
			CGameManager::GetInstance()->End_War(TRUE);
			CGameManager::GetInstance()->Set_NextWorld(nullptr);
			CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_TileField);
		}
	}

	if (Management Key_Down(KEY_RBUTTON))
	{
		CUI_Manager::GetInstance()->Function_Assemble();
	}

	if (!CGameManager::GetInstance()->Get_BattleIng())
	{
		Set_Outtro();
	}

	if (m_pWorld->Get_WorldName() == L"Binah.world")
	{
		Binah_OtherSetting();
	
		CBinah* pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

		if (pBinah->Get_Boss_HP()->Get_Hp() < 150000)
		{
			pBinah->Set_NowHP(-9999);
			pBinah->Set_Active(false);
			pBinah->Get_Boss_HP()->Set_Active(false);
			Change_World();
		}

	}

	return	iExit;
}

_int CScene_Combat::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CScene_Combat::Render_Scene()
{
}

HRESULT CScene_Combat::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_Combat::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// 빛 정보
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	if (m_pWorld->Get_WorldName() == L"쿠로.world")
	{
		tLightInfo.Diffuse = D3DXCOLOR(0.28f, 0.12f, 0.72f, 1.f);
	}
	else
	{
		tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	}
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	// 빛 위치	
	tLightInfo.Position = _vec3(0.f, 40.f, 0.f);

	// 빛 방향방향
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);

	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0) < 0) return E_FAIL;
	D3DLIGHT9* pLightInfo = Management Get_Light();

	return S_OK;
}

HRESULT CScene_Combat::Binah_OtherSetting()
{
	//Binah Light Setting
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tLightInfo.Position = _vec3(0.f, 40.f, 0.f);
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);
	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0) < 0) return E_FAIL;

	D3DLIGHT9* pLightInfo = Management Get_Light();

	//Binah Sky Sphere Setting
	if (m_bCreate_Binah_Sky)
	{
		Set_SkySphere(L"../../Reference/Resource/Texture/Test/Binah_Sky.jpg");
		m_bCreate_Binah_Sky = false;
	}

	return S_OK;
}

HRESULT CScene_Combat::Ready_SceneObject()
{
	CGameObject* pGameObject = nullptr;

	// Dynamic Camera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(_vec3(-3.f, 5.f, 9.f), _vec3(0.f, 2.f, 0.f), VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_DYNAMIC);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);


	return S_OK;
}

void CScene_Combat::Set_FogEvent()
{
	m_fFogTotalTime = 0.7f;
	m_tFogColor[0] = D3DXCOLOR(1.f, 0.2f, 0.2f, 1.f);
	m_tFogColor[1] = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_fFogStart[0] = 3.f;
	m_fFogStart[1] = 23.f;
	m_fFogEnd[0] = 7.f;
	m_fFogEnd[1] = 100.f;

	//// 커스텀 포그 [0]
	//m_tCustomFog[0].vPos = _vec3(0.f, 0.f, 0.f);
	//
	//m_tCustomFog[0].fView_Under = 0.f;
	//m_tCustomFog[0].fView_Over = 0.f;
	//
	//m_tCustomFog[0].fDistance_Start = 10.f;
	//m_tCustomFog[0].fDistance_End = 12.f;
	//
	//m_tCustomFog[0].vColor_Start = _vec4(0.f, 0.f, 1.f, 1.f);
	//m_tCustomFog[0].vColor_End = _vec4(0.f, 0.f, 1.f, 1.f);
	//
	//// 커스텀 포그 [1]
	//m_tCustomFog[1].vPos = _vec3(0.f, 0.f, 0.f);
	//
	//m_tCustomFog[1].fView_Under = 0.f;
	//m_tCustomFog[1].fView_Over = 0.f;
	//
	//m_tCustomFog[1].fDistance_Start = 20.f;
	//m_tCustomFog[1].fDistance_End = 30.f;
	//
	//m_tCustomFog[1].vColor_Start = _vec4(1.f, 0.f, 0.f, 0.f);
	//m_tCustomFog[1].vColor_End = _vec4(1.f, 0.f, 0.f, 0.f);
}

void CScene_Combat::Update_FogEvent(const _float& fTimeDelta)
{
	m_fFogNowTime += fTimeDelta;
	_float fRatio = m_fFogNowTime / m_fFogTotalTime;
	fRatio = CUtility::Clamp_float(fRatio, 0.f, 1.f);

	CUtility::LerpEase_float(E_EASE::OutQuint, fRatio);

	_vec4 vColor;
	vColor.x = CUtility::Lerp_float(m_tFogColor[0].r, m_tFogColor[1].r, fRatio);
	vColor.y = CUtility::Lerp_float(m_tFogColor[0].g, m_tFogColor[1].g, fRatio);
	vColor.z = CUtility::Lerp_float(m_tFogColor[0].b, m_tFogColor[1].b, fRatio);
	vColor.w = CUtility::Lerp_float(m_tFogColor[0].a, m_tFogColor[1].a, fRatio);

	_float fStart = CUtility::Lerp_float(m_fFogStart[0], m_fFogStart[1], fRatio);
	_float fEnd = CUtility::Lerp_float(m_fFogEnd[0], m_fFogEnd[1], fRatio);

	Management Set_FogColor(vColor);
	Management Set_FogStart(fStart);
	Management Set_FogEnd(fEnd);

	T_CustomFog tCustomFog = T_CustomFog();
	CFogManager::GetInstance()->SetCustomFog(tCustomFog);
}

void CScene_Combat::Active_AutoCam()
{
	m_bAutoCam = !m_bAutoCam;

	if (m_bAutoCam)
	{
		Set_Fit_Camera();
	}
}

void CScene_Combat::Set_Fit_Camera()
{
	if (CRenderer::GetInstance()->Get_CutIn())
		return;

	// 가장 z가 큰 유닛을 저장한다(ALL). 살아있는 유닛중 가장 z가 큰 유닛 (Alive)

	vector<CCharacter*>& pChar = (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER];

	_bool bAllDead = TRUE;
	_float fCharBack = -100.f;
	for (_int i = 0; i < (_int)pChar.size(); ++i)
	{
		if (nullptr == pChar[i])
			continue;

		if (fCharBack <= pChar[i]->Get_Position().z && FALSE == pChar[i]->Get_UnitDead())
		{
			fCharBack = pChar[i]->Get_Position().z;
			bAllDead = FALSE;
		}
	}

	if (!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster))
	{
		return;
	}

	vector<CGameObject*>* pVecMonster = Management Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);

	_float fMonFront = 0.f;
	_float fCamCenterZ = 0.f;
	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (fMonFront >= (*pVecMonster)[i]->Get_Position().z && FALSE == static_cast<CUnit*>((*pVecMonster)[i])->Get_UnitDead())
		{
			fMonFront = (*pVecMonster)[i]->Get_Position().z;
			bAllDead = FALSE;
		}
	}

	if (bAllDead)
		return;

	if (CScene::Get_MainScene()->Get_Binah_Cam())
	{
		fCamCenterZ = fCharBack;
	}
	else
	{
		fCamCenterZ = (fCharBack + fMonFront) * 0.5f;
	}

	_float fDiff = fCharBack - fMonFront;

	_vec3 vCamAt = _vec3(0.f, m_fBaseY_At, CUtility::Clamp_float(fCamCenterZ, fCharBack - m_fMax_At_Z_Diff, fCharBack));

	_float fEye_X = CUtility::Clamp_float(m_fFarX * fDiff, m_fMinEye_X, m_fMaxEye_X);
	_float fEye_Y = CUtility::Clamp_float(m_fBaseY_Eye + (m_fFarY * fDiff), m_fMinEye_Y, m_fMaxEye_Y);
	_float fEye_Z = CUtility::Clamp_float(m_fFarZ * fDiff, m_fMinEye_Z, m_fMaxEye_Z);

	_vec3 vCamEye = vCamAt + _vec3(fEye_X, fEye_Y, fEye_Z) * m_fAutoCam_Ratio;

	CCamera::m_pMainCam->Set_CamEye(vCamEye);
	CCamera::m_pMainCam->Set_CamAt(vCamAt);

	m_vLastCam_Eye = vCamEye;
	m_vLastCam_At = vCamAt;
}

void CScene_Combat::Update_Fit_Camera(const _float& fTimeDelta)
{
	if (CRenderer::GetInstance()->Get_CutIn() || FALSE == m_bAutoCam)
		return;

	if (m_pWorld->Get_WorldName() == L"쿠로.world")
	{
		CKuro* pKuro = static_cast<CKuro*>(Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster)->front());

		_bool bCheck = pKuro->Update_KuroCam(fTimeDelta);

		if (bCheck)
		{
			_vec3 vCamEye = CCamera::m_pMainCam->Get_CamEye();
			_vec3 vCamAt = CCamera::m_pMainCam->Get_CamAt();

			_vec3 vDir_Eye = vCamEye - m_vLastCam_Eye;
			_vec3 vDir_At = vCamAt - m_vLastCam_At;

			if (D3DXVec3Length(&vDir_Eye) <= 0.1f)
			{
				CCamera::m_pMainCam->Set_CamEye(vCamEye + m_vCamShake);
			}
			else
			{
				D3DXVec3Normalize(&vDir_Eye, &vDir_Eye);
				CCamera::m_pMainCam->Set_CamEye(m_vLastCam_Eye + (vDir_Eye * m_fCamSpeed * fTimeDelta) + m_vCamShake);
			}

			if (D3DXVec3Length(&vDir_At) <= 0.1f)
			{
				CCamera::m_pMainCam->Set_CamAt(vCamAt + m_vCamShake);
			}
			else
			{
				D3DXVec3Normalize(&vDir_At, &vDir_At);
				CCamera::m_pMainCam->Set_CamAt(m_vLastCam_At + (vDir_At * m_fCamSpeed * fTimeDelta) + m_vCamShake);
			}

			m_vLastCam_Eye = CCamera::m_pMainCam->Get_CamEye() - m_vCamShake;
			m_vLastCam_At = CCamera::m_pMainCam->Get_CamAt() - m_vCamShake;
			return;
		}
	}

	// 가장 z가 큰 유닛을 저장한다(ALL). 살아있는 유닛중 가장 z가 큰 유닛 (Alive)

	vector<CCharacter*>& pChar = (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER];

	_bool bAllDead = TRUE;
	_float fCharBack = -100.f;
	for (_int i = 0; i < (_int)pChar.size(); ++i)
	{
		if (nullptr == pChar[i])
			continue;

		if (fCharBack <= pChar[i]->Get_Position().z && FALSE == pChar[i]->Get_UnitDead())
		{
			fCharBack = pChar[i]->Get_Position().z;
			bAllDead = FALSE;
		}
	}

	vector<CGameObject*>* pVecMonster = Management Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);

	_float fMonFront = 0.f;
	_float fCamCenterZ = 0.f;
	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (fMonFront >= (*pVecMonster)[i]->Get_Position().z && FALSE == static_cast<CUnit*>((*pVecMonster)[i])->Get_UnitDead())
		{
			fMonFront = (*pVecMonster)[i]->Get_Position().z;
			bAllDead = FALSE;
		}
	}

	if (bAllDead)
		return;

	if (CScene::Get_MainScene()->Get_Binah_Cam())
	{
		fCamCenterZ = fCharBack;
	}
	else
	{
		fCamCenterZ = (fCharBack + fMonFront) * 0.5f;
	}

	_float fDiff = fCharBack - fMonFront;

	_vec3 vCamAt = _vec3(0.f, m_fBaseY_At, CUtility::Clamp_float(fCamCenterZ, fCharBack - m_fMax_At_Z_Diff, fCharBack));

	_float fEye_X = CUtility::Clamp_float(m_fFarX * fDiff, m_fMinEye_X, m_fMaxEye_X);
	_float fEye_Y = CUtility::Clamp_float(m_fBaseY_Eye + (m_fFarY * fDiff), m_fMinEye_Y, m_fMaxEye_Y);
	_float fEye_Z = CUtility::Clamp_float(m_fFarZ * fDiff, m_fMinEye_Z, m_fMaxEye_Z);

	_vec3 vCamEye = vCamAt + _vec3(fEye_X, fEye_Y, fEye_Z) * m_fAutoCam_Ratio;

	_vec3 vDir_Eye = vCamEye - m_vLastCam_Eye;
	_vec3 vDir_At = vCamAt - m_vLastCam_At;

	if (D3DXVec3Length(&vDir_Eye) <= 0.1f)
	{
		CCamera::m_pMainCam->Set_CamEye(vCamEye + m_vCamShake);
	}
	else
	{
		D3DXVec3Normalize(&vDir_Eye, &vDir_Eye);
		CCamera::m_pMainCam->Set_CamEye(m_vLastCam_Eye + (vDir_Eye * m_fCamSpeed * fTimeDelta) + m_vCamShake);
	}

	if (D3DXVec3Length(&vDir_At) <= 0.1f)
	{
		CCamera::m_pMainCam->Set_CamAt(vCamAt + m_vCamShake);
	}
	else
	{
		D3DXVec3Normalize(&vDir_At, &vDir_At);
		CCamera::m_pMainCam->Set_CamAt(m_vLastCam_At + (vDir_At * m_fCamSpeed * fTimeDelta) + m_vCamShake);
	}

	m_vLastCam_Eye = CCamera::m_pMainCam->Get_CamEye() - m_vCamShake;
	m_vLastCam_At = CCamera::m_pMainCam->Get_CamAt() - m_vCamShake;
	//CUtility::Print_Wstring_DMode(L"제일 뒤에있는 캐릭터 fCharBack :" + to_wstring(fCharBack));
	//CUtility::Print_Wstring_DMode(L"제일 뒤에있는 몬스터 fMonFront :" + to_wstring(fMonFront));
	//CUtility::Print_Wstring_DMode(L"fCamCenterZ :" + to_wstring(fCamCenterZ));
	//CUtility::Print_Wstring_DMode(L"fDiff :" + to_wstring(fDiff));
}

void CScene_Combat::Active_ShakeCam(const _float & fShakeTime, const _float & fShakePower)
{
	m_fCamShake_Time = fShakeTime;
	m_fCamShake_Power = fShakePower;
}

void CScene_Combat::Update_ShakeCam(const _float & fTimeDelta)
{
	m_fCamShake_Time -= fTimeDelta;
	if (0.f < m_fCamShake_Time)
	{
		_vec3 vCamRight = CCamera::m_pMainCam->Get_CamRight();
		D3DXVec3Normalize(&vCamRight, &vCamRight);
		_vec3 vCamUp = CCamera::m_pMainCam->Get_CamUp();
		D3DXVec3Normalize(&vCamUp, &vCamUp);

		m_vCamShake = (vCamRight)* CWall512::GetInstance()->GetFloatValue(-1.f, 1.f) + (vCamUp)* CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
		m_vCamShake *= m_fCamShake_Power;
	}
	else
	{
		m_vCamShake = VEC_ZERO;
	}
}

void CScene_Combat::End_Battle(E_SCENETAG eTag)
{
	for (auto& tPair : *Get_LayerMapObject(E_LAYERTAG::Character))
	{
		for (auto& pChar : tPair.second)
		{
			static_cast<CUnit*>(pChar)->Reset();
			static_cast<CUnit*>(pChar)->Reset_HP();
		}
	}

	CGameManager::GetInstance()->End_War(CGameManager::GetInstance()->Get_IsBattleWin());

	CGameManager::GetInstance()->Set_NextWorld(nullptr);
	if (CUI_Manager::GetInstance()->m_pComVtory_Animation)
	{
		CUI_Manager::GetInstance()->m_pComVtory_Animation->Set_Dead(TRUE);
	}

	if (CGameManager::GetInstance()->Get_IsTileFiledBattle())
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_TileField);
	}
	else
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

}

void CScene_Combat::TimeEvent(_float fTimeDelta)
{
	if (m_pWorld->Get_WorldMod() == E_WORLD_MOD::MOD_TOTAL)
	{
		return;
	}

	m_fTimer += fTimeDelta;

	switch (m_iEventCount)
	{
	case 0:
		Spawn_Special();
		CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Intro.camdat", L"Intro1", m_vCamSpot1, 180.f);

		++m_iEventCount;
		break;
	case 1:
		if (m_fTimer > 1.8f)
		{
			CActionCam_Mgr::GetInstance()->Stop_ActionCam();
			Spawn_Striker();
			CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Intro.camdat", L"Intro2", m_vCamSpot2, 180.f);

			++m_iEventCount;
		}
		break;
	case 2:
		if (m_fTimer > 3.f)
		{
			CActionCam_Mgr::GetInstance()->Stop_ActionCam();
			Play_Game();
			Set_Fit_Camera();
			m_bAutoCam = TRUE;
			++m_iEventCount;
		}
		break;
	default:
		break;
	}

}

void CScene_Combat::Spawn_Striker()
{
	map<_int, vector<CCharacter*>>* pMapFormation = CGameManager::GetInstance()->Get_CurrentSquad();

	_vec3 vReaderPos = (m_pWorld->Get_StartPos(1) + m_pWorld->Get_StartPos(2)) * 0.5f;
	_vec3 vReadyPos[3] =
	{
		vReaderPos + VEC_LOOK + VEC_RIGHT,
		vReaderPos + VEC_LOOK + VEC_LEFT,
		vReaderPos + VEC_LOOK * 2
	};
	m_vCamSpot2 = vReaderPos;

	_int iPosCount = 0;
	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pChar = nullptr;

		pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];


		if (pChar)
		{
			pChar->Set_Active(TRUE);
			pChar->AddRef();
			if (i == CGameManager::GetInstance()->GetReaderIndex(CGameManager::GetInstance()->Get_CurrentSquadEnum()))
			{
				pChar->Set_Pos(vReaderPos);
				pChar->Set_Reader(TRUE);
			}
			else
			{
				pChar->Set_Pos(vReadyPos[iPosCount]);
				++iPosCount;
				pChar->Set_Reader(FALSE);
			}

			pChar->Set_Rotation(_vec3(0.f, 180.f, 0.f));
			Add_GameObject(E_LAYERTAG::Character, pChar->Get_ObjTag(), pChar);
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = nullptr;

		pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];

		if (pChar)
		{
			pChar->Set_Play(TRUE);
			pChar->Set_Active(FALSE);
		}
	}
}

void CScene_Combat::Spawn_Special()
{
	map<_int, vector<CCharacter*>>* pMapFormation = CGameManager::GetInstance()->Get_CurrentSquad();

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = nullptr;
		pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];

		if (pChar)
		{
			pChar->Set_Active(TRUE);
			pChar->AddRef();
			pChar->Set_Pos(m_pWorld->Get_StartPos(i + 4));
			pChar->Set_Rotation(_vec3(0.f, 180.f, 0.f));
			Add_GameObject(E_LAYERTAG::Character, pChar->Get_ObjTag(), pChar);
		}
	}
	m_vCamSpot1 = (m_pWorld->Get_StartPos(4) + m_pWorld->Get_StartPos(5)) * 0.5f;
}

void CScene_Combat::Set_Outtro()
{
	if (m_bEnd)
	{
		return;
	}

	if (CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers().empty())
	{
		return;
	}
	if (m_pBattleUI_Stage)
	{
		m_pBattleUI_Stage->Set_Active(FALSE);
	}
	
	m_bEnd = TRUE;
	_vec3 vDir = VEC_LOOK;

	_float fStartDegree = -60.f;
	_float fAddDegree = 40.f;
	_float fDistance = 1.1f;

	_matrix matRotY;

	_int iIndex = 0;

	if (m_pWorld->Get_WorldName() == L"쿠로.world")
	{
		m_vCamSpot2 = _vec3(0.f, 0.f, -34.f);
	}

	for (auto& pChar : *Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
	{
		if (pChar)
		{
			_vec3 vSpawnDir;
			D3DXMatrixRotationY(&matRotY, fStartDegree * Deg2Rad);
			D3DXVec3TransformNormal(&vSpawnDir, &vDir, &matRotY);

			pChar->Set_Pos(m_vCamSpot2 + vSpawnDir * fDistance);
			pChar->Set_Look(m_vCamSpot2);
			static_cast<CUnit*>(pChar)->Clear_Astar();
		}
		fStartDegree += fAddDegree;
	}

	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Intro.camdat", L"Outtro1", m_vCamSpot2, 180.f);
}

void CScene_Combat::Change_World()
{
	if (!m_pWorld2)
	{
		m_pWorld2 = CWorldCreator::Create(L"Hod.world");
		m_pWorld2->Ready_FrontLine();
		m_pWorld2->Spawn_Frontline();

		map<_int, vector<CCharacter*>>* pMapFormation = CGameManager::GetInstance()->Get_CurrentSquad();		

		for (_int i = 0; i < 4; ++i)
		{
			CCharacter* pChar = nullptr;

			pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];

			if (pChar)
			{
				if (!static_cast<CUnit*>(pChar)->Get_UnitDead())
				{
					pChar->Reset();

					pChar->Set_Pos(m_pWorld2->Get_StartPos(i));
					pChar->Set_Play(TRUE);
				}
			}
		}
	}
}

void CScene_Combat::Play_Game()
{
	if (m_pBattleStartUI &&
		m_pBattleStartOthersUI)
	{
		m_pBattleStartUI->Set_Active(TRUE);
		m_pBattleStartOthersUI->Set_Active(TRUE);

		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"전투시작_Start.wav", 0.15f); 
	}

	map<_int, vector<CCharacter*>>* pMapFormation = CGameManager::GetInstance()->Get_CurrentSquad();

	_int iPosCount = 0;
	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pChar = nullptr;

		pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];

		if (pChar)
		{
			pChar->Set_Active(TRUE);

			pChar->Set_Pos(m_pWorld->Get_StartPos(i));
			pChar->Set_Rotation(_vec3(0.f, 180.f, 0.f));
			pChar->Set_Play(TRUE);

		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = nullptr;

		pChar = (*pMapFormation)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];

		if (pChar)
		{
			pChar->Set_Play(TRUE);
			pChar->Set_Active(FALSE);
		}
	}
}

void CScene_Combat::Update_BattleStartUI(const _float & fTimeDelta)
{
	if (E_WORLD_MOD::MOD_MISSION == m_pWorld->Get_WorldMod())
	{
		if (m_pBattleStartUI &&
			m_pBattleStartOthersUI)
		{
			if (m_pBattleStartUI->Get_Active() &&
				m_pBattleStartOthersUI->Get_Active())
			{
				m_fBattleStartDelay -= fTimeDelta;

				if (0.f >= m_fBattleStartDelay)
				{
					m_pBattleStartUI->Set_Dead(TRUE);
					m_pBattleStartUI = nullptr;
					m_pBattleStartOthersUI->Set_Dead(TRUE);
					m_pBattleStartOthersUI = nullptr;

					return;
				}

				if (m_fBattleStartOriginDelay * Half >= m_fBattleStartDelay)
				{
					m_pBattleStartUI->Add_UiMoving(T_UiMoving(_vec2(WINCX, 0.f), m_fBattleStartOriginDelay * Half, E_EASE::OutQuint));
				}
			}
		}
	}
}

