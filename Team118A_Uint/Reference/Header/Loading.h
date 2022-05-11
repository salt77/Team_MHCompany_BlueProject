#pragma once

#ifndef __LOADING_H__ 
#define __LOADING_H__

#include "Scene.h" 

BEGIN(Engine) 

class CUI_Object; 

class ENGINE_DLL CLoading final : public CScene
{
public:
	enum LOADINGID
	{
		LOAD_DIALOG, 
		LOAD_END
	};

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CUI_Object** ppUiObj, LOADINGID eID);
	virtual void			Free();

public: 
	virtual void			Render_Scene() override;

public:
	LOADINGID				Get_LoadingID() const { return m_eLoading; }
	CRITICAL_SECTION*		Get_Crt() { return &m_Crt; }
	_bool					Get_Finish() const { return m_bFinish; }
	CUI_Object**			Get_LoadUiPrefab() { return &m_pUIObject; }

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

public:
	HRESULT					Ready_Loading(LOADINGID eLoading); 
	HRESULT					Ready_Loading(LOADINGID eLoading, CUI_Object** ppUiObj); 

private:
	_uint					Loading_ForDialog(); 

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_Crt;

	LOADINGID				m_eLoading;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bFinish;
	_tchar					m_szLoading[256];

	CUI_Object*				m_pUIObject = nullptr; 
};

END

#endif //__LOADING_H__