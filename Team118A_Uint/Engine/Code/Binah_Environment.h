#pragma once
#ifndef __BINAH_ENVIRONMENT_H__
#define __BINAH_ENVIRONMENT_H__

#include "GameObject.h"
#include "Binah.h"

BEGIN(Engine)

class ENGINE_DLL CBinah_Environment final : public CGameObject
{
private:
	explicit				CBinah_Environment(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual					~CBinah_Environment();

public:
	static	CBinah_Environment*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, const _vec3 vStartPos, const _vec3& vDir);
	virtual void			Free() override;

public:
	virtual HRESULT			Ready_Object(const _vec3 vStartPos, const _vec3& vDir);
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass) override;

	virtual void			Set_Constantable();

private:
	void					Add_Component();
	void					Movement(const _float& fTimeDelta);

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();

private:
	CSMesh*					m_pSMeshCom		= nullptr;
	CShader*				m_pShaderCom	= nullptr;
	CBinah*					m_pBinah		= nullptr;

private:
	_vec3					m_vStartPos		= VEC_ZERO;
	_vec3					m_vDir			= VEC_ZERO;
	_float					m_fSpeed		= 0.f;
};

END
#endif //__BINAH_ENVIRONMENT_H__