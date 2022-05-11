#pragma once
#ifndef __Droid_HelmetBoss_SMG_H__
#define __Droid_HelmetBoss_SMG_H__

#include "Monster.h"

class ENGINE_DLL CDroid_HelmetBoss_SMG final : public CMonster
{
protected:
	explicit CDroid_HelmetBoss_SMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CDroid_HelmetBoss_SMG(const CDroid_HelmetBoss_SMG& rhs);
	virtual ~CDroid_HelmetBoss_SMG();
public:
	static CDroid_HelmetBoss_SMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void					Free() override;

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
};

#endif //__Droid_HelmetBoss_SMG_H__