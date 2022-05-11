#pragma once

#ifndef __Camera_TPS_H__
#define __Camera_TPS_H__

#include "Camera.h"

USING(Engine)

#include "Character.h"

class ENGINE_DLL CCamera_TPS final : public CCamera
{
public:
	enum Mode_TPS
	{
		TPS_Normal, TPS_Aiming, TPS_Stand, 
		TPS_End
	};

private:
	explicit CCamera_TPS(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag = E_OBJTAG::OBJ_Camera_TPS);
	explicit CCamera_TPS(const CCamera_TPS& rhs);
	virtual ~CCamera_TPS();

public:
	static CCamera_TPS*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
								   HWND hWnd,
								   T_CAMINFO tCamInfo,
								   E_OBJTAG eObjTag = E_OBJTAG::OBJ_Camera_TPS);

	virtual void			Free() override;

public:
	HRESULT			Ready_Object(T_CAMINFO tCamInfo);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;

public:
	const _vec2&	Get_MouseMove() { return m_vMovePos; }
	const _bool&	Get_MouseFix() { return m_bFix; }

public:
	void			Set_TpsCamMode(Mode_TPS eMode) { m_eTpsCamMode = eMode; }

private:
	void			Key_Input(const _float& fTimeDelta);
	void			Update_MousePos();
	void			Mouse_Move();
	void			Mouse_Fix();

	void			TPS_CameraPosition();

	// 카메라의 위 아래 회전을 180도로 고정 
	_bool			Can_Angle(_vec3 vPos, _vec3 vTargetPos);

private:
	_float			m_fAngle = 0.f;
	_float			m_fSpeed = 10.f;

	_float			m_fPlayerToCamDis = 0.85f;
	_float			m_fDistanceFromGround = 0.4f;

	_vec2			m_vMovePos = _vec2(0.f, 0.f);
	_vec2			m_vPtMousePos = _vec2(0.f, 0.f);
	_vec2			m_vCenterPos = _vec2(0.f, 0.f);

	Mode_TPS		m_eTpsCamMode = TPS_Normal;

	// Instance 
	CCharacter*		m_pPlayer = nullptr;

public:
	HWND			m_Hwnd;
};

#endif // __Camera_TPS_H__
