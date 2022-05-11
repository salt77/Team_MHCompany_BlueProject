#pragma once
#ifndef __SHIRO_H__
#define __SHIRO_H__

#include "BossMonster.h"

class ENGINE_DLL CShiro final : public CBossMonster
{
protected:
	explicit CShiro(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CShiro(const CShiro& rhs);
	virtual ~CShiro();
public:
	static CShiro*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free() override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;

	virtual void		Set_Constantable() override;

public:
	virtual void		OnEnable() override;
	virtual void		OnDisable() override;
	virtual void		OnDestroy() override;

protected:
	virtual void		Animation_Change(const _float& fTimeDelta);

	virtual void		Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void		Begin_Battle(_float fDeltaTime);
	virtual void		Idle(_float fDeltaTime);
	virtual void		Shoot_Start(_float fDeltaTime);
	virtual void		Shoot(_float fDeltaTime);
	virtual void		Shoot_End(_float fDeltaTime);
	virtual void		Dead(_float fDeltaTime);
	virtual void		ExSkill(_float fDeltaTime);
	virtual void		Boss_Attack_1(_float fDeltaTime);
	void				Move_Left(const _float& fTimeDelta); 
	void				Move_Right(const _float& fTimeDelta); 

protected:
	void				Trigger_Attack_1() override;
	void				Trigger_ExSkill() override; 
	void				Trigger_1() override;				// 2번째 ExSkill 
	void				Trigger_2() override;				// 1번째 ExSkill 위에서 떨어지는 타이밍 

private:
	_float				m_fCountingTime = 0.f;
	_float				m_fExSkill_Cool = 10.f; 
};

#endif //__SHIRO_H__