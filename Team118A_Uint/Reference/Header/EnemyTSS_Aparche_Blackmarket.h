#pragma once
#ifndef __EnemyTSS_Aparche_Blackmarket_H__
#define __EnemyTSS_Aparche_Blackmarket_H__

#include "Monster.h"

class ENGINE_DLL CEnemyTSS_Aparche_Blackmarket final : public CMonster
{
protected:
	explicit CEnemyTSS_Aparche_Blackmarket(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CEnemyTSS_Aparche_Blackmarket(const CEnemyTSS_Aparche_Blackmarket& rhs);
	virtual ~CEnemyTSS_Aparche_Blackmarket();
public:
	static CEnemyTSS_Aparche_Blackmarket*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_Attack_2() override; 
	void			Trigger_Attack_3() override; 
	void			Trigger_1() override; 
	void			Trigger_2() override; 
	void			Trigger_3() override; 
	void			Trigger_4() override; 
	void			Trigger_5() override;
	void			Trigger_6() override;
	void			Trigger_7() override;
	void			Trigger_8() override;

private:
	void			Create_NormalBullet(); 
	void			Create_MissileBullet(_bool bLeft); 

private:
	char*			m_szLeftSecondWeapon = "bone_WP_L_01"; 
	char*			m_szRightSecondWeapon = "bone_WP_R_01"; 

	CSphereCollider*	m_pLeftSecondCol = nullptr; 
	CSphereCollider*	m_pRightSecondCol = nullptr; 
};

#endif