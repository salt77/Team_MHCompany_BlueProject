#pragma once
#ifndef __BoxCat__
#define __BoxCat__

#include "Monster.h"

class ENGINE_DLL CBoxCat final : public CMonster
{
protected:
	explicit CBoxCat(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CBoxCat(const CBoxCat& rhs);
	virtual ~CBoxCat();
public:
	static CBoxCat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void	Free() override;

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
};

#endif //__BoxCat__