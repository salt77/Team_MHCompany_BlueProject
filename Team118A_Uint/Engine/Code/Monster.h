#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Unit.h"
#include "DataManager.h"
#include "Boss_HpBar.h"

BEGIN(Engine)

class ENGINE_DLL CMonster : public CUnit
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	static CMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	virtual void		Free() override;

public:	
	virtual	HRESULT		Ready_Object()									override;
	virtual _int		Start_Object(const _float& fTimeDelta)			override;
	virtual	_int		Update_Object(const _float& fTimeDelta)			override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void		Render_Object(_int iPass)						override;
	virtual void		Set_Constantable()								override;

public:	
	virtual void		OnEnable()		override;
	virtual void		OnDisable()		override;
	virtual void		OnDestroy()		override;
	virtual void		On_Collision(CGameObject* pDest)	override;
	virtual void		Load_CombatUi() override;

public:	
	T_MONSTER_INFO					Get_MonsterInfo()				{ return m_tMonsterInfo;				}
	virtual const E_CHAR_ATKTYPE	Get_AttackType()	override	{ return m_tMonsterInfo.eAtkType;		}
	virtual const E_CHAR_DFFTYPE	Get_DefenseType()	override	{ return m_tMonsterInfo.eDiffType;		}
	CBoss_HpBar*					Get_Boss_HP()					{ return m_pBossHpBar;					}
	
public:	
	virtual void		Set_MonsterInfo(T_MONSTER_INFO tMonsterInfo);
	virtual void		Begin_Battle(_float fDeltaTime) override;

protected:
	virtual _int		Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType) override;

protected:	
	virtual void		Ready_DMesh() override;
	virtual void		Update_Collider(const _float& fTimeDelta) override;
	virtual void		Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag) override;

	void				Update_BossWarningEffect(const _float& fTimeDelta); 
	void				Check_PlayerAllDead(); 

protected:
	_bool				m_bNormalBoss = FALSE;
	_bool				m_bCanExSkill = TRUE;

	T_MONSTER_INFO		m_tMonsterInfo;

	_float				m_fMonsterExSkillTimer = 3.f;
	_float				m_fMonsterExSkillOriginTimer = 15.f;
				
	_float				m_fWarningEffectTime = 4.f;

	CBoss_HpBar*		m_pBossHpBar = nullptr;
	CSkillObject*		m_pBossWarningEffect = nullptr;
};

END

#endif