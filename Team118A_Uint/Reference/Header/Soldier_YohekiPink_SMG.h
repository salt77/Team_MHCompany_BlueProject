#pragma once
#ifndef __Soldier_YohekiPink_SMG_H__
#define __Soldier_YohekiPink_SMG_H__

#include "Monster.h"

class ENGINE_DLL CSoldier_YohekiPink_SMG final : public CMonster
{
protected:
	explicit CSoldier_YohekiPink_SMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSoldier_YohekiPink_SMG(const CSoldier_YohekiPink_SMG& rhs);
	virtual ~CSoldier_YohekiPink_SMG();
public:
	static CSoldier_YohekiPink_SMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__Soldier_YohekiPink_SMG_H__