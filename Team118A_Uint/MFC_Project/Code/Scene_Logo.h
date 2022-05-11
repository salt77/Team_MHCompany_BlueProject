#pragma once
#ifndef __Logo_H__
#define __Logo_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Scene.h"

USING(Engine)

class CScene_Logo final : public CScene
{
private:
	CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Logo();

public:
	static CScene_Logo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free() override;

public:
	virtual HRESULT	Ready_Scene() override;
	virtual	_int	Start_Scene(const _float& fTimeDelta) override; 
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene() override;

protected:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_LightInfo();
	HRESULT			Ready_SceneObject();

	static _bool	bUniquePrototype;
};


#endif // __Logo_H__
