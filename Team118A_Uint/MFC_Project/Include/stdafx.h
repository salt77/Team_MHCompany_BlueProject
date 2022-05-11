
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����



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


// ����
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


// ��� Ʈ���� ��ȸ�ϸ鼭 Expand �� ��
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

// ��� Ʈ�� �θ� ��ȸ�ϸ鼭
// �ֻ��� �θ��� �̸����� ���ڿ��� ������
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
	cout << "�ܼ� �����Դϴ�. �ܼ�â�� ����� �ٽ� �� ������ �� ������ ����մϴ�." << endl;
	cout << "(�ۼ��� : ����)" << endl;
	cout << "[Shift] + [U] : �ܼ�â �ʱ�ȭ �� GameObject ��ӹ��� ������Ʈ Debug_This_In_Console Method ����" << endl;
	cout << "[Shift] + [D] : ���̾��Ű Ʈ�� ������Ʈ" << endl;
	cout << "[Shift] + [M] : BGM OfOff" << endl;
}

// ���� �������� ĳ�����Ͽ� ��ȯ�Ѵ�.
inline CMainFrame* Get_MFC_MainFrame()
{
	CWnd* pWnd = AfxGetMainWnd();
	return static_cast<CMainFrame*>(pWnd);
}

// PopupTool�� Ʈ���� ��ȯ�Ѵ�.
inline CPopupTool_ObjectTree* Get_MFC_PopupObject_Tree()
{
	CWnd* pWnd = AfxGetMainWnd();
	
	return static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree;
}

// PopupTool�� m_pPickObject m_pPickObject�� ��ȯ�Ѵ�.
inline CGameObject*	Get_PopupTool_PickObject()
{
	CWnd* pWnd = AfxGetMainWnd();
	return static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject;
}

// PopupTool�� m_pPickObject�� �ʱ�ȭ���ش�.
inline void	PopupTool_PickObject_Clear()
{
	CWnd* pWnd = AfxGetMainWnd();
	CGameObject* pObj = static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject;
	if (pObj)
	{
		static_cast<CMainFrame*>(pWnd)->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;
	}
}
