
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원



#include "Engine_Define.h"
#include "Management.h"
#include "MainFrm.h"
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")  
#include <iostream>

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern _int g_iWinY;
extern _int g_iWinX;
extern int g_iSoundStack;
extern char g_szConsoleText[MAX_PATH];


// 사운드
#include <io.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


// 모든 트리를 순회하면서 Expand 해 줌
inline HTREEITEM ExpandAll_Tree(HTREEITEM hItem, CTreeCtrl& treeControl)
{
	if (NULL == hItem)
	{
		return nullptr;
	}

	if (treeControl.ItemHasChildren(hItem))
	{
		treeControl.Expand(hItem, TVE_EXPAND);
	}
	HTREEITEM hChild = treeControl.GetNextItem(hItem, TVGN_CHILD);
	if (NULL != hChild)
	{
		hChild = ExpandAll_Tree(hChild, treeControl);
	}

	HTREEITEM hNext = treeControl.GetNextItem(hItem, TVGN_NEXT);
	if (NULL != hNext)
	{
		hNext = ExpandAll_Tree(hNext, treeControl);
	}
	return nullptr;
}

// 모든 트리 부모를 순회하면서
// 최상위 부모의 이름부터 문자열에 더해줌
inline _int Combine_TreePath(CTreeCtrl & TC, HTREEITEM & item, wstring & string)
{
	_int iStack = 0;
	HTREEITEM hParent = TC.GetParentItem(item);
	if (nullptr == hParent)
	{
	}
	else
	{
		iStack++;
		iStack += Combine_TreePath(TC, hParent, string);
	}
	string.append(TC.GetItemText(item));

	return iStack;
}

inline void Printf_Console_Description()
{
	cout << "콘솔 설명서입니다. 콘솔창을 지우고 다시 쓸 때마다 이 설명서를 출력합니다." << endl;
	cout << "(작성자 : 민현)" << endl;
	cout << "[Shift] + [U] : 콘솔창 초기화 후 GameObject 상속받은 오브젝트 Debug_This_In_Console Method 실행" << endl;
	cout << "[Shift] + [D] : 하이얼아키 트리 업데이트" << endl;
	cout << "[Shift] + [M] : BGM OfOff" << endl;
}

// 메인 프레임을 캐스팅하여 반환한다.
inline CMainFrame* Get_MFC_MainFrame()
{
	CWnd* pWnd = AfxGetMainWnd();
	return static_cast<CMainFrame*>(pWnd);
}

// PopupTool의 트리를 반환한다.
inline CPopupTool_ObjectTree* Get_MFC_PopupObject_Tree()
{
	CWnd* pWnd = AfxGetMainWnd();
	
	return static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree;
}

// PopupTool의 m_pPickObject m_pPickObject를 반환한다.
inline CGameObject*	Get_PopupTool_PickObject()
{
	CWnd* pWnd = AfxGetMainWnd();
	return static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject;
}

// PopupTool의 m_pPickObject를 초기화해준다.
inline void	PopupTool_PickObject_Clear()
{
	CWnd* pWnd = AfxGetMainWnd();
	CGameObject* pObj = static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject;
	if (pObj)
	{
		static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;
	}
}
