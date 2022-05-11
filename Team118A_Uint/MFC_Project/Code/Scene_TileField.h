#pragma once
#ifndef __SCENE_TILE_FIELD_H__
#define __SCENE_TILE_FIELD_H__

#include "Scene.h"

class CTileField;
class CScene_TileField final: public CScene
{
private:
	CScene_TileField(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_TileField();

public:
	static CScene_TileField*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;

public:

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


private:
	CTileField*		m_pTileField = nullptr;

};

#endif // !__SCENE_LOBBY_H__