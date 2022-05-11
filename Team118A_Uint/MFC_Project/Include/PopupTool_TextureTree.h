#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "MFC_Line.h"

// CPopupTool_TextureTree 대화 상자입니다.

class CPopupTool_TextureTree : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupTool_TextureTree)

public:
	CPopupTool_TextureTree(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopupTool_TextureTree();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_POPUPTOOL_TEXTURETREE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	void			Load_Texture_Path(_tchar* wPath, intptr_t lhandle, _finddata_t& findData);
	HTREEITEM		InsertTree(HTREEITEM hParent, _tchar* wText, TV_INSERTSTRUCTW& ti, CTreeCtrl& treeCtrl);
	afx_msg void	SelChange_TextureTree(NMHDR *pNMHDR, LRESULT *pResult);
	void			Render_Picture_Control(const CStatic& m_PictureControl, CTexture* pTex);
	
public:
	CTreeCtrl			m_tTreeCtrl_Texture;
	TV_INSERTSTRUCTW	TI_Tex;
	HTREEITEM			hLastTreeItem;
	HTREEITEM			hTempItem;
	CStatic				m_Texture_PictureControl;

	wstring				m_wstrPickTexturePath = L"";
	_bool				m_bClick = FALSE;
	_bool				m_bCreateU = FALSE;
	_bool				m_bCreateV = FALSE;

	_vec2				m_vMinUV = _vec2(0.f, 0.f);
	_vec2				m_vMaxUV = _vec2(1.f, 1.f);
	afx_msg void Button_Click_PictureControl();
	afx_msg void Button_Create_New_UV();
	void		Render_PictureControl();
	afx_msg void Button_UV_Set_Max();
};
