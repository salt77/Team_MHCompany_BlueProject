#pragma once
#ifndef __Gasmask_Arius_HMG_H__
#define __Gasmask_Arius_HMG_H__

#include "Monster.h"

class ENGINE_DLL CGasmask_Arius_HMG final : public CMonster
{
protected:
	explicit CGasmask_Arius_HMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CGasmask_Arius_HMG(const CGasmask_Arius_HMG& rhs);
	virtual ~CGasmask_Arius_HMG();
public:
	static CGasmask_Arius_HMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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