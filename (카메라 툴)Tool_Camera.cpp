/*       �ڵ� ���÷μ� �ݺ��Ǵ� �ʿ���� �ڵ带 �Ϻ� �����߽��ϴ�.      */	

#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Camera.h"
#include "afxdialogex.h"

#include "MFC_CamControl_Eye.h"
#include "MFC_CamControl_At.h" 
#include "MFC_Line.h"

#include "Scene_Manager.h"
#include "Line.h"


// CTool_Camera ��ȭ �����Դϴ�.

USING(Engine)

IMPLEMENT_DYNAMIC(CTool_Camera, CDialogEx)

CTool_Camera::CTool_Camera(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_CAMERA, pParent)
	, m_cstrCamNameInsert(_T(""))
	, m_cstrControlSpeed(_T(""))
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_cstrAllSpeed(_T(""))
	, m_fMainCamSpeed(0)
{
}

CTool_Camera::CTool_Camera(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Camera::~CTool_Camera()
{
	Free();
}

void CTool_Camera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JY_CAM_List_ActionCam, m_ListCam);
	DDX_Control(pDX, IDC_JY_CAM_List_ControlPointEyeList, m_ListControlPoint_Eye);
	DDX_Text(pDX, IDC_JY_CAM_Edit_NameInsert, m_cstrCamNameInsert);
	DDX_Text(pDX, IDC_JY_CAM_Edit_ControlSpeed, m_cstrControlSpeed);
	DDX_Control(pDX, IDC_JY_CAM_Combo_ActionType, m_ComboCamType);
	DDX_Control(pDX, IDC_JY_CAM_List_ControlPointAtList, m_ListControlPoint_At);
	DDX_Text(pDX, IDC_JY_CAM_Edit_PosX, m_fPosX);
	DDX_Text(pDX, IDC_JY_CAM_Edit_PosY, m_fPosY);
	DDX_Text(pDX, IDC_JY_CAM_Edit_PosZ, m_fPosZ);
	DDX_Control(pDX, IDC_JY_CAM_Edit_PosX, m_EditPosX);
	DDX_Control(pDX, IDC_JY_CAM_Edit_PosY, m_EditPosY);
	DDX_Control(pDX, IDC_JY_CAM_Edit_PosZ, m_EditPosZ);
	DDX_Control(pDX, IDC_JY_CAM_Combo_Deploy_1216, m_Combo_Deploy);
	DDX_Control(pDX, IDC_JY_CAM_Edit_ControlSpeed, m_EditSpeed);
	DDX_Control(pDX, IDC_JY_CAM_Edit_NameInsert, m_EditCamName);
	DDX_Control(pDX, IDC_JY_CAM_ALLSPEED_0107, m_EditAllSpeed);
	DDX_Text(pDX, IDC_JY_CAM_ALLSPEED_0107, m_cstrAllSpeed);
	DDX_Control(pDX, IDC_JY_CAM_CheckFreeCam_0122, m_ButtonCheckFreeCam);
	DDX_Text(pDX, IDC_JY_CAM_MainCamSpeed_0122, m_fMainCamSpeed);
	DDX_Control(pDX, IDC_JY_CAM_MainCamSpeed_0122, m_EditMainCamSpeed);
}


BEGIN_MESSAGE_MAP(CTool_Camera, CDialogEx)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_AddCam, &CTool_Camera::OnBnClickedCamAdd)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_DeleteCam, &CTool_Camera::OnBnClickedCamDelete)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_DeleteControl, &CTool_Camera::OnBnClickedControlPointDelete)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_SceneLoad, &CTool_Camera::OnBnClickedCamSceneLoad)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_Save, &CTool_Camera::OnBnClickedCamToolSAVE)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_Load, &CTool_Camera::OnBnClickedCamToolLOAD)
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_JY_CAM_List_ControlPointEyeList, &CTool_Camera::OnSelchangeControlPointList_Eye)
	ON_LBN_SELCHANGE(IDC_JY_CAM_List_ActionCam, &CTool_Camera::OnSelchangeCamList)
	ON_CBN_SELCHANGE(IDC_JY_CAM_Combo_ActionType, &CTool_Camera::OnCbnSelchangeCamType)
	ON_LBN_SELCHANGE(IDC_JY_CAM_List_ControlPointAtList, &CTool_Camera::OnSelchangeControlPointList_At)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_PosApply, &CTool_Camera::OnBnClickedControlPointPosApply)
	ON_CBN_SELCHANGE(IDC_JY_CAM_Combo_Deploy_1216, &CTool_Camera::OnCbnSelchangeDeployType)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_ActionStart, &CTool_Camera::OnBnClickedActionStart)
	ON_BN_CLICKED(IDC_JY_CAM_Btn_SpeedApply, &CTool_Camera::OnBnClickedSpeedApply)
	ON_EN_CHANGE(IDC_JY_CAM_Edit_ControlSpeed, &CTool_Camera::OnEnChangeCurPointSpeed)
	ON_EN_CHANGE(IDC_JY_CAM_ALLSPEED_0107, &CTool_Camera::OnEnChangeAllChange)
	ON_EN_CHANGE(IDC_JY_CAM_Edit_PosX, &CTool_Camera::OnEditChangePosX)
	ON_EN_CHANGE(IDC_JY_CAM_Edit_PosY, &CTool_Camera::OnEditChangePosY)
	ON_EN_CHANGE(IDC_JY_CAM_Edit_PosZ, &CTool_Camera::OnEditChangePosZ)
	ON_BN_CLICKED(IDC_BUTTON_JY_CAM_PickObjPosApply_0122, &CTool_Camera::OnBnClickedButtonPickObjPosApply)
	ON_BN_CLICKED(IDC_JY_CAM_CheckFreeCam_0122, &CTool_Camera::OnBnClickedJyCamCheckfreecam0122)
	ON_EN_CHANGE(IDC_JY_CAM_MainCamSpeed_0122, &CTool_Camera::OnEnChangeJyCamMaincamspeed0122)
END_MESSAGE_MAP()


// CTool_Camera �޽��� ó�����Դϴ�.

// Ÿ�̸� ����, �޺��ڽ��� String �߰� ��
HRESULT CTool_Camera::Ready_DefaultSettings()
{
	SetTimer(m_iTimerEvent, 10, NULL);

	m_pGraphicDev = Management GetDevice();
	m_pGraphicDev->AddRef();

	m_ComboCamType.AddString(L"Normal");
	m_ComboCamType.AddString(L"Observe");
	m_ComboCamType.AddString(L"Subject");

	m_ComboCamType.SetCurSel(0);
	m_eCamActionType = (ActionType)m_ComboCamType.GetCurSel();

	m_Combo_Deploy.AddString(L"Normal");
	m_Combo_Deploy.AddString(L"One_At");

	m_Combo_Deploy.SetCurSel(0);
	m_eDeployType = (DeployType)m_Combo_Deploy.GetCurSel();
	m_ePreDeployType = m_eDeployType;

	return S_OK;
}

// ���� �ٲ� ������ ���� ����
HRESULT CTool_Camera::Ready_Line()
{
	if (!Management Get_Scene())
	{
		return S_OK; 
	}

	CGameObject*	pGameObject = CMFC_Line::Create(m_pGraphicDev, E_OBJTAG::OBJ_Test);
	// (�� �ڵ� ��Ģ) assert�� ���� ������ �߻��� �� �ִ� �κ� ĳġ 
	assert(pGameObject);
	m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject(E_OBJTAG::OBJ_Test, pGameObject);

	// (�� �ڵ� ��Ģ) ��� ���谡 �и��ϱ� ������ dynamic_cast���� static_cast ��� 
	m_pEyeLine = static_cast<CMFC_Line*>(pGameObject);
	assert(m_pEyeLine);

	return S_OK;
}

// ���� ����� ������ MapŬ���� + ���� ���� 
void CTool_Camera::Scene_Change()
{
	m_eCurScene = CScene_Manager::GetInstance()->Get_SceneTag();

	if (m_eCurScene != m_ePreScene)
	{
		m_cstrCamCurText = L"";

		m_pScene = Management Get_Scene();
		assert(m_pScene);

		Ready_Line();

		Delete_MapClear();
		m_ePreScene = m_eCurScene;
	}
}

// ī�޶� ��� ����� ������ üũ 
void CTool_Camera::Mode_Change()
{
	if (m_eCamActionType != m_ePreCamActionType)		// FSM �������� ķ ��� ���� üũ
	{
		switch (m_ePreCamActionType)
		{
		case CTool_Camera::Action_Normal:
			break;

		case CTool_Camera::Action_Observe:
			if (m_pMainCam)
			{
				m_pMainCam->Set_CamOriginSettings();
			}
			break;

		case CTool_Camera::Action_Subject:
			break;

		case CTool_Camera::Action_End:
			break;

		default:
			assert(0);		// �����ϸ� �ȵǴ� ����Ʈ �� ���� ó�� 
			break;
		}

		m_ePreCamActionType = m_eCamActionType;
	}
}

// G�� ������ ������ ī�޶� ��Ʈ���� ���� �Լ� 
void CTool_Camera::Create_ControlSphere()
{
	m_pScene = Management Get_Scene();

	if (m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Get_GameObject(E_OBJTAG::OBJ_DynamicCamera))
	{
		if (Deploy_Normal == m_eDeployType)
		{
			// Control_Eye 
			CGameObject*	pGameObject =
				CMFC_CamControl_Eye::Create(
					m_pGraphicDev,
					E_OBJTAG::OBJ_CamControl_Eye,
					static_cast<CDynamicCamera*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera))->Get_CamEye(),
					0.1f);
			assert(pGameObject);
			m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject(E_OBJTAG::OBJ_CamControl_Eye, pGameObject);

			// CTOWSTR�� CString�� wstring���� ��ȯ���ִ� �Լ��� define�ؼ� ��� 
			m_mapActionEye[m_cstrCamCurText.CTOWSTR].emplace_back(static_cast<CMFC_CamControl_Eye*>(pGameObject));
			m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].resize(m_mapActionEye[m_cstrCamCurText.CTOWSTR].size());
			m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].emplace_back(1.f);

			// Control_At 
			pGameObject =
				CMFC_CamControl_At::Create(
					m_pGraphicDev,
					E_OBJTAG::OBJ_CamControl_At,
					static_cast<CDynamicCamera*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera))->Get_CamAt(),
					0.1f);
			assert(pGameObject);
			m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject(E_OBJTAG::OBJ_CamControl_At, pGameObject);

			m_mapActionAt[m_cstrCamCurText.CTOWSTR].emplace_back(static_cast<CMFC_CamControl_At*>(pGameObject));

			// List�� String �߰� 
			WCHAR	wszBuff[16];
			swprintf_s(wszBuff, L"%zu", m_mapActionEye[m_cstrCamCurText.CTOWSTR].size() - 1);
			m_ListControlPoint_Eye.AddString(wszBuff);
			m_ListControlPoint_At.AddString(wszBuff);

			m_mapDeployType[m_cstrCamCurText.CTOWSTR] = Deploy_Normal;
		}
	}
}

// ī�޶� ��Ʈ�� ListBox�� String���� ���ΰ�ħ 
void CTool_Camera::Refresh_ControlListString()
{
	m_ListControlPoint_Eye.ResetContent();
	m_ListControlPoint_At.ResetContent();

	WCHAR	wszBuff[16];

	for (size_t i = 0; i < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size(); ++i)
	{
		swprintf_s(wszBuff, L"%zu", i);

		m_ListControlPoint_Eye.AddString(wszBuff);

		if (1 < m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
		{
			m_ListControlPoint_At.AddString(wszBuff);
		}
	}

	if (1 == m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
	{
		swprintf_s(wszBuff, L"%d", 0);

		m_ListControlPoint_At.AddString(wszBuff);
	}
}

// ī�޶� ������(���� �ε������� ���� �ε�������)
void CTool_Camera::MoveOn_Line()
{
	// ��� ������ �Լ� �۵��ϸ� �� �� �ٷ� return; 
	if (m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].empty())
	{
		return;
	}

	m_fProgressCam = CUtility::Clamp_float(m_fProgressCam, 0.f, 1.f);

	Eye_Line();
	At_Line();

	if (1.f <= m_fProgressCam)
	{
		m_fProgressCam = 0.f;
		m_fTime = 0.f;

		if ((_int)m_mapActionEye[m_cstrCamCurText.CTOWSTR].size() - 3 > m_iCurLineIndex)
		{
			++m_iCurLineIndex;
		}
		else
		{
			m_eCamActionType = Action_Normal;
		}
	}

	m_fTime += m_fTimeDelta;
	m_fProgressCam = m_fTime / m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR][m_iCurLineIndex];
}

// �����(Eye)
void CTool_Camera::Eye_Line()
{
	vector<CMFC_CamControl_Eye*>	vecTemp = m_mapActionEye[m_cstrCamCurText.CTOWSTR];

	if (3 < vecTemp.size())
	{
		if (0 < m_iCurLineIndex &&
			m_iCurLineIndex < vecTemp.size() - 2)
		{
			Make_Spline(&m_vLinePos,
				vecTemp[m_iCurLineIndex - 1]->Get_Info(E_INFO::INFO_POS),
				vecTemp[m_iCurLineIndex]->Get_Info(E_INFO::INFO_POS),
				vecTemp[m_iCurLineIndex + 1]->Get_Info(E_INFO::INFO_POS),
				vecTemp[m_iCurLineIndex + 2]->Get_Info(E_INFO::INFO_POS));
		}

		if (m_pEyeLine)
		{
			if (m_iCurLineIndex < vecTemp.size() - 3)
			{
				m_pEyeLine->Emplace_Line(m_vLinePos);
			}
		}

		switch (m_eCamActionType)
		{
		case CTool_Camera::Action_Normal:
			break;

		case CTool_Camera::Action_Observe:
			m_pMainCam->Set_CamAt(m_vLinePos);
			break;

		case CTool_Camera::Action_Subject:
			m_pMainCam->Set_CamEye(m_vLinePos);
			break;

		default:
			assert(0);
			break;
		}
	}
}

// ����� 
void CTool_Camera::Make_Spline(_vec3* vOutPos, const _vec3 & vPos0, const _vec3 & vPos1, const _vec3 & vPos2, const _vec3 & vPos3)
{
	_float fWeight = m_fProgressCam / 1.f;

	D3DXVec3CatmullRom(vOutPos, &vPos0, &vPos1, &vPos2, &vPos3, fWeight);
}

// ��ü ���� 
void CTool_Camera::Delete_All()
{
	if (!m_mapActionEye.empty() &&
		!m_mapActionAt.empty())
	{
		map<const wstring, vector<CMFC_CamControl_Eye*>>::iterator	iter_EyeMap = m_mapActionEye.begin();
		map<const wstring, vector<CMFC_CamControl_At*>>::iterator	iter_AtMap = m_mapActionAt.begin();

		for (; iter_EyeMap != m_mapActionEye.end(); ++iter_EyeMap)
		{
			for (size_t i = 0; i < iter_EyeMap->second.size(); ++i)
			{
				iter_EyeMap->second[i]->Set_Dead(TRUE);
			}
		}

		for (; iter_AtMap != m_mapActionAt.end(); ++iter_AtMap)
		{
			for (size_t i = 0; i < iter_AtMap->second.size(); ++i)
			{
				iter_AtMap->second[i]->Set_Dead(TRUE);
			}
		}

		Delete_MapClear();
	}
}

// Map Ŭ���� + ListBox Ŭ���� 
void CTool_Camera::Delete_MapClear()
{
	m_mapActionEye.clear();
	m_mapActionAt.clear();

	m_ListCam.ResetContent();
	m_ListControlPoint_Eye.ResetContent();
	m_ListControlPoint_At.ResetContent();
}

void CTool_Camera::Free()
{
	m_mapActionEye.clear();
	m_mapActionAt.clear();

	Safe_Release(m_pGraphicDev);
}

void CTool_Camera::Click_Action()
{
	switch ((ActionType)m_ComboCamType.GetCurSel())
	{
	case CTool_Camera::Action_Normal:
		m_eCamActionType = (ActionType)m_ComboCamType.GetCurSel();
		break;

	case CTool_Camera::Action_Subject:
		if (!m_mapActionEye.empty() &&
			!m_mapActionAt.empty() &&
			!m_mapActionEye[m_cstrCamCurText.CTOWSTR].empty())
		{
			m_eCamActionType = (ActionType)m_ComboCamType.GetCurSel();

			m_iCurLineIndex = 1;
			m_fProgressCam = 0.f;

			m_pMainCam = static_cast<CDynamicCamera*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, 0));
			assert(m_pMainCam);
			m_pMainCam->Set_CamEye(+m_mapActionEye[m_cstrCamCurText.CTOWSTR][0]->Get_Info(E_INFO::INFO_POS));

			if (m_pEyeLine)
			{
				m_pEyeLine->Clear_VecPoint();
			}
		}
		break;

	case CTool_Camera::Action_Observe:
		if (!m_mapActionEye.empty() &&
			!m_mapActionAt.empty() &&
			!m_mapActionEye[m_cstrCamCurText.CTOWSTR].empty())
		{
			m_eCamActionType = (ActionType)m_ComboCamType.GetCurSel();

			m_iCurLineIndex = 1;
			m_fProgressCam = 0.f;

			m_pMainCam = static_cast<CDynamicCamera*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, 0));
			assert(m_pMainCam);
			m_pMainCam->Set_CamAt(+m_mapActionEye[m_cstrCamCurText.CTOWSTR][0]->Get_Info(E_INFO::INFO_POS));

			if (m_pEyeLine)
			{
				m_pEyeLine->Clear_VecPoint();
			}
		}
		break;
	default:
		return;
		break;
	}
}


// ī�޶� �̺�Ʈ ����(�� �ȿ��� ��Ʈ�� ��ü���� ����)
void CTool_Camera::OnBnClickedCamAdd()
{
	UpdateData(TRUE);

	m_iCurLineIndex = 1;
	if (m_pEyeLine)
	{
		m_pEyeLine->Clear_VecPoint();
	}

	vector<CMFC_CamControl_Eye*>	vecEye;
	vector<CMFC_CamControl_At*>		vecAt;
	vecEye.reserve(30);
	vecAt.reserve(30);

	_bool	bDupl = false;
	CString	cstrName = L"";

	m_EditCamName.GetWindowTextW(cstrName);

	map<const wstring, vector<CMFC_CamControl_Eye*>>::iterator	iter = m_mapActionEye.begin();

	for (; iter != m_mapActionEye.end(); ++iter)
	{
		if (cstrName.CTOWSTR == iter->first)
		{
			bDupl = true;
		}
	}

	if (L"" != m_cstrCamNameInsert &&
		!bDupl)
	{
		m_mapActionEye.emplace(m_cstrCamNameInsert.CTOWSTR, vecEye);
		m_mapActionAt.emplace(m_cstrCamNameInsert.CTOWSTR, vecAt);

		m_ListCam.AddString(m_cstrCamNameInsert);
	}

	UpdateData(FALSE);

	m_EditCamName.SetWindowTextW(L"");
}

// ī�޶� �̺�Ʈ ���� 
void CTool_Camera::OnBnClickedCamDelete()
{
	if (-1 != m_iCamCurSel)
	{
		if (ActionType::Action_Subject != m_eCamActionType &&
			ActionType::Action_Observe != m_eCamActionType)
		{
			for (size_t i = 0; i < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size(); ++i)
			{
				m_mapActionEye[m_cstrCamCurText.CTOWSTR][i]->Set_Dead(TRUE);
			}
			for (size_t i = 0; i < m_mapActionAt[m_cstrCamCurText.CTOWSTR].size(); ++i)
			{
				m_mapActionAt[m_cstrCamCurText.CTOWSTR][i]->Set_Dead(TRUE);
			}

			m_mapActionEye.erase(m_cstrCamCurText.CTOWSTR);
			m_mapActionAt.erase(m_cstrCamCurText.CTOWSTR);
			m_mapSectionSpeed.erase(m_cstrCamCurText.CTOWSTR);

			m_ListCam.DeleteString(m_iCamCurSel);

			if (m_mapActionEye.empty() &&
				m_mapActionAt.empty())
			{
				Delete_MapClear();
			}

			m_ListControlPoint_Eye.ResetContent();
			m_ListControlPoint_At.ResetContent();

			m_iCurLineIndex = 1;

			if (m_pEyeLine)
			{
				m_pEyeLine->Clear_VecPoint();
			}
		}
	}
}

// ���� ������ ī�޶� ��Ʈ�� ���� 
void CTool_Camera::OnBnClickedControlPointDelete()
{
	if (-1 != m_iCamCurSel &&
		(-1 != m_iControlCurSel_Eye || -1 != m_iControlCurSel_At))
	{
		if (Action_Normal == m_eCamActionType)
		{
			if (1 < m_mapActionAt[m_cstrCamCurText.CTOWSTR].size() &&
				3 < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size())
			{
				if (-1 != m_iControlCurSel_Eye)
				{
					m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Dead(TRUE);
					m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Dead(TRUE);

					m_mapActionEye[m_cstrCamCurText.CTOWSTR].erase(m_mapActionEye[m_cstrCamCurText.CTOWSTR].begin() + m_iControlCurSel_Eye);
					m_mapActionAt[m_cstrCamCurText.CTOWSTR].erase(m_mapActionAt[m_cstrCamCurText.CTOWSTR].begin() + m_iControlCurSel_Eye);
					m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].erase(m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].begin() + m_iControlCurSel_Eye);

					m_ListControlPoint_Eye.DeleteString(m_iControlCurSel_Eye);
					m_ListControlPoint_At.DeleteString(m_iControlCurSel_Eye);
				}
			}
			else if (1 == m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
			{
				if (Control_EyePoint == m_eControlPointType)
				{
					if (3 < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size())
					{
						m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Dead(TRUE);
						m_mapActionEye[m_cstrCamCurText.CTOWSTR].erase(m_mapActionEye[m_cstrCamCurText.CTOWSTR].begin() + m_iControlCurSel_Eye);
						m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].erase(m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].begin() + m_iControlCurSel_Eye);

						m_ListControlPoint_Eye.DeleteString(m_iControlCurSel_Eye);
					}
				}
			}

			Refresh_ControlListString();

			if (m_pEyeLine)
			{
				m_pEyeLine->Clear_VecPoint();
			}

			m_iCurLineIndex = 1;
			m_iControlCurSel_Eye = -1;
			m_iControlCurSel_At = -1;
		}
	}
}

// ī�޶� ��Ʈ�� ��ġ���� ���� ��ư
void CTool_Camera::OnBnClickedControlPointPosApply()
{
	UpdateData(TRUE);

	_vec3	vPos = VEC_ZERO;
	vPos.x = m_fPosX;
	vPos.y = m_fPosY;
	vPos.z = m_fPosZ;

	_int	iCurSel = -1;
	CString	cstrPointName = L"";

	switch (m_eControlPointType)
	{
	case CTool_Camera::Control_EyePoint:
		iCurSel = m_ListControlPoint_Eye.GetCurSel();
		m_ListControlPoint_Eye.GetText(iCurSel, cstrPointName);

		m_mapActionEye[m_cstrCamCurText.CTOWSTR][iCurSel]->Set_Pos(vPos);
		break;

	case CTool_Camera::Control_AtPoint:
		iCurSel = m_ListControlPoint_At.GetCurSel();
		m_ListControlPoint_At.GetText(iCurSel, cstrPointName);

		m_mapActionAt[m_cstrCamCurText.CTOWSTR][iCurSel]->Set_Pos(vPos);
		break;

	case CTool_Camera::Control_End:
		break;

	default:
		assert(0);
		break;
	}

	UpdateData(FALSE);
}

// ī�޶� �̺�Ʈ ListBox���� ���� 
void CTool_Camera::OnSelchangeCamList()
{
	m_iCamCurSel = m_ListCam.GetCurSel();

	if (ActionType::Action_Observe != m_eCamActionType &&
		ActionType::Action_Subject != m_eCamActionType)
	{
		if (-1 != m_iCamCurSel)
		{
			m_ListCam.GetText(m_iCamCurSel, m_cstrCamCurText);

			map<const wstring, vector<CMFC_CamControl_Eye*>>::iterator		iter_Eye = m_mapActionEye.begin();
			map<const wstring, vector<CMFC_CamControl_At*>>::iterator		iter_At = m_mapActionAt.begin();

			for (; iter_Eye != m_mapActionEye.end(); ++iter_Eye)
			{
				if (m_cstrCamCurText.CTOWSTR == iter_Eye->first)
				{
					Refresh_ControlListString();

					if (0 < (iter_Eye->second).size())
					{
						for (size_t i = 0; i < iter_Eye->second.size(); ++i)
						{
							iter_Eye->second[i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NORMAL);
							if (i <= m_mapActionAt[m_cstrCamCurText.CTOWSTR].size() - 1)
							{
								m_mapActionAt[m_cstrCamCurText.CTOWSTR][i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NORMAL);
							}
						}
					}
				}
				else
				{
					for (size_t i = 0; i < iter_Eye->second.size(); ++i)
					{
						iter_Eye->second[i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NONE);
						if (i <= iter_At->second.size() - 1)
						{
							iter_At->second[i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NONE);
						}
					}
				}

				if (iter_At != m_mapActionAt.end())
				{
					++iter_At;
				}
			}

			if (m_pEyeLine)
			{
				m_pEyeLine->Clear_VecPoint();
			}

			m_iCurLineIndex = 1;

			m_eDeployType = m_mapDeployType[m_cstrCamCurText.CTOWSTR];
			m_ePreDeployType = m_eDeployType;
			m_Combo_Deploy.SetCurSel(m_eDeployType);
		}
	}
}

// ī�޶� ��Ʈ��(Eye)�� ListBox���� ���� 
void CTool_Camera::OnSelchangeControlPointList_Eye()
{
	m_eControlPointType = Control_EyePoint;

	m_iControlCurSel_Eye = m_ListControlPoint_Eye.GetCurSel();

	size_t	vecSize = m_mapActionEye[m_cstrCamCurText.CTOWSTR].size();

	if (-1 != m_iCamCurSel &&
		-1 != m_iControlCurSel_Eye)
	{
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_SELECTED);

		for (size_t i = 0; i < vecSize; ++i)
		{
			if (i != m_iControlCurSel_Eye)
			{
				m_mapActionEye[m_cstrCamCurText.CTOWSTR][i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NORMAL);
			}
		}
	}

	if (3 < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size())
	{
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][0]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][vecSize - 1]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][vecSize - 2]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
	}

	if (-1 != m_iControlCurSel_Eye)
	{
		// Pos Edit�� ������Ʈ 
		_vec3	vPos = m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Get_Info(E_INFO::INFO_POS);

		WCHAR	wszBuff[64];
		swprintf_s(wszBuff, L"%.2f", vPos.x);
		m_EditPosX.SetWindowTextW(wszBuff);
		swprintf_s(wszBuff, L"%.2f", vPos.y);
		m_EditPosY.SetWindowTextW(wszBuff);
		swprintf_s(wszBuff, L"%.2f", vPos.z);
		m_EditPosZ.SetWindowTextW(wszBuff);

		// Speed ������Ʈ 
		if (!m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].empty())
		{
			swprintf_s(wszBuff, L"%.2f", m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]);
		}
		else
		{
			swprintf_s(wszBuff, L"%.2f", 10.f);
		}

		m_EditSpeed.SetWindowTextW(wszBuff);
	}
}

// ī�޶� ��Ʈ��(At)�� ListBox���� ���� 
void CTool_Camera::OnSelchangeControlPointList_At()
{
	m_eControlPointType = Control_AtPoint;

	m_iControlCurSel_At = m_ListControlPoint_At.GetCurSel();

	size_t	vecSize = m_mapActionEye[m_cstrCamCurText.CTOWSTR].size();

	if (-1 != m_iCamCurSel &&
		-1 != m_iControlCurSel_At)
	{
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_SELECTED);

		for (size_t i = 0; i < m_mapActionEye[m_cstrCamCurText.CTOWSTR].size(); ++i)
		{
			if (i != m_iControlCurSel_At)
			{
				if (1 < m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
				{
					m_mapActionAt[m_cstrCamCurText.CTOWSTR][i]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NORMAL);
				}
				else if (1 == m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
				{
					m_mapActionAt[m_cstrCamCurText.CTOWSTR][0]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_NORMAL);
				}
			}
		}
	}

	if (1 < m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
	{
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][0]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][vecSize - 1]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][vecSize - 2]->Set_RenderType(E_WIRE_RENDER::WIRE_RENDER_INTERPOL);
	}

	if (-1 != m_iControlCurSel_At)
	{
		_vec3	vPos = m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Get_Info(E_INFO::INFO_POS);

		WCHAR	wszBuff[64];
		swprintf_s(wszBuff, L"%f", vPos.x);
		m_EditPosX.SetWindowTextW(wszBuff);
		swprintf_s(wszBuff, L"%f", vPos.y);
		m_EditPosY.SetWindowTextW(wszBuff);
		swprintf_s(wszBuff, L"%f", vPos.z);
		m_EditPosZ.SetWindowTextW(wszBuff);
	}
}

// ComboBox���� ī�޶� Ÿ�� ���� (�Ϲ�, �ְ�, ����)
void CTool_Camera::OnCbnSelchangeCamType()
{
	switch ((ActionType)m_ComboCamType.GetCurSel())
	{
	case CTool_Camera::Action_Normal:
		m_eCamActionType = (ActionType)m_ComboCamType.GetCurSel();
		break;

	case CTool_Camera::Action_Subject:
		break;

	case CTool_Camera::Action_Observe:
		break;

	default:
		assert(0);
		break;
	}
}

// ī�޶� ��ġ Ÿ�� ����(�Ϲݸ��, At �Ѱ��� ��ġ���)
void CTool_Camera::OnCbnSelchangeDeployType()
{
	// �Էµ� �ؽ�Ʈ�� ������ �ٷ� return; 
	if (L"" == m_cstrCamCurText)
	{
		return;
	}

	m_eDeployType = (DeployType)m_Combo_Deploy.GetCurSel();

	switch (m_eDeployType)
	{
	case CTool_Camera::Deploy_Normal:
		if (m_mapActionEye[m_cstrCamCurText.CTOWSTR].empty() ||
			1 != m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
		{
			m_ePreDeployType = m_eDeployType;
		}
		else
		{
			m_eDeployType = m_ePreDeployType;
			m_Combo_Deploy.SetCurSel((_int)m_ePreDeployType);
		}
		break;

	case CTool_Camera::Deploy_OneAt:
		if (m_mapActionEye[m_cstrCamCurText.CTOWSTR].empty() ||
			1 == m_mapActionAt[m_cstrCamCurText.CTOWSTR].size())
		{
			m_ePreDeployType = m_eDeployType;
		}
		else
		{
			m_eDeployType = m_ePreDeployType;
			m_Combo_Deploy.SetCurSel((_int)m_ePreDeployType);
		}
		break;

	case CTool_Camera::Deploy_End:
		break;

	default:
		break;
	}
}

// ���� ���õ� �ε������� ���� �ε��������� ī�޶� �ӵ� ���� ����
void CTool_Camera::OnBnClickedSpeedApply()
{
	if (-1 != m_iCamCurSel &&
		-1 != m_iControlCurSel_Eye)
	{
		m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].reserve(m_mapActionEye[m_cstrCamCurText.CTOWSTR].size());

		UpdateData(TRUE);

		m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye] = (_float)_ttof(m_cstrControlSpeed);

		UpdateData(FALSE);
	}
}

// �׼��� ���ۉ��� �� ó�� ���� 
void CTool_Camera::OnBnClickedActionStart()
{
	// MFC ��ư Ŭ�� �� ������ �Լ� ���� (���� �ϳ���)
	Click_Action();
}

// �ۼ��� �̺�Ʈ�� ���� 
void CTool_Camera::OnBnClickedCamToolSAVE()
{
	CFileDialog Dlg(FALSE, L"camdat", L"*.camdat", OFN_OVERWRITEPROMPT, 0, 0, 0, 1);
	TCHAR szFilePath[MAX_PATH] = { NULL };

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_CamTest");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;

		_int	iActionCamEyeSize = (_int)m_mapActionEye.size();
		_int	iActionCamAtSize = (_int)m_mapActionEye.size();
		_int	iSectionSpeedSize = (_int)m_mapSectionSpeed.size();

		_int	iVectorEyeSize = -1;
		_int	iVectorAtSize = -1;
		_int	iVectorSectionSpeedSize = -1;

		// map �����̳��� ������ ���� 
		CUtility::Data_WriteInt(hFile, iActionCamEyeSize, dwbyte);
		CUtility::Data_WriteInt(hFile, iActionCamAtSize, dwbyte);
		CUtility::Data_WriteInt(hFile, iSectionSpeedSize, dwbyte);

		map<const wstring, vector<CMFC_CamControl_Eye*>>::iterator	iter_Eye = m_mapActionEye.begin();
		map<const wstring, vector<CMFC_CamControl_At*>>::iterator	iter_At = m_mapActionAt.begin();
		map<const wstring, vector<_float>>::iterator				iter_SecSpeed = m_mapSectionSpeed.begin();

		for (; iter_Eye != m_mapActionEye.end(); ++iter_Eye)
		{
			iVectorEyeSize = (_int)iter_Eye->second.size();

			// map �����̳��� Key���� Value(vector)�� ����� ���� 
			CUtility::Data_WriteWString(hFile, iter_Eye->first, dwbyte);
			CUtility::Data_WriteInt(hFile, iVectorEyeSize, dwbyte);

			vector<CMFC_CamControl_Eye*>::iterator	iter_ControlEye = iter_Eye->second.begin();

			// ��ü�� ��ȸ�ϸ鼭 Save ȣ�� 
			for (; iter_ControlEye != iter_Eye->second.end(); ++iter_ControlEye)
			{
				WriteFile(hFile, &(*iter_ControlEye)->Get_Info(E_INFO::INFO_POS), sizeof(_vec3), &dwbyte, nullptr);
			}
		}

		for (; iter_At != m_mapActionAt.end(); ++iter_At)
		{
			iVectorAtSize = (_int)iter_At->second.size();

			// map �����̳��� Key���� Value(vector)�� ����� ���� 
			CUtility::Data_WriteWString(hFile, iter_At->first, dwbyte);
			CUtility::Data_WriteInt(hFile, iVectorAtSize, dwbyte);

			vector<CMFC_CamControl_At*>::iterator	iter_ControlAt = iter_At->second.begin();

			// ��ü�� ��ȸ�ϸ鼭 Save ȣ�� 
			for (; iter_ControlAt != iter_At->second.end(); ++iter_ControlAt)
			{
				WriteFile(hFile, &(*iter_ControlAt)->Get_Info(E_INFO::INFO_POS), sizeof(_vec3), &dwbyte, nullptr);
			}
		}

		// ������ ���ǵ� Save 
		for (; iter_SecSpeed != m_mapSectionSpeed.end(); ++iter_SecSpeed)
		{
			iVectorSectionSpeedSize = (_int)iter_SecSpeed->second.size();

			CUtility::Data_WriteWString(hFile, iter_SecSpeed->first, dwbyte);
			CUtility::Data_WriteInt(hFile, iVectorSectionSpeedSize, dwbyte);

			vector<_float>::iterator		iter_SpeedValue = iter_SecSpeed->second.begin();

			for (; iter_SpeedValue != iter_SecSpeed->second.end(); ++iter_SpeedValue)
			{
				CUtility::Data_WriteFloat(hFile, *iter_SpeedValue, dwbyte);
			}
		}

		CloseHandle(hFile);
	}
}

// �ε� 
void CTool_Camera::OnBnClickedCamToolLOAD()
{
	CFileDialog Dlg(TRUE,// FALSE�� �ٸ��̸����� ����. 
		L"camdat",
		L"*.camdat",
		OFN_OVERWRITEPROMPT,
		0, 0, 0, 1);

	TCHAR szFilePath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_CamTest");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		Delete_All();

		m_pScene = Management Get_Scene();

		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;

		_int	iActionCamEyeSize = -1;
		_int	iActionCamAtSize = -1;
		_int	iSectionSpeedSize = -1;

		_int	iVectorEyeSize = -1;
		_int	iVectorAtSize = -1;
		_int	iVectorSecSpeedSize = -1;

		_float	fSectionSpeed = -1.f;

		_vec3	vPos = VEC_ZERO;

		wstring wstrKey = L"";

		// 1. map�� ������ 
		CUtility::Data_ReadInt(hFile, iActionCamEyeSize, dwbyte);
		CUtility::Data_ReadInt(hFile, iActionCamAtSize, dwbyte);
		CUtility::Data_ReadInt(hFile, iSectionSpeedSize, dwbyte);

		// 2. map �����ŭ ��ȸ�ϸ鼭 Key���� Value�� ������ 
		for (_int i = 0; i < iActionCamEyeSize; ++i)
		{
			CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
			CUtility::Data_ReadInt(hFile, iVectorEyeSize, dwbyte);

			for (_int j = 0; j < iVectorEyeSize; ++j)
			{
				_int iObjTag = (_int)E_OBJTAG::OBJ_END;
				CUtility::Data_WriteInt(hFile, iObjTag, dwbyte);

				CGameObject* pGameObject = CMFC_CamControl_Eye::Create(
					m_pGraphicDev,
					(E_OBJTAG)iObjTag,
					VEC_ZERO,
					0.1f);
				assert(pGameObject);
				ReadFile(hFile, &vPos, sizeof(_vec3), &dwbyte, nullptr);
				pGameObject->Set_Pos(vPos);
				m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject((E_OBJTAG)iObjTag, pGameObject);

				m_mapActionEye[wstrKey].emplace_back(static_cast<CMFC_CamControl_Eye*>(pGameObject));
			}
		}

		for (_int i = 0; i < iActionCamAtSize; ++i)
		{
			CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
			CUtility::Data_ReadInt(hFile, iVectorAtSize, dwbyte);

			for (_int j = 0; j < iVectorAtSize; ++j)
			{
				_int iObjTag = (_int)E_OBJTAG::OBJ_END;
				CUtility::Data_WriteInt(hFile, iObjTag, dwbyte);

				CGameObject* pGameObject = CMFC_CamControl_At::Create(
					m_pGraphicDev,
					(E_OBJTAG)iObjTag,
					VEC_ZERO,
					0.1f);
				assert(pGameObject);
				ReadFile(hFile, &vPos, sizeof(_vec3), &dwbyte, nullptr);
				pGameObject->Set_Pos(vPos);
				m_pScene->Get_Layer(E_LAYERTAG::GameLogic)->Add_GameObject((E_OBJTAG)iObjTag, pGameObject);

				m_mapActionAt[wstrKey].emplace_back(static_cast<CMFC_CamControl_At*>(pGameObject));
			}
		}

		for (_int i = 0; i < iSectionSpeedSize; ++i)
		{
			CUtility::Data_ReadWString(hFile, wstrKey, dwbyte);
			CUtility::Data_ReadInt(hFile, iVectorSecSpeedSize, dwbyte);

			m_mapSectionSpeed[wstrKey].reserve(iVectorSecSpeedSize);

			for (_int j = 0; j < iVectorSecSpeedSize; ++j)
			{
				CUtility::Data_ReadFloat(hFile, fSectionSpeed, dwbyte);

				m_mapSectionSpeed[wstrKey].emplace_back(fSectionSpeed);
			}
		}

		CloseHandle(hFile);

		// MFC�� List�� ActionCam�� Key���� �߰� 
		map<const wstring, vector<CMFC_CamControl_Eye*>>::iterator	iter_Map = m_mapActionEye.begin();
		for (; iter_Map != m_mapActionEye.end(); ++iter_Map)
		{
			m_ListCam.AddString((iter_Map->first).c_str());
		}
	}
}

// Ÿ�̸� ����, �޺��ڽ��� String �߰� ��
BOOL CTool_Camera::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Ready_DefaultSettings();

	return TRUE;
}

// ���̳� ��尡 �ٲ���� üũ, ��Ʈ�� GŰ�� ��ġ 
void CTool_Camera::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_iTimerEvent)
	{
		m_fTimeDelta = Management Get_TimeDelta();

		Scene_Change();
		Mode_Change();

		if (!m_bCreateLine)
		{
			m_bCreateLine = TRUE;

			m_pScene = Management Get_Scene();
			Ready_Line();

			_float	fCamSpeed = CCamera::m_pMainCam->Get_CamSpeed();

			WCHAR	wszBuff[16];
			swprintf_s(wszBuff, L"%.2f", fCamSpeed);
			m_EditMainCamSpeed.SetWindowTextW(wszBuff);
		}

		if (0 != m_mapActionEye.size() &&
			-1 != m_iCamCurSel)
		{
			MoveOn_Line();

			if (Management Key_Down(KEY_G))
			{
				Create_ControlSphere();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTool_Camera::OnEnChangeCurPointSpeed()
{
	UpdateData(TRUE);

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_Edit_ControlSpeed, cstrValue);

	if (L"-" == cstrValue || m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].empty())
	{
		return;
	}

	if (-1 != m_iCamCurSel &&
		-1 != m_iControlCurSel_Eye)
	{
		_float	fTime = (_float)_ttof(m_cstrControlSpeed);
		fTime = CUtility::Clamp_float(fTime, 0.01f, fTime);
		m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye] = fTime;
	}

	UpdateData(FALSE);
}


void CTool_Camera::OnEnChangeAllChange()
{
	UpdateData(TRUE);

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_ALLSPEED_0107, cstrValue);

	if (L"-" == cstrValue || m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].empty())
	{
		return;
	}

	if (-1 != m_iCamCurSel)
	{
		for (_uint i = 0; i < m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR].size(); ++i)
		{
			_float	fTime = (_float)_ttof(m_cstrAllSpeed);
			fTime = CUtility::Clamp_float(fTime, 0.01f, fTime);
			m_mapSectionSpeed[m_cstrCamCurText.CTOWSTR][i] = fTime;
		}
	}

	UpdateData(FALSE);
}

void CTool_Camera::OnEditChangePosX()
{
	UpdateData(TRUE);

	_vec3	vPos = VEC_ZERO;

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_Edit_PosX, cstrValue);

	if (L"-" == cstrValue)
	{
		return;
	}
	switch (m_eControlPointType)
	{
	case CTool_Camera::Control_EyePoint:
		if (-1 == m_iControlCurSel_Eye)
		{
			return;
		}

		vPos = m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Get_Position();
		vPos.x = m_fPosX;
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Pos(vPos);
		break;

	case CTool_Camera::Control_AtPoint:
		if (-1 == m_iControlCurSel_At)
		{
			return;
		}

		vPos = m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Get_Position();
		vPos.x = m_fPosX;
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Set_Pos(vPos);
		break;
	case CTool_Camera::Control_End:
		return;

	default:
		assert(0);
		break;
	}

	UpdateData(FALSE);
}

void CTool_Camera::OnEditChangePosY()
{
	UpdateData(TRUE);

	_vec3	vPos = VEC_ZERO;

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_Edit_PosX, cstrValue);

	if (L"-" == cstrValue || -1 == m_iCamCurSel)
	{
		return;
	}
	switch (m_eControlPointType)
	{
	case CTool_Camera::Control_EyePoint:
		if (-1 == m_iControlCurSel_Eye)
		{
			return;
		}

		vPos = m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Get_Position();
		vPos.y = m_fPosY;
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Pos(vPos);
		break;
	case CTool_Camera::Control_AtPoint:
		if (-1 == m_iControlCurSel_At)
		{
			return;
		}

		vPos = m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Get_Position();
		vPos.y = m_fPosY;
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Set_Pos(vPos);
		break;
	case CTool_Camera::Control_End:
		return;

	default:
		assert(0);
		break;
	}

	UpdateData(FALSE);
}

void CTool_Camera::OnEditChangePosZ()
{
	UpdateData(TRUE);

	_vec3	vPos = VEC_ZERO;

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_Edit_PosX, cstrValue);

	if (L"-" == cstrValue || -1 == m_iCamCurSel)
	{
		return;
	}
	switch (m_eControlPointType)
	{
	case CTool_Camera::Control_EyePoint:
		if (-1 == m_iControlCurSel_Eye)
		{
			return;
		}

		vPos = m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Get_Position();
		vPos.z = m_fPosZ;
		m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Pos(vPos);
		break;
	case CTool_Camera::Control_AtPoint:
		if (-1 == m_iControlCurSel_At)
		{
			return;
		}

		vPos = m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Get_Position();
		vPos.z = m_fPosZ;
		m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Set_Pos(vPos);
		break;
	case CTool_Camera::Control_End:
		return;

	default:
		assert(0);
		break;
	}

	UpdateData(FALSE);
}


void CTool_Camera::OnBnClickedButtonPickObjPosApply()
{
	if (-1 == m_iCamCurSel)
	{
		return;
	}

	if (ControlPointType::Control_EyePoint == m_eControlPointType)
	{
		if (-1 != m_iControlCurSel_Eye &&
			!m_mapActionEye[m_cstrCamCurText.CTOWSTR].empty())
		{
			m_mapActionEye[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_Eye]->Set_Pos(Get_PopupTool_PickObject()->Get_Position());
		}
	}
	else if (ControlPointType::Control_AtPoint == m_eControlPointType)
	{
		if (-1 != m_iControlCurSel_At &&
			!m_mapActionAt[m_cstrCamCurText.CTOWSTR].empty())
		{
			_vec3	vTargetPos = Get_PopupTool_PickObject()->Get_Position();
			vTargetPos.y += 0.75f;

			m_mapActionAt[m_cstrCamCurText.CTOWSTR][m_iControlCurSel_At]->Set_Pos(vTargetPos);
		}
	}
}


void CTool_Camera::OnBnClickedJyCamCheckfreecam0122()
{
	if (m_ButtonCheckFreeCam.GetCheck())
	{
		CCamera::m_pMainCam->Set_FixMode(TRUE);
	}
	else
	{
		CCamera::m_pMainCam->Set_FixMode(FALSE);
	}
}


void CTool_Camera::OnEnChangeJyCamMaincamspeed0122()
{
	UpdateData(TRUE);

	CString		cstrValue = L"";
	GetDlgItemText(IDC_JY_CAM_MainCamSpeed_0122, cstrValue);

	if (L"-" == cstrValue)
	{
		return;
	}

	CCamera::m_pMainCam->Set_CamSpeed(m_fMainCamSpeed);

	UpdateData(FALSE);
}
