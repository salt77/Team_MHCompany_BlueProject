#pragma once
#ifndef __SCENE_COMBAT_H__
#define __SCENE_COMBAT_H__

#include "Scene.h"
class CWorldCreator;
class CClientScene_Combat final: public CScene
{
private:
	CClientScene_Combat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Combat();

public:
	static CClientScene_Combat*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;

public:

public:
	virtual HRESULT	Ready_Scene() override;
	virtual	_int	Start_Scene(const _float& fTimeDelta) override;
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene() override;

protected:
	HRESULT			Ready_Prototype();
	HRESULT			Ready_LightInfo();
	HRESULT			Binah_OtherSetting();
	HRESULT			Ready_SceneObject();

public:
	virtual void		Set_FogEvent();
	virtual void		Update_FogEvent(const _float& fTimeDelta);

public:
	virtual void		Active_AutoCam();
	virtual void		Set_Fit_Camera();
	virtual void		Update_Fit_Camera(const _float& fTimeDelta);

public:
	virtual void		Active_ShakeCam(const _float& fShakeTime, const _float& fShakePower);
	virtual void		Update_ShakeCam(const _float& fTimeDelta);

public:
	void				End_Battle(E_SCENETAG eTag);

private:
	void				Spawn_Striker();
	void				Spawn_Special();
	void				Set_Outtro();
	void				Change_World();
	void				Play_Game();
	void				TimeEvent(_float fTimeDelta);
	void				Update_BattleStartUI(const _float& fTimeDelta);

private:
	_bool				m_bEnd = FALSE;
	_float				m_fTimer = 0.f;
	
	_int				m_iEventCount = 0;
	_vec3				m_vCamSpot1;
	_vec3				m_vCamSpot2;

private:
	CWorldCreator*		m_pWorld = nullptr;
	CWorldCreator*		m_pWorld2 = nullptr;

	_float				m_fBattleStartOriginDelay = 1.8f;
	_float				m_fBattleStartDelay = m_fBattleStartOriginDelay;

	CUI_Object*			m_pBattleStartUI = nullptr;
	CUI_Object*			m_pBattleStartOthersUI = nullptr;
	CUI_Object*			m_pBattleUI_Stage = nullptr;

private:
	_bool				m_bCreate_Binah_Sky = true;

};

#endif // !__SCENE_LOBBY_H__