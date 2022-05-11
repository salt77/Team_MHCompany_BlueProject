#pragma once

#ifndef __DRONE_BULLET_H__
#define __DRONE_BULLET_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Player;
class CRun_Map; 

class ENGINE_DLL CDrone_Bullet final : public CAttackObj
{
private:
	explicit CDrone_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit CDrone_Bullet(const CDrone_Bullet& rhs);
	virtual ~CDrone_Bullet();

public:
	static	CDrone_Bullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								   const _float& fRunAngle, 
								   const _vec3& vToFireDir, 
								   E_LAYERTAG eTargetLayerTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _float& fRunAngle, const _vec3& vToFireDir);
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
	void					Check_Dead(); 

private:
	_float					m_fSpeed = 0.f; 
	_float					m_fSpeedInterpol = 0.1f; 
	_float					m_fRunAngle = 0.f; 
	_float					m_fDistanceOriginToPipe = 0.f; 
	_float					m_fDeadAngle = 15.f;

	_vec3					m_vToFireDir = VEC_ZERO; 

	CRun_Player*			m_pPlayer = nullptr; 
	CRun_Map*				m_pRunMap = nullptr; 
};

END

#endif //__DRONE_BULLET_H__