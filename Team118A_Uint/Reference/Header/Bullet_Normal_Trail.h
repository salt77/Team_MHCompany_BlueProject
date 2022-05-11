#pragma once

#include "AttackObj.h"

BEGIN(Engine)


class ENGINE_DLL CBullet_Normal_Trail final : public CAttackObj
{
public:
	explicit CBullet_Normal_Trail(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CBullet_Normal_Trail(const CBullet_Normal_Trail& rhs);
	virtual ~CBullet_Normal_Trail();

public:
	static	CBullet_Normal_Trail*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _vec3& vPos, const _vec3& vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType);
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

};

END