#pragma once
#ifndef __MainApp_H__
#define __MainApp_H__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Management.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp();

public:
	static CMainApp*		Create();
	virtual	void			Free();

public:
	HRESULT Ready_Main();

public:
	void Update_Main(const _float& fTimeDelta);
	
private:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT					Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);

private:
	CGraphicDev*			m_pDeviceClass		= nullptr;
	CManagement*			m_pManagementClass	= nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev		= nullptr;
};

#endif // !__MainApp_H__