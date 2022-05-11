#pragma once

#ifndef __WorldCreator_H__
#define __WorldCreator_H__

#include "Engine_Define.h"
#include "Monster.h"
#include "Base.h"


USING(Engine)

class ENGINE_DLL CWorldCreator final : public CBase
{
private: 
	explicit CWorldCreator();
	virtual ~CWorldCreator();
public:
	static CWorldCreator*					Create(wstring wstrWorldName);
	virtual void							Free() override;

public:
	void									Update_World(const _float& fTimeDelta);

public:
	void									Spawn_World();
	void									Spawn_Frontline();

	void									Set_WorldName(wstring wstrWorldName);
	void									Reset_World();

	_vec3									Get_StartPos(_int iIndex);
	vector<T_MONSTER_INFO>&					Get_MonsterInfo();
	const E_WORLD_MOD&						Get_WorldMod()				{ return m_eWorldMod;			}
	const E_WORLD_TYPE&						Get_WorldType()				{ return m_eWorldType;			}
	const wstring&							Get_WorldName()				{ return m_wstrWorldName;		}
	_int									Get_HighLevelMonster();

	_int									Get_GroupSize()				{ return (_int)m_vecGroupInfo.size(); }

	void									Ready_FrontLine();
private:
	HRESULT									Load_World(wstring wstrWorldName);
	HRESULT									Ready_World(wstring wstrWorldName);
	
	
	CUnit*									NameTranslateSpawn(wstring wstrName, _vec3 vPos, _vec3 vRot, T_MONSTER_INFO& tMonsterInfo);
	
private:
	wstring									m_wstrWorldName;
	queue<vector<CUnit*>*>					m_qFrontLines;

	vector<CGameObject*>                    m_vecMapObject;
	vector<CGameObject*>                    m_vecObstacleObject;
	vector<vector<T_MONSTER_INFO>>			m_vecGroupInfo;
	vector<vector<_vec3>>					m_vecGroupPos;
	vector<vector<_vec3>>					m_vecGroupRot;
	vector<T_MONSTER_INFO>					m_vecMonsterPool;
	vector<_vec3>							m_vecStartPos;
	E_WORLD_MOD								m_eWorldMod			= E_WORLD_MOD::MOD_END;
	E_WORLD_TYPE							m_eWorldType		= E_WORLD_TYPE::TYPE_END;

	_bool									m_bWorldBattleIng = TRUE;
	_bool									m_bIsTileFieldWorld = FALSE;
	_bool									m_bBattleEndDelay = FALSE; 

	_float									m_fBattleEndDelay = 2.f; 

	queue<_int>								m_qUserUnitLevel;
};



#endif