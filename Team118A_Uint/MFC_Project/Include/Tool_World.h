#pragma once
#ifndef __TOOL_WORLD_H__
#define __TOOL_WORLD_H__


#include "PopupTool_ObjectTree.h"
#include "afxwin.h"
// CTool_World 대화 상자입니다.

class CStartMark;
class CTool_World final: public CDialogEx
{
	DECLARE_DYNAMIC(CTool_World)

public:
	CTool_World(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_World(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_World();
public:
	void ToolWorldUpdate(_float fDeltaTime);


private:
	virtual BOOL OnInitDialog();

private:
	void LoadMonster();
	void LoadCharacter();
	void MonsterPoolReLoad();
	void GroupReLoad();
	void GroupObjectReLoad();
	void MonsterPoolObjectListReLoad();
	void FastKey();

	
	void DeleteAll();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_WORLD };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	T_MONSTER_INFO NameTranslate(wstring wstrName);

	void	MonsterRayCast();
	void	StartMarkRayCast();
	_bool	MapRayCast(_vec3* pOutPos);

	void	MonsterInfoUpdate(_int iSelectIndex);
	void	MonsterCheck();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
	CListBox m_cListMonster;
	CListBox m_cListMonsterPool;

private:
	vector<T_MONSTER_INFO>  m_vecMonsterPool;
	vector<vector<CMonster*>>		m_vecMonster;
	vector<vector<CMonster*>>		m_vecGroup;

	E_WORLD_MOD m_eWorldMod=E_WORLD_MOD::MOD_END;
	E_WORLD_TYPE m_eWorldType=E_WORLD_TYPE::TYPE_END;
public:
	afx_msg void OnBnClickedMw1222BtnAddMonsterPool();
	afx_msg void OnBnClickedMw1222BtnDeleteMonsterPool();
private:
	
public:
	afx_msg void OnBnClickedMw1222RadioListMonster();
	afx_msg void OnBnClickedMw1222RadioListCharacter();
private:
	CComboBox m_cComboMonsterType;
	CComboBox m_cComboAttackType;
	CComboBox m_cComboDefenceType;
	CComboBox m_cComboWeaponType;
public:
	afx_msg void OnLbnSelchangeMw1222ListMonsterPool();
private:
	CString m_strLevel;
	CString m_strDisplayName;
public:
	afx_msg void OnEnChangeMw1222EditDisplayName();
	afx_msg void OnEnChangeMw1222EditLevel();
	afx_msg void OnCbnSelchangeMw1222ComboTypeMonster();
	afx_msg void OnCbnSelchangeMw1222ComboTypeAttack();
	afx_msg void OnCbnSelchangeMw1222ComboTypeDefence();
	afx_msg void OnCbnSelchangeMw1222ComboTypeWeapon();
	afx_msg void OnBnClickedMw1222BtnSpawnMonster();
private:
	CString m_strColRadius;
	CString m_strScaleX;
	CString m_strScaleY;
	CString m_strScaleZ;

	CMonster* m_pPickObject = nullptr;
	CStartMark* m_pStartPickObject = nullptr;

	
public:
	afx_msg void OnEnChangeMw1223EditColRadius();
	afx_msg void OnEnChangeMw1223EditScaleX();
	afx_msg void OnEnChangeMw1223EditScaleY();
	afx_msg void OnEnChangeMw1223EditScaleZ();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedMw1223BtnSaveFieldInfo();
	afx_msg void OnBnClickedMw1223BtnLoadFieldInfo();
	afx_msg void OnBnClickedMw1222BtnAddGroup();
	afx_msg void OnBnClickedMw1222BtnDeleteGroup();
private:
	CListBox m_cListGroup;
	CString m_strPickObjectDisplayName;
public:
	afx_msg void OnBnClickedMw1226BtnPickObjectDelete();
	afx_msg void OnBnClickedMw1222BtnAddObject();
	afx_msg void OnBnClickedMw1222BtnDeleteObject();
private:
	CListBox m_cListGroupObject;
public:
	afx_msg void OnLbnSelchangeMw1222ListGroup();
private:
	CComboBox m_cComboWorldMod;
	CComboBox m_cComboWorldType;
public:
	afx_msg void OnBnClickedMw1227BtnSetStartPos();
	afx_msg void OnCbnSelchangeMw1227ComboWorldMod();
	afx_msg void OnCbnSelchangeMw1227ComboWorldType();
};

#endif // !__TOOL_WORLD_H__
