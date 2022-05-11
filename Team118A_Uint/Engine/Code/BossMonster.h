#pragma once

#ifndef __BOSSMONSTER_H__
#define __BOSSMONSTER_H__

#include "Monster.h"
#include "DataManager.h"
#include "Wall512.h"

BEGIN(Engine)

class ENGINE_DLL CBossMonster abstract : public CMonster
{
public:
	explicit CBossMonster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CBossMonster(const CBossMonster& rhs);
	virtual ~CBossMonster();

public:
	virtual	void		Free() override; 

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
	virtual void		Load_CombatUi() override;

public:
	virtual void		Begin_Battle(_float fDeltaTime) override;
	virtual void		Boss_Groggy(_float fDeltaTime);

public:
	void				Boss_FSM_Check(void(CUnit::*pNext)(_float));

public:
	virtual void		Set_MonsterInfo(T_MONSTER_INFO tMonsterInfo);

protected:
	virtual	void		Update_ExSkill2_Gage(const _float& fTimeDelta); 


	// 그로기 게이지 관련
protected:
	virtual _int		Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType);
	void				Set_GroggyPercent(const _float& fPercentigy);

	virtual void		Add_GroggyGage(const _float& fDamage);

	const _float		Get_GroggyRatio()					{ return m_fGroggy_Gage / m_fGroggy_MaxGage; }

	// 페이즈 관련
protected:
	void				Add_Phase()							{ m_iPhase++;							}

	const _int&			Get_Phase()							{ return m_iPhase;						}

protected:
	_float				m_fExSkill2_Gage			= 0.f;
	_float				m_fExSkill2_MaxGage			= 3.f;

	_float				m_fGroggy_Gage				= 0.f;			// 차있는 그로기 체력
	_float				m_fGroggy_MaxGage			= 0.f;			// 최대 그로기 체력 (다 차면 그로기 상태로 변함)
	_float				m_fGroggy_Time				= 0.f;			// 그로기 현재 남은 시간
	_float				m_fGroggy_MaxTime			= 5.f;			// 그로기 걸렸을때 그로기 모션 나올 시간 (혹은 자신의 보스 Groggy 애니메이션끝나면 바로 Idle로 돌아와도됨 선택)

	_int				m_iPhase					= 1;			// 현재 페이즈

	E_ACTION			m_eMainAction = E_ACTION::ACTION_END;
};

END

#endif //__BOSSMONSTER_H__