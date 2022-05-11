#pragma once

#ifndef __TILE_FIELD_H__
#define __TILE_FIELD_H__

#include "Engine_Define.h"

#include "Base.h"
#include "CombatTile.h"
#include "Mascot.h"
#include "WorldUI.h"
#include "StaticObject.h"
USING(Engine)

class ENGINE_DLL CTileField final : public CBase
{
private: 
	explicit CTileField();
	virtual ~CTileField();
public:
	static CTileField*						Create(wstring wstrTileFieldName);
	virtual void							Free() override;

public:
	void									Update_TileField(const _float& fTimeDelta);
	HRESULT									Ready_TileField(wstring wstrTileFieldName);

	void									Spawn_FieldTile();
	const vector<T_MONSTER_INFO>&			Get_MonsterPoolInfo() { return m_vecMonsterPool; }
	
public:  
	void									Add_Squad_Mascot(CMascot* pMascot);
	vector<E_SQUAD>							Get_Squad();
	void									Change_Turn();
	void									Call_TurnChange();

	_bool									Get_CombatSceneChange() { return m_bCombatSceneChange; }
	void									Set_CombatSceneChange(_bool bChange) { m_bCombatSceneChange = bChange; }

	void									End_War(_bool bWin);

	void									Set_Enswer(_int iAnswer) { m_iAnswer = iAnswer; }
	void									Close_CheckUI();

	void									Release_PlayerMascot(CMascot* pMascot);
	void									Release_PlayerMascot(E_SQUAD eSquad);

	const vector<CMascot*>&					Get_PlayerMascot() { return m_vecPlayerMascot; }
	const vector<CMascot*>&					Get_MonsterMascot() { return m_vecMonsterMascot; }

private:
	HRESULT									Load_TileField(wstring wstrTileFieldName);
	void									Add_MonsterPool(const vector<T_MONSTER_INFO>& vecMonsterPool);
	
	void									Player_Update(_float fTimeDelta);
	void									Monster_Update(_float fTimeDelta);

	void									Pickup_Mascot();
	void									Pick_Move();
	void									Pick_End();

	void									Pick_StartPos();
	void									Select_Player(_int iIndex);
	void									Select_NextPlayer();

	void									Enable_Check(_int iTileIndex);
	void									Pick_MoveTile();
	void									Pick_WarpTile();
	_bool									Pick_SwapTile();

	void									Check_WeakTile();

	void									Check_Move();
	_bool									Check_Tile();
	_bool									Check_Meeting();

	//몬스터 행동
	_bool									Action_Monster();
	_bool									Action_Aggressive();
	_bool									Action_Tracking();
	CMascot*								Search_Player();
	void									Search_Tile(CMascot*& pPlayer);

	//ASTAR 
	_int									Find_Path(_int iTileIndex);
	_int									PathFinding(_int iTargetIndex);
	list<T_TILENODE>						Add_OpenList(T_TILENODE* pParentTileNode, _int iTargetIndex);

	//타일 간 연결 관계 설정
	void									Set_NearTileIndex(_int iTileIndex);

	//사라진 마스코트를 제거
	void									Clear_DeadMascot();
	
	enum									E_CHECK { CHECK_TURN_END, CHECK_STAGE_START ,CHECK_TURN_CHANGE, CHECK_MOVE, CHECK_START, CHECK_WARP, CHECK_CHAR_READY, CHECK_CHAR_COMBAT, CHECK_END };
	void									Open_CheckUI(E_CHECK eCheckUI);
	
	void									Set_EnemyInfo();
	void									Set_SquadWindow(CMascot* pPlayerMascot);


	void									After_ReleasePlayer();
private:
	void									Game_Start();
	void									Start_Game();

	void									Check_Accept();
	void									Check_Cancle();
	void									Change_Squad();
	void									Change_TurnButton();
	void									Change_AutoTurnChange();

	void									CharReady_Release();
	void									CharReady_Swap();
	void									CharReady_Trans();
	void									CharReady_Cancel();

	void									CharCombat_Select();
	void									CharCombat_Trans();
	void									CharCombat_Cancel();

	void									Check();
	
	void									SetUIActive(_bool bActive);

private:
	_bool									m_bUIActive = TRUE;
	_float									m_fUITimer = 0.f;

	_bool									m_bAutoTurnChange = FALSE;
	_int									m_iTurn = 0;

	E_CHECK									m_eCheck = CHECK_END;
	_bool									m_bCheck = FALSE;

	_int									m_iAnswer = 0;
	CUI_Object*								m_pCheckUI = nullptr;
	CUI_Object*								m_pTileFieldUI = nullptr;

	CUI_Object*								m_pMyPhase = nullptr;
	CUI_Object*								m_pEnemyPhase = nullptr;
	CUI_Object*								m_pStageStart = nullptr;

	CUI_Object*								m_pCharReadyUI = nullptr;
	CUI_Object*								m_pCharCombatUI = nullptr;

	_int									m_iMoveIndex;

	map<_int, CWorldUI*>					m_mapStartWorldUI;
	vector<CStaticObject*>					m_vecEnableMesh;
	vector<CStaticObject*>					m_vecSelectTile;

	CMascot*								m_pSelectMascot = nullptr;
	CMascot*								m_pSwapMascot = nullptr;

	_int									m_iSelectIndex;

	_bool									m_bGameStart = FALSE;
	_bool									m_bPlayerTurn = FALSE;

	const _int								m_iMaxArray = 56;
	const _int								m_iMaxX = 8;
	const _int								m_iMaxZ = 7;

	CCombatTile*							m_arrayCombatTile[56];

	vector<T_MONSTER_INFO>					m_vecMonsterPool;
	vector<CMascot*>						m_vecPlayerMascot;
	vector<CMascot*>						m_vecMonsterMascot;

	wstring									m_wstrTileFieldName;
	
	_bool									m_bCombatSceneChange = FALSE;

	//너비 우선 탐색용 컨테이너
	vector<_int>							m_vecSearchedTile;
	vector<_int>							m_vecSearchTile;
	

	//ASTAR 
	list<T_TILENODE>						m_listClosedNode;
	list<T_TILENODE>						m_listOpenNode;


	CMascot*								m_pWarPlayer = nullptr;
	CMascot*								m_pWarMonster = nullptr;

	CMascot*								m_pPickMascot = nullptr;
	CRectBuffer*							m_pPickRectBuffer = nullptr;
	_float									m_fPickTimer = 0.f;

	_bool									m_bKeyDown = FALSE;
};



#endif