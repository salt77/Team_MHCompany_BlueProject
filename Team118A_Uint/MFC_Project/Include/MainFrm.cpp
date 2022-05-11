
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MFC_Project.h"

#include "MainFrm.h"

#include "Scene_Manager.h"

HWND g_hWnd;
HINSTANCE g_hInst;
_int g_iWinY;
_int g_iWinX;
int g_iSoundStack;
char g_szConsoleText[MAX_PATH]{};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.

#ifdef  _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //  _DEBUG
	//_CrtSetBreakAlloc(�޸� �ѹ�);
}

CMainFrame::~CMainFrame()
{
	CScene_Manager::DestroyInstance();
	
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pManagement);
	Safe_Release(m_pDeviceClass);

	FreeConsole();
}

HRESULT CMainFrame::ReadyMainApp()
{
	Management Ready_GraphicDev(g_hWnd, E_WINMODE::MODE_WIN, g_iWinX, g_iWinY, &m_pDeviceClass);
	LPDIRECT3DDEVICE9 pGraphicDev = m_pDeviceClass->GetDevice();

	CManagement** pManagement = &m_pManagement;

	CXL_ParsingMgr::GetInstance()->Ready_Manager(L"../../Reference/Resource/Xlsx/��� ����ī�̺� ����.xlsx", nullptr);
	
	Management Ready_Managers(&g_hWnd);
	Management Create_Management(pGraphicDev, pManagement);
	Management Ready_Renderer(pGraphicDev);


	NULL_CHECK_RETURN(m_pManagement, E_FAIL);
	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->GetDevice();
	(m_pGraphicDev)->AddRef();

	(m_pGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);

	ReadyGlobalResources();
	ReadyDefaultSettings();

	CScene_Manager::GetInstance()->Ready_Scene_Manager(m_pGraphicDev);
	CScene_Manager::GetInstance()->Set_Scene_Start();

	return S_OK;
}

CGameObject * CMainFrame::Get_PickObject()
{
	return m_pMenuForm->m_pPopupTool_ObjectTree->Get_PickObject();
}

HRESULT CMainFrame::ReadyGlobalResources()
{
	return S_OK;
}

HRESULT CMainFrame::ReadyDefaultSettings()
{
	_matrix matView, matProj;

	D3DXMatrixLookAtLH(&matView, &_vec3(0.f, 0.f, -2.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.f), (_float)g_iWinX / g_iWinY, 1.f, 1000.f);
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����
	//(*lpCreateStruct).cx = 1880;    //���� ũ��
	//(*lpCreateStruct).cy = 730;     //���� ũ��

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// ����
	//cs.cx = 1880;    //���� ũ��
	//cs.cy = 730;     //���� ũ��

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame �޽��� ó����

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	SetMenu(NULL);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	RECT rect;
	GetWindowRect(&rect);
	SetWindowPos(nullptr, 0, 0, 1880, 830, SWP_NOMOVE);

	g_iWinX = WINCX;
	g_iWinY = WINCY;
	// 1280, 720
	m_tMainSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE);
	m_tMainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMainView), CSize(WINCX, WINCY), pContext);

	m_tMainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMenuForm), CSize(500, 820), pContext);

	g_hWnd = m_tMainSplitter.GetPane(0, 0)->GetSafeHwnd();

	m_tMainSplitter.GetPane(0, 0)->GetClientRect(&rect);

	m_pMainView = static_cast<CMainView*>(m_tMainSplitter.GetPane(0, 0));
	m_pMenuForm = static_cast<CMenuForm*>(m_tMainSplitter.GetPane(0, 1));
	if (FAILED(ReadyMainApp()))
		return FALSE;

	return TRUE;
}