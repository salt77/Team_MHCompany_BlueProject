#pragma once

#ifndef __BULLET_DEAD_H__
#define __BULLET_DEAD_H__

#include "GameObject.h"

BEGIN(Engine)

class CEffectObject; 
class CShader; 

class ENGINE_DLL CBullet_Dead final : public CGameObject
{
private:
	explicit CBullet_Dead(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CBullet_Dead(const CBullet_Dead& rhs);
	virtual ~CBullet_Dead();

public:
	static	CBullet_Dead*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const E_RUN_BULLETTYPE& eBulletType = E_RUN_BULLETTYPE::BULLET_RED);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const E_RUN_BULLETTYPE& eBulletType);
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

	E_RUN_BULLETTYPE		m_eBulletType = E_RUN_BULLETTYPE::BULLET_END; 

	CShader*				m_pShaderCom = nullptr; 
};

END

#endif //__BULLET_DEAD_H__