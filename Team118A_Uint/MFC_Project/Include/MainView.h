#pragma once
#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include "MenuForm.h"

static _bool		m_bBgmState = TRUE;
// CMainView 보기
void CALLBACK GameLogic(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime);

class CMainView : public CView
{
	DECLARE_DYNCREATE(CMainView)

protected:
	CMainView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMainView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
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