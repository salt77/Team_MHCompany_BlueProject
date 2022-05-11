#include "stdafx.h"
#include "Scene_Manager.h"
#include "Scene_Loading.h"

CScene_Loading::CScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CScene_Loading::~CScene_Loading()
{

}

unsigned int CScene_Loading::Thread_Main(void * pArg)
{
	CScene_Loading*		pLoading = (CScene_Loading*)pArg;

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
	case E_SCENETAG::Scene_TileField:
		iFlag = pLoading->Loading_TileField();
		break;
	case E_SCENETAG::Scene_RunTest:
		iFlag = pLoading->Loading_RunTest();
		break;
	default:
		assert(FALSE);
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

CScene_Loading * CScene_Loading::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_SCENETAG eSceneTag)
{
	CScene_Loading*	pInstance = new CScene_Loading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eSceneTag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_Loading::Free()
{
	Safe_Release(m_pGraphicDev);

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

HRESULT CScene_Loading::Ready_Loading(E_SCENETAG eLoadingSceneTag)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingSceneTag = eLoadingSceneTag;

	return 0;
}

_uint CScene_Loading::Loading_LoadingScene()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_Logo()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_Lobby()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_Lobby2()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_Cafe()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩


	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_Combat()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_TileField()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}

_uint CScene_Loading::Loading_RunTest()
{
	m_wstrLoading = L"NowLoading~";

	//////////////////////////////////////////////////////////////

	//여기서 로딩

	//////////////////////////////////////////////////////////////

	m_wstrLoading = L"LoadingComplete!";

	m_bFinish = true;

	return 0;
}
