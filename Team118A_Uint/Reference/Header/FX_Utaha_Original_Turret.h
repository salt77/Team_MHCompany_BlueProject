#pragma once
#ifndef __FX_Utaha_Original_Turret_H__
#define __FX_Utaha_Original_Turret_H__

#include "Monster.h"

class ENGINE_DLL CFX_Utaha_Original_Turret final : public CMonster
{
protected:
	explicit CFX_Utaha_Original_Turret(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CFX_Utaha_Original_Turret(const CFX_Utaha_Original_Turret& rhs);
	virtual ~CFX_Utaha_Original_Turret();
public:
	static CFX_Utaha_Original_Turret*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

private:
};

#endif