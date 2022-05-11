#pragma once

#ifndef __CHESED_WEB_LASER_H__
#define __CHESED_WEB_LASER_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Chesed;
class CRun_Map; 
class CRun_Player; 
class CParticleObject; 

class ENGINE_DLL Chesed_Web_Laser final : public CAttackObj
{
public:
	enum E_WebLaser_AtkType
	{
		WebLaser_Normal, WebLaser_Rotate, WebLaser_Cross, 
		WebLaser_End
	};

private:
	explicit Chesed_Web_Laser(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit Chesed_Web_Laser(const Chesed_Web_Laser& rhs);
	virtual ~Chesed_Web_Laser();

public:
	static	Chesed_Web_Laser*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const E_WebLaser_AtkType& eLaserType, E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const E_WebLaser_AtkType& eLaserType);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override;

public:
	virtual void			On_Collision(CGameObject* pDest) override;

public:
	static void				Collision_Raycast(CGameObject * pThis, CGameObject * pPlayer);

private:
	virtual void			Set_Constantable() override; 
	virtual void			Set_Constantable_Fog(CShader* pShader) override; 

private:
	void					Add_Component(); 
	void					Ready_Attack(const _float& fTimeDelta); 
	void					Check_Dead();

private:
	_bool					m_bHitPlayer = FALSE; 

	_uint					m_iRotate = 0; 
	_uint					m_iReverse = 0; 

	_float					m_fReadyTime = 2.5f; 
	_float					m_fRunAngle = 0.f; 
	_float					m_fSpawnInterpolAngle = -5.f; 
	_float					m_fPipeAngle = 0.f; 
	_float					m_fPipeOriginAngle = 0.f; 
	_float					m_fDistanceFromPipeMidLook = 0.f; 
	_float					m_fReverseLaserArriveDelay = 1.5f; 

	// Other ReadyTime 
	_float					m_fOtherReadyTime = 1.5f; 
	_float					m_fOtherMoveSpeed = 35.f; 

	// Cross Rotate Speed 
	_float					m_fCrossRotateSpeed = 1.5f; 

	_vec3					m_vAxisZ = VEC_ZERO; 
	_vec3					m_vPipeRotateDir = VEC_ZERO; 
	_vec3					m_vRayStartPos = VEC_ZERO; 
	_vec3					m_vRayDir = VEC_ZERO; 

	E_WebLaser_AtkType		m_eWebLaserType = WebLaser_End; 

	CSphereCollider*		m_pSphereColliderCom_2 = nullptr; 
	CParticleObject*		m_pParticleWebLaser = nullptr; 
	CParticleObject*		m_pSparkParticle_1 = nullptr; 
	CParticleObject*		m_pSparkParticle_2 = nullptr;
	CRun_Chesed*			m_pChesed = nullptr;
	CRun_Map*				m_pRunMap = nullptr; 
	CRun_Player*			m_pPlayer = nullptr; 
};

END

#endif //__CHESED_WEB_LASER_H__