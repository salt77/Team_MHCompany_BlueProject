#include "MyFont.h"

USING(Engine)

CMyFont::CMyFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pGraphicDev->AddRef();
}


CMyFont::~CMyFont()
{
}

void CMyFont::Free()
{
	Engine::Safe_Release(m_pFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pGraphicDev);
}

CMyFont * CMyFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, const T_FONT & st_Font)
{
	CMyFont*	pInstance = new CMyFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(st_Font)))
	{
		MSG_BOX("Font Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMyFont::Ready_Font(const T_FONT & st_Font)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	lstrcpy(Font_Desc.FaceName, st_Font.FontName.c_str());
	if (st_Font.FontName == L"RSU" ||
		st_Font.FontName == L"Noto Sans TC Medium" ||
		st_Font.FontName == L"Noto Sans TC Bold")
	{
		Font_Desc.CharSet = DEFAULT_CHARSET;
	}
	else
	{
		Font_Desc.CharSet = HANGEUL_CHARSET;;
	}
	Font_Desc.Width = st_Font.iWidth;
	Font_Desc.Height = st_Font.iHeight;
	Font_Desc.Weight = st_Font.iWeight;
	Font_Desc.MipLevels = 0;
	Font_Desc.Italic = FALSE;
	Font_Desc.OutputPrecision = 0;
	Font_Desc.Quality = 0;
	Font_Desc.PitchAndFamily = 0;

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont)))
	{
		MSG_BOX("Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		MSG_BOX("Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CMyFont::Render_Font(const wstring& pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	RECT	rc{ _long(pPos->x) + (_long)(WINCX >> 1), _long(pPos->y) + (_long)(WINCY >> 1) };
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pFont->DrawTextW(m_pSprite, pString.c_str(), lstrlen(pString.c_str()), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}