#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Ui.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Dialog_Manager.h"
IMPLEMENT_DYNAMIC(CTool_Ui, CDialogEx)

CTool_Ui::CTool_Ui(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_GroupBox_PivotAnchor(_T(""))
	, m_StaticText_PivotAnchor_State(_T(""))
	, m_Edit_TextBox(_T(""))
	, m_Edit_Ui_Position_X(_T(""))
	, m_Edit_Ui_Position_Y(_T(""))
	, m_Edit_Ui_Position_Z(_T(""))
	, m_Edit_Ui_Size_X(_T(""))
	, m_Edit_Ui_Size_Y(_T(""))
	, m_Edit_Ui_SortingLayer(_T(""))
	, m_Edit_Ui_OrderInLayer(_T(""))
	, m_Edit_Ui_Text_Position_X(_T(""))
	, m_Edit_Ui_Text_Position_Y(_T(""))
	, m_Edit_Texure_ProtoTag(_T(""))
	, m_Edit_ChildSize_X(_T(""))
	, m_Edit_ChildSize_Y(_T(""))
	, m_Edit_Padding(_T(""))
	, m_Edit_Child_Spacing_X(_T(""))
	, m_Edit_Child_Spacing_Y(_T(""))
	, m_Edit_EventNumber(_T(""))
	, m_Edit_UV_Min_X(_T(""))
	, m_Edit_UV_Min_Y(_T(""))
	, m_Edit_UV_Max_X(_T(""))
	, m_Edit_UV_Max_Y(_T(""))
	, m_Edit_Animation_ProgMax(_T(""))
	, m_Edit_Animation_Int1(_T(""))
	, m_Edit_Animation_Int2(_T(""))
	, m_Edit_Animation_Float1(_T(""))
	, m_Edit_Animation_Float2(_T(""))
	, m_Edit_NameTag(_T(""))
	, m_Edit_FontSize(_T(""))
	, m_Edit_Align_Text(_T(""))
{
}

CTool_Ui::CTool_Ui(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Ui::~CTool_Ui()
{
}

void CTool_Ui::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MH_MFC_COLORBUTTON, m_MH_mfcColorButton);
	DDX_Text(pDX, IDC_MH_UITOOL_STATICBOX_PIVOT_ANCHOR, m_GroupBox_PivotAnchor);
	DDX_Text(pDX, IDC_STATICTEXT_PivotAnchor_State, m_StaticText_PivotAnchor_State);
	DDX_Text(pDX, IDC_MH_UITOOL_Edit_TextBox, m_Edit_TextBox);
	DDX_Text(pDX, IDC_MH_UI_POSITION_X, m_Edit_Ui_Position_X);
	DDV_MaxChars(pDX, m_Edit_Ui_Position_X, 9);
	DDX_Text(pDX, IDC_MH_UI_POSITION_Y, m_Edit_Ui_Position_Y);
	DDV_MaxChars(pDX, m_Edit_Ui_Position_Y, 9);
	DDX_Text(pDX, IDC_MH_UI_POSITION_Z, m_Edit_Ui_Position_Z);
	DDV_MaxChars(pDX, m_Edit_Ui_Position_Z, 9);
	DDX_Text(pDX, IDC_MH_UI_SIZE_X, m_Edit_Ui_Size_X);
	DDV_MaxChars(pDX, m_Edit_Ui_Size_X, 9);
	DDX_Text(pDX, IDC_MH_UI_SIZE_Y, m_Edit_Ui_Size_Y);
	DDV_MaxChars(pDX, m_Edit_Ui_Size_Y, 9);
	DDX_Text(pDX, IDC_MH_UI_SORTING_LAYER, m_Edit_Ui_SortingLayer);
	DDV_MaxChars(pDX, m_Edit_Ui_SortingLayer, 4);
	DDX_Text(pDX, IDC_MH_UI_ORDERINLAYER, m_Edit_Ui_OrderInLayer);
	DDV_MaxChars(pDX, m_Edit_Ui_OrderInLayer, 4);
	DDX_Text(pDX, IDC_MH_UI_TEXTPOSITION_X, m_Edit_Ui_Text_Position_X);
	DDV_MaxChars(pDX, m_Edit_Ui_Text_Position_X, 9);
	DDX_Text(pDX, IDC_MH_UI_TEXTPOSITION_Y, m_Edit_Ui_Text_Position_Y);
	DDV_MaxChars(pDX, m_Edit_Ui_Text_Position_Y, 9);
	DDX_Control(pDX, IDC_MH_SLIDER_TEXT_COLOR_ALPHA, m_Slider_TextAlpha);
	DDX_Control(pDX, IDC_MH_UITOOL_BOLD, m_CheckBox_FontBold);
	DDX_Text(pDX, IDC_MH_EDIT_UI_IMAGE, m_Edit_Texure_ProtoTag);
	DDX_Control(pDX, IDC_MH_UITOOL_COMBO_CREATE_UI_TYPE, m_ComboBox_CreateType);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_OPTION, m_ScrollView_Static_Option);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_CHILD_SIZE, m_ScrollView_Static_ChildSize);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_SPACING, m_ScrollView_Static_Spacing);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_POSITION_X, m_ScrollView_Static_Child_X);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_POSITION_Y, m_ScrollView_Static_Child_Y);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_PADDING, m_ScrollView_Static_Padding);
	DDX_Control(pDX, IDC_MH_UITOOL_CHILDSIZE_X, m_ScrollView_Edit_Child_SizeX);
	DDX_Control(pDX, IDC_MH_UITOOL_CHILDSIZE_Y, m_ScrollView_Edit_Child_SizeY);
	DDX_Control(pDX, IDC_MH_UITOOL_COMBO_PADDING_DIR, m_ScrollView_ComboBox_Padding);
	DDX_Control(pDX, IDC_MH_UITOOL_PADDING_AMOUNT, m_ScrollView_Edit_Padding);
	DDX_Text(pDX, IDC_MH_UITOOL_CHILDSIZE_X, m_Edit_ChildSize_X);
	DDV_MaxChars(pDX, m_Edit_ChildSize_X, 8);
	DDX_Text(pDX, IDC_MH_UITOOL_CHILDSIZE_Y, m_Edit_ChildSize_Y);
	DDV_MaxChars(pDX, m_Edit_ChildSize_Y, 8);
	DDX_Text(pDX, IDC_MH_UITOOL_PADDING_AMOUNT, m_Edit_Padding);
	DDV_MaxChars(pDX, m_Edit_Padding, 8);
	DDX_Control(pDX, IDC_MH_UITOOL_SPACING_Y, m_ScrollView_Edit_Child_Spacing_Y);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_SPACING_X, m_ScrollView_Static_Spacing_X);
	DDX_Control(pDX, IDC_MH_UITOOL_STATIC_SCROLLVIEW_SPACING_Y, m_ScrollView_Static_Spacing_Y);
	DDX_Control(pDX, IDC_MH_UITOOL_SPACING_X, m_ScrollView_Edit_Child_Spacing_X);
	DDX_Text(pDX, IDC_MH_UITOOL_SPACING_X, m_Edit_Child_Spacing_X);
	DDV_MaxChars(pDX, m_Edit_Child_Spacing_X, 8);
	DDX_Text(pDX, IDC_MH_UITOOL_SPACING_Y, m_Edit_Child_Spacing_Y);
	DDV_MaxChars(pDX, m_Edit_Child_Spacing_Y, 8);
	DDX_Control(pDX, IDC_MH_UITOOL_CHILDSIZE_USE, m_CheckBox_ChildSizeUse);
	DDX_Text(pDX, IDC_MH_1216_EDIT_EVENTNUMBER, m_Edit_EventNumber);
	DDV_MaxChars(pDX, m_Edit_EventNumber, 2);
	DDX_Control(pDX, IDC_MH_1216_COMBO_EVENTTYPE, m_ComboBox_EventType);
	DDX_Control(pDX, IDC_MH_UITOOL_COMBO_GEOMETRY, m_ScrollView_ComboBox_Geometry);
	DDX_Control(pDX, IDC_MH_1216_LISTBOX_EVENTVECTOR_CHANGED, m_ListBox_EventList_Changed);
	DDX_Control(pDX, IDC_MH_COMBO_FONTTYPE, m_Combo_FontType);
	DDX_Text(pDX, IDC_MH_1220_EDIT_UV_MIN_X, m_Edit_UV_Min_X);
	DDV_MaxChars(pDX, m_Edit_UV_Min_X, 6);
	DDX_Text(pDX, IDC_MH_1220_EDIT_UV_MIN_Y, m_Edit_UV_Min_Y);
	DDV_MaxChars(pDX, m_Edit_UV_Min_Y, 6);
	DDX_Text(pDX, IDC_MH_1220_EDIT_UV_MAX_X, m_Edit_UV_Max_X);
	DDV_MaxChars(pDX, m_Edit_UV_Max_X, 6);
	DDX_Text(pDX, IDC_MH_1220_EDIT_UV_MAX_Y, m_Edit_UV_Max_Y);
	DDV_MaxChars(pDX, m_Edit_UV_Max_Y, 6);
	DDX_Control(pDX, IDC_MH_1220_COMBO_EASETYPE, m_Combo_EaseType);
	DDX_Control(pDX, IDC_MH_1221_COMBO_ANIMATION_TYPE, m_Combo_AnimType);
	DDX_Text(pDX, IDC_MH_1221_EDIT_PROGRESS_MAXTIME, m_Edit_Animation_ProgMax);
	DDX_Text(pDX, IDC_MH_1221_EDIT_PARAM_INT_1, m_Edit_Animation_Int1);
	DDV_MaxChars(pDX, m_Edit_Animation_Int1, 8);
	DDX_Text(pDX, IDC_MH_1221_EDIT_PARAM_INT_2, m_Edit_Animation_Int2);
	DDX_Text(pDX, IDC_MH_1221_EDIT_PARAM_FLOAT_1, m_Edit_Animation_Float1);
	DDX_Text(pDX, IDC_MH_1221_EDIT_PARAM_FLOAT_2, m_Edit_Animation_Float2);
	DDX_Control(pDX, IDC_MH_1221_CHECK_CHILDTO, m_CheckBox_ChildTo);
	DDX_Control(pDX, IDC_MH_1221_MFCCOLORBUTTON_IMAGECOLOR, m_MFCColorButton_ImageColor);
	DDX_Control(pDX, IDC_MH_1221_SLIDER_IMAGECOLOR_ALPHA, m_Slider_ImageAlpha);
	DDX_Control(pDX, IDC_MH_1221_CHECK_CHILD_CLICKCHECK, m_CheckBox_ChildClickCheck);
	DDX_Text(pDX, IDC_MH_1221_EDIT_UI_NAMETAG, m_Edit_NameTag);
	DDX_Control(pDX, IDC_MH_1222_CHECK_CLICK_ANIMATION, m_CheckBox_ClickAnimation);
	DDX_Control(pDX, IDC_MH_1222_CHECK_ENABLEANIMATION, m_CheckBox_OnEnableAnimation);
	DDX_Control(pDX, IDC_MH_1222_LIST_EVENTLISTS, m_ListBox_EventLists);
	DDX_Control(pDX, IDC_MH_1223_CHECK_VERTICAL_SCROLL, m_CheckBox_VerticalScroll);
	DDX_Control(pDX, IDC_MH_1223_CHECK_HORIZONTAL_SCROLL, m_CheckBox_HorizontalScroll);
	DDX_Control(pDX, IDC_MH_1223_CHECK_MASKONOFF, m_CheckBox_MaskOnOff);
	DDX_Control(pDX, IDC_MH_1227_COMBO_NUMBER_FONT_TYPE, m_ComboBox_NumberFontType);
	DDX_Text(pDX, IDC_MH_1230_EDIT_FONTSIZE, m_Edit_FontSize);
	DDX_Control(pDX, IDC_MH_1230_CHECKBOX_ITALIC2, m_CheckBox_Italic_2);
	DDX_Text(pDX, IDC_MH_0102_STATIC_ALIGN_TEXT, m_Edit_Align_Text);
}


BEGIN_MESSAGE_MAP(CTool_Ui, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_TL, &CTool_Ui::Clicked_PivotAnchor_TL)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_TC, &CTool_Ui::Clicked_PivotAnchor_TC)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_TR, &CTool_Ui::Clicked_PivotAnchor_TR)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_CL, &CTool_Ui::Clicked_PivotAnchor_CL)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_C, &CTool_Ui::Clicked_PivotAnchor_C)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_CR, &CTool_Ui::Clicked_PivotAnchor_CR)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_BL, &CTool_Ui::Clicked_PivotAnchor_BL)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_BC, &CTool_Ui::Clicked_PivotAnchor_BC)
	ON_BN_CLICKED(IDC_MH_Ui_MFCBUTTON_Pivot_Anchor_BR, &CTool_Ui::Clicked_PivotAnchor_BR)
	ON_BN_CLICKED(IDC_MH_UITOOL_BOLD, &CTool_Ui::Click_Bold_Button)
	ON_BN_CLICKED(IDC_MH_UITOOL_BUTTON_CREATE, &CTool_Ui::Button_Create_Ui)
	ON_BN_CLICKED(IDC_MH_UITOOL_BUTTON_SAVE_PREFAB, &CTool_Ui::Button_Save_Prefab)
	ON_BN_CLICKED(IDC_MH_UITOOL_BUTTON_LOAD_PREFAB, &CTool_Ui::Button_Load_Prefab)
	ON_BN_CLICKED(IDC_MH_UITOOL_BUTTON_SAVE_UI_LAYER, &CTool_Ui::Button_Save_Ui_Layer)
	ON_BN_CLICKED(IDC_MH_UITOOL_BUTTON_LOAD_UI_LAYER, &CTool_Ui::Button_Load_Ui_Layer)
	ON_BN_CLICKED(IDC_MH_1216_EVENT_BUTTON_PARENT, &CTool_Ui::Button_Event_Parent)
	ON_BN_CLICKED(IDC_MH_1216_EVENT_BUTTON_CHILD, &CTool_Ui::Button_Event_Child)
	ON_BN_CLICKED(IDC_MH_1216_EVENT_BUTTON_CLEAR, &CTool_Ui::Button_Event_Clear)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MH_1221_SPIN_CHANGE_INDEX, &CTool_Ui::Spin_Click_ChangeIndex)
	ON_BN_CLICKED(IDC_MH_1221_BUTTON_ADD_EVENT, &CTool_Ui::Button_AddEvent)
	ON_BN_CLICKED(IDC_MH_0102_BUTTON_ALIGN_LEFT, &CTool_Ui::Button_TextAlign_Left)
	ON_BN_CLICKED(IDC_MH_0102_BUTTON_ALIGN_CENTER, &CTool_Ui::Button_TextAlign_Center)
	ON_BN_CLICKED(IDC_MH_0102_BUTTON_ALIGN_RIGHT, &CTool_Ui::Button_TextAlign_Right)
END_MESSAGE_MAP()

BOOL CTool_Ui::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ppPickedObject = &Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject;
	UpdateData(TRUE);
	m_ComboBox_CreateType.SetCurSel(0);
	m_ScrollView_ComboBox_Geometry.SetCurSel(0);
	m_Slider_ImageAlpha.SetRange(0, m_iRangeMax);
	m_Slider_TextAlpha.SetRange(0, m_iRangeMax);
	UpdateData(FALSE);
	return TRUE;
}


HBRUSH CTool_Ui::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CTool_Ui::OnShowWindow(BOOL bShow, UINT nStatus)		/////////////////////	Ready
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetTimer(TIMERID, (_uint)(1.f / 30.f), NULL);

		UpdateData(TRUE);
		//m_Button_PivotAnchor_LT.SetImage(IDR_MAINFRAME);

		UpdateData(FALSE);
	}
	else
	{
		KillTimer(TIMERID);
	}
}

CUI_Object * CTool_Ui::Get_PickObject()
{
	UpdateData(TRUE);
	if (nullptr == *m_ppPickedObject)
		return nullptr;

	if (nullptr == dynamic_cast<CUI_Object*>((*m_ppPickedObject)))
	{
		cout << endl;
		cout << "선택된 오브젝트가 UI가 아닙니다." << endl;
		return nullptr;
	}
	CUI_Object* pUi = static_cast<CUI_Object*>((*m_ppPickedObject));

	// Transform Pivot, Anchor
	_tchar szText[MAX_PATH]{};
	lstrcpy(szText, L"Pivot - ");
	switch (pUi->Get_UiTransform().eTransformPivot)
	{
	case Engine::E_Pivot::Pivot_TL:				lstrcat(szText, L"TL");				break;
	case Engine::E_Pivot::Pivot_TC:				lstrcat(szText, L"TC");				break;
	case Engine::E_Pivot::Pivot_TR:				lstrcat(szText, L"TR");				break;
	case Engine::E_Pivot::Pivot_CL:				lstrcat(szText, L"CL");				break;
	case Engine::E_Pivot::Pivot_Center:			lstrcat(szText, L"C");				break;
	case Engine::E_Pivot::Pivot_CR:				lstrcat(szText, L"CR");				break;
	case Engine::E_Pivot::Pivot_BL:				lstrcat(szText, L"BL");				break;
	case Engine::E_Pivot::Pivot_BC:				lstrcat(szText, L"BC");				break;
	case Engine::E_Pivot::Pivot_BR:				lstrcat(szText, L"BR");				break;
	default: assert(FALSE);  break;
	}

	// 텍스트
	m_StaticText_PivotAnchor_State.SetString(szText);
	m_Edit_TextBox.SetString(pUi->Get_Text().c_str());

	// Transform
	m_Edit_Ui_Position_X.SetString(to_wstring(pUi->Get_UiTransform().iPosX).c_str());
	m_Edit_Ui_Position_Y.SetString(to_wstring(pUi->Get_UiTransform().iPosY).c_str());
	m_Edit_Ui_Position_Z.SetString(to_wstring(pUi->Get_UiTransform().iPosZ).c_str());

	m_Edit_Ui_Size_X.SetString(to_wstring(pUi->Get_UiTransform().iSizeX).c_str());
	m_Edit_Ui_Size_Y.SetString(to_wstring(pUi->Get_UiTransform().iSizeY).c_str());

	m_Edit_Ui_SortingLayer.SetString(to_wstring(pUi->Get_UiTransform().iSorting_Layer).c_str());
	m_Edit_Ui_OrderInLayer.SetString(to_wstring(pUi->Get_UiTransform().iOrder_In_Layer).c_str());

	// TextOption
	m_Edit_Ui_Text_Position_X.SetString(to_wstring(pUi->Get_TextOption().iTextPosX).c_str());
	m_Edit_Ui_Text_Position_Y.SetString(to_wstring(pUi->Get_TextOption().iTextPosY).c_str());

	// Font
	m_CheckBox_FontBold.SetCheck(pUi->Get_TextOption().bBold);
	m_CheckBox_ChildTo.SetCheck(pUi->Get_UiAnimation().bMyChildFollowMyAnim);
	_int iCheck = _int(pUi->Get_Tex1_ProtoTag().rfind(L"Resource/"));
	if (iCheck != -1)
		m_Edit_Texure_ProtoTag.SetString(pUi->Get_Tex1_ProtoTag().substr(iCheck + 9).c_str());

	m_ListBox_EventList_Changed.ResetContent();
	const vector<_int>* vecEvent = pUi->Get_vecClickEvent();
	_int iSize = (_int)(*vecEvent).size();
	for (_int i = 0; i < iSize; ++i)
	{
		wstring wstrTemp;
		if ((*vecEvent)[i] == -1)
		{
			wstrTemp = L"부모";
		}
		else
		{
			wstrTemp = wstring(to_wstring((*vecEvent)[i]) + L"번째 자식");
		}
		if (i != iSize - 1)
			wstrTemp.append(L"의");

		m_ListBox_EventList_Changed.AddString(wstrTemp.c_str());
	}

	m_Slider_TextAlpha.SetPos((_int)(pUi->Get_ImageAlpha() * m_iRangeMax));
	m_Slider_ImageAlpha.SetPos((_int)(pUi->Get_ImageColor().a * m_iRangeMax));

	m_ComboBox_EventType.SetCurSel(pUi->Get_ButtonEventType());

	_int iFontType = (_int)pUi->Get_TextOption().eFontType;
	iFontType %= 4;
	m_Combo_FontType.SetCurSel(iFontType);

	m_Combo_EaseType.SetCurSel(pUi->Get_Ease());

	m_Combo_AnimType.SetCurSel((_int)pUi->Get_UiAnimation().eAnimType);

	m_MFCColorButton_ImageColor.SetColor(D3DCOLORTOCOLORREF(pUi->Get_ImageColor()));
	m_MH_mfcColorButton.SetColor(D3DCOLORTOCOLORREF(pUi->Get_TextOption().tColor));

	m_CheckBox_ChildClickCheck.SetCheck(pUi->Get_ChildClickCheck());
	m_CheckBox_ClickAnimation.SetCheck(pUi->Get_ClickAnimationOnOff());
	m_CheckBox_OnEnableAnimation.SetCheck(pUi->Get_OnEnableAnimationOnOff());
	m_CheckBox_MaskOnOff.SetCheck(pUi->Get_MaskOnOff());
	m_CheckBox_Italic_2.SetCheck(pUi->Get_Italic());

	wstring wstrTextAlign = L"Align ";
	if ((_int)E_TextAlign::Left == pUi->Get_TextAlign())
		wstrTextAlign.append(L"L");
	if ((_int)E_TextAlign::Center == pUi->Get_TextAlign())
		wstrTextAlign.append(L"C");
	if ((_int)E_TextAlign::Right == pUi->Get_TextAlign())
		wstrTextAlign.append(L"R");

	m_Edit_NameTag.SetString(pUi->Get_Name().c_str());

	m_ListBox_EventLists.ResetContent();
	list<vector<_int>>* pEventList = pUi->Get_EventVecList();
	auto iterTypeList = pUi->Get_EventTypeList()->begin();
	for (auto& iterEventList : (*pEventList))
	{

		wstring wstrEvent = L"";
		switch (*iterTypeList)
		{
		case Engine::E_UiButtonEventType::E_REFLECT:
			wstrEvent.append(L"반복_");
			break;
		case Engine::E_UiButtonEventType::E_FALSE:
			wstrEvent.append(L"끄기_");
			break;
		case Engine::E_UiButtonEventType::E_TRUE:
			wstrEvent.append(L"켜기_");
			break;
		case Engine::E_UiButtonEventType::E_POPUP:
			wstrEvent.append(L"팝업_");
			break;
		default:
			assert(FALSE);
			break;
		}

		for (_int j = 0; j < (_int)iterEventList.size(); j++)
		{
			wstrEvent.append(to_wstring(iterEventList[j]));
		}
		m_ListBox_EventLists.AddString(wstrEvent.c_str());

		iterTypeList++;
	}

	/*if(pUi->Get_TextOption().wstrFontName == FontTag_경기천년제목_Medium)*/

	// 아직 연동안된 목록
	// wstrFontName, iFontSize, eFontStyle, eTextVertexOfAngle
	UpdateFor_ScrollView(*m_ppPickedObject);

	CUI_Damage* pNumber = dynamic_cast<CUI_Damage*>(*m_ppPickedObject);

	if (nullptr == pNumber)
		m_ComboBox_NumberFontType.ShowWindow(SW_HIDE);
	else
	{
		m_ComboBox_NumberFontType.ShowWindow(SW_SHOW);
		m_ComboBox_NumberFontType.SetCurSel(pNumber->Get_NumberType());
	}
	UpdateData(FALSE);
	return pUi;
}

void CTool_Ui::UpdateFor_PivotAndAnchor()
{
	if (Management Key_Pressing(KEY_Alt))
	{
		m_b_TrueIsImage_FalseIsText = TRUE;
		m_GroupBox_PivotAnchor.SetString(L"Text - VertexOfAngle");
	}
	else
	{
		m_b_TrueIsImage_FalseIsText = FALSE;
		if (Management Key_Pressing(KEY_Shift))
		{
			m_GroupBox_PivotAnchor.SetString(L"Image - Pivot");
			m_b_TrueIsPivot_FalseIsAnchor = TRUE;
		}
		else
		{
			m_GroupBox_PivotAnchor.SetString(L"Image - Anchor");
			m_b_TrueIsPivot_FalseIsAnchor = FALSE;
		}
	}
}

void CTool_Ui::UpdateMFC_Position(CUI_Object* pUi)
{
	UpdateData(TRUE);
	m_Edit_Ui_Position_X.SetString(to_wstring(pUi->Get_UiTransform().iPosX).c_str());
	m_Edit_Ui_Position_Y.SetString(to_wstring(pUi->Get_UiTransform().iPosY).c_str());
	UpdateData(FALSE);
}

void CTool_Ui::UpdateFor_ScrollView(CGameObject* pPickObject)
{
	if (nullptr == pPickObject)
		return;

	CUI_Scroll_View* pScrollView = dynamic_cast<CUI_Scroll_View*>(pPickObject);
	if (nullptr == pScrollView)
	{
		m_ScrollView_Static_Option.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Spacing.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Child_X.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Child_Y.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Padding.ShowWindow(SW_HIDE);
		m_ScrollView_Static_ChildSize.ShowWindow(SW_HIDE);
		m_ScrollView_Edit_Child_SizeX.ShowWindow(SW_HIDE);
		m_ScrollView_Edit_Child_SizeY.ShowWindow(SW_HIDE);
		m_ScrollView_ComboBox_Padding.ShowWindow(SW_HIDE);
		m_ScrollView_Edit_Padding.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Spacing_X.ShowWindow(SW_HIDE);
		m_ScrollView_Static_Spacing_Y.ShowWindow(SW_HIDE);
		m_ScrollView_Edit_Child_Spacing_X.ShowWindow(SW_HIDE);
		m_ScrollView_Edit_Child_Spacing_Y.ShowWindow(SW_HIDE);
		m_ScrollView_ComboBox_Geometry.ShowWindow(SW_HIDE);
		m_CheckBox_ChildSizeUse.ShowWindow(SW_HIDE);
		m_CheckBox_VerticalScroll.ShowWindow(SW_HIDE);
		m_CheckBox_HorizontalScroll.ShowWindow(SW_HIDE);

		return;
	}

	m_ScrollView_Static_Option.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Spacing.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Child_X.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Child_Y.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Padding.ShowWindow(SW_SHOW);
	m_ScrollView_Static_ChildSize.ShowWindow(SW_SHOW);
	m_ScrollView_Edit_Child_SizeX.ShowWindow(SW_SHOW);
	m_ScrollView_Edit_Child_SizeY.ShowWindow(SW_SHOW);
	m_ScrollView_ComboBox_Padding.ShowWindow(SW_SHOW);
	m_ScrollView_Edit_Padding.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Spacing_X.ShowWindow(SW_SHOW);
	m_ScrollView_Static_Spacing_Y.ShowWindow(SW_SHOW);
	m_ScrollView_Edit_Child_Spacing_X.ShowWindow(SW_SHOW);
	m_ScrollView_Edit_Child_Spacing_Y.ShowWindow(SW_SHOW);
	m_ScrollView_ComboBox_Geometry.ShowWindow(SW_SHOW);
	m_CheckBox_ChildSizeUse.ShowWindow(SW_SHOW);
	m_CheckBox_VerticalScroll.ShowWindow(SW_SHOW);
	m_CheckBox_HorizontalScroll.ShowWindow(SW_SHOW);

	m_Edit_ChildSize_X.SetString(to_wstring(pScrollView->Get_ChildSize(E_POSITION::POS_X)).c_str());
	m_Edit_ChildSize_Y.SetString(to_wstring(pScrollView->Get_ChildSize(E_POSITION::POS_Y)).c_str());

	_int iPadding = m_ScrollView_ComboBox_Padding.GetCurSel();
	m_Edit_Padding.SetString(to_wstring(pScrollView->Get_Padding((E_DIR)iPadding)).c_str());

	m_Edit_Child_Spacing_X.SetString(to_wstring(pScrollView->Get_Spacing(E_POSITION::POS_X)).c_str());
	m_Edit_Child_Spacing_Y.SetString(to_wstring(pScrollView->Get_Spacing(E_POSITION::POS_Y)).c_str());


	_int iGeo = (_int)pScrollView->Get_Geometry();
	_bool bChildUse = pScrollView->Get_ChildSizeUse();

	m_CheckBox_ChildSizeUse.SetCheck(bChildUse);
	m_ScrollView_ComboBox_Geometry.SetCurSel(iGeo);

	m_CheckBox_VerticalScroll.SetCheck(pScrollView->Get_ScrollOption(E_Geometry::E_VERTICAL));
	m_CheckBox_HorizontalScroll.SetCheck(pScrollView->Get_ScrollOption(E_Geometry::E_HORIZONTAL));


}

CUI_Object * CTool_Ui::Is_UiObject()
{
	if (nullptr == m_ppPickedObject)
		return nullptr;

	if (nullptr == *m_ppPickedObject)
		return nullptr;

	if (nullptr == dynamic_cast<CUI_Object*>((*m_ppPickedObject)))
	{
		cout << endl;
		cout << "선택된 오브젝트가 UI가 아닙니다." << endl;
		return nullptr;
	}
	CUI_Object* pUi = static_cast<CUI_Object*>((*m_ppPickedObject));
	return pUi;
}

void CTool_Ui::OnTimer(UINT_PTR nIDEvent)					/////////////////////	Update
{
	CDialogEx::OnTimer(nIDEvent);

	static _bool bClicked = FALSE;


	_int iDiffX = abs(Management Get_MouseMovementFigure().x) >= 2.f ? (_int)Management Get_MouseMovementFigure().x : 0;
	_int iDiffY = abs(Management Get_MouseMovementFigure().y) >= 2.f ? -(_int)Management Get_MouseMovementFigure().y : 0;

	UpdateData(TRUE);
	if (TIMERID == nIDEvent)
	{
		CUI_Object* pUi = Is_UiObject();
		if (nullptr == pUi)
			return;

		wstring wText = m_Edit_TextBox.GetString();
		pUi->Set_Ui_Text(wText);

		_float Color_R = Ref2Float(m_MH_mfcColorButton.GetColor(), E_RGB::RGB_R);
		_float Color_G = Ref2Float(m_MH_mfcColorButton.GetColor(), E_RGB::RGB_G);
		_float Color_B = Ref2Float(m_MH_mfcColorButton.GetColor(), E_RGB::RGB_B);
		pUi->Set_Ui_TextColor(_vec4(Color_R, Color_G, Color_B, m_Slider_TextAlpha.GetPos() / 255.f));

		if (GetDlgItem(IDC_MH_UI_SIZE_X) == GetFocus())
			pUi->Set_Ui_SizeX(_wtoi(m_Edit_Ui_Size_X.GetString()));
		else
			m_Edit_Ui_Size_X.SetString(to_wstring(pUi->Get_UiTransform().iSizeX).c_str());

		if (GetDlgItem(IDC_MH_UI_SIZE_Y) == GetFocus())
			pUi->Set_Ui_SizeY(_wtoi(m_Edit_Ui_Size_Y.GetString()));
		else
			m_Edit_Ui_Size_Y.SetString(to_wstring(pUi->Get_UiTransform().iSizeY).c_str());

		if (GetDlgItem(IDC_MH_UI_POSITION_X) == GetFocus())
			pUi->Set_Ui_PosX(_wtoi(m_Edit_Ui_Position_X.GetString()));
		else
			m_Edit_Ui_Position_X.SetString(to_wstring(pUi->Get_UiTransform().iPosX).c_str());

		if (GetDlgItem(IDC_MH_UI_POSITION_Y) == GetFocus())
			pUi->Set_Ui_PosY(_wtoi(m_Edit_Ui_Position_Y.GetString()));
		else
			m_Edit_Ui_Position_Y.SetString(to_wstring(pUi->Get_UiTransform().iPosY).c_str());


		if (GetDlgItem(IDC_MH_UI_TEXTPOSITION_X) == GetFocus())
			pUi->Set_Ui_TextPosX(_wtoi(m_Edit_Ui_Text_Position_X.GetString()));
		else
			m_Edit_Ui_Text_Position_X.SetString(to_wstring(pUi->Get_TextOption().iTextPosX).c_str());

		if (GetDlgItem(IDC_MH_UI_TEXTPOSITION_Y) == GetFocus())
			pUi->Set_Ui_TextPosY(_wtoi(m_Edit_Ui_Text_Position_Y.GetString()));
		else
			m_Edit_Ui_Text_Position_Y.SetString(to_wstring(pUi->Get_TextOption().iTextPosY).c_str());


		if (GetDlgItem(IDC_MH_UI_ORDERINLAYER) == GetFocus())
			pUi->Set_Order_In_Layer(_wtoi(m_Edit_Ui_OrderInLayer.GetString()));
		else
			m_Edit_Ui_OrderInLayer.SetString(to_wstring(pUi->Get_OrderInLayer()).c_str());

		if (GetDlgItem(IDC_MH_UI_SORTING_LAYER) == GetFocus())
			pUi->Set_Sorting_Layer(_wtoi(m_Edit_Ui_SortingLayer.GetString()));
		else
			m_Edit_Ui_SortingLayer.SetString(to_wstring(pUi->Get_SortingLayer()).c_str());

		pUi->Set_ButtonEventType((E_UiButtonEventType)(m_ComboBox_EventType.GetCurSel()));

		_int iFontType = m_Combo_FontType.GetCurSel();
		iFontType += m_CheckBox_FontBold.GetCheck() * 4;
		pUi->Set_Ui_FontType((E_FontType)iFontType);

		pUi->Set_Ease((E_EASE)m_Combo_EaseType.GetCurSel());


		if (GetDlgItem(IDC_MH_1220_EDIT_UV_MIN_X) == GetFocus())
			pUi->Set_UV(E_MinMax::Min, E_POSITION::POS_X, (_float)_wtof(m_Edit_UV_Min_X.GetString()));
		else if (pUi->Get_UiTransform().vDrawUvMin.x < 1000.f)
			m_Edit_UV_Min_X.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiTransform().vDrawUvMin.x), 3).c_str());
		if (GetDlgItem(IDC_MH_1220_EDIT_UV_MIN_Y) == GetFocus())
			pUi->Set_UV(E_MinMax::Min, E_POSITION::POS_Y, (_float)_wtof(m_Edit_UV_Min_Y.GetString()));
		else if (pUi->Get_UiTransform().vDrawUvMin.y < 1000.f)
			m_Edit_UV_Min_Y.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiTransform().vDrawUvMin.y), 3).c_str());

		if (GetDlgItem(IDC_MH_1220_EDIT_UV_MAX_X) == GetFocus())
			pUi->Set_UV(E_MinMax::Max, E_POSITION::POS_X, (_float)_wtof(m_Edit_UV_Max_X.GetString()));
		else if (pUi->Get_UiTransform().vDrawUvMax.x < 1000.f)
			m_Edit_UV_Max_X.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiTransform().vDrawUvMax.x), 3).c_str());
		if (GetDlgItem(IDC_MH_1220_EDIT_UV_MAX_Y) == GetFocus())
			pUi->Set_UV(E_MinMax::Max, E_POSITION::POS_Y, (_float)_wtof(m_Edit_UV_Max_Y.GetString()));
		else if (pUi->Get_UiTransform().vDrawUvMax.y < 1000.f)
			m_Edit_UV_Max_Y.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiTransform().vDrawUvMax.y), 3).c_str());


		_int iAnimType = m_Combo_AnimType.GetCurSel();
		pUi->Set_AnimType((E_UiAnimationType)iAnimType);

		if (GetDlgItem(IDC_MH_1221_EDIT_PROGRESS_MAXTIME) == GetFocus())
			pUi->Set_ProgMax((_float)_wtof(m_Edit_Animation_ProgMax.GetString()));
		else
			m_Edit_Animation_ProgMax.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiAnimation().fProgMax), 1).c_str());

		if (GetDlgItem(IDC_MH_1221_EDIT_PARAM_INT_1) == GetFocus())
			pUi->Set_int_Parameter_1((_int)_wtoi(m_Edit_Animation_Int1.GetString()));
		else
			m_Edit_Animation_Int1.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiAnimation().int_Parameter_1), -2).c_str());
		if (GetDlgItem(IDC_MH_1221_EDIT_PARAM_INT_2) == GetFocus())
			pUi->Set_int_Parameter_2((_int)_wtoi(m_Edit_Animation_Int2.GetString()));
		else
			m_Edit_Animation_Int2.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiAnimation().int_Parameter_2), -1).c_str());

		if (GetDlgItem(IDC_MH_1221_EDIT_PARAM_FLOAT_1) == GetFocus())
			pUi->Set_float_Parameter_1((_float)_wtof(m_Edit_Animation_Float1.GetString()));
		else
			m_Edit_Animation_Float1.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiAnimation().float_Parameter_1), 2).c_str());
		if (GetDlgItem(IDC_MH_1221_EDIT_PARAM_FLOAT_2) == GetFocus())
			pUi->Set_float_Parameter_2((_float)_wtof(m_Edit_Animation_Float2.GetString()));
		else
			m_Edit_Animation_Float2.SetString(CUtility::RoundWString(to_wstring(pUi->Get_UiAnimation().float_Parameter_2), 2).c_str());

		if (m_CheckBox_ChildTo.GetCheck())
			pUi->Set_ChildTo(TRUE);
		else
			pUi->Set_ChildTo(FALSE);

		D3DXCOLOR color = m_MFCColorButton_ImageColor.GetColor();
		color.a = m_Slider_ImageAlpha.GetPos() / (_float)m_iRangeMax;
		_float fTempRed = color.r;
		color.r = color.b;
		color.b = fTempRed;
		pUi->Set_ImageColor(color);

		if (m_CheckBox_ChildClickCheck.GetCheck())
			pUi->Set_ChildClickCheck(TRUE);
		else
			pUi->Set_ChildClickCheck(FALSE);



		if (GetDlgItem(IDC_MH_1221_EDIT_UI_NAMETAG) == GetFocus())
			pUi->Set_NameTag(m_Edit_NameTag.GetString());


		if (m_CheckBox_ClickAnimation.GetCheck())
			pUi->Set_ClickAnimationOnOff(TRUE);
		else
			pUi->Set_ClickAnimationOnOff(FALSE);

		if (m_CheckBox_OnEnableAnimation.GetCheck())
			pUi->Set_OnEnableAnimationOnOff(TRUE);
		else
			pUi->Set_OnEnableAnimationOnOff(FALSE);
		
		if (m_CheckBox_MaskOnOff.GetCheck())
			pUi->Set_MaskOnOff(TRUE);
		else
			pUi->Set_MaskOnOff(FALSE);

		if (m_CheckBox_Italic_2.GetCheck())
			pUi->Set_Italic(TRUE);
		else
			pUi->Set_Italic(FALSE);

		if (GetDlgItem(IDC_MH_1230_EDIT_FONTSIZE) == GetFocus())
			pUi->Set_Ui_FontSize(_wtoi(m_Edit_FontSize.GetString()));
		else
			m_Edit_FontSize.SetString(CUtility::RoundWString(to_wstring(pUi->Get_TextOption().iFontSize), -1).c_str());
		
		// 스크롤 뷰 전용
		CUI_Scroll_View* pScrollView = dynamic_cast<CUI_Scroll_View*>(pUi);
		if (nullptr != pScrollView)
		{
			// ChildSize
			_vec3 vChildSize = VEC_ZERO;
			vChildSize = _vec3((_float)pScrollView->Get_ChildSize(E_POSITION::POS_X), (_float)pScrollView->Get_ChildSize(E_POSITION::POS_Y), 0.f);
			_vec3 vEditChildSize = _vec3((_float)_wtof(m_Edit_ChildSize_X.GetString()), (_float)_wtof(m_Edit_ChildSize_Y.GetString()), 0.f);

			_vec3 vDiffChildSize = vChildSize - vEditChildSize;
			if (abs(vDiffChildSize.x) > 0.1f && GetDlgItem(IDC_MH_UITOOL_CHILDSIZE_X) != GetFocus())
				m_Edit_ChildSize_X.SetString(CUtility::RoundWString(to_wstring(vChildSize.x), 0).c_str());
			else if (GetDlgItem(IDC_MH_UITOOL_CHILDSIZE_X) == GetFocus())
				pScrollView->Set_ChildSize(E_POSITION::POS_X, _wtoi(m_Edit_ChildSize_X.GetString()));

			if (abs(vDiffChildSize.y) > 0.1f && GetDlgItem(IDC_MH_UITOOL_CHILDSIZE_Y) != GetFocus())
				m_Edit_ChildSize_Y.SetString(CUtility::RoundWString(to_wstring(vChildSize.y), 0).c_str());
			else if (GetDlgItem(IDC_MH_UITOOL_CHILDSIZE_Y) == GetFocus())
				pScrollView->Set_ChildSize(E_POSITION::POS_Y, _wtoi(m_Edit_ChildSize_Y.GetString()));


			_vec3 vSpacing = VEC_ZERO;
			vSpacing = _vec3((_float)pScrollView->Get_Spacing(E_POSITION::POS_X), (_float)pScrollView->Get_Spacing(E_POSITION::POS_Y), 0.f);
			_vec3 vEditSpacing = _vec3((_float)_wtof(m_Edit_Child_Spacing_X.GetString()), (_float)_wtof(m_Edit_Child_Spacing_Y.GetString()), 0.f);

			_vec3 vDiffSpacing = vSpacing - vEditSpacing;
			if (abs(vDiffSpacing.x) > 0.1f && GetDlgItem(IDC_MH_UITOOL_SPACING_X) != GetFocus())
				m_Edit_Child_Spacing_X.SetString(CUtility::RoundWString(to_wstring(vSpacing.x), 0).c_str());
			else if (GetDlgItem(IDC_MH_UITOOL_SPACING_X) == GetFocus())
				pScrollView->Set_Spacing(E_POSITION::POS_X, _wtoi(m_Edit_Child_Spacing_X.GetString()));

			if (abs(vDiffSpacing.x) > 0.1f && GetDlgItem(IDC_MH_UITOOL_SPACING_Y) != GetFocus())
				m_Edit_Child_Spacing_Y.SetString(CUtility::RoundWString(to_wstring(vSpacing.y), 0).c_str());
			else if (GetDlgItem(IDC_MH_UITOOL_SPACING_Y) == GetFocus())
				pScrollView->Set_Spacing(E_POSITION::POS_Y, _wtoi(m_Edit_Child_Spacing_Y.GetString()));

			_float fPadding = (_float)pScrollView->Get_Padding((E_DIR)m_ScrollView_ComboBox_Padding.GetCurSel());
			_float fEditPadding = (_float)_wtof(m_Edit_Padding.GetString());

			_float fDiffPadding = fPadding - fEditPadding;
			if (abs(fDiffPadding) > 0.1f && GetDlgItem(IDC_MH_UITOOL_PADDING_AMOUNT) != GetFocus())
				m_Edit_Padding.SetString(CUtility::RoundWString(to_wstring(fPadding), 0).c_str());
			else if (GetDlgItem(IDC_MH_UITOOL_PADDING_AMOUNT) == GetFocus())
				pScrollView->Set_Padding((E_DIR)m_ScrollView_ComboBox_Padding.GetCurSel(), _wtoi(m_Edit_Padding.GetString()));

			if (m_CheckBox_ChildSizeUse.GetCheck())
				pScrollView->Set_ChildSizeUse(TRUE);
			else
				pScrollView->Set_ChildSizeUse(FALSE);

			if (m_CheckBox_VerticalScroll.GetCheck())
				pScrollView->Set_ScrollOption(E_Geometry::E_VERTICAL, TRUE);
			else
				pScrollView->Set_ScrollOption(E_Geometry::E_VERTICAL, FALSE);

			if (m_CheckBox_HorizontalScroll.GetCheck())
				pScrollView->Set_ScrollOption(E_Geometry::E_HORIZONTAL, TRUE);
			else
				pScrollView->Set_ScrollOption(E_Geometry::E_HORIZONTAL, FALSE);
			
			

			pScrollView->Set_Geometry((E_Geometry)m_ScrollView_ComboBox_Geometry.GetCurSel());

			/*
						if (GetDlgItem(IDC_MH_UITOOL_CHILDSIZE_USE) != GetFocus())
							m_CheckBox_ChildSizeUse.SetCheck(bChildUse);
						else
							pScrollView->Set_ChildSizeUse(m_CheckBox_ChildSizeUse.GetCheck());

						if (GetDlgItem(IDC_MH_UITOOL_COMBO_GEOMETRY) != GetFocus())
							m_ScrollView_ComboBox_Geometry.SetCurSel(iGeo);
						else
							pScrollView->Set_Geometry((E_Geometry)m_ScrollView_ComboBox_Geometry.GetCurSel());
			*/

		}


		CUI_Damage* pNumber = dynamic_cast<CUI_Damage*>(pUi);

		if (nullptr != pNumber)
		{
			pNumber->Set_NumberType((_int)m_ComboBox_NumberFontType.GetCurSel());
		}

		POINT ptMouse = CUtility::Get_MousePoint(g_hWnd);
		if (Management Key_Down(KEY_LBUTTON) && pUi->Is_OnMouse(ptMouse))
		{
			bClicked = TRUE;
		}
		else if (bClicked && !(Management Key_Pressing(KEY_LBUTTON)))
		{
			bClicked = FALSE;
		}
		if (Management Key_Pressing(KEY_LBUTTON) && bClicked)
		{
			pUi->Add_Ui_PosX(iDiffX);
			pUi->Add_Ui_PosY(iDiffY);
			Get_PickObject();
		}
		if (Management Key_Down(KEY_LBUTTON))
		{
			ptMouse.x -= (LONG)(WINCX * 0.5f);
			ptMouse.y -= (LONG)(WINCY * 0.5f);
			ptMouse.y *= 1;
			cout << "마우스 좌표 : " << ptMouse.x << ", " << ptMouse.y << endl;
		}

		if (GetDlgItem(IDC_MH_EDIT_UI_IMAGE) == GetFocus() && Management Key_Down(KEY_LBUTTON))
		{
			POINT ptMouse = CUtility::Get_MousePoint(g_hWnd);
			pUi->Set_Tex1_Tag(Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_TextureTree->m_wstrPickTexturePath);
			pUi->Set_UV(E_MinMax::Min, E_POSITION::POS_X, Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_TextureTree->m_vMinUV.x);
			pUi->Set_UV(E_MinMax::Min, E_POSITION::POS_Y, Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_TextureTree->m_vMinUV.y);
			pUi->Set_UV(E_MinMax::Max, E_POSITION::POS_X, Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_TextureTree->m_vMaxUV.x);
			pUi->Set_UV(E_MinMax::Max, E_POSITION::POS_Y, Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_TextureTree->m_vMaxUV.y);
		}
		
		if (GetDlgItem(IDC_MH_1222_LIST_EVENTLISTS) == GetFocus() && Management Key_Down(KEY_Delete))
		{
			_int iIndex = m_ListBox_EventLists.GetCurSel();
			auto iterVec	= pUi->Get_EventVecList()->begin();
			auto iterType	= pUi->Get_EventTypeList()->begin();
			for (_int i = 0; i < iIndex; ++i)
			{
				iterVec++;
				iterType++;
			}
			(*pUi->Get_EventVecList()).erase(iterVec);
			(*pUi->Get_EventTypeList()).erase(iterType);
			Get_PickObject();

		}
		UpdateFor_PivotAndAnchor();
	}
	UpdateData(FALSE);
}

void CTool_Ui::Clicked_PivotAnchor_TL()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::TL);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_TL);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_TL);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_TC()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::TC);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_TC);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_TC);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_TR()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::TR);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_TR);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_TR);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_CL()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::CL);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_CL);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_CL);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_C()
{

	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::C);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_Center);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_Center);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_CR()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	UpdateData(TRUE);
	if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::CR);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_CR);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_CR);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_BL()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::BL);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_BL);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_BL);
	}
	UpdateMFC_Position(pUi);
}

void CTool_Ui::Clicked_PivotAnchor_BC()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::BC);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_BC);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_BC);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Clicked_PivotAnchor_BR()
{
	CUI_Object* pUi = Get_PickObject();
	if (nullptr == pUi) return;

	else if (m_b_TrueIsImage_FalseIsText)
	{
		pUi->Set_Ui_TextVertexOfAngle(E_VertexOfAngle::BR);
	}
	else
	{
		if (m_b_TrueIsPivot_FalseIsAnchor)	pUi->Set_Ui_TransformPivot(E_Pivot::Pivot_BR);
		else								pUi->Set_Ui_Anchor(E_Anchor::Anchor_BR);
	}
	UpdateMFC_Position(pUi);
}


void CTool_Ui::Click_Bold_Button()
{
	if (nullptr == dynamic_cast<CUI_Object*>(*m_ppPickedObject))
		return;

	CGameObject* pObject = Get_MFC_MainFrame()->Get_PickObject();
	if (nullptr == dynamic_cast<CUI_Object*>(pObject))
	{
		cout << endl;
		cout << "선택된 오브젝트가 UI가 아닙니다." << endl;
		return;
	}
	CUI_Object* pUi = static_cast<CUI_Object*>(pObject);
	if (nullptr == pUi)
		return;

	_bool bCheck = FALSE;
	if (m_CheckBox_FontBold.GetCheck())
		bCheck = TRUE;

	pUi->Set_Ui_FontBold(bCheck);
}

void CTool_Ui::Button_Create_Ui()
{
	_int iUi_Object			= 0;
	_int iUi_Slider_Vert	= 1;
	_int iUi_Scroll_View	= 2;
	_int iUi_Number			= 3;


	if (nullptr == dynamic_cast<CUI_Object*>(*m_ppPickedObject))
	{
		if (m_ComboBox_CreateType.GetCurSel() == iUi_Object)
		{
			// UiLayer Back에 추가한다.
			CGameObject* pUi = CUI_Object::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
			assert(pUi);
			Management Get_Scene()->Get_Layer(E_LAYERTAG::UI)->Add_GameObject(E_OBJTAG::OBJ_UI, pUi);
		}
		else if (m_ComboBox_CreateType.GetCurSel() == iUi_Slider_Vert)
		{
			CGameObject* pUi = CUI_Slider_Vert::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SLIDER);
			assert(pUi);
			Management Get_Scene()->Get_Layer(E_LAYERTAG::UI)->Add_GameObject(E_OBJTAG::OBJ_UI_SLIDER, pUi);
		}
		else if (m_ComboBox_CreateType.GetCurSel() == iUi_Scroll_View)
		{
			CGameObject* pUi = CUI_Scroll_View::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
			assert(pUi);
			Management Get_Scene()->Get_Layer(E_LAYERTAG::UI)->Add_GameObject(E_OBJTAG::OBJ_UI_SCROLLVIEW, pUi);
		}
		else if (m_ComboBox_CreateType.GetCurSel() == iUi_Number)
		{
			CGameObject* pUi = CUI_Damage::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI, 9999999.f, E_NumberFontType::Normal, 0);
			assert(pUi);
			Management Get_Scene()->Get_Layer(E_LAYERTAG::UI)->Add_GameObject(E_OBJTAG::OBJ_UI, pUi);
		}

		Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->Update_Tree();
		return;
	}

	// pPickObject 가 UI일 경우 자식으로 추가한다.
	CUI_Object* pPickUi = static_cast<CUI_Object*>((*m_ppPickedObject));

	if (m_ComboBox_CreateType.GetCurSel() == iUi_Object)
	{
		CUI_Object* pUi = CUI_Object::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
		assert(pUi);
		pPickUi->Set_Child(pUi);
	}
	else if (m_ComboBox_CreateType.GetCurSel() == iUi_Slider_Vert)
	{
		CUI_Object* pUi = CUI_Slider_Vert::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SLIDER);
		assert(pUi);
		pPickUi->Set_Child(pUi);
	}
	else if (m_ComboBox_CreateType.GetCurSel() == iUi_Scroll_View)
	{
		CUI_Object* pUi = CUI_Scroll_View::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
		assert(pUi);
		pPickUi->Set_Child(pUi);
	}
	else if (m_ComboBox_CreateType.GetCurSel() == iUi_Number)
	{
		CUI_Object* pUi = CUI_Damage::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI, 9999999.f, E_NumberFontType::Normal, 0);
		assert(pUi);
		pPickUi->Set_Child(pUi);
	}

	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->Update_Tree();
}


void CTool_Ui::Button_Save_Prefab()
{
	if (nullptr == dynamic_cast<CUI_Object*>(*m_ppPickedObject))
		return;

	CFileDialog Dlg(FALSE, L"uidat", L"*.uidat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_UI\\Prefab");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		(*m_ppPickedObject)->Save(hFile, dwByte);

		CloseHandle(hFile);
	}
}


void CTool_Ui::Button_Load_Prefab()
{
	CFileDialog Dlg(TRUE, L"uidat", L"*.uidat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };
	Get_MFC_MainFrame()->m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_UI\\Prefab");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		wstring wstrUITag;
		CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

		_int iObjTag = 0;
		CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

		CUI_Object* pUi = nullptr;
		CLayer* pLayer = Management Get_Scene()->Get_Layer(E_LAYERTAG::UI);
		if (L"CUI_Object" == wstrUITag)
		{
			pUi = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI);
			pUi->Load(hFile, dwByte);
		}
		else if (L"CUI_Slider_Vert" == wstrUITag)
		{
			pUi = CUI_Slider_Vert::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI_SLIDER);
			pUi->Load(hFile, dwByte);
		}
		else if (L"CUI_Scroll_View" == wstrUITag)
		{
			pUi = CUI_Scroll_View::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
			pUi->Load(hFile, dwByte);
		}
		else if (L"CUI_Damage" == wstrUITag)
		{
			pUi = CUI_Damage::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI, 9999999.f, E_NumberFontType::Normal, 0);
			pUi->Load(hFile, dwByte);
		}

		assert(pUi);


		if (nullptr != dynamic_cast<CUI_Object*>(*m_ppPickedObject))
		{
			static_cast<CUI_Object*>(*m_ppPickedObject)->Set_Child(pUi);
		}
		else
		{
			if (L"CUI_Object" == wstrUITag)
			{
				pLayer->Add_GameObject(E_OBJTAG::OBJ_UI, pUi);
			}
			else if (L"CUI_Slider_Vert" == wstrUITag)
			{
				pLayer->Add_GameObject(E_OBJTAG::OBJ_UI_SLIDER, pUi);
			}
			else if (L"CUI_Scroll_View" == wstrUITag)
			{
				pLayer->Add_GameObject(E_OBJTAG::OBJ_UI_SCROLLVIEW, pUi);
			}
			else if (L"CUI_Damage" == wstrUITag)
			{
				pLayer->Add_GameObject(E_OBJTAG::OBJ_UI, pUi);
			}
		}

		CloseHandle(hFile);
	}

	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->Update_Tree();
}


void CTool_Ui::Button_Save_Ui_Layer()
{
	CFileDialog Dlg(FALSE, L"uidat", L"*.uidat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_UI\\Layer");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CLayer* pLayer = Management Get_Scene()->Get_Layer(E_LAYERTAG::UI);
		vector<CGameObject*>* pvecUi_Object = pLayer->Get_vecGameObject(E_OBJTAG::OBJ_UI);

		_int iSize = (_int)pvecUi_Object->size();
		WriteFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);
		for (_int i = 0; i < (_int)pvecUi_Object->size(); i++)
		{
			(*pvecUi_Object)[i]->Save(hFile, dwByte);
		}

		vector<CGameObject*>* pvecSlider = pLayer->Get_vecGameObject(E_OBJTAG::OBJ_UI_SLIDER);

		if (pvecSlider)
		{
			_int iSize_Slider = (_int)pvecSlider->size();
			WriteFile(hFile, &iSize_Slider, sizeof(_int), &dwByte, nullptr);
			for (_int i = 0; i < (_int)pvecSlider->size(); i++)
			{
				(*pvecSlider)[i]->Save(hFile, dwByte);
			}
		}
		else
		{
			_int Temp = 0;
			WriteFile(hFile, &Temp, sizeof(_int), &dwByte, nullptr);
		}

		vector<CGameObject*>* pvecScroll = pLayer->Get_vecGameObject(E_OBJTAG::OBJ_UI_SCROLLVIEW);

		if (pvecScroll)
		{
			_int iSize_Scroll = (_int)pvecScroll->size();
			WriteFile(hFile, &iSize_Scroll, sizeof(_int), &dwByte, nullptr);
			for (_int i = 0; i < (_int)pvecScroll->size(); i++)
			{
				(*pvecScroll)[i]->Save(hFile, dwByte);
			}
		}
		else
		{
			_int Temp = 0;
			WriteFile(hFile, &Temp, sizeof(_int), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CTool_Ui::Button_Load_Ui_Layer()
{
	CLayer* pLayer = Management Get_Scene()->Get_Layer(E_LAYERTAG::UI);
	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;
	(*m_ppPickedObject) = nullptr;
	pLayer->Set_Dead_AllObject();

	CFileDialog Dlg(TRUE, L"uidat", L"*.uidat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_UI\\Layer");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_int iSize = 0;
		CUtility::Data_ReadInt(hFile, iSize, dwByte);
		for (_int i = 0; i < iSize; i++)
		{
			wstring wstrUITag;
			CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

			_int iObjTag = 0;
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			CUI_Object* pUi = nullptr;
			if (L"CUI_Object" == wstrUITag)
			{
				pUi = CUI_Object::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
				pUi->Load(hFile, dwByte);
			}
			else if (L"CUI_Damage" == wstrUITag)
			{
				pUi = CUI_Damage::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI, 9999999.f, E_NumberFontType::Normal, 0);
				pUi->Load(hFile, dwByte);
			}
			pLayer->Add_GameObject(E_OBJTAG::OBJ_UI, pUi);
		}

		_int iSize_Slider = 0;
		CUtility::Data_ReadInt(hFile, iSize_Slider, dwByte);
		for (_int i = 0; i < iSize_Slider; i++)
		{
			wstring wstrUITag;
			CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

			_int iObjTag = 0;
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			CUI_Object* pUi = nullptr;
			if (L"CUI_Slider_Vert" == wstrUITag)
			{
				pUi = CUI_Slider_Vert::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SLIDER);
				pUi->Load(hFile, dwByte);
			}
			pLayer->Add_GameObject(E_OBJTAG::OBJ_UI_SLIDER, pUi);
		}

		_int iSize_Scroll = 0;
		CUtility::Data_ReadInt(hFile, iSize_Scroll, dwByte);
		for (_int i = 0; i < iSize_Scroll; i++)
		{
			wstring wstrUITag;
			CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

			_int iObjTag = 0;
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			CUI_Object* pUi = nullptr;
			if (L"CUI_Scroll_View" == wstrUITag)
			{
				pUi = CUI_Scroll_View::Create(Get_MFC_MainFrame()->m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
				pUi->Load(hFile, dwByte);
			}
			pLayer->Add_GameObject(E_OBJTAG::OBJ_UI_SCROLLVIEW, pUi);
		}

		CloseHandle(hFile);
	}

	Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->Update_Tree();
}


void CTool_Ui::Button_Event_Parent()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Add_EventQueue(-1);
	Get_PickObject();
}


void CTool_Ui::Button_Event_Child()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	_int iIndex = _wtoi(m_Edit_EventNumber.GetString());

	if (1 > iIndex)
		return;

	pUi->Add_EventQueue(iIndex);
	Get_PickObject();
}


void CTool_Ui::Button_Event_Clear()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Clear_EventQueue();
	Get_PickObject();

}


void CTool_Ui::Spin_Click_ChangeIndex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	if (nullptr == *m_ppPickedObject)
		return;

	if (nullptr == dynamic_cast<CUI_Object*>((*m_ppPickedObject)))
	{
		CUtility::Print_Wstring_DMode(L"선택된 오브젝트가 UI가 아닙니다.");
		return;
	}
	CUI_Object* pUi = static_cast<CUI_Object*>(*m_ppPickedObject);

	if (nullptr == pUi->Get_Parent())
	{
		CUtility::Print_Wstring_DMode(L"선택된 오브젝트가 부모가 없습니다.");
		return;
	}

	CUI_Object* pParent = pUi->Get_Parent();

	_bool bFind = FALSE;
	_bool bStopFind = FALSE;
	_int iFindIndex = 0;
	_int iSize = (_int)pParent->Get_ChildList()->size();

	if (1 >= iSize)
	{
		CUtility::Print_Wstring_DMode(L"부모의 자식의 개수가 1개 이하입니다.");
		return;
	}
	list<CUI_Object*>::iterator iter = pParent->Get_ChildList_NotConst()->begin();
	list<CUI_Object*> tempList;
	CUI_Object* pBefore = nullptr;
	CUI_Object* pNext = nullptr;
	for (; iter != pParent->Get_ChildList()->end(); ++iter)
	{
		tempList.push_back((*iter));
		if (bStopFind)
		{
			continue;
		}
		if (bFind)
		{
			pNext = (*iter);
			bStopFind = TRUE;
		}
		else if ((*iter) == pUi)
		{
			CUtility::Print_Wstring_DMode(L"자식을 찾았습니다 " + to_wstring(iFindIndex));
			bFind = TRUE;
		}
		else
		{
			++iFindIndex;
			pBefore = (*iter);
		}
	}

	if (FALSE == bFind)
	{
		CUtility::Print_Wstring_DMode(L"찾지 못했습니다.");
		return;
	}

	_int iIndex = 0;
	if (pNMUpDown->iDelta > 0)
	{
		if (iFindIndex < iSize - 1 && pNext)
		{
			pParent->Get_ChildList_NotConst()->clear();
			list<CUI_Object*>::iterator iter = tempList.begin();
			for (; iter != tempList.end(); ++iter)
			{
				if (iIndex == iFindIndex)
				{
					pParent->Get_ChildList_NotConst()->push_back(*(++iter));
					pParent->Get_ChildList_NotConst()->push_back(*(--iter));
					++iter;
					++iIndex;
				}
				else
				{
					pParent->Get_ChildList_NotConst()->push_back(*iter);
				}
				++iIndex;
			}
		}
	}
	else
	{
		if (iFindIndex >= 1 && pBefore)
		{
			pParent->Get_ChildList_NotConst()->clear();
			list<CUI_Object*>::iterator iter = tempList.begin();
			for (; iter != tempList.end(); ++iter)
			{
				if (iIndex == iFindIndex - 1)
				{
					pParent->Get_ChildList_NotConst()->push_back(*(++iter));
					pParent->Get_ChildList_NotConst()->push_back(*(--iter));
					++iter;
					++iIndex;
				}
				else
				{
					pParent->Get_ChildList_NotConst()->push_back(*iter);
				}
				++iIndex;
			}
		}
	}
}

void CTool_Ui::Button_AddEvent()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Push_ClickEvent();

	Get_PickObject();
}

void CTool_Ui::Button_TextAlign_Left()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Set_TextAlign(E_TextAlign::Left);
	m_Edit_Align_Text.SetString(L"Align L");
}

void CTool_Ui::Button_TextAlign_Center()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Set_TextAlign(E_TextAlign::Center);
	m_Edit_Align_Text.SetString(L"Align C");
}

void CTool_Ui::Button_TextAlign_Right()
{
	CUI_Object* pUi = Is_UiObject();
	if (nullptr == pUi)
		return;

	pUi->Set_TextAlign(E_TextAlign::Right);
	m_Edit_Align_Text.SetString(L"Align R");
}
