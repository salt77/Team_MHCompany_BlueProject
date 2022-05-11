#pragma once
#ifndef __Terror_Droid_Mimesis_Shield_H__
#define __Terror_Droid_Mimesis_Shield_H__

#include "Monster.h" 

BEGIN(Engine)

class CEffectObject;

class ENGINE_DLL CTerror_Droid_Mimesis_Shield final : public CMonster
{
protected:
	explicit CTerror_Droid_Mimesis_Shield(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTerror_Droid_Mimesis_Shield(const CTerror_Droid_Mimesis_Shield& rhs);
	virtual ~CTerror_Droid_Mimesis_Shield();
public:
	static CTerror_Droid_Mimesis_Shield*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_Attack_1() override;
	void			Trigger_1() override;
	void			Trigger_2() override;
	void			Trigger_3() override;

private:
	void			Update_ExSkillFire(const _float& fTimeDelta);
	void			Create_AutomataBullet(const _vec3& vDir);

private:
	_bool				m_bExFireTrigger = FALSE;

	char*				m_szSecondWeaponName = "Bip001_Prop1";
	char*				m_szRightClavicle = "Bip001_R_Clavicle";
	char*				m_szLeftClavicle = "Bip001_L_Clavicle";

	_int				m_iOriginDeff = 0;

	_float				m_fExFireOriginDelay = 0.05f;
	_float				m_fExFireDelay = m_fExFireOriginDelay;

	CSphereCollider*	m_pSecondWeaponCol = nullptr;
	CSphereCollider*	m_pRightClavicleCol = nullptr;
	CSphereCollider*	m_pLeftClavicleCol = nullptr;

	CEffectObject*		m_pEffectShield = nullptr;
};

END

#endif