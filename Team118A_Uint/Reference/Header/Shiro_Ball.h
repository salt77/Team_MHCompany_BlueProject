#pragma once

#ifndef __SHIRO_BALL_H__
#define __SHIRO_BALL_H__

#include "AttackObj.h" 

BEGIN(Engine)

class CSphereCollider; 

class ENGINE_DLL CShiro_Ball final : public CAttackObj
{
private:
	explicit CShiro_Ball(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	explicit CShiro_Ball(const CShiro_Ball& rhs);
	virtual ~CShiro_Ball();

public:
	static	CShiro_Ball*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual	HRESULT		Ready_Object(const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType);
	virtual _int		Start_Object(const _float& fTimeDelta);
	virtual	_int		Update_Object(const _float& fTimeDelta);
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta);
	virtual	void		Render_Object(_int iPass = 0);

	virtual void		Debug_This_In_Console();

	virtual void		Set_Constantable();

private:
	void				Add_Component(); 

	void				Movement(const _float& fTimeDelta); 

public:
	virtual void		OnEnable();
	virtual void		OnDisable();
	virtual void		OnDestroy();
	virtual void		On_Collision(CGameObject* pDest);

private:
	_bool				m_bLanding = FALSE; 

	_float				m_fFallSpeed = 0.2f; 
	_float				m_fTime = 0.f;

	_vec3				m_vStartPos = VEC_ZERO;
};

END

#endif //__SHIRO_BALL_H__