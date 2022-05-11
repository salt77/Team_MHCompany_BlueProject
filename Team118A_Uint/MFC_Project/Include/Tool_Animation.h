#pragma once
#ifndef __TOOL_ANIMATION_H__
#define __TOOL_ANIMATION_H__

#include "PopupTool_ObjectTree.h"
#include "DMesh_Test.h"
#include "afxwin.h"
#include "afxcmn.h"
// CTool_Animation 대화 상자입니다.

class CTool_Animation : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Animation)

public:
	CTool_Animation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Animation(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Animation();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_ANIMATION };
#endif

protected:
	_uint			TIMERID = 5342;
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	
	
public:
	_bool	NullCheck_DMesh();
	void	Update_DMesh_ListBox();
	void	Update_DMesh_AnimationListBox();
	void	OnTimer_Frame_Time();


	afx_msg void	DClick_DMesh_ListBox();
	afx_msg void	DClick_AnimationList();
	afx_msg void	Button_Animation_Play_Once();
	afx_msg void	Button_Animation_Play_Auto();
	afx_msg void	Slider_DragAndClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	Button_Animation_CamMoveToFront();
	afx_msg void	Button_Animation_Stop();
	
public:		// 애니메이션 액션 트리
	CTreeCtrl			m_tAnimationTree;
	HTREEITEM			hParentItem;
	HTREEITEM			hFocusItem;
	HTREEITEM			hLastFocusItem;
	TV_INSERTSTRUCTW	TI_Anim;
	
public:		// MFC 변수 모음
	CListBox		m_ListBox_DMeshes;
	CListBox		m_ListBox_Animation_List;
	CEdit			m_Edit_Animation_Time;
	CEdit			m_Edit_Animation_Frame;
	CString			m_str_Animation_Time;
	CString			m_str_Animation_Frame;
	CSliderCtrl		m_Slider_AnimationRatio;


public:		// 사용자 정의 변수 모음
	vector<pair<wstring, wstring>>	m_vecKeyNameValueFullPath;
	CDMesh_Test*			m_pDMesh = nullptr;

	_float					m_fDeltaTime = 1.f / 60.f;
	_bool					m_bUpdateAnimList = TRUE;
	_bool					m_bAnimSliderChange = FALSE;
	_bool					m_bFirstAnimSet = TRUE;
	E_AnimationPlayType		m_eAnimPlayType = E_AnimationPlayType::Stop;
	_bool					m_bCharacterList = TRUE;




public:		// 트리거용 모음
	CListBox m_ListBox_CurAnimTrg;
	CListBox m_ListBox_CanAddTrg;
	_int		m_iLastPickAnimIndex = ANIM_ERROR;
	
	void	Clear_TrgListBox();
	void	Update_AnimTrgListBox(_int iAnimIndex);
	void	Update_ActionTree();
	afx_msg void Button_Add_Current_AnimationFrame();
	afx_msg void Button_AddAction();
	afx_msg void Click_AnimAction_Tree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Button_Start_ChooseAction();
	afx_msg void Button_Load_Anim_ActNTrg();
	afx_msg void Button_Save_Anim_ActNTrg();

private:
	wstring wstrLastPickActionName;
	CButton m_Radio_Character;
	CButton m_CheckBox_CamLoop;
public:
	afx_msg void Button_CurTriggerList();
	afx_msg void Click_Character_Radio();
	afx_msg void Click_Monster_Radio();
};

#endif // !__TOOL_ANIMATION_H__