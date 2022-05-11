#include "stdafx.h"
#include "../Include/stdafx.h"
#include "MainApp.h"

CMainApp::CMainApp()
{

}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_Main()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	Management	Ready_Managers(&g_hWnd);
	
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);

	FAILED_CHECK_RETURN(Management Ready_Renderer(m_pGraphicDev), E_FAIL);

	return S_OK;
}

void CMainApp::Update_Main(const _float& fTimeDelta)
{
	assert(m_pManagementClass);
	
	Management UpdateKeyManager(g_hWnd);
	
	float fFixedTimeDelta = Management Update_TimeScale(fTimeDelta);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_R) && PlayFabClientAPI::IsClientLoggedIn())
	{
		CPlayFab::GetInstance()->Save_UserInfo();
	}

	// 업데이트부
	CScene::Add_PlayTime(fFixedTimeDelta);
	m_pManagementClass->Update_Scene(fFixedTimeDelta);
	m_pManagementClass->LateUpdate_Scene(fFixedTimeDelta);

	// 랜더부
	if (Management Key_Down(KEY_1))
	{
		Management Set_Render_Defferd(TRUE);
	}
	if (Management Key_Down(KEY_2)) 
	{
		Management Set_Render_Defferd(FALSE);
	}

	m_pManagementClass->Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);
	
	m_pManagementClass->Render_End();

	// 씬전환체크부
	CScene_Manager::GetInstance()->Set_Scene();

}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Ready_Main()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free()
{
	CScene_Manager::DestroyInstance();

	Safe_Release(m_pGraphicDev);
	// 디버그에서는 누수안나는데 릴리즈에선 난다?
	//Safe_Release(m_pGraphicDev);
	Safe_Release(m_pManagementClass);
	Safe_Release(m_pDeviceClass);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	CPlayFab::DestroyInstance();

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Management Ready_GraphicDev(g_hWnd, E_WINMODE::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();
	
	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();

	CScene_Manager::GetInstance()->Ready_Scene_Manager(*ppGraphicDev);
	CPlayFab::GetInstance()->Setting_PlayFab();

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 & pGraphicDev, CManagement ** ppManagement)
{
	if (Management Create_Management(m_pGraphicDev, ppManagement)< 0) return E_FAIL;

	CFontMgr::GetInstance()->m_pLoadingGage = &g_iLoadingGage;
	CScene_Manager::GetInstance()->Set_Scene_Start();

	return S_OK;
}
