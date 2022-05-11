#pragma once
#ifndef __UI_SLIDER_VERT_H__
#define __UI_SLIDER_VERT_H__
#include "UI_Object.h"

class ENGINE_DLL CUI_Slider_Vert final: public CUI_Object
{
protected:
	explicit CUI_Slider_Vert(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	explicit CUI_Slider_Vert(const CGameObject& rhs);
	virtual ~CUI_Slider_Vert();

public:
	static CUI_Slider_Vert*		Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag) ;
	virtual void				Free() override;
	virtual void				Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void				Load(HANDLE hFile, DWORD& dwByte) override;
	virtual CUI_Slider_Vert*	Clone();

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;
	virtual	void		Child_Render_Object() override;


	virtual void		Debug_This_In_Console() override;

public:
	void				Add_Component();

public:
	void				Bar_Action();

public:
	_float				Get_Slider_Ratio();

private:
	CUI_Object*			m_pBar = nullptr;
	_bool				m_bBarClicked = FALSE;
	_float				m_fSliderMin = 0;
	_float				m_fSliderMax = 100;

};

#endif // !__UI_SLIDER_VERT_H__