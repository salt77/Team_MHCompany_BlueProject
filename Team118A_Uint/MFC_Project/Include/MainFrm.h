
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "MenuForm.h"
#include "MainView.h"
class CMainFrame : public CFrameWnd
{
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:
	CSplitterWnd	m_tMainSplitter;

	CMenuForm*		m_pMenuForm;
	CMainView*		m_pMainView;

// 작업입니다.
public:
	HRESULT ReadyMainApp();
	CGameObject*	Get_PickObject();

private:
	HRESULT ReadyGlobalResources();
	HRESULT ReadyDefaultSettings();

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

public:
	CManagement*		m_pManagement = nullptr;
	CGraphicDev*		m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


