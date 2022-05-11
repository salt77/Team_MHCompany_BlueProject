#pragma once
#ifndef __Scene_Manager_H__
#define __Scene_Manager_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Scene.h"
#include "ClientScene_Loading.h"

// Object


USING(Engine)

class CScene_Manager : public CBase
{
	DECLARE_SINGLETON(CScene_Manager)

private:
	CScene_Manager();
	virtual ~CScene_Manager();
	virtual void Free() override;

public:
	void					Ready_Scene_Manager(LPDIRECT3DDEVICE9 & pGraphicDev) { m_pGraphicDev = pGraphicDev; }

	void					Set_Time(const _float& fTimeDelta) { m_fTime_Min += fTimeDelta; }

	// ·Îµù
	void					Set_Loading(E_SCENETAG eSceneTag);
	_bool					Get_Is_Loading();

	_bool					Get_Loading_Finish();
	const wstring			Get_Loading_String();

	// ¾À
	_int					Set_Scene();
	_int					Set_Scene_Start();
	void					Set_UIManagerFunc();

	E_SCENETAG				Get_SceneTag() { return m_eSceneTag; }
	E_SCENETAG				Get_SceneTagNext() { return m_eSceneTagNext; }
	_bool					Get_LoadingScene() { return m_pLoading; }


private:
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;

	CClientScene_Loading*	m_pLoading = nullptr;
	CScene*					m_pScene = nullptr;

	E_SCENETAG				m_eSceneTag = E_SCENETAG::Scene_END;
	E_SCENETAG				m_eSceneTagNext = E_SCENETAG::Scene_END;

	bool					m_bStart = false;
	float					m_fTime_Min = 0.f;


};

#endif