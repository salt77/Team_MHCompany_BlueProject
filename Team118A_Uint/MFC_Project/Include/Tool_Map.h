#pragma once
#ifndef __TOOL_MAP_H__
#define __TOOL_MAP_H__

#include "afxwin.h"
#include "afxcmn.h"

#include "PopupTool_ObjectTree.h"			

class CStageMap;
class CObstacle_Combat;
class CObstacle_Cafe;
class CTool_Map : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Map)

public:
	CTool_Map(CWnd* pParent = NULL);
	CTool_Map(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);
	virtual ~CTool_Map();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_MAP };
#endif

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL					OnInitDialog();

public:
	afx_msg void					OnBnClicked_JH_Add_Map();
	afx_msg void					OnBnClicked_JH_Save_Map();
	afx_msg void					OnBnClicked_JH_Load_Map();
	afx_msg void					OnBnClicked_JH_Add_Obstacle();
	afx_msg void					OnBnClicked_JH_Save_Obstacle();
	afx_msg void					OnBnClicked_JH_Load_Obstacle();
	afx_msg void					OnBnClicked_JH_RayCast();

private:
	void							Loading();
	void							Load_Map_Path(_tchar* wPath, intptr_t lhandle, _finddata_t& findData);
	void							Load_Obstacle_Path(_tchar* wPath, intptr_t lhandle, _finddata_t& findData);
	HTREEITEM						InsertTree(HTREEITEM hParent, _tchar* wText, TV_INSERTSTRUCTW& ti, CTreeCtrl& treeCtrl);
	void							TotalMap();
	void							CreateTile(_int Index, wstring MapName);
	void							CreateWall_L(_int Index, wstring pWall_LName);
	void							CreateWall_R(_int Index, wstring pWall_RName);

protected:
	CPopupTool_ObjectTree*			m_pPopupTool = nullptr;

private:
	vector<pair<wstring, wstring>>	m_vecMapPathAndName;
	vector<pair<wstring, wstring>>	m_vecObstaclePathAndName;

private:
	CTreeCtrl						m_TreeMap;
	CTreeCtrl						m_TreeObstacle;
	CButton							m_CheckBox_RayCast;
	HTREEITEM						hMapItem;
	HTREEITEM						hObstacleItem;
	TV_INSERTSTRUCTW				TI_Map;
	TV_INSERTSTRUCTW				TI_Obstacle;

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	CStageMap*						m_pMap = nullptr;
	CStageMap*						m_pTile = nullptr;
	CStageMap*						m_pWall_L = nullptr;
	CStageMap*						m_pWall_R = nullptr;
	CObstacle_Cafe*					m_pObstacle_Cafe = nullptr;
	vector<CGameObject*>*			m_vecObstacle_Cafe;
};

#endif // !__TOOL_MAP_H__