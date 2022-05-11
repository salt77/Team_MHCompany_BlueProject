#ifndef __GraphicDev_h__
#define __GraphicDev_h__

/*
�׷��� ����̽�
����� : ����ȣ

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
	LPDIRECT3D9				m_pSDK = nullptr;			// COM ��ü(Ŭ���� ������)
	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;	// �׸���� ���õ� COM ��ü
};

END
#endif // __GraphicDev_h__
