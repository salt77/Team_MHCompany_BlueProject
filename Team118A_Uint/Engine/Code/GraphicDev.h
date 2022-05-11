#ifndef __GraphicDev_h__
#define __GraphicDev_h__

/*
그래픽 디바이스
담당자 : 최은호

*/

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	CGraphicDev();
	virtual		~CGraphicDev();

public:
	LPDIRECT3DDEVICE9		GetDevice() { return m_pGraphicDev; }

public:
	HRESULT		Ready_GraphicDev(HWND hWnd,	E_WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);
	void		Render_Begin(D3DXCOLOR Color);
	void		Render_End();
	void		Render_End(HWND hWnd);

public:
	virtual void Free();

private:
	LPDIRECT3D9				m_pSDK = nullptr;			// COM 객체(클래스 포인터)
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;	// 그리기와 관련된 COM 객체
};

END
#endif // __GraphicDev_h__
