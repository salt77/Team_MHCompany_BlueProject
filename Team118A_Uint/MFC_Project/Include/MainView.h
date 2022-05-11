#pragma once
#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "MenuForm.h"

static _bool		m_bBgmState = TRUE;
// CMainView ����
void CALLBACK GameLogic(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

class CMainView : public CView
{
	DECLARE_DYNCREATE(CMainView)

protected:
	CMainView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMainView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	CMenuForm*	 pMenuForm;

};


#endif // !__MAINVIEW_H__