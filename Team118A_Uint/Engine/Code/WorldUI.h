#pragma once

#ifndef __WORLD_UI_H__
#define __WORLD_UI_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"
#include "UI_Object.h"

BEGIN(Engine)

class ENGINE_DLL CWorldUI :
	public CGameObject
{
private:
	explicit CWorldUI(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUIPrefabPath);
	explicit CWorldUI(const CWorldUI& rhs);
	virtual ~CWorldUI();

public:
	static CWorldUI*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUIPrefabPath);
	virtual void		Free();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;

	CUI_Object*			Get_UIObj() { return m_pUIObj; }
private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	CUI_Object*			m_pUIObj = nullptr;
	_vec2				m_vUILocalPos = _vec2(0.f, 0.f);
	wstring				m_wstrUIPrefabPath = L"";
};

END

#endif