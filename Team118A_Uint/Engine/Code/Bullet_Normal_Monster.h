#pragma once

#include "AttackObj.h"

BEGIN(Engine)

class ENGINE_DLL CBullet_Normal_Monster final : public CAttackObj
{
public:
	enum E_MovementType
	{
		Move_FallDown, Move_TankBullet, Move_RocketLauncher, Move_HeliMissile, Move_AutomataBullet, Move_GoliathEx, 
		Move_Normal,  
		Move_End
	};

private:
	explicit CBullet_Normal_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CBullet_Normal_Monster(const CBullet_Normal_Monster& rhs);
	virtual ~CBullet_Normal_Monster();

public:
	static	CBullet_Normal_Monster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType, E_MovementType eMoveType = Move_Normal);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(_vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType, E_MovementType eMoveType);
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
	_bool					m_bCollisionLand = FALSE; 

	E_MovementType			m_eMovementType = Move_Normal; 

	_float					m_fAttackRange = 2.5f; 

	_vec3					m_vStartPos = VEC_ZERO;
};

END