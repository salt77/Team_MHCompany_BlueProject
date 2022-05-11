#pragma once
#ifndef __TechSoldier_Decagram_AR_H__
#define __TechSoldier_Decagram_AR_H__

#include "Monster.h"

class ENGINE_DLL CTechSoldier_Decagram_AR final : public CMonster
{
protected:
	explicit CTechSoldier_Decagram_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTechSoldier_Decagram_AR(const CTechSoldier_Decagram_AR& rhs);
	virtual ~CTechSoldier_Decagram_AR();
public:
	static CTechSoldier_Decagram_AR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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