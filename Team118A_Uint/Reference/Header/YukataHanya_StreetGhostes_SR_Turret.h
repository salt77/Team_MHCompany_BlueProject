#pragma once
#ifndef __YukataHanya_StreetGhostes_SR_Turret_H__
#define __YukataHanya_StreetGhostes_SR_Turret_H__

#include "Monster.h"

class ENGINE_DLL CYukataHanya_StreetGhostes_SR_Turret final : public CMonster
{
protected:
	explicit CYukataHanya_StreetGhostes_SR_Turret(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CYukataHanya_StreetGhostes_SR_Turret(const CYukataHanya_StreetGhostes_SR_Turret& rhs);
	virtual ~CYukataHanya_StreetGhostes_SR_Turret();
public:
	static CYukataHanya_StreetGhostes_SR_Turret*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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