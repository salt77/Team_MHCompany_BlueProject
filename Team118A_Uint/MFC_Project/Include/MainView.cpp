// MainView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "MainView.h"

#include "MainFrm.h"
#include "MenuForm.h"
#include "Scene_Manager.h"

// CMainView

IMPLEMENT_DYNCREATE(CMainView, CView)
LPDIRECT3DDEVICE9	m_pDevice = nullptr;

CMainView::CMainView()
{

}

CMainView::~CMainView()
{
}

BEGIN_MESSAGE_MAP(CMainView, CView)
END_MESSAGE_MAP()

// CMainView 그리기입니다.

void CMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMainView 진단입니다.

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView 메시지 처리기입니다.

// 
// 
void CMainView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetTimer(180, 1, GameLogic);

	CMainFrame* pMain = static_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	m_pDevice = pMain->m_pGraphicDev;
	RECT rcView{};
	GetClientRect(&rcView);

	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, g_iWinX + iGapX, g_iWinY + iGapY, SWP_NOMOVE);

	Printf_Console_Description();


	Management Ready_Timer(L"Timer");
	Management Ready_Frame(L"Frame180", 180.f);
}

void CALLBACK GameLogic(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	Management UpdateKeyManager(g_hWnd);


	if (Management Key_Down(KEY_F1))
	{
		Management Set_Render_Defferd();
	}

	if (m_pDevice)
	{
		Management Set_TimeDelta(L"Timer");
		float fFixedTimeDelta = Management Get_TimeDelta(L"Timer");

		if (Management IsPermit_Call(L"Frame180", fFixedTimeDelta))
		{
			CScene::Add_PlayTime(fFixedTimeDelta);
			Management Update_Scene(fFixedTimeDelta);
			Management LateUpdate_Scene(fFixedTimeDelta);

			Get_MFC_MainFrame()->m_pMenuForm->m_pTool_World->ToolWorldUpdate(fFixedTimeDelta);
			Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Tile->ToolTileUpdate(fFixedTimeDelta);

			if (Management Key_Pressing(KEY_Shift) && Management Key_Down(KEY_U))
			{
				CUtility::Print_Wstring_DMode(L"");
				Printf_Console_Description();

				Management Debugging_Console();
			}
			if (Management Key_Pressing(KEY_Shift) && Management Key_Down(KEY_D))
			{
				CUtility::Print_Wstring_DMode(L"");
				Printf_Console_Description();

				Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->Update_Tree();
				CUtility::Print_Wstring_DMode(L"하이얼아키 뷰 업데이트 성공");
			}

			D3DXCOLOR vColor = Management Get_LightBackBufferColor();

			Management Render_Begin(vColor);

			CManagement::GetInstance()->Render_Scene(m_pDevice);

			Management Render_End();

			//파티클 툴 렌더
			Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Particle->ViewRender(fFixedTimeDelta);
		}

		// 씬 전환 체크부
		CScene_Manager::GetInstance()->Set_Scene();

		// 프로그램 종료	
		if (Management Key_Pressing(KEY_Shift) && Management Key_Down(KEY_ESCAPE))
		{
			AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_EXIT, NULL);
		}
	}

}
