#pragma once

#ifndef __Camera_Cafe_H__
#define __Camera_Cafe_H__

#include "Camera.h"

USING(Engine)

class ENGINE_DLL CCamera_Cafe final : public CCamera
{
private:
	explicit CCamera_Cafe(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag = E_OBJTAG::OBJ_Camera_Cafe);
	explicit CCamera_Cafe(const CCamera_Cafe& rhs);
	virtual ~CCamera_Cafe();

public:
	static CCamera_Cafe*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								   HWND hWnd,
								   T_CAMINFO tCamInfo,
								   E_OBJTAG eObjTag = E_OBJTAG::OBJ_Camera_Cafe);

	virtual void			Free() override;

public:
	HRESULT			Ready_Object(T_CAMINFO tCamInfo);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;

private:
	void		Update_MousePos();
	void		Compute_Distance(const _vec3& vFrontDir); 

private:
	void		PointMove(const _float& fTimeDelta);

private:
	_vec2		m_vMovePos = _vec2(0.f, 0.f);
	_vec2		m_vPtMousePos = _vec2(0.f, 0.f);
	_vec2		m_vCenterPos = _vec2(0.f, 0.f);

	_float		m_fAngle = 0.f;
	_float		m_fSpeed = 10.f;
	_float		m_fForwardMoveDis = 0.f;
	_float		m_fVerticalMoveDis = 0.f; 
	_float		m_fHorizonMoveDis = 0.f; 

	// 범위 제한 변수 
	_float		m_fForwardLimit = 2.f;
	_float		m_fVerticalLimit = 1.f;
	_float		m_fHorizonLimit = 3.f;

	_vec3		m_vForwardVirtualPos = VEC_ZERO; 
	_vec3		m_vVerticalVirtualPos = VEC_ZERO; 
	_vec3		m_vHorizonVirtualPos = VEC_ZERO; 

	POINT		m_pPickZoom;
	POINT		m_pPickMove;
	_float		m_fZoomTime = 0.f;
	_float		m_fMoveTime = 0.f;
	_float		m_fRLSpeed = 6.f;
	_float		m_fUpSpeed = 3.f;

public:
	HWND	m_Hwnd;
};

#endif // DynamicCamera_h__
