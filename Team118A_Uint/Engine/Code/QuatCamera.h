#pragma once

#ifndef __QuatCamera_H__
#define __QuatCamera_H__

#include "Camera.h"

BEGIN(Engine)

class CRun_Player; 
class CRun_Map; 
class CParticleObject; 
class CUI_Object; 

class ENGINE_DLL CQuatCamera final : public CCamera
{
private:
	explicit CQuatCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag = E_OBJTAG::OBJ_DynamicCamera);
	explicit CQuatCamera(const CQuatCamera& rhs);
	virtual ~CQuatCamera();

public:
	static CQuatCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
							   HWND hWnd,
							   T_CAMINFO tCamInfo,
							   E_OBJTAG eObjTag = E_OBJTAG::OBJ_DynamicCamera);

	virtual void		Free() override;

public:
	HRESULT			Ready_Object(T_CAMINFO tCamInfo);
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(_int iPass) override; 

public:
	void			Set_StartGame_Camera();
	void			Set_EndGame_Camera() { m_bEndGame = TRUE; }

private:
	void			Add_Component(); 
	void			Set_Constantable() override; 
	void			Key_Input(const _float& fTimeDelta);
	void			Update_MousePos();
	void			Rotate_Cam(const _float& fTimeDelta); 
	void			CutScene_View(const _float& fTimeDelta); 
	void			EndGame_View(const _float& fTimeDelta);
	void			Mouse_Fix();

private:
	_bool				m_bRenderSphere = FALSE; 
	_bool				m_bCutSceneView = FALSE; 
	_bool				m_bEndGame = FALSE; 

	_float				m_fCamDistance = 17.5f; 
	_float				m_fCamAngle = 60.f; 
	_float				m_fFollowSpeed = 0.1f; 
	_float				m_fRunAngle = 0.f; 
	_float				m_fCutSceneSpeed = 0.001f; 
	_float				m_fEndGameInterpolAngle = 0.f; 

	_vec2				m_vMovePos = VEC2_ZERO;
	_vec2				m_vPtMousePos = VEC2_ZERO;
	_vec2				m_vCenterPos = VEC2_ZERO;

	_vec3				m_vTargetEyePos = VEC_ZERO; 

	_quat				m_quatRot; 

	CRun_Player*		m_pPlayer = nullptr; 
	CRun_Map*			m_pRunMap = nullptr;
	CParticleObject*	m_pParticle_Wind = nullptr;
	CUI_Object*			m_pFadeInOutUI = nullptr; 

	LPD3DXMESH			m_pSphereMesh = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr; 

	CShader*			m_pSphereShaderCom = nullptr; 

public:
	HWND				m_Hwnd;
};

END

#endif // __QuatCamera_H__
