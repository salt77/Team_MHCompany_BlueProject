#pragma once
#ifndef __Drone_Decagram_Vulcan_H__
#define __Drone_Decagram_Vulcan_H__

#include "Monster.h"

class ENGINE_DLL CDrone_Decagram_Vulcan final : public CMonster
{
protected:
	explicit CDrone_Decagram_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CDrone_Decagram_Vulcan(const CDrone_Decagram_Vulcan& rhs);
	virtual ~CDrone_Decagram_Vulcan();
public:
	static CDrone_Decagram_Vulcan*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_5() override;
	void			Trigger_6() override;
	void			Trigger_7() override;
	void			Trigger_Dead() override;

private:
	char*			m_szSecondWeaponBone = "Bone_Vulcan_R"; 

	CSphereCollider*	m_pSecondWeaponCol = nullptr; 
};

#endif