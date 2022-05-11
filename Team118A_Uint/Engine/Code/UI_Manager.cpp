#include "UI_Manager.h"
#include "Scene.h"
#include "Sound_Manager.h"
#include "DataManager.h"
#include "SMesh.h"
#include "GameManager.h"
#include "XL_ParsingMgr.h"
#include "EffectObject.h"
#include "Invite_Character.h"
#include "CharacterMgr.h"
#include "Mascot.h"
#include "Run_Player.h"
#include "Dialog_Manager.h"

USING(Engine)

#include "UI_Scroll_View.h"
#include "UI_Slider_Vert.h"
#include "UI_Damage.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{

}

CUI_Manager::~CUI_Manager()
{
	Free();
}

void CUI_Manager::Free()
{
	m_pComVtory_Animation = nullptr;
}

void CUI_Manager::Initialize(HWND * pHwnd)
{
	m_pHwnd = pHwnd;
}

void CUI_Manager::Scene_Change_Start()
{
	map<E_OBJTAG, vector<CGameObject*>>* pLayer = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::UI_Lobby);
	pLayer->clear();
	Safe_Release(m_pComVtory_Animation);
	m_pComVtory_Animation = nullptr;
}

void CUI_Manager::Scene_Change_End()
{
	if (m_pLobbyPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pLobbyPanel->Get_Parent()->Get_ObjTag(), m_pLobbyPanel->Get_Parent());
	}

	if (m_pHighInfoPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pHighInfoPanel->Get_ObjTag(), m_pHighInfoPanel);
	}
	if (m_pCollectionPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pCollectionPanel->Get_ObjTag(), m_pCollectionPanel);
	}
	if (m_pCharDetail)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pCharDetail->Get_ObjTag(), m_pCharDetail);
	}
	if (m_pBattleFormationPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pBattleFormationPanel->Get_ObjTag(), m_pBattleFormationPanel);
	}
	if (m_pWorkPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pWorkPanel->Get_ObjTag(), m_pWorkPanel);
	}
	if (m_pMissionPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pMissionPanel->Get_ObjTag(), m_pMissionPanel);
	}
	if (m_pMissionInfoBackground)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pMissionInfoBackground->Get_ObjTag(), m_pMissionInfoBackground);
	}

	if (m_pTotal_Panel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pTotal_Panel->Get_ObjTag(), m_pTotal_Panel);
	}

	if (m_pTotalInfo_Panel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pTotalInfo_Panel->Get_ObjTag(), m_pTotalInfo_Panel);
	}

	if (m_pBestCafePanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pBestCafePanel->Get_ObjTag(), m_pBestCafePanel);
	}
	if (m_pCombatIngPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pCombatIngPanel->Get_ObjTag(), m_pCombatIngPanel);
	}

	if (m_pEnemyInfoPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pEnemyInfoPanel->Get_ObjTag(), m_pEnemyInfoPanel);
	}

	if (m_pCombatVictoryPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pCombatVictoryPanel->Get_ObjTag(), m_pCombatVictoryPanel);
	}
	if (m_pStoryPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pStoryPanel->Get_ObjTag(), m_pStoryPanel);
	}

	if (m_pRegisterPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pRegisterPanel->Get_ObjTag(), m_pRegisterPanel);
	}
	if (m_pRankingPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pRankingPanel->Get_ObjTag(), m_pRankingPanel);
	}

	if (m_pTileMissionInfoPanel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pTileMissionInfoPanel->Get_ObjTag(), m_pTileMissionInfoPanel);
	}

	if (m_pGacha_Panel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pGacha_Panel->Get_ObjTag(), m_pGacha_Panel);
	}

	if (m_pGachaResult_Panel)
	{
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pGachaResult_Panel->Get_ObjTag(), m_pGachaResult_Panel);
	}

}

// UI�� ���õ� ��ư�̳� �ɼ��� ������������ Update
void CUI_Manager::Update_UIManager(const _float & fTimeDelta)
{
	if (nullptr == m_pLobbyPanel)
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_U) && CKeyManager::GetInstance()->Key_Down(KEY_0))
		{
			Create_Ui_Lobby();
		}
		else if (CKeyManager::GetInstance()->Key_Pressing(KEY_U) && CKeyManager::GetInstance()->Key_Down(KEY_2))
		{
			Create_GachaResult_UI();
			m_pGachaResult_Panel->Set_Active(TRUE);
			Change_GachaResultType(E_GachaResultType::Start_Direct);
		}
	}

	// �ɼ� â On �� �� �����̴��� �޾ƿ���
	if (m_pOptionPanel && m_pOptionPanel->Get_ChildByIndex(1)->Get_Active())
	{
		_float fBGM = static_cast<CUI_Slider_Vert*>(m_pOptionPanel->
			Get_ChildByIndex(1)->Get_ChildByIndex(6)->Get_ChildByIndex(5)->Get_ChildByIndex(0))->Get_Slider_Ratio();
		CSound_Manager::GetInstance()->Set_Volume_BGM(fBGM);

		_float fFX = static_cast<CUI_Slider_Vert*>(m_pOptionPanel->
			Get_ChildByIndex(1)->Get_ChildByIndex(6)->Get_ChildByIndex(5)->Get_ChildByIndex(1))->Get_Slider_Ratio();
		CSound_Manager::GetInstance()->Set_Volume_FX(fFX);

		_float fVoice = static_cast<CUI_Slider_Vert*>(m_pOptionPanel->
			Get_ChildByIndex(1)->Get_ChildByIndex(6)->Get_ChildByIndex(5)->Get_ChildByIndex(2))->Get_Slider_Ratio();
		CSound_Manager::GetInstance()->Set_Volume_Voice(fVoice);
	}

	if (m_pLobbyPanel && m_pLobbyPanel->Get_Active())
	{
		m_pLobby_APNumber_Left->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_AP()) + L"/" + to_wstring(CDataManager::GetInstance()->Get_Level() * 10));
		m_pLobby_CreditNumber->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_Credit()));
		m_pLobby_BluePyroxeneNumber->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_BluePyroxene()));
	}

	if (m_pHighInfoPanel && m_pHighInfoPanel->Get_Active())
	{
		//m_pHighPanel_APNumber_Left->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_AP()) + L"/" + to_wstring(CDataManager::GetInstance()->Get_Level() * 10));
		m_pHighPanel_CreditNumber->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_Credit()));
		m_pHighPanel_BluePyroxeneNumber->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_BluePyroxene()));
	}

	if (m_pBattleFormationPanel)
	{
		if (m_pBattleFormationPanel->Get_Active() && !m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_Active())
		{
			BattleFormation_MouseControl();
		}
	}

	Update_LobbyUI(fTimeDelta);
	//Update_StudentDetail();
	Update_RunGame_UI(fTimeDelta);
	Update_Combat_Ing(fTimeDelta);
	Update_Register(fTimeDelta);
	Update_GachaPanelFor_CreateParticle(fTimeDelta);
	Update_GachaResult(fTimeDelta);
}

// Prefab�� Load�ؼ� ��ȭ���ش�.
CUI_Object * CUI_Manager::Load_UI_Prefab(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;

	wstring wstrUITag;
	CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

	_int iObjTag = 0;
	E_OBJTAG eTag = E_OBJTAG::OBJ_END;
	CUtility::Data_ReadInt(hFile, iObjTag, dwByte);
	eTag = (E_OBJTAG)iObjTag;

	CUI_Object* pUi = nullptr;
	if (L"CUI_Object" == wstrUITag)
	{
		pUi = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Slider_Vert" == wstrUITag)
	{
		pUi = CUI_Slider_Vert::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Scroll_View" == wstrUITag)
	{
		pUi = CUI_Scroll_View::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Damage" == wstrUITag)
	{
		pUi = CUI_Damage::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_NUMBER, 9999999.f, E_NumberFontType::Normal, 0);
		pUi->Load(hFile, dwByte);
	}

	assert(pUi);

	CloseHandle(hFile);

	return pUi;
}

// Layer�� Load�ؼ� ��ȯ���ش�. ��� Load�ϴ� �ֻ��� �θ����
// pParent��� �ƹ��͵� ���� �θ� ����� �ڽ����� �־��ش�.
CUI_Object * CUI_Manager::Load_UI_Layer(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;

	CUI_Object* pParent = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI);
	pParent->Set_ImageAlpha(0.f);
	pParent->Set_ChildClickCheck(TRUE);
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
			pUi = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI);
			pUi->Load(hFile, dwByte);
			pParent->Set_Child(pUi);
		}
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
			pUi = CUI_Slider_Vert::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER);
			pUi->Load(hFile, dwByte);
			pParent->Set_Child(pUi);
		}
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
			pUi = CUI_Scroll_View::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
			pUi->Load(hFile, dwByte);
			pParent->Set_Child(pUi);
		}
	}

	CloseHandle(hFile);
	return pParent;
}

_vec3 CUI_Manager::Click_Map()
{

	vector<CGameObject*>* pMaps = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
	if (nullptr == pMaps)
	{
		return VEC_ZERO;
	}


	_int iMapSize = (_int)pMaps->size();
	_vec3 vPickPos = VEC_ZERO;
	_vec3 vResultPos = VEC_ZERO;
	_float fDist = 999999.f;
	_float fMinDist = 999999.f;
	_bool bFind = FALSE;
	for (_int i = 0; i < iMapSize; ++i)
	{
		if (static_cast<CSMesh*>(static_cast<CStageMap*>((*pMaps)[i])->Get_Component(ComTag_SMesh))->PickingMesh(*m_pHwnd, &vPickPos, &fDist, (*pMaps)[i]->Get_Transform()->Get_WorldMatrix()))
		{
			// �浹
			CUtility::Print_Wstring_DMode(L"�ʰ� �浹����.");
			if (fMinDist > fDist)
			{
				vPickPos.y = 0.f;
				vResultPos = vPickPos;
				bFind = TRUE;
				m_vPickingPos = vResultPos;
				fMinDist = fDist;
			}
		}
	}

	return vResultPos;
}

#pragma region �κ� �� �ֻ��� UI


void CUI_Manager::Create_Ui_HighInfoPanel()
{
	if (m_pHighInfoPanel)
		return;

	m_pHighInfoPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��ܹ�_�θ�.uidat");
	m_pHighInfoPanel->Set_Sorting_Layer(99);
	m_pHighInfoPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pHighInfoPanel->Get_ObjTag(), m_pHighInfoPanel);
	m_pHighInfoPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Goto_BeforePanel, this));
	m_pHighInfoPanel->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Goto_Lobby, this));

	m_pHighPanel_PanelName				= m_pHighInfoPanel->Get_ChildByIndex(1);
	m_pHighPanel_APNumber_Left			= m_pHighInfoPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0);
	m_pHighPanel_CreditNumber			= m_pHighInfoPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0);
	m_pHighPanel_BluePyroxeneNumber		= m_pHighInfoPanel->Get_ChildByIndex(4)->Get_ChildByIndex(0);
}

// Lobby UI�� �����Ѵ�.
void CUI_Manager::Create_Ui_Lobby()
{
	if (m_pLobbyPanel)
		return;

	// UI_Lobby�� 0�� �ε����� LobbyPanel�̴�.
	CUI_Object* pUi = nullptr;
	pUi = Load_UI_Layer(L"../../Reference/Data/Data_UI/Layer/UI_Lobby.uidat");
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, pUi->Get_ObjTag(), pUi);
	pUi->Set_NameTag(L"�κ�");
	pUi->Set_Active(TRUE);
	m_pLobbyPanel = pUi->Get_ChildByIndex(0);
	m_pLobby_APNumber_Left = m_pLobbyPanel->Get_ChildByIndex(7)->Get_ChildByIndex(3);
	m_pLobby_CreditNumber = m_pLobbyPanel->Get_ChildByIndex(8)->Get_ChildByIndex(2);
	m_pLobby_BluePyroxeneNumber = m_pLobbyPanel->Get_ChildByIndex(9)->Get_ChildByIndex(3);
	m_pLobbyPanel->Get_ChildByIndex(10)->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnOff_Option, this, true), true);

	_int iCharSize = (_int)m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildList()->size();
	for (_int i = 0; i < iCharSize; i++)
	{
		m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Active(FALSE);
	}
	m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex((_int)CDataManager::GetInstance()->Get_UserInfo().eLobby_Char)->Set_Active(TRUE);


	// ���� ��ư Ŭ�� 
	m_pLobbyPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Work_OnEnable, this));

	// �α��� ��ư Ŭ�� 
	m_pLobbyPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_LoginRegister, this));

	// ��ŷ ��ư Ŭ�� 
	m_pLobbyPanel->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_Ranking, this));

	// �κ� ĳ���� �ʻ�ȭ �� ID, Level
	m_pLobby_ID = m_pLobbyPanel->Get_ChildByIndex(6);
	m_pLobby_Level = m_pLobbyPanel->Get_ChildByIndex(6)->Get_ChildByIndex(0);
	m_pLobby_Charillust = m_pLobbyPanel->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0);

	m_pLobby_ID->Set_Ui_Text(L"ID : �α��� �ؾ��մϴ�.");
	m_pLobby_Level->Set_Ui_Text(L"LV. 1");


	wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
		CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)CDataManager::GetInstance()->Get_UserInfo().eLobby_Char, E_CHAR_ENUMTYPE::CNUM_NAME) +
		L".png";
	m_pLobby_Charillust->Set_Tex1_Tag(wstrTexName);


	// ī�� ��ư Ŭ��
	m_pLobbyPanel->Get_ChildByIndex(12)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Scene_Change_Cafe, this));

	// �л��� ��ư Ŭ��
	m_pLobbyPanel->Get_ChildByIndex(12)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Create_Ui_CharCollection, this));

	// ��â ��ư Ŭ��
	m_pLobbyPanel->Get_ChildByIndex(12)->Get_ChildByIndex(4)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Open_Ui_BattleFormation, this, (_int)E_WORLD_MOD::MOD_END), (_int)E_WORLD_MOD::MOD_MISSION);

	// ���� ��ư Ŭ��
	m_pLobbyPanel->Get_ChildByIndex(12)->Get_ChildByIndex(7)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Enter_Shop_UI, this));

	// ����(��í) ��ư Ŭ��
	m_pLobbyPanel->Get_ChildByIndex(12)->Get_ChildByIndex(8)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnEnable_GachaPanel, this));

	// ģ�� ����Ʈ
	m_pLobby_FriendList = m_pLobbyPanel->Get_ChildByIndex(13);

	// �����̴� �Ϸ���Ʈ �θ�
	m_pLobby_Moveillust = m_pLobbyPanel->Get_ChildByIndex(14);

	m_pLobbyPanel->Get_ChildByIndex(15)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_FriendList, this));

	m_pOptionPanel = pUi->Get_ChildByIndex(1);
	m_pOptionPanel->Get_ChildByIndex(1)->Get_ChildByIndex(5)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::InventoryPanel_OnOff, this, 1), 1);

	Create_Ui_Inventory(pUi);
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_WorkPanel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_Cafe();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_Story();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_MissionPanel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_EnemyInfoPanel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_TotalPanel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Combat_Ing_Panel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Combat_Victory_Panel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_Register();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Ui_Ranking();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_Gacha_UI();
	if (m_pLoadingGage) *m_pLoadingGage += 1;

	Create_GachaResult_UI();
	if (m_pLoadingGage) *m_pLoadingGage += 1;



	// ��ܹ� �г��� �ε�
	Create_Ui_HighInfoPanel();
	if (m_pLoadingGage) *m_pLoadingGage += 1;
	
	//��â UI ����
	Create_Ui_BattleFormation();
	
	Sleep(100);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����ȭ��.mp3", 0.1f);
}

void CUI_Manager::OnOff_Option(_int iState)
{
	m_pOptionPanel->Set_Active(iState != 0 ? true : false);
}

void CUI_Manager::Goto_BeforePanel()
{
	_bool bMissionPanel = m_pMissionPanel ? m_pMissionPanel->Get_Active() : FALSE;
	_bool bCharDetailPanel = m_pCharDetail ? m_pCharDetail->Get_Active() : FALSE;
	_bool bStoryEpisodePanel = m_pStory_Choose_Episode_Panel ? m_pStory_Choose_Episode_Panel->Get_Active() : FALSE;
	_bool bSubStoryPanel = m_pStory_SubParent_Panel ? m_pStory_SubParent_Panel->Get_Active() : FALSE;
	_bool bTotalPanel = m_pTotal_Panel ? m_pTotal_Panel->Get_Active() : FALSE;
	_bool bStoryPanel = m_pStoryPanel ? m_pStoryPanel->Get_Active() : FALSE;

	Goto_Lobby();

	if (bMissionPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pWorkPanel->Set_Active(TRUE);
		CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����.mp3", 0.1f);
		m_pHighPanel_PanelName->Set_Ui_Text(L"����");
	}
	else if (bCharDetailPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pCollectionPanel->Set_Active(TRUE);

		Initialize_StudentDetail();
		m_pHighPanel_PanelName->Set_Ui_Text(L"�л���");
	}
	else if (bSubStoryPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pStoryPanel->Set_Active(TRUE);
		m_pStory_SubParent_Panel->Set_Active(FALSE);
		m_pHighPanel_PanelName->Set_Ui_Text(L"���丮");
	}
	else if (bStoryEpisodePanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pStoryPanel->Set_Active(TRUE);
		m_pStory_Choose_Episode_Panel->Set_Active(FALSE);
		m_pHighPanel_PanelName->Set_Ui_Text(L"���丮");
	}
	else if (bStoryPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pWorkPanel->Set_Active(TRUE);
		CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����.mp3", 0.1f);
		m_pHighPanel_PanelName->Set_Ui_Text(L"����");
	}
	else if (bTotalPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);

		m_pWorkPanel->Set_Active(TRUE);
		CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����.mp3", 0.1f);
		m_pHighPanel_PanelName->Set_Ui_Text(L"����");
	}
	else
	{
		CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����ȭ��.mp3", 0.1f);
	}
}

void CUI_Manager::Goto_Lobby()
{
	_bool	bStudentDetail_Active = FALSE;

	if (m_pCharDetail)
	{
		bStudentDetail_Active = m_pCharDetail->Get_Active();
	}

	if (m_pBattleFormationPanel)
	{
		Set_LobbyInfoUI();
	}

	vector<CGameObject*>* pVecUi = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::UI_Lobby, E_OBJTAG::OBJ_UI);

	if (nullptr == pVecUi)
	{
		return;
	}

	auto iter = (*pVecUi).begin();

	for (; iter != (*pVecUi).end(); ++iter)
	{
		if (iter == (*pVecUi).begin())
			continue;

		(*iter)->Set_Active(FALSE);
	}

	BattleFormation_ActiveSquad(FALSE);

	if (m_pLobbyPanel)
	{
		m_pLobbyPanel->Set_Active(TRUE);
	}

	if (m_pOptionPanel)
		m_pOptionPanel->Set_Active(FALSE);

	if (m_pInventoryPanel)
		m_pInventoryPanel->Set_Active(FALSE);
}

void CUI_Manager::Set_LobbyInfoUI()
{
	if (nullptr == m_pLobby_ID)
		return;
	E_CHAR_NAME eName = E_CHAR_NAME::Name_Neru;


	_int iCharSize = (_int)m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildList()->size();
	for (_int i = 0; i < iCharSize; i++)
	{
		m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Active(FALSE);
	}

	if (CGameManager::GetInstance()->Get_CurrentSquad_Strikers())
	{
		for (auto& iter : (*CGameManager::GetInstance()->Get_CurrentSquad_Strikers()))
		{
			if (nullptr == iter)
				continue;

			m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex((_int)CDataManager::GetInstance()->Get_UserInfo().eLobby_Char)->Set_Active(FALSE);
			eName = iter->Get_CharInfo().eName[0];
			CDataManager::GetInstance()->Set_LobbyChar(eName);
			m_pLobbyPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex((_int)eName)->Set_Active(TRUE);
			break;
		}
	}


	m_pLobby_ID->Set_Ui_Text(L"ID : " + wstring(CDataManager::GetInstance()->Get_UserInfo().wszNickName));
	m_pLobby_Level->Set_Ui_Text(L"Lv. " + to_wstring(CDataManager::GetInstance()->Get_UserInfo().iLevel));

	wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
		CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)eName, E_CHAR_ENUMTYPE::CNUM_NAME) +
		L".png";
	m_pLobby_Charillust->Set_Tex1_Tag(wstrTexName);

}

void CUI_Manager::SceneChange_Goto_Lobby()
{
	m_pFunc_LobbyScene(E_SCENETAG::Scene_Lobby);
}

void CUI_Manager::Set_FriendList()
{
	_int iFriendSize = (_int)m_vecPlayfab_FriendInfo.size();
	_int iFriendPrefabSize = (_int)m_pLobby_FriendList->Get_ChildList()->size();
	_int iDiff = iFriendSize - iFriendPrefabSize;
	for (_int i = 0; i < iDiff; i++)
	{
		CUI_Object* pFriendPrefab = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]ģ��Prefab.uidat");
		m_pLobby_FriendList->Set_Child(pFriendPrefab);
	}

	for (_int i = iFriendSize; i < (_int)m_pLobby_FriendList->Get_ChildList()->size(); ++i)
	{
		m_pLobby_FriendList->Get_ChildByIndex(i)->Set_Active(FALSE);
	}
	
	for (_int i = 0; i < iFriendSize; i++)
	{
		T_PlayfabInfo& tInfo = m_vecPlayfab_FriendInfo[i];
		CUI_Object* pFriendPrefab = m_pLobby_FriendList->Get_ChildByIndex(i);

		pFriendPrefab->Set_Active(TRUE);
		pFriendPrefab->Set_Ui_PosY(60 - 63 * i);
		wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
			CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)tInfo.iLobbyChar, E_CHAR_ENUMTYPE::CNUM_NAME) +
			L".png";
		pFriendPrefab->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

		pFriendPrefab->Get_ChildByIndex(2)->Set_Ui_Text(tInfo.wstrNickname);
		pFriendPrefab->Get_ChildByIndex(3)->Set_Ui_Text(L"Lv." + to_wstring(tInfo.iAccountLevel));


		// ������ �α���
		T_DATEINFO tMine;
		CDataManager::GetInstance()->Get_CurrentDate(&tMine);

		time_t tFriendTime = 0;
		for (_int i = 0; i < (_int)m_vecPlayfab_RankingInfo.size(); ++i)
		{
			if (m_vecPlayfab_RankingInfo[i].strPlayFabID == tInfo.strPlayFabID)
			{
				tFriendTime = m_vecPlayfab_RankingInfo[i].tLastLogin;
			}
		}

		T_DATEINFO tFriend;
		CDataManager::GetInstance()->Get_ConvertDate(&tFriend, tFriendTime);

		_int iDiffSecond = CDataManager::GetInstance()->Get_DateDiffrence(tMine, tFriend);
		_int minute = 60;
		_int hour = 3600;
		_int day = hour * 24;

		_int iDiffDay = iDiffSecond / day;
		_int iDiffHour = (iDiffSecond % day) / hour;
		_int iDiffMinute = (iDiffSecond % hour) / minute;
		iDiffDay *= -1;
		iDiffHour *= -1;
		iDiffMinute *= -1;
		if (1 <= iDiffDay)
		{
			pFriendPrefab->Get_ChildByIndex(4)->Set_Ui_Text(L"������ ���� : " +  to_wstring(iDiffDay) + L"�� ��");
		}
		else if (1 <= iDiffHour)
		{
			pFriendPrefab->Get_ChildByIndex(4)->Set_Ui_Text(L"������ ���� : " + to_wstring(iDiffHour) + L"�ð� ��");
		}
		else if (10 >= iDiffMinute)
		{
			pFriendPrefab->Get_ChildByIndex(4)->Set_Ui_Text(L"������ ���� : " + to_wstring(iDiffMinute) + L"�ð� ��");
		}
		else if (10 <= iDiffMinute)
		{
			pFriendPrefab->Get_ChildByIndex(4)->Set_Ui_Text(L"������ ���� : ��� ��");
		}
		else
		{
			pFriendPrefab->Get_ChildByIndex(4)->Set_Ui_Text(L"������ ���� : ����");
		}
	}

}

void CUI_Manager::Update_LobbyUI(const _float& fTimeDelta)
{
	if (nullptr == m_pLobby_FriendList)
	{
		return;
	}

	_int iFriendSize = (_int)m_pLobby_FriendList->Get_ChildList()->size();
	m_pLobbyPanel->Get_ChildByIndex(15)->Set_Active(0 != iFriendSize);

	if (5 <= iFriendSize)
	{
		for (_int i = 0; i < iFriendSize; ++i)
		{
			CUI_Object* pFriendPrefab = m_pLobby_FriendList->Get_ChildByIndex(i);
			CUI_Object* pFriendPrefabNext = pFriendPrefabNext = m_pLobby_FriendList->Get_ChildByIndex(i == 0 ? iFriendSize - 1 : (i - 1));

			pFriendPrefab->Add_Ui_PosY(1);

			if (pFriendPrefab->Get_Ui_Pos().y >= 130)
			{
				pFriendPrefab->Set_Ui_PosY((_int)pFriendPrefabNext->Get_Ui_Pos().y - 63);
			}
		}
	}
	else if (0 == iFriendSize)
	{
		m_pLobby_FriendList->Set_Active(FALSE);
	}

	m_fMoveillustCoolTime += fTimeDelta;

	if (m_fMoveillustCoolTime >= 3.f && FALSE == m_bMoveillust)
	{
		m_bMoveillust = TRUE;
		for (_int i = 0; i < 3; ++i)
		{
			CUI_Object* pNext = m_pLobby_Moveillust->Get_ChildByIndex(i == 0 ? 2 : (i - 1));
			m_fMoveillust_Start_X[i] = m_pLobby_Moveillust->Get_ChildByIndex(i)->Get_Ui_Pos().x;

			m_pLobby_Moveillust->Get_ChildByIndex(i)->Add_Ui_PosX(-1);
		}
	}

	if (m_bMoveillust)
	{
		_float fRatio = m_fMoveillustCoolTime - 3.f;

		// �̵� �Ϸ�
		for (_int i = 0; i < 3; ++i)
		{
			_float f_X = CUtility::Lerp_float(m_fMoveillust_Start_X[i], m_fMoveillust_Start_X[i] - 292.f, CUtility::LerpEase_float(E_EASE::InCirc, fRatio * 1.f));
			m_pLobby_Moveillust->Get_ChildByIndex(i)->Set_Ui_PosX((_int)f_X);
			m_pLobby_Moveillust->Get_ChildByIndex(i)->Update_Object(0.f);
		}

		if (1.f <= fRatio * 1.f)
		{
			m_bMoveillust = FALSE;
			m_fMoveillustCoolTime = 0.f;
			for (_int i = 0; i < 3; ++i)
			{
				CUI_Object* pNext = m_pLobby_Moveillust->Get_ChildByIndex(i == 0 ? 2 : (i - 1));
				
				if (m_pLobby_Moveillust->Get_ChildByIndex(i)->Get_Ui_Pos().x <= -292.f)
				{
					m_pLobby_Moveillust->Get_ChildByIndex(i)->Set_Ui_PosX((_int)m_fMoveillust_Start_X[i == 0 ? 2 : (i - 1)]);
				}
			}
			return;
		}
	}

	//for (_int i = 0; i < 3; ++i)
	//{
	//	CUI_Object* pNext = m_pLobby_Moveillust->Get_ChildByIndex(i == 0 ? 2 : (i - 1));
	//	m_pLobby_Moveillust->Get_ChildByIndex(i)->Add_Ui_PosX(-1);
	//
	//	if (m_pLobby_Moveillust->Get_ChildByIndex(i)->Get_Ui_Pos().x <= -300)
	//	{
	//		m_pLobby_Moveillust->Get_ChildByIndex(i)->Set_Ui_PosX((_int)pNext->Get_Ui_Pos().x + 292);
	//	}
	//}

}

void CUI_Manager::OnClick_FriendList()
{
	m_pLobby_FriendList->Set_Active(!m_pLobby_FriendList->Get_Active());
}

#pragma endregion

#pragma region �κ��丮

void CUI_Manager::Create_Ui_Inventory(CUI_Object* pParent)
{
	m_pInventoryPanel = pParent->Get_ChildByIndex(2);
	m_pInventoryPanel->Get_ChildByIndex(5)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::InventoryPanel_OnOff, this, 0), 1);

	m_pInventory_Scroll = m_pInventoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(3)->Get_ChildByIndex(0);

	// ���� OnOff
	m_pInventoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Click_Filter, this));

	// ���� ����
	m_pInventoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Click_Sort, this));

	// �������� ��������
	m_pInventoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Click_Ascending, this));



	m_pInventory_FilterPanel = m_pInventory_Scroll->Get_Parent()->Get_ChildByIndex(1);

	for (_int i = 0; i < (_int)E_ItemFilter::��ȭ��; i++)
	{
		m_pInven_Filter[i] = m_pInventory_FilterPanel->Get_ChildByIndex(i + 2);
		m_pInven_Filter[i]->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_FilterState, this, i), i);
		m_pInven_Filter[i]->Get_ChildByIndex(1)->Set_Active(FALSE);
	}


	CUI_Object* pFilter_Cancel = m_pInven_Filter[(_int)E_ItemFilter::����ǰ]->Get_Parent()->Get_ChildByIndex(8);
	CUI_Object* pFilter_Accept = m_pInven_Filter[(_int)E_ItemFilter::����ǰ]->Get_Parent()->Get_ChildByIndex(9);
	pFilter_Cancel->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Filter_Ok, this, (_int)FALSE), (_int)FALSE);
	pFilter_Accept->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Filter_Ok, this, (_int)TRUE), (_int)TRUE);

	m_pInventory_SortPanel = m_pInventory_Scroll->Get_Parent()->Get_ChildByIndex(2);

	for (_int i = 0; i < (_int)E_ItemSortType::E_End; ++i)
	{
		m_pInven_SortButton[i] = m_pInventory_SortPanel->Get_ChildByIndex(i + 1);
		m_pInven_SortButton[i]->Get_ChildByIndex(1)->Set_Active(FALSE);
		m_pInven_SortButton[i]->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_SortState, this, i), i);
	}
	m_pInventory_SortPanel->Get_ChildByIndex(5)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Sort_Ok, this, (_int)FALSE), (_int)FALSE);
	m_pInventory_SortPanel->Get_ChildByIndex(6)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Sort_Ok, this, (_int)TRUE), (_int)TRUE);
	// �׽�Ʈ
	CGameManager::GetInstance()->Get_Inventory(m_bInven_SortState, E_ItemSortType::Basic, &m_vecSortItem);
	Inven_Create_Scroll();
}


void CUI_Manager::InventoryPanel_OnOff(_int iState)
{
	if (iState == 0)
	{
		m_pInventoryPanel->Set_Active(FALSE);
	}
	else
	{
		m_pInventoryPanel->Set_Active(TRUE);
		m_pHighPanel_PanelName->Set_Ui_Text(L"�κ��丮");
	}
	m_pHighInfoPanel->Set_Active(TRUE);
}

void CUI_Manager::Inven_Click_Filter()
{
	m_pInventory_FilterPanel->Set_Active(TRUE);

	for (_int i = 0; i < (_int)E_ItemFilter::��ȭ��; i++)
	{
		m_pInven_Filter[i]->Get_ChildByIndex(1)->Set_Active(m_bInven_SortState[i]);
	}
}

void CUI_Manager::Inven_Click_Sort()
{
	m_pInventory_SortPanel->Set_Active(TRUE);
}

void CUI_Manager::Inven_Click_Ascending()
{
	if (E_AscendingType::�������� == m_eInven_AscendingType)
	{
		m_eInven_AscendingType = E_AscendingType::��������;
	}
	else
	{
		m_eInven_AscendingType = E_AscendingType::��������;
	}

	m_vecSortItem.clear();
	CGameManager::GetInstance()->Get_Inventory(m_bInven_SortState, m_eInven_ItemSort_Type, &m_vecSortItem);
	if (E_AscendingType::�������� == m_eInven_AscendingType)
	{
		reverse(m_vecSortItem.begin(), m_vecSortItem.end());
	}
	Inven_Create_Scroll();
}

void CUI_Manager::Inven_Create_Scroll()
{
	_int iDiff = (_int)m_vecSortItem.size() - (_int)m_pInventory_Scroll->Get_ChildList()->size();
	if (iDiff > 0)
	{
		for (_int i = 0; i < iDiff; ++i)
		{
			CUI_Object* pUi = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�κ��丮������.uidat");
			m_pInventory_Scroll->Set_Child(pUi);
		}
	}

	auto iter = m_pInventory_Scroll->Get_ChildList()->begin();
	_int iCount = 0;
	for (; iter != m_pInventory_Scroll->Get_ChildList()->end(); ++iter, ++iCount)
	{
		if ((_int)m_vecSortItem.size() > iCount)
		{
			(*iter)->Set_Active(TRUE);
			(*iter)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_vecSortItem[iCount]->iCount));
			(*iter)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecSortItem[iCount]->wstrImagePath);
			(*iter)->Clear_Function(E_UiFunctionType::OnClick);
			(*iter)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Inven_Change_SelectItem, this, (_int)m_vecSortItem[iCount]->eItemTag), (_int)m_vecSortItem[iCount]->eItemTag);
			// ��ȣ�� �����ϰڴ�.
		}
		else
		{
			(*iter)->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Inven_Change_SelectItem(_int iItemTag)
{
	// ������ ������
	T_ItemInfo* pItem = CGameManager::GetInstance()->Get_Item(iItemTag);
	CUI_Object* pChooseItemImage = m_pInventoryPanel->Get_ChildByIndex(1);
	CUI_Object* pItemNameText = m_pInventoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0);
	CUI_Object* pItemCountText = m_pInventoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_ChildByIndex(1);
	CUI_Object* pDescriptionText1 = m_pInventoryPanel->Get_ChildByIndex(4);
	CUI_Object* pDescriptionText2 = m_pInventoryPanel->Get_ChildByIndex(5);

	D3DXIMAGE_INFO info = CProtoMgr::GetInstance()->Get_TextureInfo(pItem->wstrImagePath);
	_int iWidth = info.Width;
	_float fRT = info.Width / 180.f;

	pChooseItemImage->Set_Ui_SizeX((_int)(info.Width / fRT));
	pChooseItemImage->Set_Ui_SizeY((_int)(info.Height / fRT));
	pChooseItemImage->Set_Tex1_Tag(pItem->wstrImagePath);

	pItemNameText->Set_Ui_Text(pItem->wstrName);
	pItemCountText->Set_Ui_Text(to_wstring(pItem->iCount));

	if (15 <= (_int)pItem->wstrDescription.size())
	{
		pDescriptionText1->Set_Ui_Text(pItem->wstrDescription.substr(0, 15));
		pDescriptionText2->Set_Ui_Text(pItem->wstrDescription.substr(15, (_int)pItem->wstrDescription.size()));
	}
	else
	{
		pDescriptionText1->Set_Ui_Text(pItem->wstrDescription);
	}
}

void CUI_Manager::Inven_FilterState(_int iFilter)
{
	m_bInven_SortTemp[iFilter] = !m_bInven_SortTemp[iFilter];


	for (_int i = 0; i < (_int)E_ItemFilter::��ȭ��; i++)
	{
		m_pInven_Filter[i]->Get_ChildByIndex(1)->Set_Active(m_bInven_SortTemp[i]);
	}

	m_pInven_Filter[iFilter]->Get_ChildByIndex(1)->Set_Active(m_bInven_SortTemp[iFilter]);
}

void CUI_Manager::Inven_Filter_Ok(_int iBool)
{
	for (_int i = 0; i < (_int)E_ItemFilter::��ȭ��; ++i)
	{
		if (TRUE == iBool)
		{
			m_bInven_SortState[i] = m_bInven_SortTemp[i];
		}
		else
		{
			m_bInven_SortTemp[i] = m_bInven_SortState[i];
		}
	}

	if (TRUE == iBool)
	{
		m_vecSortItem.clear();
		CGameManager::GetInstance()->Get_Inventory(m_bInven_SortState, m_eInven_ItemSort_Type, &m_vecSortItem);

		if (E_AscendingType::�������� == m_eInven_AscendingType)
		{
			reverse(m_vecSortItem.begin(), m_vecSortItem.end());
		}
		Inven_Create_Scroll();
		m_pInventory_FilterPanel->Set_Active(FALSE);
	}
}


void CUI_Manager::Inven_SortState(_int iSort)
{
	m_eInven_ItemSort_Type = (E_ItemSortType)iSort;

	for (_int i = 0; i < (_int)E_ItemSortType::E_End; ++i)
	{
		m_pInven_SortButton[i]->Get_ChildByIndex(1)->Set_Active(FALSE);
	}

	m_pInven_SortButton[iSort]->Get_ChildByIndex(1)->Set_Active(TRUE);
}

void CUI_Manager::Inven_Sort_Ok(_int iBool)
{
	if (TRUE == iBool)
	{
		m_vecSortItem.clear();
		CGameManager::GetInstance()->Get_Inventory(m_bInven_SortState, m_eInven_ItemSort_Type, &m_vecSortItem);
		if (E_AscendingType::�������� == m_eInven_AscendingType)
		{
			reverse(m_vecSortItem.begin(), m_vecSortItem.end());
		}
		Inven_Create_Scroll();
	}

	m_pInventory_SortPanel->Set_Active(FALSE);
}


#pragma endregion

#pragma region �л���

// �л��� UI ���� 
void CUI_Manager::Create_Ui_CharCollection()
{
	if (m_pLobbyPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);
	}

	if (!m_pCollectionPanel)
	{
		CUI_Object* pUi = nullptr;
		CUI_Object* pUi_Char = nullptr;
		pUi = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Collection_ScrollView.uidat");

		CGameManager::GetInstance()->Collect_Update();
		vector<T_CharInfo>	vecCharInfo = CCharacterMgr::GetInstance()->Get_vecCharInfo();

		wstring		wstrFileName = L"";
		wstring		wstrTexName = L"";

		size_t		iStartPos = 0;
		size_t		iEndPos = 0;

		for (size_t i = 0; i < vecCharInfo.size(); ++i)
		{
			// pUi = ��ũ�� �䰡 �Ǿ� �� 
			// pUiChar = ĳ���� ���ڰ� �Ǿ� �� 
			if (vecCharInfo[i].bGetChar)
			{
				wstrFileName = L"../../Reference/Data/Data_UI/Prefab/Collection_Char/0.uidat";
				wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
					CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)vecCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
					L".png";

				pUi_Char = Load_UI_Prefab(wstrFileName);
				pUi->Get_ChildByIndex(0)->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Child(pUi_Char);

				pUi_Char->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

				iStartPos = wstrTexName.rfind(L"/");
				wstrTexName = wstrTexName.substr(iStartPos + 1);
				iEndPos = wstrTexName.rfind(L".");
				wstrTexName = wstrTexName.substr(0, iEndPos);
				pUi_Char->Get_ChildByIndex(0)->Set_NameTag(wstrTexName);

				pUi_Char->Set_Active(TRUE);
			}
		}
		// ȹ���� ĳ���ͺ��� ������ ���� ��ȹ�� ĳ���� ���� 
		for (size_t i = 0; i < vecCharInfo.size(); ++i)
		{
			if (!vecCharInfo[i].bGetChar)
			{
				wstrFileName = L"../../Reference/Data/Data_UI/Prefab/Collection_Char/None_0.uidat";
				wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
					CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)vecCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
					L".png";

				pUi_Char = Load_UI_Prefab(wstrFileName);
				pUi->Get_ChildByIndex(0)->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Child(pUi_Char);

				pUi_Char->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

				iStartPos = wstrTexName.rfind(L"/");
				wstrTexName = wstrTexName.substr(iStartPos + 1);
				iEndPos = wstrTexName.rfind(L".");
				wstrTexName = wstrTexName.substr(0, iEndPos);
				pUi_Char->Get_ChildByIndex(0)->Set_NameTag(wstrTexName);

				pUi_Char->Set_Active(TRUE);
			}
		}

		CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, pUi->Get_ObjTag(), pUi);

		pUi->Set_NameTag(L"�л���");
		pUi->Set_Active(TRUE);

		// ��� ������ ������ �Ҵ� 
		m_pCollectionPanel = pUi;
		m_pCollectionScroll = pUi->Get_ChildByIndex(0)->Get_ChildByIndex(8)->Get_ChildByIndex(0);
		m_pCollectionSortPanel = pUi->Get_ChildByIndex(0)->Get_ChildByIndex(8)->Get_ChildByIndex(1);

		StudentBook_AddFunction();
	}

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)m_eCharSortType, E_CHAR_ENUMTYPE::CNUM_OTHERS));

	m_pCollectionPanel->Set_Active(TRUE);
	m_pCollectionScroll->Set_Active(TRUE);

	m_pCollectionSortPanel->Set_Active(FALSE);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�л��޴�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"�л���");
}

void CUI_Manager::StudentBook_AddFunction()
{
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_All, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_All, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_Striker, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_Striker, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_Special, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickFilter_Special, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_SortBtnClick, this));
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_ClickSort_Reverse, this));

	m_pCollectionSortPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_OffSortScreen, this));
	m_pCollectionSortPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_OffSortScreen, this));
	m_pCollectionSortPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_SortScreen_Ok, this));

	// �л��� â���� ���� ��ư Ŭ�� 
	for (size_t i = 3; i < m_pCollectionSortPanel->Get_ChildList()->size(); ++i)
	{
		m_pCollectionSortPanel->Get_ChildByIndex((_int)i)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentBook_SelChangeSortType, this, (_int)i), (_int)i);
	}
	// ĳ���� ���� Ŭ�� 
	StudentBook_RebindCharacterInfo();
}

void CUI_Manager::StudentBook_RebindCharacterInfo()
{
	for (size_t i = 0; i < m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		_int	iNameEnum = CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex((_int)i)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo);

		m_pCollectionScroll->Get_ChildByIndex((_int)i)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Create_Ui_CharDetailInfo, this, iNameEnum), iNameEnum);
	}
}

void CUI_Manager::StudentBook_Sort(E_CHAR_SORT eSortType, _bool bIsReverse)
{
	vector<T_CharInfo>	vecCharInfo;
	vecCharInfo.reserve((_uint)E_CHAR_NAME::Name_End);
	vecCharInfo.resize((_uint)E_CHAR_NAME::Name_End);

	if (bIsReverse)
	{
		vecCharInfo = CCharacterMgr::GetInstance()->Get_vecCharSort(E_CHAR_SORT::SORT_REVERSE);
	}
	else
	{
		vecCharInfo = CCharacterMgr::GetInstance()->Get_vecCharSort(eSortType);
	}

	_int	iIndex = 0;

	size_t	iStartPos = 0;
	size_t	iEndPos = 0;

	wstring	wstrTexName = L"";

	for (size_t i = 0; i < vecCharInfo.size(); ++i)
	{
		if (vecCharInfo[i].bGetChar)
		{
			wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
				CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)vecCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
				L".png";

			m_pCollectionScroll->Get_ChildByIndex(iIndex)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

			iStartPos = wstrTexName.rfind(L"/");
			wstrTexName = wstrTexName.substr(iStartPos + 1);
			iEndPos = wstrTexName.rfind(L".");
			wstrTexName = wstrTexName.substr(0, iEndPos);
			m_pCollectionScroll->Get_ChildByIndex(iIndex)->Get_ChildByIndex(0)->Set_NameTag(wstrTexName);

			++iIndex;
		}
	}

	for (size_t i = 0; i < vecCharInfo.size(); ++i)
	{
		if (!vecCharInfo[i].bGetChar)
		{
			wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
				CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)vecCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
				L".png";

			m_pCollectionScroll->Get_ChildByIndex(iIndex)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

			iStartPos = wstrTexName.rfind(L"/");
			wstrTexName = wstrTexName.substr(iStartPos + 1);
			iEndPos = wstrTexName.rfind(L".");
			wstrTexName = wstrTexName.substr(0, iEndPos);
			m_pCollectionScroll->Get_ChildByIndex(iIndex)->Get_ChildByIndex(0)->Set_NameTag(wstrTexName);

			++iIndex;
		}
	}

	switch (m_eCurrentFilterType)
	{
	case Engine::E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER:
		StudentBook_ClickFilter_Striker();
		break;

	case Engine::E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL:
		StudentBook_ClickFilter_Special();
		break;

	case Engine::E_CHAR_COMBATTYPE::COMBATTYPE_END:
		StudentBook_ClickFilter_All();
		break;

	default:
		assert(0);
		break;
	}
}

void CUI_Manager::StudentBook_ClickFilter_All()
{
	m_eCurrentFilterType = E_CHAR_COMBATTYPE::COMBATTYPE_END;

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(TRUE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(FALSE);

	if (m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active())
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(TRUE);
	}

	for (size_t i = 0; i < m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		m_pCollectionScroll->Get_ChildByIndex((_int)i)->Set_Active(TRUE);
	}

	StudentBook_RebindCharacterInfo();
}

void CUI_Manager::StudentBook_ClickFilter_Striker()
{
	m_eCurrentFilterType = E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER;

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(FALSE);

	if (m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_Active())
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(FALSE);
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(TRUE);
	}

	for (size_t i = 0; i < m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		E_CHAR_COMBATTYPE*	eCombatType = (E_CHAR_COMBATTYPE*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(
			(E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex((_int)i)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo),
			E_CHAR_STRUCTALL::CHARINFO_eCombatType);

		if (E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER != *eCombatType)
		{
			m_pCollectionScroll->Get_ChildByIndex((_int)i)->Set_Active(FALSE);
		}
		else
		{
			m_pCollectionScroll->Get_ChildByIndex((_int)i)->Set_Active(TRUE);
		}
	}

	StudentBook_RebindCharacterInfo();
}

void CUI_Manager::StudentBook_ClickFilter_Special()
{
	m_eCurrentFilterType = E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL;

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(TRUE);

	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(FALSE);

	if (m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_Active())
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(TRUE);
	}

	for (size_t i = 0; i < m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		E_CHAR_COMBATTYPE*	eCombatType = (E_CHAR_COMBATTYPE*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(
			(E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex((_int)i)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo),
			E_CHAR_STRUCTALL::CHARINFO_eCombatType);

		if (E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL != *eCombatType)
		{
			m_pCollectionScroll->Get_ChildByIndex((_int)i)->Set_Active(FALSE);
		}
		else
		{
			m_pCollectionScroll->Get_ChildByIndex((_int)i)->Set_Active(TRUE);
		}
	}

	StudentBook_RebindCharacterInfo();
}

void CUI_Manager::StudentBook_SortBtnClick()
{
	if (!m_pCollectionSortPanel)
		return;

	m_pCollectionSortPanel->Set_Active(TRUE);

	for (size_t i = 3; i < m_pCollectionSortPanel->Get_ChildList()->size(); ++i)
	{
		m_pCollectionSortPanel->Get_ChildByIndex((_int)i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);
	}

	m_pCollectionSortPanel->Get_ChildByIndex(3 + (_int)m_eCharSortType)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
}

void CUI_Manager::StudentBook_OffSortScreen()
{
	if (!m_pCollectionSortPanel)
		return;

	m_pCollectionSortPanel->Set_Active(FALSE);
}

void CUI_Manager::StudentBook_SelChangeSortType(_int iSortType)
{
	for (size_t i = 3; i < m_pCollectionSortPanel->Get_ChildList()->size(); ++i)
	{
		m_pCollectionSortPanel->Get_ChildByIndex((_int)i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);
	}

	m_pCollectionSortPanel->Get_ChildByIndex((_int)iSortType)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_eCharSortType = E_CHAR_SORT(iSortType - 3);
}

void CUI_Manager::StudentBook_SortScreen_Ok()
{
	if (E_CHAR_SORT::SORT_STAR == m_eCharSortType)
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Active(TRUE);
	}
	else
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Active(FALSE);
	}
	m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)m_eCharSortType, E_CHAR_ENUMTYPE::CNUM_OTHERS));

	StudentBook_Sort(m_eCharSortType);
	StudentBook_OffSortScreen();
	StudentBook_RebindCharacterInfo();
}

void CUI_Manager::StudentBook_ClickSort_Reverse()
{
	StudentBook_Sort(m_eCharSortType, TRUE); 

	m_bIsReverseSort = !m_bIsReverseSort; 

	if (m_bIsReverseSort)
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Active(FALSE); 
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Active(TRUE); 
	}
	else 
	{
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Active(TRUE);
		m_pCollectionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Active(FALSE);
	}
}


#pragma endregion

#pragma region �л� ������

// �л��ο��� Ŭ������ �� ĳ���� �� ���� â 
void CUI_Manager::Create_Ui_CharDetailInfo(_int iNameEnum)
{
	if (m_pCollectionSortPanel->Get_Active())
	{
		return;
	}

	m_eCurrentCharDetail = (E_CHAR_NAME)iNameEnum;

	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	if (m_pCollectionPanel)
	{
		m_pCollectionPanel->Set_Active(FALSE);
	}

	if (!m_pCharDetail)
	{
		CUI_Object* pUi = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Student_Detail_CharUV.uidat");

		CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, pUi->Get_ObjTag(), pUi);
		pUi->Set_NameTag(L"�л� ������");

		m_pCharDetail = pUi;
		m_pCharDetail_LeftLow = pUi->Get_ChildByIndex(61);
		m_pCharDetail_RightMid = pUi->Get_ChildByIndex(62);
		m_pCharDetail_DetailStat = pUi->Get_ChildByIndex(63);
		m_pCharDetail_DetailStat_Highbar = pUi->Get_ChildByIndex(64);
		m_pCharDetail_DetailSkill = pUi->Get_ChildByIndex(65);
		m_pCharDetail_DetailEquip = pUi->Get_ChildByIndex(66);

		if (!m_pCharDetail || !m_pCharDetail_DetailStat || !m_pCharDetail_LeftLow || !m_pCharDetail_RightMid ||
			!m_pCharDetail_DetailSkill || !m_pCharDetail_DetailEquip || !m_pCharDetail_DetailStat_Highbar)
		{
			assert(0);
		}

		StudentDetail_AddFunction();
	}

	StudentDetail_Update_CharDetailInfo((E_CHAR_NAME)iNameEnum);
	StudentDetail_Update_Variable();

	m_pCharDetail->Set_Active(TRUE);
	m_pCharDetail->Get_ChildByIndex(67)->Set_Active(TRUE);
	m_pCharDetail->Get_ChildByIndex(68)->Set_Active(TRUE);

	m_iCharExperienceAfterLvUp = *(_uint*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iExperience);
}


void CUI_Manager::StudentDetail_AddFunction()
{
	// ���� ������ â(������ 5)
	m_pCharDetail_DetailStat_Highbar->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_OnOffDetailStat, this));

	//6->0->1->8 (�� ���� ��ư)
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(8)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_OnOffDetailStat, this));
	m_pCharDetail_RightMid->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickGeneralStat, this));
	m_pCharDetail_RightMid->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickGeneralStat, this));
	m_pCharDetail_RightMid->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickLevelUpStat, this));
	m_pCharDetail_RightMid->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickLevelUpStat, this));

	//6->1->4 (��ų ���� ��ư)
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_OnOffDetailSkill, this));
	//6->2->4 (��� ���� ��ư)
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_OnOffEquipLevelUp, this));
	//7->1->0 ~ 3->0 (ĳ���� ������ ���������_���ð����� ����)
	for (_uint i = 0; i < 4; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_Update_SimpleSkillInfo, this));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickCharLevelUpItem, this, i), i);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_SelChangeSkill, this, i), i);
		// Right->7->1(����)->0 ~ 3(�����ʿ������Back)->2(ȸ����ư)
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(2)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickCharLevelUpItemBack, this, i), i);
	}

	m_pCharDetail->Get_ChildByIndex(67)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickPreCharInfo, this));
	m_pCharDetail->Get_ChildByIndex(68)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickNextCharInfo, this));

	//7->3(�ڵ����� ��ư �θ�)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickCharLevelUpAutoSel, this));
	//7->4(������ ��ư �θ�)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::StudentDetail_ClickLevelUp_Char, this));
}

void CUI_Manager::StudentDetail_Update_CharDetailInfo(E_CHAR_NAME eName)
{
	// ������ ĳ������ ������ �°� ������ ���� 
	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	switch (tCharInfo.eSchoolType)
	{
	case E_CHAR_SCHOOL::SCHOOL_ABYDOS:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Abydos_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_GEHENNA:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Gehenna_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Hyakkiyako_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Millennium_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_TRINITY:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Trinity_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Sanhaijing_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_REDWINTER:
		m_pCharDetail->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_Redwinter_Collection.png");
		break;

	case E_CHAR_SCHOOL::SCHOOL_END:
		break;

	case E_CHAR_SCHOOL::SCHOOL_NULL:
		break;

	default:
		assert(0);
		break;
	}

	// ĳ���� �̹��� ���� �ڵ� ����ٰ� ���� 
	for (_uint i = 0; i < (_uint)m_pCharDetail->Get_ChildList()->size(); ++i)
	{
		if (0 != m_pLastClick_Ui->Get_ChildList()->size())
		{
			if (m_pLastClick_Ui->Get_ChildByIndex(0)->Get_Name() + L"_�̹���" == m_pCharDetail->Get_ChildByIndex((_int)i)->Get_Name())
			{
				m_pCharDetail->Get_ChildByIndex((_int)i)->Set_Active(TRUE);
			}
			else
			{
				m_pCharDetail->Get_ChildByIndex(i)->Set_Active(FALSE);
			}
		}
		else
		{
			if (CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)eName, E_CHAR_ENUMTYPE::CNUM_NAME) + L"_�̹���" == m_pCharDetail->Get_ChildByIndex((_int)i)->Get_Name())
			{
				m_pCharDetail->Get_ChildByIndex((_int)i)->Set_Active(TRUE);
			}
			else
			{
				m_pCharDetail->Get_ChildByIndex(i)->Set_Active(FALSE);
			}
		}
	}

	m_pCharDetail_LeftLow->Get_ChildByIndex(0)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_KORNAME));
	// �� ���� ���� �ڵ� ����ٰ� ���� 
	m_pCharDetail_LeftLow->Get_ChildByIndex(1)->Set_Active(FALSE);
	m_pCharDetail_LeftLow->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pCharDetail_LeftLow->Get_ChildByIndex(3)->Set_Active(FALSE);
	for (_uint i = 0; i < (_uint)tCharInfo.eStar; ++i)
	{
		m_pCharDetail_LeftLow->Get_ChildByIndex(i + 1)->Set_Active(TRUE);
	}
	m_pCharDetail_LeftLow->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eSchoolType, E_CHAR_ENUMTYPE::CNUM_SCHOOL));

	switch (tCharInfo.eCombatType)
	{
	case E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER:
		m_pCharDetail_LeftLow->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Active(TRUE);
		m_pCharDetail_LeftLow->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(FALSE);
		break;

	case E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL:
		m_pCharDetail_LeftLow->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Active(FALSE);
		m_pCharDetail_LeftLow->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(TRUE);
		break;

	case E_CHAR_COMBATTYPE::COMBATTYPE_NULL:
		break;

	default:
		assert(0);
		break;
	}
	// ���ϴ� ĳ���� ���� 
	m_pCharDetail_LeftLow->Get_ChildByIndex(5)->Set_Ui_Text(L"Lv." + to_wstring(tCharInfo.iLevel));

	// Left->7(����ġ��_Back)->0(����ġ ���α׷�����), 1(����ġ Text)
	// Right->7->5(����ġ��_Back)->0(����ġ�� ��·�), 1(����ġ ���α׷�����), 2(����ġ Text)
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)tCharInfo.iExperience / tCharInfo.iMaxExperience, 1.f));
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.iExperience) + L"/" + to_wstring(tCharInfo.iMaxExperience));

	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)m_iCharExperienceAfterLvUp / tCharInfo.iMaxExperience, 1.f));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_ResetMask(FALSE); 
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)tCharInfo.iExperience / tCharInfo.iMaxExperience, 1.f));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExperience) + L"/" + to_wstring(tCharInfo.iMaxExperience));

	m_pCharDetail_LeftLow->Set_Active(TRUE);
	m_pCharDetail_RightMid->Set_Active(TRUE);
	m_pCharDetail_DetailStat->Set_Active(FALSE);
	m_pCharDetail_DetailSkill->Set_Active(FALSE);

	StudentDetail_ClickGeneralStat();

	m_pCharDetail_LeftLow->Get_ChildByIndex(6)->Set_Ui_Text(L"RANK" + to_wstring(1));

	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(4)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iMaxHp));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(5)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iDeffence));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(6)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iAttack));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(7)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHeal));
	//6->0->2->0(����), 1(������), 3(����_����Ÿ��), 5(����_���Ÿ��)
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eRole, E_CHAR_ENUMTYPE::CNUM_ROLE));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.ePosition, E_CHAR_ENUMTYPE::CNUM_POSITION));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(3)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eAtkType, E_CHAR_ENUMTYPE::CNUM_ATKTYPE));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(5)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eDffType, E_CHAR_ENUMTYPE::CNUM_DFFTYPE));
	// �ʿ� ���� ����/����� �ؽ��� �ڵ� ���� ����ٰ�
	//6->0->2->6 ~ 8(�ʹ���)->1(���� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_" + CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eInCityBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF) + L".png");
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_" + CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eOutDoorBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF) + L".png");
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(8)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_" + CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eInDoorBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF) + L".png");
	// ��ų ������ ���� �ؽ��� �ڵ� ���� 
	//6->1->0 ~ 3->1->0
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrExSkillImageFile);
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrNormalSkillImageFile);
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrReinforceSkillImageFile);
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrSubSkillImageFile);
	// ��ų ���� �ϴ� 1�� ���� 
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex]));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Normal]));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Reinforce]));
	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Sub]));

	//9(��ų����â)->0(�̸�), 1(����), 3(�ڽ�Ʈ), 4(����), 5(����)
	//10 ~ 13(ȭ��ǥ)

	// ��� ���� ����ٰ� ���� 
	//6,2,0,0
	if (tCharInfo.bHide)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
	}
	else
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);
	}

	switch (tCharInfo.eWeaponType)
	{
	case E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"AR");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_AR.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"GL");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_GL.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_HG:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"HG");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_HG.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"MG");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_MG.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_MT:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"MT");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_MT.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"SG");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_SG.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"SMG");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_SMG.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"SR");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_SR.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_RL:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"RL");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_RL.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_RG:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"RG");
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Weapon_Icon_RG.png");
		break;

	case E_CHAR_WEAPONTYPE::WEAPONTYPE_NULL:
		m_pCharDetail_RightMid->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"��������");
		break;

	default:
		assert(0);
		break;
	}

	// �� ���� ���� 
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iMaxHp));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iAttack));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iDeffence));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHeal));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHitRatio));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iEvasion));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iCritical));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iCriticalDamage));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iStability));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iRange));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iCrowdReinforce));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iCrowdResist));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.iCostHeal));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(3)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eInCityBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eOutDoorBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF));
	m_pCharDetail_DetailStat->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(3)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eInDoorBuff, E_CHAR_ENUMTYPE::CNUM_MAPBUFF));

	// ������ �� ���� 
	//7->0->1->1(LV), 3(ü��), 5(����), 7(���ݷ�), 9(ġ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.iLevel));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iMaxHp));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(5)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iDeffence));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(7)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iAttack));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(9)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHeal));

	//7->1->0 ~ 3->0->0->0(������ġ������ �̹��� �� ���� ����)
	//7->1->0->1(������ġ������ ���� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������)->iCount));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������)->iCount));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������)->iCount));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������)->iCount));

	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Credit()));
}

void CUI_Manager::StudentDetail_Update_Variable()
{
	m_iCurrentCredit = CGameManager::GetInstance()->Get_Credit();

	// Enum���� ���� �� �־ �ϳ��� ����..
	m_iCurrentReinforceItem[0] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������)->iCount;
	m_iCurrentReinforceItem[1] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������)->iCount;
	m_iCurrentReinforceItem[2] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������)->iCount;
	m_iCurrentReinforceItem[3] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������)->iCount;

	for (_uint i = 0; i < 4; ++i)
	{
		m_iLvUpItemInsertCount[i] = 0;
	}
}

void CUI_Manager::StudentDetail_Update_SimpleSkillInfo()
{
	if (!m_pLastClick_Ui)
		return;

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	//9(��ų����â)->0(�̸�), 1(����), 3(�ڽ�Ʈ), 4(����), 5(����)
	//10 ~ 13(ȭ��ǥ)
	if (m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_Active())
	{
		//����â
		m_pCharDetail_RightMid->Get_ChildByIndex(9)->Set_Active(FALSE);
		//ȭ��ǥ
		m_pCharDetail_RightMid->Get_ChildByIndex(10)->Set_Active(FALSE);
		m_pCharDetail_RightMid->Get_ChildByIndex(11)->Set_Active(FALSE);
		m_pCharDetail_RightMid->Get_ChildByIndex(12)->Set_Active(FALSE);
		m_pCharDetail_RightMid->Get_ChildByIndex(13)->Set_Active(FALSE);
	}
	else
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(9)->Set_Active(TRUE);
		wstring		wstrDesc = L""; 

		if (L"EX��ų_Back" == m_pLastClick_Ui->Get_Name())
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(0)->Set_Ui_Text(tCharInfo.wstrExSkillName);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(1)->Set_Ui_Text(L"Ex ��ų");
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(2)->Set_Active(TRUE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.iExSkillCost));
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(4)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])); 
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(5)->Set_Ui_Text(tCharInfo.wstrExSkillDesc.replace(tCharInfo.wstrExSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fExSkill).substr(0, to_wstring(tCharInfo.fExSkill).find(L".") + 3)));

			m_pCharDetail_RightMid->Get_ChildByIndex(10)->Set_Active(TRUE);
			m_pCharDetail_RightMid->Get_ChildByIndex(11)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(12)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(13)->Set_Active(FALSE);
		}
		else if (L"�⺻��ų_Back" == m_pLastClick_Ui->Get_Name())
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(0)->Set_Ui_Text(tCharInfo.wstrNormalSkillName);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(1)->Set_Ui_Text(L"Normal ��ų");
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(2)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(3)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(4)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Normal]));
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(5)->Set_Ui_Text(tCharInfo.wstrNormalSkillDesc.replace(tCharInfo.wstrNormalSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fNormalSkill).substr(0, to_wstring(tCharInfo.fNormalSkill).find(L".") + 3)));

			m_pCharDetail_RightMid->Get_ChildByIndex(10)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(11)->Set_Active(TRUE);
			m_pCharDetail_RightMid->Get_ChildByIndex(12)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(13)->Set_Active(FALSE);
		}
		else if (L"��ȭ��ų_Back" == m_pLastClick_Ui->Get_Name())
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(0)->Set_Ui_Text(tCharInfo.wstrReinforceSkillName);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(1)->Set_Ui_Text(L"Reinforce ��ų");
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(2)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(3)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(4)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Reinforce]));
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(5)->Set_Ui_Text(tCharInfo.wstrReinforceSkillDesc.replace(tCharInfo.wstrReinforceSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fReinforceSkill).substr(0, to_wstring(tCharInfo.fReinforceSkill).find(L".") + 3)));

			m_pCharDetail_RightMid->Get_ChildByIndex(10)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(11)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(12)->Set_Active(TRUE);
			m_pCharDetail_RightMid->Get_ChildByIndex(13)->Set_Active(FALSE);
		}
		else if (L"���꽺ų_Back" == m_pLastClick_Ui->Get_Name())
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(0)->Set_Ui_Text(tCharInfo.wstrSubSkillName);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(1)->Set_Ui_Text(L"Sub ��ų");
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(2)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(3)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(4)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Sub]));
			m_pCharDetail_RightMid->Get_ChildByIndex(9)->Get_ChildByIndex(5)->Set_Ui_Text(tCharInfo.wstrSubSkillDesc.replace(tCharInfo.wstrSubSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fSubSkill).substr(0, to_wstring(tCharInfo.fSubSkill).find(L".") + 3)));

			m_pCharDetail_RightMid->Get_ChildByIndex(10)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(11)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(12)->Set_Active(FALSE);
			m_pCharDetail_RightMid->Get_ChildByIndex(13)->Set_Active(TRUE);
		}
	}
}

void CUI_Manager::StudentDetail_OnOffDetailStat()
{
	if (m_pCharDetail_DetailStat->Get_Active())
	{
		//m_pCharDetail_DetailStat_Highbar->Set_Active(FALSE);
		m_pCharDetail_DetailStat->Set_Active(FALSE);
	}
	else
	{
		//m_pCharDetail_DetailStat_Highbar->Set_Active(TRUE);
		m_pCharDetail_DetailStat->Set_Active(TRUE);
	}
}

void CUI_Manager::StudentDetail_OnOffDetailSkill()
{
	if (m_pCharDetail_DetailSkill->Get_Active())
	{
		m_pCharDetail_DetailSkill->Set_Active(FALSE);
	}
	else
	{
		m_pCharDetail_DetailSkill->Set_Active(TRUE);
	}

	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	m_eSelSkillUpgrade = E_SkillType::Skill_Ex;

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	//0->Left��ų����_Back	1->Pic_���缱�ý�ų	 2->Text_���缱�ý�ų		3->Text_���缱�ý�ų����	
	//4->���缱�ý�ų����_Back	5->�������Ľ�ų����_Back		6->Arrow	8->���� ���� ũ���� ���� 
	//0->0 ~ 3(��ų����)->1(Pic_��ų�̹���), 2(Text_��ųLV)
	//4->0(LV), 2(Cost)
	//5->��

	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_NameTag(L"Ex");
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_NameTag(L"Normal");
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_NameTag(L"Reinforce");
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_NameTag(L"Sub");

	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrExSkillImageFile);
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrNormalSkillImageFile);
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrReinforceSkillImageFile);
	m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrSubSkillImageFile);

	// Skill ���� �ϴ� 1�� ����
	for (_uint i = 0; i < (_uint)E_SkillType::SkillType_End; ++i)
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[i]));
	}

	// ó������ ������ Ex��ų�� ���� �Ǿ����� 
	m_pCharDetail_DetailSkill->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrExSkillImageFile);
	m_pCharDetail_DetailSkill->Get_ChildByIndex(2)->Set_Ui_Text(tCharInfo.wstrExSkillName);
	m_pCharDetail_DetailSkill->Get_ChildByIndex(3)->Set_Ui_Text(L"EX ��ų"); 

	// ���� ��ų ����
	wstring		wstrOriginDesc = L""; 

	switch (m_eSelSkillUpgrade)
	{
	case Engine::E_SkillType::Skill_Ex:
		wstrOriginDesc = tCharInfo.wstrExSkillDesc; 
		tCharInfo.wstrExSkillDesc.replace(tCharInfo.wstrExSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fExSkill).substr(0, to_wstring(tCharInfo.fExSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrExSkillDesc, L" ", 20)); 

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fExSkill + tCharInfo.fExSkill_Increase).substr(0, to_wstring(tCharInfo.fExSkill + tCharInfo.fExSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20)); 
		}

		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExSkillCost)); 
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExSkillCost)); 
		break;

	case Engine::E_SkillType::Skill_Normal:
		wstrOriginDesc = tCharInfo.wstrNormalSkillDesc;
		tCharInfo.wstrNormalSkillDesc.replace(tCharInfo.wstrNormalSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fNormalSkill).substr(0, to_wstring(tCharInfo.fNormalSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrNormalSkillDesc, L" ", 20)); 

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fNormalSkill + tCharInfo.fNormalSkill_Increase).substr(0, to_wstring(tCharInfo.fNormalSkill + tCharInfo.fNormalSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20)); 
		}
		break;

	case Engine::E_SkillType::Skill_Reinforce:
		wstrOriginDesc = tCharInfo.wstrReinforceSkillDesc;
		tCharInfo.wstrReinforceSkillDesc.replace(tCharInfo.wstrReinforceSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fReinforceSkill).substr(0, to_wstring(tCharInfo.fReinforceSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrReinforceSkillDesc, L" ", 20)); 

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fReinforceSkill + tCharInfo.fReinforceSkill_Increase).substr(0, to_wstring(tCharInfo.fReinforceSkill + tCharInfo.fReinforceSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}
		break;

	case Engine::E_SkillType::Skill_Sub:
		wstrOriginDesc = tCharInfo.wstrSubSkillDesc;
		tCharInfo.wstrSubSkillDesc.replace(tCharInfo.wstrSubSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fSubSkill).substr(0, to_wstring(tCharInfo.fSubSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrSubSkillDesc, L" ", 20)); 

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fSubSkill + tCharInfo.fSubSkill_Increase).substr(0, to_wstring(tCharInfo.fSubSkill + tCharInfo.fSubSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}
		break;

	default:
		assert(0); 
		break;
	}

	if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 == (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(L"�ִ� ������ �����߽��ϴ�.");
	}

	m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade]));

	if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex] + 1));
	}

	m_pCharDetail_DetailSkill->Get_ChildByIndex(8)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Credit()));

	// ó������ ������ Ex��ų ���� ������ ��� 
	// ��ų������â->7->0 ~ 3(�����۵�)->0, 1(�����۰���)->0->0(������ �̹���)
	// 
	// ĳ���� Enum���� 10���� ���� ���� ������(0 ~ 9) * 4 + 124 + ��ų ����
	switch (tCharInfo.eSchoolType)
	{
	case E_CHAR_SCHOOL::SCHOOL_ABYDOS:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_GEHENNA:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_REDWINTER:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	case E_CHAR_SCHOOL::SCHOOL_TRINITY:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->wstrImagePath);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
		break;

	default:
		assert(0);
		break;
	}

	for (_uint i = 1; i < (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex] - 1; ++i)
	{
		//if ((_uint)������_�׺��_��ũ < (i-1) * 4)
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);

		_uint iIndex = ((i - 1) * 4) + ((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG] % 10) * 4 + (_uint)E_ItemTag::����_�ϴϿ�_���� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex];
		if (iIndex <= (_uint)E_ItemTag::������_�׺��_��ũ)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(iIndex)->wstrImagePath);
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(iIndex - 39)->wstrImagePath);
		}
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item(((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG] % 10) * 4 + (_uint)E_ItemTag::����_�ϴϿ�_���� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
	}
	for (_uint i = (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex] - 1; i < (_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 2; ++i)
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ImageAlpha(0.f);
	}

	//7->0->1->10 ~ 14 (ȭ��ǥ)
	//7->0->1->15 ~ 19 (�������� ����)
	//7->1->0 ~ 3->0->0->0(������ġ������ �̹��� �� ���� ����)
	//7->1->0->1(������ġ������ ���� ����)
}

void CUI_Manager::StudentDetail_OnOffEquipLevelUp()
{
	if (m_pCharDetail_DetailEquip->Get_Active())
	{
		m_pCharDetail_DetailEquip->Set_Active(FALSE);
	}
	else
	{
		m_pCharDetail_DetailEquip->Set_Active(TRUE);
	}
}

void CUI_Manager::StudentDetail_ClickGeneralStat()
{
	m_pCharDetail_RightMid->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(1)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(3)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(4)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(5)->Set_Active(TRUE);

	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Set_Active(FALSE);
}

void CUI_Manager::StudentDetail_ClickLevelUpStat()
{
	m_pCharDetail_RightMid->Get_ChildByIndex(0)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(1)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(2)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(4)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(5)->Set_Active(TRUE);

	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Set_Active(TRUE);

	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(TRUE);

	//7->1->0->1(������ġ������ ���� ����)
	for (_uint i = 0; i < 4; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iLvUpItemInsertCount[i]));
	}
	//7->2(ũ���� ��� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_iCurrentCredit));
}

void CUI_Manager::StudentDetail_ClickLiberationStat()
{
	m_pCharDetail_RightMid->Get_ChildByIndex(0)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(1)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(3)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(4)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(5)->Set_Active(FALSE);

	m_pCharDetail_RightMid->Get_ChildByIndex(6)->Set_Active(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Set_Active(FALSE);
}

void CUI_Manager::StudentDetail_ClickCharLevelUpItem(_uint iIndex)
{
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	//7->1->0 ~ 3->0 (ĳ���� ������ ��������� ���ð����� ����)
	if (m_iCharLevelAfterLvUp < CGameManager::GetInstance()->Get_Level())
	{
		switch (iIndex)
		{
		case 0:
			if (0 < m_iCurrentReinforceItem[0] && ItemPrice_Tier1 <= m_iCurrentCredit)
			{
				m_iCurrentCredit -= ItemPrice_Tier1;
				--m_iCurrentReinforceItem[0];
				++m_iLvUpItemInsertCount[iIndex];
				m_iCharExperienceAfterLvUp += CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������)->iItemEffect;
			}
			break;

		case 1:
			if (0 < m_iCurrentReinforceItem[1] && ItemPrice_Tier2 <= m_iCurrentCredit)
			{
				m_iCurrentCredit -= ItemPrice_Tier2;
				--m_iCurrentReinforceItem[1];
				++m_iLvUpItemInsertCount[iIndex];
				m_iCharExperienceAfterLvUp += CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������)->iItemEffect;
			}
			break;

		case 2:
			if (0 < m_iCurrentReinforceItem[2] && ItemPrice_Tier3 <= m_iCurrentCredit)
			{
				m_iCurrentCredit -= ItemPrice_Tier3;
				--m_iCurrentReinforceItem[2];
				++m_iLvUpItemInsertCount[iIndex];
				m_iCharExperienceAfterLvUp += CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������)->iItemEffect;
			}
			break;

		case 3:
			if (0 < m_iCurrentReinforceItem[3] && ItemPrice_Tier4 <= m_iCurrentCredit)
			{
				m_iCurrentCredit -= ItemPrice_Tier4;
				--m_iCurrentReinforceItem[3];
				++m_iLvUpItemInsertCount[iIndex];
				m_iCharExperienceAfterLvUp += CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������)->iItemEffect;
			}
			break;

		default:
			assert(0);
			break;
		}
	}

	Check_LevelUp(&m_iCharExperienceAfterLvUp);

	//7->1->0 ~ 3->0->0->0(����ġ������ �̹��� �� ���� ����)
	for (_uint i = 0; i < 4; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iCurrentReinforceItem[i]));
	}
	//7->1->0->1(����ġ������ ���� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(iIndex)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iLvUpItemInsertCount[iIndex]));
	//7->2(ũ���� ��� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_iCurrentCredit));

	// Right->7->5(����ġ��_Back)->0(����ġ�� ��·�), 1(����ġ ���α׷�����), 2(����ġ Text)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)m_iCharExperienceAfterLvUp / (m_iCharLevelAfterLvUp * CharExpInc), 1.f));

	// Right->7->3(�ڵ�����_Back)->0(��Ȱ��ȭ), 1(Ȱ��ȭ)
	// Right->7->4(������_Back)->0(��Ȱ��ȭ), 1(Ȱ��ȭ)
	if (0 < m_iLvUpItemInsertCount[0] ||
		0 < m_iLvUpItemInsertCount[1] ||
		0 < m_iLvUpItemInsertCount[2] ||
		0 < m_iLvUpItemInsertCount[3])
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Active(FALSE);
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Active(TRUE);
	}

	Check_StatAfterLvUp();
}

void CUI_Manager::StudentDetail_ClickCharLevelUpAutoSel()
{
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	//7->3(�ڵ����� ��ư �θ�)
	T_ItemInfo*	tTier4 = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������);
	T_ItemInfo*	tTier3 = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������);
	T_ItemInfo*	tTier2 = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������);
	T_ItemInfo*	tTier1 = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������);

	_uint	iCharLv = *(_uint*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iLevel);
	_int	iNeedExp = 0;

	for (_uint i = iCharLv; i <= CGameManager::GetInstance()->Get_Level(); ++i)
	{
		iNeedExp += i * CharExpInc;
	}

	iNeedExp -= *(_uint*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iExperience);

	while (m_iCharLevelAfterLvUp <= CGameManager::GetInstance()->Get_Level() ||
		(0 >= m_iCurrentReinforceItem[0] &&
			0 >= m_iCurrentReinforceItem[1] &&
			0 >= m_iCurrentReinforceItem[2] &&
			0 >= m_iCurrentReinforceItem[3]))
	{
		if (tTier4->iItemEffect <= iNeedExp &&
			0 < m_iCurrentReinforceItem[3] &&
			ItemPrice_Tier4 <= m_iCurrentCredit)
		{
			StudentDetail_ClickCharLevelUpItem(3);

			iNeedExp -= tTier4->iItemEffect;
		}
		else if (tTier3->iItemEffect <= iNeedExp &&
			0 < m_iCurrentReinforceItem[2] &&
			ItemPrice_Tier3 <= m_iCurrentCredit)
		{
			StudentDetail_ClickCharLevelUpItem(2);

			iNeedExp -= tTier3->iItemEffect;
		}
		else if (tTier2->iItemEffect <= iNeedExp &&
			0 < m_iCurrentReinforceItem[1] &&
			ItemPrice_Tier2 <= m_iCurrentCredit)
		{
			StudentDetail_ClickCharLevelUpItem(1);

			iNeedExp -= tTier2->iItemEffect;
		}
		else if (tTier1->iItemEffect <= iNeedExp &&
			0 < m_iCurrentReinforceItem[0] &&
			ItemPrice_Tier1 <= m_iCurrentCredit)
		{
			StudentDetail_ClickCharLevelUpItem(0);

			iNeedExp -= tTier1->iItemEffect;
		}
		else
		{
			break;
		}
	}
}

void CUI_Manager::StudentDetail_ClickCharLevelUpItemBack(_uint iIndex)
{
	// Right->7->1(����)->0 ~ 3(�����ʿ������Back)->2(ȸ����ư)
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	if (0 < m_iLvUpItemInsertCount[iIndex])
	{
		--m_iLvUpItemInsertCount[iIndex];
		++m_iCurrentReinforceItem[iIndex];

		m_iCurrentCredit = CGameManager::GetInstance()->Get_Credit();

		m_iCurrentCredit -= m_iLvUpItemInsertCount[0] * ItemPrice_Tier1;
		m_iCurrentCredit -= m_iLvUpItemInsertCount[1] * ItemPrice_Tier2;
		m_iCurrentCredit -= m_iLvUpItemInsertCount[2] * ItemPrice_Tier3;
		m_iCurrentCredit -= m_iLvUpItemInsertCount[3] * ItemPrice_Tier4;

		Check_LevelDown(iIndex);
	}

	//7->1->0 ~ 3->0->0->0(������ġ������ �̹��� �� ���� ����)
	for (_uint i = 0; i < 4; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iCurrentReinforceItem[i]));
	}
	//7->1->0->1(������ġ������ ���� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(iIndex)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iLvUpItemInsertCount[iIndex]));
	//7->2(ũ���� ��� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_iCurrentCredit));

	// Right->7->5(����ġ��_Back)->0(����ġ�� ��·�), 1(����ġ ���α׷�����), 2(����ġ Text)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)m_iCharExperienceAfterLvUp / (m_iCharLevelAfterLvUp * CharExpInc), 1.f));

	// Right->7->3(�ڵ�����_Back)->0(��Ȱ��ȭ), 1(Ȱ��ȭ)
	// Right->7->4(������_Back)->0(��Ȱ��ȭ), 1(Ȱ��ȭ)
	if (0 == m_iLvUpItemInsertCount[0] &&
		0 == m_iLvUpItemInsertCount[1] &&
		0 == m_iLvUpItemInsertCount[2] &&
		0 == m_iLvUpItemInsertCount[3])
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Active(TRUE);
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Active(FALSE);
	}

	Check_StatAfterLvUp();
}

void CUI_Manager::StudentDetail_ClickLevelUp_Char()
{
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	CCharacterMgr::GetInstance()->Set_CharInfo(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iLevel, &m_iCharLevelAfterLvUp);
	CCharacterMgr::GetInstance()->Set_CharInfo(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iExperience, &m_iCharExperienceAfterLvUp);
	// Max ����ġ�� Level �������� �� �������ֱ�
	CGameManager::GetInstance()->Set_Credit(m_iCurrentCredit);
	CGameManager::GetInstance()->Set_ItemCount((_int)E_ItemTag::����_Ȱ������, m_iCurrentReinforceItem[0]);
	CGameManager::GetInstance()->Set_ItemCount((_int)E_ItemTag::�Ϲ�_Ȱ������, m_iCurrentReinforceItem[1]);
	CGameManager::GetInstance()->Set_ItemCount((_int)E_ItemTag::���_Ȱ������, m_iCurrentReinforceItem[2]);
	CGameManager::GetInstance()->Set_ItemCount((_int)E_ItemTag::�ֻ��_Ȱ������, m_iCurrentReinforceItem[3]);

	// ������ �� ���� 
	//7->0->1->1(LV), 3(ü��), 5(����), 7(���ݷ�), 9(ġ����)
	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	// ���ϴ� 
	m_pCharDetail_LeftLow->Get_ChildByIndex(5)->Set_Ui_Text(L"Lv." + to_wstring(tCharInfo.iLevel));
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_AfterPass(TRUE);
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)tCharInfo.iExperience / tCharInfo.iMaxExperience, 1.f));
	m_pCharDetail_LeftLow->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.iExperience) + L"/" + to_wstring(tCharInfo.iMaxExperience));

	// ���ߴ� 
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Active(TRUE);
	//m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2(_float(m_iCharExperienceAfterLvUp / tCharInfo.iMaxExperience), 1.f));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_ResetMask(FALSE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_Mask(_vec2(0.f, 0.f), _vec2((_float)tCharInfo.iExperience / tCharInfo.iMaxExperience, 1.f));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Set_AfterPass(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExperience) + L"/" + to_wstring(tCharInfo.iMaxExperience));

	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tCharInfo.iLevel));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iMaxHp));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(5)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iDeffence));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(7)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iAttack));
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(9)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHeal));

	Initialize_StudentDetail();

	//7->1->0 ~ 3->0->0->0(����ġ������ �̹��� �� ���� ����)
	for (_uint i = 0; i < 4; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iCurrentReinforceItem[i]));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iLvUpItemInsertCount[i]));
	}

	//7->2(ũ���� ��� ����)
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_iCurrentCredit));

	// ������ ��ư Ȱ��/��Ȱ��ȭ
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Set_Active(FALSE);
}

void CUI_Manager::StudentDetail_SelChangeSkill(_uint iSkillTypeIndex)
{
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	m_eSelSkillUpgrade = (E_SkillType)iSkillTypeIndex;

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	// ���� ��ų ����
	m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade]));

	// ���� ��ų ����
	if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade] + 1)
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Set_Ui_Text(L"Lv." + to_wstring((_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade] + 1));
	}

	wstring		wstrOriginDesc = L"";

	switch (m_eSelSkillUpgrade)
	{
	case Engine::E_SkillType::Skill_Ex:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrExSkillImageFile);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(2)->Set_Ui_Text(tCharInfo.wstrExSkillName);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(3)->Set_Ui_Text(L"EX ��ų");
		wstrOriginDesc = tCharInfo.wstrExSkillDesc;
		tCharInfo.wstrExSkillDesc.replace(tCharInfo.wstrExSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fExSkill).substr(0, to_wstring(tCharInfo.fExSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrExSkillDesc, L" ", 20));

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fExSkill + tCharInfo.fExSkill_Increase).substr(0, to_wstring(tCharInfo.fExSkill + tCharInfo.fExSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}

		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExSkillCost));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tCharInfo.iExSkillCost));
		break;

	case Engine::E_SkillType::Skill_Normal:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrNormalSkillImageFile);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(2)->Set_Ui_Text(tCharInfo.wstrNormalSkillName);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(3)->Set_Ui_Text(L"�⺻ ��ų");
		wstrOriginDesc = tCharInfo.wstrNormalSkillDesc;
		tCharInfo.wstrNormalSkillDesc.replace(tCharInfo.wstrNormalSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fNormalSkill).substr(0, to_wstring(tCharInfo.fNormalSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrNormalSkillDesc, L" ", 20));

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fNormalSkill + tCharInfo.fNormalSkill_Increase).substr(0, to_wstring(tCharInfo.fNormalSkill + tCharInfo.fNormalSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}

		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		break;

	case Engine::E_SkillType::Skill_Reinforce:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrReinforceSkillImageFile);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(2)->Set_Ui_Text(tCharInfo.wstrReinforceSkillName);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(3)->Set_Ui_Text(L"��ȭ ��ų");
		wstrOriginDesc = tCharInfo.wstrReinforceSkillDesc;
		tCharInfo.wstrReinforceSkillDesc.replace(tCharInfo.wstrReinforceSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fReinforceSkill).substr(0, to_wstring(tCharInfo.fReinforceSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrReinforceSkillDesc, L" ", 20));

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fReinforceSkill + tCharInfo.fReinforceSkill_Increase).substr(0, to_wstring(tCharInfo.fReinforceSkill + tCharInfo.fReinforceSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}

		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		break;

	case Engine::E_SkillType::Skill_Sub:
		m_pCharDetail_DetailSkill->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/" + tCharInfo.wstrSubSkillImageFile);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(2)->Set_Ui_Text(tCharInfo.wstrSubSkillName);
		m_pCharDetail_DetailSkill->Get_ChildByIndex(3)->Set_Ui_Text(L"���� ��ų");
		wstrOriginDesc = tCharInfo.wstrSubSkillDesc;
		tCharInfo.wstrSubSkillDesc.replace(tCharInfo.wstrSubSkillDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fSubSkill).substr(0, to_wstring(tCharInfo.fSubSkill).find(L".") + 3));
		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Set_Ui_Text(CUtility::Auto_Line(tCharInfo.wstrSubSkillDesc, L" ", 20));

		if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 != (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])
		{
			wstrOriginDesc.replace(wstrOriginDesc.find(L"%SkillStat%"), 11, to_wstring(tCharInfo.fSubSkill + tCharInfo.fSubSkill_Increase).substr(0, to_wstring(tCharInfo.fSubSkill + tCharInfo.fSubSkill_Increase).find(L".") + 3));
			m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(CUtility::Auto_Line(wstrOriginDesc, L" ", 20));
		}

		m_pCharDetail_DetailSkill->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Get_ChildByIndex(2)->Set_Ui_Text(L"0");
		break;

	default:
		assert(0);
		break;
	}

	if ((_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 1 == (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade]) 
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(5)->Set_Ui_Text(L"�ִ� ������ �����߽��ϴ�.");
	}

	//��ų������â->7->0 ~ 3(�����۵�)->0, 1(�����۰���)->0->0(������ �̹���)
	switch (tCharInfo.eSchoolType)
	{
	case E_CHAR_SCHOOL::SCHOOL_ABYDOS:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�ƺ񵵽� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_GEHENNA:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_��;��� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�з��Ͼ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_REDWINTER:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_�ܿ￬�� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_���ذ� + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	case E_CHAR_SCHOOL::SCHOOL_TRINITY:
		if (E_SkillType::Skill_Ex == m_eSelSkillUpgrade)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_��������BD_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->wstrImagePath);
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_�����Ʈ_Ʈ����Ƽ + (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade])->iCount));
		}
		break;

	default:
		assert(0);
		break;
	}

	for (_uint i = 1; i < (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade] - 1; ++i)
	{
		//if ((_uint)������_�׺��_��ũ < (i-1) * 4)
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);

		_uint iIndex = ((i - 1) * 4) + ((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG] % 10) * 4 + (_uint)E_ItemTag::����_�ϴϿ�_���� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex];
		if (iIndex <= (_uint)E_ItemTag::������_�׺��_��ũ)
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(iIndex)->wstrImagePath);
		}
		else
		{
			m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(iIndex - 39)->wstrImagePath);
		}
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Item(((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG] % 10) * 4 + (_uint)E_ItemTag::����_�ϴϿ�_���� + (_uint)tCharInfo.eSkillLvArr[(_uint)E_SkillType::Skill_Ex])->iCount));
	}
	for (_uint i = (_uint)tCharInfo.eSkillLvArr[(_uint)m_eSelSkillUpgrade] - 1; i < (_uint)E_CHAR_SKILL_LV::SKILL_LV_END - 2; ++i)
	{
		m_pCharDetail_DetailSkill->Get_ChildByIndex(7)->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ImageAlpha(0.f);
	}
}

void CUI_Manager::StudentDetail_ClickNextCharInfo()
{
	if (!m_pCollectionScroll || m_eCurrentCharDetail == E_CHAR_NAME::Name_End)
		return;

	for (_uint i = 0; i < (_uint)m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		wstring wstrName = m_pCollectionScroll->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_Name();

		if (m_eCurrentCharDetail ==
			(E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo))
		{
			if (i + 1 < m_pCollectionScroll->Get_ChildList()->size())
			{
				Create_Ui_CharDetailInfo(CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex(i + 1)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo));
			}
			else
			{
				Create_Ui_CharDetailInfo(CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo));
			}

			break;
		}
	}

	Initialize_StudentDetail();
}

void CUI_Manager::StudentDetail_ClickPreCharInfo()
{
	if (!m_pCollectionScroll || m_eCurrentCharDetail == E_CHAR_NAME::Name_End)
		return;

	for (_int i = 0; i < (_int)m_pCollectionScroll->Get_ChildList()->size(); ++i)
	{
		wstring wstrName = m_pCollectionScroll->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_Name();

		if (m_eCurrentCharDetail ==
			(E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo))
		{
			if (0 <= i - 1)
			{
				Create_Ui_CharDetailInfo(CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex(i - 1)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo));
			}
			else
			{
				Create_Ui_CharDetailInfo(CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_pCollectionScroll->Get_ChildByIndex((_int)m_pCollectionScroll->Get_ChildList()->size() - 1)->Get_ChildByIndex(0)->Get_Name(), E_Translation_Type::Translation_CharInfo));
			}

			break;
		}
	}

	Initialize_StudentDetail();
}

void CUI_Manager::Check_LevelUp(_uint* iExperience)
{
	if (m_iCharLevelAfterLvUp * CharExpInc <= *iExperience)
	{
		*iExperience -= m_iCharLevelAfterLvUp * CharExpInc;
		++m_iCharLevelAfterLvUp;

		Check_LevelUp(iExperience);
	}
}

void CUI_Manager::Check_LevelDown(const _uint& iIndex, _int iLeftExp)
{
	_int	iNextExp = m_iCharExperienceAfterLvUp;

	if (0 != iLeftExp)
	{
		iNextExp += iLeftExp;
	}
	else
	{
		switch (iIndex)
		{
		case 0:
			iNextExp -= CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������)->iItemEffect;
			break;

		case 1:
			iNextExp -= CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������)->iItemEffect;
			break;

		case 2:
			iNextExp -= CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������)->iItemEffect;
			break;

		case 3:
			iNextExp -= CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������)->iItemEffect;
			break;

		default:
			assert(0);
			break;
		}
	}

	if (0 > iNextExp)
	{
		if (1 < m_iCharLevelAfterLvUp)
		{
			--m_iCharLevelAfterLvUp;
			m_iCharExperienceAfterLvUp = m_iCharLevelAfterLvUp * CharExpInc;

			Check_LevelDown(iIndex, iNextExp);
		}
		else
		{
			m_iCharExperienceAfterLvUp = 0;
		}
	}
	else
	{
		m_iCharExperienceAfterLvUp = iNextExp;
	}
}

void CUI_Manager::Check_StatAfterLvUp()
{
	if (E_CHAR_NAME::Name_End == m_eCurrentCharDetail)
	{
		assert(0);
	}

	T_CharInfo	tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eCurrentCharDetail);

	// Right->7->0->1->10 ~ 14(ȭ��ǥ)
	// Right->7->0->1->15 ~ 19(���� Lv, ü��, ����, ���ݷ�, ġ����)
	if (tCharInfo.iLevel < m_iCharLevelAfterLvUp)
	{
		for (_uint i = 10; i < 20; ++i)
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Set_Active(TRUE);
		}

		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(15)->Set_Ui_Text(to_wstring(m_iCharLevelAfterLvUp));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(16)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iMaxHp + (m_iCharLevelAfterLvUp - tCharInfo.iLevel) * tCharInfo.iIncLevel_HP));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(17)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iDeffence + (m_iCharLevelAfterLvUp - tCharInfo.iLevel) * tCharInfo.iIncLevel_DEF));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(18)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iAttack + (m_iCharLevelAfterLvUp - tCharInfo.iLevel) * tCharInfo.iIncLevel_ATK));
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(19)->Set_Ui_Text(to_wstring(tCharInfo.tUnitInfo.iHeal + (m_iCharLevelAfterLvUp - tCharInfo.iLevel) * tCharInfo.iIncLevel_HEAL));
	}
	else
	{
		for (_uint i = 10; i < 20; ++i)
		{
			m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Initialize_StudentDetail()
{
	m_eSelSkillUpgrade = E_SkillType::Skill_Ex;

	m_iCurrentCredit = CGameManager::GetInstance()->Get_Credit();

	m_iCurrentReinforceItem[0] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::����_Ȱ������)->iCount;
	m_iCurrentReinforceItem[1] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�Ϲ�_Ȱ������)->iCount;
	m_iCurrentReinforceItem[2] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::���_Ȱ������)->iCount;
	m_iCurrentReinforceItem[3] = CGameManager::GetInstance()->Get_Item((_int)E_ItemTag::�ֻ��_Ȱ������)->iCount;

	for (_uint i = 0; i < 4; ++i)
	{
		m_iLvUpItemInsertCount[i] = 0;
	}

	if (E_CHAR_NAME::Name_End != m_eCurrentCharDetail)
	{
		m_iCharLevelAfterLvUp = *(_uint*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iLevel);
		m_iCharExperienceAfterLvUp = *(_uint*)CCharacterMgr::GetInstance()->Get_CharInfo_Detail(m_eCurrentCharDetail, E_CHAR_STRUCTALL::CHARINFO_iExperience);
	}

	for (_uint i = 10; i < 20; ++i)
	{
		m_pCharDetail_RightMid->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Set_Active(FALSE);
	}
}

void CUI_Manager::Update_StudentDetail()
{
	if (!m_pCharDetail)
	{
		return;
	}

	// ���� �߰�
	if (m_pCharDetail->Get_Active())
	{

	}
}

#pragma endregion

#pragma region ��â

void CUI_Manager::Open_Ui_BattleFormation(_int eWorldMod)
{
	if (m_pLobbyPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);
	}


	m_eWorldMod = E_WORLD_MOD(eWorldMod);

	for (auto& tPair : m_mapCharBtn)
	{
		tPair.second->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
	}

	switch (m_eWorldMod)
	{
	case Engine::E_WORLD_MOD::MOD_MISSION:
		BattleFormation_MissionInit();
		break;
	case Engine::E_WORLD_MOD::MOD_WANTED:
		m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(TRUE);
		break;
	case Engine::E_WORLD_MOD::MOD_SPECIAL:
		m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(TRUE);
		break;
	case Engine::E_WORLD_MOD::MOD_TOTAL:
		m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(TRUE);
		break;
	case Engine::E_WORLD_MOD::MOD_END:
		m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(FALSE);
		for (_int i = 0; i < 4; ++i)
		{
			m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(8 + i)->Set_Active(FALSE);
		}
		break;
	default:
		break;
	}

	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(TRUE);
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Set_Active(FALSE);

	BattleFormation_ChangeSquad();
	BattleFormation_ActiveSquad(TRUE);

	m_pBattleFormationPanel->Set_Active(TRUE);
	m_bInputMouse = TRUE;

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�δ���,�ӹ�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"��â");
}


//��â ��ư�� ����� �����ϴ� �Լ�
void CUI_Manager::BattleFormation_AddFunction()
{
	//�δ� ���� ��ư
	m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_ChangeSquadInt, this, (_int)E_SQUAD::SQUAD_1), (_int)E_SQUAD::SQUAD_1);
	m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_ChangeSquadInt, this, (_int)E_SQUAD::SQUAD_2), (_int)E_SQUAD::SQUAD_2);
	m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_ChangeSquadInt, this, (_int)E_SQUAD::SQUAD_3), (_int)E_SQUAD::SQUAD_3);
	m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_ChangeSquadInt, this, (_int)E_SQUAD::SQUAD_4), (_int)E_SQUAD::SQUAD_4);

	for (_int i = 0; i < 6; ++i)
	{
		m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_DeleteSlot, this, i), i);
	}

	//������â ����
	for (_int i = 4; i < 6; ++i)
	{
		m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i)->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_SquadSlotClick, this, i), i);
	}
	m_pBattleFormationPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_OpenFormation, this));

	//������â �ݱ�
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_CloseFormation, this));
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(7)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_CloseFormation, this));

	//������â Ȯ��
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(8)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_AcceptSquad, this));

	//������â ���
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_Particpate, this));
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_SquadRelease, this));
}

//��â���� �� ��ư Ŭ���� �д� ��â�� �����ִ� �Լ�
void CUI_Manager::BattleFormation_OpenFormation()
{
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Set_Active(TRUE);
	BattleFormation_ActiveSquad(FALSE);

	wstring wstrPrevSquadNum = L"";
	for (auto& tPair : m_mapCharBtn)
	{
		switch (m_ePrevSquad)
		{
		case E_SQUAD::SQUAD_1:
			wstrPrevSquadNum = L"1";
			break;
		case E_SQUAD::SQUAD_2:
			wstrPrevSquadNum = L"2";
			break;
		case E_SQUAD::SQUAD_3:
			wstrPrevSquadNum = L"3";
			break;
		case E_SQUAD::SQUAD_4:
			wstrPrevSquadNum = L"4";
			break;
		default:
			break;
		}
		tPair.second->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(wstrPrevSquadNum);

		tPair.second->Get_ChildByIndex(2)->Set_Active(FALSE);
		tPair.second->Get_ChildByIndex(3)->Set_Active(FALSE);
		tPair.second->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);

		E_SQUAD eSquad = CGameManager::GetInstance()->DuplicateCheck_Character(tPair.first, E_SQUAD::SQUAD_1, E_SQUAD::SQUAD_4);
		if (eSquad != E_SQUAD::SQUAD_END)
		{
			if (eSquad == m_ePrevSquad)
			{
				tPair.second->Get_ChildByIndex(2)->Set_Active(TRUE);
			}
			else
			{
				tPair.second->Get_ChildByIndex(3)->Set_Active(TRUE);
				tPair.second->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
				wstring wstrDuplicateNumber = L"";
				switch (eSquad)
				{
				case E_SQUAD::SQUAD_1:
					wstrDuplicateNumber = L"1";
					break;
				case E_SQUAD::SQUAD_2:
					wstrDuplicateNumber = L"2";
					break;
				case E_SQUAD::SQUAD_3:
					wstrDuplicateNumber = L"3";
					break;
				case E_SQUAD::SQUAD_4:
					wstrDuplicateNumber = L"4";
					break;
				default:
					break;
				}

				tPair.second->Get_ChildByIndex(3)->Set_Ui_Text(wstrDuplicateNumber);
				tPair.second->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(wstrDuplicateNumber);

			}
		}
	}


	BattleFormation_NameCopy();
	BattleFormation_UpdateSquad();
	m_bInputMouse = FALSE;
}

//�д� ��â���� �ݱ� / ��� ��ư Ŭ���� �д���â�� ���泻���� ����ϰ� �ݾ��ִ� �Լ�
void CUI_Manager::BattleFormation_CloseFormation()
{
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(TRUE);
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Set_Active(FALSE);

	m_pBattleFormationPanel->Get_ChildByIndex(4)->Set_Active(FALSE);
	BattleFormation_ActiveSquad(TRUE);

	m_bInputMouse = TRUE; 
}

//�д� ��â���� ����� �д������� �о���� �Լ�
void CUI_Manager::BattleFormation_NameCopy()
{
	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pCharacter = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
		if (!pCharacter)
		{
			m_vecSquadStudentName[i] = E_CHAR_NAME::Name_End;
		}
		else
		{
			m_vecSquadStudentName[i] = pCharacter->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pCharacter = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];

		if (!pCharacter)
		{
			m_vecSquadStudentName[i + 4] = E_CHAR_NAME::Name_End;
		}
		else
		{
			m_vecSquadStudentName[i + 4] = pCharacter->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
		}
	}
}

//��â ������ ĳ���� ���� ������ ���� ����� �����ϴ� �Լ�
void CUI_Manager::BattleFormation_SetList()
{
	m_mapCharBtn.clear();
	CGameManager::GetInstance()->Collect_Update();
	vector<T_CharInfo>	tCharInfo = CCharacterMgr::GetInstance()->Get_vecCharInfo();

	wstring		wstrFileName = L"";
	wstring		wstrTexName = L"";

	size_t		iStartPos = 0;
	size_t		iEndPos = 0;

	CUI_Object* pUi_Char = nullptr;
	CUI_Object* pScrollView = nullptr;


	for (size_t i = 0; i < tCharInfo.size(); ++i)
	{
		// pUi = ��ũ�� �䰡 �Ǿ� �� 
		// pUiChar = ĳ���� ���ڰ� �Ǿ� �� 

		if (tCharInfo[i].bGetChar)
		{
			if (tCharInfo[i].eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
			{
				pScrollView = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(6)->Get_ChildByIndex(5);
			}
			else
			{
				pScrollView = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(6)->Get_ChildByIndex(6);
			}

			wstrFileName = L"../../Reference/Data/Data_UI/Prefab/BattleStudent.uidat";
			wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
				CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
				L".png";

			pUi_Char = Load_UI_Prefab(wstrFileName);
			pScrollView->Set_Child(pUi_Char);

			BattleFormation_SetWindow(pUi_Char->Get_ChildByIndex(0), tCharInfo[i]);

			iStartPos = wstrTexName.rfind(L"/");
			wstrTexName = wstrTexName.substr(iStartPos + 1);
			iEndPos = wstrTexName.rfind(L".");
			wstrTexName = wstrTexName.substr(0, iEndPos);

			pUi_Char->Set_NameTag(wstrTexName);
			
			wstring KorName = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)i, E_CHAR_ENUMTYPE::CNUM_KORNAME);
			pUi_Char->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Ui_Text(KorName);

			pUi_Char->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::BattleFormation_AddSquad, this, (_int)tCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG]), (_int)tCharInfo[i].eName[(_uint)E_CHAR_LANG::LANG_ENG]);

			pUi_Char->Set_Active(TRUE);

			m_mapCharBtn.emplace(tCharInfo[i].eName[(_int)E_CHAR_LANG::LANG_ENG], pUi_Char);
		}
	}
}

// �д� ������ �ʻ�ȭ ���ý� �д뿡 �߰��ϴ� �Լ�
void CUI_Manager::BattleFormation_AddSquad(_int iCharName)
{
	_int iEmptyIndex = 10;
	if (CCharacterMgr::GetInstance()->Get_CharInfo(E_CHAR_NAME(iCharName)).eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		for (_int i = 0; i < 4; ++i)
		{
			if (m_vecSquadStudentName[i] == E_CHAR_NAME(iCharName))
			{
				BattleFormation_DeleteSlot(i);

				if (m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active())
				{
					m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(3)->Set_Active(TRUE);
				}
				m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Set_Active(FALSE);



				BattleFormation_UpdateSquad();
				return;
			}
			else
			{
				if (m_vecSquadStudentName[i] == E_CHAR_NAME::Name_End)
				{
					if (iEmptyIndex > i)
					{
						iEmptyIndex = i;
					}
				}
			}
		}
	}
	else
	{
		for (_int i = 0; i < 2; ++i)
		{
			if (m_vecSquadStudentName[i + 4] == E_CHAR_NAME(iCharName))
			{
				BattleFormation_DeleteSlot(i + 4);

				if (m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active())
				{
					m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(3)->Set_Active(TRUE);
				}
				m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Set_Active(FALSE);
				BattleFormation_UpdateSquad();
				return;
			}
			else
			{
				if (m_vecSquadStudentName[i + 4] == E_CHAR_NAME::Name_End)
				{
					if (iEmptyIndex > i + 4)
					{
						iEmptyIndex = i + 4;
					}
				}
			}
		}
	}

	if (iEmptyIndex < 6)
	{
		if (m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_Active())
		{
			m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Active(TRUE);
			m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(8)->Get_ChildByIndex(1)->Set_Active(FALSE);
		}

		m_vecSquadStudentName[iEmptyIndex] = E_CHAR_NAME(iCharName);
		m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Set_Active(TRUE);
		if (m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active())
		{
			m_mapCharBtn[E_CHAR_NAME(iCharName)]->Get_ChildByIndex(3)->Set_Active(FALSE);
		}

		BattleFormation_SetCharInfo(E_CHAR_NAME(iCharName));
	}
	BattleFormation_UpdateSquad();
}

//�д� �� �� Ȯ�� ��ư�� ������ �۵��Ͽ� ��������� �����ϴ� �Լ�
void CUI_Manager::BattleFormation_AcceptSquad()
{
	//�ߺ� �˻�
	for (_int i = 0; i < (_int)m_vecSquadStudentName.size(); ++i)
	{
		E_SQUAD eDuplicateSquad = CGameManager::GetInstance()->DuplicateCheck_Character(m_vecSquadStudentName[i], E_SQUAD::SQUAD_1, E_SQUAD::SQUAD_4, m_ePrevSquad);
		if (eDuplicateSquad != E_SQUAD::SQUAD_END)
		{
			CGameManager::GetInstance()->DeleteStudent(m_vecSquadStudentName[i], eDuplicateSquad, eDuplicateSquad);
		}
	}

	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pCharacter = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
		if (pCharacter)
		{
			pCharacter->Set_Dead(TRUE);
			Safe_Release(pCharacter);
			(*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i] = nullptr;
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pCharacter = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
		if (pCharacter)
		{
			Safe_Release(pCharacter);
			(*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i] = nullptr;
		}
	}

	for (_int i = 0; i < (_int)m_vecSquadStudentName.size(); ++i)
	{


		if (m_vecSquadStudentName[i] != E_CHAR_NAME::Name_End)
		{
			E_CHAR_COMBATTYPE eCombatType = CCharacterMgr::GetInstance()->Get_CharInfo(m_vecSquadStudentName[i]).eCombatType;
			_int iIndex = i;
			if (iIndex >= 4)
			{
				iIndex -= 4;
			}

			CGameManager::GetInstance()->Add_Formation(m_vecSquadStudentName[i], m_ePrevSquad, eCombatType, iIndex);
		}
	}

	CGameManager::GetInstance()->Spawn_Formation(m_ePrevSquad);
	BattleFormation_SetSquadPosition();
	BattleFormation_UpdateCard();
	BattleFormation_CloseFormation();
}

//�� UI �ϴ� �д�� ����� ���Ž�Ű�� �Լ�
void CUI_Manager::BattleFormation_UpdateSquad()
{


	for (_int i = 0; i < 6; ++i)
	{
		CUI_Object* pWindow = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Get_ChildByIndex(i)->Get_ChildByIndex(1);
		if (m_vecSquadStudentName[i] == E_CHAR_NAME::Name_End)
		{
			pWindow->Set_Active(FALSE);
		}
		else
		{
			BattleFormation_SetWindow(pWindow, CCharacterMgr::GetInstance()->Get_CharInfo(m_vecSquadStudentName[i]));
			pWindow->Set_Active(TRUE);

			if (m_mapCharBtn[m_vecSquadStudentName[i]]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_Active())
			{
				pWindow->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);
			}
			else
			{
				pWindow->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
			}
		}
	}
}

//�δ� ��ư�� �������� �۵��Ͽ� ���̴� �δ���� ���Ž�Ű�� �Լ�
void CUI_Manager::BattleFormation_ChangeSquad()
{

	if ((_int)m_eCurrentSquad < 0 || (_int)m_eCurrentSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return;
	}
	if (m_ePrevSquad != m_eCurrentSquad)
	{
		map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
		if (pSquad)
		{
			for (auto& pChar : (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
			{
				if (pChar)
				{
					pChar->Set_Dead(TRUE);
				}
			}
		}
		m_ePrevSquad = m_eCurrentSquad;

		CGameManager::GetInstance()->Spawn_Formation(m_eCurrentSquad);
		CGameManager::GetInstance()->Set_CurrentSquadEnum(m_ePrevSquad);
	}
	BattleFormation_SetSquadPosition();
	BattleFormation_UpdateCard();

	if (m_eWorldMod == E_WORLD_MOD::MOD_MISSION)
	{
		_int iIndex = (_int)m_ePrevSquad;
		if (m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(8 + iIndex)->Get_Active())
		{
			m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(FALSE);
			m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(TRUE);
		}
		else
		{
			m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(TRUE);
			m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(FALSE);
		}
	}
}

//�Ű������� �ش��ϴ� �д�� �ٲ��ִ� �Լ�
void CUI_Manager::BattleFormation_ChangeSquadInt(_int eSquad)
{
	m_eCurrentSquad = E_SQUAD(eSquad);
	BattleFormation_ChangeSquad();
}

//�д��� ��Ƽ�� ���θ� �����ϴ� �Լ�
void CUI_Manager::BattleFormation_ActiveSquad(_bool bActive)
{
	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
	if (!pSquad)
	{
		return;
	}
	for (auto& pChar : (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			pChar->Set_Active(bActive);
		}
	}
}

//UI �� ��ġ�Ǿ�� �ϴ� ��ġ�� ĳ���� �������� �������ִ� �Լ�
void CUI_Manager::BattleFormation_SetSquadPosition()
{
	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
	if (!pSquad)
	{
		return;
	}

	_float fCenterX = _float(WINCX >> 1);
	_float fCenterY = _float(WINCY >> 1);
	_vec3 vCenterPos = CUtility::Ui2World(fCenterX, fCenterY);
	_matrix matInvView;
	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	_vec3 vViewDir = _vec3(matInvView._41, matInvView._42, matInvView._43) - vCenterPos;
	_int iIndex = 0;

	
	vector<CGameObject*>* pVecPlayer = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	_bool bCheck = FALSE;


	if (pVecPlayer)
	{
		for (auto& pObj : *pVecPlayer)
		{
			for (auto& pChar : (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
			{
				if (pChar)
				{
					if (pObj == pChar)
					{
						bCheck = TRUE;
						break;
					}
				}
			}
			if (bCheck)
			{
				break;
			}
		}
	}

	
	for (auto& pChar : (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			if (bCheck)
			{
				pChar->Set_Active(TRUE);
			}
			else
			{
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player, pChar);
				pChar->AddRef();
			}
		}
	}
	
	

	for (auto& pChar : (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
		
			pChar->Reset();
			pChar->Set_ShowWindow(TRUE);

			_vec3 vPos = CUtility::Ui2World(_float(m_tCharacterSlotPoint[iIndex].x), _float(m_tCharacterSlotPoint[iIndex].y));
			pChar->Set_Pos(vPos);

			pChar->Set_Direction(vViewDir);
			pChar->Set_Scale(_vec3(0.25f, 0.25f, 0.25f));
				
		}
		++iIndex;
	}
}

void CUI_Manager::Create_Ui_BattleFormation()
{	if (!m_pBattleFormationPanel)
	{
		_int iX = 300;
		_int iY = WINCY - 485;
		for (_int i = 0; i < 4; ++i)
		{
			m_tCharacterRect[i].left = _long(iX - 100);
			m_tCharacterRect[i].right = _long(iX + 100);
			m_tCharacterRect[i].top = _long(iY - 300);
			m_tCharacterRect[i].bottom = _long(iY + 200);

			m_tCharacterSlotPoint[i].x = iX;
			m_tCharacterSlotPoint[i].y = iY;
			iX += 230;
		}


		CUI_Object* pUi = nullptr;

		pUi = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/BattleFormationPanel.uidat");

		pUi->Set_NameTag(L"��â");
		pUi->Set_Active(FALSE);

		m_pBattleFormationPanel = pUi;
		BattleFormation_AddFunction();
		BattleFormation_SetList();

		if (m_vecSquadStudentName.empty())
		{
			m_vecSquadStudentName.resize(6);
			for (auto& eCharName : m_vecSquadStudentName)
			{
				eCharName = E_CHAR_NAME::Name_End;
			}
		}
		CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pBattleFormationPanel->Get_ObjTag(), m_pBattleFormationPanel);

		m_eCurrentSquad = E_SQUAD::SQUAD_1;
	}
}

// �д� �� ��Ͽ� �ߴ� �ʻ�ȭ�� �˸��� ������ �������ִ� �Լ�
void CUI_Manager::BattleFormation_SetWindow(CUI_Object * pUIObj, const T_CharInfo & tCharInfo)
{
	E_CHAR_NAME eCharName = tCharInfo.eName[(_int)E_CHAR_LANG::LANG_ENG];
	wstring wstrCharName = CCharacterMgr::GetInstance()->Translation_EnumToString((_int)eCharName, E_CHAR_ENUMTYPE::CNUM_NAME);
	pUIObj->Set_NameTag(wstrCharName);

	wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
		CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG], E_CHAR_ENUMTYPE::CNUM_NAME) +
		L".png";
	pUIObj->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

	if (tCharInfo.eAtkType == E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION)
	{
		pUIObj->Get_ChildByIndex(0)->Set_ImageColor(COLOR_EXPLOSION);
	}
	else if (tCharInfo.eAtkType == E_CHAR_ATKTYPE::ATKTYPE_PIERCE)
	{
		pUIObj->Get_ChildByIndex(0)->Set_ImageColor(COLOR_PIERCE);
	}
	else
	{
		pUIObj->Get_ChildByIndex(0)->Set_ImageColor(COLOR_MYSTERY);
	}

	//��
	pUIObj->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring((_int)tCharInfo.eStar));

	//������
	wstring wstrRoleTexPath = L"";
	switch (tCharInfo.eRole)
	{
	case E_CHAR_ROLE::ROLE_DEALER:
		wstrRoleTexPath = L"Dealer";
		break;
	case E_CHAR_ROLE::ROLE_HEALER:
		wstrRoleTexPath = L"Healer";
		break;
	case E_CHAR_ROLE::ROLE_SUPPORTER:
		wstrRoleTexPath = L"Supporter";
		break;
	case E_CHAR_ROLE::ROLE_TANKER:
		wstrRoleTexPath = L"Tanker";
		break;
	default:
		break;
	}
	wstrRoleTexPath = L"../../Reference/Resource/Texture/UI/Position/" + wstrRoleTexPath + L".png";
	pUIObj->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrRoleTexPath);

	//����
	wstring wstrLevelText = L"Lv." + to_wstring(tCharInfo.iLevel);
	pUIObj->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(L"");

	CUI_Object* pOutLine = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�����ƿ�����.uidat");
	
	
	for (_int i = 0; i < 5; ++i)
	{
		pOutLine->Get_ChildByIndex(i)->Set_Ui_Text(wstrLevelText);
	}
	pUIObj->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Child(pOutLine);
	
	
	
	pUIObj->Set_Active(TRUE);



}

//�д�� ��� �ʻ�ȭ ���ý� �д� ��Ͽ��� �ʻ�ȭ�� �����ϴ� �Լ�
void CUI_Manager::BattleFormation_DeleteSlot(_int iSlot)
{
	CUI_Object* pUIObj = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Get_ChildByIndex(iSlot)->Get_ChildByIndex(1);


	if (m_mapCharBtn[m_vecSquadStudentName[iSlot]]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active())
	{
		m_mapCharBtn[m_vecSquadStudentName[iSlot]]->Get_ChildByIndex(3)->Set_Active(TRUE);
	}
	m_mapCharBtn[m_vecSquadStudentName[iSlot]]->Get_ChildByIndex(2)->Set_Active(FALSE);

	m_vecSquadStudentName[iSlot] = E_CHAR_NAME::Name_End;


	pUIObj->Set_Active(FALSE);

	_bool bCheck = TRUE;
	for (auto& eCharName : m_vecSquadStudentName)
	{
		if (eCharName == E_CHAR_NAME::Name_End)
		{
			continue;
		}

		if (m_mapCharBtn[eCharName]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_Active())
		{
			bCheck = FALSE;
			
			break;
		}
	}
	if (bCheck)
	{
		m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Active(FALSE);
		m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(8)->Get_ChildByIndex(1)->Set_Active(TRUE);

		m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(TRUE);
		m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Set_Active(FALSE);
	}
}

void CUI_Manager::BattleFormation_UpdateCard()
{
	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);

	for (_int i = 0; i < 4; ++i)
	{
		CCharacter* pChar = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
		CUI_Object* pCharCard = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i+4)->Get_ChildByIndex(2);
		CUI_Object* pEmptyIcon = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i+4)->Get_ChildByIndex(0);
		if (pChar)
		{
			//���� ���� ����

			BattleFormation_SetCard(pCharCard, pChar->Get_CharInfo(), i);
			pCharCard->Set_Active(TRUE);
			pEmptyIcon->Set_Active(FALSE);
		}
		else
		{
			pCharCard->Set_Active(FALSE);
			pEmptyIcon->Set_Active(TRUE);
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
		CUI_Object* pWindow = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i + 8)->Get_ChildByIndex(2);
		CUI_Object* pCharCard = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i + 8)->Get_ChildByIndex(4);

		if (pChar)
		{
			BattleFormation_SetWindow(pWindow, pChar->Get_CharInfo());
			BattleFormation_SetCard(pCharCard, pChar->Get_CharInfo(), i + 4);
			pCharCard->Set_Active(TRUE);
			pWindow->Set_Active(TRUE);

			if (m_mapCharBtn[pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_Active())
			{
				pWindow->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);
			}
			else
			{
				pWindow->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
			}
		}
		else
		{
			pCharCard->Set_Active(FALSE);
			pWindow->Set_Active(FALSE);
		}
	}

}

// �� â���� ĳ������ ������ ���Ǿ� �ִ� ������ �����ϴ� �Լ�
void CUI_Manager::BattleFormation_SetCard(CUI_Object * pCard, const T_CharInfo & tCharInfo, _int iIndex)
{
	if (!pCard)
	{
		return;
	}

	//�̸�
	wstring KorName = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eName[0], E_CHAR_ENUMTYPE::CNUM_KORNAME);
	pCard->Get_ChildByIndex(0)->Set_Ui_Text(KorName);

	//��
	pCard->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring((_int)tCharInfo.eStar));

	//����
	wstring wstrLevelText = L"Lv." + to_wstring(tCharInfo.iLevel);
	pCard->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(wstrLevelText);

	//���ݼӼ� ����
	wstring wstAtkType = L"";
	switch (tCharInfo.eAtkType)
	{
	case E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION:
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_ImageColor(COLOR_EXPLOSION);
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"����");
		break;
	case E_CHAR_ATKTYPE::ATKTYPE_PIERCE:
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_ImageColor(COLOR_PIERCE);
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"����");
		break;
	case E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_ImageColor(COLOR_MYSTERY);
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"�ź�");
		break;
	default:
		break;
	}


	//������
	if (tCharInfo.eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		wstring wstrPosition = L"";
		switch (tCharInfo.ePosition)
		{
		case E_CHAR_POSITION::POSITION_BACK:
			wstrPosition = L"BACK";
			break;
		case E_CHAR_POSITION::POSITION_FRONT:
			wstrPosition = L"FRONT";
			break;
		case E_CHAR_POSITION::POSITION_MIDDLE:
			wstrPosition = L"MIDDLE";
			break;
		default:
			break;
		}
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(wstrPosition);
	}

	//����
	if (tCharInfo.eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		if (iIndex == CGameManager::GetInstance()->GetReaderIndex(m_ePrevSquad))
		{
			pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Active(TRUE);
		}
		else
		{
			pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Active(FALSE);
		}
	}

	//������ �ΰ�
	wstring wstrRoleTexPath = L"";
	switch (tCharInfo.eRole)
	{
	case E_CHAR_ROLE::ROLE_DEALER:
		wstrRoleTexPath = L"Dealer";
		break;
	case E_CHAR_ROLE::ROLE_HEALER:
		wstrRoleTexPath = L"Healer";
		break;
	case E_CHAR_ROLE::ROLE_SUPPORTER:
		wstrRoleTexPath = L"Supporter";
		break;
	case E_CHAR_ROLE::ROLE_TANKER:
		wstrRoleTexPath = L"Tanker";
		break;
	default:
		break;
	}
	wstrRoleTexPath = L"../../Reference/Resource/Texture/UI/Position/" + wstrRoleTexPath + L".png";
	if (tCharInfo.eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(4)->Set_Tex1_Tag(wstrRoleTexPath);
	}
	else
	{
		pCard->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Tex1_Tag(wstrRoleTexPath);
	}
}

void CUI_Manager::BattleFormation_SquadSlotClick(_int iSlotNum)
{
	map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);
	_int iIndex = iSlotNum;

	iIndex -= 4;

	CCharacter* pChar = nullptr;

	pChar = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][iIndex];
	if (pChar)
	{
		// ĳ���� ���� ����
	}
	BattleFormation_OpenFormation();


}

void CUI_Manager::BattleFormation_MouseControl()
{
	if (!m_bInputMouse)
	{
		return;
	}

	POINT tPoint = CUtility::Get_MousePoint(*m_pHwnd);

	if (!m_bKeyDown && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		m_bKeyDown = TRUE;

		m_bMove = FALSE;
		m_pPickCharacter = nullptr;
		m_fPickTime = CScene::Get_PlayTime();

		for (_int i = 0; i < 4; ++i)
		{

			if (tPoint.x >= m_tCharacterRect[i].left && tPoint.x < m_tCharacterRect[i].right)
			{
				if (tPoint.y >= m_tCharacterRect[i].top && tPoint.y < m_tCharacterRect[i].bottom)
				{

					map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);

					CCharacter* pChar = nullptr;
					pChar = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
					if (!pChar)
					{
						BattleFormation_OpenFormation();
						return;
					}
					else
					{
						m_iPickIndex = i;
						m_tPickPoint = tPoint;
						m_pPickCharacter = pChar;
						m_pPickCharacter->Set_Action(E_ACTION::Formation_Pickup);
					}

					break;
				}
			}
		}
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_pPickCharacter && !m_bMove)
		{
			if (m_fPickTime + 1.f < CScene::Get_PlayTime())
			{
				_vec3 vPos = CUtility::Ui2World(_float(m_tCharacterSlotPoint[m_iPickIndex].x), _float(m_tCharacterSlotPoint[m_iPickIndex].y));
				m_pPickCharacter->Set_Pos(vPos);


				//Create_Ui_CharDetailInfo((_int)m_pPickCharacter->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]);

				m_pPickCharacter->Set_Action(E_ACTION::Formation_Idle);
				m_pPickCharacter = nullptr;
			}
		}

		if (m_pPickCharacter)
		{
			POINT tNewPoint = m_tCharacterSlotPoint[m_iPickIndex];
			tNewPoint.x += tPoint.x - m_tPickPoint.x;
			tNewPoint.y -= tPoint.y - m_tPickPoint.y;

			if (abs(tPoint.x - m_tPickPoint.x) > 5)
				m_bMove = TRUE;
			if (abs(tPoint.y - m_tPickPoint.y) > 5)
				m_bMove = TRUE;

			_vec3 vPos = CUtility::Ui2World(_float(tNewPoint.x), _float(tNewPoint.y));
			m_pPickCharacter->Set_Pos(vPos);
			
			for (_int i = 0; i < 4; ++i)
			{
				if (i == m_iPickIndex)
				{
					continue;
				}

				map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);

				CCharacter* pChar = nullptr;
				pChar = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
				if (pChar)
				{
					pChar->Set_Scale(_vec3(0.25f, 0.25f, 0.25f));
				}

				if (tPoint.x >= m_tCharacterRect[i].left && tPoint.x < m_tCharacterRect[i].right)
				{
					if (tPoint.y >= m_tCharacterRect[i].top && tPoint.y < m_tCharacterRect[i].bottom)
					{
						if (pChar)
						{
							pChar->Set_Scale(_vec3(0.2f, 0.2f, 0.2f));
						}
					}
				}
			}
		}

	}
	if (m_bKeyDown && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		m_bKeyDown = FALSE;
		if (m_pPickCharacter)
		{
			_bool bCheck = FALSE;

			if (!m_bMove)
			{
				//ĳ���� ���� ����
				BattleFormation_OpenFormation();
				BattleFormation_SetCharInfo(m_pPickCharacter->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]);

				
			}
			else
			{
				for (_int i = 0; i < 4; ++i)
				{
					if (i == m_iPickIndex)
					{
						continue;
					}

					if (tPoint.x >= m_tCharacterRect[i].left && tPoint.x < m_tCharacterRect[i].right)
					{
						if (tPoint.y >= m_tCharacterRect[i].top && tPoint.y < m_tCharacterRect[i].bottom)
						{
							CGameManager::GetInstance()->Swap_Character(m_ePrevSquad, E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER, m_iPickIndex, i);
							bCheck = TRUE;

							BattleFormation_UpdateCard();
							BattleFormation_SetSquadPosition();
							break;
						}
					}
				}
			}
			if (!bCheck)
			{
				_vec3 vPos = CUtility::Ui2World(_float(m_tCharacterSlotPoint[m_iPickIndex].x), _float(m_tCharacterSlotPoint[m_iPickIndex].y));
				m_pPickCharacter->Set_Pos(vPos);
			}

		}
		if (m_pPickCharacter)
		{
			m_pPickCharacter->Set_Action(E_ACTION::Formation_Idle);
		}
		m_pPickCharacter = nullptr;
	}
}

void CUI_Manager::BattleFormation_Particpate()
{
	_bool bCheck = FALSE;
	for (auto& pChar : (*CGameManager::GetInstance()->Get_Squad(m_ePrevSquad))[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			bCheck = TRUE;
			break;
		}
	}

	if (!bCheck)
	{
		return;
	}

	for (auto& pChar : (*CGameManager::GetInstance()->Get_Squad(m_ePrevSquad))[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			pChar->Set_Dead(TRUE);	
		}
	}
	for (auto& pChar : (*CGameManager::GetInstance()->Get_Squad(m_ePrevSquad))[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL])
	{
		if (pChar)
		{
			pChar->Set_Dead(TRUE);
		}
	}


	m_pBattleFormationPanel->Set_Active(FALSE);

	BattleFormation_ActiveSquad(FALSE);

	if (m_pLobbyPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(FALSE);
	}

	CGameManager::GetInstance()->Add_PlayerMascot(m_ePrevSquad);
}

void CUI_Manager::BattleFormation_SquadRelease()
{
	m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(8 + (_int)m_ePrevSquad)->Set_Active(FALSE);
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(FALSE);

	CGameManager::GetInstance()->Get_TileField()->Release_PlayerMascot(m_ePrevSquad);

	map<_int, vector<CCharacter*>>* pMapSquad = CGameManager::GetInstance()->Get_Squad(m_ePrevSquad);

	CCharacter* pChar = nullptr;
	for (_int i = 0; i < 4; ++i)
	{
		pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
		if (pChar)
		{
			m_mapCharBtn[pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
		if (pChar)
		{
			m_mapCharBtn[pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
		}
	}

	// ������� UI ����
	for (_int i = 0; i < 2; ++i)
	{
		CCharacter* pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
		CUI_Object* pWindow = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i + 8)->Get_ChildByIndex(2);
		CUI_Object* pCharCard = m_pBattleFormationPanel->Get_ChildByIndex(1)->Get_ChildByIndex(i + 8)->Get_ChildByIndex(4);

		if (pChar)
		{
			pWindow->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::BattleFormation_MissionInit()
{
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(TRUE);
	for (auto& pMascot : CGameManager::GetInstance()->Get_TileField()->Get_PlayerMascot())
	{
		E_SQUAD eReadySquad = pMascot->Get_Squad();
		m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(8 + (_int)eReadySquad)->Set_Active(TRUE);

		map<_int, vector<CCharacter*>>* pMapSquad = CGameManager::GetInstance()->Get_Squad(eReadySquad);

		CCharacter* pChar = nullptr;
		for (_int i = 0; i < 4; ++i)
		{
			pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i];
			if (pChar)
			{
				E_CHAR_NAME eCharName = pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
				m_mapCharBtn[eCharName]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);
			}
		}

		for (_int i = 0; i < 2; ++i)
		{
			pChar = (*pMapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i];
			if (pChar)
			{
				E_CHAR_NAME eCharName = pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
				m_mapCharBtn[eCharName]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);
			}
		}
	}
}

//�д� �� ���� ĳ���� ������ �����ϴ� �Լ�
void CUI_Manager::BattleFormation_SetCharInfo(E_CHAR_NAME eCharName)
{
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Set_Active(TRUE);

	CUI_Object* pBasicInfo = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Get_ChildByIndex(0);
	CUI_Object* pSkillInfo = m_pBattleFormationPanel->Get_ChildByIndex(4)->Get_ChildByIndex(9)->Get_ChildByIndex(1);
	T_CharInfo tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(eCharName);


	//���Ӽ� ����
	wstring wstrName = L"";
	D3DCOLOR colorTemp;
	switch (tCharInfo.eDffType)
	{
	case E_CHAR_DFFTYPE::DFFTYPE_LIGHT:
		wstrName = L"���尩";
		colorTemp = COLOR_EXPLOSION;
		break;
	case E_CHAR_DFFTYPE::DFFTYPE_HEAVY:
		wstrName = L"���尩";
		colorTemp = COLOR_PIERCE;
		break;
	case E_CHAR_DFFTYPE::DFFTYPE_SPECICAL:
		wstrName = L"Ư���尩";
		colorTemp = COLOR_MYSTERY;
		break;
	default:
		break;
	}
	pBasicInfo->Get_ChildByIndex(0)->Set_Ui_Text(wstrName);
	pBasicInfo->Get_ChildByIndex(0)->Set_ImageColor(colorTemp);

	//���ݼӼ� ����

	switch (tCharInfo.eAtkType)
	{
	case E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION:
		wstrName = L"����";
		colorTemp = COLOR_EXPLOSION;
		break;
	case E_CHAR_ATKTYPE::ATKTYPE_PIERCE:
		wstrName = L"����";
		colorTemp = COLOR_PIERCE;
		break;
	case E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:
		wstrName = L"�ź�";
		colorTemp = COLOR_MYSTERY;
		break;
	}
	pBasicInfo->Get_ChildByIndex(1)->Set_Ui_Text(wstrName);
	pBasicInfo->Get_ChildByIndex(1)->Set_ImageColor(colorTemp);

	for (_int i = 0; i < 4; ++i)
	{
		pSkillInfo->Get_ChildByIndex(i + 1)->Get_ChildByIndex(0)->Set_ImageColor(colorTemp);
	}

	//����
	BattleFormation_SetWindow(pBasicInfo->Get_ChildByIndex(4), tCharInfo);



	//������ Text
	switch (tCharInfo.ePosition)
	{
	case E_CHAR_POSITION::POSITION_BACK:
		wstrName = L"BACK";
		break;
	case E_CHAR_POSITION::POSITION_FRONT:
		wstrName = L"FRONT";
		break;
	case E_CHAR_POSITION::POSITION_MIDDLE:
		wstrName = L"MIDDLE";
		break;
	default:
		break;
	}
	pBasicInfo->Get_ChildByIndex(6)->Set_Ui_Text(wstrName);

	//Ŭ����
	switch (tCharInfo.eRole)
	{
	case E_CHAR_ROLE::ROLE_DEALER:
		wstrName = L"����";
		pBasicInfo->Get_ChildByIndex(7)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Dealer.png");
		break;
	case E_CHAR_ROLE::ROLE_HEALER:
		wstrName = L"����";
		pBasicInfo->Get_ChildByIndex(7)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Healer.png");
		break;
	case E_CHAR_ROLE::ROLE_SUPPORTER:
		wstrName = L"������";
		pBasicInfo->Get_ChildByIndex(7)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Supporter.png");
		break;
	case E_CHAR_ROLE::ROLE_TANKER:
		wstrName = L"��Ŀ";
		pBasicInfo->Get_ChildByIndex(7)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Tanker.png");
		break;
	}
	pBasicInfo->Get_ChildByIndex(7)->Set_Ui_Text(wstrName);
	
	//�п� ������


	//�п���
	switch (tCharInfo.eSchoolType)
	{
	case Engine::E_CHAR_SCHOOL::SCHOOL_ABYDOS:
		wstrName = L"�ƺ񵵽� ����б�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/ABYDOS.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_TRINITY:
		wstrName = L"Ʈ����Ƽ �����п�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/TRINITY.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_GEHENNA:
		wstrName = L"��� �п�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/GEHENNA.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
		wstrName = L"�з��Ͼ� ���̾� ����";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/MILLENNIUM.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
		wstrName = L"��;��� �����п�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/HYAKKIYAKO.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
		wstrName = L"���ذ� ������б�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/SHANHAIJING.png");
		break;
	case Engine::E_CHAR_SCHOOL::SCHOOL_REDWINTER:
		wstrName = L"�����ܿ� �����п�";
		pBasicInfo->Get_ChildByIndex(5)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/REDWINTER.png");
		break;
	default:
		break;
	}
	
	pBasicInfo->Get_ChildByIndex(8)->Set_Ui_Text(wstrName);

	//�л���
	wstring KorName = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_uint)tCharInfo.eName[0], E_CHAR_ENUMTYPE::CNUM_KORNAME);
	pBasicInfo->Get_ChildByIndex(9)->Set_Ui_Text(KorName);

	//�ð���
	wstrName = L"../../Reference/Resource/Texture/UI/BaseImage/Icon_";
	switch (tCharInfo.eInCityBuff)
	{
		case E_CHAR_MAPBUFF::MAPBUFF_A:
			wstrName += L"A.png";
			break;
		case E_CHAR_MAPBUFF::MAPBUFF_B:
			wstrName += L"B.png";
			break;
		case E_CHAR_MAPBUFF::MAPBUFF_C:
			wstrName += L"C.png";
			break;
		case E_CHAR_MAPBUFF::MAPBUFF_D:
			wstrName += L"D.png";
			break;
		case E_CHAR_MAPBUFF::MAPBUFF_S:
			wstrName += L"S.png";
			break;
		case E_CHAR_MAPBUFF::MAPBUFF_SS:
			wstrName += L"SS.png";
			break;
	}
	
	pBasicInfo->Get_ChildByIndex(10)->Set_Tex1_Tag(wstrName);

	//����
	wstrName = L"../../Reference/Resource/Texture/UI/BaseImage/Icon_";
	switch (tCharInfo.eOutDoorBuff)
	{
	case E_CHAR_MAPBUFF::MAPBUFF_A:
		wstrName += L"A.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_B:
		wstrName += L"B.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_C:
		wstrName += L"C.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_D:
		wstrName += L"D.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_S:
		wstrName += L"S.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_SS:
		wstrName += L"SS.png";
		break;
	}
	pBasicInfo->Get_ChildByIndex(11)->Set_Tex1_Tag(wstrName);

	//�ǳ���
	wstrName = L"../../Reference/Resource/Texture/UI/BaseImage/Icon_";
	switch (tCharInfo.eInDoorBuff)
	{
	case E_CHAR_MAPBUFF::MAPBUFF_A:
		wstrName += L"A.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_B:
		wstrName += L"B.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_C:
		wstrName += L"C.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_D:
		wstrName += L"D.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_S:
		wstrName += L"S.png";
		break;
	case E_CHAR_MAPBUFF::MAPBUFF_SS:
		wstrName += L"SS.png";
		break;
	}
	pBasicInfo->Get_ChildByIndex(12)->Set_Tex1_Tag(wstrName);


	// ��ų ����
	for (_int i = 0; i < 4; ++i)
	{
		WCHAR wszLevelText[16];
		swprintf_s(wszLevelText, L"Lv.%d", (_int)tCharInfo.eSkillLvArr[i]);
		pSkillInfo->Get_ChildByIndex(i+1)->Set_Ui_Text(wszLevelText);

		
	}
	wstring wstrSkillPath = L"../../Reference/Resource/Texture/UI/Skill/";
	pSkillInfo->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrSkillPath + tCharInfo.wstrExSkillImageFile);
	pSkillInfo->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrSkillPath + tCharInfo.wstrNormalSkillImageFile);
	pSkillInfo->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrSkillPath + tCharInfo.wstrReinforceSkillImageFile);
	pSkillInfo->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrSkillPath + tCharInfo.wstrSubSkillImageFile);

}

void CUI_Manager::BattleFormation_ParticpateReset()
{
	for (_int i = 0; i < 4; ++i)
	{
		m_pBattleFormationPanel->Get_ChildByIndex(0)->Get_ChildByIndex(8 + i)->Set_Active(FALSE);
	}
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Active(TRUE);
	m_pBattleFormationPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Set_Active(FALSE);
	
}

#pragma endregion

#pragma region �ӹ� & ����

void CUI_Manager::Create_Ui_WorkPanel()
{
	m_pWorkPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]����â_�θ�.uidat");
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pWorkPanel->Get_ObjTag(), m_pWorkPanel);
	m_pWorkPanel->Set_Active(FALSE);
}

void CUI_Manager::Create_Ui_MissionPanel()
{
	// �̼� â Load
	m_pMissionPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]������������_�θ�.uidat");
	m_pMissionPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pMissionPanel->Get_ObjTag(), m_pMissionPanel);

	m_pMission_Thumnail = m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1);
	m_pMission_Normal_Scroll = m_pMissionPanel->Get_ChildByIndex(1)->Get_ChildByIndex(2);
	m_pMission_Hard_Scroll = m_pMissionPanel->Get_ChildByIndex(1)->Get_ChildByIndex(3);

	// �ӹ����� â Load 
	m_pMissionInfoBackground = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�ӹ�����_�θ�.uidat");
	m_pMissionInfoBackground->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pMissionInfoBackground->Get_ObjTag(), m_pMissionInfoBackground);
	m_pMissionInfoPanel = m_pMissionInfoBackground->Get_ChildByIndex(0);

	m_pMissionInfoPanel->Get_ChildByIndex(6)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Mission_ClickEnemyInfo, this));
	m_pMissionInfoPanel->Get_ChildByIndex(8)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Mission_Start, this));


	m_pTileMissionInfoPanel				= Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Tile_�ӹ�â_�θ�.uidat");
	m_pTileMissionInfoPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pTileMissionInfoPanel->Get_ObjTag(), m_pTileMissionInfoPanel);
	m_pTileMissionInfo_StageNumber		= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	m_pTileMissionInfo_StageName		= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1);
	m_pTileMissionInfo_RecommentLevel	= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3);
	m_pTileMissionInfo_MapType			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4);
	m_pTileMissionInfo_EnemyInfoButton	= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(6);
	m_pTileMissionInfo_EnemyInfoButton->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::TileMission_ClickEnemyInfo, this));
	m_pTileMissionInfo_Item1			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(13);
	m_pTileMissionInfo_Item2			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(14);
	m_pTileMissionInfo_Item3			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(15);
	m_pTileMissionInfo_Item4			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(16);
	m_pTileMissionInfo_ClearPanel		= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(17);
	m_pTileMissionInfo_Start			= m_pTileMissionInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(18);
	map<E_WORLD_MOD, list<CWorldCreator*>>* pWorlds = CGameManager::GetInstance()->Get_Worlds();
	for (auto& iter : (*pWorlds))
	{
		_int iCount = 0;
		for (auto& iterWorld : iter.second)
		{

			CUI_Object* pStage = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�������� Prefab.uidat");
			pStage->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(iCount));
			pStage->Get_ChildByIndex(4)->Set_Ui_Text(iterWorld->Get_WorldName());
			pStage->Get_ChildByIndex(5)->Add_Function_voidPtr(E_UiFunctionType::OnClick, bind(&CUI_Manager::Mission_GoInside, this, iterWorld), iterWorld);

			m_pMission_Normal_Scroll->Set_Child(pStage);
			m_vecMissionTypes[0].push_back(make_pair(iter.first, pStage));
			iCount++;
		}
	}
	
	vector<wstring> vecTileFiles;
	_finddata_t findData;
	intptr_t lHandle = 0;
	intptr_t lResult = 0;
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Data/Data_Tile/", "*.*", &findData);
	while (-1 != lResult)
	{
		wchar_t szName[MAX_PATH] = L"";

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);

		if (strcmp(".", findData.name) && strcmp("..", findData.name) && strcmp("desktop.ini", findData.name))
		{
			vecTileFiles.push_back(szName);
		}
		lResult = _findnext(lHandle, &findData);
	}

	_findclose(lHandle);

	_int iCount = 0;
	for (_int i = 0; i < (_int)vecTileFiles.size(); ++i)
	{
		CUI_Object* pStage = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�������� Prefab.uidat");
		pStage->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(iCount));
		pStage->Get_ChildByIndex(4)->Set_Ui_Text(vecTileFiles[i]);
		pStage->Get_ChildByIndex(5)->Add_Function_wstring(E_UiFunctionType::OnClick, bind(&CUI_Manager::TileMission_GoInside, this, vecTileFiles[i]), vecTileFiles[i]);

		m_pMission_Hard_Scroll->Set_Child(pStage);
		m_vecMissionTypes[1].push_back(make_pair(E_WORLD_MOD::MOD_MISSION, pStage));
		iCount++;
	}
}

void CUI_Manager::Create_Ui_EnemyInfoPanel()
{
	m_pEnemyInfoPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]������_�г�.uidat");
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pEnemyInfoPanel->Get_ObjTag(), m_pEnemyInfoPanel);
	m_pEnemyInfoPanel->Set_Active(FALSE);

	m_pEnenyInfo_Scroll = m_pEnemyInfoPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2);
}

void CUI_Manager::Work_OnEnable()
{
	m_pWorkPanel->Set_Active(TRUE);
	m_pHighInfoPanel->Set_Active(TRUE);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����.mp3", 0.1f);
	// �ӹ�
	m_pWorkPanel->Get_ChildByIndex(0)->Clear_Function(E_UiFunctionType::OnClick);
	m_pWorkPanel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Mission_OnEnable, this));

	// ���丮
	m_pWorkPanel->Get_ChildByIndex(1)->Clear_Function(E_UiFunctionType::OnClick);
	m_pWorkPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Story_OnEnable, this));

	// ���� ����
	m_pWorkPanel->Get_ChildByIndex(2)->Clear_Function(E_UiFunctionType::OnClick);
	m_pWorkPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::MissionWanted_OnEnable, this));

	// Ư�� �Ƿ�
	m_pWorkPanel->Get_ChildByIndex(3)->Clear_Function(E_UiFunctionType::OnClick);
	m_pWorkPanel->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::MissionSpecial_OnEnable, this));

	// �ѷ���
	m_pWorkPanel->Get_ChildByIndex(4)->Clear_Function(E_UiFunctionType::OnClick);
	m_pWorkPanel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalPanel, this));

	m_pHighPanel_PanelName->Set_Ui_Text(L"����");
}

void CUI_Manager::Mission_OnEnable()
{
	m_pMissionPanel->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Growth_Bg.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(L"�ӹ�");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Work/Information_CharacterDetail_03_Kr.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(FALSE);
	Load_Mission(E_WORLD_MOD::MOD_MISSION);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�ӹ���������.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"�ӹ�");
}

void CUI_Manager::MissionWanted_OnEnable()
{
	m_pMissionPanel->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_ComputerCenter.jpg");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(L"���� ����");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Work/�������.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Ui_Text(L"ī���������� �����ľ��մϴ�.");
	Load_Mission(E_WORLD_MOD::MOD_WANTED);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�������&Ư���ӹ�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"�������");
}

void CUI_Manager::MissionSpecial_OnEnable()
{
	m_pMissionPanel->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/BG_ComputerCenter.jpg");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(L"Ư�� �ӹ�");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Work/���� ���.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Ui_Text(L"�뷮���� ������� ������ ���Ƴ��� �մϴ�.\n�ټ��� ���� ����� �� �ִ� ������ �ʿ��մϴ�.");
	Load_Mission(E_WORLD_MOD::MOD_SPECIAL);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�������&Ư���ӹ�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"Ư���ӹ�");
}

void CUI_Manager::MissionTotal_OnEnable()
{
	m_pMissionPanel->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Raid_Bg.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(L"�ѷ���");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Work/Boss_Portrait_Binah_LobbyBG.png");
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Active(FALSE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Active(TRUE);
	m_pMissionPanel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Set_Ui_Text(L"������ �������� ����ġ�� �ٸ� ������� ������ ���� �����ϼ���");
	Load_Mission(E_WORLD_MOD::MOD_TOTAL);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�ѷ����޴�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"�ѷ���");
	
}

void CUI_Manager::Load_Mission(E_WORLD_MOD eMod)
{
	m_pMissionPanel->Set_Active(TRUE);
	m_pHighInfoPanel->Set_Active(TRUE);


	for (auto& iter : m_vecMissionTypes[0])
	{
		iter.second->Set_Active(iter.first == eMod);
	}
	for (auto& iter : m_vecMissionTypes[1])
	{
		iter.second->Set_Active(iter.first == eMod);
	}

	m_pWorkPanel->Set_Active(FALSE);
	return;
}

void CUI_Manager::Mission_GoInside(void* pArg)
{
	CWorldCreator* pWorld = static_cast<CWorldCreator*>(pArg);
	CGameManager::GetInstance()->Set_NextWorld(static_cast<CWorldCreator*>(pArg));
	m_pMissionInfoBackground->Set_Active(TRUE);

	// ������ �̸��� ǥ��
	m_pMissionInfoPanel->Get_ChildByIndex(2)->Set_Ui_Text(L"Sub    " + pWorld->Get_WorldName());

	// ���� Ǯ�� ù��° ������ ������ ǥ��
	m_pMissionInfoPanel->Get_ChildByIndex(3)->Set_Ui_Text(L"Lv." + to_wstring(pWorld->Get_HighLevelMonster()));

	switch (pWorld->Get_WorldType())
	{
	case Engine::E_WORLD_TYPE::TYPE_STREET:
		m_pMissionInfoPanel->Get_ChildByIndex(4)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_OutDoor.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_FIELD:
		m_pMissionInfoPanel->Get_ChildByIndex(4)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InCity.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_INDOOR:
		m_pMissionInfoPanel->Get_ChildByIndex(4)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InDoor.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_END:
		m_pMissionInfoPanel->Get_ChildByIndex(4)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/X������.png");
		break;
	}

}

void CUI_Manager::Mission_Start()
{
	m_pFunc_CombatScene(E_SCENETAG::Scene_Combat);
}

void CUI_Manager::Mission_ClickEnemyInfo()
{
	m_pEnemyInfoPanel->Set_Active(TRUE);

	CWorldCreator* pNextWorld = CGameManager::GetInstance()->Get_NextWorld();

	vector<T_MONSTER_INFO>& pVecMonInfo = pNextWorld->Get_MonsterInfo();

	_int iMonSize = (_int)pVecMonInfo.size();
	_int iScrollChildSize = (_int)m_pEnenyInfo_Scroll->Get_ChildList()->size();

	// Monster������ ǥ���� UI ������ �����ϸ� ���� �����ϰ� �ڽ����� �־��ش�.
	for (_int i = 0; i < iMonSize - iScrollChildSize; i++)
	{
		CUI_Object* pEnemyInfoPrefab = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]������_Prefab.uidat");
		m_pEnenyInfo_Scroll->Set_Child(pEnemyInfoPrefab);
	}

	iScrollChildSize = (_int)m_pEnenyInfo_Scroll->Get_ChildList()->size();
	for (_int i = 0; i < iScrollChildSize; ++i)
	{
		CUI_Object* pInfo = m_pEnenyInfo_Scroll->Get_ChildByIndex(i);
		if (i >= iMonSize)
		{
			pInfo->Set_Active(FALSE);
			continue;
		}

		T_MONSTER_INFO& tInfo = pVecMonInfo[i];
		wstring wstrImageName;
		wstrImageName = L"../../Reference/Resource/Texture/UI/EnemyInfo/";
		wstrImageName.append(tInfo.wszName);
		wstrImageName.append(L".png");
		pInfo->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Square_White.png");
		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrImageName);
		if (tInfo.wszDisplayName != L"")
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(tInfo.wszDisplayName);
		}
		else
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(L"������ �̸� ����");
		}

		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(TRUE);
		switch (tInfo.eMonsterType)
		{
		case Engine::E_MONSTER_TYPE::TYPE_NORMAL:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		case Engine::E_MONSTER_TYPE::TYPE_ELITE:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/����Ʈ������.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_BOSS:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/����������.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_END:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		}
		pInfo->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eAtkType, E_CHAR_ENUMTYPE::CNUM_ATKTYPE));
		pInfo->Get_ChildByIndex(2)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eDiffType, E_CHAR_ENUMTYPE::CNUM_DFFTYPE));

	}
}

void CUI_Manager::TileMission_GoInside(wstring wstrArg)
{
	CGameManager::GetInstance()->Set_TileField(CTileField::Create(wstrArg));
	m_pMissionPanel->Set_Active(TRUE);
	m_pHighInfoPanel->Set_Active(TRUE);
	m_pTileMissionInfoPanel->Set_Active(TRUE);
	CTileField* pTileField = CGameManager::GetInstance()->Get_TileField();
	m_pTileMissionInfo_Start->Clear_Function(E_UiFunctionType::OnClick);
	m_pTileMissionInfo_Start->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::TileMission_Start, this));
	

	// ������ �̸��� ǥ��
	_int iIndex = (_int)wstrArg.find(L".tile");
	wstrArg = wstrArg.substr(0, iIndex);
	m_pTileMissionInfo_StageName->Set_Ui_Text(wstrArg);
	
	for (auto& iter : pTileField->Get_MonsterMascot())
	{
		if (iter->Get_Boss())
		{
			// ������ ���� 
			m_pTileMissionInfo_RecommentLevel->Set_Ui_Text(L"Lv." + to_wstring(iter->Get_WorldCreator()->Get_HighLevelMonster()));
		}
	}
	
	switch (pTileField->Get_MonsterMascot()[0]->Get_WorldCreator()->Get_WorldType())
	{
	case Engine::E_WORLD_TYPE::TYPE_STREET:
		m_pTileMissionInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_OutDoor.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_FIELD:
		m_pTileMissionInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InCity.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_INDOOR:
		m_pTileMissionInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InDoor.png");
		break;
	case Engine::E_WORLD_TYPE::TYPE_END:
		m_pTileMissionInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/X������.png");
		break;
	}

	//m_pTileMissionInfo_StageNumber	= m_pTileMissionInfoPanel->Get_ChildByIndex(0);
	//m_pTileMissionInfo_StageName		= m_pTileMissionInfoPanel->Get_ChildByIndex(1);
	//m_pTileMissionInfo_RecommentLevel	= m_pTileMissionInfoPanel->Get_ChildByIndex(3);
	m_pTileMissionInfo_Item1->Get_ChildByIndex(1)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(0)->wstrImagePath);
	m_pTileMissionInfo_Item2->Get_ChildByIndex(1)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(1)->wstrImagePath);
	m_pTileMissionInfo_Item3->Get_ChildByIndex(1)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(2)->wstrImagePath);
	m_pTileMissionInfo_Item4->Get_ChildByIndex(1)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Item(3)->wstrImagePath);
	//m_pTileMissionInfo_ClearPanel		= m_pTileMissionInfoPanel->Get_ChildByIndex(17);

}

void CUI_Manager::TileMission_Start()
{
	m_pFunc_TileScene(E_SCENETAG::Scene_TileField);
	m_pLobbyPanel->Set_Active(FALSE);
	m_pMissionPanel->Set_Active(FALSE);
	m_pTileMissionInfoPanel->Set_Active(FALSE);
	m_pCombatVictoryPanel->Set_Active(FALSE);
}

void CUI_Manager::TileMission_ClickEnemyInfo()
{
	m_pEnemyInfoPanel->Set_Active(TRUE);

	CTileField* pTileField = CGameManager::GetInstance()->Get_TileField();

	const vector<T_MONSTER_INFO>& pVecMonInfo = pTileField->Get_MonsterPoolInfo();

	_int iMonSize = (_int)pVecMonInfo.size();
	_int iScrollChildSize = (_int)m_pEnenyInfo_Scroll->Get_ChildList()->size();

	// Monster������ ǥ���� UI ������ �����ϸ� ���� �����ϰ� �ڽ����� �־��ش�.
	for (_int i = 0; i < iMonSize - iScrollChildSize; i++)
	{
		CUI_Object* pEnemyInfoPrefab = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]������_Prefab.uidat");
		m_pEnenyInfo_Scroll->Set_Child(pEnemyInfoPrefab);
	}

	iScrollChildSize = (_int)m_pEnenyInfo_Scroll->Get_ChildList()->size();
	for (_int i = 0; i < iScrollChildSize; ++i)
	{
		CUI_Object* pInfo = m_pEnenyInfo_Scroll->Get_ChildByIndex(i);
		if (i >= iMonSize)
		{
			pInfo->Set_Active(FALSE);
			continue;
		}

		const T_MONSTER_INFO& tInfo = pVecMonInfo[i];
		wstring wstrImageName;
		wstrImageName = L"../../Reference/Resource/Texture/UI/EnemyInfo/";
		wstrImageName.append(tInfo.wszName);
		wstrImageName.append(L".png");
		pInfo->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Square_White.png");
		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrImageName);
		if (tInfo.wszDisplayName != L"")
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(tInfo.wszDisplayName);
		}
		else
		{
			pInfo->Get_ChildByIndex(0)->Set_Ui_Text(L"������ �̸� ����");
		}

		pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(TRUE);
		switch (tInfo.eMonsterType)
		{
		case Engine::E_MONSTER_TYPE::TYPE_NORMAL:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		case Engine::E_MONSTER_TYPE::TYPE_ELITE:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/����Ʈ������.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_BOSS:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/����������.png");
			break;
		case Engine::E_MONSTER_TYPE::TYPE_END:
			pInfo->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Active(FALSE);
			break;
		}
		pInfo->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eAtkType, E_CHAR_ENUMTYPE::CNUM_ATKTYPE));
		pInfo->Get_ChildByIndex(2)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)tInfo.eDiffType, E_CHAR_ENUMTYPE::CNUM_DFFTYPE));
	}
}


#pragma endregion

#pragma region ���׸���(ī��)

void CUI_Manager::Create_Ui_TotalPanel()
{
	if (m_pTotal_Panel)
	{
		return;
	}


	m_pTotal_Panel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�ѷ�����������_�θ�.uidat");
	m_pTotal_Panel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pTotal_Panel->Get_ObjTag(), m_pTotal_Panel);

	m_pTotal_Frame_Binah		= m_pTotal_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(0);
	m_pTotal_Frame_Binah->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalInfoPanel, this, ETotalPanel_BossType::E_TotalPanel_Binah), ETotalPanel_BossType::E_TotalPanel_Binah);
	m_pTotal_Frame_Hieronymus	= m_pTotal_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(1);
	m_pTotal_Frame_Hieronymus->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalInfoPanel, this, ETotalPanel_BossType::E_TotalPanel_Hieronymus), ETotalPanel_BossType::E_TotalPanel_Hieronymus);
	m_pTotal_Frame_Chesed		= m_pTotal_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(2);
	m_pTotal_Frame_Chesed->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalInfoPanel, this, ETotalPanel_BossType::E_TotalPanel_Chesed), ETotalPanel_BossType::E_TotalPanel_Chesed);
	m_pTotal_Frame_Kuro			= m_pTotal_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(3);
	m_pTotal_Frame_Kuro->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalInfoPanel, this, ETotalPanel_BossType::E_TotalPanel_Kuro), ETotalPanel_BossType::E_TotalPanel_Kuro);
	
	m_pTotal_Bottom_Ranking		= m_pTotal_Panel->Get_ChildByIndex(2);

	m_pTotalInfo_Panel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�ѷ���������������_�θ�.uidat");
	m_pTotalInfo_Panel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pTotalInfo_Panel->Get_ObjTag(), m_pTotalInfo_Panel);

	m_pTotalInfo_Background					= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	m_pTotalInfo_Bossillust					= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(1);
	m_pTotalInfo_BossName					= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(2);
	m_pTotalInfo_HealthText_OutLine			= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(3);
	m_pTotalInfo_MapType					= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(4);
	m_pTotalInfo_Recommand_Level_OutLine	= m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(5);

	m_pTotalInfo_MissionStartButton = m_pTotalInfo_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(4);
	m_pTotalInfo_MissionStartButton->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_TotalMission_Start, this));

	m_pTotalInfo_Frame_BossSkill_1 = m_pTotalInfo_Panel->Get_ChildByIndex(1);
	m_pTotalInfo_Frame_BossSkill_2 = m_pTotalInfo_Panel->Get_ChildByIndex(2);
	m_pTotalInfo_Frame_BossSkill_3 = m_pTotalInfo_Panel->Get_ChildByIndex(3);
	m_pTotalInfo_Frame_BossSkill_4 = m_pTotalInfo_Panel->Get_ChildByIndex(4);
	m_pTotalInfo_Frame_BossSkill_5 = m_pTotalInfo_Panel->Get_ChildByIndex(5);

}

void CUI_Manager::OnClick_TotalPanel()
{
	m_pTotal_Panel->Set_Active(TRUE);
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_�ѷ����޴�.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"�ѷ���");
}

void CUI_Manager::OnClick_TotalInfoPanel(_int iBossType)
{
	map<E_WORLD_MOD, list<CWorldCreator*>>* pWorlds = CGameManager::GetInstance()->Get_Worlds();
	list<CWorldCreator*>& pTotalList = (*pWorlds)[E_WORLD_MOD::MOD_TOTAL];

	_int iBossLevel = 100;

	switch (iBossType)
	{
	case CUI_Manager::E_TotalPanel_Binah:
		for (auto& iter : pTotalList)
		{
			if (iter->Get_WorldName() == L"Binah.world")
			{
				CGameManager::GetInstance()->Set_NextWorld(iter);
				iBossLevel = iter->Get_HighLevelMonster();
			}
		}

		m_pTotalInfo_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Binah_LobbyBG.png");
		m_pTotalInfo_Bossillust->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Binah_Lobby.png");
		m_pTotalInfo_BossName->Set_Ui_Text(L"��");
		for (_int i = 0; i < 5; ++i)
		{
			m_pTotalInfo_HealthText_OutLine->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(L"UIMgr ����ü�� �Է� �ʿ�");
		}

		// ��ų 5�� ����
		m_pTotalInfo_Frame_BossSkill_1->Set_Ui_Text(L"���1");
		m_pTotalInfo_Frame_BossSkill_1->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Binah_ExSkill.png");

		m_pTotalInfo_Frame_BossSkill_2->Set_Ui_Text(L"���2");
		m_pTotalInfo_Frame_BossSkill_2->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Binah_ExSkill_1.png");

		m_pTotalInfo_Frame_BossSkill_3->Set_Ui_Text(L"���3");
		m_pTotalInfo_Frame_BossSkill_3->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Binah_ExSkill_2.png");

		m_pTotalInfo_Frame_BossSkill_4->Set_Ui_Text(L"���4");
		m_pTotalInfo_Frame_BossSkill_4->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Binah_ExSkill_Empty.png");

		m_pTotalInfo_Frame_BossSkill_5->Set_Ui_Text(L"���5");
		m_pTotalInfo_Frame_BossSkill_5->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/BINAH_NORMALSKILL.png");

		break;
	case CUI_Manager::E_TotalPanel_Hieronymus:
		for (auto& iter : pTotalList)
		{
			if (iter->Get_WorldName() == L"�����δϹ���.world")
			{
				CGameManager::GetInstance()->Set_NextWorld(iter);
			}
		}

		m_pTotalInfo_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Hieronymus_LobbyBG_Street.png");
		m_pTotalInfo_Bossillust->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Hieronymus_Lobby.png");
		m_pTotalInfo_BossName->Set_Ui_Text(L"�����δϹ���");
		for (_int i = 0; i < 5; ++i)
		{
			m_pTotalInfo_HealthText_OutLine->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(L"UIMgr ����ü�� �Է� �ʿ�");
		}

		// ��ų 5�� ����
		m_pTotalInfo_Frame_BossSkill_1->Set_Ui_Text(L"���1");
		m_pTotalInfo_Frame_BossSkill_1->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Hieronymus_ExSkill.png");

		m_pTotalInfo_Frame_BossSkill_2->Set_Ui_Text(L"���2");
		m_pTotalInfo_Frame_BossSkill_2->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Hieronymus_ExSkill_1.png");

		m_pTotalInfo_Frame_BossSkill_3->Set_Ui_Text(L"���3");
		m_pTotalInfo_Frame_BossSkill_3->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Hieronymus_ExSkill_2.png");

		m_pTotalInfo_Frame_BossSkill_4->Set_Ui_Text(L"���4");
		m_pTotalInfo_Frame_BossSkill_4->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Hieronymus_ExSkill_3.png");

		m_pTotalInfo_Frame_BossSkill_5->Set_Ui_Text(L"���5");
		m_pTotalInfo_Frame_BossSkill_5->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Hieronymus_ExSkill_4.png");

		break;
	case CUI_Manager::E_TotalPanel_Chesed:

		m_pTotalInfo_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Chesed_LobbyBG.png");
		m_pTotalInfo_Bossillust->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Chesed_Lobby.png");
		m_pTotalInfo_BossName->Set_Ui_Text(L"�켼��");
		for (_int i = 0; i < 5; ++i)
		{
			m_pTotalInfo_HealthText_OutLine->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(L"UIMgr ����ü�� �Է� �ʿ�");
		}

		// ��ų 5�� ����
		m_pTotalInfo_Frame_BossSkill_1->Set_Ui_Text(L"���1");
		m_pTotalInfo_Frame_BossSkill_1->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Chesed_ExSkill.png");

		m_pTotalInfo_Frame_BossSkill_2->Set_Ui_Text(L"���2");
		m_pTotalInfo_Frame_BossSkill_2->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Chesed_ExSkill_1.png");

		m_pTotalInfo_Frame_BossSkill_3->Set_Ui_Text(L"���3");
		m_pTotalInfo_Frame_BossSkill_3->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Chesed_ExSkill_2.png");

		m_pTotalInfo_Frame_BossSkill_4->Set_Ui_Text(L"���4");
		m_pTotalInfo_Frame_BossSkill_4->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Chesed_PassiveSkill.png");

		m_pTotalInfo_Frame_BossSkill_5->Set_Ui_Text(L"���5");
		m_pTotalInfo_Frame_BossSkill_5->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/CHILL.png");

		break;
	case CUI_Manager::E_TotalPanel_Kuro:
		for (auto& iter : pTotalList)
		{
			if (iter->Get_WorldName() == L"���.world")
			{
				CGameManager::GetInstance()->Set_NextWorld(iter);
			}
		}

		m_pTotalInfo_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Shirokuro_LobbyBG.png");
		m_pTotalInfo_Bossillust->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Boss_Portrait/Boss_Portrait_Shirokuro_Lobby.png");
		m_pTotalInfo_BossName->Set_Ui_Text(L"���");
		for (_int i = 0; i < 5; ++i)
		{
			m_pTotalInfo_HealthText_OutLine->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(L"UIMgr ����ü�� �Է� �ʿ�");
		}

		// ��ų 5�� ����
		m_pTotalInfo_Frame_BossSkill_1->Set_Ui_Text(L"���1");
		m_pTotalInfo_Frame_BossSkill_1->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Shirokuro_ExSkill.png");

		m_pTotalInfo_Frame_BossSkill_2->Set_Ui_Text(L"���2");
		m_pTotalInfo_Frame_BossSkill_2->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Shirokuro_ExSkill_1.png");

		m_pTotalInfo_Frame_BossSkill_3->Set_Ui_Text(L"���3");
		m_pTotalInfo_Frame_BossSkill_3->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Shirokuro_ExSkill_2.png");

		m_pTotalInfo_Frame_BossSkill_4->Set_Ui_Text(L"���4");
		m_pTotalInfo_Frame_BossSkill_4->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/Shirokuro_ExSkill_3.png");

		m_pTotalInfo_Frame_BossSkill_5->Set_Ui_Text(L"���5");
		m_pTotalInfo_Frame_BossSkill_5->Get_ChildByIndex(1)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Skill/STATBUFF.png");
		break;
	}

	for (_int i = 0; i < 5; ++i)
	{
		m_pTotalInfo_Recommand_Level_OutLine->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(L"Lv." + to_wstring(iBossLevel));
	}

	// �ɼ��� �ƴ� ���
	if (iBossType != CUI_Manager::E_TotalPanel_Chesed)
	{
		switch (CGameManager::GetInstance()->Get_NextWorld()->Get_WorldType())
		{
		case Engine::E_WORLD_TYPE::TYPE_STREET:
			m_pTotalInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_OutDoor.png");
			break;
		case Engine::E_WORLD_TYPE::TYPE_FIELD:
			m_pTotalInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InCity.png");
			break;
		case Engine::E_WORLD_TYPE::TYPE_INDOOR:
			m_pTotalInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InDoor.png");
			break;
		case Engine::E_WORLD_TYPE::TYPE_END:
			m_pTotalInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/X������.png");
			break;
		}
	}
	else		// �ɼ���
	{
		m_pTotalInfo_MapType->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Icon_InDoor.png");
	}
	

	m_eLastBossType = (ETotalPanel_BossType)iBossType;

	m_pTotalInfo_Panel->Set_Active(TRUE);
}

void CUI_Manager::OnClick_TotalMission_Start()
{
	switch (m_eLastBossType)
	{
	case CUI_Manager::E_TotalPanel_Binah:
	case CUI_Manager::E_TotalPanel_Hieronymus:
	case CUI_Manager::E_TotalPanel_Kuro:
		Mission_Start();
		break;
	case CUI_Manager::E_TotalPanel_Chesed:
		// ���� ������ �� ���� �ʿ�
		m_pTotalInfo_Panel->Set_Active(FALSE);
		m_pTotal_Panel->Set_Active(FALSE);
		m_pWorkPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(FALSE);
		m_pLobbyPanel->Set_Active(FALSE);
		m_pFunc_RunGameScene(E_SCENETAG::Scene_RunTest);
		return;
		break;
	}
}

void CUI_Manager::Scene_Change_Cafe()
{
	CUtility::Print_Wstring_DMode(L"ī�� �̵� = F4");

	m_pFunc_CafeScene(E_SCENETAG::Scene_Cafe);
}

void CUI_Manager::Create_Ui_Cafe()
{
	//�ֻ��� �θ�
	if (nullptr == m_pBestCafePanel)
	{
		m_pBestCafePanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]ī��_Fixing.uidat");
		m_pBestCafePanel->Set_Active(FALSE);
		CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pBestCafePanel->Get_ObjTag(), m_pBestCafePanel);
		m_pBestCafePanel->Set_NameTag(L"ī��_UI");

		//�ֻ��� �θ� �ڽĵ�
		m_pParentLeftPanel = m_pBestCafePanel->Get_ChildByIndex(0);
		m_pParentRightPanel = m_pBestCafePanel->Get_ChildByIndex(1);
		m_pParentTopPanel = m_pBestCafePanel->Get_ChildByIndex(2);

		//�θ�_���� ���̾��� �ڽĵ�
		m_pLeftLayerPanel = m_pParentLeftPanel->Get_ChildByIndex(0);
		m_pLeftLayerHidePanel = m_pParentLeftPanel->Get_ChildByIndex(1);
		m_pFixMode1Panel = m_pParentLeftPanel->Get_ChildByIndex(2);
		m_pFixMode2Panel = m_pParentLeftPanel->Get_ChildByIndex(3);
		m_pFurInfoPanel = m_pParentLeftPanel->Get_ChildByIndex(4);
		m_pGiftInvenPanel = m_pParentLeftPanel->Get_ChildByIndex(5);
		m_pFurInvenPanel = m_pParentLeftPanel->Get_ChildByIndex(6);
		m_pFurInvenInfoPanel = m_pParentLeftPanel->Get_ChildByIndex(7);
		m_pFurLineUpPanel = m_pParentLeftPanel->Get_ChildByIndex(8);
		m_pPresetPanel = m_pParentLeftPanel->Get_ChildByIndex(9);

		//�θ�_�����ʷ��̾��� �ڽĵ�
		m_pButtonInvPanel = m_pParentRightPanel->Get_ChildByIndex(0);
		m_pInvTimePanel = m_pParentRightPanel->Get_ChildByIndex(1);
		m_pInvRejectPanel = m_pParentRightPanel->Get_ChildByIndex(2);
		m_pInvInfoPanel = m_pParentRightPanel->Get_ChildByIndex(3);
		m_pInvLineUpPanel = m_pParentRightPanel->Get_ChildByIndex(4);
		m_pInvitingPanel = m_pParentRightPanel->Get_ChildByIndex(5);
		m_pRightLayerPanel = m_pParentRightPanel->Get_ChildByIndex(6);
		m_pCafeInfoPanel = m_pParentRightPanel->Get_ChildByIndex(7);
		m_pCafeProfitPanel = m_pParentRightPanel->Get_ChildByIndex(8);
		m_pCafeProfitStatusPanel = m_pParentRightPanel->Get_ChildByIndex(9);
		m_pCafeProfitStatusFPanel = m_pParentRightPanel->Get_ChildByIndex(10);

		//�θ�_��� ���̾��� �ڽĵ�
		m_pProfitInvTimePanel = m_pParentTopPanel->Get_ChildByIndex(0);
		m_pHideParentPanel = m_pParentTopPanel->Get_ChildByIndex(1);
		m_pOpenParentPanel = m_pParentTopPanel->Get_ChildByIndex(2);

		for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
		{
			m_pUi_Furniture = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]ī��_���� ��ũ�Ѻ�.uidat");

			switch (CGameManager::GetInstance()->Get_Furniture(i)->eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = CGameManager::GetInstance()->Get_Furniture(i)->wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);
			m_pUi_Furniture->Set_Ui_Text(wstrFullText);

			m_pUi_Furniture->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Furniture(i)->wstrTexturePath);
			m_pBestCafePanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Set_Child(m_pUi_Furniture);

			m_pUi_Furniture->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Info, this, i), i);
			m_pUi_Furniture->Get_ChildByIndex(2)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Create, this, i), i);

			m_pFixMode1Panel->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_SelInfo, this, i), i);

			m_pUi_Furniture->Set_Active(TRUE);
			m_vecUi_Furniture.emplace_back(m_pUi_Furniture);
		}
		m_pFurLineUpPanel->Get_ChildByIndex(1)->Set_Active(false);
		m_pFurLineUpPanel->Get_ChildByIndex(5)->Set_Active(true);
		m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Ui_Text(L"���� ����");

		Cafe_Ui_Event();
	}
	else
	{
		if (m_pLobbyPanel != nullptr)
		{
			m_pLobbyPanel->Set_Active(FALSE);
		}
		m_pBestCafePanel->Set_Active(TRUE);
		m_pHighInfoPanel->Set_Active(TRUE);
		m_pHighPanel_PanelName->Set_Ui_Text(L"ī��");
	}

}

void CUI_Manager::Cafe_Ui_Event()
{
	//Click Event
	m_pLeftLayerPanel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FixMode, this));
	m_pFixMode2Panel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Rotation, this));
	m_pFixMode1Panel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Storage, this));
	m_pLeftLayerPanel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_AllStorage, this));
	m_pFurLineUpPanel->Get_ChildByIndex(6)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_LineUp, this));
	m_pParentRightPanel->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_InfoOpen, this));
	m_pCafeInfoPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_InfoExit, this));
	m_pParentRightPanel->Get_ChildByIndex(8)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Profit, this));
	m_pPresetPanel->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName, this));
	m_pPresetPanel->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetNameEnd, this));
	m_pPresetPanel->Get_ChildByIndex(2)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetNameEnd, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName1, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName2, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName3, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName4, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetName5, this));
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::FurnitureSort_Descending, this));
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::FurnitureSort_Ascending, this));
	m_pFurLineUpPanel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::FurnitureSort_AutoSetting, this));
	m_pFurLineUpPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::FurnitureSort_AutoSetting, this));

	//Asort
	m_pLeftLayerPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OpenInven_AutoSetting, this));
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Furniture, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Furniture, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_Table, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_Closet, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_Chair, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_Bed, this));
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_EtcFur, this));
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Deco, this));
	m_pFurInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Deco, this));
	m_pFurInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_Props, this));
	m_pFurInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_WallDeco, this));
	m_pFurInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_FloorDeco, this));
	m_pFurInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_EtcDeco, this));
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(6)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Indoor, this));
	m_pFurInvenPanel->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_Indoor, this));
	m_pFurInvenPanel->Get_ChildByIndex(5)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_FloorDeco, this));
	m_pFurInvenPanel->Get_ChildByIndex(5)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_FilterTag_WallDeco, this));

	//Preset
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetInit1, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetSave1, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetApply1, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetInit2, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetSave2, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetApply2, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetInit3, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetSave3, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetApply3, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetInit4, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetSave4, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetApply4, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetInit5, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetSave5, this));
	m_pPresetPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_PresetApply5, this));

	//Gift
	m_pGiftInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gift_1, this));
	m_pGiftInvenPanel->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gift_2, this));
	m_pGiftInvenPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gift_3, this));
	m_pGiftInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gift_4, this));
	m_pGiftInvenPanel->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gift_5, this));

	//All Storage
	m_pLeftLayerPanel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::AllStorage, this));

	//Exception
	m_pFurInvenInfoPanel->Add_Function_void(E_UiFunctionType::OnEnable, bind(&CUI_Manager::Cafe_Ui_Exception, this));
	m_pFurInvenInfoPanel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Cafe_Ui_Exception, this));
	m_pFurLineUpPanel->Add_Function_void(E_UiFunctionType::OnEnable, bind(&CUI_Manager::Cafe_Ui_Exception, this));
	m_pFurLineUpPanel->Get_ChildByIndex(6)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Cafe_Ui_Exception, this));
	m_pFurLineUpPanel->Get_ChildByIndex(7)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Cafe_Ui_Exception, this));

	//Rest Sound
	m_pOpenParentPanel->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pHideParentPanel->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pCafeProfitPanel->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pLeftLayerHidePanel->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pLeftLayerPanel->Get_ChildByIndex(5)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pFixMode1Panel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pFixMode1Panel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pFixMode1Panel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pFurInfoPanel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pFixMode2Panel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pFixMode2Panel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pFixMode2Panel->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pCafeProfitPanel->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pCafeInfoPanel->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pCafeInfoPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pLeftLayerPanel->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pPresetPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pParentRightPanel->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pParentRightPanel->Get_ChildByIndex(7)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pParentRightPanel->Get_ChildByIndex(7)->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pParentRightPanel->Get_ChildByIndex(8)->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pFurInvenPanel->Get_ChildByIndex(2)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Back, this));
	m_pGiftInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
	m_pLeftLayerPanel->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Sound_Click, this));
}

void CUI_Manager::Cafe_Ui_Exception()
{
	if (m_pFurInvenInfoPanel->Get_Active() ||
		m_pFurLineUpPanel->Get_Active()
		)
	{
		m_pBestCafePanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Set_ChildClickCheck(false);
	}
	else
	{
		m_pBestCafePanel->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Set_ChildClickCheck(true);
	}
}

void CUI_Manager::Furniture_FixMode()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);
	m_pFixMode1Panel->Set_Active(false);
	m_pFunc_PickFurniture(true);
}

void CUI_Manager::Furniture_Rotation()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);
	m_pFunc_RotFurniture(true);
}

void CUI_Manager::Furniture_Storage()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_DeleteFur.wav", 0.3f);
	m_pFunc_StoFurniture(true);
}

void CUI_Manager::Furniture_AllStorage()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_DeleteFur.wav", 0.3f);
	m_pFunc_AllStoFurniture(true);
}

void CUI_Manager::Furniture_Info(_int iFur)
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 0.3f);

	if (!m_pFurLineUpPanel->Get_ChildByIndex(1)->Get_Active())
	{
		wstring wstrFullText;

		m_pFurInvenInfoPanel->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Furniture(iFur)->wstrTexturePath);
		m_pFurInvenInfoPanel->Get_ChildByIndex(1)->Set_Ui_Text(CGameManager::GetInstance()->Get_Furniture(iFur)->wstrName);
		m_pFurInvenInfoPanel->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Furniture(iFur)->iAmenity));

		wstrFullText = CGameManager::GetInstance()->Get_Furniture(iFur)->wstrDescription;
		wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
		m_pFurInvenInfoPanel->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
	}
}

void CUI_Manager::Furniture_Create(_int iFur)
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_CreateFur.wav", 0.3f);

	if (!m_pFurLineUpPanel->Get_ChildByIndex(1)->Get_Active() &&
		!m_pFurLineUpPanel->Get_ChildByIndex(3)->Get_Active()
		)
	{
		m_wstrMeshPath = CGameManager::GetInstance()->Get_Furniture(iFur)->wstrMeshPath;
		m_wstrMeshName = CGameManager::GetInstance()->Get_Furniture(iFur)->wstrMeshName;

		if (m_wstrMeshName == L"���� ���� �ٴ�.X" ||
			m_wstrMeshName == L"�� Ÿ�� �ٴ�.X" ||
			m_wstrMeshName == L"�Ķ��� ������ �ٴ�.X" ||
			m_wstrMeshName == L"��� ����Ʈ ������ �ٴ�.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ��ȫ�� �ٴ�.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeTile(E_LAYERTAG::Map);

			Furniture_CreateTile();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else if (m_wstrMeshName == L"���� ���� ��_Right.X" ||
			m_wstrMeshName == L"���� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"�Ķ� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"���� �ĵ� ���� ��_Right.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ���ݸ� ��_Right.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeWall_L(E_LAYERTAG::Map);
			CScene::Get_MainScene()->Set_Dead_CafeWall_R(E_LAYERTAG::Map);

			Furniture_CreateWall();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else
		{
			m_pObstacle_Cafe = CObstacle_Cafe::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Obstacle, m_wstrMeshPath, m_wstrMeshName);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
			m_pObstacle_Cafe->Set_CreateNew(true);
			CParticleObject* pFurParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"Cafe_CreateFur.particle");
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pFurParticle);

			m_pFunc_PickFurniture(true);
			m_pFunc_CreateFurniturePick(true);
		}

		m_pFixMode2Panel->Set_Active(false);
		m_pFunc_UpdateAmenity(true);
	}

	CGameObject* pObj = CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);

	if (pObj != nullptr)
	{
		static_cast<CInvite_Character*>(pObj)->Set_CreateUpdate(true);
	}
}

void CUI_Manager::Furniture_CreateTile()
{
	_float fX = 0.f, fZ = 0.f;
	_float fOneX = 0.f, fOneZ = 0.f;

	for (size_t i = 0; i < 24; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fZ = i * (0.6f);

			fOneX = 0.6f * 11.5f;
			fOneZ = 0.6f * 11.75f;

			m_pTile = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_CafeTile, m_wstrMeshPath, m_wstrMeshName);
			assert(m_pTile);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile, m_pTile);

			m_pTile->Set_Pos(_vec3(fX - fOneX, 0.f, fZ - fOneZ));
			m_pTile->Set_Scale(1.f, 1.f, 1.f);
		}
	}
}

void CUI_Manager::Furniture_CreateWall()
{
	if (m_wstrMeshName == L"���� ���� ��_Right.X")
	{
		_float fX_L = 0.f, fY_L = 0.f;
		_float fOneX_L = 0.f, fOneY_L = 0.f;

		for (size_t i = 0; i < 1; ++i)
		{
			for (size_t j = 0; j < 24; ++j)
			{
				fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
				fY_L = i * (0.6f);

				fOneX_L = 0.6f * 11.25f;
				fOneY_L = 0.f;

				m_pWall_L = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/x[Cafe]x_Default/", L"���� ���� ��_Left.X");
				assert(m_pWall_L);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

				m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
				m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
			}
		}
	}

	if (m_wstrMeshName == L"���� Ÿ�� ��_Right.X")
	{
		_float fX_L = 0.f, fY_L = 0.f;
		_float fOneX_L = 0.f, fOneY_L = 0.f;

		for (size_t i = 0; i < 1; ++i)
		{
			for (size_t j = 0; j < 24; ++j)
			{
				fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
				fY_L = i * (0.6f);

				fOneX_L = 0.6f * 11.25f;
				fOneY_L = 0.f;

				m_pWall_L = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/x[Cafe]x_GameCenter/", L"���� Ÿ�� ��_Left.X");
				assert(m_pWall_L);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

				m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
				m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
			}
		}
	}

	if (m_wstrMeshName == L"�Ķ� Ÿ�� ��_Right.X")
	{
		_float fX_L = 0.f, fY_L = 0.f;
		_float fOneX_L = 0.f, fOneY_L = 0.f;

		for (size_t i = 0; i < 1; ++i)
		{
			for (size_t j = 0; j < 24; ++j)
			{
				fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
				fY_L = i * (0.6f);

				fOneX_L = 0.6f * 11.25f;
				fOneY_L = 0.f;

				m_pWall_L = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/x[Cafe]x_Momo/", L"�Ķ� Ÿ�� ��_Left.X");
				assert(m_pWall_L);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

				m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
				m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
			}
		}
	}

	if (m_wstrMeshName == L"���� �ĵ� ���� ��_Right.X")
	{
		_float fX_L = 0.f, fY_L = 0.f;
		_float fOneX_L = 0.f, fOneY_L = 0.f;

		for (size_t i = 0; i < 1; ++i)
		{
			for (size_t j = 0; j < 24; ++j)
			{
				fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
				fY_L = i * (0.6f);

				fOneX_L = 0.6f * 11.25f;
				fOneY_L = 0.f;

				m_pWall_L = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/x[Cafe]x_PoolParty/", L"���� �ĵ� ���� ��_Left.X");
				assert(m_pWall_L);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

				m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
				m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
			}
		}
	}

	if (m_wstrMeshName == L"�߷�Ÿ�� ���ݸ� ��_Right.X")
	{
		_float fX_L = 0.f, fY_L = 0.f;
		_float fOneX_L = 0.f, fOneY_L = 0.f;

		for (size_t i = 0; i < 1; ++i)
		{
			for (size_t j = 0; j < 24; ++j)
			{
				fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
				fY_L = i * (0.6f);

				fOneX_L = 0.6f * 11.25f;
				fOneY_L = 0.f;

				m_pWall_L = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/x[Cafe]x_Valentine/", L"�߷�Ÿ�� ���ݸ� ��_Left.X");
				assert(m_pWall_L);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

				m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
				m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
			}
		}
	}

	_float fY_R = 0.f, fZ_R = 0.f;
	_float fOneY_R = 0.f, fOneZ_R = 0.f;

	for (size_t i = 0; i < 1; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fZ_R = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fY_R = i * (0.6f);

			fOneZ_R = 0.6f * 11.5f;
			fOneY_R = 0.f;

			m_pWall_R = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, m_wstrMeshPath, m_wstrMeshName);
			assert(m_pWall_R);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_R);

			m_pWall_R->Set_Pos(_vec3(7.2f, fY_R - fOneY_R, fZ_R - fOneZ_R));
			m_pWall_R->Set_Scale(1.f, 1.f, 0.5f);
		}
	}
}

void CUI_Manager::Furniture_SelInfo(_int iFur)
{
	m_pFunc_SelFurnitureInfo(true);
}

void CUI_Manager::Furniture_LineUp()
{
	if (m_pFurLineUpPanel->Get_ChildByIndex(1)->Get_Active())
	{
		m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Ui_Text(L"������");
	}

	if (m_pFurLineUpPanel->Get_ChildByIndex(3)->Get_Active())
	{
		m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Ui_Text(L"�̸�");
	}

	if (m_pFurLineUpPanel->Get_ChildByIndex(5)->Get_Active())
	{
		m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Set_Ui_Text(L"���� ����");
	}
}

void CUI_Manager::Furniture_InfoOpen()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_CafeInfoAmenity(true);
	m_pFunc_OpenFurInfo(true);
}

void CUI_Manager::Furniture_InfoExit()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Back.wav", 1.f);

	m_pCafeInfoPanel->Get_ChildByIndex(0)->Set_Active(true);
	m_pCafeInfoPanel->Get_ChildByIndex(1)->Set_Active(false);

	m_pFunc_ExitFurInfo(true);
}

void CUI_Manager::Furniture_Profit()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Profit.wav", 0.1f);

	m_pFunc_ProfitRecieve(true);
}

void CUI_Manager::Furniture_PresetName()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName(true);
}

void CUI_Manager::Furniture_PresetNameEnd()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetNameEnd(true);
}

void CUI_Manager::Furniture_PresetName1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName1(true);
}

void CUI_Manager::Furniture_PresetName2()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName2(true);
}

void CUI_Manager::Furniture_PresetName3()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName3(true);
}

void CUI_Manager::Furniture_PresetName4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName4(true);
}

void CUI_Manager::Furniture_PresetName5()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetName5(true);
}

void CUI_Manager::FurnitureSort_AutoSetting()
{
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(1)->OnClick();
}

void CUI_Manager::FurnitureSort_Ascending()
{
	//���� �±� : ������
	if (m_pFurLineUpPanel->Get_ChildByIndex(1)->Get_Active())
	{
		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Get_Active())
		{
			sort(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end(), Furniture_Compare_Amenity);
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(5)->Get_Active())
		{
			sort(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end(), Furniture_Compare_Amenity);
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(7)->Get_Active())
		{
			sort(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end(), Furniture_Compare_Amenity);
		}

		//m_vecFurAmenity = CGameManager::GetInstance()->Get_vecFurniture();
		//sort(m_vecFurAmenity.begin(), m_vecFurAmenity.end(), Furniture_Compare_Amenity);

		//for (_uint i = 0; i < m_vecFurAmenity.size(); ++i)
		//{
		//	switch (m_vecFurAmenity[i].eFurnitureStar)
		//	{
		//	case E_FurnitureStar::STAR_1:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
		//		break;
		//	case E_FurnitureStar::STAR_2:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
		//		break;
		//	case E_FurnitureStar::STAR_3:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
		//		break;
		//	default:
		//		assert(false);
		//		break;
		//	}

		//	wstring wstrFullText;
		//	wstrFullText = m_vecFurAmenity[i].wstrName;
		//	wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

		//	m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
		//	m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFurAmenity[i].wstrTexturePath);

		//	m_vecUi_Furniture[i]->Set_Active(TRUE);
		//}
	}

	//���� �±� : �̸�
	if (m_pFurLineUpPanel->Get_ChildByIndex(3)->Get_Active())
	{
		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Get_Active())
		{
			sort(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end(), Furniture_Compare_Name);
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(5)->Get_Active())
		{
			sort(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end(), Furniture_Compare_Name);
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(7)->Get_Active())
		{
			sort(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end(), Furniture_Compare_Name);
		}

		//m_vecFurName = CGameManager::GetInstance()->Get_vecFurniture();
		//sort(m_vecFurName.begin(), m_vecFurName.end(), Furniture_Compare_Name);

		//for (_uint i = 0; i < m_vecFurName.size(); ++i)
		//{
		//	switch (m_vecFurName[i].eFurnitureStar)
		//	{
		//	case E_FurnitureStar::STAR_1:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
		//		break;
		//	case E_FurnitureStar::STAR_2:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
		//		break;
		//	case E_FurnitureStar::STAR_3:
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
		//		m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
		//		break;
		//	default:
		//		assert(false);
		//		break;
		//	}

		//	wstring wstrFullText;
		//	wstrFullText = m_vecFurName[i].wstrName;
		//	wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

		//	m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
		//	m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFurName[i].wstrTexturePath);

		//	m_vecUi_Furniture[i]->Set_Active(TRUE);
		//}
	}
}

void CUI_Manager::FurnitureSort_Descending()
{
	//���� �±� : ������
	if (m_pFurLineUpPanel->Get_ChildByIndex(1)->Get_Active())
	{
		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Get_Active())
		{
			sort(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end(), Furniture_Compare_Amenity);
			reverse(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end());

			for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
			{
				if (m_vecFilter_Furniture[i].wstrFilter == L"����")
				{
					switch (m_vecFilter_Furniture[i].eFurnitureStar)
					{
					case E_FurnitureStar::STAR_1:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
						break;
					case E_FurnitureStar::STAR_2:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
						break;
					case E_FurnitureStar::STAR_3:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
						break;
					default:
						assert(false);
						break;
					}

					wstring wstrFullText;
					wstrFullText = m_vecFilter_Furniture[i].wstrName;
					wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

					m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
					m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

					if (m_bOneforFurAmenity)
					{
						m_vecUi_Furniture[i]->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_FurAmenityInfo, this, i), i);
						m_vecUi_Furniture[i]->Get_ChildByIndex(2)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Furniture_Filter_FurAmenityCreate, this, i), i);
					}

					m_vecUi_Furniture[i]->Set_Active(TRUE);
				}
				else
				{
					m_vecUi_Furniture[i]->Set_Active(FALSE);
				}
			}

			m_bOneforFurAmenity = false;
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(5)->Get_Active())
		{
			sort(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end(), Furniture_Compare_Amenity);
			reverse(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end());

			for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
			{
				if (m_vecFilter_Deco[i].wstrFilter == L"���")
				{
					switch (m_vecFilter_Deco[i].eFurnitureStar)
					{
					case E_FurnitureStar::STAR_1:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
						break;
					case E_FurnitureStar::STAR_2:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
						break;
					case E_FurnitureStar::STAR_3:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
						break;
					default:
						assert(false);
						break;
					}

					wstring wstrFullText;
					wstrFullText = m_vecFilter_Deco[i].wstrName;
					wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

					m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
					m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

					m_vecUi_Furniture[i]->Set_Active(TRUE);
				}
				else
				{
					m_vecUi_Furniture[i]->Set_Active(FALSE);
				}
			}
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(7)->Get_Active())
		{
			sort(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end(), Furniture_Compare_Amenity);
			reverse(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end());

			for (_uint i = 0; i < m_vecFilter_Indoor.size(); ++i)
			{
				if (m_vecFilter_Indoor[i].wstrFilter == L"����")
				{
					switch (m_vecFilter_Indoor[i].eFurnitureStar)
					{
					case E_FurnitureStar::STAR_1:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
						break;
					case E_FurnitureStar::STAR_2:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
						break;
					case E_FurnitureStar::STAR_3:
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
						m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
						break;
					default:
						assert(false);
						break;
					}

					wstring wstrFullText;
					wstrFullText = m_vecFilter_Indoor[i].wstrName;
					wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

					m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
					m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Indoor[i].wstrTexturePath);

					m_vecUi_Furniture[i]->Set_Active(TRUE);
				}
				else
				{
					m_vecUi_Furniture[i]->Set_Active(FALSE);
				}
			}
		}
	}

	//���� �±� : �̸�
	if (m_pFurLineUpPanel->Get_ChildByIndex(3)->Get_Active())
	{
		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->Get_Active())
		{
			sort(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end(), Furniture_Compare_Name);
			reverse(m_vecFilter_Furniture.begin(), m_vecFilter_Furniture.end());
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(5)->Get_Active())
		{
			sort(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end(), Furniture_Compare_Name);
			reverse(m_vecFilter_Deco.begin(), m_vecFilter_Deco.end());
		}

		if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(7)->Get_Active())
		{
			sort(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end(), Furniture_Compare_Name);
			reverse(m_vecFilter_Indoor.begin(), m_vecFilter_Indoor.end());
		}
	}
}

_bool CUI_Manager::Furniture_Compare_Name(const T_FurnitureInfo & tInfoBegin, const T_FurnitureInfo & tInfoEnd)
{
	return tInfoBegin.wstrName < tInfoEnd.wstrName;
}

_bool CUI_Manager::Furniture_Compare_Amenity(const T_FurnitureInfo & tInfoBegin, const T_FurnitureInfo & tInfoEnd)
{
	return tInfoBegin.iAmenity < tInfoEnd.iAmenity;
}

void CUI_Manager::Furniture_Sort_AmenityInfo(_int iFur)
{
	wstring wstrFullText;

	m_pFurInvenInfoPanel->Get_ChildByIndex(0)->Set_Tex1_Tag(m_vecFurAmenity[iFur].wstrTexturePath);
	m_pFurInvenInfoPanel->Get_ChildByIndex(1)->Set_Ui_Text(m_vecFurAmenity[iFur].wstrName);
	m_pFurInvenInfoPanel->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_vecFurAmenity[iFur].iAmenity));

	wstrFullText = m_vecFurAmenity[iFur].wstrDescription;
	wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
	m_pFurInvenInfoPanel->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
}

void CUI_Manager::Furniture_Sort_AmenityCreate(_int iFur)
{
	if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_Ui_Text() == L"������")
	{
		m_wstrMeshPath = m_vecFurAmenity[iFur].wstrMeshPath;
		m_wstrMeshName = m_vecFurAmenity[iFur].wstrMeshName;

		if (m_wstrMeshName == L"���� ���� �ٴ�.X" ||
			m_wstrMeshName == L"�� Ÿ�� �ٴ�.X" ||
			m_wstrMeshName == L"�Ķ��� ������ �ٴ�.X" ||
			m_wstrMeshName == L"��� ����Ʈ ������ �ٴ�.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ��ȫ�� �ٴ�.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeTile(E_LAYERTAG::Map);

			Furniture_CreateTile();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else if (m_wstrMeshName == L"���� ���� ��_Right.X" ||
			m_wstrMeshName == L"���� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"�Ķ� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"���� �ĵ� ���� ��_Right.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ���ݸ� ��_Right.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeWall_L(E_LAYERTAG::Map);
			CScene::Get_MainScene()->Set_Dead_CafeWall_R(E_LAYERTAG::Map);

			Furniture_CreateWall();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else
		{
			m_pObstacle_Cafe = CObstacle_Cafe::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Obstacle, m_wstrMeshPath, m_wstrMeshName);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
			m_pObstacle_Cafe->Set_CreateNew(true);
			CParticleObject* m_pFurParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"Cafe_CreateFur.particle");
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pFurParticle);


			m_pFunc_PickFurniture(true);
			m_pFunc_CreateFurniturePick(true);
		}

		m_pFixMode2Panel->Set_Active(false);
		m_pFunc_UpdateAmenity(true);
	}
}

void CUI_Manager::Furniture_Sort_NameInfo(_int iFur)
{
	wstring wstrFullText;

	m_pFurInvenInfoPanel->Get_ChildByIndex(0)->Set_Tex1_Tag(m_vecFurName[iFur].wstrTexturePath);
	m_pFurInvenInfoPanel->Get_ChildByIndex(1)->Set_Ui_Text(m_vecFurName[iFur].wstrName);
	m_pFurInvenInfoPanel->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_vecFurName[iFur].iAmenity));

	wstrFullText = m_vecFurName[iFur].wstrDescription;
	wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
	m_pFurInvenInfoPanel->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
}

void CUI_Manager::Furniture_Sort_NameCreate(_int iFur)
{
	if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_Ui_Text() == L"�̸�")
	{
		m_wstrMeshPath = m_vecFurName[iFur].wstrMeshPath;
		m_wstrMeshName = m_vecFurName[iFur].wstrMeshName;

		if (m_wstrMeshName == L"���� ���� �ٴ�.X" ||
			m_wstrMeshName == L"�� Ÿ�� �ٴ�.X" ||
			m_wstrMeshName == L"�Ķ��� ������ �ٴ�.X" ||
			m_wstrMeshName == L"��� ����Ʈ ������ �ٴ�.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ��ȫ�� �ٴ�.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeTile(E_LAYERTAG::Map);

			Furniture_CreateTile();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else if (m_wstrMeshName == L"���� ���� ��_Right.X" ||
			m_wstrMeshName == L"���� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"�Ķ� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"���� �ĵ� ���� ��_Right.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ���ݸ� ��_Right.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeWall_L(E_LAYERTAG::Map);
			CScene::Get_MainScene()->Set_Dead_CafeWall_R(E_LAYERTAG::Map);

			Furniture_CreateWall();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else
		{
			m_pObstacle_Cafe = CObstacle_Cafe::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Obstacle, m_wstrMeshPath, m_wstrMeshName);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
			m_pObstacle_Cafe->Set_CreateNew(true);
			CParticleObject* m_pFurParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"Cafe_CreateFur.particle");
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pFurParticle);

			m_pFunc_PickFurniture(true);
			m_pFunc_CreateFurniturePick(true);
		}

		m_pFixMode2Panel->Set_Active(false);
		m_pFunc_UpdateAmenity(true);
	}
}

void CUI_Manager::OpenInven_AutoSetting()
{
	m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(2)->OnClick();
	m_pFurInvenPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->OnClick();
}

void CUI_Manager::Furniture_Filter_All()
{
	//FurnitureSort_Descending();
	//FurnitureSort_Ascending();
}

void CUI_Manager::Furniture_Filter_Furniture()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_vecFilter_Furniture = CGameManager::GetInstance()->Get_vecFurniture();

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilter == L"����")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_Filter_FurAmenityInfo(_int iFur)
{
	wstring wstrFullText;

	m_pFurInvenInfoPanel->Get_ChildByIndex(0)->Set_Tex1_Tag(m_vecFilter_Furniture[iFur].wstrTexturePath);
	m_pFurInvenInfoPanel->Get_ChildByIndex(1)->Set_Ui_Text(m_vecFilter_Furniture[iFur].wstrName);
	m_pFurInvenInfoPanel->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(m_vecFilter_Furniture[iFur].iAmenity));

	wstrFullText = m_vecFilter_Furniture[iFur].wstrDescription;
	wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
	m_pFurInvenInfoPanel->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
}

void CUI_Manager::Furniture_Filter_FurAmenityCreate(_int iFur)
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_CreateFur.wav", 0.3f);

	if (m_pFurInvenPanel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_Ui_Text() == L"������")
	{
		m_wstrMeshPath = m_vecFilter_Furniture[iFur].wstrMeshPath;
		m_wstrMeshName = m_vecFilter_Furniture[iFur].wstrMeshName;

		if (m_wstrMeshName == L"���� ���� �ٴ�.X" ||
			m_wstrMeshName == L"�� Ÿ�� �ٴ�.X" ||
			m_wstrMeshName == L"�Ķ��� ������ �ٴ�.X" ||
			m_wstrMeshName == L"��� ����Ʈ ������ �ٴ�.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ��ȫ�� �ٴ�.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeTile(E_LAYERTAG::Map);

			Furniture_CreateTile();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else if (m_wstrMeshName == L"���� ���� ��_Right.X" ||
			m_wstrMeshName == L"���� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"�Ķ� Ÿ�� ��_Right.X" ||
			m_wstrMeshName == L"���� �ĵ� ���� ��_Right.X" ||
			m_wstrMeshName == L"�߷�Ÿ�� ���ݸ� ��_Right.X"
			)
		{
			CScene::Get_MainScene()->Set_Dead_CafeWall_L(E_LAYERTAG::Map);
			CScene::Get_MainScene()->Set_Dead_CafeWall_R(E_LAYERTAG::Map);

			Furniture_CreateWall();

			CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
		}
		else
		{
			m_pObstacle_Cafe = CObstacle_Cafe::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Obstacle, m_wstrMeshPath, m_wstrMeshName);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
			m_pObstacle_Cafe->Set_CreateNew(true);
			CParticleObject* m_pFurParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"Cafe_CreateFur.particle");
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pFurParticle);

			m_pFunc_PickFurniture(true);
			m_pFunc_CreateFurniturePick(true);
		}

		m_pFixMode2Panel->Set_Active(false);
		m_pFunc_UpdateAmenity(true);
	}
}

void CUI_Manager::Furniture_Filter_FurNameInfo(_int iFur)
{

}

void CUI_Manager::Furniture_Filter_FurNameCreate(_int iFur)
{

}

void CUI_Manager::Furniture_FilterTag_Table()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilterTag == L"���̺�")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_Closet()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilterTag == L"����")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_Chair()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilterTag == L"����")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_Bed()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilterTag == L"ħ��")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_EtcFur()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Furniture.size(); ++i)
	{
		if (m_vecFilter_Furniture[i].wstrFilterTag == L"��Ÿ_����")
		{
			switch (m_vecFilter_Furniture[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Furniture[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Furniture[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_Filter_Deco()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_vecFilter_Deco = CGameManager::GetInstance()->Get_vecFurniture();

	for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
	{
		if (m_vecFilter_Deco[i].wstrFilter == L"���")
		{
			switch (m_vecFilter_Deco[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Deco[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_Props()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
	{
		if (m_vecFilter_Deco[i].wstrFilterTag == L"��ǰ")
		{
			switch (m_vecFilter_Deco[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Deco[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_WallDeco()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
	{
		if (m_vecFilter_Deco[i].wstrFilterTag == L"�����")
		{
			switch (m_vecFilter_Deco[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Deco[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_FloorDeco()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
	{
		if (m_vecFilter_Deco[i].wstrFilterTag == L"�ٴ����")
		{
			switch (m_vecFilter_Deco[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Deco[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_FilterTag_EtcDeco()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Deco.size(); ++i)
	{
		if (m_vecFilter_Deco[i].wstrFilterTag == L"��Ÿ_���")
		{
			switch (m_vecFilter_Deco[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Deco[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Deco[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_Filter_Indoor()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_vecFilter_Indoor = CGameManager::GetInstance()->Get_vecFurniture();

	for (_uint i = 0; i < m_vecFilter_Indoor.size(); ++i)
	{
		if (m_vecFilter_Indoor[i].wstrFilter == L"����")
		{
			switch (m_vecFilter_Indoor[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Indoor[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Indoor[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_Filter_FloorIndoor()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Indoor.size(); ++i)
	{
		if (m_vecFilter_Indoor[i].wstrFilterTag == L"�ٴ�")
		{
			switch (m_vecFilter_Indoor[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Indoor[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Indoor[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Furniture_Filter_WallIndoor()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	for (_uint i = 0; i < m_vecFilter_Indoor.size(); ++i)
	{
		if (m_vecFilter_Indoor[i].wstrFilterTag == L"����")
		{
			switch (m_vecFilter_Indoor[i].eFurnitureStar)
			{
			case E_FurnitureStar::STAR_1:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"1");
				break;
			case E_FurnitureStar::STAR_2:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-2).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"2");
				break;
			case E_FurnitureStar::STAR_3:
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Cafe/��׶���_����ũ (2-3).png");
				m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"3");
				break;
			default:
				assert(false);
				break;
			}

			wstring wstrFullText;
			wstrFullText = m_vecFilter_Indoor[i].wstrName;
			wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 9);

			m_vecUi_Furniture[i]->Set_Ui_Text(wstrFullText);
			m_vecUi_Furniture[i]->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Tex1_Tag(m_vecFilter_Indoor[i].wstrTexturePath);

			m_vecUi_Furniture[i]->Set_Active(TRUE);
		}
		else
		{
			m_vecUi_Furniture[i]->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Sound_Click()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);
}

void CUI_Manager::Sound_Back()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Back.wav", 1.f);
}

void CUI_Manager::Sound_Create()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_CreateFur.wav", 0.3f);
}

void CUI_Manager::Sound_Delete()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_DeleteFur.wav", 0.3f);
}

void CUI_Manager::Furniture_PresetInit1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetInit1(true);
}

void CUI_Manager::Furniture_PresetSave1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetSave1(true);
}

void CUI_Manager::Furniture_PresetApply1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetApply1(true);
}

void CUI_Manager::Furniture_PresetInit2()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetInit2(true);
}

void CUI_Manager::Furniture_PresetSave2()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetSave2(true);
}

void CUI_Manager::Furniture_PresetApply2()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetApply2(true);
}

void CUI_Manager::Furniture_PresetInit3()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetInit3(true);
}

void CUI_Manager::Furniture_PresetSave3()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetSave3(true);
}

void CUI_Manager::Furniture_PresetApply3()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetApply3(true);
}

void CUI_Manager::Furniture_PresetInit4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetInit4(true);
}

void CUI_Manager::Furniture_PresetSave4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetSave4(true);
}

void CUI_Manager::Furniture_PresetApply4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetApply4(true);
}

void CUI_Manager::Furniture_PresetInit5()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetInit5(true);
}

void CUI_Manager::Furniture_PresetSave5()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetSave5(true);
}

void CUI_Manager::Furniture_PresetApply5()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Click.wav", 1.f);

	m_pFunc_PresetApply5(true);
}

void CUI_Manager::Gift_1()
{
	m_pFunc_Gift1(true);
}

void CUI_Manager::Gift_2()
{
	m_pFunc_Gift2(true);
}

void CUI_Manager::Gift_3()
{
	m_pFunc_Gift3(true);
}

void CUI_Manager::Gift_4()
{
	m_pFunc_Gift4(true);
}

void CUI_Manager::Gift_5()
{
	m_pFunc_Gift5(true);
}

void CUI_Manager::AllStorage()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_DeleteFur.wav", 0.3f);

	CGameObject* pObj = CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);

	if (pObj != nullptr)
	{
		static_cast<CInvite_Character*>(pObj)->Set_AllStorage(true);
	}
}

#pragma endregion

#pragma region ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ȣ


// ���� UI ���� 
void CUI_Manager::Enter_Shop_UI()
{
	///////////////////////////////////////////////////////////////////////////////////
	// �κ�ȭ�� -> ����

	if (m_pLobbyPanel)
	{
		// �κ� ��Ȱ��ȭ && ��� UI Ȱ��ȭ
		m_pLobbyPanel->Set_Active(FALSE);
		m_pHighInfoPanel->Set_Active(TRUE);
		m_pHighPanel_PanelName->Set_Ui_Text(L"����");
	}

	///////////////////////////////////////////////////////////////////////////////////
	// ���� ���� ����

	if (!m_pCollectionPanel)
	{
		// ���ʼ��� - ���� ���� ��ü
		m_pShop_Main = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Shop/Shop1.uidat");

		m_pShop_Main->Set_Active(TRUE);
		CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pShop_Main->Get_ObjTag(), m_pShop_Main);



		// ���ʼ��� - ��ư ���ε�
		for (_int i = 0; i < 8; i++)
		{
			m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_Shop_Menu_Btn, this, i / 2), i / 2);
		}



		// ���ʼ��� - ������ ����Ʈ
		for (_int i = 0; i < 4; i++)
		{
			m_pShop_List[i] = m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(8 + i);
			m_pShop_List[i]->Set_Active(TRUE);
		}
		m_pShop_ResetList = m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(13);
		m_pShop_MultiList = m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(14);

		// ������ ������Ʈ
		CGameManager::GetInstance()->Collect_Update();


		CreateShopItem(0, (_int)E_ItemTag::�׷�_������, (_int)E_ItemTag::���Ĺ�_������);

		CreateShopItem(1, 0, 1);
		CreateShopItem(2, 0, 2);
		CreateShopItem(3, 1, 3);


		// �ʱ� ����
		Click_Shop_Menu_Btn(0);
	}
}

void CUI_Manager::Click_Shop_Menu_Btn(_int iIndex)
{
	if (iIndex == m_iShop_BtnSel)
	{
		return;
	}

	m_iShop_BtnSel = iIndex;

	// ����, ��Ƽ ���� �г�
	Reset_MultiChoice();

	// ��ü ��ư ��Ȱ��ȭ
	for (_int i = 0; i < 4; i++)
	{
		m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(i * 2)->Set_Active(FALSE);
		m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(i * 2 + 1)->Set_Active(TRUE);

		m_pShop_List[i]->Set_Active(FALSE);
	}

	// ���� ��ư Ȱ��ȭ
	m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(iIndex * 2)->Set_Active(TRUE);
	m_pShop_Main->Get_ChildByIndex(0)->Get_ChildByIndex(iIndex * 2 + 1)->Set_Active(FALSE);
	m_pShop_List[iIndex]->Set_Active(TRUE);
}

void CUI_Manager::CreateShopItem(_int iIndex, _int iStartItem, _int iEndItem)
{
	vector<CUI_Object*>* pVectorObj = &m_vectorShopObj[iIndex];
	vector<T_ShopItemInfo> vectorInfo = m_vectorSjopItm[iIndex];

	// ������ �ʱ�ȭ
	for (auto iter : *(m_pShop_List[iIndex]->Get_ChildList()))
	{
		iter->Set_Dead(TRUE);
	}

	pVectorObj->clear();
	vectorInfo.clear();

	//////////////////////////////////////////////////////////////////

	pVectorObj->reserve(iEndItem - iStartItem);

	for (_int i = 0; i < iEndItem - iStartItem; ++i)
	{
		// ������ �غ� �� ����
		CUI_Object* pItem = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Shop/Item_03.uidat");
		pItem->Set_Active(TRUE);

		m_pShop_List[iIndex]->Set_Child(pItem);
		pVectorObj->emplace_back(pItem);

		pItem->Get_ChildByIndex(0)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem, this, (_int)i), (_int)i);
		pItem->Get_ChildByIndex(2)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem, this, (_int)i), (_int)i);
		pItem->Get_ChildByIndex(3)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem, this, (_int)i), (_int)i);
		pItem->Get_ChildByIndex(4)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem, this, (_int)i), (_int)i);
		pItem->Get_ChildByIndex(5)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem, this, (_int)i), (_int)i);

		pItem->Get_ChildByIndex(6)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::ClickShopItem_Purchase, this, (_int)i), (_int)i);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		pItem->Set_NameTag(L"1�� ����Ʈ");

		// ������ ���� ��������
		T_ItemInfo tItemInfo = *CGameManager::GetInstance()->Get_Item((_int)iStartItem + i);
		T_ShopItemInfo tShopItemIfo = T_ShopItemInfo();

		tShopItemIfo.bAvailable = TRUE;
		tShopItemIfo.iID = (_int)tItemInfo.eItemTag;

		tShopItemIfo.iRare = 0; // 0(�ϱ�),1(�Ϲ�),2(���),3(�ֻ��)
		tShopItemIfo.eBuyType = tItemInfo.eBuyType;

		tShopItemIfo.iPrice = i * 10;
		tShopItemIfo.iAmount = i;
		tShopItemIfo.iMax = i * 2 - 5;

		vectorInfo.emplace_back(tShopItemIfo);

		/////////////////////////////////////////////////////////////
		// ���� �Է�


		// ��ü �̸�
		pItem->Set_NameTag(tItemInfo.wstrName);


		// ������ �̸�
		pItem->Get_ChildByIndex(1)->Set_Ui_Text(tItemInfo.wstrName);
		// ������ �ؽ�ó
		pItem->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Tex1_Tag(tItemInfo.wstrImagePath);


		// ������ �Ǹ� ����
		pItem->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(L"x" + to_wstring(tShopItemIfo.iAmount));

		// ������ ���� ���� ����
		if (0 <= tShopItemIfo.iMax)
		{
			// �� ���ִ� �����϶��� ǥ��
			pItem->Get_ChildByIndex(4)->Set_Active(TRUE);
			pItem->Get_ChildByIndex(4)->Set_Ui_Text(to_wstring(tShopItemIfo.iMax) + L"ȸ ���� ����");
		}
		else
		{
			pItem->Get_ChildByIndex(4)->Set_Active(FALSE);
		}


		// ������ ���� ����;
		CUI_Object* pCreditIcon = pItem->Get_ChildByIndex(6)->Get_ChildByIndex(2);
		pCreditIcon->Set_Ui_Text(to_wstring(tShopItemIfo.iPrice));

		// ������ ���� ��ȭ
		if (E_Money::ũ���� == tShopItemIfo.eBuyType)
		{
			pCreditIcon->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Shop/Shop_Gold.png");
		}
		else if (E_Money::�����׸� == tShopItemIfo.eBuyType)
		{
			pCreditIcon->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Shop/Shop_SecretStone.png");
		}
		else if (E_Money::û�ּ� == tShopItemIfo.eBuyType)
		{
			pCreditIcon->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Shop/Shop_Gem.png");
		}
		else if (E_Money::����_�ѷ��� == tShopItemIfo.eBuyType)
		{
			pCreditIcon->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Shop/Shop_RaidCoin.png");
		}
		else if (E_Money::����_������ȸ == tShopItemIfo.eBuyType)
		{
			pCreditIcon->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Shop/Shop_ArenaCoin.png");
		}
	}
}

void CUI_Manager::ClickShopItem(_int iIndex)
{
	if (1 == m_iShop_BtnSel || 2 == m_iShop_BtnSel)
	{
		return;
	}

	CUI_Object* pObj = m_vectorShopObj[m_iShop_BtnSel][iIndex]->Get_ChildByIndex(5);
	pObj->Set_Active(!pObj->Get_Active());

	Add_MultiChoice(iIndex);
}

void CUI_Manager::ClickShopItem_Purchase(_int iIndex)
{
	// ���Ź�ư
	_int iA = iIndex;
	iA = iA;
}

void CUI_Manager::Add_MultiChoice(_int iIndex)
{
	m_pShop_ResetList->Set_Active(FALSE);
	m_pShop_MultiList->Set_Active(TRUE);

	m_listItemSelect.emplace_back(iIndex);
}

void CUI_Manager::Reset_MultiChoice()
{
	for (_int i = 0; i < 4; i++)
	{
		for (auto iter : m_vectorShopObj[i])
		{
			iter->Get_ChildByIndex(5)->Set_Active(FALSE);
		}
	}

	m_listItemSelect.clear();

	if (1 == m_iShop_BtnSel || 2 == m_iShop_BtnSel)
	{
		m_pShop_ResetList->Set_Active(FALSE);
		m_pShop_MultiList->Set_Active(FALSE);
	}
	else
	{
		m_pShop_ResetList->Set_Active(TRUE);
		m_pShop_MultiList->Set_Active(FALSE);
	}
}

void CUI_Manager::Click_MultuChoice_Cancle()
{
}

void CUI_Manager::Click_MultuChoice_Ok()
{
}

#pragma endregion

#pragma region ����  â
void CUI_Manager::Create_Combat_Ing_Panel()
{
	m_pCombatIngPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]����â �θ�.uidat");
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pCombatIngPanel->Get_ObjTag(), m_pCombatIngPanel);
	m_pCombatIngPanel->Set_Active(FALSE);

	m_pCombatIngBackground = m_pCombatIngPanel->Get_ChildByIndex(0);
	m_pCombatIngBackground->Set_Active(FALSE);

	m_pCombatIng_SkillThumnail = m_pCombatIngPanel->Get_ChildByIndex(1);
	m_pCombatIng_SkillThumnail->Set_Active(FALSE);
	for (_int i = 0; i < 3; ++i)
	{
		m_pCombatIng_Skill[i] = m_pCombatIngPanel->Get_ChildByIndex(i + 2);
		m_pCombatIng_Skill[i]->Get_ChildByIndex(1)->Set_AfterPass(TRUE);
		m_pCombatIng_Skill[i]->Get_ChildByIndex(1)->Set_PassNumber((_int)E_UI_ShaderPassType::FillCircle);
		m_pCombatIng_Skill[i]->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_Skill, this, i), i);
	}

	m_pCombatIng_Cost = m_pCombatIngPanel->Get_ChildByIndex(5);

	m_pCombatIng_GameSpeed = m_pCombatIngPanel->Get_ChildByIndex(6);
	m_pCombatIng_GameSpeed->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_BattleSpeed, this));

	m_pCombatIng_Auto = m_pCombatIngPanel->Get_ChildByIndex(7);
	m_pCombatIng_Auto->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Click_AutoBattle, this));

	m_pCombatIng_GraphBackground = m_pCombatIngPanel->Get_ChildByIndex(8);
	m_pCombatIng_Graph = m_pCombatIngPanel->Get_ChildByIndex(9);
	m_pCombatIng_Graph->Set_ResetMask(FALSE);

	// ����
	m_pCombatIng_Assemble = m_pCombatIngPanel->Get_ChildByIndex(10);
	m_pCombatIng_Assemble->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Set_Assemble, this));

	// ����
	m_pCombatIng_Dissipation = m_pCombatIngPanel->Get_ChildByIndex(11);
	m_pCombatIng_Dissipation->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Set_Dissipation, this));

	m_pCombatIng_Announcement = m_pCombatIngPanel->Get_ChildByIndex(12);

	if (CGameManager::GetInstance()->Get_BattleIng())
	{
		Skill_Change();
	}
}

void CUI_Manager::Update_Combat_Ing(const _float& fTimeDelta)
{
	if (nullptr == m_pCombatIngPanel)
		return;

	if (FALSE == CGameManager::GetInstance()->Get_BattleIng())
	{
		Update_Combat_Victory(fTimeDelta);
		return;
	}

	_float fCost = CGameManager::GetInstance()->Get_Cost();
	for (_int i = 0; i < 5; i++)
	{
		m_pCombatIng_Cost->Get_ChildByIndex(0)->Get_ChildByIndex(i)->Set_Ui_Text(to_wstring((_int)fCost));
	}

	m_pCombatIng_Graph->Set_Mask(_vec2(0.f, 0.f), _vec2(fCost * 0.1f, 1.f));

	// ĳ���� ã��
	map<_int, vector<CCharacter*>>* mapSquad = CGameManager::GetInstance()->Get_CurrentSquad();

	for (_int i = 0; i < (_int)(*CGameManager::GetInstance()->Get_SkillCost()).size(); ++i)
	{
		CCharacter*	pSkill_Char = nullptr;
		// ��Ʈ����Ŀ
		if ((*CGameManager::GetInstance()->Get_SkillCost())[i] < 10)
		{
			_int iIndex = (*CGameManager::GetInstance()->Get_SkillCost())[i];
			pSkill_Char = (*mapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][iIndex];
		}
		else	// �����
		{
			_int iIndex = (*CGameManager::GetInstance()->Get_SkillCost())[i] % 10;
			pSkill_Char = (*mapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][iIndex];
		}

		if (pSkill_Char)
		{
			m_pCombatIng_Skill[i]->Get_ChildByIndex(1)->Set_Angle((fCost / pSkill_Char->Get_CharInfo().iExSkillCost) * 2.f * PI * Rad2Deg);
		}
	}

	if (m_pCombatIng_FocusUnit)
	{
		CEffectObject* pRangeEffect = CGameManager::GetInstance()->Get_RangeEffect(E_RangeEffect::Circle_OutLine);
		_float fUnitRange = m_pCombatIng_FocusUnit->Get_UnitRange();

		pRangeEffect->Set_Effect_Scale(_vec3(fUnitRange, fUnitRange, fUnitRange));

		pRangeEffect->Set_Effect_Pos(m_pCombatIng_FocusUnit->Get_Position());

		if (CKeyManager::GetInstance()->Key_Up(KEY_LBUTTON))
		{
			// �״�� ���� ���콺�� �ִٸ� ���� x
			Active_Skill(m_pCombatIng_FocusButton->Is_OnMouse(CUtility::Get_MousePoint(*m_pHwnd)));
			m_pCombatIng_FocusUnit = nullptr;
			m_pCombatIng_FocusButton = nullptr;
		}
	}


	m_fCombatIng_AnnounceTime -= fTimeDelta;
	if (0.f <= m_fCombatIng_AnnounceTime)
	{
		m_pCombatIng_Announcement->Set_Active(TRUE);

		_float fRatio = (m_fCombatIng_AnnounceTimeMax - m_fCombatIng_AnnounceTime) / 0.4f;
		fRatio = CUtility::Clamp_float(fRatio, 0.f, 1.f);
		_int iTextSize = (_int)m_wstrCombatIng_AnnounceText.size();
		_int iFontSize = m_pCombatIng_Announcement->Get_TextOption().iFontSize;
		_int iRenderSize = (_int)(iTextSize * fRatio);

		m_pCombatIng_Announcement->Set_Ui_SizeX(40 * (_int)(iTextSize * fRatio));
		m_pCombatIng_Announcement->Set_Ui_Text(m_wstrCombatIng_AnnounceText.substr(0, iRenderSize));
		m_pCombatIng_Announcement->Set_ImageAlpha(0.5f);
		m_pCombatIng_Announcement->Set_TextAlpha(1.f);
	}
	else
	{
		_float fRatio = abs(m_fCombatIng_AnnounceTime * 0.5f);
		fRatio = CUtility::Clamp_float(fRatio, 0.f, 1.f);

		m_pCombatIng_Announcement->Set_ImageAlpha((1.f - fRatio) * 0.5f);
		m_pCombatIng_Announcement->Set_TextAlpha((1.f - fRatio));
	}

	if (m_bCombatIng_Assemble && CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
	{
		Function_Assemble();
	}

	if (m_bCombatIng_Dissipation && CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
	{
		Click_Map();
		for (auto& Character : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
		{
			if (nullptr == Character)
				continue;
			Character->FSM_FunctionSetting(&CUnit::Dissipation, nullptr);
			Character->Set_Assemble(FALSE);
		}
		m_bCombatIng_Dissipation = FALSE;

		m_pCombatIng_Assemble->Set_Active(TRUE);
		m_pCombatIng_Dissipation->Set_Active(FALSE);
	}
}

void CUI_Manager::Active_Combat_Ing()
{
	Skill_Change();
	Goto_Lobby();

	if (m_pLobbyPanel)
	{
		m_pLobbyPanel->Set_Active(FALSE);
	}

	if (m_pCombatIngPanel)
	{
		m_pCombatIngPanel->Set_Active(TRUE);
	}

	if (m_pCombatIng_Assemble)
	{
		m_pCombatIng_Assemble->Set_Active(TRUE);
	}

	if (m_pCombatIng_Dissipation)
	{
		m_pCombatIng_Dissipation->Set_Active(FALSE);
	}
}

void CUI_Manager::Click_Skill(_int iSkillSlot)
{
	CUnit* pUnit = CGameManager::GetInstance()->Get_SkillUnit(iSkillSlot);

	if (nullptr == pUnit)
	{
		return;
	}

	m_pCombatIng_FocusUnit = pUnit;
	m_pCombatIng_FocusButton = m_pCombatIng_Skill[iSkillSlot];
	CGameManager::GetInstance()->Set_SkillOnMouse(TRUE);
	static_cast<CCharacter*>(pUnit)->Set_GuideSkill();
	static_cast<CCharacter*>(pUnit)->Set_Aiming(TRUE);
	m_pCombatIngBackground->Set_Active(TRUE);
}

void CUI_Manager::Active_Skill(_bool bActive)
{
	CGameManager::GetInstance()->Set_SkillOnMouse(FALSE);
	m_pCombatIngBackground->Set_Active(FALSE);


	if (bActive)
	{
		return;
	}

	CGameManager::GetInstance()->Use_Skill(static_cast<CCharacter*>(m_pCombatIng_FocusUnit));
	Skill_Change();

	m_pCombatIng_SkillThumnail->Set_Active(TRUE);
	m_pCombatIng_SkillThumnail->Update_Object(0.f);
	E_CHAR_NAME eCharEnum = static_cast<CCharacter*>(m_pCombatIng_FocusUnit)->Get_CharInfo().eName[0];
	wstring wstrCharName_Eng = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_NAME);
	wstring wstrFullPath = L"../../Reference/Resource/Texture/UI/Character_Collection/Character/Skill_Portrait_";
	wstrFullPath.append(wstrCharName_Eng);
	wstrFullPath.append(L".png");
	m_pCombatIng_SkillThumnail->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrFullPath);

	m_pCombatIng_SkillThumnail->Invoke_Add(1.5f, bind(&CUI_Object::Set_Active, m_pCombatIng_SkillThumnail, false), false);
}

void CUI_Manager::Skill_Change()
{
	_float fCost = CGameManager::GetInstance()->Get_Cost();
	// ĳ���� ã��
	map<_int, vector<CCharacter*>>* mapSquad = CGameManager::GetInstance()->Get_CurrentSquad();

	_int iCharSize = (_int)(*CGameManager::GetInstance()->Get_SkillCost()).size();
	for (_int i = 0; i < iCharSize; ++i)
	{
		if (nullptr == m_pCombatIng_Skill[i])
		{
			continue;
		}

		m_pCombatIng_Skill[i]->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);

		CCharacter*	pSkill_Char = nullptr;
		// ��Ʈ����Ŀ
		if ((*CGameManager::GetInstance()->Get_SkillCost())[i] < 10)
		{
			_int iIndex = (*CGameManager::GetInstance()->Get_SkillCost())[i];
			pSkill_Char = (*mapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][iIndex];
		}
		else	// �����
		{
			_int iIndex = (*CGameManager::GetInstance()->Get_SkillCost())[i] % 10;
			pSkill_Char = (*mapSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][iIndex];
		}

		if (nullptr == pSkill_Char)
		{
			m_pCombatIng_Skill[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"");
			m_pCombatIng_Skill[i]->Get_ChildByIndex(0)->Set_ImageAlpha(0.f);
		}
		m_pCombatIngPanel->Get_ChildByIndex(i + 2)->Set_Active(TRUE);

		if (pSkill_Char)
		{
			E_CHAR_NAME eCharEnum = pSkill_Char->Get_CharInfo().eName[0];
			wstring wstrCharName_Eng = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_NAME);
			wstring wstrCharName_Kor = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_KORNAME);
			wstring wstrFullPath = L"../../Reference/Resource/Texture/UI/Character_Collection/Character/Skill_Portrait_";

			wstrFullPath.append(wstrCharName_Eng);
			wstrFullPath.append(L".png");
			// Fill�̹���
			if (fCost < pSkill_Char->Get_CharInfo().iExSkillCost)
			{
				m_pCombatIng_Skill[i]->Get_ChildByIndex(1)->Set_Angle((fCost / pSkill_Char->Get_CharInfo().iExSkillCost));
			}
			else
			{
				m_pCombatIng_Skill[i]->Get_ChildByIndex(1)->Set_Angle(0.f);
			}

			// �ʿ� �ڽ�Ʈ ����
			m_pCombatIng_Skill[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(pSkill_Char->Get_CharInfo().iExSkillCost));

			// ĳ���� �̹��� ����
			m_pCombatIng_Skill[i]->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrFullPath);

			// ���� Ÿ�Կ� ���� Mask �̹��� �� ����
			switch (pSkill_Char->Get_CharInfo().eAtkType)
			{
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_NULL:			m_pCombatIng_Skill[i]->Set_ImageColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f)); break;
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION:		m_pCombatIng_Skill[i]->Set_ImageColor(COLOR_EXPLOSION); break;
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_PIERCE:		m_pCombatIng_Skill[i]->Set_ImageColor(COLOR_PIERCE); break;
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:		m_pCombatIng_Skill[i]->Set_ImageColor(COLOR_MYSTERY); break;
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_NORMAL:		m_pCombatIng_Skill[i]->Set_ImageColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f)); break;
			case Engine::E_CHAR_ATKTYPE::ATKTYPE_END:			m_pCombatIng_Skill[i]->Set_ImageColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f)); break;
			default: break;
			}
		}
	}

	if (m_pCombatIngPanel)
	{
		for (_int i = iCharSize; i <= 3; ++i)
		{
			m_pCombatIngPanel->Get_ChildByIndex(i + 2)->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Click_BattleSpeed()
{
	E_BattleSpeed eBattleSpeed = CGameManager::GetInstance()->Get_BattleSpeed();

	switch (eBattleSpeed)
	{
	case Engine::E_BattleSpeed::_X1:	CGameManager::GetInstance()->Set_BattleSpeed(E_BattleSpeed::_X2);		break;
	case Engine::E_BattleSpeed::_X2:	CGameManager::GetInstance()->Set_BattleSpeed(E_BattleSpeed::_X3);		break;
	case Engine::E_BattleSpeed::_X3:	CGameManager::GetInstance()->Set_BattleSpeed(E_BattleSpeed::_X1);		break;
	}

	Set_BattleSpeed_Image();
}

void CUI_Manager::Click_AutoBattle()
{
	_bool bAuto = CGameManager::GetInstance()->Get_AutoBattle();

	CGameManager::GetInstance()->Set_AutoBattle(!bAuto);

	Set_AutoBattle_Image();
}

void CUI_Manager::Set_BattleSpeed_Image()
{
	E_BattleSpeed eBattleSpeed = CGameManager::GetInstance()->Get_BattleSpeed();

	switch (eBattleSpeed)
	{
	case Engine::E_BattleSpeed::_X1:	m_pCombatIng_GameSpeed->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Button/Ingame_Speed_Off.png");	break;
	case Engine::E_BattleSpeed::_X2:	m_pCombatIng_GameSpeed->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Button/Ingame_Speed_On.png");	break;
	case Engine::E_BattleSpeed::_X3:	m_pCombatIng_GameSpeed->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Button/Ingame_Speed_On_2.png");	break;
	}
}

void CUI_Manager::Set_AutoBattle_Image()
{
	if (CGameManager::GetInstance()->Get_AutoBattle())
	{
		m_pCombatIng_Auto->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Button/Ingame_Auto_On.png");
	}
	else
	{
		m_pCombatIng_Auto->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Button/Ingame_Auto_Off.png");
	}
}

void CUI_Manager::Set_Assemble()
{
	m_bCombatIng_Assemble = TRUE;
	m_bCombatIng_Dissipation = FALSE;
}

void CUI_Manager::Set_Dissipation()
{
	m_bCombatIng_Assemble = FALSE;
	m_bCombatIng_Dissipation = TRUE;
}

// 1�� �̻��� �ð��� Announcement ���ּ���. (SizeLerp �Ǵ� �ð�������)
void CUI_Manager::Set_Announcement(wstring wstrText, const _float& fMaxShowTime)
{
	if (1.f > fMaxShowTime)
	{
		CUtility::Print_Wstring_DMode(L"1�� �̻��� �ð��� Announcement ���ּ���.");
		return;
	}

	m_fCombatIng_AnnounceTimeMax = fMaxShowTime;
	m_fCombatIng_AnnounceTime = fMaxShowTime;

	m_wstrCombatIng_AnnounceText = wstrText;
}

void CUI_Manager::Function_Assemble()
{
	if (nullptr == m_pCombatIngPanel)
		return;

	if (FALSE == CGameManager::GetInstance()->Get_BattleIng())
	{
		return;
	}

	Click_Map();
	_int iCount = 0;

	for (auto& Character : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (nullptr == Character)
			continue;

		_vec3 vAddPos = VEC_ZERO;
		if (1 == iCount)
			vAddPos.x += 1.0f;
		else if (2 == iCount)
			vAddPos.y += 1.0f;
		else if (3 == iCount)
		{
			vAddPos.x += 1.0f;
			vAddPos.y += 1.0f;
		}
		Character->Clear_Astar();
		Character->Set_Assemble(FALSE);
		Character->Find_Astar(Character->Get_Position(), m_vPickingPos + vAddPos, Character->Get_CanJump());
		Character->FSM_FunctionSetting(&CUnit::Assemble, nullptr);
		Character->Set_Assemble(TRUE);
		++iCount;
	}
	m_bCombatIng_Assemble = FALSE;
	m_pCombatIng_Assemble->Set_Active(FALSE);
	m_pCombatIng_Dissipation->Set_Active(TRUE);
}

#pragma endregion


#pragma region ���� �¸� â

void CUI_Manager::Create_Combat_Victory_Panel()
{
	// ���� �¸� â
	m_pCombatVictoryPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Combat_Victory.uidat");
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pCombatVictoryPanel->Get_ObjTag(), m_pCombatVictoryPanel);
	m_pCombatVictoryPanel->Set_Active(FALSE);

	m_pComVtory_Defeat = m_pCombatVictoryPanel->Get_ChildByIndex(0);
	m_pComVtory_Defeat->Set_Active(FALSE);
	m_pComVtory_EndButton = m_pCombatVictoryPanel->Get_ChildByIndex(5);
	m_pComVtory_EndButton->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Combat_Victory_End_Button, this));
}

void CUI_Manager::Setting_Combat_Victory()
{
	if (nullptr == m_pCombatVictoryPanel)
	{
		return;
	}

	if (m_pCombatIngPanel)
	{
		m_pCombatIngPanel->Set_Active(FALSE);
	}

	// �г��� ����
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(CDataManager::GetInstance()->Get_NickName());

	// �÷��̾� ����ġ
	_float fExpRatio = CDataManager::GetInstance()->Get_EXP() / (_float)CDataManager::GetInstance()->Get_Level() * 10;
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_EXP()) + L" / " + to_wstring(CDataManager::GetInstance()->Get_Level() * 10));
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2(fExpRatio, 1.f));
	
	// �÷��̾� ����
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring(CDataManager::GetInstance()->Get_Level()));
	m_pCombatVictoryPanel->Set_Active(TRUE);

	// ���� �ð�
	_float fBattleTime = CGameManager::GetInstance()->Get_BattleTotalTime(); 
	_int iMinute = (_int)fBattleTime / 60;
	_int iSecond = (_int)fBattleTime % 60;
	wstring wstrMinute;
	if (iMinute < 10)
		wstrMinute = L"0" + to_wstring(iMinute);
	else
		wstrMinute = to_wstring(iMinute);

	wstring wstrSecond;
	if (iSecond < 10)
		wstrSecond = L"0" + to_wstring(iSecond);
	else
		wstrSecond = to_wstring(iSecond);

	m_pCombatVictoryPanel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(L"���� �ð� " + wstrMinute + L" : " + wstrSecond);

	// ������ ����Ʈ
	m_pDamageReport_Panel = m_pCombatVictoryPanel->Get_ChildByIndex(7);
	m_pDamageReport_Panel->Set_Active(FALSE);

	// ���� ���� ���� :			�̸� ����ġ ����
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(CGameManager::GetInstance()->Get_NickName());
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Mask(_vec2(0.f, 0.f), _vec2(0.5f, 1.f));
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Ui_Text(CGameManager::GetInstance()->Get_EXP() + L"/" + to_wstring(CGameManager::GetInstance()->Get_Level() * 10));
	m_pCombatVictoryPanel->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv. " + to_wstring(CGameManager::GetInstance()->Get_Level()));

	// ���� ���� �ð� (���� ����)

	// Ȯ�� ��ư

	// ĳ���� �Ϸ���Ʈ �� ���� ����
	CUI_Object* pCharMask_1 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	CUI_Object* pCharMask_2 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	CUI_Object* pCharMask_3 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(4)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	CUI_Object* pCharMask_4 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(5)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	CUI_Object* pCharMask_5 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	CUI_Object* pCharMask_6 = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);

	map<_int, vector<CCharacter*>>* pSquadMap = CGameManager::GetInstance()->Get_CurrentSquad();
	vector<CCharacter*>& vecStriker = (*pSquadMap)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER];
	vector<CCharacter*>& vecSpecial = (*pSquadMap)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL];

	_int iMaxDamage = 1;

	for (_int i = 0; i < 6; ++i)
	{
		CCharacter* pChar = nullptr;
		CUI_Object* pCharacterMask = m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(i + 2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
		if (i < 4)
		{
			pChar = vecStriker[i];
		}
		else
		{
			pChar = vecSpecial[i - 4];
		}
		if (nullptr == pChar)
		{
			m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(i + 2)->Set_Active(FALSE);
			pCharacterMask->Get_ChildByIndex(2)->Set_Active(FALSE);
			pCharacterMask->Get_Parent()->Get_Parent()->Set_Active(FALSE);
			pCharacterMask->Get_ChildByIndex(1)->Set_ImageAlpha(0.f);
			pCharacterMask->Get_Parent()->Set_ImageColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f));
			continue;
		}
		m_pCombatVictoryPanel->Get_ChildByIndex(6)->Get_ChildByIndex(i + 2)->Set_Active(TRUE);
		pCharacterMask->Get_ChildByIndex(2)->Set_Active(TRUE);
		pCharacterMask->Get_Parent()->Get_Parent()->Set_Active(TRUE);

		// ĳ���� �̸��� �޾ƿ´�
		E_CHAR_NAME eCharEnum = pChar->Get_CharInfo().eName[0];
		wstring wstrCharName_Eng = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_NAME);
		wstring wstrCharName_Kor = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_KORNAME);
		wstring wstrFullPath = L"../../Reference/Resource/Texture/UI/Character_Collection/Character/Skill_Portrait_";

		wstrFullPath.append(wstrCharName_Eng);
		wstrFullPath.append(L".png");

		// ĳ���� �ʻ�ȭ 
		pCharacterMask->Get_ChildByIndex(1)->Set_ImageAlpha(1.f);
		pCharacterMask->Get_ChildByIndex(1)->Set_Tex1_Tag(wstrFullPath);

		// OutLine ��Ʈ

		pCharacterMask->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_Text(L"Lv. " + to_wstring(pChar->Get_CharInfo().iLevel));
		pCharacterMask->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(L"Lv. " + to_wstring(pChar->Get_CharInfo().iLevel));
		pCharacterMask->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv. " + to_wstring(pChar->Get_CharInfo().iLevel));
		pCharacterMask->Get_ChildByIndex(2)->Get_ChildByIndex(3)->Set_Ui_Text(L"Lv. " + to_wstring(pChar->Get_CharInfo().iLevel));
		pCharacterMask->Get_ChildByIndex(2)->Get_ChildByIndex(4)->Set_Ui_Text(L"Lv. " + to_wstring(pChar->Get_CharInfo().iLevel));

		// �� ����
		pCharacterMask->Get_Parent()->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring((_int)pChar->Get_CharInfo().eStar));

		// ���� Ÿ�Կ� ���� Mask �̹��� �� ����
		switch (pChar->Get_CharInfo().eAtkType)
		{
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_NULL:			pCharacterMask->Get_Parent()->Set_ImageColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.f)); break;
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION:		pCharacterMask->Get_Parent()->Set_ImageColor(COLOR_EXPLOSION); break;
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_PIERCE:		pCharacterMask->Get_Parent()->Set_ImageColor(COLOR_PIERCE); break;
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:		pCharacterMask->Get_Parent()->Set_ImageColor(COLOR_MYSTERY); break;
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_NORMAL:		pCharacterMask->Get_Parent()->Set_ImageColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f)); break;
		case Engine::E_CHAR_ATKTYPE::ATKTYPE_END:			pCharacterMask->Get_Parent()->Set_ImageColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f)); break;
		default: break;
		}

		// ���ұ� (�̹����� �ٲ����)
		switch (pChar->Get_CharInfo().eRole)
		{
		case Engine::E_CHAR_ROLE::ROLL_NULL:	break;
		case Engine::E_CHAR_ROLE::ROLE_TANKER:		pCharacterMask->Get_Parent()->Get_ChildByIndex(2)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Tanker.png");		break;
		case Engine::E_CHAR_ROLE::ROLE_DEALER:		pCharacterMask->Get_Parent()->Get_ChildByIndex(2)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Dealer.png");		break;
		case Engine::E_CHAR_ROLE::ROLE_HEALER:		pCharacterMask->Get_Parent()->Get_ChildByIndex(2)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Healer.png");		break;
		case Engine::E_CHAR_ROLE::ROLE_SUPPORTER:	pCharacterMask->Get_Parent()->Get_ChildByIndex(2)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Position/Supporter.png");	break;
		case Engine::E_CHAR_ROLE::ROLE_END:		break;
		default:								break;
		}

		// �ִ� ������ ����
		if (pChar->Get_TotalDamage() > iMaxDamage)
		{
			iMaxDamage = pChar->Get_TotalDamage();
		}
	}


	for (_int i = 0; i < 6; ++i)
	{
		CCharacter* pChar = nullptr;
		CUI_Object* pCharLine = m_pDamageReport_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(i);
		if (i < 4)
		{
			pChar = vecStriker[i];
		}
		else
		{
			pChar = vecSpecial[i - 4];
		}
		if (nullptr == pChar)
		{
			pCharLine->Get_ChildByIndex(2)->Set_Active(FALSE);
			pCharLine->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_ImageAlpha(0.f);
			pCharLine->Get_ChildByIndex(0)->Set_Ui_SizeY(10);
			pCharLine->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Ui_Text(L"0");
			continue;
		}
		pCharLine->Get_ChildByIndex(2)->Set_Active(TRUE);


		// ĳ���� �̸��� �޾ƿ´�
		E_CHAR_NAME eCharEnum = pChar->Get_CharInfo().eName[0];
		wstring wstrCharName_Eng = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_NAME);
		wstring wstrCharName_Kor = CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eCharEnum, E_CHAR_ENUMTYPE::CNUM_KORNAME);
		wstring wstrFullPath = L"../../Reference/Resource/Texture/UI/Character_Collection/Character/Skill_Portrait_";

		wstrFullPath.append(wstrCharName_Eng);
		wstrFullPath.append(L".png");

		// ĳ���� �ʻ�ȭ 
		pCharLine->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_ImageAlpha(1.f);
		pCharLine->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Tex1_Tag(wstrFullPath);
		pCharLine->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(wstrCharName_Kor);
		// ������ �׷��� Ratio
		_float fDamageRatio = (pChar->Get_TotalDamage() / (_float)iMaxDamage);
		_int iHeight = (_int)(fDamageRatio * 100.f) + 10;

		pCharLine->Get_ChildByIndex(0)->Set_Ui_SizeY(iHeight);
		pCharLine->Get_ChildByIndex(1)->Set_Ui_PosY(-100 + iHeight);
		pCharLine->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(pChar->Get_TotalDamage()));
	}

	if (CGameManager::GetInstance()->Get_IsBattleWin())
	{
	}
	else
	{
	m_pComVtory_Defeat->Set_Active(TRUE);
	}

}

void CUI_Manager::Active_Combat_Victory()
{

	if (m_pCombatVictoryPanel)
	{
		m_pCombatIngPanel->Set_Active(FALSE);
		if (CGameManager::GetInstance()->Get_IsBattleWin())
		{
			m_pCombatVictoryPanel->Get_ChildByIndex(0)->Set_Active(FALSE);
			m_pCombatVictoryPanel->Get_ChildByIndex(1)->Set_Active(TRUE);
			m_pComVtory_Animation = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pComVtory_Animation);
			m_pComVtory_Animation->Set_Ui_SizeX(512);
			m_pComVtory_Animation->Set_Ui_SizeY(128);
			m_pComVtory_Animation->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Font/ImageFont_Result_Victory.png");
			m_pComVtory_Animation->Add_UiMoving(T_UiMoving(_vec2(0.f, -100.f), 0.4f, E_EASE::InOutQuint));
			m_pComVtory_Animation->Add_UiMoving(T_UiMoving(_vec2(0.f, 100.f), 0.4f, E_EASE::InOutQuint));
			m_pComVtory_Animation->Add_UiMoving(T_UiMoving(_vec2(-400.f, 300.f), 0.8f, E_EASE::InOutQuint));

			m_pComVtory_Animation_fSizeX = 512.f;
			m_pComVtory_Animation_fSizeY = 128.f;
			m_pComVtory_UpdateTimer = 0.f;

			CSound_Manager::GetInstance()->PlayBGM(L"BGM_����_�¸�.ogg", 0.1f);
		}
		else
		{
			m_pCombatVictoryPanel->Get_ChildByIndex(0)->Set_Active(TRUE);
			m_pCombatVictoryPanel->Get_ChildByIndex(1)->Set_Active(FALSE);
			CSound_Manager::GetInstance()->PlayBGM(L"BGM_����_�й�.mp3", 0.1f);
		}
	}
}

void CUI_Manager::Update_Combat_Victory(const _float& fTimeDelta)
{
	m_pComVtory_UpdateTimer += fTimeDelta;

	if (m_pComVtory_Animation)
	{
		if (m_pComVtory_Animation->Get_ImageColor().a <= 0.f)
		{
			m_pComVtory_Animation->Set_Dead(TRUE);
			m_pComVtory_Animation = nullptr;
			return;
		}

		_float fR = CUtility::Clamp_float(m_pComVtory_UpdateTimer - 1.1f, 0.f, 0.5f);
		_float fAlpha = 1.f - (fR * 2.f);

		m_pComVtory_Animation->Set_ImageAlpha(fAlpha);

		if (0.4f < m_pComVtory_UpdateTimer)
		{
			m_pComVtory_Animation_fSizeX -= fTimeDelta * 256.f;
			m_pComVtory_Animation_fSizeX -= fTimeDelta * 64.f;
		}
		else
		{
			m_pComVtory_Animation_fSizeX += fTimeDelta * 2.f * 256.f;
			m_pComVtory_Animation_fSizeX += fTimeDelta * 2.f * 64.f;
		}

		m_pComVtory_Animation->Set_Ui_SizeX((_int)m_pComVtory_Animation_fSizeX);
		m_pComVtory_Animation->Set_Ui_SizeY((_int)m_pComVtory_Animation_fSizeY);
	}
}

void CUI_Manager::Combat_Victory_End_Button()
{
	m_fpComVtory_End_Button(E_SCENETAG::Scene_Lobby);
	m_pCombatVictoryPanel->Set_Active(FALSE);
}

void CUI_Manager::Create_Ui_Story()
{
	m_pStoryPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]���丮â �θ�.uidat");
	m_pStoryPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pStoryPanel->Get_ObjTag(), m_pStoryPanel);

	m_pStory_Main_Button				= m_pStoryPanel->Get_ChildByIndex(0);
	m_pStory_Sub_Button					= m_pStoryPanel->Get_ChildByIndex(1);
	m_pStory_SubParent_Panel			= m_pStoryPanel->Get_ChildByIndex(3);
	m_pStory_Choose_Episode_Panel		= m_pStoryPanel->Get_ChildByIndex(4);

	m_pStory_Choose_Episode_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(3)->
		Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_Story_Part_1, this));

	m_pStory_Choose_Episode_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->
		Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::OnClick_Story_Part_2, this));

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"�ĵ����� ���ؼ� ������ ������\n�ʴ´�!\n�̽Ŀ���ȸ�� �߱��ϴ� ������\n���̶� �����ΰ�?");

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"���� �ָ� �����̵�\n�ذ��ص帳�ϴ�.\n���õ� ������ ���� �پ�ٴϴ�\n��ż��� ����.");

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"������ �����, ������ �����Ѵ�!\n�з��Ͼ��� ���̽��͵��� �����ִ�\n��÷�� ��ũ���!");

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"����� ��Ź�� å������.\n�޾���� �ҳ���� ����ϴ�\n������ ������.");

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"������ õ���� �Բ��ϴ�\n�����ϰ� �ູ�� ������");

	m_pStory_SubParent_Panel->Get_ChildByIndex(0)->Get_ChildByIndex(5)->Get_ChildByIndex(3)->
		Set_Ui_Text(L"�츮�� ��, ���Ǵ�!\n�׻� ���ǰ��� ��Ÿ��\n���ǽ������� ������!");

	m_pStory_Episode_ScrollView_Panel	= m_pStory_Choose_Episode_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(1);
}

void CUI_Manager::Story_OnEnable()
{
	m_pStoryPanel->Set_Active(TRUE);
	m_pWorkPanel->Set_Active(FALSE);

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_���丮.mp3", 0.1f);
	m_pHighPanel_PanelName->Set_Ui_Text(L"���丮");
}

void CUI_Manager::Load_MainStory()
{
}

void CUI_Manager::Load_SubStory()
{
}

void CUI_Manager::OnClick_Story_Part_1()
{
	CDialog_Manager::GetInstance()->Start_DialogEvent(L"1�� 1��");

	m_pHighInfoPanel->Set_Active(FALSE);
}

void CUI_Manager::OnClick_Story_Part_2()
{
	CDialog_Manager::GetInstance()->Start_DialogEvent(L"1�� 2��"); 

	m_pHighInfoPanel->Set_Active(FALSE);
}

void CUI_Manager::Create_Ui_Register()
{
	if (m_pRegisterPanel)
		return;


	m_pRegisterPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]�α��� â �θ�.uidat");
	m_pRegisterPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pRegisterPanel->Get_ObjTag(), m_pRegisterPanel);

	m_pRegister_Edit_Id				= m_pRegisterPanel->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	m_pRegister_Edit_Password		= m_pRegisterPanel->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Get_ChildByIndex(0);
	m_pRegister_Button_Login		= m_pRegisterPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2);
	m_pRegister_Announce			= m_pRegisterPanel->Get_ChildByIndex(1);

	m_pRegister_Edit_Id->Set_Ui_Text(L"");
	m_pRegister_Edit_Password->Set_Ui_Text(L"");

	m_pRegister_Edit_Id->Get_Parent()->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Change_RegisterType, this, (_int)E_RegisterType::Regi_ID), (_int)E_RegisterType::Regi_ID);
	m_pRegister_Edit_Password->Get_Parent()->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Change_RegisterType, this, (_int)E_RegisterType::Regi_PassWord), (_int)E_RegisterType::Regi_PassWord);

	m_pRegister_Button_Login->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Register_Button_Login, this));

	m_pRegister_Edit_Id->Set_Ui_TextColor(_vec4(0.5f, 0.5f, 0.5f, 1.f));
	m_pRegister_Edit_Id->Set_Ui_Text(L"UserName");

	m_pRegister_Edit_Password->Set_Ui_TextColor(_vec4(0.5f, 0.5f, 0.5f, 1.f));
	m_pRegister_Edit_Password->Set_Ui_Text(L"Password");


}

void CUI_Manager::Update_Register(const _float& fTimeDelta)
{
	if (nullptr == m_pRegisterPanel || nullptr == m_pStrID || nullptr == m_pStrPassword)
		return;

	if (CKeyManager::GetInstance()->Key_Down(KEY_ESCAPE))
	{
		m_pRegisterPanel->Set_Active(FALSE);
	}

	if (FALSE == m_pRegisterPanel->Get_Active())
		return;

	if (0.f < m_fRegisterAnnounceTime)
	{
		m_fRegisterAnnounceTime -= fTimeDelta;
		m_pRegister_Announce->Set_Active(TRUE);
		m_pRegister_Announce->Set_Ui_Text(m_wstrCombatIng_AnnounceText);
	}
	else
	{
		m_pRegister_Announce->Set_Active(FALSE);
	}

	if (m_eRegisterType != E_RegisterType::RegisterEnd && CKeyManager::GetInstance()->Key_Down(KEY_Enter))
	{
		Register_Button_Login();
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_Tab))
	{
		if (E_RegisterType::Regi_ID == m_eRegisterType)
		{
			m_eRegisterType = E_RegisterType::Regi_PassWord;
		}
		else if (E_RegisterType::Regi_PassWord == m_eRegisterType)
		{
			m_eRegisterType = E_RegisterType::Regi_ID;
		}
	}

	if (E_RegisterType::RegisterEnd != m_eRegisterType)
	{
		string* pCurrentStr = nullptr;
		switch (m_eRegisterType)
		{
		case CUI_Manager::E_RegisterType::Regi_ID:			pCurrentStr = m_pStrID;				break;
		case CUI_Manager::E_RegisterType::Regi_PassWord:	pCurrentStr = m_pStrPassword;		break;
		}

		if (CKeyManager::GetInstance()->Key_Down(KEY_0)) pCurrentStr->append("0");
		if (CKeyManager::GetInstance()->Key_Down(KEY_1)) pCurrentStr->append("1");
		if (CKeyManager::GetInstance()->Key_Down(KEY_2)) pCurrentStr->append("2");
		if (CKeyManager::GetInstance()->Key_Down(KEY_3)) pCurrentStr->append("3");
		if (CKeyManager::GetInstance()->Key_Down(KEY_4)) pCurrentStr->append("4");
		if (CKeyManager::GetInstance()->Key_Down(KEY_5)) pCurrentStr->append("5");
		if (CKeyManager::GetInstance()->Key_Down(KEY_6)) pCurrentStr->append("6");
		if (CKeyManager::GetInstance()->Key_Down(KEY_7)) pCurrentStr->append("7");
		if (CKeyManager::GetInstance()->Key_Down(KEY_8)) pCurrentStr->append("8");
		if (CKeyManager::GetInstance()->Key_Down(KEY_9)) pCurrentStr->append("9");

		if (CKeyManager::GetInstance()->Key_Down(KEY_Q)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "Q" : "q");
		if (CKeyManager::GetInstance()->Key_Down(KEY_W)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "W" : "w");
		if (CKeyManager::GetInstance()->Key_Down(KEY_E)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "E" : "e");
		if (CKeyManager::GetInstance()->Key_Down(KEY_R)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "R" : "r");
		if (CKeyManager::GetInstance()->Key_Down(KEY_T)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "T" : "t");
		if (CKeyManager::GetInstance()->Key_Down(KEY_Y)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "Y" : "y");
		if (CKeyManager::GetInstance()->Key_Down(KEY_U)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "U" : "u");
		if (CKeyManager::GetInstance()->Key_Down(KEY_I)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "I" : "i");
		if (CKeyManager::GetInstance()->Key_Down(KEY_O)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "O" : "o");
		if (CKeyManager::GetInstance()->Key_Down(KEY_P)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "P" : "p");

		if (CKeyManager::GetInstance()->Key_Down(KEY_A)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "A" : "a");
		if (CKeyManager::GetInstance()->Key_Down(KEY_S)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "S" : "s");
		if (CKeyManager::GetInstance()->Key_Down(KEY_D)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "D" : "d");
		if (CKeyManager::GetInstance()->Key_Down(KEY_F)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "F" : "f");
		if (CKeyManager::GetInstance()->Key_Down(KEY_G)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "G" : "g");
		if (CKeyManager::GetInstance()->Key_Down(KEY_H)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "H" : "h");
		if (CKeyManager::GetInstance()->Key_Down(KEY_J)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "J" : "j");
		if (CKeyManager::GetInstance()->Key_Down(KEY_K)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "K" : "k");
		if (CKeyManager::GetInstance()->Key_Down(KEY_L)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "L" : "l");

		if (CKeyManager::GetInstance()->Key_Down(KEY_Z)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "Z" : "z");
		if (CKeyManager::GetInstance()->Key_Down(KEY_X)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "X" : "x");
		if (CKeyManager::GetInstance()->Key_Down(KEY_C)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "C" : "c");
		if (CKeyManager::GetInstance()->Key_Down(KEY_V)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "V" : "v");
		if (CKeyManager::GetInstance()->Key_Down(KEY_B)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "B" : "b");
		if (CKeyManager::GetInstance()->Key_Down(KEY_N)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "N" : "n");
		if (CKeyManager::GetInstance()->Key_Down(KEY_M)) pCurrentStr->append(CKeyManager::GetInstance()->Key_Down(KEY_Shift) ? "M" : "m");


		if (CKeyManager::GetInstance()->Key_Down(KEY_Back))
		{
			if (pCurrentStr->size() >= 1)
			{
				*pCurrentStr = pCurrentStr->substr(0, (_int)pCurrentStr->size() - 1);
			}
		}
		else if (CKeyManager::GetInstance()->Key_Pressing(KEY_Back))
		{
			m_fRegister_BackSpaceTime += fTimeDelta;
			if (0.1f <= m_fRegister_BackSpaceTime)
			{
				m_fRegister_BackSpaceTime = 0.f;
				if (pCurrentStr->size() >= 1)
				{
					*pCurrentStr = pCurrentStr->substr(0, (_int)pCurrentStr->size() - 1);
				}
			}
		}
		else
			m_fRegister_BackSpaceTime = 0.f;

		switch (m_eRegisterType)
		{
		case CUI_Manager::E_RegisterType::Regi_ID:			
			if (*pCurrentStr == "")
			{
				m_pRegister_Edit_Id->Set_Ui_TextColor(_vec4(0.5f, 0.5f, 0.5f, 1.f));
				m_pRegister_Edit_Id->Set_Ui_Text(L"UserName");
			}
			else
			{
				m_pRegister_Edit_Id->Set_Ui_TextColor(_vec4(0.f, 0.f, 0.f, 1.f));
				m_pRegister_Edit_Id->Set_Ui_Text(CUtility::String2Wstring(*pCurrentStr));
			}

			break;
		case CUI_Manager::E_RegisterType::Regi_PassWord:
			if (*pCurrentStr == "")
			{
				m_pRegister_Edit_Password->Set_Ui_TextColor(_vec4(0.5f, 0.5f, 0.5f, 1.f));
				m_pRegister_Edit_Password->Set_Ui_Text(L"Password");
			}
			else
			{
				m_pRegister_Edit_Password->Set_Ui_TextColor(_vec4(0.f, 0.f, 0.f, 1.f));
				m_pRegister_Edit_Password->Set_Ui_Text(CUtility::String2Wstring(*pCurrentStr));
			}
			break;
		}
	}

}

void CUI_Manager::Change_RegisterType(_int iType)
{
	m_eRegisterType = (E_RegisterType)iType;
}

void CUI_Manager::Register_Button_Login()
{
	if (nullptr == m_fpPlayFab_Login)
		return;

	if (PlayFab_ID_Minimum > m_pStrID->size() || PlayFab_ID_Maximum < m_pStrID->size())
	{
		Register_Set_Announce(1.f, L"���̵��� ���� �� ������ ������ϴ�. (" + to_wstring(PlayFab_ID_Minimum) + L"~" + to_wstring(PlayFab_ID_Maximum) + L")");
	}
	else if (PlayFab_Password_Minimum > m_pStrPassword->size() || PlayFab_Password_Maximum < m_pStrPassword->size())
	{
		Register_Set_Announce(1.f, L"�н������� ���� �� ������ ������ϴ�. (" + to_wstring(PlayFab_Password_Minimum) + L"~" + to_wstring(PlayFab_Password_Maximum) + L")");
	}
	else
	{
		string strRandName = "Rand";
		strRandName.append(to_string(CWall512::GetInstance()->GetValue(0, 1000000)));

		if (m_fpPlayFab_Login)
		{
			m_fpPlayFab_Login();
			*m_pStrID = "";
			*m_pStrPassword = "";
			m_pRegister_Edit_Id->Set_Ui_Text(L"");
			m_pRegister_Edit_Password->Set_Ui_Text(L"");
		}
	}
}

void CUI_Manager::Register_Set_Announce(_float fTime, wstring wstrText)
{
	m_fRegisterAnnounceTime			= fTime;
	m_wstrCombatIng_AnnounceText	= wstrText;
}

void CUI_Manager::Click_LoginRegister()
{
	m_pRegisterPanel->Set_Active(TRUE);
	m_pHighInfoPanel->Set_Active(TRUE);
	m_pHighPanel_PanelName->Set_Ui_Text(L"ȸ������");
}

void CUI_Manager::Create_Ui_Ranking()
{
	if (m_pRankingPanel)
		return;

	m_pRankingPanel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��ŷâ_�θ�.uidat");
	m_pRankingPanel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, m_pRankingPanel->Get_ObjTag(), m_pRankingPanel);

	m_pRanking_LobbyChar		= m_pRankingPanel->Get_ChildByIndex(0)->Get_ChildByIndex(2);
	m_pRanking_NickName			= m_pRankingPanel->Get_ChildByIndex(1);
	m_pRanking_Level			= m_pRankingPanel->Get_ChildByIndex(2);
	m_pRanking_RankingNumber	= m_pRankingPanel->Get_ChildByIndex(3);
	m_pRanking_TierIcon			= m_pRankingPanel->Get_ChildByIndex(4);

	m_pRanking_ScrollView = m_pRankingPanel->Get_ChildByIndex(5);
}

void CUI_Manager::Ranking_Setting()
{
	m_fpPlayFab_UpdateRankInfo();
}

void CUI_Manager::Ranking_UI_Setting()
{

	if (nullptr == m_pRankingPanel || FALSE == m_pRankingPanel->Get_Active())
	{
		return;
	}

	// �÷��̾� ��ŷ ���� ����
	_int iChildSize = (_int)m_pRanking_ScrollView->Get_ChildList()->size();
	_int iVecCount = (_int)m_vecPlayfab_RankingInfo.size();
	_int iDiff = iVecCount - iChildSize;

	for (_int i = 0; i < iDiff; ++i)
	{
		CUI_Object* pRank = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��ŷâ_��ũ�Ѻ�_�ڽ�.uidat");
		m_pRanking_ScrollView->Set_Child(pRank);
		pRank->Set_Active(FALSE);
		pRank->Get_ChildByIndex(12)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Ranking_PickBattle, this, iChildSize + i), iChildSize + i);
		pRank->Get_ChildByIndex(13)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Ranking_Add_Friend, this, iChildSize + i), iChildSize + i);
	}

	for (_int i = 0; i < (_int)m_pRanking_ScrollView->Get_ChildList()->size(); ++i)
	{
		m_pRanking_ScrollView->Get_ChildByIndex(i)->Set_Active(i < iVecCount);
	}
	
	wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
		CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)CDataManager::GetInstance()->Get_UserInfo().eLobby_Char, E_CHAR_ENUMTYPE::CNUM_NAME) +
		L".png";

	m_pRanking_LobbyChar->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

	wstring wstrName = wstring(CDataManager::GetInstance()->Get_UserInfo().wszNickName);
	m_pRanking_NickName->Set_Ui_Text(wstrName);
	m_pRanking_Level->Set_Ui_Text(to_wstring(CDataManager::GetInstance()->Get_UserInfo().iLevel));
	
	wstring wstrTierIconPath = L"../../Reference/Resource/Texture/UI/Rank/Arena_RankIcon_";
	if (m_iPlayerRank <= 1)			wstrTierIconPath.append(L"04.png");
	else if (m_iPlayerRank <= 3)	wstrTierIconPath.append(L"03.png");
	else if (m_iPlayerRank <= 5)	wstrTierIconPath.append(L"02.png");
	else							wstrTierIconPath.append(L"01.png");
	m_pRanking_TierIcon->Set_Tex1_Tag(wstrTierIconPath);
	m_pRanking_TierIcon->Set_Ui_Text(to_wstring(m_iPlayerRank) + L"��");

	
	// ��ŷ ����
	for (_int i = 0; i < iVecCount; ++i)
	{
		T_PlayfabInfo& tInfo = m_vecPlayfab_RankingInfo[i];
		CUI_Object* pUiObject = m_pRanking_ScrollView->Get_ChildByIndex(i);

		wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
			CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)tInfo.iLobbyChar, E_CHAR_ENUMTYPE::CNUM_NAME) +
			L".png";

		pUiObject->Get_ChildByIndex(1)->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);

		pUiObject->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(tInfo.iRank) + L"��");
		pUiObject->Get_ChildByIndex(3)->Set_Ui_Text(to_wstring(tInfo.iBossScore[0]));
		pUiObject->Get_ChildByIndex(5)->Set_Ui_Text(tInfo.wstrNickname);
		pUiObject->Get_ChildByIndex(7)->Set_Ui_Text(L"Lv." + to_wstring(tInfo.iAccountLevel));

		// �����̶�� ���� ��ư ����
		pUiObject->Get_ChildByIndex(12)->Set_Active(tInfo.wstrNickname != CDataManager::GetInstance()->Get_NickName());
		// �����̶�� ģ���߰� ��ư ����
		pUiObject->Get_ChildByIndex(13)->Set_Active(tInfo.wstrNickname != CDataManager::GetInstance()->Get_NickName());

		for (_int i = 0; i < 6; ++i)
		{
			CUI_Object* pCharSlot = pUiObject->Get_ChildByIndex(10)->Get_ChildByIndex(i);
			if (tInfo.iSquadChar_Enum[i] == -1)
			{
				pCharSlot->Get_ChildByIndex(1)->Set_Active(FALSE);
			}
			else
			{
				pCharSlot->Get_ChildByIndex(1)->Set_Active(TRUE);
				wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Collection/" +
					CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)tInfo.iSquadChar_Enum[i], E_CHAR_ENUMTYPE::CNUM_NAME) +
					L".png";
				pCharSlot->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrTexName);
				for (_int j = 0; j < 5; j++)
				{
					pCharSlot->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(j)->
						Set_Ui_Text(L"Lv." + to_wstring(tInfo.iSquadChar_Level[i]));
				}
				pCharSlot->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(tInfo.iSquadChar_Star[i]));

				wstring wstrRoleTexPath = L"";
				switch (CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)tInfo.iSquadChar_Enum[i]).eRole)
				{
				case E_CHAR_ROLE::ROLE_DEALER:
					wstrRoleTexPath = L"Dealer";
					break;
				case E_CHAR_ROLE::ROLE_HEALER:
					wstrRoleTexPath = L"Healer";
					break;
				case E_CHAR_ROLE::ROLE_SUPPORTER:
					wstrRoleTexPath = L"Supporter";
					break;
				case E_CHAR_ROLE::ROLE_TANKER:
					wstrRoleTexPath = L"Tanker";
					break;
				default:
					break;
				}
				wstrRoleTexPath = L"../../Reference/Resource/Texture/UI/Position/" + wstrRoleTexPath + L".png";
				pCharSlot->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Tex1_Tag(wstrRoleTexPath);
			}
		}

		wstrTierIconPath = L"../../Reference/Resource/Texture/UI/Rank/Arena_RankIcon_";
		if (tInfo.iRank <= 1)			wstrTierIconPath.append(L"04.png");
		else if (tInfo.iRank <= 3)		wstrTierIconPath.append(L"03.png");
		else if (tInfo.iRank <= 5)		wstrTierIconPath.append(L"02.png");
		else							wstrTierIconPath.append(L"01.png");

		CUI_Object* pRankIcon = pUiObject->Get_ChildByIndex(11);
		pRankIcon->Set_Tex1_Tag(wstrTierIconPath);
	}
}


void CUI_Manager::Click_Ranking()
{
	m_pRankingPanel->Set_Active(TRUE);
	Ranking_Setting();
}

void CUI_Manager::Ranking_Set_Rank(const vector<T_PlayfabInfo>& vecRankInfo, _int iPlayerPoint, _int iPlayerRank)
{
	m_vecPlayfab_RankingInfo.clear();
	m_vecPlayfab_RankingInfo	= vecRankInfo;
	m_iPlayerRank				= iPlayerRank;
	m_iPlayerPoint				= iPlayerPoint;
}

void CUI_Manager::FriendList_Set(const vector<T_PlayfabInfo>& vecFrinedList)
{
	m_vecPlayfab_FriendInfo.clear();
	m_vecPlayfab_FriendInfo = vecFrinedList;

	if (FALSE == m_vecPlayfab_FriendInfo.empty())
	{
		m_pLobby_FriendList->Set_Active(TRUE);
	}
	Set_FriendList();
}

void CUI_Manager::Ranking_PickBattle(_int iRank)
{
	for (_int i = 0; i < 4; ++i)
	{
		CGameManager::GetInstance()->m_iEnemyUserSquad[i] = m_vecPlayfab_RankingInfo[iRank].iSquadChar_Enum[i];
		CGameManager::GetInstance()->m_iEnemyUserSquadLevel[i] = m_vecPlayfab_RankingInfo[iRank].iSquadChar_Level[i];
	}
	CGameManager::GetInstance()->m_bUserBattle = TRUE;
	Mission_Start();
}

void CUI_Manager::Ranking_Add_Friend(_int iRank)
{
	m_strFriendPlayFabId = m_vecPlayfab_RankingInfo[iRank].strPlayFabID;
	m_fpPlayFab_AddFriend();
}

#pragma endregion

#pragma region ����_TPS ���� UI 

void CUI_Manager::Create_Run_Combat_UI()
{
	m_pRunPlayerHpBar = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Run_Player_HpBar.uidat");
	assert(m_pRunPlayerHpBar); 
	CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Combat)->Add_GameObject(m_pRunPlayerHpBar->Get_ObjTag(), m_pRunPlayerHpBar);
	m_pRunPlayerHpBar->Set_Active(TRUE);
	m_pRunPlayerHpBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(L"100/100"); 

	m_pRunPlayerSkillBar = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Run_Player_Skill.uidat"); 
	assert(m_pRunPlayerSkillBar); 
	CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Combat)->Add_GameObject(m_pRunPlayerSkillBar->Get_ObjTag(), m_pRunPlayerSkillBar); 
	m_pRunPlayerSkillBar->Set_Active(TRUE); 
	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ResetMask(FALSE); 
	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Mask(VEC2_ZERO, _vec2(m_fRunGame_SkillBar, 1.f)); 

	m_pRunPlayerComboHit = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Run_ComboHit.uidat"); 
	assert(m_pRunPlayerComboHit); 
	CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Combat)->Add_GameObject(m_pRunPlayerComboHit->Get_ObjTag(), m_pRunPlayerComboHit);
	m_pRunPlayerComboHit->Set_Active(FALSE); 

	m_pRunPlayer_EndGameUI = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Run_��������.uidat"); 
	assert(m_pRunPlayer_EndGameUI); 
	CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Combat)->Add_GameObject(m_pRunPlayer_EndGameUI->Get_ObjTag(), m_pRunPlayer_EndGameUI); 
	m_pRunPlayer_EndGameUI->Set_Active(FALSE); 

	m_pRunPlayer_EndGameUI->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Goto_BeforePanel_UI, this)); 

	m_pRunPlayer_HurtScreenUI = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]����ȿ��.uidat"); 
	assert(m_pRunPlayer_HurtScreenUI); 
	CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Combat)->Add_GameObject(m_pRunPlayer_HurtScreenUI->Get_ObjTag(), m_pRunPlayer_HurtScreenUI); 
	m_pRunPlayer_HurtScreenUI->Set_Active(FALSE); 
}

void CUI_Manager::Update_RunGame_UI(const _float & fTimeDelta)
{
	if (!m_pRunPlayerComboHit) 
	{
		return; 
	}
	if (!m_pRunPlayerComboHit->Get_Active())
	{
		return;
	}

	m_fRunGame_ComboHit_Delay -= fTimeDelta; 

	if (0.f >= m_fRunGame_ComboHit_Delay) 
	{
		m_iRunGame_HitCount = 0;
		m_fRunGame_ComboHit_Delay = m_fRunGame_ComboHit_OriginDelay;

		m_pRunPlayerComboHit->Set_Active(FALSE); 
	}

	if (m_pRunPlayer_HurtScreenUI)
	{
		if (0.f >= m_pRunPlayer_HurtScreenUI->Get_ImageColor().a)
		{
			m_pRunPlayer_HurtScreenUI->Set_Active(FALSE); 
		}
	}
}

void CUI_Manager::Set_RunSkillBar(const _float & fValue)
{
	if (!m_pRunPlayerSkillBar)
	{
		return; 
	}

	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);

	m_fRunGame_SkillBar = fValue;
	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Mask(VEC2_ZERO, _vec2(m_fRunGame_SkillBar, 1.f));
}

void CUI_Manager::Add_RunSkillBar(const _float & fValue)
{
	if (!m_pRunPlayerSkillBar)
	{
		return;
	}

	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ResetMask(FALSE);

	m_fRunGame_SkillBar += fValue; 
	m_pRunPlayerSkillBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Mask(VEC2_ZERO, _vec2(m_fRunGame_SkillBar, 1.f)); 
}

void CUI_Manager::Add_RunComboHit(const _uint & iComboHit)
{
	if (!m_pRunPlayerComboHit)
	{
		return; 
	}

	m_pRunPlayerComboHit->Set_Active(FALSE); 
	m_pRunPlayerComboHit->Set_Active(TRUE); 

	m_fRunGame_ComboHit_Delay = m_fRunGame_ComboHit_OriginDelay; 
	m_iRunGame_HitCount += iComboHit; 

	m_pRunPlayerComboHit->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iRunGame_HitCount)); 
}

void CUI_Manager::Get_ApplyRunPlayerHpText() 
{
	if (!m_pRunPlayerHpBar || !CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player)) 
	{
		return; 
	}

	CRun_Unit*	pPlayer = static_cast<CRun_Unit*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	m_pRunPlayerHpBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->
		Set_Ui_Text(to_wstring(pPlayer->Get_UnitInfo().iNowHp) + L"/" + to_wstring(pPlayer->Get_UnitInfo().iMaxHp));
	m_pRunPlayerHpBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Mask(VEC2_ZERO, _vec2((_float)pPlayer->Get_UnitInfo().iNowHp / pPlayer->Get_UnitInfo().iMaxHp, 1.f));
	m_pRunPlayerHpBar->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ResetMask(FALSE); 
}

void CUI_Manager::Set_ClearAll_RunCombatUI()
{
	if (m_pRunPlayerHpBar)
	{
		m_pRunPlayerHpBar->Set_Dead(TRUE); 
		m_pRunPlayerHpBar = nullptr; 
	}
	if (m_pRunPlayerSkillBar) 
	{
		m_pRunPlayerSkillBar->Set_Dead(TRUE); 
		m_pRunPlayerSkillBar = nullptr;
	}
	if (m_pRunPlayerComboHit) 
	{
		m_pRunPlayerComboHit->Set_Dead(TRUE); 
		m_pRunPlayerComboHit = nullptr; 
	}
	if (m_pRunPlayer_HurtScreenUI)
	{
		m_pRunPlayer_HurtScreenUI->Set_Dead(TRUE); 
		m_pRunPlayer_HurtScreenUI = nullptr; 
	}
}

void CUI_Manager::Active_RunEndGameUI(const _bool& bWin)
{
	if (!m_pRunPlayer_EndGameUI)
	{
		return; 
	}

	if (m_pRunPlayerHpBar)
	{
		m_pRunPlayerHpBar->Set_Active(FALSE); 
	}
	if (m_pRunPlayerSkillBar) 
	{
		m_pRunPlayerSkillBar->Set_Active(FALSE); 
	}

	if (0.f == m_pRunPlayer_EndGameUI->Get_ChildByIndex(0)->Get_ImageColor().a)
	{
		m_pRunPlayer_EndGameUI->Get_ChildByIndex(0)->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, 1.f);
		m_pRunPlayer_EndGameUI->Get_ChildByIndex(4)->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, 1.5f);
	}

	m_pRunPlayer_EndGameUI->Set_Active(TRUE); 
	m_pRunPlayer_EndGameUI->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(CDataManager::GetInstance()->Get_NickName()); 
	m_pRunPlayer_EndGameUI->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"Lv." + to_wstring(CDataManager::GetInstance()->Get_Level())); 
	m_pRunPlayer_EndGameUI->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE); 

	_uint	iCount = (_uint)CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI)->size(); 
	for (_uint i = 0; i < iCount; ++i) 
	{
		CUI_Object*	pUi = static_cast<CUI_Object*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, i)); 
		if (L"���̵�" == pUi->Get_Name())
		{
			pUi->Set_Dead(TRUE); 
		}
	}

	if (bWin)
	{
		m_pRunPlayer_EndGameUI->Get_ChildByIndex(0)->Set_Active(FALSE); 
		m_pRunPlayer_EndGameUI->Get_ChildByIndex(4)->Set_Active(TRUE); 
	}
}

void CUI_Manager::Goto_BeforePanel_UI()
{
	m_fpComVtory_End_Button(E_SCENETAG::Scene_Lobby); 

	m_pRunPlayer_EndGameUI->Set_Dead(TRUE); 
	m_pRunPlayer_EndGameUI = nullptr; 
}

void CUI_Manager::Active_RunHurtScreenUI()
{
	if (!m_pRunPlayer_HurtScreenUI)
	{
		return; 
	}

	m_pRunPlayer_HurtScreenUI->Set_Active(TRUE); 
	m_pRunPlayer_HurtScreenUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, 2.f); 
}

void CUI_Manager::Create_Gacha_UI()
{
	if (m_pGacha_Panel)
		return;

	m_pGacha_Panel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��í â �θ�.uidat");
	m_pGacha_Panel->Set_Active(FALSE);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, E_OBJTAG::OBJ_UI, m_pGacha_Panel);
	
	// ���
	m_pGacha_Background			= m_pGacha_Panel->Get_ChildByIndex(1)->Get_ChildByIndex(0);

	// �Ⱦ�ĳ���� �̹���
	m_pGacha_Aru_Big			= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(0);
	m_pGacha_Aru_Small			= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(3);
	m_pGacha_Wakamo_Big			= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(1);
	m_pGacha_Wakamo_Small		= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(4);
	m_pGacha_Shiroko_Big		= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(2);
	m_pGacha_Shiroko_Small		= m_pGacha_Panel->Get_ChildByIndex(2)->Get_ChildByIndex(5);

	// �Ⱦ� ��ư
	m_pGacha_High_Button_1		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	m_pGacha_High_Button_1->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Change_GachaType, this, 1), 1);
	m_pGacha_High_Button_2		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(1);
	m_pGacha_High_Button_2->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Change_GachaType, this, 2), 2);
	m_pGacha_High_Button_3		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(2);
	m_pGacha_High_Button_3->Add_Function_int(E_UiFunctionType::OnClick, bind(&CUI_Manager::Change_GachaType, this, 3), 3);

	m_pGacha_High_Line_1		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(3);
	m_pGacha_High_Line_2		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(4);
	m_pGacha_High_Line_3		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(5);

	m_pGacha_Center_Line_1		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(0);
	m_pGacha_Center_Name		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(1);
	m_pGacha_Center_Line_2		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(2);
	m_pGacha_Center_Line_3		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(3);
	m_pGacha_Button_Gacha_1		= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(4);
	m_pGacha_Button_Gacha_10	= m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(6)->Get_ChildByIndex(5);

	m_pGacha_Button_Start		= m_pGacha_Panel->Get_ChildByIndex(4)->Get_ChildByIndex(1);
	m_pGacha_Button_Start->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Gacha_Start, this));

}

void CUI_Manager::OnEnable_GachaPanel()
{
	m_pLobbyPanel->Set_Active(FALSE);
	m_pHighInfoPanel->Set_Active(TRUE);
	m_pHighPanel_PanelName->Set_Ui_Text(L"����");

	Change_GachaType(1);
	m_pGacha_Panel->Set_Active(TRUE);
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_�ý���_����.mp3", 0.1f);
}

void CUI_Manager::Change_GachaType(_int iType)
{
	m_pGacha_Aru_Big			->Set_Active(FALSE);
	m_pGacha_Aru_Small			->Set_Active(FALSE);
	m_pGacha_Wakamo_Big			->Set_Active(FALSE);
	m_pGacha_Wakamo_Small		->Set_Active(FALSE);
	m_pGacha_Shiroko_Big		->Set_Active(FALSE);
	m_pGacha_Shiroko_Small		->Set_Active(FALSE);
	m_pGacha_High_Line_1		->Set_Active(FALSE);
	m_pGacha_High_Line_2		->Set_Active(FALSE);
	m_pGacha_High_Line_3		->Set_Active(FALSE);

	D3DXCOLOR colorNotActive = D3DXCOLOR(169.f / 255.f, 224.f / 255.f, 245.f / 255.f, 1.f);
	D3DXCOLOR colorActive = D3DXCOLOR(242.f / 255.f, 252.f / 255.f, 253.f / 255.f, 1.f);
	m_pGacha_High_Button_1->Set_ImageColor(colorNotActive);
	m_pGacha_High_Button_2->Set_ImageColor(colorNotActive);
	m_pGacha_High_Button_3->Set_ImageColor(colorNotActive);

	m_pGacha_Center_Line_3->Set_Ui_Text(L"10ȸ ���� ��, ��2 �̻� �л� 1�� Ȯ��!\n�� �̹� ������ �л��� �����׸��� �ش� �л��� �������� ��ȯ�˴ϴ�.");
	switch (iType)
	{
	case 1:
		m_pGacha_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Event_BackGround/BG_KohshinjoRoom.jpg");
		m_pGacha_Aru_Big->Set_Active(TRUE);
		m_pGacha_Aru_Small->Set_Active(TRUE);
		m_pGacha_High_Line_2->Set_Active(TRUE);
		m_pGacha_High_Line_3->Set_Active(TRUE);
		m_pGacha_High_Button_1->Set_ImageColor(colorActive);
		m_pGacha_Center_Line_1->Set_Ui_Text(L"2000/01/01 ���� ��~3000/01/01 12:00����");
		m_pGacha_Center_Name->Set_Ui_Text(L"�Ⱦ� ����!");
		m_pGacha_Center_Line_2->Set_Ui_Text(L"�Ʒ�(��3) ���� Ȯ�� UP!");
		break;
	case 2:
		m_pGacha_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Event_BackGround/Event_Image_801.png");
		m_pGacha_Wakamo_Big->Set_Active(TRUE);
		m_pGacha_Wakamo_Small->Set_Active(TRUE);
		m_pGacha_High_Line_3->Set_Active(TRUE);
		m_pGacha_High_Button_2->Set_ImageColor(colorActive);
		m_pGacha_Center_Line_1->Set_Ui_Text(L"2000/12/12 ���� ��~4000/12/12 12:00����");
		m_pGacha_Center_Name->Set_Ui_Text(L"�Ⱦ� ����!");
		m_pGacha_Center_Line_2->Set_Ui_Text(L"��ī��(��3) ���� Ȯ�� UP!");
		break;
	case 3:
		m_pGacha_Background->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Event_BackGround/BG_ClassRoom.jpg");
		m_pGacha_Shiroko_Big->Set_Active(TRUE);
		m_pGacha_Shiroko_Small->Set_Active(TRUE);
		m_pGacha_High_Line_1->Set_Active(TRUE);
		m_pGacha_High_Button_3->Set_ImageColor(colorActive);
		m_pGacha_Center_Line_1->Set_Ui_Text(L"1111/12/12 ���� ��~9999/12/12 12:00����");
		m_pGacha_Center_Name->Set_Ui_Text(L"��� ����");
		m_pGacha_Center_Line_2->Set_Ui_Text(L"�پ��� �л����� �����ϼ���!");
		break;
	}
}

void CUI_Manager::Gacha_Start()
{
	m_iGachaTime += 10;
	m_pGacha_Panel->Get_ChildByIndex(3)->Get_ChildByIndex(0)->Get_ChildByIndex(7)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iGachaTime));
	Gacha_Roulette();
	m_pLobbyPanel->Set_Active(FALSE);
	m_pGacha_Panel->Set_Active(FALSE);
	m_pGachaResult_Panel->Set_Active(TRUE);
	m_pGacha_Button_Start->Get_Parent()->Set_Active(FALSE);
	Change_GachaResultType(E_GachaResultType::Start_Direct);

	for (_int i = 0; i < 10; i++)
	{
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeX(m_iResultSize_Start);
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeY(m_iResultSize_Start);
	}

	if (m_pHighInfoPanel)
	{
		m_pHighInfoPanel->Set_Active(FALSE);
	}
}

void CUI_Manager::Update_GachaPanelFor_CreateParticle(const _float& fTimeDelta)
{
	if (nullptr == m_pGacha_Panel || FALSE == m_pGacha_Panel->Get_Active())
		return;
}

void CUI_Manager::Create_GachaResult_UI()
{
	if (m_pGachaResult_Panel)
		return;

	m_pGachaResult_Panel = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��í ���â �θ�.uidat");
	m_pGachaResult_Panel->Set_Active(FALSE);
	m_pGachaResult_Panel->Set_Order_In_Layer(10);
	CScene::Get_MainScene()->Add_GameObjectNextFrame_ForLoading(E_LAYERTAG::UI_Lobby, E_OBJTAG::OBJ_UI, m_pGachaResult_Panel);

	// ��í ���� �ִϸ��̼� �θ�
	m_pGachaResult_StartDirectParent	= m_pGachaResult_Panel->Get_ChildByIndex(0);
	m_pGachaResult_StartDirectParent->Set_Active(FALSE);
	m_pGachaResult_StartDirect_Circle	= m_pGachaResult_StartDirectParent->Get_ChildByIndex(0);
	m_pGachaResult_StartDirect_SRank	= m_pGachaResult_StartDirectParent->Get_ChildByIndex(1);
	m_pGachaResult_StartDirect_Fade		= m_pGachaResult_StartDirectParent->Get_ChildByIndex(2);

	// ����â
	m_pGachaResult_SignParent			= m_pGachaResult_Panel->Get_ChildByIndex(1);
	m_pGachaResult_SignParent->Set_Active(FALSE);
	m_pGachaResult_InSide				= m_pGachaResult_SignParent->Get_ChildByIndex(1);
	

	// 10�� â �θ�
	m_pGachaResult_TenParent = m_pGachaResult_Panel->Get_ChildByIndex(2);
	for (_int i = 0; i < 10; i++)
	{
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_PosX(-420 + ((i % 5) * 210));
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeX(m_iResultSize_Start);
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeY(m_iResultSize_Start);
		m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Rotation(E_ROTATION::ROT_Z, 10.f);
	}

	m_pGachaResult_OneResult = m_pGachaResult_Panel->Get_ChildByIndex(3);
	m_pGachaResult_OneResult->Set_Active(FALSE);
	m_pGachaResult_One_RareCard			= m_pGachaResult_OneResult->Get_ChildByIndex(0);
	m_pGachaResult_One_StudentParent	= m_pGachaResult_OneResult->Get_ChildByIndex(1);
	m_pGachaResult_One_Description		= m_pGachaResult_OneResult->Get_ChildByIndex(2);		// -400, -160
	m_pGachaResult_One_Name_Parent		= m_pGachaResult_OneResult->Get_ChildByIndex(3);		// -470, -180
	m_pGachaResult_One_Name_Star1		= m_pGachaResult_One_Name_Parent->Get_ChildByIndex(1);
	m_pGachaResult_One_Name_Star2		= m_pGachaResult_One_Name_Parent->Get_ChildByIndex(2);
	m_pGachaResult_One_Name_Star3		= m_pGachaResult_One_Name_Parent->Get_ChildByIndex(3);
	m_pGachaResult_One_CharInfoParent	= m_pGachaResult_OneResult->Get_ChildByIndex(4);		// 350, 0
	m_pGachaResult_One_Skip				= m_pGachaResult_OneResult->Get_ChildByIndex(5);		// 350, 0
	m_pGachaResult_One_Skip->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Skip_Gacha, this));


	m_pGachaResult_FinalResult = m_pGachaResult_Panel->Get_ChildByIndex(4);
	m_pGachaResult_FinalResult->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CUI_Manager::Exit_Gacha, this));
	m_pGachaResult_FinalResult->Set_Active(FALSE);

	m_pGachaResult_BlackMask = m_pGachaResult_Panel->Get_ChildByIndex(5);
	m_pGachaResult_BlackMask->Set_Active(FALSE);

	CUI_Object* pExitButton = m_pGachaResult_FinalResult->Get_ChildByIndex(0);
	m_pGachaResult_FinalResult->Clear_Child();
	for (_int i = 0; i < 10; i++)
	{
		m_pGachaResult_Prefab[i] = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]��í ������� Prefab.uidat");
		m_pGachaResult_Prefab[i]->Set_Ui_PosX(-420 + ((i % 5) * 210));
		m_pGachaResult_Prefab[i]->Set_Ui_PosY(150 + ((i / 5) * -250));
		m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_PosX(3);
		m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_PosY(16);
		m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_SizeX(172);
		m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Ui_SizeY(142);

		m_pGachaResult_FinalResult->Set_Child(m_pGachaResult_Prefab[i]);
	}
	m_pGachaResult_FinalResult->Set_Child(pExitButton);

	// ��í�� ���� �� �ִ� ���� �߰�
	m_vecGachaUnit_R.push_back((_int)E_CHAR_NAME::Name_Suzumi);
	m_vecGachaUnit_R.push_back((_int)E_CHAR_NAME::Name_Fuuka);
	m_vecGachaUnit_R.push_back((_int)E_CHAR_NAME::Name_Hibiki);

	m_vecGachaUnit_SR.push_back((_int)E_CHAR_NAME::Name_Momoi);
	m_vecGachaUnit_SR.push_back((_int)E_CHAR_NAME::Name_Sumire);
	m_vecGachaUnit_SR.push_back((_int)E_CHAR_NAME::Name_Pina);

	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Neru);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Aru);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Wakamo);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Aris);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Hina);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Shun);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Iori);
	m_vecGachaUnit_SSR.push_back((_int)E_CHAR_NAME::Name_Shiroko);
}

void CUI_Manager::Gacha_Roulette()
{
	for (_int i = 0; i < 10; ++i)
	{
		// 3�� Ȯ�� 2.5%
		// 2�� Ȯ�� 9%
		// �� �� 1��
		_float fRand = CWall512::GetInstance()->GetFloatValue(0.f, 100.f);
		_int iRandUnit = -1;
		_int iRandTier = -1;
		if (fRand <= 25.f)
		{
			iRandUnit = m_vecGachaUnit_R[CWall512::GetInstance()->GetValue(0, (_int)m_vecGachaUnit_R.size())];
			iRandTier = 1;
		}
		else if (fRand <= 25.f + 25.f)
		{
			iRandUnit = m_vecGachaUnit_SR[CWall512::GetInstance()->GetValue(0, (_int)m_vecGachaUnit_SR.size())];
			iRandTier = 2;
		}
		else
		{
			iRandUnit = m_vecGachaUnit_SSR[CWall512::GetInstance()->GetValue(0, (_int)m_vecGachaUnit_SSR.size())];
			iRandTier = 3;
		}

		m_iArrGachaResultUnit[i] = iRandUnit;
		m_iArrGachaResultTier[i] = iRandTier;
	}

	for (_int i = 0; i < 10; i++)
	{
		if (m_iArrGachaResultTier[i] == 3)
		{
			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_SSRCard.png");
		}
		else if(m_iArrGachaResultTier[i] == 2)
		{
			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_SRCard.png");
		}
		else
		{
			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_RCard_1.png");
		}
	}
}

void CUI_Manager::Update_GachaResult(const _float & fTimeDelta)
{
	if (nullptr == m_pGachaResult_Panel || FALSE == m_pGachaResult_Panel->Get_Active())
		return;


	m_fGachaResultTime += fTimeDelta;

	switch (m_eResultType)
	{
	case E_GachaResultType::Start_Direct:
	{
		
		if (5.f <= m_fGachaResultTime)
		{
			Change_GachaResultType(Sign);
		}
		else if (3.f <= m_fGachaResultTime)
		{
			m_pGachaResult_StartDirect_Circle->Set_Active(FALSE);
			m_pGachaResult_StartDirect_SRank->Set_Active(FALSE);
			m_pGachaResult_StartDirect_Fade->Set_Active(TRUE);
		}
		break;
	}
	case E_GachaResultType::Sign:
		m_fGachaFocusCoolTime += fTimeDelta;
		if (0.7f <= m_fGachaFocusCoolTime)
		{
			m_fGachaFocusCoolTime = 0.f;
			_vec2 vUiPos = _vec2(WINCX * 0.5f, WINCY * 0.5f);
			CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Gacha_Focus_Rotation.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.05f);
			_float fRandZ = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);
			static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_Start	= _vec3(0.f, 0.f, fRandZ);
			static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_End		= _vec3(0.f, 0.f, fRandZ);
		}

		if (m_bGachaSignMask)
		{
			m_fGachaSignMaskTime += fTimeDelta;

			m_pGachaResult_BlackMask->Set_ImageAlpha(m_fGachaSignMaskTime);

			if (1.f <= m_fGachaSignMaskTime)
			{
				m_pUiSkillObject_GachaInside->Set_Dead(TRUE);
				Change_GachaResultType(E_GachaResultType::TenGacha);
			}
		}
		if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
		{
			m_pGachaResult_BlackMask->Set_Active(FALSE);
			m_bGachaSignMask = TRUE;
		}
		break;
	case E_GachaResultType::TenGacha:
	{
		// 1.5�ʿ� ���� (0.15�ʸ��� 0.4�ʿ� ���� �̵�)
		for (_int i = 0; i < 10; ++i)
		{
			_float fFinalY = 150.f + ((i / 5) * -250.f);
			_float fRatio = CUtility::Clamp_float((m_fGachaResultTime - (0.15f * i)) * 2.5f, 0.f, 1.f);

			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_PosY((_int)(fFinalY + 1000.f * (1.f - fRatio)));
			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeX((_int)CUtility::Lerp_float((_float)m_iResultSize_Start, (_float)m_iResultSize_End, fRatio));
			m_pGachaResult_TenParent->Get_ChildByIndex(i)->Set_Ui_SizeY((_int)CUtility::Lerp_float((_float)m_iResultSize_Start, (_float)m_iResultSize_End, fRatio));

			if (1.f <= fRatio && FALSE == m_bArrGachaResultFirstEffect[i] && 2 <= m_iArrGachaResultTier[i])
			{
				_vec2 vUiPos = m_pGachaResult_TenParent->Get_ChildByIndex(i)->Get_Ui_Pos();
				vUiPos.x += WINCX * 0.5f;
				vUiPos.y += WINCY * 0.5f;
				m_bArrGachaResultFirstEffect[i] = TRUE;
				CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Gacha_Ten_RareEffect.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.05f);
				if (m_iArrGachaResultTier[i] == 2)
				{
					static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_Start = _vec4(1.f, 1.f, 0.f, 1.f);
					static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_End	= _vec4(1.f, 1.f, 0.5f, 0.f);
					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Result_SR.wav", 1.f);
				}
				else if (m_iArrGachaResultTier[i] == 1)
				{
					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Result_R.wav", 1.f);
				}
				else if(m_iArrGachaResultTier[i] == 3)
				{
					static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_Start = _vec4(1.f, 0.f, 1.f, 1.f);
					static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_End	= _vec4(1.f, 0.f, 0.749f, 0.f);
					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Result_SSR.wav", 1.f);
				}
			}
		}

		if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
		{
			Change_GachaResultType(E_GachaResultType::Result);
		}
		break;
	}
	case E_GachaResultType::Result:
		Update_GachaAnimation();

		if (m_iGachaOneAnim == 10)
		{
			Change_GachaResultType(E_GachaResultType::FinalResult);
			m_iGachaOneAnim = 0;
		}
		break;
	case E_GachaResultType::FinalResult:
	{
		_float fRotRatio = CUtility::Clamp_float(m_fGachaResultTime * 0.5f, 0.f, 1.f);
		for (_int i = 0; i < 10; i++)
		{
			m_pGachaResult_Prefab[i]->Set_Active(TRUE);
			m_pGachaResult_Prefab[i]->Set_Rotation(E_ROTATION::ROT_Y, (1.f - fRotRatio) * 360.f);

			if (2 <= m_iArrGachaResultTier[i])
			{
				m_fArrGachaEffectCool[i] -= fTimeDelta;
				if (m_fArrGachaEffectCool[i] <= 0.f)
				{
					m_fArrGachaEffectCool[i] = CWall512::GetInstance()->GetFloatValue(0.2f, 0.5f);

					for (_int j = 0; j < (_int)CWall512::GetInstance()->GetValue(1, 4); ++j)
					{
						_float fPosX = -420.f + ((i % 5) * 210.f) + (WINCX * 0.5f);
						_float fPosY = 150.f + ((i / 5) * -250.f) + (WINCY * 0.5f);
						_float fRandAngle = 0.f;

						fPosX += CWall512::GetInstance()->GetFloatValue(-50.f, 50.f);
						if (0 == CWall512::GetInstance()->GetValue(0, 2))
						{
							fRandAngle = 1.f;
							fPosY += 192.f * 0.5f;
						}
						else
						{
							fRandAngle = -1.f;
							fPosY -= 192.f * 0.5f;
						}
						D3DXCOLOR color;
						if(m_iArrGachaResultTier[i] == 2)
						{
							color = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
						}
						else
						{
							color = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);
						}
						_vec2 vPos = _vec2(fPosX, fPosY);
						_vec3 vDir = _vec3(0.f, sinf(fRandAngle), 0.f);
						Gacha_Particle(vPos, vDir, 0.3f, 0.9f, color);
					}
				}
			}
		}
		break;
	}
	case E_GachaResultType::Gacha_End:
		break;
	}
}

void CUI_Manager::Exit_Gacha()
{
	Change_GachaResultType(E_GachaResultType::Gacha_End);
	m_pGacha_Panel->Set_Active(TRUE);
	m_pLobbyPanel->Set_Active(FALSE);
	m_pGachaResult_Panel->Set_Active(FALSE);
	m_pGacha_Button_Start->Get_Parent()->Set_Active(FALSE);

	if (m_pHighInfoPanel)
	{
		m_pHighInfoPanel->Set_Active(TRUE);
	}
}

void CUI_Manager::Skip_Gacha()
{
	m_iGachaOneAnim = 10;
}

void CUI_Manager::Change_GachaResultType(E_GachaResultType ResultType)
{
	ZeroMemory(m_bArrGachaResultFirstEffect, sizeof(_bool) * 10);

	m_eResultType			= ResultType;
	m_fGachaResultTime		= 0.f;
	m_fGachaFocusCoolTime	= 0.3f;
	m_fGachaSignMaskTime	= 0.f;
	m_fGachaResultTime		= 0;

	m_bGachaSignMask = FALSE;

	m_pGachaResult_StartDirectParent->Set_Active(FALSE);
	m_pGachaResult_SignParent->Set_Active(FALSE);
	m_pGachaResult_TenParent->Set_Active(FALSE);
	m_pGachaResult_OneResult->Set_Active(FALSE);
	m_pGachaResult_FinalResult->Set_Active(FALSE);

	switch (m_eResultType)
	{
	case E_GachaResultType::Start_Direct:
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"BGM_UI_Gacha_���ۺκи���.wav", 1.f);
		m_pGachaResult_StartDirectParent->Set_Active(TRUE);
		m_pGachaResult_StartDirect_Circle->Set_Active(FALSE);
		m_pGachaResult_StartDirect_SRank->Set_Active(FALSE);
		m_pGachaResult_StartDirect_Fade->Set_Active(FALSE);
		m_pGachaResult_StartDirect_Fade->Update_Object(0.f);
		_vec2 vUiPos = _vec2(WINCX * 0.5f, WINCY * 0.5f);
		CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Gacha_Start.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.05f);
		break;
	}
	case E_GachaResultType::Sign:
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_LevelUp.wav", 1.f);
		m_pGachaResult_SignParent->Set_Active(TRUE);
		_vec2 vUiPos = _vec2(WINCX * 0.5f, WINCY * 0.28f);

		CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Gacha_Inside.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.048f);

		m_pUiSkillObject_GachaInside = pUiSkillObject;
		break;
	}
	case E_GachaResultType::TenGacha:
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Result_SSR.wav", 1.f);
		m_pGachaResult_TenParent->Set_Active(TRUE);
		break;
	case E_GachaResultType::Result:
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Normal_05.wav", 1.f);
		for (_int i = 0; i < (_int)m_pGachaResult_One_StudentParent->Get_ChildList()->size(); ++i)
		{
			m_pGachaResult_One_StudentParent->Get_ChildByIndex(i)->Set_Active(FALSE);
		}
		m_pGachaResult_OneResult->Set_Active(TRUE);
		Update_GachaAnimation();

		break;
	case E_GachaResultType::FinalResult:
		m_pGachaResult_FinalResult->Set_Active(TRUE);
		for (_int i = 0; i < 10; i++)
		{
			m_pGachaResult_Prefab[i]->Set_Active(TRUE);


			// �� ĳ���͸� Newǥ�� �ѱ�
			_bool bAlreadyHave = CDataManager::GetInstance()->Get_UserInfo().bCharacter[m_iArrGachaResultUnit[i]];

			m_pGachaResult_Prefab[i]->Get_ChildByIndex(3)->Set_Active(FALSE == bAlreadyHave);

			if (bAlreadyHave)
			{
				CGameManager::GetInstance()->Add_ItemCount(m_iArrGachaResultUnit[i], 10);
			}
			else
			{
				// ĳ���� �����ϰ� ����
				CDataManager::GetInstance()->Set_HaveChar(m_iArrGachaResultUnit[i], TRUE);
			}

			wstring wstrTexName = L"../../Reference/Resource/Texture/UI/Character_Collection/Character/Skill_Portrait_" +
				CCharacterMgr::GetInstance()->Translation_EnumToString((_uint)m_iArrGachaResultUnit[i], E_CHAR_ENUMTYPE::CNUM_NAME) +
				L".png";

			m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Get_ChildByIndex(1)->Set_Tex1_Tag(wstrTexName);


			// �� ���� ����
			_int iStar = (_int)m_iArrGachaResultTier[i];
			m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Active(FALSE);
			m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Active(FALSE);
			m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Active(FALSE);

			if (iStar == 1)
			{
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(0)->Set_Ui_SizeY(100);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Set_ImageColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f));
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_PosX(0);
			}
			else if (iStar == 2)
			{
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(0)->Set_Ui_SizeY(250);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Set_ImageColor(D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_PosX(-15);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_PosX(15);
			}
			else if (iStar == 3)
			{
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(0)->Set_Ui_SizeY(300);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(1)->Set_ImageColor(D3DXCOLOR(1.f, 0.f, 1.f, 1.f));
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Active(TRUE);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(0)->Set_Ui_PosX(0);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_PosX(-25);
				m_pGachaResult_Prefab[i]->Get_ChildByIndex(2)->Get_ChildByIndex(2)->Set_Ui_PosX(25);
			}
		}
		break;
	case E_GachaResultType::Gacha_End:
		break;
	}

}

void CUI_Manager::Update_GachaAnimation()
{
	_int iCharEnum = m_iArrGachaResultUnit[m_iGachaOneAnim];
	if (m_fGachaResultTime <= 0.02f)
	{
		CUtility::Print_Wstring_DMode(L"UnitEnum : " + to_wstring(m_iArrGachaResultUnit[m_iGachaOneAnim]) + L", Tier : " + to_wstring(m_iArrGachaResultTier[m_iGachaOneAnim]));

		m_pGachaResult_One_RareCard->Set_Active(TRUE);
		m_pGachaResult_One_RareCard->Set_Ui_SizeX(192);
		m_pGachaResult_One_RareCard->Set_Ui_SizeY(192);

		m_pGachaResult_One_Name_Star1->Set_Active(FALSE);
		m_pGachaResult_One_Name_Star2->Set_Active(FALSE);
		m_pGachaResult_One_Name_Star3->Set_Active(FALSE);
		m_pGachaResult_One_Description->Set_Active(FALSE);
		m_pGachaResult_One_Name_Parent->Set_Active(FALSE);
		m_pGachaResult_One_CharInfoParent->Set_Active(FALSE);

		for (_int i = 0; i < (_int)m_pGachaResult_One_StudentParent->Get_ChildList()->size(); ++i)
		{
			m_pGachaResult_One_StudentParent->Get_ChildByIndex(i)->Set_Active(FALSE);
		}
		if (m_iArrGachaResultTier[m_iGachaOneAnim] == 1)
		{
			m_pGachaResult_OneResult->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/Gacha_BG_R.png");
			m_pGachaResult_One_RareCard->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_RCard_1.png");
			m_pGachaResult_One_Name_Star1->Set_Active(TRUE);
		}
		if (m_iArrGachaResultTier[m_iGachaOneAnim] == 2)
		{
			m_pGachaResult_OneResult->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/Gacha_BG_SR.png");
			m_pGachaResult_One_RareCard->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_SRCard.png");
			m_pGachaResult_One_Name_Star1->Set_Active(TRUE);
			m_pGachaResult_One_Name_Star2->Set_Active(TRUE);
		}
		if (m_iArrGachaResultTier[m_iGachaOneAnim] == 3)
		{
			m_pGachaResult_OneResult->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/Gacha_BG_SSR.png");
			m_pGachaResult_One_RareCard->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Gacha/FX_TEX_GT_SSRCard.png");
			m_pGachaResult_One_Name_Star1->Set_Active(TRUE);
			m_pGachaResult_One_Name_Star2->Set_Active(TRUE);
			m_pGachaResult_One_Name_Star3->Set_Active(TRUE);
		}

		// �̸�
		wstring wstrKorName = CXL_ParsingMgr::GetInstance()->Translation_EnumToString(iCharEnum, E_CHAR_ENUMTYPE::CNUM_KORNAME);
		m_pGachaResult_One_Name_Parent->Set_Ui_Text(wstrKorName);
		// �ڱ�Ұ�
		wstring wstrHobby = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).wstrHobby;
		m_pGachaResult_One_Description->Get_ChildByIndex(0)->Set_Ui_Text(L"��� : " + wstrHobby);



		// �б� ��ũ, �ѱ��̸�, �����̸�
		E_CHAR_SCHOOL eSchool = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).eSchoolType;
		switch (eSchool)
		{
		case Engine::E_CHAR_SCHOOL::SCHOOL_ABYDOS:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/ABYDOS.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Abydos HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_TRINITY:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/TRINITY.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Trinity HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_GEHENNA:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/GEHENNA.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Gehenna HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/MILLENNIUM.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Millenium HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/HYAKKIYAKO.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Hyakkiyako HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/SHANHAIJING.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Shanhaijing HighSchool");
			break;
		case Engine::E_CHAR_SCHOOL::SCHOOL_REDWINTER:
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/School/REDWINTER.png");
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(2)->Set_Ui_Text(L"Redwinter HighSchool");
			break;
		default:
			break;
		}
		m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(1)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eSchool, E_CHAR_ENUMTYPE::CNUM_SCHOOL));

		// ���Ƹ�
		wstring wstrCircle = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).wstrCircle;
		m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(4)->Set_Ui_Text(wstrCircle);
		
		// ����
		E_CHAR_COMBATTYPE eCombatType = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).eCombatType;
		if (eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
		{
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(6)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Font/ImgFont_Main.png");
		}
		else if(eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL)
		{
			m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(6)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Font/ImgFont_Support.png");
		}

		// ����
		E_CHAR_WEAPONTYPE eWeapon = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).eWeaponType;
		m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(8)->Set_Ui_Text(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)eWeapon, E_CHAR_ENUMTYPE::CNUM_WEAPONTYPE));

		// ����
		wstring birthday = CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)iCharEnum).wstrBirthDay;
		m_pGachaResult_One_CharInfoParent->Get_ChildByIndex(10)->Set_Ui_Text(birthday);
	}
	else if (m_fGachaResultTime <= 1.f)
	{
		_float fRatio = CUtility::Clamp_float(m_fGachaResultTime, 0.f, 1.f);
		m_pGachaResult_One_RareCard->Set_Active(TRUE);
		m_pGachaResult_One_RareCard->Set_Ui_SizeX((_int)(192 + fRatio * 192.f));
		m_pGachaResult_One_RareCard->Set_Ui_SizeY((_int)(192 + CUtility::Clamp_float(m_fGachaResultTime, 0.f, 1.f) * 192.f));
	}
	else if (m_fGachaResultTime <= 2.f)
	{
		_float fRatio = CUtility::Clamp_float(m_fGachaResultTime - 1.f, 0.f, 1.f);
		m_pGachaResult_BlackMask->Set_Active(TRUE);
		m_pGachaResult_BlackMask->Set_ImageAlpha(fRatio);
	}
	else if (m_fGachaResultTime <= 3.f)
	{
		_float fRatio = CUtility::Clamp_float(m_fGachaResultTime - 2.f, 0.f, 1.f);
		m_pGachaResult_One_RareCard->Set_Active(FALSE);
		m_pGachaResult_One_StudentParent->Set_Active(TRUE);
		m_pGachaResult_One_Description->Set_Active(TRUE);
		m_pGachaResult_One_Name_Parent->Set_Active(TRUE);
		m_pGachaResult_One_CharInfoParent->Set_Active(TRUE);
		m_pGachaResult_One_StudentParent->Get_ChildByIndex(iCharEnum)->Set_Active(TRUE);
		m_pGachaResult_One_StudentParent->Set_Ui_PosX((_int)(CUtility::Lerp_float(450, 0, fRatio)));
		m_pGachaResult_One_StudentParent->Set_Ui_PosY((_int)(CUtility::Lerp_float(-76, 50, fRatio)));

		if (m_fGachaResultTime >= 2.07f)
		{
			m_pGachaResult_BlackMask->Set_Active(FALSE);
		}

		// 0, 40
	}
	else
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON))
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"UI_Gacha_Normal_05.wav", 1.f);
			m_fGachaResultTime = 0.f;
			m_iGachaOneAnim++;
			m_pGachaResult_One_RareCard->Set_Active(FALSE);
			m_pGachaResult_BlackMask->Set_Active(FALSE);
			m_pGachaResult_One_StudentParent->Set_Active(FALSE);
			m_pGachaResult_One_Description->Set_Active(FALSE);
			m_pGachaResult_One_Name_Parent->Set_Active(FALSE);
			m_pGachaResult_One_CharInfoParent->Set_Active(FALSE);
		}
	}
}

void CUI_Manager::Gacha_Particle(_vec2 vPos, _vec3 vDir, _float fMinDist, _float fMaxDist, D3DXCOLOR color)
{
	_vec2 vUiPos = vPos;
	CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Gacha_Particle_Sky.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.05f);

	_float fRandDir = CWall512::GetInstance()->GetFloatValue(fMinDist, fMaxDist);
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_Start = (_vec4)color;
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vColor_End	= _vec4(0.f, 0.f, 0.f, 0.f);
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vPos_Start	= VEC_ZERO;
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vPos_End		= vDir * fRandDir;

	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vScale_End	= VEC_ZERO;

	_float fZRand = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_Start.z = fZRand;
	static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_End.z	= fZRand;
}


#pragma endregion 