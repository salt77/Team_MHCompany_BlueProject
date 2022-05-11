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

// eSceneTag�� (�������� enum)���� �������� �ε��� ������ 
// ���� �̹� �ε����̿��µ� �ٽ� �ε� �ϸ� �ߴ���
void CScene_Manager::Set_Loading(E_SCENETAG eSceneTag)
{
	if (m_pLoading) 
	{
		// �̹� �ε��� �ϴ� ����
		assert(FALSE);
	}
	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;

	m_eSceneTagNext = eSceneTag;

	m_pLoading = CScene_Loading::Create(m_pGraphicDev, eSceneTag);

	// �ε� ��ü�� �����ϴµ� �����Ͽ���.
	assert(m_pLoading);
}

// _bool�� ���� �ε������� �Ǵ�
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

// _bool�� ���� �ε��� �������� �Ǵ�
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

// wstring�� ���� �ε����� �ε���ü�� ���ڿ�(�ε� ���¸� �������)�� ��ȯ
// �ε��� ������ L"No Loading" ��ȯ
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

// �ε��� ���� �����̸� ���� ���� ���� ������ �ٲ۴�
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
		// �� ��ü�� �����ϰ� �ҷ���� ���� �۵�
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
		// �߸��� �� ����
		break;
	}

	assert(m_pScene);


	if (Management Set_Scene(m_pScene)< 0) return E_FAIL;

	CUI_Manager::GetInstance()->Scene_Change_End();

	CGameManager::GetInstance()->Reset_BattleTotalTime();
	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->bUpdateNextFrame = TRUE;

	
	return S_OK;
}

// ��ó�� �� �Ŵ����� �����Ҷ� �ΰ� ȣ���Ѵ�.
// �ΰ� ���� ȣ������ ������ �ٸ� ������ �۵����� ���� ���� �� ����
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
