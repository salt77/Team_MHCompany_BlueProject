#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#ifndef __POPUPTOOL_OBJECTTOOL_H__
#define __POPUPTOOL_OBJECTTOOL_H__

// CPopupTool_ObjectTree 대화 상자입니다.

class CPopupTool_ObjectTree : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupTool_ObjectTree)

public:
	CPopupTool_ObjectTree(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopupTool_ObjectTree();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_POPUPTOOL_OBJECTTREE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void	OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	void			Update_Tree();

	// 자식도 트리에 넣기
	HTREEITEM		Add_ChildInTree(HTREEITEM hItem, CTreeCtrl& treeCtrl, CUI_Object* pUi, _int iDepth);
	void			Add_ListItemAndTree(HTREEITEM& hItem, CGameObject* pObject);
	void			Clear_ListItemAndTree();
	CGameObject*	Get_PickObject() { return m_pPickObject; }

	// 클릭, 더블 클릭
	afx_msg void	Click_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	DbClick_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	ObjectTree_Button_Update();

	// 콘솔 초기화 버튼
	afx_msg void	Button_Console_Clear();

	// 팝업창 띄우기
	afx_msg void	Button_Show_Popup_Texture();

	// 오브젝트 삭제
	afx_msg void	Button_SelectObjectSetDead();

	// 
public:
	_uint				TIMERID = 1000;

	// 오브젝트 트리를 컨트롤하기 위한 ITEM 과 구조체
	TV_INSERTSTRUCTW	TI_Obj;
	CTreeCtrl			m_tTreeCtrl;
	HTREEITEM			hRootLayer[(_int)E_LAYERTAG::TAG_END];
	HTREEITEM			hLastTreeItem;
	HTREEITEM			hTempItem;

	// Tree를 초기화 할 때마다 모든 오브젝트를 순회하면서 HTREEITEM과 오브젝트를 Map에 넣고 
	// 현재 선택된 오브젝트를 내보내줄 때 사용
	map<HTREEITEM, CGameObject*>	m_mapItemObject;
	CGameObject*					m_pPickObject = nullptr;


private:
	CString m_Edit_Position_X;
	CString m_Edit_Position_Y;
	CString m_Edit_Position_Z;
	CString m_Edit_Rotation_X;
	CString m_Edit_Rotation_Y;
	CString m_Edit_Rotation_Z;
	CString m_Edit_Scale_X;
	CString m_Edit_Scale_Y;
	CString m_Edit_Scale_Z;
	CEdit	m_Control_Position_X;
	CEdit	m_Control_Position_Y;
	CEdit	m_Control_Position_Z;
	CEdit	m_Control_Rotation_X;
	CEdit	m_Control_Rotation_Y;
	CEdit	m_Control_Rotation_Z;
	CEdit	m_Control_Scale_X;
	CEdit	m_Control_Scale_Y;
	CEdit	m_Control_Scale_Z;

public:
	_bool	bUpdateNextFrame = FALSE;
};

#endif // !__POPUPTOOL_OBJECTTOOL_H__