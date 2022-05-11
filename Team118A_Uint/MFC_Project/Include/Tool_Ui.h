#pragma once
#include "afxcolorbutton.h"
#ifndef __TOOL_UI_H__
#define __TOOL_UI_H__

#include "PopupTool_ObjectTree.h"
#include "afxwin.h"
#include "afxbutton.h"
#include "afxcmn.h"
// CTool_Ui 대화 상자입니다.


class CTool_Ui : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Ui)

private:	// COLORREF To Float
	_float Ref2Float(COLORREF rgb, E_RGB eType)
	{
		int nBitmask = 0;
		int nResult = 0;
		switch (eType)
		{
		case E_RGB::RGB_R:
		{
			nBitmask = 0x0000ff;  //255
			nResult = rgb & nBitmask;
			break;
		}
		case E_RGB::RGB_G:
		{
			nBitmask = 0x00ff00;  //255
			nResult = (rgb & nBitmask) >> 8;
			break;
		}
		case E_RGB::RGB_B:
		{
			nBitmask = 0xff0000;  //255
			nResult = (rgb & nBitmask) >> 16;
			break;
		}
		}
		float fResult = nResult / (_float)255;
		return fResult;
	}

public:
	CTool_Ui(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Ui(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Ui();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_UI };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL	OnInitDialog();
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnShowWindow(BOOL bShow, UINT nStatus);
	CUI_Object*		Get_PickObject();

public:
	void	UpdateFor_PivotAndAnchor();
	void	UpdateMFC_Position(CUI_Object* pUi);

	void	UpdateFor_ScrollView(CGameObject* pPickObject);

	CUI_Object* Is_UiObject();

public:		// Pivot Anchor 버튼 Click
	afx_msg void	Clicked_PivotAnchor_TL();
	afx_msg void	Clicked_PivotAnchor_TC();
	afx_msg void	Clicked_PivotAnchor_TR();
	afx_msg void	Clicked_PivotAnchor_CL();
	afx_msg void	Clicked_PivotAnchor_C();
	afx_msg void	Clicked_PivotAnchor_CR();
	afx_msg void	Clicked_PivotAnchor_BL();
	afx_msg void	Clicked_PivotAnchor_BC();
	afx_msg void	Clicked_PivotAnchor_BR();

	afx_msg void	Click_Bold_Button();
	afx_msg void	Button_Create_Ui();
	afx_msg void	Button_Save_Prefab();
	afx_msg void	Button_Load_Prefab();
	afx_msg void	Button_Save_Ui_Layer();
	afx_msg void	Button_Load_Ui_Layer();
	afx_msg void	Button_Event_Parent();
	afx_msg void	Button_Event_Child();
	afx_msg void	Button_Event_Clear();

	afx_msg void	Spin_Click_ChangeIndex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	Button_AddEvent();

public:
	CMFCColorButton		m_MH_mfcColorButton;
	CString				m_GroupBox_PivotAnchor;
	CString				m_StaticText_PivotAnchor_State;
	CString				m_Edit_TextBox;
	CString				m_Edit_Ui_Position_X;
	CString				m_Edit_Ui_Position_Y;
	CString				m_Edit_Ui_Position_Z;
	CString				m_Edit_Ui_Size_X;
	CString				m_Edit_Ui_Size_Y;
	CString				m_Edit_Ui_SortingLayer;
	CString				m_Edit_Ui_OrderInLayer;
	CString				m_Edit_Ui_Text_Position_X;
	CString				m_Edit_Ui_Text_Position_Y;
	CSliderCtrl			m_Slider_TextAlpha;
	CButton				m_CheckBox_FontBold;

public:
	CString m_Edit_Texure_ProtoTag;
	CComboBox m_ComboBox_CreateType;

public:		// ScrollView Option
	// SW_HIDE, SW_SHOW 용
	CStatic		m_ScrollView_Static_Option;
	CStatic		m_ScrollView_Static_Spacing;
	CStatic		m_ScrollView_Static_Child_X;
	CStatic		m_ScrollView_Static_Child_Y;
	CStatic		m_ScrollView_Static_Padding;
	CStatic		m_ScrollView_Static_Spacing_X;
	CStatic		m_ScrollView_Static_Spacing_Y;
	CEdit		m_ScrollView_Edit_Child_Spacing_X;
	CEdit		m_ScrollView_Edit_Child_Spacing_Y;
	CButton		m_CheckBox_ChildSizeUse;
	CButton		m_CheckBox_ClickAnimation;
	CButton		m_CheckBox_OnEnableAnimation;
	CButton		m_CheckBox_ChildClickCheck;
	CButton		m_CheckBox_VerticalScroll;
	CButton		m_CheckBox_HorizontalScroll;
	CButton		m_CheckBox_ChildTo;
	CComboBox	m_ScrollView_ComboBox_Geometry;

	CStatic m_ScrollView_Static_ChildSize;
	CComboBox m_ScrollView_ComboBox_Padding;
	CComboBox m_Combo_FontType;
	CComboBox m_Combo_EaseType;
	CComboBox m_Combo_AnimType;
	CEdit m_ScrollView_Edit_Padding;
	CEdit m_ScrollView_Edit_Child_SizeX;
	CEdit m_ScrollView_Edit_Child_SizeY;

	CComboBox m_ComboBox_EventType;
	CListBox m_ListBox_EventList_Changed;
	CMFCColorButton m_MFCColorButton_ImageColor;
	CSliderCtrl m_Slider_ImageAlpha;
	CListBox m_ListBox_EventLists;

	// Edit 용
	CString m_Edit_ChildSize_X;
	CString m_Edit_ChildSize_Y;
	CString m_Edit_Padding;
	CString m_Edit_Child_Spacing_X;
	CString m_Edit_Child_Spacing_Y;
	CString m_Edit_UV_Min_X;
	CString m_Edit_UV_Min_Y;
	CString m_Edit_UV_Max_X;
	CString m_Edit_UV_Max_Y;
	CString m_Edit_Animation_ProgMax;
	CString m_Edit_Animation_Int1;
	CString m_Edit_Animation_Int2;
	CString m_Edit_Animation_Float1;
	CString m_Edit_Animation_Float2;
	CString m_Edit_NameTag;
	CString m_Edit_EventNumber;



public:
	_uint			TIMERID = 61;
	_int			m_iRangeMax = 255;
	_bool			m_b_TrueIsPivot_FalseIsAnchor	= TRUE;
	_bool			m_b_TrueIsImage_FalseIsText		= TRUE;
	CGameObject**	m_ppPickedObject = nullptr;

	CButton m_CheckBox_MaskOnOff;
	CComboBox m_ComboBox_NumberFontType;
	CString m_Edit_FontSize;
	CButton m_CheckBox_Italic_2;
	CString m_Edit_Align_Text;
	afx_msg void Button_TextAlign_Left();
	afx_msg void Button_TextAlign_Center();
	afx_msg void Button_TextAlign_Right();
};

#endif // !__TOOL_UI_H__