#pragma once

#include "AttackObj.h"

#include "SphereCollider.h"
#include "RectBuffer.h"

BEGIN(Engine)

class CEffectObject;
class CParticleObject;

class ENGINE_DLL CBullet_Bezier final : public CAttackObj
{
public:
	explicit CBullet_Bezier(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CBullet_Bezier(const CBullet_Bezier& rhs);
	virtual ~CBullet_Bezier();

public:
	static	CBullet_Bezier*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override; 


public:
	virtual void			On_Collision(CGameObject* pDest);

public:
	void					Set_Bezier(const _float& fTotalTime, const _vec3& P1, const _vec3& P2, const _vec3& P3, const _vec3& P4);

public:
	const _float&			Get_TotalTime()			{	return m_fTotalTime;	}


public:
	static	list<CBullet_Bezier*>*		Create_Bezier(_int iCount, _int iCountEveryInterval, 
		const _float & fTotalTimeMin, const _float & fTotalTimeMax, const _float& fDelayMin, const _float& fDelayMax,
		const _vec3& vStart, const _vec3& vEnd, const _vec3& vStartRand, const _vec3& vEndRand,
		LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag);

private:
	void					Add_Component();
	virtual void			Set_Constantable() override;

	void					Movement(const _float& fTimeDelta);

private:
	_vec3					m_vBezier[4];
	_float					m_fTotalTime	= 1.f;

};

END