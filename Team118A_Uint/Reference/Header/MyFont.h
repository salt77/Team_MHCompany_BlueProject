#pragma once
#ifndef __MYFONT_H__
#define __MYFONT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CMyFont final: public CBase
{
private:
	explicit CMyFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyFont();

public:
	virtual void		Free() override;
	static CMyFont*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const T_FONT& st_Font);

public:
	HRESULT				Ready_Font(const T_FONT& st_Font);

	void				Render_Font(const wstring& pString,
									const _vec2* pPos,
									D3DXCOLOR Color);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXFONT				m_pFont;
	LPD3DXSPRITE			m_pSprite;
};

END

#endif // !__MYFONT_H__