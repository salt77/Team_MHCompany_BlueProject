#pragma once

#ifndef __CHESED_DIRECT_LASER_H__
#define __CHESED_DIRECT_LASER_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Chesed;
class CRun_Map; 
class CRun_Player; 
class CParticleObject; 

class ENGINE_DLL Chesed_Direct_Laser final : public CAttackObj
{
private:
	explicit Chesed_Direct_Laser(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit Chesed_Direct_Laser(const Chesed_Direct_Laser& rhs);
	virtual ~Chesed_Direct_Laser();

public:
	static	Chesed_Direct_Laser*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object();
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
	void					Movement(const _float& fTimeDelta); 
	void					Toward_Player(const _float& fTimeDelta); 

private:
	_float					m_fTargetRunAngle = 0.f; 
	_float					m_fFrontTargetRunAngle = 15.f; 
	_float					m_fBackTargetRunAngle = 5.f; 
	_float					m_fTargetPipeAngle = 0.f; 

	CParticleObject*		m_pDirectLaserParticle = nullptr; 
	CParticleObject*		m_pSparkParticle = nullptr; 
	CRun_Chesed*			m_pChesed = nullptr;
	CRun_Map*				m_pRunMap = nullptr; 
	CRun_Player*			m_pPlayer = nullptr; 
};

END

#endif //__CHESED_DIRECT_LASER_H__