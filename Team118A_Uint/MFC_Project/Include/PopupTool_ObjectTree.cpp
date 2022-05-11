#include "stdafx.h"
#include "MFC_Project.h"
#include "PopupTool_ObjectTree.h"
#include "afxdialogex.h"

// CPopupTool_ObjectTree 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopupTool_ObjectTree, CDialogEx)

CPopupTool_ObjectTree::CPopupTool_ObjectTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_POPUPTOOL_OBJECTTREE, pParent)
	, m_pPickObject(nullptr)
	, m_Edit_Position_X(_T(""))
	, m_Edit_Position_Y(_T(""))
	, m_Edit_Position_Z(_T(""))
	, m_Edit_Rotation_X(_T(""))
	, m_Edit_Rotation_Y(_T(""))
	, m_Edit_Rotation_Z(_T(""))
	, m_Edit_Scale_X(_T(""))
	, m_Edit_Scale_Y(_T(""))
	, m_Edit_Scale_Z(_T(""))
{
	m_Edit_Position_X.SetString(L"0.0");
	m_Edit_Position_Y.SetString(L"0.0");
	m_Edit_Position_Z.SetString(L"0.0");
	m_Edit_Rotation_X.SetString(L"0.0");
	m_Edit_Rotation_Y.SetString(L"0.0");
	m_Edit_Rotation_Z.SetString(L"0.0");
	m_Edit_Scale_X.SetString(L"0.0");
	m_Edit_Scale_Y.SetString(L"0.0");
	m_Edit_Scale_Z.SetString(L"0.0");
}
CPopupTool_ObjectTree::~CPopupTool_ObjectTree()
{
}

void CPopupTool_ObjectTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_PUPUP_TOOL_OBJECT_TREE, m_tTreeCtrl);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_X, m_Edit_Position_X);
	DDV_MaxChars(pDX, m_Edit_Position_X, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Y, m_Edit_Position_Y);
	DDV_MaxChars(pDX, m_Edit_Position_Y, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Z, m_Edit_Position_Z);
	DDV_MaxChars(pDX, m_Edit_Position_Z, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_X, m_Edit_Rotation_X);
	DDV_MaxChars(pDX, m_Edit_Rotation_X, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_Y, m_Edit_Rotation_Y);
	DDV_MaxChars(pDX, m_Edit_Rotation_Y, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_Z, m_Edit_Rotation_Z);
	DDV_MaxChars(pDX, m_Edit_Rotation_Z, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_X, m_Edit_Scale_X);
	DDV_MaxChars(pDX, m_Edit_Scale_X, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_Y, m_Edit_Scale_Y);
	DDV_MaxChars(pDX, m_Edit_Scale_Y, 12);
	DDX_Text(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_Z, m_Edit_Scale_Z);
	DDV_MaxChars(pDX, m_Edit_Scale_Z, 12);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_X, m_Control_Position_X);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Y, m_Control_Position_Y);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Z, m_Control_Position_Z);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_X, m_Control_Rotation_X);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_Y, m_Control_Rotation_Y);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_ROTATION_Z, m_Control_Rotation_Z);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_X, m_Control_Scale_X);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_Y, m_Control_Scale_Y);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_EDIT_SCALE_Z, m_Control_Scale_Z);
}

BEGIN_MESSAGE_MAP(CPopupTool_ObjectTree, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDD_PUPUP_TOOL_OBJECT_TREE, &CPopupTool_ObjectTree::DbClick_PupupToolObjectTree)
	ON_NOTIFY(NM_CLICK, IDD_PUPUP_TOOL_OBJECT_TREE, &CPopupTool_ObjectTree::Click_PupupToolObjectTree)
	ON_BN_CLICKED(IDC_MH_OBJECTTREE_BUTTON_UPDATE, &CPopupTool_ObjectTree::ObjectTree_Button_Update)
	ON_BN_CLICKED(IDC_MH_POPUPTOOL_CONSOLECLEAR_BUTTON, &CPopupTool_ObjectTree::Button_Console_Clear)
	ON_BN_CLICKED(IDC_MH_POPUPTOOL_TEXTURELIST_BUTTON, &CPopupTool_ObjectTree::Button_Show_Popup_Texture)
	ON_BN_CLICKED(IDC_MH_POPUPTOOL_OBJECT_DELETE, &CPopupTool_ObjectTree::Button_SelectObjectSetDead)
END_MESSAGE_MAP()

// CPopupTool_ObjectTree 메시지 처리기입니다.


// 창이 켜졌을 때 Timer를 세팅해주고 
// 꺼질 때 m_pPickObject와 List를 클리어해준다.
void CPopupTool_ObjectTree::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetTimer(TIMERID, (_uint)(1.f / 30.f), NULL);

		UpdateData(TRUE);

		UpdateData(FALSE);
	}
	else
	{
		KillTimer(TIMERID);

		Clear_ListItemAndTree();
		m_pPickObject = nullptr;
	}
}

// m_pPickObject가 있으면 Transform관련된 값들을 수정할 수 있다.
void CPopupTool_ObjectTree::OnTimer(UINT_PTR nIDEvent)
{
	static _int iStack = 0;
	if (bUpdateNextFrame)
	{
		iStack++;
		if (iStack == 2)
		{
			iStack = 0;
			bUpdateNextFrame = FALSE;
			//Update_Tree();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
	if (TIMERID == nIDEvent)
	{

		if (nullptr == m_pPickObject && TRUE == m_Control_Position_X.IsWindowEnabled())
		{
			m_Control_Position_X.	EnableWindow(FALSE);
			m_Control_Position_Y.	EnableWindow(FALSE);
			m_Control_Position_Z.	EnableWindow(FALSE);
			m_Control_Rotation_X.	EnableWindow(FALSE);
			m_Control_Rotation_Y.	EnableWindow(FALSE);
			m_Control_Rotation_Z.	EnableWindow(FALSE);
			m_Control_Scale_X.		EnableWindow(FALSE);
			m_Control_Scale_Y.		EnableWindow(FALSE);
			m_Control_Scale_Z.		EnableWindow(FALSE);
		}
		else if (m_pPickObject && FALSE == m_Control_Position_X.IsWindowEnabled())
		{
			m_Control_Position_X.	EnableWindow(TRUE);
			m_Control_Position_Y.	EnableWindow(TRUE);
			m_Control_Position_Z.	EnableWindow(TRUE);
			m_Control_Rotation_X.	EnableWindow(TRUE);
			m_Control_Rotation_Y.	EnableWindow(TRUE);
			m_Control_Rotation_Z.	EnableWindow(TRUE);
			m_Control_Scale_X.		EnableWindow(TRUE);
			m_Control_Scale_Y.		EnableWindow(TRUE);
			m_Control_Scale_Z.		EnableWindow(TRUE);
		}

		if (nullptr == m_pPickObject)
		{
			return;
		}

		UpdateData(TRUE);
		
		// Edit에 적혀있는 값과 오브젝트의 실제 Transform 값이 0.01이상 차이난다면 
		// Edit창에 반영한다.
		_float fStandard = 0.009f;

		// Position
		_vec3 vObjPosition = VEC_ZERO;
		vObjPosition = m_pPickObject->Get_Info(E_INFO::INFO_POS);
		_vec3 vEditPosition = _vec3((_float)_wtof(m_Edit_Position_X.GetString()), (_float)_wtof(m_Edit_Position_Y.GetString()), (_float)_wtof(m_Edit_Position_Z.GetString()));

		_vec3 vDiffPos = vObjPosition - vEditPosition;
		if	(abs(vDiffPos.x) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_X) != GetFocus())
				m_Edit_Position_X.SetString(CUtility::RoundWString(to_wstring(vObjPosition.x), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_X) == GetFocus())
			m_pPickObject->Set_Pos(E_POSITION::POS_X, (_float)_wtof(m_Edit_Position_X.GetString()));

		if	(abs(vDiffPos.y) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Y) != GetFocus())
			m_Edit_Position_Y.SetString(CUtility::RoundWString(to_wstring(vObjPosition.y), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Y) == GetFocus())
			m_pPickObject->Set_Pos(E_POSITION::POS_Y, (_float)_wtof(m_Edit_Position_Y.GetString()));

		if	(abs(vDiffPos.z) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Z) != GetFocus())
			m_Edit_Position_Z.SetString(CUtility::RoundWString(to_wstring(vObjPosition.z), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_TRANSFORM_Z) == GetFocus())
			m_pPickObject->Set_Pos(E_POSITION::POS_Z, (_float)_wtof(m_Edit_Position_Z.GetString()));

		// Rotation
		_vec3 vObjRotation = VEC_ZERO;
		vObjRotation = m_pPickObject->Get_Angle();
		_vec3 vEditRotation = _vec3((_float)_wtof(m_Edit_Rotation_X.GetString()), (_float)_wtof(m_Edit_Rotation_Y.GetString()), (_float)_wtof(m_Edit_Rotation_Z.GetString()));

		_vec3 vDiffRot = vObjRotation - vEditRotation;
		if	(abs(vDiffRot.x) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_X) != GetFocus())
			m_Edit_Rotation_X.SetString(CUtility::RoundWString(to_wstring(vObjRotation.x), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_X) == GetFocus())
			m_pPickObject->Set_Rotation(E_ROTATION::ROT_X, (_float)_wtof(m_Edit_Rotation_X.GetString()));

		if	(abs(vDiffRot.y) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_Y) != GetFocus())
			m_Edit_Rotation_Y.SetString(CUtility::RoundWString(to_wstring(vObjRotation.y), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_Y) == GetFocus())
			m_pPickObject->Set_Rotation(E_ROTATION::ROT_Y, (_float)_wtof(m_Edit_Rotation_Y.GetString()));

		if	(abs(vDiffRot.z) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_Z) != GetFocus())
			m_Edit_Rotation_Z.SetString(CUtility::RoundWString(to_wstring(vObjRotation.z), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_ROTATION_Z) == GetFocus())
			m_pPickObject->Set_Rotation(E_ROTATION::ROT_Z, (_float)_wtof(m_Edit_Rotation_Z.GetString()));

		// Scale
		_vec3 vObjScale = VEC_ZERO;
		vObjScale = m_pPickObject->Get_Scale();
		_vec3 vEditScale = _vec3((_float)_wtof(m_Edit_Scale_X.GetString()), (_float)_wtof(m_Edit_Scale_Y.GetString()), (_float)_wtof(m_Edit_Scale_Z.GetString()));

		_vec3 vDiffScale = vObjScale - vEditScale;
		if (abs(vDiffScale.x) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_X) != GetFocus())
			m_Edit_Scale_X.SetString(CUtility::RoundWString(to_wstring(vObjScale.x), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_X) == GetFocus())
			m_pPickObject->Set_Scale(E_SCALE::SCALE_X, (_float)_wtof(m_Edit_Scale_X.GetString()));

		if	(abs(vDiffScale.y) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_Y) != GetFocus())
			m_Edit_Scale_Y.SetString(CUtility::RoundWString(to_wstring(vObjScale.y), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_Y) == GetFocus())
			m_pPickObject->Set_Scale(E_SCALE::SCALE_Y, (_float)_wtof(m_Edit_Scale_Y.GetString()));

		if	(abs(vDiffScale.z) > fStandard && GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_Z) != GetFocus())
			m_Edit_Scale_Z.SetString(CUtility::RoundWString(to_wstring(vObjScale.z), 2).c_str());
		else if (GetDlgItem(IDC_MH_POPUPTOOL_EDIT_SCALE_Z) == GetFocus())
			m_pPickObject->Set_Scale(E_SCALE::SCALE_Z, (_float)_wtof(m_Edit_Scale_Z.GetString()));

		UpdateData(FALSE);

		return;
	}

}

// 트리의 내용을 Update해야 하는 경우 
// 모든 레이어를 순회하면서 GetName()을 이용해 모든 오브젝트를 트리에 표기해준다.
void CPopupTool_ObjectTree::Update_Tree()
{
	UpdateData(TRUE);

	Clear_ListItemAndTree();
	m_tTreeCtrl.DeleteAllItems();
	TI_Obj.hInsertAfter = TVI_LAST;
	TI_Obj.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	//---------------------------------------------------------		루트 세팅
	
	// 트리 클리어

	// 씬이 없으면 강제종료
	CScene* pScene = Management Get_Scene();
	if (nullptr == pScene)
		return;

	for (_uint i = 0; i < (_uint)E_LAYERTAG::TAG_END; ++i)
	{
		TI_Obj.hParent = 0;
		
		switch ((E_LAYERTAG)i)
		{
		case Engine::E_LAYERTAG::Environment:		TI_Obj.item.pszText = L"LayerTag : Environment";			break;
		case Engine::E_LAYERTAG::GameLogic:			TI_Obj.item.pszText = L"LayerTag : GameLogic";				break;
		case Engine::E_LAYERTAG::UI:				TI_Obj.item.pszText = L"LayerTag : UI";						break;
		case Engine::E_LAYERTAG::Map:				TI_Obj.item.pszText = L"LayerTag : Map";					break;
		case Engine::E_LAYERTAG::Obstacle:			TI_Obj.item.pszText = L"LayerTag : Obstacle";				break;
		case Engine::E_LAYERTAG::Effect:			TI_Obj.item.pszText = L"LayerTag : Effect";					break;
		case Engine::E_LAYERTAG::Effect2:			TI_Obj.item.pszText = L"LayerTag : Effect2";				break;
		case Engine::E_LAYERTAG::Damage_Font:		TI_Obj.item.pszText = L"LayerTag : Damage_Font";			break;
		case Engine::E_LAYERTAG::UI_Lobby:			TI_Obj.item.pszText = L"LayerTag : UI_Lobby";				break;
		case Engine::E_LAYERTAG::UI_Combat:			TI_Obj.item.pszText = L"LayerTag : UI_Combat";				break;
		case Engine::E_LAYERTAG::Monster:			TI_Obj.item.pszText = L"LayerTag : Monster";				break;
		case Engine::E_LAYERTAG::Character:			TI_Obj.item.pszText = L"LayerTag : Character";				break;
		case Engine::E_LAYERTAG::AttackObj_Player:	TI_Obj.item.pszText = L"LayerTag : AttackObj_Player";		break;
		case Engine::E_LAYERTAG::AttackObj_Monster:	TI_Obj.item.pszText = L"LayerTag : AttackObj_Monster";		break;
		case Engine::E_LAYERTAG::TAG_END:			assert(FALSE);												break;
		default:									assert(FALSE);												break;
		}
		hRootLayer[i] = m_tTreeCtrl.InsertItem(&TI_Obj);
		m_tTreeCtrl.SetCheck(hRootLayer[i], TRUE);

		// 트리 최상위 부모 추가
		CLayer* pLayer = pScene->Get_Layer((E_LAYERTAG)i);
		if(nullptr == pLayer)
			continue;

		if (E_LAYERTAG::Damage_Font == (E_LAYERTAG)i)
			continue;

		// 오브젝트 vector순회
		for (_uint j = 0; j < (_uint)E_OBJTAG::OBJ_END; ++j)
		{
			vector<CGameObject*>* pVecObj = pLayer->Get_vecGameObject((E_OBJTAG)j);

			if (nullptr == pVecObj || (*pVecObj).empty())
				continue;

			for (_uint k = 0; k < (_uint)((*pVecObj).size()); ++k)
			{
				// 오브젝트 순회 오브젝트의 자식이 있다면 자식까지 추가 (재귀적으로)
				CGameObject* pObject = (*pVecObj)[k];

				wstring wstrName = pObject->Get_Name();
				switch (pObject->Get_ObjTag())
				{
				case Engine::E_OBJTAG::OBJ_BackGround:		wstrName.append(L"_BackGround");									break;
				case Engine::E_OBJTAG::OBJ_Player:			wstrName.append(L"_Player");										break;
				case Engine::E_OBJTAG::OBJ_DynamicCamera:	wstrName.append(L"_DynamicCamera");									break;
				case Engine::E_OBJTAG::OBJ_UI:				wstrName.append(L"_Ui");											break;
				case Engine::E_OBJTAG::OBJ_UI_SLIDER:		wstrName.append(L"_Ui_Slider");										break;
				case Engine::E_OBJTAG::OBJ_UI_SLIDER_BAR:	wstrName.append(L"_Slider_Bar");									break;
				case Engine::E_OBJTAG::OBJ_UI_SCROLLVIEW:	wstrName.append(L"_Ui_ScrollView");									break;
				case Engine::E_OBJTAG::OBJ_Map:				wstrName.append(L"_Map");											break;
				case Engine::E_OBJTAG::OBJ_Obstacle:		wstrName.append(L"_Obstacle");										break;
				case Engine::E_OBJTAG::OBJ_Test:			wstrName.append(L"_Test");											break;
				case Engine::E_OBJTAG::OBJ_Camera_Cafe:		wstrName.append(L"_Cafe_Camera");									break;
				case Engine::E_OBJTAG::OBJ_CafeTile:		wstrName.append(L"_CafeTile");										break;
				case Engine::E_OBJTAG::OBJ_BlockTile:		wstrName.append(L"_BlockTile");										break;
				case Engine::E_OBJTAG::OBJ_UI_BossHpbar:	wstrName.append(L"보스 체력바");										break;
				case Engine::E_OBJTAG::OBJ_BlockTile_L:		wstrName.append(L"_BlockTile_L");									break;
				case Engine::E_OBJTAG::OBJ_BlockTile_R:		wstrName.append(L"_BlockTile_R");									break;
				case Engine::E_OBJTAG::OBJ_Run_Player:		wstrName.append(L"_Run_Player");									break;
				case Engine::E_OBJTAG::OBJ_Run_Monster:		wstrName.append(L"_Run_Monster");									break;
				case Engine::E_OBJTAG::OBJ_Run_BossMonster:	wstrName.append(L"_Run_BossMonster");								break;
				case Engine::E_OBJTAG::OBJ_Run_Map:			wstrName.append(L"_Run_Map");										break;
				case Engine::E_OBJTAG::OBJ_END:				wstrName.append(L"_END (Tag작업안됌. 민현한테전달)");					break;
				default:									wstrName.append(L"_(새로 추가된 태그 오브젝트 (민현에게 전달))");		break;
				}
				TI_Obj.item.pszText = &wstrName[0];

				TI_Obj.hParent = hRootLayer[i];

				hLastTreeItem = m_tTreeCtrl.InsertItem(&TI_Obj);

				if (m_pPickObject == pObject)
				{
					m_tTreeCtrl.SetItemState(hLastTreeItem, TVIS_BOLD, TVIS_BOLD);
				}
				Add_ListItemAndTree(hLastTreeItem, pObject);
				m_tTreeCtrl.SetCheck(hLastTreeItem, pObject->Get_Active());

				if (E_OBJTAG::OBJ_UI				== pObject->Get_ObjTag() ||
					E_OBJTAG::OBJ_UI_SLIDER			== pObject->Get_ObjTag() ||
					E_OBJTAG::OBJ_UI_SLIDER_BAR		== pObject->Get_ObjTag() ||
					E_OBJTAG::OBJ_UI_SCROLLVIEW		== pObject->Get_ObjTag())
				{
					CUI_Object* pUi = static_cast<CUI_Object*>(pObject);

					Add_ChildInTree(hLastTreeItem, m_tTreeCtrl, pUi, 1);
				}
			}
		}
	}


	for (_int i = 0; i < (_int)E_LAYERTAG::TAG_END; ++i)
	{
		ExpandAll_Tree(hRootLayer[i], m_tTreeCtrl);
	}

	UpdateData(FALSE);
}

// UI_Object에서 자식은 Layer에 없기 때문에 재귀적으로 접근하는 함수를 만들어서
// 자식들을 모두 트리에 집어 넣는다.
HTREEITEM CPopupTool_ObjectTree::Add_ChildInTree(HTREEITEM hItem, CTreeCtrl & treeCtrl, CUI_Object * pUi, _int iDepth)
{
	const list<CUI_Object*>* pUiList = pUi->Get_ChildList();

	// 예외처리
	if (pUiList->empty() || 1 > (_int)pUiList->size())
		return hItem;

	_int iChildIndex = 0;

	for (auto& iter : (*pUiList))
	{
		_tchar szText[MAX_PATH]{};
		_tchar szName[MAX_PATH]{};

		swprintf_s(szName, iter->Get_Name().c_str());
		if (iter->Get_Name() == L"이름 없음")
		{
			switch (iter->Get_ObjTag())
			{
			case Engine::E_OBJTAG::OBJ_UI:				swprintf_s(szText, L"_Ui %d_%d", iDepth, iChildIndex);				break;
			case Engine::E_OBJTAG::OBJ_UI_SLIDER:		swprintf_s(szText, L"_Slider %d_%d", iDepth, iChildIndex);			break;
			case Engine::E_OBJTAG::OBJ_UI_SLIDER_BAR:	swprintf_s(szText, L"_Slider Bar %d_%d", iDepth, iChildIndex);		break;
			case Engine::E_OBJTAG::OBJ_UI_SCROLLVIEW:	swprintf_s(szText, L"_ScrollView %d_%d", iDepth, iChildIndex);		break;
			}
			lstrcat(szName, szText);
		}

		TI_Obj.item.pszText = szName;
		TI_Obj.hParent = hItem;
		hTempItem = m_tTreeCtrl.InsertItem(&TI_Obj);

		if (m_pPickObject == iter)
		{
			m_tTreeCtrl.SetItemState(hTempItem, TVIS_BOLD, TVIS_BOLD);
		}

		m_tTreeCtrl.SetCheck(hTempItem, iter->Get_Active());
		Add_ListItemAndTree(hTempItem, iter);
		Add_ChildInTree(hTempItem, treeCtrl, iter, iDepth + 1);

		++iChildIndex;
	}

	return hItem;
}

//	m_mapItemObject에 오브젝트와 아이템을 집어넣는다.
void CPopupTool_ObjectTree::Add_ListItemAndTree(HTREEITEM & hItem, CGameObject * pObject)
{
	m_mapItemObject.emplace(hItem, pObject);
}

//  m_mapItemObject 클리어
void CPopupTool_ObjectTree::Clear_ListItemAndTree()
{
	m_mapItemObject.clear();
}

// 트리를 클릭 했을 때 트리를 Update해준다. 선택한 Tree가 map에서 Object와 연결되었다면
// m_pPickObject를 해당 Object로 변경한다.
void CPopupTool_ObjectTree::Click_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	UINT nFlags = 0;
	POINT  pt = CUtility::Get_MousePoint(m_tTreeCtrl.m_hWnd);
	HTREEITEM hHitItem = m_tTreeCtrl.HitTest(pt, &nFlags);
	if (nullptr == hHitItem)
		return;

	_bool bCheckBox = FALSE;
	if (m_tTreeCtrl.GetCheck(hHitItem))
	{
		bCheckBox = TRUE;
	}

	for (_int i = 0; i < (_int)E_LAYERTAG::TAG_END; i++)
	{
		// 오브젝트 클릭이 아닌 Layer 클릭
		if (hHitItem == hRootLayer[i])
		{
			if ((nFlags & TVHT_ONITEMSTATEICON) != 0)
			{
				CLayer* pLayer = Management Get_Scene()->Get_Layer((E_LAYERTAG)i);
				if (nullptr == pLayer)
					return;

				pLayer->Set_Active(!bCheckBox);
				#ifdef _DEBUG
					if (!bCheckBox)
						cout << "레이어 Active TRUE로 변경 성공" << endl;
					else
						cout << "레이어 Active FALSE로 변경 성공" << endl;
				#endif 
			}
			m_pPickObject = nullptr;
			return;
		}
	}

	map<HTREEITEM, CGameObject*>::iterator iter = m_mapItemObject.find(hHitItem);
	if (iter == m_mapItemObject.end())
	{
		return;
	}

	m_pPickObject = iter->second;

	if ((nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		m_pPickObject->Set_Active(!bCheckBox);
		#ifdef _DEBUG
			if (!bCheckBox)
				cout << "오브젝트 Active TRUE로 변경 성공" << endl;
			else
				cout << "오브젝트 Active FALSE로 변경 성공" << endl;
		#endif 
	}

	if (Get_MFC_MainFrame()->m_pMenuForm->Get_CurTab(eToolMenu::UI))
	{
		Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Ui->Get_PickObject();
	}

	bUpdateNextFrame = TRUE;
}

// 오브젝트를 더블클릭하면 해당 위치로 카메라 이동
void CPopupTool_ObjectTree::DbClick_PupupToolObjectTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (nullptr == m_pPickObject)
		return;

	CCamera::DebugCam_MoveToTarget(m_pPickObject->Get_Info(E_INFO::INFO_POS));

	*pResult = 0;
}

// Update버튼을 클릭하면 트리를 업데이트
void CPopupTool_ObjectTree::ObjectTree_Button_Update()
{
	Update_Tree();
}

// Console창 클리어 버튼
void CPopupTool_ObjectTree::Button_Console_Clear()
{
	system("cls");
	Printf_Console_Description();
}

// Texture Popup창 켜기
void CPopupTool_ObjectTree::Button_Show_Popup_Texture()
{
	Get_MFC_MainFrame()->m_pMenuForm->Popup_TextureTool();
}

// 선택한 오브젝트 Set_Dead
void CPopupTool_ObjectTree::Button_SelectObjectSetDead()
{
	if (nullptr == m_pPickObject)
		return;

	m_pPickObject->Set_Dead(TRUE);
	m_pPickObject = nullptr;
	Update_Tree();
	bUpdateNextFrame = TRUE;
}
