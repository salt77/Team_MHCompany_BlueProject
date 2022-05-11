#pragma once

#ifndef __CHESED_CIRCLE_BULLET_H__
#define __CHESED_CIRCLE_BULLET_H__

#include "AttackObj.h"

BEGIN(Engine)

class CRun_Player; 
class CRun_Chesed; 
class CRun_Map; 
class CTanmak_Effect; 
class CTexture; 

class ENGINE_DLL Chesed_Circle_Bullet final : public CAttackObj
{
private:
	explicit Chesed_Circle_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag);
	explicit Chesed_Circle_Bullet(const Chesed_Circle_Bullet& rhs);
	virtual ~Chesed_Circle_Bullet();

public:
	static	Chesed_Circle_Bullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										   const _float& fEarlyPipeAngle, 
										   const E_TANMAKTYPE& eType = E_TANMAKTYPE::TANMAK_NORMAL, 
										   E_LAYERTAG eTargetLayerTag = E_LAYERTAG::Character);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _float& fEarlyPipeAngle, const E_TANMAKTYPE& eType);
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
	_float					m_fLife = 1500.f;
	_float					m_fRunAngle = 0.f; 
	_float					m_fPipeAngle = 0.f; 
	_float					m_fPipeAngleSpeed = 50.f; 
	_float					m_fDistanceRounded = 0.f; 
	_float					m_fEarlyDistanceInterpol = 0.6f; 
	_float					m_fSpreadSpeed = 100.f; 
	_float					m_fDistanceOriginToPipe = 0.f; 
	_float					m_fDistancePipeRadius = 0.f; 
	_float					m_fDeadAngle = 15.f; 

	_float					m_fNextPhaseDelay = 1.5f; 
	_float					m_fNextDistanceInterpol = 0.85f; 

	E_TANMAKTYPE			m_eTanmakType = E_TANMAKTYPE::TANMAK_END; 

	CRun_Player*			m_pPlayer = nullptr; 
	CRun_Chesed*			m_pChesed = nullptr; 
	CRun_Map*				m_pRunMap = nullptr; 
	CTanmak_Effect*			m_pTanmakEffect = nullptr; 

	CRectBuffer*			m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
};

END

#endif //__CHESED_CIRCLE_BULLET_H__