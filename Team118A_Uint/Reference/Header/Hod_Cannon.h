#pragma once
#ifndef __HOD_CANNON_H__
#define __HOD_CANNON_H__

#include "BossMonster.h"

class ENGINE_DLL CHod_Cannon final : public CMonster
{
protected:
	explicit		CHod_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit		CHod_Cannon(const CHod_Cannon& rhs);
	virtual			~CHod_Cannon();

public:
	static CHod_Cannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void	Free() override;

public:
	virtual	HRESULT	Ready_Object() override;
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void	Render_Object(_int iPass) override;

	virtual void	Set_Constantable() override;
	virtual void	Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void	Idle(_float fDeltaTime);

public:
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;
	virtual void	OnDestroy() override;

private:
	_float			m_fGage = 0.f;
	E_ACTION		m_eMainAction = E_ACTION::ACTION_END;

};

#endif //__HOD_CANNON_H__