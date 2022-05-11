#pragma once
#ifndef __UI_DAMAGE_H__
#define __UI_DAMAGE_H__

#include "UI_Object.h"
class ENGINE_DLL CUI_Damage final : public CUI_Object
{
protected:
	explicit CUI_Damage(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag, _float fLifeTotal, E_NumberFontType eNumberType, _int iNumber, _bool bCritical);
	explicit CUI_Damage(const CGameObject& rhs);
	virtual ~CUI_Damage();

public:
	static CUI_Damage*		Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag, _float fLifeTotal, E_NumberFontType eNumberType, _int iNumber, _bool bCritical = FALSE);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;
	virtual CUI_Damage*		Clone();

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass = 0) override;
	virtual	void		Child_Render_Object();

	virtual void		Debug_This_In_Console() override;

	virtual void		Start_Font_Animation();
	virtual void		Animation(const _float& fTimeDelta);

public:
	void				Add_Component();
	void				Set_ConstantTable(_int iMax, _int iCount, _int iNum);
	void				Set_ConstantTable_Critical(_int iMax);
	void				Set_ConstantTable_Weak(_int iMax);
	void				Set_ConstantTable_Resist(_int iMax);
	void				Set_ConstantTable_ImmuneBlockMiss(_int iMax);




	void				Set_Number(_int iNumber)		{ m_iNumber = iNumber;							}
	void				Set_NumberType(_int iNumber)	{ m_eNumberType = (E_NumberFontType)iNumber;	}
	void				Set_Evade(_bool bState)			{ m_bMiss = bState;								}
	void				Set_Immune(_bool bState)		{ m_bImmune = bState;							}
	void				Set_Block(_bool bState)			{ m_bBlock = bState;							}

	_int				Get_NumberType() { return (_int)m_eNumberType; }

private:
	LPDIRECT3DTEXTURE9	m_pTexture_Font[(_int)E_NumberFontType::E_End];
	LPDIRECT3DTEXTURE9	m_pTexture_Critical = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture_Weak		= nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture_Resist	= nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture_Immune	= nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture_Miss		= nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture_Block	= nullptr;



	wstring				wstrFullPath[(_int)E_NumberFontType::E_End];
	E_NumberFontType	m_eNumberType = E_NumberFontType::White;
	_int				m_iNumber		= 0;
	_float				m_fLifeTime		= 1.f;
	_float				m_fLifeTotal	= 1.f;
	_float				m_fFontSizeRT	= 1.f;
	_bool				m_bCritical		= FALSE;
	_bool				m_bImmune		= FALSE;
	_bool				m_bMiss			= FALSE;
	_bool				m_bBlock		= FALSE;	
};

#endif // !__UI_DAMAGE_H__