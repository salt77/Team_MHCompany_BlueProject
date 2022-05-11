#pragma once

#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__

#include "Collider.h"

BEGIN(Engine)

// 민현 To 민우 - 전달사항
// 더러움 코드 정리할 것
// 함수, 변수 순서 뒤죽박죽 고쳐놓을 것

class ENGINE_DLL CSphereCollider final:
	public CCollider
{
public:
	CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	CSphereCollider(const CSphereCollider& rhs);
	virtual ~CSphereCollider();
public:
	virtual void				Free()override;
	virtual CComponent*			Clone()override;
	static CSphereCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent);
	HRESULT						Ready_Sphere_Collider(CGameObject* pParent);
	virtual void				Render_Collider(LPD3DXEFFECT pEffect)override;

	virtual void				SetScale(_vec3 vScale);
	virtual void				SetMatrix(_matrix matWorld)override;
	void						SetRadius(_float fRad);

	_float						GetRadius();

	const LPD3DXMESH&			Get_MeshCollider() { return m_pMesh; }

	_bool						PickingCollider(HWND hWnd,CGameObject** ppObjOut);
	_bool						PickingCollider(_vec3 vWorldRayStartPos, _vec3 vWorldRayDir, _float fRayDistance, CGameObject** ppObjOut);
private:
	_float	m_fRadius = 0.f;
	LPD3DXMESH m_pMesh = nullptr;

};

END

#endif