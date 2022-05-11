#pragma once

#ifndef __ATTACKOBJ_H__
#define __ATTACKOBJ_H__

#include "GameObject.h"
#include "SphereCollider.h"
#include "Renderer.h"
#include "EffectObject.h"
#include "ParticleObject.h"
#include "Trail_Fix.h"
#include "ProtoMgr.h"
#include "Scene.h"
#include "Wall512.h"

BEGIN(Engine)

class CEffectObject;
class CParticleObject;
class CTrail_Fix;
class CSkillObject;

class ENGINE_DLL CAttackObj abstract : public CGameObject
{
public:
	explicit CAttackObj(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CAttackObj(const CAttackObj& rhs);
	virtual ~CAttackObj();

public:
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual	HRESULT			Ready_Object() override;
	virtual _int			Start_Object(const _float& fTimeDelta);
	virtual	_int			Update_Object(const _float& fTimeDelta);
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta);
	virtual	void			Render_Object(_int iPass = 0);

	virtual void			Debug_This_In_Console();

	virtual void			Set_Constantable();

public:
	void					Set_Speed(const _float& fSpeed)			{ m_fSpeed = fSpeed;										}
	void					Set_LifeTime(const _float& fLifeTime)	{ m_fTotalLifeTime = fLifeTime;	m_fNowLifeTime = fLifeTime; }
	void					Set_DelayTime(const _float& fDelayTime) { m_fDelayTime = fDelayTime;								}
	void					Set_AfterSkillName(wstring wstrSkillName) { m_wstrAfterSkillName = wstrSkillName;					}	

public:
	const T_UnitInfo&		Get_UnitInfo()							{ return m_tUnitInfo;										}
	const E_CHAR_ATKTYPE&	Get_AtkType()							{ return m_eAtkType;										}
	const _float			Get_LifeRatio()							{ return m_fNowLifeTime / m_fTotalLifeTime;					}
	const _bool				Get_IsDelayed()							{ return (m_fDelayTime > 0.f);								}
	const _float&			Get_DelayTime()							{ return m_fDelayTime;										}

public:
	virtual void			On_Collision(CGameObject* pDest) override;


protected:
	E_CHAR_ATKTYPE		m_eAtkType					= E_CHAR_ATKTYPE::ATKTYPE_END;
	E_LAYERTAG			m_eTargetLayerTag			= E_LAYERTAG::TAG_END; 

	T_UnitInfo			m_tUnitInfo; 

	LPDIRECT3DTEXTURE9  m_pTexture					= nullptr;
	CSphereCollider*	m_pSphereColliderCom		= nullptr;
	CShader*			m_pColliderShaderCom		= nullptr;
	CEffectObject*		m_pEffectObject				= nullptr;
	CParticleObject*	m_pParticle					= nullptr;
	CShader*			m_pShaderCom				= nullptr;
	CSMesh*				m_pSMeshCom					= nullptr;
	CTrail_Fix*			m_pTrail					= nullptr;
	CSkillObject*		m_pSkillObject				= nullptr;

	_vec3				m_vDir						= VEC_ZERO;
	_float				m_fCountingTime				= 0.f;
	_float				m_fNowLifeTime				= 5.f;
	_float				m_fTotalLifeTime			= 5.f;
	_float				m_fSpeed					= 7.f;
	_float				m_fDelayTime				= 0.f;
	_float				m_fDamageStandard			= 5.f; 
	_float				m_fDamageRandRange			= 3.f; 
	_bool				m_bColUseY					= FALSE;
	wstring				m_wstrAfterSkillName		= L"Empty";
};

END

#endif //__ATTACKOBJ_H__