#pragma once
#ifndef __SchoolGirl03_Sukeban_HMG_H__
#define __SchoolGirl03_Sukeban_HMG_H__

#include "Monster.h"

class ENGINE_DLL CSchoolGirl03_Sukeban_HMG final : public CMonster
{
protected:
	explicit CSchoolGirl03_Sukeban_HMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSchoolGirl03_Sukeban_HMG(const CSchoolGirl03_Sukeban_HMG& rhs);
	virtual ~CSchoolGirl03_Sukeban_HMG();
public:
	static CSchoolGirl03_Sukeban_HMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__SchoolGirl03_Sukeban_HMG_H__