#pragma once
#ifndef __Kaitenranger_Red_AR_H__
#define __Kaitenranger_Red_AR_H__

#include "Monster.h"

class ENGINE_DLL CKaitenranger_Red_AR final : public CMonster
{
protected:
	explicit CKaitenranger_Red_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CKaitenranger_Red_AR(const CKaitenranger_Red_AR& rhs);
	virtual ~CKaitenranger_Red_AR();
public:
	static CKaitenranger_Red_AR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	virtual void	Trigger_Reload() override; // 리로드


	virtual void	Trigger_1() override; // 스킬 공격
	virtual void	Trigger_2() override; // 스킬_시작
	virtual void	Trigger_3() override; // 스킬_시작_특수효과효과
	virtual void	Trigger_4() override; // 스킬_공격 전 바람

	virtual void	Create_HitEffect() override;

private:
};

#endif