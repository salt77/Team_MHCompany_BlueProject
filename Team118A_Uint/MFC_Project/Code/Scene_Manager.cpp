#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_Manager.h"

#include "Scene_Loading.h"
#include "Scene_Logo.h"
#include "Scene_Lobby.h"
#include "Scene_Cafe.h"
#include "Scene_Combat.h"
#include "Scene_TileField.h"
#include "Scene_RunTest.h"

#include "FogManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CScene_Manager)

CScene_Manager::CScene_Manager()
{

}


CScene_Manager::~CScene_Manager()
{
	Free();
}

void CScene_Manager::Free()
{
}

// eSceneTag로 (다음씬의 enum)으로 다음씬의 로딩을 시작함 
// 만약 이미 로딩중이였는데 다시 로딩 하면 중단자
void CScene_Manager::Set_Loading(E_SCENETAG eSceneTag)
{
	if (m_pLoading) 
	{
		// 이미 로딩을 하는 중임
		assert(FALSE);
	}
	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;

	m_eSceneTagNext = eSceneTag;

	m_pLoading = CScene_Loading::Create(m_pGraphicDev, eSceneTag);

	// 로딩 객체를 생성하는데 실패하였다.
	assert(m_pLoading);
}

// _bool로 현재 로딩중인지 판단
_bool CScene_Manager::Get_Is_Loading()
{
	if (m_pLoading) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

// _bool로 현재 로딩이 끝났는지 판단
_bool CScene_Manager::Get_Loading_Finish()
{
	if (m_pLoading) 
	{
		return m_pLoading->Get_Finish();
	}
	else 
	{
		return false;
	}
}

// wstring로 현재 로딩중인 로딩객체의 문자열(로딩 상태를 담고있음)을 반환
// 로딩이 없으면 L"No Loading" 반환
const wstring CScene_Manager::Get_Loading_String()
{
	if (m_pLoading) 
	{
		return m_pLoading->Get_Wstring();
	}
	else 
	{
		return L"No Loading";
	}
}

// 로딩이 끝난 상태이면 현재 씬을 다음 씬으로 바꾼다
_int CScene_Manager::Set_Scene()
{
	if (!m_pLoading) return E_FAIL;

	if (!m_pLoading->Get_Finish()) return E_FAIL;
	
	Safe_Release(m_pLoading);
	
	m_pScene = nullptr;

	E_SCENETAG eSceneTag = m_eSceneTagNext;
	m_eSceneTagNext = E_SCENETAG::Scene_END;

	CUI_Manager::GetInstance()->Scene_Change_Start();

	CFogManager::GetInstance()->SetCustomFog(T_CustomFog());

	switch (eSceneTag)
	{
	case E_SCENETAG::Scene_LoadingScene:
		m_eSceneTag = E_SCENETAG::Scene_LoadingScene;
		//m_pScene = CLoadingScene::Create(m_pGraphicDev);

		assert(FALSE);
		// 씬 객체를 생성하고 불러줘야 정상 작동
		break;

	case E_SCENETAG::Scene_Logo:
		m_eSceneTag = E_SCENETAG::Scene_Logo;
		m_pScene = CScene_Logo::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Lobby:
		m_eSceneTag = E_SCENETAG::Scene_Lobby;
		m_pScene = CScene_Lobby::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Cafe:         
		m_eSceneTag = E_SCENETAG::Scene_Cafe;
		m_pScene = CScene_Cafe::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Combat:
		m_eSceneTag = E_SCENETAG::Scene_Combat;
		m_pScene = CScene_Combat::Create(m_pGraphicDev); 
		break;
	case E_SCENETAG::Scene_TileField:
		m_eSceneTag = E_SCENETAG::Scene_TileField;
		m_pScene = CScene_TileField::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_RunTest:
		m_eSceneTag = E_SCENETAG::Scene_RunTest;
		m_pScene = CScene_RunTest::Create(m_pGraphicDev); 
		break; 
	default:
		assert(FALSE);
		// 잘못된 씬 접근
		break;
	}

	assert(m_pScene);


	if (Management Set_Scene(m_pScene)< 0) return E_FAIL;

	CUI_Manager::GetInstance()->Scene_Change_End();

	CGameManager::GetInstance()->Reset_BattleTotalTime();
	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->bUpdateNextFrame = TRUE;

	
	return S_OK;
}

// 맨처음 씬 매니저를 시작할때 로고를 호출한다.
// 로고를 먼저 호출하지 않으면 다른 씬들의 작동에서 문제 생길 수 있음
_int CScene_Manager::Set_Scene_Start()
{
	m_pScene = CScene_Logo::Create(m_pGraphicDev);
	Set_UIManagerFunc();

	assert(m_pScene);
	
	if (Management Set_Scene(m_pScene)< 0) return E_FAIL;
	
	return _int();
}

void CScene_Manager::Set_UIManagerFunc()
{
	CUI_Manager::GetInstance()->Set_Func_CombatScene(bind(&CScene_Manager::Set_Loading, this, E_SCENETAG::Scene_Combat));
	CUI_Manager::GetInstance()->Set_Func_RunGaneScene(bind(&CScene_Manager::Set_Loading, this, E_SCENETAG::Scene_RunTest));
	CUI_Manager::GetInstance()->Set_Func_TileScene(bind(&CScene_Manager::Set_Loading, this, E_SCENETAG::Scene_TileField));
	CUI_Manager::GetInstance()->Set_Func_CafeScene(bind(&CScene_Manager::Set_Loading, this, E_SCENETAG::Scene_Cafe));
}
