
#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Animation.h"
#include "afxdialogex.h"
#include "Engine_Define.h"
#include "DMesh_Test.h"

// CTool_Animation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTool_Animation, CDialogEx)

CTool_Animation::CTool_Animation(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_ANIMATION, pParent)
	, m_str_Animation_Frame(_T(""))
	, m_str_Animation_Time(_T(""))
{

}

CTool_Animation::CTool_Animation(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_PARTICLE, pParent)
	, m_pPopupTool(pPopup)
{

}

CTool_Animation::~CTool_Animation()
{
}

void CTool_Animation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MH_ANIMATIONTOOL_ANIMATION_LIST, m_ListBox_Animation_List);
	DDX_Control(pDX, IDC_MH_ANIMATIONTOOL_ANIMATION_SLIDER, m_Slider_AnimationRatio);
	DDX_Control(pDX, IDC_MH_1216_EDIT_ANIMATION_NOWFRAME, m_Edit_Animation_Time);
	DDX_Control(pDX, IDC_MH_1216_EDIT_ANIMATION_MAXFRAME, m_Edit_Animation_Frame);
	DDX_Text(pDX, IDC_MH_1216_EDIT_ANIMATION_MAXFRAME, m_str_Animation_Frame);
	DDX_Text(pDX, IDC_MH_1216_EDIT_ANIMATION_NOWFRAME, m_str_Animation_Time);
	DDX_Control(pDX, IDC_MH_1216_ANIMATION_TREE, m_tAnimationTree);
	DDX_Control(pDX, IDC_MH_1216_ANIMATION_LIST_BOX_CHANGED, m_ListBox_DMeshes);
	DDX_Control(pDX, IDC_MH_1217_CURRENT_ANIMATION_TRIGGER_LIST, m_ListBox_CurAnimTrg);
	DDX_Control(pDX, IDC_MH_1217_CURRENT_CAN_ADD_TRIGGER_LIST, m_ListBox_CanAddTrg);
	DDX_Control(pDX, IDC_MH_1222_RADIO_CHARATER_LIST, m_Radio_Character);
	DDX_Control(pDX, IDC_MH_0118_CHECKBOX_CAMLOOP, m_CheckBox_CamLoop);
}

BEGIN_MESSAGE_MAP(CTool_Animation, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_MH_ANIMATIONTOOL_ANIMATION_LIST, &CTool_Animation::DClick_AnimationList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_MH_ANIMATIONTOOL_ANIMATION_SLIDER, &CTool_Animation::Slider_DragAndClick)
	ON_BN_CLICKED(IDC_MH_1216_BUTTON_ANIMATION_PLAY_ONCE, &CTool_Animation::Button_Animation_Play_Once)
	ON_BN_CLICKED(IDC_MH_1216_BUTTON_ANIMATION_PLAY_AUTO, &CTool_Animation::Button_Animation_Play_Auto)
	ON_BN_CLICKED(IDC_MH_1216_BUTTON_ANIM_CAM_FRONT, &CTool_Animation::Button_Animation_CamMoveToFront)
	ON_BN_CLICKED(IDC_MH_1216_BUTTON_ANIMATION_STOP, &CTool_Animation::Button_Animation_Stop)
	ON_LBN_DBLCLK(IDC_MH_1216_ANIMATION_LIST_BOX_CHANGED, &CTool_Animation::DClick_DMesh_ListBox)
	ON_BN_CLICKED(IDC_MH_1217_BUTTON_ADD_TRIGGER_CURRENT_FRAME, &CTool_Animation::Button_Add_Current_AnimationFrame)
	ON_BN_CLICKED(IDC_MH_1217_BUTTON_ADD_ACTION_ANIM, &CTool_Animation::Button_AddAction)
	ON_NOTIFY(NM_CLICK, IDC_MH_1216_ANIMATION_TREE, &CTool_Animation::Click_AnimAction_Tree)
	ON_BN_CLICKED(IDC_MH_1217_BUTTON_ACTION_START, &CTool_Animation::Button_Start_ChooseAction)
	ON_BN_CLICKED(IDC_MH_1218_BUTTON_LOAD_ACTION_AND_TRIGGER, &CTool_Animation::Button_Load_Anim_ActNTrg)
	ON_BN_CLICKED(IDC_MH_1218_BUTTON_SAVE_ACTION_AND_TRIGGER, &CTool_Animation::Button_Save_Anim_ActNTrg)
	ON_LBN_SELCHANGE(IDC_MH_1217_CURRENT_ANIMATION_TRIGGER_LIST, &CTool_Animation::Button_CurTriggerList)
	ON_BN_CLICKED(IDC_MH_1222_RADIO_CHARATER_LIST, &CTool_Animation::Click_Character_Radio)
	ON_BN_CLICKED(IDC_MH_1222_RADIO_MONSTER_LIST, &CTool_Animation::Click_Monster_Radio)
END_MESSAGE_MAP()

// 테스트용 DMesh를 만들어주고 Tool의 옵션들을 초기화해준다.
BOOL CTool_Animation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(TRUE);
	Update_DMesh_ListBox();
	m_pDMesh = CDMesh_Test::Create(Get_MFC_MainFrame()->m_pGraphicDev, E_OBJTAG::OBJ_Test, L"테스트");
	m_Slider_AnimationRatio.SetRange(0, 100);
	m_Slider_AnimationRatio.SetScrollPos(0, 0, 1);
	m_str_Animation_Frame.SetString(L"프레임");
	m_str_Animation_Frame.SetString(L"시간");
	m_Radio_Character.SetCheck(TRUE);

	TI_Anim.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI_Anim.hInsertAfter = TVI_LAST;
	UpdateData(FALSE);
	return TRUE;
}

// 윈도우가 켜졌을 때 타이머를 켜준다.
void CTool_Animation::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetTimer(TIMERID, (_uint)(1.f / 60.f), NULL);

		UpdateData(TRUE);


		UpdateData(FALSE);
	}
	else
	{
		m_iLastPickAnimIndex = ANIM_ERROR;
	}
}

// DMesh가 있을 때만 동작한다. DMesh의 테스트를 위한 여러 동작들이 있다.
void CTool_Animation::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	if (NullCheck_DMesh())
	{
		CUtility::Print_Wstring_DMode(L"m_pDMesh is nullptr ");
		m_pDMesh->Find_TestMesh();
		return;
	}

	Update_DMesh_AnimationListBox();

	UpdateData(TRUE);
	if (TIMERID == nIDEvent)
	{
		m_pDMesh->Update_Object(m_fDeltaTime);
		m_pDMesh->LateUpdate_Object(m_fDeltaTime);

		if (m_pDMesh->m_bAction_OnOff)
		{
		}
		else
		{
			_bool	bAnimFinished = m_pDMesh->Get_DMesh()->Is_AnimationsetFinish();
			_float	fAnimRT = m_pDMesh->Get_DMesh()->Get_AnimRatio();
			switch (m_eAnimPlayType)
			{
			case Engine::E_AnimationPlayType::Stop:
			{
				if (m_bAnimSliderChange)
				{
					_float fAnimMaxTime = m_pDMesh->Get_DMesh()->Get_AnimMaxTime();
					m_pDMesh->Get_DMesh()->Set_AnimaRatio(fAnimMaxTime * m_Slider_AnimationRatio.GetPos() / 100.f);
					m_bAnimSliderChange = FALSE;
				}
			}
			break;
			case Engine::E_AnimationPlayType::Play_Once:
			{
				m_pDMesh->Get_DMesh()->Play_Animation(m_fDeltaTime);
				m_Slider_AnimationRatio.SetPos((_int)(fAnimRT * 100.f));
				if (bAnimFinished)
				{
					m_eAnimPlayType = E_AnimationPlayType::Stop;
					m_pDMesh->Get_DMesh()->Set_AnimaRatio(0.f);
					m_Slider_AnimationRatio.SetPos(0);
					return;
				}
			}
			break;
			case Engine::E_AnimationPlayType::Play_Loop:
			{
				m_pDMesh->Get_DMesh()->Play_Animation(m_fDeltaTime);
				m_Slider_AnimationRatio.SetPos((_int)(fAnimRT * 100.f));
				if (bAnimFinished)
				{
					if (m_CheckBox_CamLoop.GetCheck())
					{
						Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Camera->Click_Action();
					}
					m_pDMesh->Set_ActionByEnum((E_ACTION)m_pDMesh->Get_DMesh()->Get_OldAniIndex());
					m_pDMesh->Get_DMesh()->Set_AnimaRatio(0.f);
				}
			}
			break;
			default:
				assert(FALSE);
				break;
			}

		}
		
		OnTimer_Frame_Time();


		if (Management Key_Down(KEY_Delete) && GetDlgItem(IDC_MH_1217_CURRENT_ANIMATION_TRIGGER_LIST) == GetFocus())
		{
			_int iIndex = m_ListBox_CurAnimTrg.GetCurSel();
			if (iIndex < 0 || m_iLastPickAnimIndex == ANIM_ERROR)
			{
				CUtility::Print_Wstring_DMode(L"트리거 삭제 실패 - 인덱스 Error");
				return;
			}
			m_pDMesh->Delete_T_AnimTrgByIndex(m_iLastPickAnimIndex, iIndex);

			_int iCurAnimIdx = m_iLastPickAnimIndex;
			Update_AnimTrgListBox(iCurAnimIdx);
			m_iLastPickAnimIndex = iCurAnimIdx;
		}

		if (Management Key_Down(KEY_Delete) && GetDlgItem(IDC_MH_1216_ANIMATION_TREE) == GetFocus())
		{
			m_pDMesh->Delete_T_AnimActionSysByName(wstrLastPickActionName);
			wstrLastPickActionName = L"선택 안함";
			Update_ActionTree();
		}
	}
	UpdateData(FALSE);

}

// DMesh과 Null인지 혹은 DMeshCom이 Null인지 체크 한 후 Null이면 True 반환
_bool CTool_Animation::NullCheck_DMesh()
{
	if (nullptr == m_pDMesh)
		return TRUE;

	if (nullptr == m_pDMesh->Get_DMesh())
		return TRUE;

	return FALSE;
}

// 애니메이션 파일들의 이름을 Key Value 형태로 저장해준다.
// 추후 더블 클릭 시에 해당 Key를 통해 Value값에 해당하는 Mesh를 Load한다.
void CTool_Animation::Update_DMesh_ListBox()
{
	UpdateData(TRUE);
	m_ListBox_DMeshes.ResetContent();
	UpdateData(FALSE);
	m_vecKeyNameValueFullPath.clear();
	m_vecKeyNameValueFullPath.shrink_to_fit();
	_finddata_t findData;
	intptr_t lHandle = 0;
	_tchar* pPath = L"";

	if (m_bCharacterList)
	{
		lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Character/", "*.*", &findData);
		pPath = L"../../Reference/Resource/Character/";
	}
	else
	{
		lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Monster/", "*.*", &findData);
		pPath = L"../../Reference/Resource/Monster/";
	}

	intptr_t lResult = 0;

	while (-1 != lResult)
	{
		wchar_t szFullPath[MAX_PATH] = L"";
		wchar_t szName[MAX_PATH] = L"";

		lstrcpy(szFullPath, pPath);

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);
		lstrcat(szFullPath, szName);

		if (strcmp(".", findData.name) && strcmp("..", findData.name) && strcmp("Mouth.png", findData.name))
		{
			m_vecKeyNameValueFullPath.push_back(make_pair(szName, szFullPath));
			m_ListBox_DMeshes.AddString(szName);
		}
		lResult = _findnext(lHandle, &findData);
	}

	_findclose(lHandle);

}

// Mesh가 변했을 때 애니메이션 리스트를 초기화해준다.
void CTool_Animation::Update_DMesh_AnimationListBox()
{
	if (FALSE == m_bUpdateAnimList)
		return;

	m_ListBox_Animation_List.ResetContent();
	if (nullptr == m_pDMesh->Get_DMesh())
		return;

	_int iAnimCountMax = m_pDMesh->Get_DMesh()->Get_AnimCountMax();
	CAniCtrl* pAniCtrl = m_pDMesh->Get_DMesh()->Get_AniCtrl();
	LPD3DXANIMATIONSET pAnimSet = nullptr;

	_int iStack = 0;
	for (_int i = 0; i < iAnimCountMax; i++)
	{
		LPCSTR mChar = nullptr;
		pAniCtrl->Get_AnimationCtrl()->GetAnimationSet(i, &pAnimSet);
		mChar = pAnimSet->GetName();
		wstring wstrAnimationName = CUtility::CHAR_TO_WSTRING(mChar);
		if (i < 10)
		{
			wstrAnimationName = L"0" + wstring(to_wstring(i) + L"_" + wstrAnimationName);
		}
		else
		{
			wstrAnimationName = wstring(to_wstring(i) + L"_" + wstrAnimationName);
		}
		m_ListBox_Animation_List.AddString(wstrAnimationName.c_str());
		++iStack;
	}

	m_bUpdateAnimList = FALSE;
}

// OnTimer에서 현재 애니메이션의 Frame과 시간초를 표시해준다.
void CTool_Animation::OnTimer_Frame_Time()
{
	_float fCurrentRT = m_pDMesh->Get_DMesh()->Get_AnimRatio();
	_float fAnimMaxTime = m_pDMesh->Get_DMesh()->Get_AnimMaxTime();
	
	wstring wstrTime = CUtility::RoundWString(to_wstring(fAnimMaxTime * fCurrentRT), 2) + L" / " + CUtility::RoundWString(to_wstring(fAnimMaxTime), 2);
	m_str_Animation_Time.SetString(wstrTime.c_str());

	_int iMaxFrame = m_pDMesh->Get_DMesh()->Get_AnimMaxFrame();
	_int iCurFrame = m_pDMesh->Get_DMesh()->Get_AnimCurrentFrame();

	wstring wstrFrame = CUtility::RoundWString(to_wstring(iCurFrame), -1) + L" / " + CUtility::RoundWString(to_wstring(iMaxFrame), -1);
	m_str_Animation_Frame.SetString(wstrFrame.c_str());

}



// Mesh 리스트박스를 선택하면 해당 Mesh를 Load하고 트리리스트, 액션, 애니메이션 리스트를 초기화한다.
void CTool_Animation::DClick_DMesh_ListBox()
{
	_int iIndex = m_ListBox_DMeshes.GetCurSel();

	if (0 > iIndex || (_int)m_vecKeyNameValueFullPath.size() <= iIndex)
		return;

	wstring wMeshName = m_vecKeyNameValueFullPath[iIndex].first;
	wstring wMeshFullPath = m_vecKeyNameValueFullPath[iIndex].second;
	wMeshFullPath.append(L"/");
	wMeshName.append(L".X");
	m_pDMesh->Set_DMesh(wMeshFullPath, wMeshName);

	m_bUpdateAnimList = TRUE;
	Update_DMesh_AnimationListBox();

	m_iLastPickAnimIndex = ANIM_ERROR;
	hLastFocusItem = nullptr;

	Clear_TrgListBox();
	Update_ActionTree();
}

// 애니메이션 리스트를 더블클릭하면 해당 애니메이션을 실행한다.
void CTool_Animation::DClick_AnimationList()
{
	_int iIndex = m_ListBox_Animation_List.GetCurSel();

	if (NullCheck_DMesh())
		return;


	if (0 > iIndex || (_int)m_pDMesh->Get_DMesh()->Get_AnimCountMax() <= iIndex)
		return;

	CUtility::Print_Wstring_DMode(L"Animation Index " + to_wstring(iIndex) + L"]");

	m_pDMesh->Set_AnimForMFC(iIndex);
	m_pDMesh->Get_DMesh()->Get_AniCtrl()->Set_AnimaRatioFor_Stop();
	m_pDMesh->Get_DMesh()->Set_AnimaRatio(0.f);
	m_bAnimSliderChange = TRUE;
	m_eAnimPlayType = E_AnimationPlayType::Play_Once;

	m_iLastPickAnimIndex = iIndex;

	// 처음 실행하는 경우 카메라 옮겨줌
	if (m_bFirstAnimSet)
	{
		Button_Animation_CamMoveToFront();
		m_bFirstAnimSet = FALSE;
	}
	Clear_TrgListBox();
	Update_ActionTree();
}

// 슬라이더가 변하면 Change를 True로
void CTool_Animation::Slider_DragAndClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_bAnimSliderChange = TRUE;
}

// 1회만 애니메이션 실행 버튼
void CTool_Animation::Button_Animation_Play_Once()
{
	if (NullCheck_DMesh())
		return;
	m_eAnimPlayType = E_AnimationPlayType::Play_Once;
	m_pDMesh->Set_ActionByEnum((E_ACTION)m_pDMesh->Get_DMesh()->Get_OldAniIndex());
	m_pDMesh->Get_DMesh()->Set_AnimaRatio(0.f);
}

// 반복 애니메이션 실행 버튼
void CTool_Animation::Button_Animation_Play_Auto()
{
	if (NullCheck_DMesh())
		return;
	m_pDMesh->Set_ActionByEnum((E_ACTION)m_pDMesh->Get_DMesh()->Get_OldAniIndex());
	m_eAnimPlayType = E_AnimationPlayType::Play_Loop;
}

// 애니메이션 멈추기 버튼
void CTool_Animation::Button_Animation_Stop()
{
	if (NullCheck_DMesh())
		return;
	m_eAnimPlayType = E_AnimationPlayType::Stop;
	_float	fAnimRT = m_pDMesh->Get_DMesh()->Get_AnimRatio();
	m_Slider_AnimationRatio.SetPos((_int)(fAnimRT * 100.f));
}

// 카메라가 Mesh의 정면으로 가는 버튼
void CTool_Animation::Button_Animation_CamMoveToFront()
{
	if (NullCheck_DMesh())
		return;

	_vec3 vModelPos = m_pDMesh->Get_Info(E_INFO::INFO_POS);
	_vec3 vModelLook = m_pDMesh->Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vModelLook, &vModelLook);

	CDynamicCamera* pDynamicCam = static_cast<CDynamicCamera*>(
		Management Get_Scene()->Get_Layer(E_LAYERTAG::GameLogic)->Get_GameObject(E_OBJTAG::OBJ_DynamicCamera));

	pDynamicCam->Set_CamAt(vModelPos);
	pDynamicCam->Set_CamEye(vModelPos + vModelLook * 1.5f + _vec3(0.f, 0.5f, 0.f));
}

// ListBox에 들어있는 트리거 이름들을 초기화한다.
void CTool_Animation::Clear_TrgListBox()
{
	UpdateData(TRUE);
	m_ListBox_CanAddTrg.ResetContent();
	m_ListBox_CurAnimTrg.ResetContent();
	UpdateData(FALSE);

	vector<pair<E_Trigger, std::function<void()>>>* pMap = m_pDMesh->Get_vecTrg();

	for (auto& iter : *pMap)
	{
		m_ListBox_CanAddTrg.AddString(CUtility::Get_TriggerName(iter.first).c_str());
	}

	_int iCurAnimIdx = m_ListBox_Animation_List.GetCurSel();
	Update_AnimTrgListBox(iCurAnimIdx);
}

// Animation을 클릭하면 해당 애니메이션 트리거를 리스트박스에 표시해준다.
void CTool_Animation::Update_AnimTrgListBox(_int iAnimIndex)
{
	if (NullCheck_DMesh())
		return;

	UpdateData(TRUE);
	m_ListBox_CurAnimTrg.ResetContent();

	if (TRIGGER_DEFAULT != iAnimIndex)
	{
		list<T_AnimTrg>::iterator iter = (*m_pDMesh->Get_AnimTrg())[iAnimIndex].begin();

		for (; iter != (*m_pDMesh->Get_AnimTrg())[iAnimIndex].end(); ++iter)
		{
			m_ListBox_CurAnimTrg.AddString(wstring(to_wstring(iter->iTriggerFrame) + L" " + CUtility::Get_TriggerName(iter->eTrigger)).c_str());
		}
	}
	
	UpdateData(FALSE);
}

// 액션트리를 초기화해준다.
void CTool_Animation::Update_ActionTree()
{
	if (NullCheck_DMesh())
		return;

	UpdateData(TRUE);
	CUtility::Print_Wstring_DMode(L"실행 Tool_Animation - Update_ActionTree");
	m_tAnimationTree.DeleteAllItems();


	vector<T_AnimActionSys>*	pActionVec = m_pDMesh->Get_AactionVec();

	_int iActionSize = (_int)(*pActionVec).size();
	_tchar szActionName[MAX_PATH]{};
	for (_int i = 0; i < iActionSize; ++i)
	{
		TI_Anim.hParent = 0;
		swprintf_s(szActionName, (*pActionVec)[i].wstrActionKey.c_str());
		TI_Anim.item.pszText = szActionName;
		hParentItem = m_tAnimationTree.InsertItem(&TI_Anim);
		TI_Anim.hParent = hParentItem;

		queue<_int> qAnimIndex = (*pActionVec)[i].qAnimation;
		_int iSize = (_int)qAnimIndex.size();
		for (_int  j = 0; j < iSize; ++j)
		{
			_int iAnimIndex = qAnimIndex.front();
			qAnimIndex.pop();

			CAniCtrl* pAniCtrl = m_pDMesh->Get_DMesh()->Get_AniCtrl();
			LPCSTR mChar = nullptr;
			LPD3DXANIMATIONSET pAnimSet = nullptr;
			pAniCtrl->Get_AnimationCtrl()->GetAnimationSet(iAnimIndex, &pAnimSet);
			mChar = pAnimSet->GetName();
			wstring wstrAnimationName = CUtility::CHAR_TO_WSTRING(mChar);
			swprintf_s(szActionName, wstrAnimationName.c_str());
			TI_Anim.item.pszText = szActionName;
			m_tAnimationTree.InsertItem(&TI_Anim);
		}
		ExpandAll_Tree(hParentItem, m_tAnimationTree);
	}

	UpdateData(FALSE);
}

// 현재 애니메이션 프레임에 선택한 트리거를 추가한다.
void CTool_Animation::Button_Add_Current_AnimationFrame()
{
	if (NullCheck_DMesh())
		return;

	// 애니메이션 리스트 인덱스
	_int iCurAnimIdx = m_ListBox_Animation_List.GetCurSel();

	if (0 > iCurAnimIdx || (_int)m_pDMesh->Get_DMesh()->Get_AnimCountMax() <= iCurAnimIdx)
	{
		CUtility::Print_Wstring_DMode(L"애니메이션 인덱스 문제로 인해 return  [Index : " + to_wstring(iCurAnimIdx) + L"]");
		return;
	}

	// 선택 트리거 인덱스 
	_int iTrgIndex = m_ListBox_CanAddTrg.GetCurSel();

	if (0 > iTrgIndex || m_ListBox_CanAddTrg.GetCount() <= iTrgIndex)
	{
		CUtility::Print_Wstring_DMode(L"선택 트리거 인덱스 문제로 인해 return  [Index : " + to_wstring(iTrgIndex) + L"]");
		return;
	}

	_int iMaxFrame = m_pDMesh->Get_DMesh()->Get_AnimMaxFrame();
	_int iCurFrame = m_pDMesh->Get_DMesh()->Get_AnimCurrentFrame();
	_float fCurrentRT = m_pDMesh->Get_DMesh()->Get_AnimRatio();
	_float fAnimMaxTime = m_pDMesh->Get_DMesh()->Get_AnimMaxTime();
	T_AnimTrg tAnimTrg;
	tAnimTrg.iTrgIndex = iTrgIndex;
	tAnimTrg.iTriggerFrame = iCurFrame;
	tAnimTrg.iMaxFrame = iMaxFrame;
	tAnimTrg.fActiveDeltaTime = fAnimMaxTime * fCurrentRT;
	tAnimTrg.eTrigger = (*m_pDMesh->Get_vecTrg())[iTrgIndex].first;

	(*m_pDMesh->Get_AnimTrg())[iCurAnimIdx].push_back(tAnimTrg);

	Update_AnimTrgListBox(iCurAnimIdx);
}

// 현재 선택한 애니메이션을 선택한 액션에 추가한다.
void CTool_Animation::Button_AddAction()
{
	if (NullCheck_DMesh())
		return;

	if (ANIM_ERROR == m_iLastPickAnimIndex)
	{
		CUtility::Print_Wstring_DMode(L"애니메이션을 선택해 주세요");
		return;
	}

	if (nullptr == hLastFocusItem)
	{
		CUtility::Print_Wstring_DMode(L"액션의 부모를 선택해 주세요");
		return;
	}

	CString strActionName = m_tAnimationTree.GetItemText(hLastFocusItem);

	CUtility::Print_Wstring_DMode(L"액션 : " + wstring(strActionName) + L"에 애니메이션 " + to_wstring(m_iLastPickAnimIndex) + L"추가");
	m_pDMesh->Add_AnimAction(wstring(strActionName), m_iLastPickAnimIndex);
	Update_ActionTree();
	hLastFocusItem = nullptr;
}

// 액션트리를 선택하고 저장해준다.
void CTool_Animation::Click_AnimAction_Tree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	UINT nFlags = 0;
	POINT  pt = CUtility::Get_MousePoint(m_tAnimationTree.m_hWnd);
	hLastFocusItem = m_tAnimationTree.HitTest(pt, &nFlags);

	HTREEITEM hParent = m_tAnimationTree.GetParentItem(hLastFocusItem);

	if (0 != hParent) 
	{
		CUtility::Print_Wstring_DMode(L"액션의 자식을 선택했습니다. 자동으로 부모액션을 선택합니다. ");
		hLastFocusItem = hParent;

		wstrLastPickActionName = m_tAnimationTree.GetItemText(hLastFocusItem);
		return;
	}
	else
	{
		wstrLastPickActionName = m_tAnimationTree.GetItemText(hLastFocusItem);
	}
}

// 액션에 들어있는 애니메이션들을 m_pDMesh의 큐에넣어서 실행한다.
void CTool_Animation::Button_Start_ChooseAction()
{
	if (NullCheck_DMesh())
		return; 

	wstring wstrPickActionName = m_tAnimationTree.GetItemText(hLastFocusItem);

	m_pDMesh->Choose_ActionForTest(wstrPickActionName);
}

// 액션과 트리거를  Save한다.
void CTool_Animation::Button_Save_Anim_ActNTrg()
{
	if (NullCheck_DMesh())
		return;

	CFileDialog Dlg(FALSE, L"ChrAnimdat", L"*.ChrAnimdat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Unit\\ActionTrigger");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		m_pDMesh->Save(hFile, dwByte);

		CloseHandle(hFile);
	}
}

// 액션과 트리거를  Load한다.
void CTool_Animation::Button_Load_Anim_ActNTrg()
{
	if (nullptr == m_pDMesh)
		return;

	CFileDialog Dlg(TRUE, L"ChrAnimdat", L"*.ChrAnimdat", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Unit\\ActionTrigger");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		m_pDMesh->Load(hFile, dwByte);

		CloseHandle(hFile);
	}

	Clear_TrgListBox();
	Update_ActionTree();
}


void CTool_Animation::Button_CurTriggerList()
{
}


// 캐릭터 버튼을 누르면 캐릭터들만 리스트박스에 추가한다.
void CTool_Animation::Click_Character_Radio()
{
	m_bCharacterList = TRUE;
	Update_DMesh_ListBox();
}

// 몬스터 버튼을 누르면 몬스터들만 리스트박스에 추가한다.
void CTool_Animation::Click_Monster_Radio()
{
	m_bCharacterList = FALSE;
	Update_DMesh_ListBox();
}
