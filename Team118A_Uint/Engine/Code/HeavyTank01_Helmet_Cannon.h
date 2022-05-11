#pragma once
#ifndef __HeavyTank01_Helmet_Cannon_H__
#define __HeavyTank01_Helmet_Cannon_H__

#include "Monster.h" 

class ENGINE_DLL CHeavyTank01_Helmet_Cannon final : public CMonster
{
protected:
	explicit CHeavyTank01_Helmet_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CHeavyTank01_Helmet_Cannon(const CHeavyTank01_Helmet_Cannon& rhs);
	virtual ~CHeavyTank01_Helmet_Cannon();
public:
	static CHeavyTank01_Helmet_Cannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _bool bIsBoss = FALSE); 
	virtual void			Free() override;

public:
	virtual	HRESULT	Ready_Object(_bool bIsBoss);
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void	Render_Object(_int iPass) override;

	virtual void	Set_Constantable() override;

public:
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;
	virtual void	OnDestroy() override;

protected:
	virtual void	Trigger_Attack_1() override;
	virtual void	Trigger_Attack_2() override;
	virtual void	Trigger_1() override;
	virtual void	Trigger_2() override;
	virtual void	Trigger_3() override;
	virtual void	Trigger_4() override;
	virtual void	Trigger_5() override;
	virtual void	Trigger_7() override; 
	virtual void	Trigger_9() override;
	virtual void	Trigger_Dead() override;

private:
	void			Fire_NormalAttack(CSphereCollider* pCollider); 
	void			Create_FireEffect(); 
	void			Fire_ExSkill(_uint iExFireCount, CSphereCollider* pCollider_1, CSphereCollider* pCollider_2 = nullptr); 

private: 
	char*			m_szWeaponSecond = "Weapon_03"; 

	_float			m_fAttackRange = 2.f; 

	_vec3			m_vTargetPos = VEC_ZERO; 
	_vec3			m_vRotateDir = VEC_ZERO; 

	CSphereCollider*	m_pSecondWeaponCol = nullptr; 
};

#endif