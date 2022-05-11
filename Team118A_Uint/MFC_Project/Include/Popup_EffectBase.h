#pragma once
#include "afxwin.h"
#ifndef __POPUPTOOL_EFFECTBASE_H__
#define __POPUPTOOL_EFFECTBASE_H__


// CPopup_EffectBase 대화 상자입니다.

class CPopup_EffectBase : public CDialogEx
{
	DECLARE_DYNAMIC(CPopup_EffectBase)

public:
	CPopup_EffectBase(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopup_EffectBase();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EH_POPUP_EFFECTBASE };
#endif

public:
	void	Change_Picture();
	void	Set_TexPath(T_EffectInfo tInfo);

	virtual BOOL OnInitDialog();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	void	Load_ResourceData(CString pPath, CListBox* pListBox);

private:
	void	Change_Picture_EffectBase(CString* pTexPath, CStatic* pPicture, CString* pMeshPath = nullptr, _bool bSprtiteDraw = FALSE);

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
private:
	afx_msg void OnBnClicked_Eh_Load_Resource();
	afx_msg void OnLbnSelchange_Eh_Listbox_Effectbase();


	afx_msg void OnBnClicked_Eh_Load_EMesh();
	afx_msg void OnLbnSelchange_Eh_Listbox_Emesh();
	afx_msg void OnBnClicked_Eh_EMesh_Rotation_1();
	afx_msg void OnBnClicked_Eh_EMesh_Rotation_Reset();
	afx_msg void OnBnClicked_Eh_EMesh_Rotation_2();

	afx_msg void OnBnClicked_Eh_EMesh_Scale_1();
	afx_msg void OnBnClicked_Eh_EMesh_Scale_Reset();
	afx_msg void OnBnClicked_Eh_EMesh_Scale_2();

	afx_msg void OnBnClicked_Eh_CheckBox_UVTex();
	afx_msg void OnCbnSelchange_Eh_TextureFoler();
	afx_msg void OnBnClicked_Eh_Play();
	afx_msg void OnBnClicked_Eh_Update();

	afx_msg void OnBnClicked_Eh_BlackWhite();
	afx_msg void OnBnClicked_Eh_BlackAlpha();

	afx_msg void OnBnClicked_EH_Set_CheckBox_IsSprite();

	afx_msg void OnBnClicked_Eh_SetBase();
	afx_msg void OnBnClicked_Eh_SetFilter_1();
	afx_msg void OnBnClicked_Eh_SetFilter_2();
	afx_msg void OnBnClicked_Eh_SetFilter_3();
	afx_msg void OnBnClicked_Eh_SetEMesh();

private:
	T_EffectTexture m_tEffectTexture;

	_float			m_fMeshAngle = 0.f;
	_float			m_fMeshScale = 1.f;
	CString			m_pathDefualtTex;
	_bool			m_bPlaySprite = FALSE;
	_float			m_fSpriteTimeAcc = 0.f;

	CString			m_pathBaseTex;

	_uint			TIMERID = 7900;
	
private:
	CStatic			m_pictureEffectBase;
	CListBox		m_listBox_EffectBase;


	// 스프라이트
	CStatic			m_pictureSpriteView;

	_int			m_iTextureFolder = 0;
	CComboBox		m_comboBox_TextureFolder;

	_int			m_iSpriteIndex;
	_float			m_fSpriteTime;
	_int			m_iSprite_Row;
	_int			m_iSprite_Column;

	// EMesh
	CStatic			m_pictureEMeshView;
	CListBox		m_listBox_EMesh;
	
	BOOL			m_bCheckBox_UVTex;
	BOOL			m_bBlackWhite;
	BOOL			m_bBlackAlpha;

	BOOL			m_bIsSprite;
	CString			m_textBaseTexturePath;

	CString			m_pathView_Base;
	CString			m_pathView_Filter_1;
	CString			m_pathView_Filter_2;
	CString			m_pathView_Filter_4;

public:
};

#endif // !__POPUPTOOL_EFFECTBASE_H__