#pragma once
#ifndef __HeavyTank_Decagram_Cannon_H__
#define __HeavyTank_Decagram_Cannon_H__

#include "Monster.h"

BEGIN(Engine)

class CEffectObject; 

class ENGINE_DLL CHeavyTank_Decagram_Cannon final : public CMonster
{
protected:
	explicit CHeavyTank_Decagram_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CHeavyTank_Decagram_Cannon(const CHeavyTank_Decagram_Cannon& rhs);
	virtual ~CHeavyTank_Decagram_Cannon();
public:
	static CHeavyTank_Decagram_Cannon*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void	Trigger_Attack_1() override;
	virtual void	Trigger_1() override; 
	virtual void	Trigger_2() override; 
	virtual void	Trigger_7() override; 
	virtual void	Trigger_Dead() override; 
	virtual void	Trigger_ExSkill() override; 

private:
	void			Create_FireEffect(); 
	void			Create_Bomb(const _float& fTimeDelta); 

private:
	_bool			m_bCreateBomb = FALSE; 

	_float			m_fSkillArriveOriginTime = 1.5f; 
	_float			m_fSkillArriveTime = m_fSkillArriveOriginTime;

	_vec3			m_vTargetPos = VEC_ZERO; 
};

END

#endif