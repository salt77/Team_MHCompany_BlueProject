#pragma once
#ifndef __Soldier_Helmet_SG_H__
#define __Soldier_Helmet_SG_H__

#include "Monster.h"

class ENGINE_DLL CSoldier_Helmet_SG final : public CMonster
{
protected:
	explicit CSoldier_Helmet_SG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSoldier_Helmet_SG(const CSoldier_Helmet_SG& rhs);
	virtual ~CSoldier_Helmet_SG();
public:
	static CSoldier_Helmet_SG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__Soldier_Helmet_SG_H__