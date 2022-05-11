#pragma once
#ifndef __Logo_H__
#define __Logo_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Scene.h"

USING(Engine)

class CClientScene_Logo final : public CScene
{
private:
	CClientScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Logo();

public:
	static CClientScene_Logo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;

public:
	virtual HRESULT	Ready_Scene() override;
	virtual _int	Start_Scene(const _float& fTimeDelta) override; 
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene() override;

protected:
	virtual HRESULT	Ready_SceneObject() override;

private:
	HRESULT			Ready_Prototype();

	static _bool	bUniquePrototype;
};


#endif // __Logo_H__
