#pragma once

#ifndef __TANMAK_EFFECT_H__
#define __TANMAK_EFFECT_H__

#include "GameObject.h"

BEGIN(Engine)

class CEffectObject; 

class ENGINE_DLL CTanmak_Effect final : public CGameObject
{
private:
	explicit CTanmak_Effect(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CTanmak_Effect(const CTanmak_Effect& rhs);
	virtual ~CTanmak_Effect();

public:
	static	CTanmak_Effect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object();
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override;

private:
	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

private:
	void					Add_Component();

private:
	CEffectObject*			m_pEffectObject = nullptr; 

	CShader*				m_pShaderCom = nullptr;
};

END

#endif //__TANMAK_EFFECT_H__