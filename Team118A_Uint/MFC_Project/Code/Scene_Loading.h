#pragma once
#ifndef __SCENE_LOADING_H__
#define __SCENE_LOADING_H__

#include "Base.h"

enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

class CScene_Loading final: public CBase
{
private:
	CScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_Loading();

public:
	static CScene_Loading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_SCENETAG eSceneTag);
	virtual void		Free() override;

public:
	HRESULT				Ready_Loading(E_SCENETAG eLoadingSceneTag);

	_uint				Loading_LoadingScene();
	_uint				Loading_Logo();
	_uint				Loading_Lobby();
	_uint				Loading_Lobby2();
	_uint				Loading_Cafe();
	_uint				Loading_Combat();
	_uint				Loading_TileField();
	_uint				Loading_RunTest(); 
public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

public:
	E_SCENETAG			Get_LoadingID() const { return m_eLoadingSceneTag; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() const { return m_bFinish; }
	const wstring		Get_Wstring() { return m_wstrLoading; }

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;

	E_SCENETAG			m_eLoadingSceneTag;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	_bool				m_bFinish;

	wstring				m_wstrLoading;
};

#endif // !__SCENE_LOADING_H__