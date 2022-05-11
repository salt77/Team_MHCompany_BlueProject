#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#ifndef __POPUPTOOL_OBJECTTOOL_H__
#define __POPUPTOOL_OBJECTTOOL_H__

// CPopupTool_ObjectTree ��ȭ �����Դϴ�.

class CPopupTool_ObjectTree : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupTool_ObjectTree)

public:
	CPopupTool_ObjectTree(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopupTool_ObjectTree();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_POPUPTOOL_OBJECTTREE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void	OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	void			Update_Tree();

	// �ڽĵ� Ʈ���� �ֱ�
	HTREEITEM		Add_ChildInTree(HTREEITEM hItem, CTreeCtrl& treeCtrl, CUI_Object* pUi, _int iDepth);
	void			Add_ListItemAndTree(HTREEITEM& hItem, CGameObject* pObject);
	void			Clear_ListItemAndTree();
	CGameObject*	Get_PickObject() { return m_pPickObject; }

	// Ŭ��, ���� Ŭ��
	afx_msg void	Click_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	DbClick_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	ObjectTree_Button_Update();

	// �ܼ� �ʱ�ȭ ��ư
	afx_msg void	Button_Console_Clear();

	// �˾�â ����
	afx_msg void	Button_Show_Popup_Texture();

	// ������Ʈ ����
	afx_msg void	Button_SelectObjectSetDead();

	// 
public:
	_uint				TIMERID = 1000;

	// ������Ʈ Ʈ���� ��Ʈ���ϱ� ���� ITEM �� ����ü
	TV_INSERTSTRUCTW	TI_Obj;
	CTreeCtrl			m_tTreeCtrl;
	HTREEITEM			hRootLayer[(_int)E_LAYERTAG::TAG_END];
	HTREEITEM			hLastTreeItem;
	HTREEITEM			hTempItem;

	// Tree�� �ʱ�ȭ �� ������ ��� ������Ʈ�� ��ȸ�ϸ鼭 HTREEITEM�� ������Ʈ�� Map�� �ְ� 
	// ���� ���õ� ������Ʈ�� �������� �� ���
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