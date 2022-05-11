#pragma once
#ifndef __Yukata_StreetGhostes_SG_H__
#define __Yukata_StreetGhostes_SG_H__

#include "Monster.h"

class ENGINE_DLL CYukata_StreetGhostes_SG final : public CMonster
{
protected:
	explicit CYukata_StreetGhostes_SG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CYukata_StreetGhostes_SG(const CYukata_StreetGhostes_SG& rhs);
	virtual ~CYukata_StreetGhostes_SG();
public:
	static CYukata_StreetGhostes_SG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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