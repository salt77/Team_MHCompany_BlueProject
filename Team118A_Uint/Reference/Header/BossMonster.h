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


	// �׷α� ������ ����
protected:
	virtual _int		Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType);
	void				Set_GroggyPercent(const _float& fPercentigy);

	virtual void		Add_GroggyGage(const _float& fDamage);

	const _float		Get_GroggyRatio()					{ return m_fGroggy_Gage / m_fGroggy_MaxGage; }

	// ������ ����
protected:
	void				Add_Phase()							{ m_iPhase++;							}

	const _int&			Get_Phase()							{ return m_iPhase;						}

protected:
	_float				m_fExSkill2_Gage			= 0.f;
	_float				m_fExSkill2_MaxGage			= 3.f;

	_float				m_fGroggy_Gage				= 0.f;			// ���ִ� �׷α� ü��
	_float				m_fGroggy_MaxGage			= 0.f;			// �ִ� �׷α� ü�� (�� ���� �׷α� ���·� ����)
	_float				m_fGroggy_Time				= 0.f;			// �׷α� ���� ���� �ð�
	_float				m_fGroggy_MaxTime			= 5.f;			// �׷α� �ɷ����� �׷α� ��� ���� �ð� (Ȥ�� �ڽ��� ���� Groggy �ִϸ��̼ǳ����� �ٷ� Idle�� ���ƿ͵��� ����)

	_int				m_iPhase					= 1;			// ���� ������

	E_ACTION			m_eMainAction = E_ACTION::ACTION_END;
};

END

#endif //__BOSSMONSTER_H__