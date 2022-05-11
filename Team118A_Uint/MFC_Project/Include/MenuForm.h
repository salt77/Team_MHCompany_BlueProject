#pragma once
#ifndef __MENUFORM_H__
#define __MENUFORM_H__

#include "Tool_World.h"
#include "Tool_Camera.h"
#include "Tool_Ui.h"
#include "Tool_Map.h"

#include "Tool_Particle.h"
#include "Tool_Effect.h"
#include "Tool_Tile.h"
#include "Tool_Animation.h"

#include "PopupTool_ObjectTree.h"
#include "PopupTool_TextureTree.h"
#include "Popup_LightOption.h"
#include "Popup_EffectBase.h"
#include "Popup_EffectFilter.h"

#include "afxcmn.h"

// CMenuForm 폼 뷰입니다.
enum class eToolMenu
{
	World, Camera, UI, Map, Particle, Effect, Animation, Tile, TOOL_END
};

class CMenuForm : public CFormView
{
	DECLARE_DYNCREATE(CMenuForm)

protected:
	CMenuForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMenuForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_MENUFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const
	{

	}
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const
	{

	}
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	eToolMenu			ToolMenuState;

	CTool_World*		m_pTool_World;
	CTool_Tile*			m_pTool_Tile;
	CTool_Camera*		m_pTool_Camera;
	CTool_Ui*			m_pTool_Ui;
	CTool_Map*			m_pTool_Map;
	CTool_Particle*		m_pTool_Particle;
	CTool_Effect*		m_pTool_Effect;
	CTool_Animation*	m_pTool_Animation;

	CPopupTool_ObjectTree*	m_pPopupTool_ObjectTree;
	CPopupTool_TextureTree*	m_pPopupTool_TextureTree;
	CPopup_LightOption*		m_pPopup_LightOption;
	CPopup_EffectBase*		m_pPopup_EffectBase;
	CPopup_EffectFilter*	m_pPopup_EffectFilter;

public:
	void		Tool_Update(NMHDR *pNMHDR);
	void		Tool_Release();
	_bool		Get_CurTab(eToolMenu eTab);
	void		Popup_TextureTool();

	CTabCtrl m_tTabCtrl;
	afx_msg void MH_On_Selchange_Menutab(NMHDR *pNMHDR, LRESULT *pResult);
};


#endif // !__MENUFORM_H__