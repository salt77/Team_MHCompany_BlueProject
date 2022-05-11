#pragma once
#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Component.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CCollider abstract:
	public CComponent
{
protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	CCollider(const CCollider& rhs);
	virtual ~CCollider();
public:
	virtual void					Free()override;
	virtual CComponent*				Clone()PURE;

	HRESULT							Ready_Collider();
	virtual void					Render_Collider(LPD3DXEFFECT pEffect);

	void							ClearColList();
	void							AddColList(const CGameObject* pColObj); 
	
	virtual void					SetScale(_vec3 vScale);
	virtual void					SetMatrix(_matrix matWorld);
	
	_vec3							GetPos();
	_vec3							GetRot();
	_vec3							GetLook();
	_vec3							GetRight();
	_vec3							GetUp();
	CGameObject*					GetParent();
	E_COLTAG						GetColliderTag();
	const _matrix&					Get_WorldMatrix() { return m_matColMatrix; }

	_bool							ColListCheck(const CGameObject* pColObj);
	
protected:
	CGameObject*					m_pParentObject = nullptr;
	_matrix							m_matColMatrix;
	_bool							m_bCol = FALSE;
	E_COLTAG						m_eColliderTag = E_COLTAG::COL_END;
	list<const CGameObject*>		m_listColObj;
	_vec3							m_vScale = _vec3(1.f, 1.f, 1.f);
	LPDIRECT3DTEXTURE9				m_pTexture[2];
};

END

#endif