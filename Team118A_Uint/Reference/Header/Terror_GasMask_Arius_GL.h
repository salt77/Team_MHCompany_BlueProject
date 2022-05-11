#pragma once
#ifndef __Terror_GasMask_Arius_GL_H__
#define __Terror_GasMask_Arius_GL_H__

#include "Monster.h"

class ENGINE_DLL CTerror_GasMask_Arius_GL final : public CMonster
{
protected:
	explicit CTerror_GasMask_Arius_GL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTerror_GasMask_Arius_GL(const CTerror_GasMask_Arius_GL& rhs);
	virtual ~CTerror_GasMask_Arius_GL();
public:
	static CTerror_GasMask_Arius_GL*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

private:
};

#endif