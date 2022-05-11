#pragma once

#include "AttackObj.h"

BEGIN(Engine)

class ENGINE_DLL CTank_Bullet_Dead final : public CAttackObj
{
public:
	explicit CTank_Projectile(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CTank_Projectile(const CTank_Projectile& rhs);
	virtual ~CTank_Projectile();

public:
	static	CTank_Projectile*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								   _vec3 vPos, 
								   _vec3 vDir, 
								   const T_UnitInfo& tUnitInfo, 
								   E_CHAR_ATKTYPE eAtkType, 
								   E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(_vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override; 

public:
	virtual void			On_Collision(CGameObject* pDest);

private:
	void					Add_Component();
	virtual void			Set_Constantable() override;

	void					Movement(const _float& fTimeDelta);

private:
	CEffectObject*			m_pEffectSmoke = nullptr; 
	CEffectObject*			m_pEffectWave = nullptr; 
};

END