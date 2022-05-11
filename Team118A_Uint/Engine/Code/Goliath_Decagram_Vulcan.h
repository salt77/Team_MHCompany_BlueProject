#pragma once
#ifndef __Goliath_Decagram_Vulcan_H__
#define __Goliath_Decagram_Vulcan_H__

#include "Monster.h"

BEGIN(Engine)

class CEffectObject; 

class ENGINE_DLL CGoliath_Decagram_Vulcan final : public CMonster
{
protected:
	explicit CGoliath_Decagram_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CGoliath_Decagram_Vulcan(const CGoliath_Decagram_Vulcan& rhs);
	virtual ~CGoliath_Decagram_Vulcan();
public:
	static CGoliath_Decagram_Vulcan*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void	Trigger_Attack_1() override;
	virtual void	Trigger_Attack_2() override;
	virtual void	Trigger_Attack_3() override; 
	virtual void	Trigger_1() override;
	virtual void	Trigger_2() override;
	virtual void	Trigger_3() override;
	virtual void	Trigger_5() override;
	virtual void	Trigger_6() override;
	virtual void	Trigger_7() override;
	virtual void	Trigger_Dead() override;
	virtual void	Trigger_Reload() override;
	virtual void	Trigger_Buff_1() override;				// 여기부터 리로드 사운드 
	virtual void	Trigger_Buff_2() override;
	virtual void	Trigger_4() override;

private:
	void			Create_FireEffect(); 

private:
	_vec3			m_vTargetPos = VEC_ZERO; 

	char*			m_szSecondWeaponBone = "Vulcan_L_02"; 
	char*			m_szExWeaponBone = "Bip001_Head_01"; 

	CSphereCollider*	m_pSecondWeaponCol = nullptr; 
	CSphereCollider*	m_pExWeaponCol = nullptr; 

	CSkillObject*		m_pWarningEffect = nullptr; 
	CSkillObject*		m_pChargeEffect = nullptr; 
};

END

#endif //__Goliath_Decagram_Vulcan_H__