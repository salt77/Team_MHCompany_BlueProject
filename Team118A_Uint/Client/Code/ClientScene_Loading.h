#pragma once
#ifndef Loading_h__
#define Loading_h__

#include "Engine_Define.h"
#include "Base.h"

class CClientScene_Loading final : public CBase
{
private:
	CClientScene_Loading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CClientScene_Loading();

public:
	static CClientScene_Loading*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_SCENETAG eSceneTag);
	virtual void		Free() override;
	
public:
	HRESULT				Ready_Loading(E_SCENETAG eLoadingSceneTag);

	_uint				Loading_LoadingScene();
	_uint				Loading_Logo();
	_uint				Loading_Lobby();
	_uint				Loading_Cafe();
	_uint				Loading_Combat();
	_uint				Loading_Running();
	_uint				Loading_Tile();

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


	static _bool		m_bLoad_Lobby;
};

#endif // Loading_h__
