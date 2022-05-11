#pragma once
#ifndef __Cook_GenbuStore_AR_H__
#define __Cook_GenbuStore_AR_H__

#include "Monster.h"

class ENGINE_DLL CCook_GenbuStore_AR final : public CMonster
{
protected:
	explicit CCook_GenbuStore_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CCook_GenbuStore_AR(const CCook_GenbuStore_AR& rhs);
	virtual ~CCook_GenbuStore_AR();
public:
	static CCook_GenbuStore_AR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void				Free() override;

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

#endif //__Cook_GenbuStore_AR_H__