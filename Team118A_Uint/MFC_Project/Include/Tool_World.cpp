#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_World.h"
#include "StageMap.h"
#include "afxdialogex.h"
#include "StartMark.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Obstacle.h"
// CTool_World ��ȭ �����Դϴ�.

USING(Engine)

IMPLEMENT_DYNAMIC(CTool_World, CDialogEx)

CTool_World::CTool_World(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_WORLD, pParent)
	, m_strLevel(_T(""))
	, m_strDisplayName(_T(""))
	, m_strColRadius(_T(""))
	, m_strScaleX(_T(""))
	, m_strScaleY(_T(""))
	, m_strScaleZ(_T(""))
	, m_strPickObjectDisplayName(_T(""))
{

}

CTool_World::CTool_World(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_World::~CTool_World()
{
}

//������Ʈ �ֱ⿡ ���缭 ����Ǵ� �Լ�, ���콺 ��ŷ�� ���� ���� ��ġ�� ����Ѵ�.
void CTool_World::ToolWorldUpdate(_float fDeltaTime)
{

	MonsterCheck();
	if (m_pPickObject)
	{
		if (m_pPickObject->Get_Dead())
		{
			Safe_Release(m_pPickObject);
			m_pPickObject = nullptr;
		}
	}


	POINT tPoint = CUtility::Get_MousePoint(g_hWnd);
	if (!(tPoint.x < 0 || tPoint.x >= WINCX || tPoint.y < 0 || tPoint.y >= WINCY))
	{
		if (Management Key_Down(KEY_LBUTTON))
		{
			if (m_pPickObject)
			{
				Safe_Release(m_pPickObject);
				m_pPickObject = nullptr;
			}
			MonsterRayCast();

			StartMarkRayCast();

		}
		if (Management Key_Pressing(KEY_LBUTTON))
		{
			if (m_pPickObject)
			{
				_vec3 vPos;
				if (MapRayCast(&vPos))
				{
					m_pPickObject->Set_Pos(vPos);


				}
			}
			else
			{
				if (m_pStartPickObject)
				{
					_vec3 vPos;
					if (MapRayCast(&vPos))
					{
						vPos.y += 0.01f;
						m_pStartPickObject->Set_Pos(vPos);
					}
				}
			}
		}
	}

	if (Management Key_Down(KEY_LEFT))
	{
		if (m_pPickObject)
		{
			m_pPickObject->Add_Rotation(E_ROTATION::ROT_Y, 15.f);
		}
	}
	if (Management Key_Down(KEY_RIGHT))
	{
		if (m_pPickObject)
		{

			m_pPickObject->Add_Rotation(E_ROTATION::ROT_Y, -15.f);

		}
	}

	FastKey();
}

//�޺��ڽ� ����
BOOL CTool_World::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadMonster();


	m_cComboAttackType.AddString(L"����");
	m_cComboAttackType.AddString(L"����");
	m_cComboAttackType.AddString(L"�ź�");
	m_cComboAttackType.AddString(L"�Ϲ�");

	m_cComboDefenceType.AddString(L"���尩");
	m_cComboDefenceType.AddString(L"���尩");
	m_cComboDefenceType.AddString(L"Ư���尩");

	m_cComboMonsterType.AddString(L"�Ϲ�");
	m_cComboMonsterType.AddString(L"����Ʈ");
	m_cComboMonsterType.AddString(L"����");

	m_cComboWeaponType.AddString(L"SG");
	m_cComboWeaponType.AddString(L"SMG");
	m_cComboWeaponType.AddString(L"HG");
	m_cComboWeaponType.AddString(L"GL");
	m_cComboWeaponType.AddString(L"SR");
	m_cComboWeaponType.AddString(L"MG");
	m_cComboWeaponType.AddString(L"AR");
	m_cComboWeaponType.AddString(L"MT");


	m_cComboWorldMod.AddString(L"�ӹ�");
	m_cComboWorldMod.AddString(L"���� ����");
	m_cComboWorldMod.AddString(L"Ư�� �Ƿ�");
	m_cComboWorldMod.AddString(L"�ѷ���");
	m_cComboWorldMod.AddString(L"����");

	m_cComboWorldType.AddString(L"�ð���");
	m_cComboWorldType.AddString(L"����");
	m_cComboWorldType.AddString(L"�ǳ���");
	m_cComboWorldType.AddString(L"����");
	return 0;
}


//���� �������� ���������� �̸��� �ҷ��� MFC����Ʈ�� �����Ѵ�.
void CTool_World::LoadMonster()
{

	m_cListMonster.ResetContent();
	string searching = "../../Reference/Resource/Monster/";
	searching += "*.*";
	_finddata_t findData;
	intptr_t handle = _findfirst(searching.c_str(), &findData);  //���� ���� �� ��� ������ ã�´�.


	if (handle == -1)
	{
		return;
	}

	int result = 0;

	do
	{
		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{

			wstring wstr = CUtility::CHAR_TO_WSTRING(findData.name);
			m_cListMonster.AddString(wstr.c_str());
		}
		result = _findnext(handle, &findData);

	} while (result != -1);

	_findclose(handle);

}

//ĳ���� �������� ĳ���������� �̸��� �ҷ��� MFC����Ʈ�� �����Ѵ�.
void CTool_World::LoadCharacter()
{
	m_cListMonster.ResetContent();
	string searching = "../../Reference/Resource/Character/";
	searching += "*.*";
	_finddata_t findData;
	intptr_t handle = _findfirst(searching.c_str(), &findData);  //���� ���� �� ��� ������ ã�´�.

	if (handle == -1)
	{
		return;
	}

	int result = 0;

	do
	{
		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
			wstring wstr = CUtility::CHAR_TO_WSTRING(findData.name);
			m_cListMonster.AddString(wstr.c_str());
		}
		result = _findnext(handle, &findData);

	} while (result != -1);

	_findclose(handle);
}

//���� Ǯ MFC����Ʈ�� �ٽ� �ҷ����� �Լ�
void CTool_World::MonsterPoolReLoad()
{
	m_cListMonsterPool.ResetContent();

	for (auto& tMonsterInfo : m_vecMonsterPool)
	{
		m_cListMonsterPool.AddString(tMonsterInfo.wszName);
	}
}

//�׷� ����Ʈ�� �ٽ� �ҷ����� �Լ�
void CTool_World::GroupReLoad()
{
	m_cListGroup.ResetContent();

	WCHAR wszGroupName[16];

	for (_int i = 0; i < _int(m_vecGroup.size()); ++i)
	{
		swprintf_s(wszGroupName, L"Group %d", i);
		m_cListGroup.AddString(wszGroupName);
	}

}

//�׷������Ʈ ����Ʈ�� �ٽ� �ҷ����� �Լ�
void CTool_World::GroupObjectReLoad()
{
	m_cListGroupObject.ResetContent();
	_int iSelect = m_cListGroup.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecGroup.size())
	{
		return;
	}
	for (auto& pMonster : m_vecGroup[iSelect])
	{
		m_cListGroupObject.AddString(pMonster->Get_MonsterInfo().wszDisplayName);
	}


}

//�׷� ������Ʈ ������ ���� ����Ǯ�� ������Ʈ ������ �����ϴ� �Լ�
void CTool_World::MonsterPoolObjectListReLoad()
{
	for (auto& vecMonster : m_vecGroup)
	{
		for (auto& pMonster : vecMonster)
		{
			T_MONSTER_INFO tMonsterInfo = pMonster->Get_MonsterInfo();
			_int iPoolIndex = 0;
			for (auto& tMonInfo : m_vecMonsterPool)
			{
				if (tMonsterInfo == tMonInfo)
				{
					m_vecMonster[iPoolIndex].emplace_back(pMonster);
					pMonster->AddRef();
					break;
				}
				++iPoolIndex;
			}
		}
	}

}

//����Ű�� �̿��ϴ� �Լ� 
void CTool_World::FastKey()
{
	if (!m_pPickObject)
	{
		return;
	}

	if (Management Key_Down(KEY_F9))
	{
		OnBnClickedMw1222BtnAddObject();
	}
}

//�ҷ����� �� ���� �����͸� ��� ����� �Լ�
void CTool_World::DeleteAll()
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Map);
	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObj : tPair.second)
		{
			pObj->Set_Dead(TRUE);
		}
	}
	pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);
	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObj : tPair.second)
		{
			pObj->Set_Dead(TRUE);
		}
	}

	m_vecMonsterPool.clear();

	for (auto& vecMonster : m_vecMonster)
	{
		for (auto& pMonster : vecMonster)
		{
			pMonster->Set_Dead(TRUE);
			Safe_Release(pMonster);
		}
		vecMonster.clear();
	}
	m_vecMonster.clear();

	for (auto& vecMonster : m_vecGroup)
	{
		for (auto& pMonster : vecMonster)
		{
			Safe_Release(pMonster);
		}
		vecMonster.clear();
	}
	m_vecGroup.clear();

	Safe_Release(m_pPickObject);
}

void CTool_World::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MW_1222_LIST_MONSTER, m_cListMonster);
	DDX_Control(pDX, IDC_MW_1222_LIST_MONSTER_POOL, m_cListMonsterPool);

	DDX_Control(pDX, IDC_MW_1222_COMBO_TYPE_MONSTER, m_cComboMonsterType);
	DDX_Control(pDX, IDC_MW_1222_COMBO_TYPE_ATTACK, m_cComboAttackType);
	DDX_Control(pDX, IDC_MW_1222_COMBO_TYPE_DEFENCE, m_cComboDefenceType);
	DDX_Control(pDX, IDC_MW_1222_COMBO_TYPE_WEAPON, m_cComboWeaponType);
	DDX_Text(pDX, IDC_MW_1222_EDIT_LEVEL, m_strLevel);
	DDX_Text(pDX, IDC_MW_1222_EDIT_DISPLAY_NAME, m_strDisplayName);
	DDX_Text(pDX, IDC_MW_1223_EDIT_COL_RADIUS, m_strColRadius);
	DDX_Text(pDX, IDC_MW_1223_EDIT_SCALE_X, m_strScaleX);
	DDX_Text(pDX, IDC_MW_1223_EDIT_SCALE_Y, m_strScaleY);
	DDX_Text(pDX, IDC_MW_1223_EDIT_SCALE_Z, m_strScaleZ);
	DDX_Control(pDX, IDC_MW_1222_LIST_GROUP, m_cListGroup);
	DDX_Text(pDX, IDC_MW_1226_EDIT_PICK_MONSTER_NAME, m_strPickObjectDisplayName);
	DDX_Control(pDX, IDC_MW_1222_LIST_GROUP_OBJECT, m_cListGroupObject);
	DDX_Control(pDX, IDC_MW_1227_COMBO_WORLD_MOD, m_cComboWorldMod);
	DDX_Control(pDX, IDC_MW_1227_COMBO_WORLD_TYPE, m_cComboWorldType);
}


BEGIN_MESSAGE_MAP(CTool_World, CDialogEx)

	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()

	ON_BN_CLICKED(IDC_MW_1222_BTN_ADD_MONSTER_POOL, &CTool_World::OnBnClickedMw1222BtnAddMonsterPool)
	ON_BN_CLICKED(IDC_MW_1222_BTN_DELETE_MONSTER_POOL, &CTool_World::OnBnClickedMw1222BtnDeleteMonsterPool)
	ON_BN_CLICKED(IDC_MW_1222_RADIO_LIST_MONSTER, &CTool_World::OnBnClickedMw1222RadioListMonster)
	ON_BN_CLICKED(IDC_MW_1222_RADIO_LIST_CHARACTER, &CTool_World::OnBnClickedMw1222RadioListCharacter)
	ON_LBN_SELCHANGE(IDC_MW_1222_LIST_MONSTER_POOL, &CTool_World::OnLbnSelchangeMw1222ListMonsterPool)
	ON_EN_CHANGE(IDC_MW_1222_EDIT_DISPLAY_NAME, &CTool_World::OnEnChangeMw1222EditDisplayName)
	ON_EN_CHANGE(IDC_MW_1222_EDIT_LEVEL, &CTool_World::OnEnChangeMw1222EditLevel)
	ON_CBN_SELCHANGE(IDC_MW_1222_COMBO_TYPE_MONSTER, &CTool_World::OnCbnSelchangeMw1222ComboTypeMonster)
	ON_CBN_SELCHANGE(IDC_MW_1222_COMBO_TYPE_ATTACK, &CTool_World::OnCbnSelchangeMw1222ComboTypeAttack)
	ON_CBN_SELCHANGE(IDC_MW_1222_COMBO_TYPE_DEFENCE, &CTool_World::OnCbnSelchangeMw1222ComboTypeDefence)
	ON_CBN_SELCHANGE(IDC_MW_1222_COMBO_TYPE_WEAPON, &CTool_World::OnCbnSelchangeMw1222ComboTypeWeapon)
	ON_BN_CLICKED(IDC_MW_1222_BTN_SPAWN_MONSTER, &CTool_World::OnBnClickedMw1222BtnSpawnMonster)
	ON_EN_CHANGE(IDC_MW_1223_EDIT_COL_RADIUS, &CTool_World::OnEnChangeMw1223EditColRadius)
	ON_EN_CHANGE(IDC_MW_1223_EDIT_SCALE_X, &CTool_World::OnEnChangeMw1223EditScaleX)
	ON_EN_CHANGE(IDC_MW_1223_EDIT_SCALE_Y, &CTool_World::OnEnChangeMw1223EditScaleY)
	ON_EN_CHANGE(IDC_MW_1223_EDIT_SCALE_Z, &CTool_World::OnEnChangeMw1223EditScaleZ)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MW_1223_BTN_SAVE_FIELD_INFO, &CTool_World::OnBnClickedMw1223BtnSaveFieldInfo)
	ON_BN_CLICKED(IDC_MW_1223_BTN_LOAD_FIELD_INFO, &CTool_World::OnBnClickedMw1223BtnLoadFieldInfo)
	ON_BN_CLICKED(IDC_MW_1222_BTN_ADD_GROUP, &CTool_World::OnBnClickedMw1222BtnAddGroup)
	ON_BN_CLICKED(IDC_MW_1222_BTN_DELETE_GROUP, &CTool_World::OnBnClickedMw1222BtnDeleteGroup)
	ON_BN_CLICKED(IDC_MW_1226_BTN_PICK_OBJECT_DELETE, &CTool_World::OnBnClickedMw1226BtnPickObjectDelete)
	ON_BN_CLICKED(IDC_MW_1222_BTN_ADD_OBJECT, &CTool_World::OnBnClickedMw1222BtnAddObject)
	ON_BN_CLICKED(IDC_MW_1222_BTN_DELETE_OBJECT, &CTool_World::OnBnClickedMw1222BtnDeleteObject)
	ON_LBN_SELCHANGE(IDC_MW_1222_LIST_GROUP, &CTool_World::OnLbnSelchangeMw1222ListGroup)
	ON_BN_CLICKED(IDC_MW_1227_BTN_SET_START_POS, &CTool_World::OnBnClickedMw1227BtnSetStartPos)
	ON_CBN_SELCHANGE(IDC_MW_1227_COMBO_WORLD_MOD, &CTool_World::OnCbnSelchangeMw1227ComboWorldMod)
	ON_CBN_SELCHANGE(IDC_MW_1227_COMBO_WORLD_TYPE, &CTool_World::OnCbnSelchangeMw1227ComboWorldType)
END_MESSAGE_MAP()


// CTool_World �޽��� ó�����Դϴ�.




//MFC����Ʈ���ִ� �̸��� ���� �������� ����ü�� ������ �������� ��ȯ�Ѵ�.
T_MONSTER_INFO CTool_World::NameTranslate(wstring wstrName)
{
	T_MONSTER_INFO tMonsterInfo;
	ZeroMemory(&tMonsterInfo, sizeof(T_MONSTER_INFO));
	wstring wstrDisplayName;
	lstrcpy(tMonsterInfo.wszName, wstrName.c_str());
	wstrDisplayName = wstrName;

	tMonsterInfo.fRadius = 0.1f;
	tMonsterInfo.vScale = { 2.5f, 2.5f, 2.5f };
	tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NORMAL;


	if (wstrName == L"Sukeban_MG")
	{
		wstrDisplayName = L"���ɹ� (MG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Sukeban_SMG")
	{
		wstrDisplayName = L"���ɹ� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"Sukeban_SR")
	{
		wstrDisplayName = L"���ɹ� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"HeavyTank01_Helmet_Cannon")
	{
		wstrDisplayName = L"������ ��ũ";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MT;
	}
	else if (wstrName == L"BoxCat")
	{
		wstrDisplayName = L"���� �׷�";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_NULL;
	}

	else if (wstrName == L"Cook_GenbuStore_AR")
	{
		wstrDisplayName = L"�丮�� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"Cook_GenbuStore_MG")
	{
		wstrDisplayName = L"�丮�� (MG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Cook_GenbuStore_SG")
	{
		wstrDisplayName = L"�丮�� (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SG;
	}
	else if (wstrName == L"Droid_Yoheki_Shield")
	{
		wstrDisplayName = L"�뿪 ���丶Ÿ\n(Shield)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Droid_Helmet_AR")
	{
		wstrDisplayName = L"���ܿ� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"Droid_Helmet_RL")
	{
		wstrDisplayName = L"���ܿ� (RL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_RL;
	}
	else if (wstrName == L"Droid_Helmet_SMG")
	{
		wstrDisplayName = L"���ܿ� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"Droid_HelmetBoss_SMG")
	{
		wstrDisplayName = L"���ܿ� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}

	else if (wstrName == L"Terror_Droid_Mimesis_AR")
	{
		wstrDisplayName = L"���ܿ� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Terror_Droid_Mimesis_RL")
	{
		wstrDisplayName = L"������ ���丶Ÿ (RL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Terror_Droid_Mimesis_Shield")
	{
		wstrDisplayName = L"������ ���丶Ÿ\n(Shield)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"Terror_GasMask_Arius_GL")
	{
		wstrDisplayName = L"�׷�ȭ �Ƹ��콺 �л� (GL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Terror_GasMask_Arius_HMG")
	{
		wstrDisplayName = L"�׷�ȭ �Ƹ��콺 �л� (HMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Terror_GasMask_Arius_SMG")
	{
		wstrDisplayName = L"�׷�ȭ �Ƹ��콺 �л� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"GasMask_Arius_HMG")
	{
		wstrDisplayName = L"�Ƹ��콺 �л� (HMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"GasMask_Arius_SMG")
	{
		wstrDisplayName = L"�Ƹ��콺 �л� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}


	else if (wstrName == L"Yukata_StreetGhostes_MG")
	{
		wstrDisplayName = L"��īŸ (MG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Yukata_StreetGhostes_RL")
	{
		wstrDisplayName = L"��īŸ (RL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Yukata_StreetGhostes_SG")
	{
		wstrDisplayName = L"��īŸ (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Yukata_StreetGhostes_SG_Turret")
	{
		wstrDisplayName = L"��īŸ (Turret)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"YukataHanya_StreetGhostes_SR")
	{
		wstrDisplayName = L"��īŸ �Ѿ� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"YukataHanya_StreetGhostes_SR_Cannon")
	{
		wstrDisplayName = L"��īŸ �Ѿ� (Cannon)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"YukataHanya_StreetGhostes_SR_Turret")
	{
		wstrDisplayName = L"��īŸ �Ѿ� (Turret)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"Yustina_AR")
	{
		wstrDisplayName = L"����Ƽ�� ���� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Yustina_SMG")
	{
		wstrDisplayName = L"����Ƽ�� ���� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Yustina_SR")
	{
		wstrDisplayName = L"����Ƽ�� ���� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"TechSoldier_Decagram_AR")
	{
		wstrDisplayName = L"����� ���丶Ÿ (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Sweeper_Decagram_Taser")
	{
		wstrDisplayName = L"������";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Drone_Decagram_Missile")
	{
		wstrDisplayName = L"����� ��� (Missile)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Drone_Decagram_Vulcan")
	{
		wstrDisplayName = L"����� ��� (Vulcan)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"EnemyTSS_Aparche_Blackmarket")
	{
		wstrDisplayName = L"������ ����ġ";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"EnemyTSS_Crusader_PMC")
	{
		wstrDisplayName = L"��ȭ�� ���� ��ũ";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"FoodTruck_GenbuStore_MG")
	{
		wstrDisplayName = L"���� �ƴմϴ�.";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"FX_Utaha_Original_Turret")
	{
		wstrDisplayName = L"���� �ƴմϴ�.";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"HeavyTank_Decagram_Cannon")
	{
		wstrDisplayName = L"HeavyTank_Decagram_Cannon";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"Kaitenranger_Black_HG")
	{
		wstrDisplayName = L"ī������ ��";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Kaitenranger_Green_HMG")
	{
		wstrDisplayName = L"ī������ �׸�";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Kaitenranger_Pink_RL")
	{
		wstrDisplayName = L"ī������ ��ũ";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Kaitenranger_Red_AR")
	{
		wstrDisplayName = L"ī������ ����";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Kaitenranger_Yellow_SR")
	{
		wstrDisplayName = L"ī������ ����";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	else if (wstrName == L"KumabotPapa_Slumpia_SG")
	{
		wstrDisplayName = L"�����Ǿ� ���� (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SG;
	}

	else if (wstrName == L"LightTank_Helmet_Vulcan")
	{
		wstrDisplayName = L"�淮 ��ũ (Vulcan)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MT;
	}

	else if (wstrName == L"RoyalGuard_RedWinter_AR")
	{
		wstrDisplayName = L"�ξⰡ�� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"RoyalGuard_RedWinter_GL")
	{
		wstrDisplayName = L"�ξⰡ�� (GL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_GL;
	}
	else if (wstrName == L"RoyalGuard_RedWinter_SMG")
	{
		wstrDisplayName = L"�ξⰡ�� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"RoyalGuard_RedWinterSergeant_AR")
	{
		wstrDisplayName = L"�ξⰡ�� �߻� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}

	else if (wstrName == L"SchoolGirl01_Sukeban_SMG")
	{
		wstrDisplayName = L"���ɹ� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"SchoolGirl02_Sukeban_SR")
	{
		wstrDisplayName = L"���ɹ� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"SchoolGirl03_Sukeban_HMG")
	{
		wstrDisplayName = L"���ɹ� (HMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}

	else if (wstrName == L"SchoolGirl01_SukebanSwim_SMG")
	{
		wstrDisplayName = L"������ ���ɹ� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"SchoolGirl02_SukebanSwim_SR")
	{
		wstrDisplayName = L"������ ���ɹ� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"SchoolGirl03_SukebanSwim_MG")
	{
		wstrDisplayName = L"������ ���ɹ� (MG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"SchoolGirlBoss02_SukebanSwim_MG")
	{
		wstrDisplayName = L"������ ���ɹ� (Boss)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}

	else if (wstrName == L"Soldier_Blackmarket_RL")
	{
		wstrDisplayName = L"������ ���丶Ÿ (RL)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_RL;
	}

	else if (wstrName == L"Soldier_Fuuki_MG")
	{
		wstrDisplayName = L"��� �����ο� (MG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Soldier_Helmet_AR")
	{
		wstrDisplayName = L"���ܿ� (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"Soldier_Helmet_SG")
	{
		wstrDisplayName = L"���ܿ� (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SG;
	}
	else if (wstrName == L"Soldier_Helmet_SR")
	{
		wstrDisplayName = L"���ܿ� (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"Soldier_Kaiserpmc_HG")
	{
		wstrDisplayName = L"PMC ���� (HG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}
	else if (wstrName == L"Soldier_RedWinterYoheki_SG")
	{
		wstrDisplayName = L"�����ܿ� �뿪 (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SG;
	}
	else if (wstrName == L"Soldier_RedWinterYoheki_SR")
	{
		wstrDisplayName = L"�����ܿ� �뿪 (SR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"Soldier_RWY_AR")
	{
		wstrDisplayName = L"�����ܿ� �뿪 (AR)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"Soldier_YohekiPink_MG")
	{
		wstrDisplayName = L"�뿪 ���� (SG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Soldier_YohekiPink_SMG")
	{
		wstrDisplayName = L"�뿪 ���� (SMG)";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"Goliath_Decagram_Vulcan")
	{
		wstrDisplayName = L"�񸮾�";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_NULL;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_NULL;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}

	//ĳ����
	else if (wstrName == L"Neru")
	{
		wstrDisplayName = L"�׷�";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
	}
	else if (wstrName == L"Maki")
	{
		wstrDisplayName = L"��Ű";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Mutsuki")
	{
		wstrDisplayName = L"����Ű";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
	}
	else if (wstrName == L"Serika")
	{
		wstrDisplayName = L"����ī";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
	}
	else if (wstrName == L"Shun")
	{
		wstrDisplayName = L"��";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
	}
	else if (wstrName == L"Akane")
	{
		wstrDisplayName = L"��ī��";
		lstrcpy(tMonsterInfo.wszDisplayName, wstrDisplayName.c_str());
		tMonsterInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		tMonsterInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		tMonsterInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
	}

	return tMonsterInfo;
}

//���� ���̾ �ִ� ������Ʈ�� �� �浹ü�� ��ŷ�ϰ� m_pPickObject�� �־��ִ� �Լ�
void CTool_World::MonsterRayCast()
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Monster);

	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObject : tPair.second)
		{
			CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pObject->Get_Component(ComTag_Collider));
			CGameObject* pPickObj = nullptr;
			pSphereCol->PickingCollider(g_hWnd, &pPickObj);

			if (pPickObj)
			{
				m_pPickObject = static_cast<CMonster*>(pPickObj);
				m_pPickObject->AddRef();

				UpdateData(true);
				m_strPickObjectDisplayName = m_pPickObject->Get_MonsterInfo().wszDisplayName;
				UpdateData(false);
				return;
			}
		}
	}

	UpdateData(true);
	m_strPickObjectDisplayName = L"Un Selected";
	UpdateData(false);
}

void CTool_World::StartMarkRayCast()
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Environment);

	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObject : tPair.second)
		{
			if (pObject->Get_ObjTag() != E_OBJTAG::OBJ_Obstacle)
			{
				continue;
			}
			CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pObject->Get_Component(ComTag_Collider));

			CGameObject* pPickObj = nullptr;
			pSphereCol->PickingCollider(g_hWnd, &pPickObj);

			if (pPickObj)
			{
				m_pStartPickObject = static_cast<CStartMark*>(pPickObj);

				return;
			}
		}
	}

	m_pStartPickObject = nullptr;
}

//�� ���̾ �ִ� ������Ʈ�� ��ŷ�� �ش� ��ġ�� pOutPos�� �������ִ� �Լ� 
_bool CTool_World::MapRayCast(_vec3 * pOutPos)
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Map);

	_float fDistance = 999999.f;
	_vec3 vResultPos;
	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObject : tPair.second)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(ComTag_Transform));
			CSMesh* pSMesh = static_cast<CSMesh*>(pObject->Get_Component(ComTag_SMesh));

			_vec3 vCheckPos;
			_float fDist;
			if (pSMesh->PickingMesh(g_hWnd, &vCheckPos, &fDist, pTransform->Get_WorldMatrix()))
			{
				if (fDist < fDistance)
				{
					fDistance = fDist;
					vResultPos = vCheckPos;
				}
			}
		}
	}

	if (fDistance < 999998.f)
	{
		*pOutPos = vResultPos;
		return TRUE;
	}


	return FALSE;

}

//MFC ���� Ǯ ����Ʈ���� ���������� �ٲ������ �ش� ���� ������ ����ϴ� ������ ������ �ϰ� �����ϴ� �Լ�
void CTool_World::MonsterInfoUpdate(_int iSelectIndex)
{
	for (auto& pMonster : m_vecMonster[iSelectIndex])
	{
		pMonster->Set_MonsterInfo(m_vecMonsterPool[iSelectIndex]);
	}
}

//���Ͱ� �����Ǿ����� ���θ� �Ǵ��ؼ� �������� �����̳ʿ��� �����ִ� �Լ�
void CTool_World::MonsterCheck()
{
	if (m_vecMonster.empty())
	{
		return;
	}

	for (auto& vecMonster : m_vecMonster)
	{
		auto iter = vecMonster.begin();
		for (; iter != vecMonster.end();)
		{
			if ((*iter)->Get_Dead())
			{
				Safe_Release(*iter);
				iter = vecMonster.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	for (auto& vecMonster : m_vecGroup)
	{
		auto iter = vecMonster.begin();
		for (; iter != vecMonster.end();)
		{
			if ((*iter)->Get_Dead())
			{
				Safe_Release(*iter);
				iter = vecMonster.erase(iter);
				GroupObjectReLoad();
			}
			else
			{
				++iter;
			}
		}
	}
}




void CTool_World::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);



}


void CTool_World::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}



//���� Ǯ ����Ʈ�� �߰��ϴ� �Լ�
void CTool_World::OnBnClickedMw1222BtnAddMonsterPool()
{
	_int iSelectIndex = m_cListMonster.GetCurSel();

	WCHAR wszMonsterName[64];
	m_cListMonster.GetText(iSelectIndex, wszMonsterName);


	m_cListMonsterPool.AddString(wszMonsterName);
	m_vecMonsterPool.emplace_back(NameTranslate(wszMonsterName));
	m_vecMonster.emplace_back(vector<CMonster*>());
}

//���� Ǯ ����Ʈ�� �ִ� ������ �����ϴ� �Լ�, �� ���� ������ ����ϴ� ���͵鵵 ��� �����ȴ�.
void CTool_World::OnBnClickedMw1222BtnDeleteMonsterPool()
{
	_int iSelectIndex = m_cListMonsterPool.GetCurSel();

	if (m_vecMonsterPool.empty())
		return;

	m_cListMonsterPool.DeleteString(iSelectIndex);
	auto iter = m_vecMonsterPool.begin();
	auto iterMonster = m_vecMonster.begin();
	for (_int i = 0; i < iSelectIndex; ++i)
	{
		++iter;
		++iterMonster;
	}
	m_vecMonsterPool.erase(iter);

	for (auto& pMonster : (*iterMonster))
	{
		pMonster->Set_Dead(TRUE);
		Safe_Release(pMonster);
	}
	m_vecMonster.erase(iterMonster);
}

//Radio ��ư ����� ����Ʈ�� �ٲ��ִ� �Լ�
void CTool_World::OnBnClickedMw1222RadioListMonster()
{
	LoadMonster();
}
void CTool_World::OnBnClickedMw1222RadioListCharacter()
{
	LoadCharacter();
}

//���� Ǯ �� ����� ������ ����ϴ� �Լ�
void CTool_World::OnLbnSelchangeMw1222ListMonsterPool()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}

	m_strDisplayName = m_vecMonsterPool[iSelect].wszDisplayName;
	m_strLevel = to_wstring(m_vecMonsterPool[iSelect].iLevel).c_str();

	m_cComboAttackType.SetCurSel((_int)m_vecMonsterPool[iSelect].eAtkType - 1);
	m_cComboDefenceType.SetCurSel((_int)m_vecMonsterPool[iSelect].eDiffType - 1);
	m_cComboWeaponType.SetCurSel((_int)m_vecMonsterPool[iSelect].eWeaponType - 1);
	m_cComboMonsterType.SetCurSel((_int)m_vecMonsterPool[iSelect].eMonsterType);


	m_strColRadius = CUtility::RoundWString(to_wstring(m_vecMonsterPool[iSelect].fRadius), 2).c_str();
	m_strScaleX = CUtility::RoundWString(to_wstring(m_vecMonsterPool[iSelect].vScale.x), 2).c_str();
	m_strScaleY = CUtility::RoundWString(to_wstring(m_vecMonsterPool[iSelect].vScale.y), 2).c_str();
	m_strScaleZ = CUtility::RoundWString(to_wstring(m_vecMonsterPool[iSelect].vScale.z), 2).c_str();

	UpdateData(false);
}

//���� �� �޸� ����
void CTool_World::OnDestroy()
{
	CDialogEx::OnDestroy();

	for (auto& vecMonster : m_vecMonster)
	{
		for (auto& pMonster : vecMonster)
		{
			Safe_Release(pMonster);
		}
		vecMonster.clear();
	}
	m_vecMonster.clear();

	for (auto& vecMonster : m_vecGroup)
	{
		for (auto& pMonster : vecMonster)
		{
			Safe_Release(pMonster);
		}
		vecMonster.clear();
	}
	m_vecGroup.clear();

	Safe_Release(m_pPickObject);
}

//������Ʈ�� ������ ���� ���� ����ü�� ���� ��ȯ���ִ� �Լ�
void CTool_World::OnBnClickedMw1222BtnSpawnMonster()
{
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}

	wstring wstrMonsterName = m_vecMonsterPool[iSelect].wszName;

	CMonster* pMonster = CMonster::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, wstrMonsterName);
	pMonster->Set_MonsterInfo(m_vecMonsterPool[iSelect]);

	if (pMonster->Get_Name() == L"Binah_Tail")
	{
		pMonster->Set_BodyBoneName("Pivot2");
	}
	if (pMonster->Get_Name() == L"Binah")
	{
		pMonster->Set_BodyBoneName("Bone_Body_00");
	}
	if (pMonster->Get_Name() == L"Hod")
	{
		pMonster->Set_BodyBoneName("bone_body_02");
	}
	if (pMonster->Get_Name() == L"HeavyTank_Decagram_Cannon")
	{
		pMonster->Set_BodyBoneName("Bone_Lower");
	}
	if (pMonster->Get_Name() == L"HeavyTank01_Helmet_Cannon" ||
		pMonster->Get_Name() == L"LightTank_Helmet_Vulcan" ||
		pMonster->Get_Name() == L"EnemyTSS_Crusader_PMC")
	{
		pMonster->Set_BodyBoneName("Bone_Body");
	}
	if (pMonster->Get_Name() == L"EnemyTSS_Aparche_Blackmarket")
	{
		pMonster->Set_BodyBoneName("bone_eye");
	}
	if (pMonster->Get_Name() == L"Drone_Decagram_Vulcan" ||
		pMonster->Get_Name() == L"Drone_Decagram_Missile")
	{
		pMonster->Set_BodyBoneName("Bone_Top");
	}

	Management Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Test, pMonster);
	m_vecMonster[iSelect].emplace_back(pMonster);
	pMonster->AddRef();
}

//���� ����
void CTool_World::OnBnClickedMw1223BtnSaveFieldInfo()
{
	CFileDialog Dlg(FALSE,
		L"world",
		L"*.world",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"..\\..\\Reference\\Data\\Data_World\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		DWORD dwByte;
		_int iSaveMapSize;
		//���� ����
		vector<CGameObject*>* vecMap = Management Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
		if (vecMap != nullptr)
		{
			iSaveMapSize = (_int)vecMap->size();
			CUtility::Data_WriteInt(hFile, iSaveMapSize, dwByte);

			for (_int i = 0; i < iSaveMapSize; ++i)
			{
				(*vecMap)[i]->Save(hFile, dwByte);
			}
		}
		else
		{
			iSaveMapSize = 0;
			CUtility::Data_WriteInt(hFile, iSaveMapSize, dwByte);
		}

		vecMap = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);
		if (vecMap != nullptr)
		{
			iSaveMapSize = (_int)vecMap->size();
			CUtility::Data_WriteInt(hFile, iSaveMapSize, dwByte);

			for (_int i = 0; i < iSaveMapSize; ++i)
			{
				(*vecMap)[i]->Save(hFile, dwByte);
			}
		}
		else
		{
			iSaveMapSize = 0;
			CUtility::Data_WriteInt(hFile, iSaveMapSize, dwByte);
		}

		//���� Ǯ ����
		_int iSize = (_int)m_vecMonsterPool.size();
		CUtility::Data_WriteInt(hFile, iSize, dwByte);

		for (auto& tMonsterInfo : m_vecMonsterPool)
		{
			WriteFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);
		}

		//�׷� ����
		iSize = (_int)m_vecGroup.size();
		CUtility::Data_WriteInt(hFile, iSize, dwByte);
		for (auto& vecMonster : m_vecGroup)
		{
			iSize = (_int)vecMonster.size();
			CUtility::Data_WriteInt(hFile, iSize, dwByte);
			for (auto& pMonster : vecMonster)
			{
				_vec3 vPos = pMonster->Get_Info(E_INFO::INFO_POS);
				CUtility::Data_WriteVec3(hFile, vPos, dwByte);
				_vec3 vRot = pMonster->Get_Angle();
				CUtility::Data_WriteVec3(hFile, vRot, dwByte);
				T_MONSTER_INFO tMonsterInfo = pMonster->Get_MonsterInfo();
				WriteFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);
			}
		}

		//��Ÿ�� ��ġ ����
		OnBnClickedMw1227BtnSetStartPos();

		map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Environment);
		for (auto& tPair : *pLayerMap)
		{
			if (tPair.first == E_OBJTAG::OBJ_Obstacle)
			{
				iSize = (_int)tPair.second.size();
				CUtility::Data_WriteInt(hFile, iSize, dwByte);
				_vec3 vPos;
				for (auto& pStartMark : tPair.second)
				{
					vPos = pStartMark->Get_Info(E_INFO::INFO_POS);
					CUtility::Data_WriteVec3(hFile, vPos, dwByte);
				}
			}
		}

		//���� ���, Ÿ�� ����
		iSize = (_int)m_eWorldMod;
		CUtility::Data_WriteInt(hFile, iSize, dwByte);
		iSize = (_int)m_eWorldType;
		CUtility::Data_WriteInt(hFile, iSize, dwByte);

		CloseHandle(hFile);
	}
}

//���� �ҷ�����
void CTool_World::OnBnClickedMw1223BtnLoadFieldInfo()
{
	CFileDialog Dlg(TRUE,
		L"world",
		L"*.world",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_World");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DeleteAll();


		DWORD dwByte;

		_int iSize;

		//���� �ҷ�����
		CUtility::Data_ReadInt(hFile, iSize, dwByte);

		for (int i = 0; i < iSize; ++i)
		{
			_int iObjTag = 0;
			wstring wstrName;
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrName, dwByte);
			CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			CStageMap* pMap;
			pMap = CStageMap::Create(Management GetDevice(), E_OBJTAG::OBJ_Map, wstrPath, wstrName);
			pMap->Load(hFile, dwByte);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, pMap);
		}

		CUtility::Data_ReadInt(hFile, iSize, dwByte);

		for (int i = 0; i < iSize; ++i)
		{
			_int iObjTag = 0;
			wstring wstrName;
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrName, dwByte);
			CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			CObstacle* pObstacle;

			pObstacle = CObstacle::Create(Management GetDevice(), E_OBJTAG::OBJ_Obstacle, wstrPath, wstrName);
			pObstacle->Load(hFile, dwByte);
			Management Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, pObstacle);

		}



		CUtility::Data_ReadInt(hFile, iSize, dwByte);

		for (_int i = 0; i < iSize; ++i)
		{
			T_MONSTER_INFO tMonsterInfo;
			ZeroMemory(&tMonsterInfo, sizeof(T_MONSTER_INFO));
			ReadFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);

			m_vecMonsterPool.emplace_back(tMonsterInfo);
			m_vecMonster.emplace_back(vector<CMonster*>());
		}
		CUtility::Data_ReadInt(hFile, iSize, dwByte);
		_int iSize2;
		for (_int i = 0; i < iSize; ++i)
		{
			CUtility::Data_ReadInt(hFile, iSize2, dwByte);

			m_vecGroup.emplace_back(vector<CMonster*>());
			_vec3 vPos;
			_vec3 vRot;
			T_MONSTER_INFO tMonsterInfo;
			for (_int j = 0; j < iSize2; ++j)
			{
				CUtility::Data_ReadVec3(hFile, vPos, dwByte);
				CUtility::Data_ReadVec3(hFile, vRot, dwByte);

				ReadFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);

				CMonster* pMonster = CMonster::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, tMonsterInfo.wszName);
				Management Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Test, pMonster);
				pMonster->Set_Pos(vPos);
				pMonster->Set_Rotation(vRot);
				pMonster->Set_MonsterInfo(tMonsterInfo);

				if (pMonster->Get_Name() == L"Binah_Tail")
				{
					pMonster->Set_BodyBoneName("Pivot2");
				}
				if (pMonster->Get_Name() == L"Binah")
				{
					pMonster->Set_BodyBoneName("Bone_Body_00");
				}
				if (pMonster->Get_Name() == L"Hod")
				{
					pMonster->Set_BodyBoneName("bone_body_02");
				}
				if (pMonster->Get_Name() == L"HeavyTank_Decagram_Cannon")
				{
					pMonster->Set_BodyBoneName("Bone_Lower");
				}
				if (pMonster->Get_Name() == L"HeavyTank01_Helmet_Cannon" ||
					pMonster->Get_Name() == L"LightTank_Helmet_Vulcan")
				{
					pMonster->Set_BodyBoneName("Bone_Body");
				}

				m_vecGroup[i].emplace_back(pMonster);
				pMonster->AddRef();

			}
		}

		//��Ÿ�� ��ġ �ҷ�����
		OnBnClickedMw1227BtnSetStartPos();

		map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Environment);
		for (auto& tPair : *pLayerMap)
		{
			if (tPair.first == E_OBJTAG::OBJ_Obstacle)
			{
				CUtility::Data_ReadInt(hFile, iSize, dwByte);
				_vec3 vPos;
				for (_int i = 0; i < iSize; ++i)
				{
					CUtility::Data_ReadVec3(hFile, vPos, dwByte);
					tPair.second[i]->Set_Pos(vPos);
				}
			}
		}

		//���� ���, Ÿ�� �ҷ�����
		CUtility::Data_ReadInt(hFile, iSize, dwByte);
		m_cComboWorldMod.SetCurSel(iSize);
		m_eWorldMod = E_WORLD_MOD(iSize);

		CUtility::Data_ReadInt(hFile, iSize, dwByte);
		m_cComboWorldType.SetCurSel(iSize);
		m_eWorldType = E_WORLD_TYPE(iSize);

		MonsterPoolObjectListReLoad();
		MonsterPoolReLoad();
		GroupReLoad();
		GroupObjectReLoad();
		CloseHandle(hFile);
	}
}

//�׷� ����
void CTool_World::OnBnClickedMw1222BtnAddGroup()
{

	WCHAR wszGroupName[16];
	swprintf_s(wszGroupName, L"Group %d", _int(m_vecGroup.size()));
	m_vecGroup.emplace_back(vector<CMonster*>());
	m_cListGroup.AddString(wszGroupName);

}

//�׷� ����
void CTool_World::OnBnClickedMw1222BtnDeleteGroup()
{
	_int iSelect = m_cListGroup.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecGroup.size())
	{
		return;
	}
	auto iter = m_vecGroup.begin();

	for (_int i = 0; i < iSelect; ++i)
	{
		++iter;
	}

	for (auto& pMonster : (*iter))
	{
		pMonster->Set_Dead(true);
		Safe_Release(pMonster);
	}
	m_vecGroup.erase(iter);
	GroupReLoad();

}

//��ŷ�� ������Ʈ�� �����ϴ� �Լ�
void CTool_World::OnBnClickedMw1226BtnPickObjectDelete()
{
	if (!m_pPickObject)
	{
		return;
	}

	m_pPickObject->Set_Dead(true);
	Safe_Release(m_pPickObject);
}

//��ŷ�� ������Ʈ�� �׷쿡 �����ϴ� �Լ�
void CTool_World::OnBnClickedMw1222BtnAddObject()
{
	_int iGroupSelect = m_cListGroup.GetCurSel();
	if (iGroupSelect < 0 || iGroupSelect >= (_int)m_vecGroup.size())
	{
		return;
	}
	if (!m_pPickObject)
	{
		return;
	}

	_int iGroupIndex = 0;
	for (auto& vecMonster : m_vecGroup)
	{
		for (auto& pMonster : vecMonster)
		{
			if (m_pPickObject == pMonster)
			{
				if (iGroupIndex == iGroupSelect)
				{
					MessageBox(L"�̹� ���� �׷쿡 �Ҽӵ� ������Ʈ �Դϴ�.", L"Error", MB_OK);
				}
				else
				{
					MessageBox(L"�̹� �ٸ� �׷쿡 �Ҽӵ� ������Ʈ �Դϴ�.", L"Error", MB_OK);
				}
				return;
			}
		}
		++iGroupIndex;
	}

	m_vecGroup[iGroupSelect].emplace_back(m_pPickObject);
	m_pPickObject->AddRef();
	GroupObjectReLoad();

}


//�׷쿡�� ������ ������Ʈ�� �����ϴ� �Լ�
void CTool_World::OnBnClickedMw1222BtnDeleteObject()
{
	_int iGroupObjectSelect = m_cListGroupObject.GetCurSel();
	_int iGroupSelect = m_cListGroup.GetCurSel();
	if (iGroupSelect < 0 || iGroupSelect >= (_int)m_vecGroup.size())
	{
		return;
	}
	if (iGroupObjectSelect < 0 || iGroupObjectSelect >= (_int)m_vecGroup[iGroupSelect].size())
	{
		return;
	}

	auto iter = m_vecGroup[iGroupSelect].begin();
	for (_int i = 0; i < iGroupObjectSelect; ++i)
	{
		++iter;
	}

	(*iter)->Set_Dead(true);
	Safe_Release(*iter);
	m_vecGroup[iGroupSelect].erase(iter);

	GroupObjectReLoad();
}


void CTool_World::OnLbnSelchangeMw1222ListGroup()
{
	GroupObjectReLoad();
}

//�÷��̾� ĳ���� ���������� ������ �� �ִ� ������Ʈ�� �����Ѵ�.
void CTool_World::OnBnClickedMw1227BtnSetStartPos()
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Environment);
	for (auto& tPair : *pLayerMap)
	{
		if (tPair.first == E_OBJTAG::OBJ_Obstacle)
		{
			return;
		}
	}
	CGameObject* pObj;
	for (_int i = 1; i < 7; ++i)
	{
		pObj = CStartMark::Create(Management GetDevice(), E_OBJTAG::OBJ_Obstacle, i);
		assert(pObj);
		Management Add_GameObject(E_LAYERTAG::Environment, pObj->Get_ObjTag(), pObj);
		pObj->Set_Pos(_vec3(0.f, 0.01f, 0.f));
		pObj->Set_Rotation(90.f, 0.f, 0.f);
		pObj->Set_Scale(0.5f, 0.5f, 0.5f);
	}
}



////////////////////////////// �� �����δ� ������ �� �Է½� ���� �����Ϳ� ���� ä���ִ� �Լ� /////////////////////////////////
void CTool_World::OnEnChangeMw1222EditDisplayName()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	lstrcpy(m_vecMonsterPool[iSelect].wszDisplayName, m_strDisplayName);
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnEnChangeMw1222EditLevel()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].iLevel = _wtoi(m_strLevel);
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnCbnSelchangeMw1222ComboTypeMonster()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].eMonsterType = (E_MONSTER_TYPE)m_cComboMonsterType.GetCurSel();
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnCbnSelchangeMw1222ComboTypeAttack()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].eAtkType = (E_CHAR_ATKTYPE)(m_cComboAttackType.GetCurSel() + 1);
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnCbnSelchangeMw1222ComboTypeDefence()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].eDiffType = (E_CHAR_DFFTYPE)(m_cComboDefenceType.GetCurSel() + 1);
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnCbnSelchangeMw1222ComboTypeWeapon()
{
	UpdateData(true);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].eWeaponType = (E_CHAR_WEAPONTYPE)(m_cComboWeaponType.GetCurSel() + 1);
	UpdateData(false);
	MonsterInfoUpdate(iSelect);
}




void CTool_World::OnEnChangeMw1223EditColRadius()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].fRadius = (_float)_wtof(m_strColRadius);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnEnChangeMw1223EditScaleX()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].vScale.x = (_float)_wtof(m_strScaleX);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnEnChangeMw1223EditScaleY()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].vScale.y = (_float)_wtof(m_strScaleY);
	MonsterInfoUpdate(iSelect);
}


void CTool_World::OnEnChangeMw1223EditScaleZ()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cListMonsterPool.GetCurSel();
	if (iSelect < 0 || iSelect >= (_int)m_vecMonsterPool.size())
	{
		return;
	}
	m_vecMonsterPool[iSelect].vScale.z = (_float)_wtof(m_strScaleZ);
	MonsterInfoUpdate(iSelect);
}

void CTool_World::OnCbnSelchangeMw1227ComboWorldMod()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cComboWorldMod.GetCurSel();
	if (iSelect<0 || iSelect>(_int)E_WORLD_MOD::MOD_END)
	{
		return;
	}
	m_eWorldMod = E_WORLD_MOD(iSelect);
}


void CTool_World::OnCbnSelchangeMw1227ComboWorldType()
{
	UpdateData(true);
	UpdateData(false);
	_int iSelect = m_cComboWorldMod.GetCurSel();
	if (iSelect<0 || iSelect>(_int)E_WORLD_TYPE::TYPE_END)
	{
		return;
	}
	m_eWorldType = E_WORLD_TYPE(iSelect);
}


