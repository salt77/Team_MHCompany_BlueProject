#pragma once

#ifndef __LOADING_MANAGER_H__
#define __LOADING_MANAGER_H__

#include "Engine_Define.h"
#include "Base.h"
#include "UI_Object.h"

BEGIN(Engine)

class ENGINE_DLL CLoadingManager :
	public CBase
{
	DECLARE_SINGLETON(CLoadingManager)

private:
	CLoadingManager();
	virtual ~CLoadingManager();

public:
	virtual void Free();
public:
	void		Set_Loading(_bool bLoading);
	_bool		Get_Loading() { return m_bLoading; }

	void		Loading_Render();
	static unsigned WINAPI ThreadFunction(void* pArg);
private:
	_bool		m_bLoading = FALSE;
	HANDLE		m_HandleThread;
	DWORD		m_dwThreadID = 0;

	CUI_Object*	m_pUIObject = nullptr;
};

END

#endif