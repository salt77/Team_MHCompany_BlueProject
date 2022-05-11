#pragma once
#ifndef __BINAH_TAIL_H__
#define __BINAH_TAIL_H__

#include "BossMonster.h"

class ENGINE_DLL CBinah_Tail final : public CMonster
{
protected:
	explicit		CBinah_Tail(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit		CBinah_Tail(const CBinah_Tail& rhs);
	virtual			~CBinah_Tail();

public:
	static CBinah_Tail*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__BINAH_TAIL_H__