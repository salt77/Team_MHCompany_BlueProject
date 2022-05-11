#include "stdafx.h"
#include "../Include/stdafx.h"
#include "ClientScene_Loading.h"
#include "Management.h"

_bool CClientScene_Loading::m_bLoad_Lobby = false;

CClientScene_Loading::CClientScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CClientScene_Loading::~CClientScene_Loading()
{

}

unsigned int CALLBACK CClientScene_Loading::Thread_Main(void* pArg)
{
	CClientScene_Loading*		pLoading = (CClientScene_Loading*)pArg;
	
	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->m_eLoadingSceneTag)
	{
	case E_SCENETAG::Scene_LoadingScene:
		iFlag = pLoading->Loading_LoadingScene();
		break;
	case E_SCENETAG::Scene_Logo:
		iFlag = pLoading->Loading_Logo();
		break;
	case E_SCENETAG::Scene_Lobby:
		iFlag = pLoading->Loading_Lobby();
		break;
	case E_SCENETAG::Scene_Cafe:
		iFlag = pLoading->Loading_Cafe();
		break;
	case E_SCENETAG::Scene_Combat:
		iFlag = pLoading->Loading_Combat();
		break;
	case E_SCENETAG::Scene_RunTest:
		iFlag = pLoading->Loading_Running(); 
		break;
	case E_SCENETAG::Scene_TileField:
		iFlag = pLoading->Loading_Tile();
		break;
	default:
		assert(FALSE);
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

HRESULT CClientScene_Loading::Ready_Loading(E_SCENETAG eLoadingSceneTag)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingSceneTag = eLoadingSceneTag;
	
	return 0;
}

_uint CClientScene_Loading::Loading_LoadingScene()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩
	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CClientScene_Loading::Loading_Logo()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CClientScene_Loading::Loading_Lobby()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	if (false == m_bLoad_Lobby)
	{
		//여기서 로딩
		// 시로 폭탄 메쉬
		Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_S", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb/", L"Shiro_Bomb.X"));
		Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_B", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb_OL/", L"Shiro_Bomb_OL.X"));

		CXL_ParsingMgr::GetInstance()->Ready_Manager(L"../../Reference/Resource/Xlsx/쥬신 블루아카이브 모작.xlsx", &g_iLoadingGage);
		//////////////////////////////////////////////////////////////

		CUI_Manager::GetInstance()->Set_LoadingGage(&g_iLoadingGage);
		CGameManager::GetInstance()->Ready_GameManager();
		CUI_Manager::GetInstance()->Create_Ui_Lobby();
		m_wstrLoading = L"LoadingComplete!";
		m_bLoad_Lobby = true;
	}

	m_bFinish = true;

	return 0;
}

_uint CClientScene_Loading::Loading_Cafe()
{
	m_bFinish = true;
	return _uint();
}

_uint CClientScene_Loading::Loading_Combat()
{
	m_bFinish = true;
	return _uint();
}

_uint CClientScene_Loading::Loading_Running()
{
	m_bFinish = true;
	return _uint();
}

_uint CClientScene_Loading::Loading_Tile()
{
	m_bFinish = true;
	return _uint();
}


CClientScene_Loading* CClientScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_SCENETAG eSceneTag)
{
	CClientScene_Loading*	pInstance = new CClientScene_Loading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eSceneTag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CClientScene_Loading::Free()
{
	Safe_Release(m_pGraphicDev);
	
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

