#pragma once
#ifndef __Droid_Yoheki_Shield_H__
#define __Droid_Yoheki_Shield_H__

#include "Monster.h"

class ENGINE_DLL CDroid_Yoheki_Shield final : public CMonster
{
protected:
	explicit CDroid_Yoheki_Shield(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CDroid_Yoheki_Shield(const CDroid_Yoheki_Shield& rhs);
	virtual ~CDroid_Yoheki_Shield();
public:
	static CDroid_Yoheki_Shield*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;

public:
	virtual	HRESULT	Ready_Object() override;
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
	virtual void	ExSkill(_float fDeltaTime) override; 
	virtual	void	Special_State_1(_float fDeltaTime) override; 
	virtual	void	Special_State_2(_float fDeltaTime) override;

	virtual void	Trigger_Attack_1() override;
	virtual void	Trigger_Attack_2() override; 
	virtual void	Trigger_Attack_3() override; 
	virtual void	Trigger_1() override; 
	virtual void	Trigger_2() override;
	virtual void	Trigger_5() override; 
	virtual void	Trigger_7() override; 
	virtual void	Trigger_Dead() override; 

private:
	_int				m_iOriginDef = 0; 

	_float				m_fExSkillOriginTime = 8.f; 
	_float				m_fExSkillTime = m_fExSkillOriginTime;

	char*				m_szShieldBoneName = "Bip001_Prop1"; 

	CSphereCollider*	m_pShieldCollider = nullptr;
	CEffectObject*		m_pEffectShield = nullptr; 
};

#endif