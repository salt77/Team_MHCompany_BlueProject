#pragma once
#ifndef __RoyalGuard_RedWinterSergeant_AR_H__
#define __RoyalGuard_RedWinterSergeant_AR_H__

#include "Monster.h"

class ENGINE_DLL CRoyalGuard_RedWinterSergeant_AR final : public CMonster
{
protected:
	explicit CRoyalGuard_RedWinterSergeant_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CRoyalGuard_RedWinterSergeant_AR(const CRoyalGuard_RedWinterSergeant_AR& rhs);
	virtual ~CRoyalGuard_RedWinterSergeant_AR();
public:
	static CRoyalGuard_RedWinterSergeant_AR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__RoyalGuard_RedWinterSergeant_AR_H__