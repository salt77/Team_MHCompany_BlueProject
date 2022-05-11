#pragma once
#ifndef __EnemyTSS_Crusader_PMC_H__
#define __EnemyTSS_Crusader_PMC_H__

#include "Monster.h"

class ENGINE_DLL CEnemyTSS_Crusader_PMC final : public CMonster
{
protected:
	explicit CEnemyTSS_Crusader_PMC(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CEnemyTSS_Crusader_PMC(const CEnemyTSS_Crusader_PMC& rhs);
	virtual ~CEnemyTSS_Crusader_PMC();
public:
	static CEnemyTSS_Crusader_PMC*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_1() override; 
	void			Trigger_2() override; 
	void			Trigger_3() override; 
	void			Trigger_ExSkill() override; 

private:
	void			Create_FireEffect(); 
	void			Create_ChargeEffect(const _float& fRatio); 
};

#endif