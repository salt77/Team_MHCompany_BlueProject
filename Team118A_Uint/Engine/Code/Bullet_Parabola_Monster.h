#pragma once

#include "AttackObj.h"

BEGIN(Engine)

class ENGINE_DLL CBullet_Parabola_Monster final : public CAttackObj
{
public:
	enum E_ParabolaType
	{
		Parabola_FlashBang, 
		Parabola_End 
	};

private:
	explicit CBullet_Parabola_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CBullet_Parabola_Monster(const CBullet_Parabola_Monster& rhs);
	virtual ~CBullet_Parabola_Monster();

public:
	static	CBullet_Parabola_Monster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(_vec3 vPos, _vec3 vTargetPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType);
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
	_bool				m_bCollisionLand = FALSE; 

	// 포물선 변수 
	_float				m_fFallSpeed = 15.f;
	_float				m_fTime = 0.f;
	_float				m_fDistance = 0.f;
	_float				m_fArriveTime = 1.f;

	_vec3				m_vStartPos = VEC_ZERO;
	_vec3				m_vTargetPos = VEC_ZERO; 
};

END