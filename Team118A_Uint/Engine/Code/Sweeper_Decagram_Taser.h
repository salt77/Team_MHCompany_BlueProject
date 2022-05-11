#pragma once
#ifndef __Sweeper_Decagram_Taser_H__
#define __Sweeper_Decagram_Taser_H__

#include "Monster.h"

class ENGINE_DLL CSweeper_Decagram_Taser final : public CMonster
{
protected:
	explicit CSweeper_Decagram_Taser(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSweeper_Decagram_Taser(const CSweeper_Decagram_Taser& rhs);
	virtual ~CSweeper_Decagram_Taser();
public:
	static CSweeper_Decagram_Taser*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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