#pragma once
#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "Engine_Define.h" 
#include "Character.h"
#include "Base.h"
#include "WorldCreator.h"
#include "TileField.h"
BEGIN(Engine)

class CEffectObject;

END

class ENGINE_DLL CGameManager : public CBase
{
	DECLARE_SINGLETON(CGameManager)

public:
	CGameManager();
	virtual ~CGameManager();

public:
	virtual void	Free() override;

	void			Ready_GameManager();
	void			Update_GameManager(const _float& fTimeDelta);

/*	式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 等檜攪 衙棲盪 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	void			Login_Account();
	void			LogOut();
	
	void			Get_CurrentDate(T_DATEINFO* pDateInfoOut);
	_int			Get_DateDiffrence(T_DATEINFO tPastDate, T_DATEINFO tFutureDate);
	const WCHAR*	Get_NickName();
	const _uint&	Get_Level();
	const _uint&	Get_EXP();
	const _uint&	Get_Credit();
	const _uint&	Get_Eligma(); 
	const _uint&	Get_AP();
	const _uint&	Get_BluePyroxene();
	void			Set_NickName(wstring wstrNickName);
	void			Set_Level(_uint iLevel);
	void			Set_EXP(_uint iEXP);
	void			Set_Credit(_uint iCredit);
	void			Set_Eligma(_uint iEligma);
	void			Set_AP(_uint iAP);
	void			Set_BluePyroxene(_uint iBluePyroxene);
	void			Add_Credit(_int iCredit);
	void			Add_Eligma(_int iEligma);
	const T_USERINFO& Get_UserInfo();
	
/*	式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 蝶纔檜雖(錯萄Creator) 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
	void			Initialize_WorldResource();
	void			Release_WorldResource();

	void			Set_NextWorld(CWorldCreator* pNextWorld)	{ m_pNextWorld = pNextWorld;													}
	void			Set_BattleIng(_bool bState)					{ m_bBattleIng = bState;														}
	void			Set_BattleSpeed(E_BattleSpeed eBattleSpeed) { m_eGameSpeed = eBattleSpeed;													}
	void			Set_AutoBattle(_bool bState)				{ m_bAutoBattle = bState;														}
	void			Set_EndBattle();
	void			Set_IsBattleWin(_bool bState)				{ m_bIsBattleWin = bState;														}	
	void			Set_IsTileFiledBattle(_bool bState);
	void			Add_BattleTotalTime(const _float& fTimeDelta) { m_fBattleTotalTime += fTimeDelta;											}
	void			Reset_BattleTotalTime()						{ m_fBattleTotalTime = 0.f;														}

	map<E_WORLD_MOD, list<CWorldCreator*>>* Get_Worlds()		{ return &m_mapWorldCreators;													}
	CWorldCreator*	Get_NextWorld()								{ return m_pNextWorld;															}
	_bool			Get_BattleIng()								{ return m_bBattleIng;															}
	_float			Get_Cost()									{ return m_fBattleCost;															}
	E_BattleSpeed	Get_BattleSpeed()							{ return m_eGameSpeed;															}
	_bool			Get_AutoBattle()							{ return m_bAutoBattle;															}
	vector<_vec3>	Get_AliveStrikersPosition();
	_bool			Get_IsBattleWin()							{ return m_bIsBattleWin;														}	
	_bool			Get_IsTileFiledBattle()						{ return m_bIsTileFiledBattle;													}
	_float			Get_BattleTotalTime()						{ return m_fBattleTotalTime;													}

	void			Add_Cost(const _float& fIncrease);
/*	式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 顫橾(CTileField) 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
	void			Set_TileField(CTileField* pNextTileField) { Safe_Release(m_pTileField); m_pTileField = pNextTileField; }
	CTileField*		Get_TileField()								{ return m_pTileField; }
	void			Add_PlayerMascot(E_SQUAD eSquad);
	vector<E_SQUAD> Get_TileFieldSquad();
	
	void			End_War(_bool bWin);

/*	式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 檣漸饜葬 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	static _bool	Sort_ItemFilter(T_ItemInfo& pSrc, T_ItemInfo& pDst);
	void			Get_Inventory(_bool* bFilter, E_ItemSortType eSortType, vector<T_ItemInfo*>* listItem);
	T_ItemInfo*		Get_Item(_int iIndex) { return &m_tAll_Item[iIndex]; }
	void			Set_ItemCount(_int iIndex, _uint iCount)	{ m_tAll_Item[iIndex].iCount = iCount;		}
	void			Add_ItemCount(_int iIndex, _int iCount)		{ m_tAll_Item[iIndex].iCount += iCount;		}
	void			Sort_Item(vector<T_ItemInfo*>* vecItem);

/*	式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 蘋む 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
	T_FurnitureInfo* Get_Furniture(_int iIndex) { return &m_tAll_Furniture[iIndex]; }
	vector<T_FurnitureInfo> Get_vecFurniture() { return m_tAll_Furniture; }

/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 ら撩 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	void			Ready_Formtion();
	void			Release_Formation();
	void			Initialize_Formation();

	_bool			Add_Formation(CCharacter * pCharacter, E_SQUAD eSquad);
	_bool			Add_Formation(E_CHAR_NAME eCharName, E_SQUAD eSquad,E_CHAR_COMBATTYPE eCombatType,_int iIndex);
	_bool			Add_Formation(E_CHAR_NAME eCharName, E_SQUAD eSquad);

	E_SQUAD			DuplicateCheck_Character(E_CHAR_NAME eCharName, E_SQUAD eSquadStart, E_SQUAD eSquadEnd, E_SQUAD eCurrentSquad = E_SQUAD::SQUAD_END);
	void			Spawn_Formation(E_SQUAD eSquad);
	void			DeleteStudent(E_CHAR_NAME eCharName, E_SQUAD eStartSquad, E_SQUAD eEndSquad);
	CCharacter*		Create_CharacterByEnum(E_CHAR_NAME eCharName);
	void			Reset_Squad(E_SQUAD eSquad);
	void			Swap_Character(E_SQUAD eSquad, E_CHAR_COMBATTYPE eCombatType, _int iDestIndex, _int iSrcIndex);

	map<_int, vector<CCharacter*>>* Get_Squad(E_SQUAD eSquad);
	map<_int, vector<CCharacter*>>* Get_CurrentSquad() { return m_mapCurrentSquad; }
	vector<CCharacter*>*			Get_CurrentSquad_Strikers();
	vector<CCharacter*>				Get_CurrentSquad_Alive_Strikers();
	void							Set_CurrentSquadEnum(E_SQUAD eCurrentSquad);
	E_SQUAD							Get_CurrentSquadEnum() { return m_eCurrentSquad; }

	_int			GetReaderIndex(E_SQUAD eSquad);
	void			SetReaderIndex(E_SQUAD eSquad,_int iNewReaderIndex);

	

/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	void			Collect_Update();


/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 蝶鑒 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	void			Initialize_SkillOrder();
	void			Use_Skill(CCharacter* pChar);

	void			Set_SkillOnMouse(_bool bState)				{ m_bSkillOnMouse = bState;				}

	vector<_int>*	Get_SkillCost()								{ return &m_vecBattleSkillOrder;		}
	_bool			Get_SkillOnMouse()							{ return m_bSkillOnMouse;				}
	CEffectObject*	Get_RangeEffect(E_RangeEffect eRange)		{ return m_pRangeEffect[(_int)eRange];	}
	CUnit*			Get_SkillUnit(_int iSkillSlot);

/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/


/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 QTE 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	



/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/


/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式 PlayFab 婦溼 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	std::function<void()>						m_fpPlayFab_Logout		= nullptr;



/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/
public:
	_bool					Get_PlayerInvn()							{ return m_bPlayerInvn;					}
	



/* 式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式*/



private:
	_bool					m_bReady = FALSE;

protected:
	T_USERINFO*				m_pLocalUser = nullptr;
	vector<T_ItemInfo>		m_tAll_Item;
	vector<T_EquipInfo>		m_tAll_Equip;
	vector<T_FurnitureInfo> m_tAll_Furniture;

	static E_ItemSortType	m_eItemSortType;

	map<_int, vector<CCharacter*>>			m_mapFormation[(_int)E_SQUAD::SQUAD_END];
	map<_int, vector<CCharacter*>>*			m_mapCurrentSquad = nullptr;
	_int									m_iReaderIndex[(_int)E_SQUAD::SQUAD_END];
	E_SQUAD									m_eCurrentSquad = E_SQUAD::SQUAD_1;
	vector<_int>							m_vecBattleSkillOrder;
	queue<_int>								m_qSkillOrder;


protected:
	map<E_WORLD_MOD, list<CWorldCreator*>>	m_mapWorldCreators;

	CWorldCreator*			m_pNextWorld				= nullptr;
	CTileField*				m_pTileField				= nullptr;

	E_BattleSpeed			m_eGameSpeed				= E_BattleSpeed::_X1;
	_bool					m_bBattleIng				= FALSE;
	_bool					m_bAutoBattle				= FALSE;
	_float					m_fBattleCost				= 0.f;
	_int					m_iLastPickSkillSlot		= 0;
	_bool					m_bPlayerInvn				= FALSE;						// 議葛攪 濠翕 羹溘 �蛹� G + H
	_bool					m_bIsBattleWin				= FALSE;
	_bool					m_bIsTileFiledBattle		= FALSE;
	_float					m_fBattleTotalTime			= 0.f;

protected:
	_bool					m_bSkillOnMouse				= FALSE;
	CEffectObject*			m_pRangeEffect[(_int)E_RangeEffect::RE_End];

protected:
	_bool					m_bQTE						= FALSE;

public:
	_bool					m_bUserBattle				= FALSE;
	_int					m_iEnemyUserSquad[4]		= { 0, 0, 0, 0 };
	_int					m_iEnemyUserSquadLevel[4]	= { 0, 0, 0, 0 };

};

#endif // !__GAMEMANAGER_H__