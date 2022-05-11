#pragma once

#ifndef __SHIRO_BOMB_S_H__
#define __SHIRO_BOMB_S_H__

#include "AttackObj.h" 

BEGIN(Engine)

class CSphereCollider; 

class ENGINE_DLL CShiro_Bomb_S final : public CAttackObj
{
private:
	explicit CShiro_Bomb_S(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	explicit CShiro_Bomb_S(const CShiro_Bomb_S& rhs);
	virtual ~CShiro_Bomb_S();

public:
	static	CShiro_Bomb_S*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vStartPos, const _vec3& vTargetPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _bool bNormalBomb);
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual	HRESULT		Ready_Object(const _vec3& vStartPos, const _vec3& vTargetPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _bool bNormalBomb);
	virtual _int		Start_Object(const _float& fTimeDelta);
	virtual	_int		Update_Object(const _float& fTimeDelta);
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta);

	virtual	void		Render_Object(_int iPass = 0);

	virtual void		Debug_This_In_Console();

	virtual void		Set_Constantable();

private:
	void				Add_Component(); 

	void				Movement(const _float& fTimeDelta); 
	void				Movement_Ex(const _float& fTimeDelta);
	void				Bomb(); 

public:
	virtual void		OnEnable();
	virtual void		OnDisable();
	virtual void		OnDestroy();
	virtual void		On_Collision(CGameObject* pDest);

private:
	_bool				m_bNormalBomb = TRUE; 

	_float				m_fFallSpeed = 15.f;
	_float				m_fTime = 0.f;
	_float				m_fDistance = 0.f; 
	_float				m_fArriveTime = 1.f; 

	_vec3				m_vStartPos = VEC_ZERO;
	_vec3				m_vTargetPos = VEC_ZERO; 
};

END

#endif //__SHIRO_BOMB_S_H__