#pragma once
#ifndef __SCENE_RUNTEST_H__
#define __SCENE_RUNTEST_H__

#include "Scene.h"

class CScene_RunTest final: public CScene
{
private:
	CScene_RunTest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_RunTest();

public:
	static CScene_RunTest*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
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