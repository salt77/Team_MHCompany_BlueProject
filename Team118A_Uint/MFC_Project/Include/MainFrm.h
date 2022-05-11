
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "MenuForm.h"
#include "MainView.h"
class CMainFrame : public CFrameWnd
{
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:
	CSplitterWnd	m_tMainSplitter;

	CMenuForm*		m_pMenuForm;
	CMainView*		m_pMainView;

// �۾��Դϴ�.
public:
	HRESULT ReadyMainApp();
	CGameObject*	Get_PickObject();

private:
	HRESULT ReadyGlobalResources();
	HRESULT ReadyDefaultSettings();

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
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


