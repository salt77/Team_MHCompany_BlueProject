#pragma once

#include "GameObject.h"

USING(Engine) 

class CBullet final : public CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	static	CBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vDir, E_OBJTAG eObjTag);
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void		Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT		Ready_Object(const _vec3& vPos, const _vec3& vDir);
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;

private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

	void				Movement(const _float& fTimeDelta); 

private:
	_float				m_fCountingTime = 0.f;
	_float				m_fSpeed = 100.f;

	_vec3				m_vDir = VEC_ZERO; 

	LPDIRECT3DTEXTURE9  m_pTexture = nullptr;
	CSphereCollider*	m_pSphereColliderCom = nullptr;

	CRectBuffer*		m_pBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CShader*			m_pColliderShaderCom = nullptr;
};
