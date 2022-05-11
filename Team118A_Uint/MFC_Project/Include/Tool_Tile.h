#pragma once
#ifndef __TOOL_TILE_H__
#define __TOOL_TILE_H__


#include "PopupTool_ObjectTree.h"
#include "afxwin.h"
// CTool_Tile 대화 상자입니다.

class CStartMark;
class CTool_Tile final: public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Tile)

public:
	CTool_Tile(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Tile(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Tile();
public:
	void	ToolTileUpdate(_float fDeltaTime);
private:
	virtual BOOL OnInitDialog();
private:
	void	Set_Tile();
	void	Load_World();

	void	Raycast_TileSelect();
	void	Raycast_Mascot();
	void	Reload_TileInfo();
	_vec3	Get_IndexTilePos(_int iTileIndex);

	_bool	Check_TileField();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MW_TOOL_TILE };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

private:
	_float m_fIntervalX = 1.f;
	_float m_fIntervalZ = 0.9f;

	_bool		m_bShow = FALSE;
	_int		m_iMaxArray = 56;
	_int		m_iMaxX = 8;
	_int		m_iMaxZ = 7;
	CCombatTile* m_arrayCombatTile[56];

	vector<CMascot*> m_vecMascot;
	CMascot*	m_pPickMascot = nullptr;

	map<_int, CWorldUI*> m_mapStartWorldUI;
private:

public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
	CListBox m_cListTile;
	CListBox m_cListWorld;
	CComboBox m_cComboTileOption;

public:
	afx_msg void OnLbnSelchangeMw0108ListTile();
	afx_msg void OnLbnSelchangeMw0108ListWorld();
	afx_msg void OnBnClickedMw0108ButtonBindWorld();
	afx_msg void OnBnClickedMw0108ButtonDeleteWorld();
	afx_msg void OnCbnSelchangeMw0109ComboTileOption();
private:
	CString m_strWorldCreaterPath;
	CListBox m_cListMascot;
public:
	afx_msg void OnLbnSelchangeMw0109ListMascot();
private:
	CComboBox m_cComboPlayType;
public:
	afx_msg void OnCbnSelchangeMw0109ComboPlayType();
	afx_msg void OnBnClickedMw0109ButtonSave();
	afx_msg void OnBnClickedMw0109ButtonLoad();
private:
	CString m_strLinkTileIndex;
public:
	afx_msg void OnEnChangeMw0110EditLinkTileIndex();
};

#endif // !__TOOL_WORLD_H__
