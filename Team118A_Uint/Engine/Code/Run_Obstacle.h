#pragma once

#ifndef __RUN_OBSTACLE_H__
#define __RUN_OBSTACLE_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Map;
class CRun_Player;
class CParticleObject;

class ENGINE_DLL CRun_Obstacle final : public CAttackObj
{
private:
	explicit CRun_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CRun_Obstacle(const CRun_Obstacle& rhs);
	virtual ~CRun_Obstacle();

public:
	static	CRun_Obstacle*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								   const _float& fRunAngle, 
								   E_LAYERTAG eTargetLayerTag); 
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _float& fRunAngle);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass = 0) override;

public:
	virtual void			On_Collision(CGameObject* pDest) override;

private:
	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

private:
	void					Add_Component();
	void					Appear(const _float& fTimeDelta);
	void					Check_Dead();

private:
	_bool					m_bShakeOneTime = FALSE;

	_float					m_fRunAngle = 0.f; 
	_float					m_fAngleFront = 65.f; 
	_float					m_fPipeAngle = 0.f; 
	_float					m_fSpeed = 300.f; 

	_vec3					m_vAppearDir = VEC_ZERO; 
	_vec3					m_vTargetPos = VEC_ZERO; 

	CRun_Player*			m_pPlayer = nullptr; 
	CRun_Map*				m_pRunMap = nullptr; 
	CParticleObject*		m_pParticleBreak = nullptr; 

	CSMesh*					m_pSMeshCom = nullptr; 
};

END

#endif //__RUN_OBSTACLE_H__