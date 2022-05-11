#pragma once
#ifndef __Terror_Droid_Mimesis_RL_H__
#define __Terror_Droid_Mimesis_RL_H__

#include "Monster.h"

class ENGINE_DLL CTerror_Droid_Mimesis_RL final : public CMonster
{
protected:
	explicit CTerror_Droid_Mimesis_RL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTerror_Droid_Mimesis_RL(const CTerror_Droid_Mimesis_RL& rhs);
	virtual ~CTerror_Droid_Mimesis_RL();
public:
	static CTerror_Droid_Mimesis_RL*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

private:
};

#endif