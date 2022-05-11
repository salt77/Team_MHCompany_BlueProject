#pragma once
#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "UI_Object.h"
#include "Character.h"
#include "Obstacle_Cafe.h"
#include "StageMap.h"
#include "Engine_Define.h"
#include "Base.h"
#include "Engine_Enum.h"

#include "BackBoard.h"
class ENGINE_DLL CUI_Manager final: public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager();

public:
	virtual void	Free() override;

	void			Initialize(HWND* pHwnd);
	void			Scene_Change_Start();
	void			Scene_Change_End();
	void			Update_UIManager(const _float& fTimeDelta);

public:
	CUI_Object*		Load_UI_Prefab(wstring wstrPath);
	CUI_Object*		Load_UI_Layer(wstring wstrPath);

public:
	_vec3			Click_Map();

public:
	void			Set_LastPickUi(CUI_Object* pObject)							{ m_pLastClick_Ui		= pObject;			}
	void			Set_Func_LobbyScene(std::function<void(E_SCENETAG)> fp)		{ m_pFunc_LobbyScene	= fp;				}
	void			Set_Func_CombatScene(std::function<void(E_SCENETAG)> fp)	{ m_pFunc_CombatScene	= fp;				}
	void			Set_Func_RunGaneScene(std::function<void(E_SCENETAG)> fp)	{ m_pFunc_RunGameScene	= fp;				}
	void			Set_Func_TileScene(std::function<void(E_SCENETAG)> fp)		{ m_pFunc_TileScene		= fp;				}
	void			Set_LoadingGage(int* pLoadingGage)							{ m_pLoadingGage		= pLoadingGage;		}

public:
	HWND*			Get_Hwnd() { return m_pHwnd; }

private:
	int*			m_pLoadingGage = nullptr;
	HWND*			m_pHwnd = nullptr;
	CUI_Object*		m_pLastClick_Ui = nullptr;				// 마지막으로 클릭된 오브젝트 (비워주지않는다.) 


public:
	_vec3			m_vPickingPos = VEC_ZERO;



#pragma region 로비 및 최상위 UI
public:
	void			Create_Ui_HighInfoPanel();
	void			Create_Ui_Lobby();
	void			Goto_Lobby();
	void			Set_LobbyInfoUI();
	void			SceneChange_Goto_Lobby();
	void			Set_FriendList();
	void			Update_LobbyUI(const _float& fTimeDelta);
	void			OnClick_FriendList();
	void			Set_LobbyHighPanelActive()			 { m_pHighInfoPanel->Set_Active(TRUE); }

private:
	void			OnOff_Option(_int iState);
	void			Goto_BeforePanel();

public:
	CUI_Object*				m_pLobbyPanel = nullptr;

private:
	CUI_Object*				m_pHighInfoPanel = nullptr;
	CUI_Object*				m_pOptionPanel = nullptr;

	// Lobby 변수  유저 정보
	CUI_Object*				m_pLobby_APNumber_Left			= nullptr;
	CUI_Object*				m_pLobby_CreditNumber			= nullptr;
	CUI_Object*				m_pLobby_BluePyroxeneNumber		= nullptr;
	CUI_Object*				m_pLobby_ID						= nullptr;
	CUI_Object*				m_pLobby_Level					= nullptr;
	CUI_Object*				m_pLobby_Charillust				= nullptr;
	CUI_Object*				m_pLobby_FriendList				= nullptr;
	CUI_Object*				m_pLobby_Moveillust				= nullptr;

	// 상단 바 유저 정보
	CUI_Object*				m_pHighPanel_PanelName			= nullptr;
	CUI_Object*				m_pHighPanel_APNumber_Left		= nullptr;
	CUI_Object*				m_pHighPanel_CreditNumber		= nullptr;
	CUI_Object*				m_pHighPanel_BluePyroxeneNumber = nullptr;

	_float					m_fMoveillustCoolTime = 0.f;
	_float					m_fMoveillust_Start_X[3];
	_bool					m_bMoveillust = FALSE;
#pragma endregion

#pragma region 인벤토리
public:
	void			Create_Ui_Inventory(CUI_Object* pParent);

private:
	void			InventoryPanel_OnOff(_int iState);
	void			Inven_Click_Filter();
	void			Inven_Click_Sort();
	void			Inven_Click_Ascending();
	void			Inven_Create_Scroll();
	void			Inven_Change_SelectItem(_int iItemTag);
	void			Inven_FilterState(_int iFilter);
	void			Inven_Filter_Ok(_int iBool);
	void			Inven_SortState(_int iSort);
	void			Inven_Sort_Ok(_int iBool);

private:
	CUI_Object*		m_pInventoryPanel = nullptr;

	// Inventory 변수
	CUI_Object*				m_pInventory_Scroll = nullptr;
	CUI_Object*				m_pInventory_FilterPanel = nullptr;
	CUI_Object*				m_pInventory_SortPanel = nullptr;
	CUI_Object*				m_pInven_Filter[(_int)E_ItemFilter::강화석];
	CUI_Object*				m_pInven_SortButton[(_int)E_ItemSortType::E_End];

	vector<T_ItemInfo*>		m_vecSortItem;
	E_AscendingType			m_eInven_AscendingType = E_AscendingType::내림차순;
	_bool					m_bInven_SortState[(_int)E_ItemFilter::강화석] = { 0, 0, 0, 0, 0, 0 };
	_bool					m_bInven_SortTemp[(_int)E_ItemFilter::강화석];
	E_ItemSortType			m_eInven_ItemSort_Type = E_ItemSortType::Basic;

#pragma endregion




#pragma region 학생부
public:
	void			Create_Ui_CharCollection();

private:			// 학생부 관련 함수 
	void			StudentBook_AddFunction();
	void			StudentBook_RebindCharacterInfo();
	void			StudentBook_Sort(E_CHAR_SORT eSortType, _bool bIsReverse = FALSE);
	void			StudentBook_ClickFilter_All();
	void			StudentBook_ClickFilter_Striker();
	void			StudentBook_ClickFilter_Special();
	void			StudentBook_SortBtnClick();
	void			StudentBook_OffSortScreen();
	void			StudentBook_SelChangeSortType(_int iSortType);
	void			StudentBook_SortScreen_Ok();
	void			StudentBook_ClickSort_Reverse();

private:
	_bool					m_bIsReverseSort = FALSE; 

	CUI_Object*				m_pCollectionPanel = nullptr;
	CUI_Object*				m_pCollectionScroll = nullptr;
	CUI_Object*				m_pCollectionSortPanel = nullptr;
	CUI_Object*				m_pCharDetail = nullptr;

#pragma endregion




#pragma region 학생 디테일
public:
	void			Create_Ui_CharDetailInfo(_int iNameEnum);

private:			// 학생 상세정보 관련 함수 
	void			StudentDetail_AddFunction();
	void			StudentDetail_Update_CharDetailInfo(E_CHAR_NAME eName = E_CHAR_NAME::Name_End);
	void			StudentDetail_Update_Variable();
	void			StudentDetail_Update_SimpleSkillInfo();
	void			StudentDetail_OnOffDetailStat();
	void			StudentDetail_OnOffDetailSkill();
	void			StudentDetail_OnOffEquipLevelUp();
	void			StudentDetail_ClickGeneralStat();
	void			StudentDetail_ClickLevelUpStat();
	void			StudentDetail_ClickLiberationStat();
	void			StudentDetail_ClickCharLevelUpItem(_uint iIndex);
	void			StudentDetail_ClickCharLevelUpAutoSel();
	void			StudentDetail_ClickCharLevelUpItemBack(_uint iIndex);
	void			StudentDetail_ClickLevelUp_Char();
	void			StudentDetail_SelChangeSkill(_uint iSkillTypeIndex);
	void			StudentDetail_ClickNextCharInfo();
	void			StudentDetail_ClickPreCharInfo();

	void			Check_LevelUp(_uint* iExperience);
	void			Check_LevelDown(const _uint& iIndex, _int iLeftExp = 0);
	void			Check_StatAfterLvUp();
	void			Initialize_StudentDetail();
	void			Update_StudentDetail();

private:
	CUI_Object*				m_pCharDetail_LeftLow = nullptr;				// 상세정보 좌하단 정보 창
	CUI_Object*				m_pCharDetail_RightMid = nullptr;				// 상세정보 우중단 정보 창
	CUI_Object*				m_pCharDetail_DetailStat_Highbar = nullptr;		// 상세정보 캐릭터 상세 스텟 창 상단바
	CUI_Object*				m_pCharDetail_DetailStat = nullptr;				// 상세정보 캐릭터 상세 스텟 창 
	CUI_Object*				m_pCharDetail_DetailSkill = nullptr;			// 상세정보 캐릭터 상세 스킬 창 
	CUI_Object*				m_pCharDetail_DetailEquip = nullptr;			// 상세정보 캐릭터 장비 레벨업 창
	CUI_Object*				m_pCharDetail_LevelUp = nullptr;				// 상세정보 레벨업 탭

	E_CHAR_SORT				m_eCharSortType = E_CHAR_SORT::SORT_NAME;
	E_CHAR_NAME				m_eCurrentCharDetail = E_CHAR_NAME::Name_End;
	E_CHAR_COMBATTYPE		m_eCurrentFilterType = E_CHAR_COMBATTYPE::COMBATTYPE_END;
	E_SkillType				m_eSelSkillUpgrade = E_SkillType::Skill_Ex;

	_uint					m_iCurrentCredit = 0;
	_uint					m_iLvUpItemInsertCount[4] = { 0, 0, 0, 0 };
	_uint					m_iCurrentReinforceItem[4] = { 0, 0, 0, 0 };
	_uint					m_iCharExperienceAfterLvUp = 0;
	_uint					m_iCharLevelAfterLvUp = 1;

#pragma endregion




#pragma region 편성창 
public:
	void			Open_Ui_BattleFormation(_int eWorldMod);
	void			Create_Ui_BattleFormation();
public:
	void			BattleFormation_SetWindow(CUI_Object* pUIObj, const T_CharInfo& tCharInfo);
	void			BattleFormation_ParticpateReset();
private:			//편성창 관련 함수
	void			BattleFormation_AddFunction();
	void			BattleFormation_OpenFormation();
	void			BattleFormation_CloseFormation();
	void			BattleFormation_NameCopy();
	void			BattleFormation_SetList();
	void			BattleFormation_SetSquadPosition();
	void			BattleFormation_AddSquad(_int iCharName);
	void			BattleFormation_AcceptSquad();
	void			BattleFormation_UpdateSquad();
	void			BattleFormation_ChangeSquad();
	void			BattleFormation_ChangeSquadInt(_int eSquad);
	void			BattleFormation_ActiveSquad(_bool bActive);
	void			BattleFormation_DeleteSlot(_int iSlot);
	void			BattleFormation_UpdateCard();
	void			BattleFormation_SetCard(CUI_Object* pCard, const T_CharInfo& tCharInfo, _int iIndex);
	void			BattleFormation_SquadSlotClick(_int iSlotNum);

	void			BattleFormation_MouseControl();

	void			BattleFormation_Particpate();
	void			BattleFormation_SquadRelease();
	void			BattleFormation_MissionInit();
	void			BattleFormation_SetCharInfo(E_CHAR_NAME eCharName);

private:			//편성창 변수
	_bool					m_bInputMouse = TRUE;

	E_WORLD_MOD				m_eWorldMod = E_WORLD_MOD::MOD_END;
	CUI_Object*				m_pBattleFormationPanel = nullptr;  // 편성창 최상위 부모 UI
	E_SQUAD					m_ePrevSquad = E_SQUAD::SQUAD_END;
	E_SQUAD					m_eCurrentSquad = E_SQUAD::SQUAD_END;
	map<E_CHAR_NAME, CUI_Object*> m_mapCharBtn;
	vector<E_CHAR_NAME>     m_vecSquadStudentName;
	_float					m_fPickTime;

	RECT					m_tCharacterRect[4];
	POINT					m_tCharacterSlotPoint[4];
	POINT					m_tPickPoint;
	_int					m_iPickIndex;
	_bool					m_bMove = FALSE;
	CCharacter*				m_pPickCharacter = nullptr;
	_bool					m_bKeyDown = FALSE;

#pragma endregion

#pragma region 임무 & 업무
public:
	void			Create_Ui_WorkPanel();
	void			Create_Ui_MissionPanel();
	void			Create_Ui_EnemyInfoPanel();

private:			//업무 & 임무 관련 함수
	void			Work_OnEnable();
	void			Mission_OnEnable();
	void			MissionWanted_OnEnable();
	void			MissionSpecial_OnEnable();
	void			MissionTotal_OnEnable();
	void			Load_Mission(E_WORLD_MOD eMod);
	void			Mission_GoInside(void* pArg);
	void			Mission_Start();
	void			Mission_ClickEnemyInfo();
	void			TileMission_GoInside(wstring wstrArg);
	void			TileMission_Start();
	void			TileMission_ClickEnemyInfo();

private:
	// 업무창 변수
	CUI_Object*				m_pWorkPanel						= nullptr;			// 업무창 부모

																					// 임무창 변수
	CUI_Object*				m_pMissionPanel						= nullptr;			// 임무창 부모
	CUI_Object*				m_pMissionInfoBackground			= nullptr;			// 임무 정보창 Background
	CUI_Object*				m_pMissionInfoPanel					= nullptr;			// 임무 정보창 부모
	CUI_Object*				m_pMission_Thumnail					= nullptr;			// 임무 테마 썸네일
	CUI_Object*				m_pMission_Normal_Scroll			= nullptr;			// 노말 난이도 스크롤 바
	CUI_Object*				m_pMission_Hard_Scroll				= nullptr;			// 하드 난이도 스크롤 바



	CUI_Object*				m_pTileMissionInfoPanel				= nullptr;			// 타일임무 정보창 부모
	CUI_Object*				m_pTileMissionInfo_StageNumber		= nullptr;
	CUI_Object*				m_pTileMissionInfo_StageName		= nullptr;
	CUI_Object*				m_pTileMissionInfo_RecommentLevel	= nullptr;
	CUI_Object*				m_pTileMissionInfo_MapType			= nullptr;
	CUI_Object*				m_pTileMissionInfo_EnemyInfoButton	= nullptr;
	CUI_Object*				m_pTileMissionInfo_Item1			= nullptr;
	CUI_Object*				m_pTileMissionInfo_Item2			= nullptr;
	CUI_Object*				m_pTileMissionInfo_Item3			= nullptr;
	CUI_Object*				m_pTileMissionInfo_Item4			= nullptr;
	CUI_Object*				m_pTileMissionInfo_ClearPanel		= nullptr;
	CUI_Object*				m_pTileMissionInfo_Start			= nullptr;



	std::function<void(E_SCENETAG)>		m_pFunc_LobbyScene;
	std::function<void(E_SCENETAG)>		m_pFunc_RunGameScene;
	std::function<void(E_SCENETAG)>		m_pFunc_CombatScene;
	std::function<void(E_SCENETAG)>		m_pFunc_TileScene;
	vector<pair<E_WORLD_MOD, CUI_Object*>>	m_vecMissionTypes[2];

private:
	// 적 정보창 
	CUI_Object*				m_pEnemyInfoPanel				= nullptr;			// 적 정보 창 부모
	CUI_Object*				m_pEnenyInfo_Scroll				= nullptr;			// 적 정보 스크롤 뷰
#pragma endregion

#pragma region 총력전 창

public:
	void					Create_Ui_TotalPanel();
	void					OnClick_TotalPanel();
	void					OnClick_TotalInfoPanel(_int iBossType);
	void					OnClick_TotalMission_Start();

private:
	CUI_Object*				m_pTotal_Panel					= nullptr;
	CUI_Object*				m_pTotal_Frame_Binah			= nullptr;
	CUI_Object*				m_pTotal_Frame_Hieronymus		= nullptr;
	CUI_Object*				m_pTotal_Frame_Chesed			= nullptr;
	CUI_Object*				m_pTotal_Frame_Kuro				= nullptr;
	CUI_Object*				m_pTotal_Bottom_Ranking			= nullptr;

	CUI_Object*				m_pTotalInfo_Panel				= nullptr;
	CUI_Object*				m_pTotalInfo_Background			= nullptr;
	CUI_Object*				m_pTotalInfo_Bossillust			= nullptr;
	CUI_Object*				m_pTotalInfo_BossName			= nullptr;
	CUI_Object*				m_pTotalInfo_HealthText_OutLine = nullptr;
	CUI_Object*				m_pTotalInfo_MapType			= nullptr;
	CUI_Object*				m_pTotalInfo_Recommand_Level_OutLine = nullptr;
	CUI_Object*				m_pTotalInfo_MissionStartButton = nullptr;
	CUI_Object*				m_pTotalInfo_Frame_BossSkill_1	= nullptr;
	CUI_Object*				m_pTotalInfo_Frame_BossSkill_2	= nullptr;
	CUI_Object*				m_pTotalInfo_Frame_BossSkill_3	= nullptr;
	CUI_Object*				m_pTotalInfo_Frame_BossSkill_4	= nullptr;
	CUI_Object*				m_pTotalInfo_Frame_BossSkill_5	= nullptr;

	enum		ETotalPanel_BossType { E_TotalPanel_Binah, E_TotalPanel_Hieronymus, E_TotalPanel_Chesed, E_TotalPanel_Kuro };
	ETotalPanel_BossType	m_eLastBossType = ETotalPanel_BossType::E_TotalPanel_Binah;
#pragma endregion


#pragma region 인테리어(카페)

public:
	void								Scene_Change_Cafe();
	void								Create_Ui_Cafe();

public:
	CObstacle_Cafe*						Get_Obstacle() { return m_pObstacle_Cafe; }
	CUI_Object*							Get_Panel_ParentLeftPanel() { return m_pParentLeftPanel; }
	CUI_Object*							Get_Panel_ParentRightPanel() { return m_pParentRightPanel; }
	CUI_Object*							Get_Panel_ParentTopPanel() { return m_pParentTopPanel; }
	CUI_Object*							Get_Panel_FixMode1Panel() { return m_pFixMode1Panel; }
	CUI_Object*							Get_Panel_FixMode2Panel() { return m_pFixMode2Panel; }
	CUI_Object*							Get_Panel_FurInfoPanel() { return m_pFurInfoPanel; }
	CUI_Object*							Get_Panel_CafeInfoPanel() { return m_pCafeInfoPanel; }
	CUI_Object*							Get_Panel_CafeProfitPanel() { return m_pCafeProfitPanel; }
	CUI_Object*							Get_Panel_PresetPanel() { return m_pPresetPanel; }

public:
	void								Set_Func_CafeScene(std::function<void(E_SCENETAG)> CafeScene) { m_pFunc_CafeScene = CafeScene; }
	void								Set_Func_PickFurniture(std::function<void(_bool)> Furniture) { m_pFunc_PickFurniture = Furniture; }
	void								Set_Func_RotFurniture(std::function<void(_bool)> Furniture) { m_pFunc_RotFurniture = Furniture; }
	void								Set_Func_StoFurniture(std::function<void(_bool)> Furniture) { m_pFunc_StoFurniture = Furniture; }
	void								Set_Func_SelFurnitureInfo(std::function<void(_bool)> Furniture) { m_pFunc_SelFurnitureInfo = Furniture; }
	void								Set_Func_AllStoFurniture(std::function<void(_bool)> Furniture) { m_pFunc_AllStoFurniture = Furniture; }
	void								Set_Func_CreateFurniturePick(std::function<void(_bool)> Furniture) { m_pFunc_CreateFurniturePick = Furniture; }
	void								Set_Func_CafeInfoAmenity(std::function<void(_bool)> Amenity) { m_pFunc_CafeInfoAmenity = Amenity; }
	void								Set_Func_OpenFurInfo(std::function<void(_bool)> Amenity) { m_pFunc_OpenFurInfo = Amenity; }
	void								Set_Func_ExitFurInfo(std::function<void(_bool)> Amenity) { m_pFunc_ExitFurInfo = Amenity; }
	void								Set_Func_UpdateAmenity(std::function<void(_bool)> Amenity) { m_pFunc_UpdateAmenity = Amenity; }
	void								Set_Func_ProfitRecieve(std::function<void(_bool)> Profit) { m_pFunc_ProfitRecieve = Profit; }
	void								Set_Func_PresetName(std::function<void(_bool)> Preset) { m_pFunc_PresetName = Preset; }
	void								Set_Func_PresetNameEnd(std::function<void(_bool)> Preset) { m_pFunc_PresetNameEnd = Preset; }
	void								Set_Func_PresetName1(std::function<void(_bool)> Preset) { m_pFunc_PresetName1 = Preset; }
	void								Set_Func_PresetName2(std::function<void(_bool)> Preset) { m_pFunc_PresetName2 = Preset; }
	void								Set_Func_PresetName3(std::function<void(_bool)> Preset) { m_pFunc_PresetName3 = Preset; }
	void								Set_Func_PresetName4(std::function<void(_bool)> Preset) { m_pFunc_PresetName4 = Preset; }
	void								Set_Func_PresetName5(std::function<void(_bool)> Preset) { m_pFunc_PresetName5 = Preset; }

public:
	void								Set_Func_PresetInit1(std::function<void(_bool)> Preset1) { m_pFunc_PresetInit1 = Preset1; }
	void								Set_Func_PresetSave1(std::function<void(_bool)> Preset1) { m_pFunc_PresetSave1 = Preset1; }
	void								Set_Func_PresetApply1(std::function<void(_bool)> Preset1) { m_pFunc_PresetApply1 = Preset1; }
	void								Set_Func_PresetInit2(std::function<void(_bool)> Preset2) { m_pFunc_PresetInit2 = Preset2; }
	void								Set_Func_PresetSave2(std::function<void(_bool)> Preset2) { m_pFunc_PresetSave2 = Preset2; }
	void								Set_Func_PresetApply2(std::function<void(_bool)> Preset2) { m_pFunc_PresetApply2 = Preset2; }
	void								Set_Func_PresetInit3(std::function<void(_bool)> Preset3) { m_pFunc_PresetInit3 = Preset3; }
	void								Set_Func_PresetSave3(std::function<void(_bool)> Preset3) { m_pFunc_PresetSave3 = Preset3; }
	void								Set_Func_PresetApply3(std::function<void(_bool)> Preset3) { m_pFunc_PresetApply3 = Preset3; }
	void								Set_Func_PresetInit4(std::function<void(_bool)> Preset4) { m_pFunc_PresetInit4 = Preset4; }
	void								Set_Func_PresetSave4(std::function<void(_bool)> Preset4) { m_pFunc_PresetSave4 = Preset4; }
	void								Set_Func_PresetApply4(std::function<void(_bool)> Preset4) { m_pFunc_PresetApply4 = Preset4; }
	void								Set_Func_PresetInit5(std::function<void(_bool)> Preset5) { m_pFunc_PresetInit5 = Preset5; }
	void								Set_Func_PresetSave5(std::function<void(_bool)> Preset5) { m_pFunc_PresetSave5 = Preset5; }
	void								Set_Func_PresetApply5(std::function<void(_bool)> Preset5) { m_pFunc_PresetApply5 = Preset5; }

public:
	void								Set_Func_Gift1(std::function<void(_bool)> Gift1) { m_pFunc_Gift1 = Gift1; }
	void								Set_Func_Gift2(std::function<void(_bool)> Gift2) { m_pFunc_Gift2 = Gift2; }
	void								Set_Func_Gift3(std::function<void(_bool)> Gift3) { m_pFunc_Gift3 = Gift3; }
	void								Set_Func_Gift4(std::function<void(_bool)> Gift4) { m_pFunc_Gift4 = Gift4; }
	void								Set_Func_Gift5(std::function<void(_bool)> Gift5) { m_pFunc_Gift5 = Gift5; }

public:
	void								Cafe_Ui_Event();
	void								Cafe_Ui_Exception();

public:
	void								Furniture_FixMode();
	void								Furniture_Rotation();
	void								Furniture_Storage();
	void								Furniture_AllStorage();
	void								Furniture_Info(_int iFur);
	void								Furniture_Create(_int iFur);
	void								Furniture_CreateTile();
	void								Furniture_CreateWall();
	void								Furniture_SelInfo(_int iFur);
	void								Furniture_LineUp();
	void								Furniture_InfoOpen();
	void								Furniture_InfoExit();
	void								Furniture_Profit();
	void								Furniture_PresetName();
	void								Furniture_PresetNameEnd();
	void								Furniture_PresetName1();
	void								Furniture_PresetName2();
	void								Furniture_PresetName3();
	void								Furniture_PresetName4();
	void								Furniture_PresetName5();

public:
	void								Furniture_PresetInit1();
	void								Furniture_PresetSave1();
	void								Furniture_PresetApply1();
	void								Furniture_PresetInit2();
	void								Furniture_PresetSave2();
	void								Furniture_PresetApply2();
	void								Furniture_PresetInit3();
	void								Furniture_PresetSave3();
	void								Furniture_PresetApply3();
	void								Furniture_PresetInit4();
	void								Furniture_PresetSave4();
	void								Furniture_PresetApply4();
	void								Furniture_PresetInit5();
	void								Furniture_PresetSave5();
	void								Furniture_PresetApply5();

public:
	void								Gift_1();
	void								Gift_2();
	void								Gift_3();
	void								Gift_4();
	void								Gift_5();

public:
	void								AllStorage();

public:
	void								FurnitureSort_AutoSetting();
	void								FurnitureSort_Ascending();
	void								FurnitureSort_Descending();
	static _bool						Furniture_Compare_Name(const T_FurnitureInfo& tInfoBegin, const T_FurnitureInfo& tInfoEnd);
	static _bool						Furniture_Compare_Amenity(const T_FurnitureInfo& tInfoBegin, const T_FurnitureInfo& tInfoEnd);
	void								Furniture_Sort_AmenityInfo(_int iFur);
	void								Furniture_Sort_AmenityCreate(_int iFur);
	void								Furniture_Sort_NameInfo(_int iFur);
	void								Furniture_Sort_NameCreate(_int iFur);
	void								Furniture_Filter_FurAmenityInfo(_int iFur);
	void								Furniture_Filter_FurAmenityCreate(_int iFur);
	void								Furniture_Filter_FurNameInfo(_int iFur);
	void								Furniture_Filter_FurNameCreate(_int iFur);

public:
	void								OpenInven_AutoSetting();
	
	void								Furniture_Filter_All();
	
	void								Furniture_Filter_Furniture();
	void								Furniture_FilterTag_Table();
	void								Furniture_FilterTag_Closet();
	void								Furniture_FilterTag_Chair();
	void								Furniture_FilterTag_Bed();
	void								Furniture_FilterTag_EtcFur();

	void								Furniture_Filter_Deco();
	void								Furniture_FilterTag_Props();
	void								Furniture_FilterTag_WallDeco();
	void								Furniture_FilterTag_FloorDeco();
	void								Furniture_FilterTag_EtcDeco();

	void								Furniture_Filter_Indoor();
	void								Furniture_Filter_FloorIndoor();
	void								Furniture_Filter_WallIndoor();

public:
	void								Sound_Click();
	void								Sound_Back();
	void								Sound_Create();
	void								Sound_Delete();
	
private:
	//카페_최상위 부모
	CUI_Object*							m_pBestCafePanel			= nullptr;

	//최상위 부모 자식
	CUI_Object*							m_pParentLeftPanel			= nullptr;		//부모_왼쪽 레이어
	CUI_Object*							m_pParentRightPanel			= nullptr;		//부모_오른쪽 레이어
	CUI_Object*							m_pParentTopPanel			= nullptr;		//부모_상단 레이어

	//부모_왼쪽 레이어 자식
	CUI_Object*							m_pLeftLayerPanel			= nullptr;		//카페_왼쪽 레이어
	CUI_Object*							m_pLeftLayerHidePanel		= nullptr;		//카페_왼쪽 레이어 숨김
	CUI_Object*							m_pFixMode1Panel			= nullptr;		//카페_편집 모드1
	CUI_Object*							m_pFixMode2Panel			= nullptr;		//카페_편집 모드2
	CUI_Object*							m_pFurInfoPanel				= nullptr;		//카페_가구 상세정보
	CUI_Object*							m_pGiftInvenPanel			= nullptr;		//카페_선물 인벤토리
	CUI_Object*							m_pFurInvenPanel			= nullptr;		//카페_인벤토리
	CUI_Object*							m_pFurInvenInfoPanel		= nullptr;		//카페_인벤토리 상세정보
	CUI_Object*							m_pFurLineUpPanel			= nullptr;		//카페_가구 정렬 기준
	CUI_Object*							m_pPresetPanel				= nullptr;		//카페_프리셋

	//부모 오른쪽 레이어 자식
	CUI_Object*							m_pButtonInvPanel			= nullptr;		//버튼_초대권
	CUI_Object*							m_pInvTimePanel				= nullptr;		//알림_초대권 시간
	CUI_Object*							m_pInvRejectPanel			= nullptr;		//알림_초대권 거부
	CUI_Object*							m_pInvInfoPanel				= nullptr;		//카페_초대권
	CUI_Object*							m_pInvLineUpPanel			= nullptr;		//카페_초대권 정렬창
	CUI_Object*							m_pInvitingPanel			= nullptr;		//카페_초대하는중
	CUI_Object*							m_pRightLayerPanel			= nullptr;		//카페_오른쪽 레이어
	CUI_Object*							m_pCafeInfoPanel			= nullptr;		//카페_카페정보
	CUI_Object*							m_pCafeProfitPanel			= nullptr;		//카페_수익현왕
	CUI_Object*							m_pCafeProfitStatusPanel	= nullptr;		//알림
	CUI_Object*							m_pCafeProfitStatusFPanel	= nullptr;		//알림_FULL

	//부모 상단 레이어 자식
	CUI_Object*							m_pProfitInvTimePanel		= nullptr;		//알림 방문 시간
	CUI_Object*							m_pHideParentPanel			= nullptr;		//버튼_카페 부모 숨김
	CUI_Object*							m_pOpenParentPanel			= nullptr;		//버튼_카페 부모 열림

private:
	std::function<void(E_SCENETAG)>		m_pFunc_CafeScene;
	std::function<void(_bool)>			m_pFunc_PickFurniture;
	std::function<void(_bool)>			m_pFunc_RotFurniture;
	std::function<void(_bool)>			m_pFunc_StoFurniture;
	std::function<void(_bool)>			m_pFunc_AllStoFurniture;
	std::function<void(_bool)>			m_pFunc_SelFurnitureInfo;
	std::function<void(_bool)>			m_pFunc_CreateFurniturePick;
	std::function<void(_bool)>			m_pFunc_CafeInfoAmenity;
	std::function<void(_bool)>			m_pFunc_OpenFurInfo;
	std::function<void(_bool)>			m_pFunc_ExitFurInfo;
	std::function<void(_bool)>			m_pFunc_UpdateAmenity;
	std::function<void(_bool)>			m_pFunc_ProfitRecieve;
	std::function<void(_bool)>			m_pFunc_PresetName;
	std::function<void(_bool)>			m_pFunc_PresetNameEnd;
	std::function<void(_bool)>			m_pFunc_PresetName1;
	std::function<void(_bool)>			m_pFunc_PresetName2;
	std::function<void(_bool)>			m_pFunc_PresetName3;
	std::function<void(_bool)>			m_pFunc_PresetName4;
	std::function<void(_bool)>			m_pFunc_PresetName5;

private:
	std::function<void(_bool)>			m_pFunc_PresetInit1;
	std::function<void(_bool)>			m_pFunc_PresetSave1;
	std::function<void(_bool)>			m_pFunc_PresetApply1;
	std::function<void(_bool)>			m_pFunc_PresetInit2;
	std::function<void(_bool)>			m_pFunc_PresetSave2;
	std::function<void(_bool)>			m_pFunc_PresetApply2;
	std::function<void(_bool)>			m_pFunc_PresetInit3;
	std::function<void(_bool)>			m_pFunc_PresetSave3;
	std::function<void(_bool)>			m_pFunc_PresetApply3;
	std::function<void(_bool)>			m_pFunc_PresetInit4;
	std::function<void(_bool)>			m_pFunc_PresetSave4;
	std::function<void(_bool)>			m_pFunc_PresetApply4;
	std::function<void(_bool)>			m_pFunc_PresetInit5;
	std::function<void(_bool)>			m_pFunc_PresetSave5;
	std::function<void(_bool)>			m_pFunc_PresetApply5;

private:
	std::function<void(_bool)>			m_pFunc_Gift1;
	std::function<void(_bool)>			m_pFunc_Gift2;
	std::function<void(_bool)>			m_pFunc_Gift3;
	std::function<void(_bool)>			m_pFunc_Gift4;
	std::function<void(_bool)>			m_pFunc_Gift5;

private:
	CObstacle_Cafe*						m_pObstacle_Cafe			= nullptr;
	CUI_Object*							m_pUi_Furniture				= nullptr;
	CStageMap*							m_pTile						= nullptr;
	CStageMap*							m_pWall_L					= nullptr;
	CStageMap*							m_pWall_R					= nullptr;
	wstring								m_wstrMeshPath;
	wstring								m_wstrMeshName;

private:
	vector<CUI_Object*>					m_vecUi_Furniture;
	vector<T_FurnitureInfo>				m_vecFurAmenity;
	vector<T_FurnitureInfo>				m_vecFurName;
	_bool								m_bOneforAmenity = true;
	_bool								m_bOneforName = true;

private:
	vector<T_FurnitureInfo>				m_vecFilter_Furniture;
	vector<T_FurnitureInfo>				m_vecFilter_Deco;
	vector<T_FurnitureInfo>				m_vecFilter_Indoor;
	_bool								m_bOneforFurAmenity = true;
	_bool								m_bOneforFurName = true;

#pragma endregion

#pragma region 상점
	// 은호
	// 제영이형꺼 복붙중
private:
	enum class eShopEnum 
	{
		상점_일반, 상점_엘리그마, 상점_총력전, 상점_전술대회,
		Shop_End
	};

public:
	void					Enter_Shop_UI();

public:
	// 탭 버튼
	void					Click_Shop_Menu_Btn(_int iIndex);
	
	// 아이템 리스트
	void					CreateShopItem(_int iIndex, _int iStartItem, _int iEndItem);

	void					ClickShopItem(_int iIndex);
	void					ClickShopItem_Purchase(_int iIndex);

	void					Add_MultiChoice(_int iIndex);
	void					Reset_MultiChoice();
	void					Click_MultuChoice_Cancle();
	void					Click_MultuChoice_Ok();
								 
private:
	// 메인
	CUI_Object*				m_pShop_Main = nullptr;

	CUI_Object*				m_pShop_ResetList = nullptr;
	CUI_Object*				m_pShop_MultiList = nullptr;
	list<_int>				m_listItemSelect; // 선택된 아이템의 인데스 목록

	CUI_Object*				m_pShop_Purchase = nullptr;
	CUI_Object*				m_pShop_Purchase_Multi = nullptr;

	// 버튼
	_int					m_iShop_BtnSel = -1; // 선택된 버튼
	CUI_Object*				m_pShop_List[4] = { nullptr ,nullptr ,nullptr ,nullptr };

	// 게임 오브젝트
	vector<CUI_Object*>		m_vectorShopObj[4];
	vector<T_ShopItemInfo>	m_vectorSjopItm[4];


	// 구매관련

	// 재화
	_int					m_iMoney[3] = { 0,0,0 }; // 엘리그마, 총력전, 전술 대회

#pragma endregion

#pragma region 전투 UI 창
public:
	void	Create_Combat_Ing_Panel();
	void	Update_Combat_Ing(const _float& fTimeDelta);
	void	Active_Combat_Ing();
	void	Click_Skill(_int iSkillSlot);
	void	Active_Skill(_bool bActive);
	void	Skill_Change();
	void	Click_BattleSpeed();
	void	Click_AutoBattle();

public:
	CUI_Object* Get_CombatIngPanel() { return m_pCombatIngPanel; }

public:
	void	Set_BattleSpeed_Image();
	void	Set_AutoBattle_Image();
	void	Set_Assemble();
	void	Set_Dissipation();
	void	Set_Announcement(wstring wstrText, const _float& fMaxShowTime);

	void	Function_Assemble();

	CUI_Object*				m_pCombatIng_Announcement = nullptr;

private:
	CUI_Object*				m_pCombatIngPanel				= nullptr;
	CUI_Object*				m_pCombatIngBackground			= nullptr;
	CUI_Object*				m_pCombatIng_SkillThumnail		= nullptr;
	CUI_Object*				m_pCombatIng_Skill[3];
	CUI_Object*				m_pCombatIng_Cost				= nullptr;
	CUI_Object*				m_pCombatIng_GameSpeed			= nullptr;
	CUI_Object*				m_pCombatIng_Auto				= nullptr;
	CUI_Object*				m_pCombatIng_GraphBackground	= nullptr;
	CUI_Object*				m_pCombatIng_Graph				= nullptr;
	CUnit*					m_pCombatIng_FocusUnit			= nullptr;
	CUI_Object*				m_pCombatIng_FocusButton		= nullptr;
	CUI_Object*				m_pCombatIng_Assemble			= nullptr;
	CUI_Object*				m_pCombatIng_Dissipation		= nullptr;

private:
	_bool					m_bCombatIng_Assemble			= FALSE;
	_bool					m_bCombatIng_Dissipation		= FALSE;
	_float					m_fCombatIng_AnnounceTime		= -10.f;
	_float					m_fCombatIng_AnnounceTimeMax	= 1.4f;
	wstring					m_wstrCombatIng_AnnounceText	= L"";
	
#pragma endregion

#pragma region 전투 승리 창
public:
	void	Create_Combat_Victory_Panel();
	void	Setting_Combat_Victory();
	void	Active_Combat_Victory();
	void	Update_Combat_Victory(const _float& fTimeDelta);
	void	Combat_Victory_End_Button();

public:
	std::function<void(E_SCENETAG)>		m_fpComVtory_End_Button			= nullptr;
	CUI_Object*				m_pCombatVictoryPanel			= nullptr;
	CUI_Object*				m_pComVtory_Animation			= nullptr;

private:	// 전투 승리 창
	CUI_Object*				m_pComVtory_Defeat				= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_1			= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_2			= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_3			= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_4			= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_5			= nullptr;
	CUI_Object*				m_pComVtory_CharSlot_6			= nullptr;
	CUI_Object*				m_pDamageReport_Panel			= nullptr;
	CUI_Object*				m_pComVtory_EndButton			= nullptr;
	_float					m_pComVtory_Animation_fSizeX	= 100.f;
	_float					m_pComVtory_Animation_fSizeY	= 100.f;
	_float					m_pComVtory_UpdateTimer			= 0.f;


#pragma endregion


#pragma region 스토리 창

public:
	void				Create_Ui_Story();
	void				Story_OnEnable();
	void				Load_MainStory();
	void				Load_SubStory();

	void				OnClick_Story_Part_1();
	void				OnClick_Story_Part_2();

private:
	CUI_Object*			m_pStoryPanel						= nullptr;
	CUI_Object*			m_pStory_Main_Button				= nullptr;
	CUI_Object*			m_pStory_Sub_Button					= nullptr;
	CUI_Object*			m_pStory_SubParent_Panel			= nullptr;
	CUI_Object*			m_pStory_Choose_Episode_Panel		= nullptr;
	CUI_Object*			m_pStory_Episode_ScrollView_Panel	= nullptr;

#pragma endregion



#pragma region 회원가입 및 로그인 창

public:
	void					Create_Ui_Register();
	void					Update_Register(const _float& fTimeDelta);
	void					Change_RegisterType(_int iType);
	void					Register_Button_Login();
	void					Register_Set_Announce(_float fTime, wstring wstrText);
	void					Click_LoginRegister();

private:
	CUI_Object*				m_pRegisterPanel					= nullptr;
	CUI_Object*				m_pRegister_Edit_Id					= nullptr;
	CUI_Object*				m_pRegister_Edit_Password			= nullptr;
	CUI_Object*				m_pRegister_Button_Login			= nullptr;
	CUI_Object*				m_pRegister_Announce				= nullptr;

public:
	string*					m_pStrID;
	string*					m_pStrPassword;
	std::function<void()>	m_fpPlayFab_Login					= nullptr;
	std::function<void()>	m_fpPlayFab_Register				= nullptr;
private:
	enum	class			E_RegisterType						{ Regi_ID, Regi_PassWord, RegisterEnd };
	E_RegisterType			m_eRegisterType						= E_RegisterType::Regi_ID;
	_float					m_fRegisterAnnounceTime				= 0.f;
	_float					m_fRegister_BackSpaceTime			= 0.f;
	wstring					m_wstrRegisterAnnounceText;

#pragma endregion



#pragma region 랭킹 창

public:
	void				Create_Ui_Ranking();
	void				Ranking_Setting();
	void				Ranking_UI_Setting();
	void				Click_Ranking();
	void				Ranking_Set_Rank(const vector<T_PlayfabInfo>& vecRankInfo, _int iPlayerPoint, _int iPlayerRank);
	void				FriendList_Set(const vector<T_PlayfabInfo>& vecFrinedList);
	void				Ranking_PickBattle(_int iRank);
	void				Ranking_Add_Friend(_int iRank);

public:
	std::function<void()>	m_fpPlayFab_UpdateRankInfo	= nullptr;
	std::function<void()>	m_fpPlayFab_AddFriend		= nullptr;
	CUI_Object*				m_pRankingPanel				= nullptr;
	string					m_strFriendPlayFabId		= "Empty";

private:
	CUI_Object*				m_pRanking_LobbyChar		= nullptr;
	CUI_Object*				m_pRanking_RankingNumber	= nullptr;
	CUI_Object*				m_pRanking_TierIcon			= nullptr;
	CUI_Object*				m_pRanking_NickName			= nullptr;
	CUI_Object*				m_pRanking_Level			= nullptr;

	CUI_Object*				m_pRanking_ScrollView		= nullptr;

	vector<T_PlayfabInfo>	m_vecPlayfab_RankingInfo;
	vector<T_PlayfabInfo>	m_vecPlayfab_FriendInfo;
	_int					m_iPlayerRank;
	_int					m_iPlayerPoint;
#define RankMax (20)

#pragma endregion

#pragma region 러닝_TPS 전투 UI 

public:
	void					Create_Run_Combat_UI(); 
	void					Update_RunGame_UI(const _float& fTimeDelta); 
	void					Set_RunSkillBar(const _float& fValue); 
	void					Add_RunSkillBar(const _float& fValue); 
	void					Add_RunComboHit(const _uint& iComboHit); 
	void					Get_ApplyRunPlayerHpText(); 
	void					Set_ClearAll_RunCombatUI(); 
	void					Active_RunEndGameUI(const _bool& bWin = FALSE); 
	void					Goto_BeforePanel_UI(); 
	void					Active_RunHurtScreenUI(); 

private:
	_uint					m_iRunGame_HitCount = 0; 

	_float					m_fRunGame_ComboHit_OriginDelay = 2.f;
	_float					m_fRunGame_ComboHit_Delay = m_fRunGame_ComboHit_OriginDelay; 
	_float					m_fRunGame_SkillBar = 0.f; 

	CUI_Object*				m_pRunPlayerHpBar = nullptr; 
	CUI_Object*				m_pRunPlayerSkillBar = nullptr; 
	CUI_Object*				m_pRunPlayerComboHit = nullptr; 
	CUI_Object*				m_pRunPlayer_EndGameUI = nullptr; 
	CUI_Object*				m_pRunPlayer_HurtScreenUI = nullptr; 

#pragma endregion 

#pragma region 가챠 창

public:
	void					Create_Gacha_UI();
	void					OnEnable_GachaPanel();
	void					Change_GachaType(_int iType);
	void					Gacha_Start();
	void					Update_GachaPanelFor_CreateParticle(const _float& fTimeDelta);

private:
	CUI_Object*				m_pGacha_Panel				= nullptr;
	CUI_Object*				m_pGacha_Background			= nullptr;
	CUI_Object*				m_pGacha_Aru_Small			= nullptr;
	CUI_Object*				m_pGacha_Aru_Big			= nullptr;
	CUI_Object*				m_pGacha_Wakamo_Small		= nullptr;
	CUI_Object*				m_pGacha_Wakamo_Big			= nullptr;
	CUI_Object*				m_pGacha_Shiroko_Small		= nullptr;
	CUI_Object*				m_pGacha_Shiroko_Big		= nullptr;
	CUI_Object*				m_pGacha_High_Button_1		= nullptr;
	CUI_Object*				m_pGacha_High_Button_2		= nullptr;
	CUI_Object*				m_pGacha_High_Button_3		= nullptr;
	CUI_Object*				m_pGacha_High_Line_1		= nullptr;
	CUI_Object*				m_pGacha_High_Line_2		= nullptr;
	CUI_Object*				m_pGacha_High_Line_3		= nullptr;
	CUI_Object*				m_pGacha_Center_Line_1		= nullptr;
	CUI_Object*				m_pGacha_Center_Name		= nullptr;
	CUI_Object*				m_pGacha_Center_Line_2		= nullptr;
	CUI_Object*				m_pGacha_Center_Line_3		= nullptr;
	CUI_Object*				m_pGacha_Button_Gacha_1		= nullptr;
	CUI_Object*				m_pGacha_Button_Gacha_10	= nullptr;
	CUI_Object*				m_pGacha_Button_Start		= nullptr;

	_int					m_iGachaTime				= 0;
	_float					m_fGachaPanelParticleCool	= 0.f;

#pragma endregion 

#pragma region 가챠 뽑기 창
	enum E_GachaResultType { Start_Direct, Sign, TenGacha, Result, FinalResult, Gacha_End };

public:
	void					Create_GachaResult_UI();
	void					Gacha_Roulette();
	void					Update_GachaResult(const _float& fTimeDelta);
	void					Exit_Gacha();
	void					Skip_Gacha();

	void					Change_GachaResultType(E_GachaResultType	ResultType);
	void					Update_GachaAnimation();
	void					Gacha_Particle(_vec2 vPos, _vec3 vDir, _float fMinDist, _float fMaxDist, D3DXCOLOR color);

private:
	CUI_Object*				m_pGachaResult_Panel = nullptr;
	CUI_Object*				m_pGachaResult_StartDirectParent	= nullptr;
	CUI_Object*				m_pGachaResult_StartDirect_Circle	= nullptr;
	CUI_Object*				m_pGachaResult_StartDirect_SRank	= nullptr;
	CUI_Object*				m_pGachaResult_StartDirect_Fade		= nullptr;

	CUI_Object*				m_pGachaResult_SignParent			= nullptr;
	CUI_Object*				m_pGachaResult_InSide				= nullptr;

	CUI_Object*				m_pGachaResult_TenParent			= nullptr;

	CUI_Object*				m_pGachaResult_OneResult			= nullptr;
	CUI_Object*				m_pGachaResult_One_RareCard			= nullptr;		
	CUI_Object*				m_pGachaResult_One_StudentParent	= nullptr;		
	CUI_Object*				m_pGachaResult_One_Description		= nullptr;		// -400, -160
	CUI_Object*				m_pGachaResult_One_Name_Parent		= nullptr;		// -470, -180
	CUI_Object*				m_pGachaResult_One_Name_Star1		= nullptr;
	CUI_Object*				m_pGachaResult_One_Name_Star2		= nullptr;
	CUI_Object*				m_pGachaResult_One_Name_Star3		= nullptr;
	CUI_Object*				m_pGachaResult_One_CharInfoParent	= nullptr;		// 350, 0
	CUI_Object*				m_pGachaResult_One_Skip				= nullptr;		// 350, 0

	CUI_Object*				m_pGachaResult_FinalResult			= nullptr;
	CUI_Object*				m_pGachaResult_Prefab[10]			= { nullptr, };

	CUI_Object*				m_pGachaResult_BlackMask			= nullptr;
private:
	CUISkillObject*			m_pUiSkillObject_GachaInside		= nullptr;

	vector<_int>			m_vecGachaUnit_R;
	vector<_int>			m_vecGachaUnit_SR;
	vector<_int>			m_vecGachaUnit_SSR;

	_int					m_iArrGachaResultUnit[10]			= { 0, };
	_int					m_iArrGachaResultTier[10]			= { 0, };
	_float					m_fArrGachaEffectCool[10]			= { 0.f, };
	_int					m_iResultSize_Start					= (_int)(192 * 2.5f);
	_int					m_iResultSize_End					= 192;
	_bool					m_bArrGachaResultFirstEffect[10]	= { FALSE, };
	_bool					m_bGachaSignMask					= FALSE;
	// X -420, -210, 0, 210, 420
	// Y 150, -100

	_int					m_iGachaOneAnim						= 0;

	E_GachaResultType		m_eResultType						= E_GachaResultType::Start_Direct;
	_float					m_fGachaResultTime					= 0.f;
	_float					m_fGachaSignMaskTime				= 0.f;
	_float					m_fGachaFocusCoolTime				= 0.f;
#pragma endregion 
};

#endif // ! __UI_MANAGER_H__