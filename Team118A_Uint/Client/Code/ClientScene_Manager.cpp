#include "stdafx.h"
#include "../Include/stdafx.h"
#include "ClientScene_Manager.h"

#include "ClientScene_Loading.h"
#include "ClientScene_Logo.h"
#include "ClientScene_Lobby.h"
#include "ClientScene_Cafe.h"
#include "ClientScene_Combat.h"
#include "ClientScene_Run.h"
#include "ClientScene_TileField.h"

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

void CScene_Manager::Set_Loading(E_SCENETAG eSceneTag)
{
	if (m_pLoading)
	{
		// 이미 로딩을 하는 중임
		assert(FALSE);
	}

	m_eSceneTagNext = eSceneTag;

	m_pLoading = CClientScene_Loading::Create(m_pGraphicDev, eSceneTag);

	// 로딩 객체를 생성하는데 실패하였다.
	assert(m_pLoading);
}

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

_int CScene_Manager::Set_Scene()
{
	if (!m_pLoading) return E_FAIL;

	if (!m_pLoading->Get_Finish()) return E_FAIL;

	Safe_Release(m_pLoading);

	m_pScene = nullptr;
	CScene* pScene = CScene::Get_MainScene();

	E_SCENETAG eBeforeTag = m_eSceneTag;
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
		m_pScene = CClientScene_Logo::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Lobby:
		m_eSceneTag = E_SCENETAG::Scene_Lobby;
		m_pScene = CClientScene_Lobby::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Cafe:
		m_eSceneTag = E_SCENETAG::Scene_Cafe;
		m_pScene = CClientScene_Cafe::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_Combat:
		m_eSceneTag = E_SCENETAG::Scene_Combat;
		m_pScene = CClientScene_Combat::Create(m_pGraphicDev);
		break;
	case E_SCENETAG::Scene_RunTest: 
		m_eSceneTag = E_SCENETAG::Scene_RunTest; 
		m_pScene = CClientScene_Run::Create(m_pGraphicDev); 
		break;
	case E_SCENETAG::Scene_TileField:
		m_eSceneTag = E_SCENETAG::Scene_TileField;
		m_pScene = CClientScene_TileField::Create(m_pGraphicDev);
		break;
	default:
		assert(FALSE);
		// 잘못된 씬 접근
		break;
	}

	assert(m_pScene);

	CUI_Manager::GetInstance()->Scene_Change_End();
	CGameManager::GetInstance()->Reset_BattleTotalTime();

	if (Management Set_Scene(m_pScene)< 0) return E_FAIL;


	return S_OK;
}

_int CScene_Manager::Set_Scene_Start()
{
	m_pScene = CClientScene_Logo::Create(m_pGraphicDev);
	Set_UIManagerFunc();
	m_eSceneTag = E_SCENETAG::Scene_Logo;
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

