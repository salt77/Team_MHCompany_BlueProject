#pragma once

#ifndef __BULLET_RUN_NORMAL_H__
#define __BULLET_RUN_NORMAL_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Map; 
class CRun_Chesed; 
class CRun_Player; 
class CParticleObject; 

class ENGINE_DLL CBullet_Run_Normal final : public CAttackObj
{
public:
	explicit CBullet_Run_Normal(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CBullet_Run_Normal(const CBullet_Run_Normal& rhs);
	virtual ~CBullet_Run_Normal();

public:
	static	CBullet_Run_Normal*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									   const _float& fSpeed, 
									   const _float& fRunAngle, 
									   const _vec3& vMidToFireDir,
									   const _float& fOriToMidDis, 
									   const _bool& bIsBossExist = FALSE, 
									   E_LAYERTAG eTargetLayerTag = E_LAYERTAG::Monster);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _float& fSpeed, 
										 const _float& fRunAngle, 
										 const _vec3& vMidToFireDir, 
										 const _float& fOriToMidDis, 
										 const _bool& bIsBossExist);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override; 

public:
	virtual void			On_Collision(CGameObject* pDest);

public:
	const _int&				Get_Damage() { return m_iDamage; }

private:
	void					Add_Component();
	virtual void			Set_Constantable() override;

	void					Movement(const _float& fTimeDelta); 
	void					Dead_Bullet(); 

private:
	_bool					m_bBossTarget = FALSE; 

	_int					m_iDamage = 1000;
	_int					m_iDamageRandRange = 500;

	_float					m_fSpeed = 0.f; 
	_float					m_fSpeedInterpol = 1.5f; 
	_float					m_fRunAngle = 0.f; 
	_float					m_fOriginToPipeMidDistance = 0.f; 
	_float					m_fScaleUpSpeed = 5000.f; 

	// To Boss 
	_float					m_fDistanceRatio = 0.9f;				// 약간 떨어진 곳에서부터 출발 
	_float					m_fRatioSpeed = 1.f; 
	_float					m_fGapAngleToPlayerAndChesed = 0.f; 
	_float					m_fStartPipeAngle = 0.f; 
	
	_vec3					m_vVirtualShotDir = VEC_ZERO; 
	_vec3					m_vPipeMidToFireDir = VEC_ZERO; 

	CParticleObject*		m_pParticleBullet = nullptr; 

	CRun_Map*				m_pRunMap = nullptr; 
	CRun_Chesed*			m_pChesed = nullptr; 
	CRun_Player*			m_pPlayer = nullptr; 
};

END

#endif //__BULLET_RUN_NORMAL_H__