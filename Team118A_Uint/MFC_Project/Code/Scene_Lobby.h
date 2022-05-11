#pragma once
#ifndef __SCENE_LOBBY_H__
#define __SCENE_LOBBY_H__

#include "Scene.h"

class CScene_Lobby final: public CScene
{
private:
	CScene_Lobby(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Lobby();

public:
	static CScene_Lobby*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;

public:
	void Set_ObstacleMove(_bool pObstacleMove) { m_bObstacleMove = pObstacleMove; }

public:
	virtual HRESULT	Ready_Scene() override;
	virtual	_int	Start_Scene(const _float& fTimeDelta) override;
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene() override;

public:
	_vec3			TriRaycast(HWND hWnd, CSMesh* pMapBufferCom, CTransform* pMapTransCom, _float* pOutDistance, _bool* bCol);


private:
	void			PickingObstacle();
	void			MoveObstacle();
	void			RotateFurniture();

protected:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_LightInfo();
	HRESULT			Ready_SceneObject();


private:
	_bool				m_bObstacleMove = false;
	CGameObject*		m_pSelObstacle = nullptr;

	_float				m_fRot = 0.f;
};

#endif // !__SCENE_LOBBY_H__