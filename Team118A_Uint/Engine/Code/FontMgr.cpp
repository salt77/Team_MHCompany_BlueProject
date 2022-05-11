#include "FontMgr.h"
#include <math.h>
USING(Engine)

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr()
	: m_pSprite(nullptr)
{
}


CFontMgr::~CFontMgr()
{
	Free();
}

void CFontMgr::Free()
{
	Clear_TextList();
	for (_int i = 0; i < 8; ++i)
	{
		for (_int j = 0; j < 2; ++j)
		{
			for (_int k = 0; k < 50; ++k)
			{
				Engine::Safe_Release(m_pFont[i][j][k]);
			}
		}
	}
	if (m_pSprite)
	{
		Engine::Safe_Release(m_pSprite);
	}
	Engine::Safe_Release(m_pGraphicDev);
}

void CFontMgr::Clear_TextList()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

HRESULT CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & pFontTag, const T_FONT & pStFont)
{
	CMyFont*		pFont = nullptr;

	pFont = Find_Font(pFontTag);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CMyFont::Create(pGraphicDev, pStFont);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}

void CFontMgr::Render_Font(const wstring & pFontTag, const wstring & pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	CMyFont*	pFont = Find_Font(pFontTag);
	NULL_CHECK(pFont);

	pFont->Render_Font(pString, pPos, Color);
}

CMyFont * CFontMgr::Find_Font(const wstring & pFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontMgr::Ready_FontMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return;
	}

	for (_int i = 0; i < (_int)E_FontType::RSU_Bold + 1; ++i)
	{
		UINT iWeight = FW_NORMAL;
		_float fHeightRatio = 1.f;
		wstring wstrFontName = L"";
		if ((_int)E_FontType::경기천년제목_Medium == i)
		{
			wstrFontName = FontTag_경기천년제목_Medium;
			iWeight = m_Weight_경기_Medium;
			fHeightRatio = 2.26f;
		}

		if ((_int)E_FontType::Noto_Sans_Medium == i)
		{
			wstrFontName = FontTag_NotoSans_Medium;
			iWeight = m_Weight_Noto_Medium;
			fHeightRatio = 1.7f;
		}
		if ((_int)E_FontType::Noto_Sans_TC_Medium == i)
		{
			wstrFontName = FontTag_NotoSansTC_Medium;
			iWeight = m_Weight_NotoTC_Medium;
			fHeightRatio = 1.7f;
		}
		if ((_int)E_FontType::RSU_Medium == i)
		{
			wstrFontName = L"RSU";
			iWeight = m_Weight_RSU_Medium;
			fHeightRatio = 3.f;
		}
		if ((_int)E_FontType::경기천년제목_Bold == i)
		{
			wstrFontName = FontTag_경기천년제목_Bold;
			iWeight = m_Weight_경기_Bold;
			fHeightRatio = 2.26f;
		}
		if ((_int)E_FontType::Noto_Sans_Bold == i)
		{
			wstrFontName = FontTag_NotoSans_Bold;
			iWeight = m_Weight_Noto_Bold;
			fHeightRatio = 1.7f;
		}
		if ((_int)E_FontType::Noto_Sans_TC_Bold == i)
		{
			wstrFontName = FontTag_NotoSansTC_Bold;
			iWeight = m_Weight_NotoTC_Bold;
			fHeightRatio = 1.7f;
		}
		if ((_int)E_FontType::RSU_Bold == i)
		{
			wstrFontName = L"RSU";
			iWeight = m_Weight_RSU_Bold;
			fHeightRatio = 3.f;
		}
		for (_int j = 0; j < 2; ++j)
		{
			for (_int k = 0; k < 50; ++k)
			{
				D3DXFONT_DESC	Font_Desc;
				ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

				lstrcpy(Font_Desc.FaceName, wstrFontName.c_str());
				if (wstrFontName == L"RSU" ||
					wstrFontName == L"Noto Sans TC Medium" ||
					wstrFontName == L"Noto Sans TC Bold")
				{
					Font_Desc.CharSet = DEFAULT_CHARSET;	// 시스템 로케일에 따른 문자 집합 사용
				}
				else
				{
					Font_Desc.CharSet = HANGEUL_CHARSET;
				}

				Font_Desc.Width = k + 1;
				Font_Desc.Height = (_int)(ceil(Font_Desc.Width * fHeightRatio));			// 높이
				Font_Desc.Weight = iWeight;
				Font_Desc.MipLevels = 1;								// 밉레벨 사용하지 않음, 0이면 밉레벨 생성함...
				Font_Desc.Italic = j;									// 이탤릭체 사용하지 않음
				Font_Desc.OutputPrecision	= CLIP_DEFAULT_PRECIS;		// 기본 정확도
				Font_Desc.Quality			= CLEARTYPE_QUALITY;
				Font_Desc.PitchAndFamily	= FIXED_PITCH | FW_DONTCARE;	// 기본 간격 및 패밀리 신경 쓰지 않음

				if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont[i][j][k])))
				{
					MSG_BOX("Font Create Failed");
					return;
				}
			}
		}
	}
}

void CFontMgr::Render_FontEveryFrame(T_RENDERFONT tRenderFont)
{
	if (nullptr == m_pGraphicDev)
		assert(FALSE);

	if (tRenderFont.iWidth >= 50)
		return;

	_int iFontType = 0;

	if (FontTag_경기천년제목_Medium == tRenderFont.szFontTag)			iFontType = 0;
	else if (FontTag_NotoSansTC_Medium == tRenderFont.szFontTag)	iFontType = 1;
	else if (FontTag_NotoSans_Medium == tRenderFont.szFontTag)		iFontType = 2;
	else if (FontTag_RSU_Medium == tRenderFont.szFontTag)			iFontType = 3;
	else if (FontTag_경기천년제목_Bold == tRenderFont.szFontTag)		iFontType = 4;
	else if (FontTag_NotoSans_Bold == tRenderFont.szFontTag)		iFontType = 5;
	else if (FontTag_NotoSansTC_Bold == tRenderFont.szFontTag)		iFontType = 6;
	else if (FontTag_RSU_Bold == tRenderFont.szFontTag)				iFontType = 7;

	_int iSize = 0;
	if (tRenderFont.iWidth >= 0)
	{
		iSize = tRenderFont.iWidth - 1;
	}
	RECT	rc{ _long(tRenderFont.vPosition.x) + (_long)(WINCX >> 1), _long(tRenderFont.vPosition.y) + (_long)(WINCY >> 1) };
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont[iFontType][tRenderFont.bItalic][iSize]->DrawTextW(m_pSprite, tRenderFont.szString.c_str(), lstrlen(tRenderFont.szString.c_str()), &rc, DT_NOCLIP, tRenderFont.Color);
	m_pSprite->End();
}

void CFontMgr::Render_FontTest(wstring wstrText, _long lLeft, _long lTop)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	wstring wstrFontName = L"RSU";
	lstrcpy(Font_Desc.FaceName, wstrFontName.c_str());
	if (wstrFontName == L"RSU" ||
		wstrFontName == L"Noto Sans TC Medium" ||
		wstrFontName == L"Noto Sans TC Bold")
	{
		Font_Desc.CharSet = DEFAULT_CHARSET;	// 시스템 로케일에 따른 문자 집합 사용
	}
	else
	{
		Font_Desc.CharSet = HANGEUL_CHARSET;
	}
	if (lTop <= 300)
	{
		Font_Desc.Width = iTestWidth;
		Font_Desc.Height = iTestHeight;			// 높이
	}
	else
	{
		Font_Desc.Width = 30;
		Font_Desc.Height = 60;			// 높이
	}
	Font_Desc.Weight = m_Weight_RSU_Medium;
	Font_Desc.MipLevels = 1;								// 밉레벨 사용하지 않음, 0이면 밉레벨 생성함...
	Font_Desc.Italic = 0;									// 이탤릭체 사용하지 않음
	Font_Desc.OutputPrecision	= CLIP_DEFAULT_PRECIS;		// 기본 정확도


	if (lTop == 0)
	{
		Font_Desc.Quality = DEFAULT_QUALITY;
	}
	else if (lTop == 100)
	{
		Font_Desc.Quality = CLEARTYPE_QUALITY;
	}
	else if (lTop == 200)
	{
		Font_Desc.Quality = PROOF_QUALITY;
	}
	else if (lTop == 300)
	{
		Font_Desc.Quality = ANTIALIASED_QUALITY;
	}
	Font_Desc.PitchAndFamily	= FIXED_PITCH | FW_DONTCARE;	// 기본 간격 및 패밀리 신경 쓰지 않음

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pTest)))
	{
		MSG_BOX("Font Create Failed");
		return;
	}


	RECT	rc{ lLeft, lTop };
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pTest->DrawTextW(m_pSprite, wstrText.c_str(), lstrlen(wstrText.c_str()), &rc, DT_NOCLIP, D3DXCOLOR(0.f,0.f,0.f,1.f));
	m_pSprite->End();

	Safe_Release(m_pTest);
}
