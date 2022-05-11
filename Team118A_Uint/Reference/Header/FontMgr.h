#pragma once
#ifndef __FONTMGR_H__
#define __FONTMGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "MyFont.h"

BEGIN(Engine)

// Font 태그 Define
#define FontTag_경기천년제목_Medium		(L"경기천년제목 Medium")
#define FontTag_경기천년제목_Bold			(L"경기천년제목 Bold")
#define FontTag_NotoSans_Medium			(L"나눔스퀘어라운드 Light")
#define FontTag_NotoSans_Bold			(L"나눔스퀘어라운드 Bold")
#define FontTag_NotoSansTC_Medium		(L"CookieRun Regular")
#define FontTag_NotoSansTC_Bold			(L"CookieRun Bold")
#define FontTag_RSU_Medium				(L"RSU Medium")
#define FontTag_RSU_Bold				(L"RSU Bold")

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	CFontMgr();
	virtual ~CFontMgr();

public:
	virtual void Free() override;

public:
	void		Clear_TextList();

public:
	HRESULT	Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
						const wstring& pFontTag,
						const T_FONT& pStFont);

	void	Render_Font(const wstring& pFontTag,
						const wstring& pString,
						const _vec2* pPos,
						D3DXCOLOR Color);

	void			Ready_FontMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	void			Render_FontEveryFrame(T_RENDERFONT tRenderFont);
	void			Render_FontTest(wstring wstrText, _long lLeft, _long lTop);

private:
	CMyFont*		Find_Font(const wstring& pFontTag);


public:

private:
	map<const wstring, CMyFont*>		m_mapFont;


	// Render_FontEveryFrame에 사용하기 위해 MyFont가 아니라 매니저에 두었다.
	// 어차피 매 프레임 or 매 출력마다 Create Release할거라 매니저에 있어도 괜찮다.
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXFONT				m_pFont[(_int)E_FontType::RSU_Bold + 1][2][50];
	LPD3DXSPRITE			m_pSprite;


public:
	LPD3DXFONT				m_pTest;
	_int					iTestWidth = 20;
	_int					iTestHeight = 40;

	UINT					m_Weight_경기_Medium		= FW_NORMAL;
	UINT					m_Weight_경기_Bold		= FW_BOLD;
	UINT					m_Weight_Noto_Medium	= FW_NORMAL;
	UINT					m_Weight_Noto_Bold		= FW_BOLD;
	UINT					m_Weight_NotoTC_Medium	= FW_NORMAL;
	UINT					m_Weight_NotoTC_Bold	= FW_BOLD;
	UINT					m_Weight_RSU_Medium		= FW_NORMAL;
	UINT					m_Weight_RSU_Bold		= FW_BOLD;



public:
	void					Add_LoadingGage()
	{
		if (m_pLoadingGage)
		{
			(*m_pLoadingGage)++;
		}
	}
	int*					m_pLoadingGage = nullptr;

};

END

#endif // !__FONTMGR_H__