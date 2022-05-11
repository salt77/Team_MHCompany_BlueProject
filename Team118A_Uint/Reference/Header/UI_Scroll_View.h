#pragma once
#ifndef __UI_Scroll_View_H__
#define __UI_Scroll_View_H__
#include "UI_Object.h"

class ENGINE_DLL CUI_Scroll_View final: public CUI_Object
{
protected:
	explicit CUI_Scroll_View(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	explicit CUI_Scroll_View(const CGameObject& rhs);
	virtual ~CUI_Scroll_View();

public:
	static CUI_Scroll_View*		Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	virtual void				Free() override;
	virtual void				Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void				Load(HANDLE hFile, DWORD& dwByte) override;
	virtual CUI_Scroll_View*	Clone();

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
	virtual void		OnMouse_ThisFrame();

public:
	void				Set_Child(CUI_Object* pUi);
	void				Set_Geometry(E_Geometry eGeometry)				{ m_eGeometry = eGeometry;							}
	void				Set_Padding(E_DIR eDir, _int iAmount)			{ m_iPadding[(_int)eDir] = iAmount;					}
	void				Set_ChildSize(E_POSITION ePos, _int iAmount)	{ m_iChildSize[(_int)ePos] = iAmount;				}
	void				Set_Spacing(E_POSITION ePos, _int iAmount)		{ m_iSpacing[(_int)ePos] = iAmount;					}
	void				Set_ChildSizeUse(_bool bState)					{ m_bChildSizeUse = bState;							}
	void				Set_ScrollOption(E_Geometry eGeo, _bool bState);

	const E_Geometry	Get_Geometry()									{ return m_eGeometry;								}
	const _int			Get_Padding(E_DIR eDir)							{ return m_iPadding[(_int)eDir];					}
	const _int			Get_ChildSize(E_POSITION ePos)					{ return m_iChildSize[(_int)ePos];					}
	const _int			Get_Spacing(E_POSITION ePos)					{ return m_iSpacing[(_int)ePos];					}
	_bool				Get_ChildSizeUse()								{ return m_bChildSizeUse;							}
	_bool				Get_ScrollOption(E_Geometry eGeo);

private:
	E_Geometry			m_eGeometry								= E_Geometry::E_GEOMETRY_END;
	_int				m_iPadding[(_int)E_DIR::E_END]			= { 0,		};
	_int				m_iChildSize[(_int)E_POSITION::POS_Z]	= { 50, 50	};
	_int				m_iSpacing[(_int)E_POSITION::POS_Z]		= { 0,		};
	_bool				m_bChildSizeUse = FALSE;
	_bool				m_bVertScroll = TRUE;
	_bool				m_bHoriScroll = TRUE;
};

#endif // !__UI_Scroll_View_H__