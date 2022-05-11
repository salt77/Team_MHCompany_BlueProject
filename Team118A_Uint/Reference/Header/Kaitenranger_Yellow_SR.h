#pragma once
#ifndef __Kaitenranger_Yellow_SR_H__
#define __Kaitenranger_Yellow_SR_H__

#include "Monster.h"

class ENGINE_DLL CKaitenranger_Yellow_SR final : public CMonster
{
protected:
	explicit CKaitenranger_Yellow_SR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CKaitenranger_Yellow_SR(const CKaitenranger_Yellow_SR& rhs);
	virtual ~CKaitenranger_Yellow_SR();
public:
	static CKaitenranger_Yellow_SR*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	virtual void	Trigger_Reload() override; // ���ε�

	virtual void	Trigger_1() override; // ��ų ����
	virtual void	Trigger_2() override; // ��ų_����
	virtual void	Trigger_3() override; // ��ų_����_Ư��ȿ��ȿ��
	virtual void	Trigger_4() override; // ��ų_����

	virtual void	Create_HitEffect() override;

private:
};

#endif