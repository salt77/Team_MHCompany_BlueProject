#pragma once
#ifndef __Drone_Decagram_Missile_H__
#define __Drone_Decagram_Missile_H__

#include "Monster.h"

class ENGINE_DLL CDrone_Decagram_Missile final : public CMonster
{
protected:
	explicit CDrone_Decagram_Missile(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CDrone_Decagram_Missile(const CDrone_Decagram_Missile& rhs);
	virtual ~CDrone_Decagram_Missile();
public:
	static CDrone_Decagram_Missile*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_1() override; 
	void			Trigger_2() override;
	void			Trigger_3() override; 
	void			Trigger_5() override;
	void			Trigger_6() override;
	void			Trigger_7() override;
	void			Trigger_Dead() override; 

private:
	void			Create_Missile(const _bool& bLeft); 

private:
	char*			m_szSecondWeaponBone = "Bone_Missile_R"; 

	_vec3			m_vTargetPos = VEC_ZERO; 

	CSphereCollider*	m_pSecondWeaponCol = nullptr; 
};

#endif