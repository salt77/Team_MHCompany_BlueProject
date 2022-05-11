#pragma once
#ifndef __SchoolGirl02_SukebanSwim_SR_H__
#define __SchoolGirl02_SukebanSwim_SR_H__

#include "Monster.h"

class ENGINE_DLL CSchoolGirl02_SukebanSwim_SR final : public CMonster
{
protected:
	explicit CSchoolGirl02_SukebanSwim_SR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSchoolGirl02_SukebanSwim_SR(const CSchoolGirl02_SukebanSwim_SR& rhs);
	virtual ~CSchoolGirl02_SukebanSwim_SR();
public:
	static CSchoolGirl02_SukebanSwim_SR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

#endif //__SchoolGirl02_SukebanSwim_SR_H__