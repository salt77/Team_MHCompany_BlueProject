#pragma once
#ifndef __Kaitenranger_Green_HMG_H__
#define __Kaitenranger_Green_HMG_H__

#include "Monster.h"

class ENGINE_DLL CKaitenranger_Green_HMG final : public CMonster
{
protected:
	explicit CKaitenranger_Green_HMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CKaitenranger_Green_HMG(const CKaitenranger_Green_HMG& rhs);
	virtual ~CKaitenranger_Green_HMG();
public:
	static CKaitenranger_Green_HMG*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	void			Hit_Shot(); // ��ų�� ���� ������

	virtual void	Create_HitEffect() override;

private:
};

#endif