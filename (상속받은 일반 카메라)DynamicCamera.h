#pragma once

#ifndef __DynamicCamera_H__
#define __DynamicCamera_H__

#include "Camera.h"

class ENGINE_DLL CDynamicCamera final : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag = E_OBJTAG::OBJ_DynamicCamera);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera();

public:
	static CDynamicCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								   HWND hWnd,
								   T_CAMINFO tCamInfo,
								   E_OBJTAG eObjTag = E_OBJTAG::OBJ_DynamicCamera);

	virtual void			Free() override;

public:
	HRESULT			Ready_Object(T_CAMINFO tCamInfo);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override; 

private:
	void			Key_Input(const _float& fTimeDelta);
	void			Update_MousePos();
	void			Mouse_Move();
	void			Mouse_Fix();
	void			Update_ShakeCamera(const _float& fTimeDelta); 

	_bool			Can_Angle(_vec3 vPos, _vec3 vTargetPos);

private:  
	_vec2			m_vMovePos = VEC2_ZERO; 
	_vec2			m_vPtMousePos = VEC2_ZERO; 
	_vec2			m_vCenterPos = VEC2_ZERO; 

public:
	HWND			m_Hwnd;
};

#endif // DynamicCamera_h__
