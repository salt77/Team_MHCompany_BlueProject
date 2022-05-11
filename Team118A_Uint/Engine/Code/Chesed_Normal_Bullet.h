#pragma once

#ifndef __CHESED_NORMAL_BULLET_H__
#define __CHESED_NORMAL_BULLET_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Player; 
class CRun_Map; 
class CTexture; 
class CTanmak_Effect; 

class ENGINE_DLL Chesed_Normal_Bullet final : public CAttackObj
{
private:
	explicit Chesed_Normal_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit Chesed_Normal_Bullet(const Chesed_Normal_Bullet& rhs);
	virtual ~Chesed_Normal_Bullet();

public:
	static	Chesed_Normal_Bullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 	
										   const _vec3& vDir, 
										   const _float& fRunAngle,
										   const _float& fPipeAngle, 
										   const E_CHESED_ATKTYPE& eAtkType, 
										   const _float& fPipeAngleRandValue = 0.f,
										   const E_TANMAKTYPE& eType = E_TANMAKTYPE::TANMAK_END,
										   E_LAYERTAG eTargetLayerTag = E_LAYERTAG::Character);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _vec3& vDir, 
										 const _float& fRunAngle, 
										 const _float& fPipeAngle, 
										 const _float& fPipeAngleRandValue,
										 const E_CHESED_ATKTYPE& eAtkType, 
										 const E_TANMAKTYPE& eType);
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
	void					Create_Bullet_Dead(); 
	void					Check_Dead(); 

private: 
	// General 
	_float					m_fLife = 3.f; 
	_float					m_fSkillCountTime = 0.f; 
	_float					m_fDeadAngle = 15.f;

	// Deploy 
	_float					m_fRunAngle = 0.f; 
	_float					m_fDistanceFromPipeMid = 0.f; 
	_float					m_fArriveTime = 5.f; 
	_float					m_fDistanceToTarget = 0.f; 

	// Rounded 
	_float					m_fPipeAngle = 0.f; 
	_float					m_fRoundedSpeed = 150.f; 
	_float					m_fDistanceRounded = 0.f; 
	_float					m_fRunSpeed = 0.f; 
	_float					m_fRunSpeedInterpol = 0.75f; 
	_float					m_fDistanceFromPipeMidInterpol = 0.85f; 

	_vec3					m_vMoveDir = VEC_ZERO; 
	_vec3					m_vTargetPos = VEC_ZERO; 

	E_CHESED_ATKTYPE		m_eAtkType = E_CHESED_ATKTYPE::CHESED_ATK_END; 
	E_TANMAKTYPE			m_eTanmakType = E_TANMAKTYPE::TANMAK_END; 

	CRun_Player*			m_pPlayer = nullptr; 
	CRun_Map*				m_pRunMap = nullptr; 

	CRectBuffer*			m_pBufferCom = nullptr; 
	CTexture*				m_pTextureCom = nullptr; 
	CTanmak_Effect*			m_pTanmakEffect = nullptr; 
};

END

#endif //__CHESED_NORMAL_BULLET_H__