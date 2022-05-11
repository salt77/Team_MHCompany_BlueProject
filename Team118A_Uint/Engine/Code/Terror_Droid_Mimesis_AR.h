#pragma once
#ifndef __Terror_Droid_Mimesis_AR_H__
#define __Terror_Droid_Mimesis_AR_H__

#include "Monster.h"

class ENGINE_DLL CTerror_Droid_Mimesis_AR final : public CMonster
{
protected:
	explicit CTerror_Droid_Mimesis_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTerror_Droid_Mimesis_AR(const CTerror_Droid_Mimesis_AR& rhs);
	virtual ~CTerror_Droid_Mimesis_AR();
public:
	static CTerror_Droid_Mimesis_AR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void		Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag) override;
	virtual void		Special_State_1(_float fDeltaTime);
	virtual void		Special_State_2(_float fDeltaTime);

protected:
	void			Trigger_Attack_1() override;
	void			Trigger_1() override; 

private:
};

#endif