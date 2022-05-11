#pragma once
#ifndef __SCENE_RUNTEST_H__
#define __SCENE_RUNTEST_H__

#include "Scene.h"

class CClientScene_Run final : public CScene
{
private:
	CClientScene_Run(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Run();

public:
	static CClientScene_Run*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free() override;

public:
	virtual HRESULT				Ready_Scene() override;
	virtual	_int				Start_Scene(const _float& fTimeDelta) override;
	virtual _int				Update_Scene(const _float& fTimeDelta);
	virtual _int				LateUpdate_Scene(const _float& fTimeDelta);
	virtual void				Render_Scene() override;

protected:
	HRESULT						Ready_Prototype();
	HRESULT						Ready_LightInfo();
	HRESULT						Ready_SceneObject();

private:
	void						End_Battle(E_SCENETAG eTag);
};

#endif // !__SCENE_RUNTEST_H__