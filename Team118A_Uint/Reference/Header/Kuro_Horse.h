#pragma once

#ifndef __KURO_HORSE_H__
#define __KURO_HORSE_H__

#include "AttackObj.h" 

BEGIN(Engine)

class CSphereCollider; 

class ENGINE_DLL CKuro_Horse final : public CAttackObj
{
private:
	explicit CKuro_Horse(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	explicit CKuro_Horse(const CKuro_Horse& rhs);
	virtual ~CKuro_Horse();

public:
	static	CKuro_Horse*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vCenter, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _float fDelayTime);
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual	HRESULT		Ready_Object(const _vec3& vCenter, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _float fDelayTime);
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
	_float				m_fStartDelayTime = 0.f;
	_float				m_fStartY = 10.f;
	_vec3				m_vCenter;
	_float				m_fDistance;
	_float				m_fStartAngle;
	_float				m_fSinValue = 0.f;

	_bool				m_bLanding = FALSE; 

	_float				m_fTime = 0.f;

	_vec3				m_vStartPos = VEC_ZERO;
};

END

#endif //__SHIRO_BALL_H__