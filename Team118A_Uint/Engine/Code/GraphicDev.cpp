#include "GraphicDev.h"

IMPLEMENT_SINGLETON(CGraphicDev)
USING(Engine)

Engine::CGraphicDev::CGraphicDev()
{

}

Engine::CGraphicDev::~CGraphicDev()
{
	Free();
}

HRESULT Engine::CGraphicDev::Ready_GraphicDev(HWND hWnd,
												E_WINMODE eMode,
												const _uint& iSizeX,
												const _uint& iSizeY,
												CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (nullptr == m_pSDK)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	_ulong		uFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		uFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		uFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;


	D3DPRESENT_PARAMETERS			Present_Parameter;
	ZeroMemory(&Present_Parameter, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameter.BackBufferWidth = iSizeX;
	Present_Parameter.BackBufferHeight = iSizeY;
	Present_Parameter.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameter.BackBufferCount = 1;

	Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameter.MultiSampleQuality = 0;

	Present_Parameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameter.hDeviceWindow = hWnd;

	if (E_WINMODE::MODE_WIN == eMode)
	{
		Present_Parameter.Windowed = TRUE;
	}
	else
	{
		Present_Parameter.Windowed = FALSE;
	}

	Present_Parameter.EnableAutoDepthStencil = TRUE;
	Present_Parameter.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameter.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	/*
	DWORD level = 0;
	if (SUCCEEDED(m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_D24S8, FALSE,
		D3DMULTISAMPLE_8_SAMPLES, &level)))
	{
		Present_Parameter.MultiSampleQuality = level - 1;
		Present_Parameter.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	}
	else if (SUCCEEDED(m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_D24S8, FALSE,
		D3DMULTISAMPLE_6_SAMPLES, &level)))
	{
		Present_Parameter.MultiSampleQuality = level - 1;
		Present_Parameter.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES;
	}
	else if (SUCCEEDED(m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_D24S8, FALSE,
		D3DMULTISAMPLE_4_SAMPLES, &level)))
	{
		Present_Parameter.MultiSampleQuality = level - 1;
		Present_Parameter.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
	}
	else if (SUCCEEDED(m_pSDK->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, D3DFMT_D24S8, FALSE,
		D3DMULTISAMPLE_2_SAMPLES, &level)))
	{
		Present_Parameter.MultiSampleQuality = level - 1;
		Present_Parameter.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}
	else
	{
		Present_Parameter.MultiSampleQuality = 0;
		Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
	}
	*/



	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, uFlag, &Present_Parameter, 
		&m_pGraphicDev)))
		return E_FAIL;

	*ppGraphicDev = this;

	return S_OK;
}

void Engine::CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,
		nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		Color,
		1.f,
		0);

	m_pGraphicDev->BeginScene();
}

void Engine::CGraphicDev::Render_End()
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

void CGraphicDev::Render_End(HWND hWnd)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);
}

void Engine::CGraphicDev::Free()
{
	_ulong dwRefCnt = 0;

	if (dwRefCnt = Engine::Safe_Release(m_pGraphicDev))
	{
		_tchar str[MAX_PATH];
		swprintf_s(str, L"m_pGraphicDev Release Failed - %d", dwRefCnt);
		MessageBox(NULL, str, L"System Message", MB_OK);
	}

	if (dwRefCnt = Engine::Safe_Release(m_pSDK))
	{
		_tchar str[MAX_PATH];
		swprintf_s(str, L"m_pSDK Release Failed - %d", dwRefCnt);
		MessageBox(NULL, str, L"System Message", MB_OK);
	}
}

