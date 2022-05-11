#pragma once
#ifndef __LightTank_Helmet_Vulcan_H__
#define __LightTank_Helmet_Vulcan_H__

#include "Monster.h"

class ENGINE_DLL CLightTank_Helmet_Vulcan final : public CMonster
{
protected:
	explicit CLightTank_Helmet_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CLightTank_Helmet_Vulcan(const CLightTank_Helmet_Vulcan& rhs);
	virtual ~CLightTank_Helmet_Vulcan();
public:
	static CLightTank_Helmet_Vulcan*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void	Trigger_Attack_1() override; 
	virtual void	Trigger_1() override; 
	virtual void	Trigger_7() override; 
	virtual void	Trigger_9() override; 
	virtual void	Trigger_Dead() override; 

private:
	char*				m_szBoneFlashBang = "Bone_FlashBang"; 

	_vec3				m_vTargetPos = VEC_ZERO; 

	CSphereCollider*	m_pFlashBangCol = nullptr; 
};

#endif //__LightTank_Helmet_Vulcan_H__