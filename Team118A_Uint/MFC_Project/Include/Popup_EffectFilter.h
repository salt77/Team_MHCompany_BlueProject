#pragma once
#include "afxwin.h"
#ifndef __POPUPTOOL_EFFECTFILTER_H__
#define __POPUPTOOL_EFFECTFILTER_H__

// CPopup_EffectFilter 대화 상자입니다.

class CPopup_EffectFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CPopup_EffectFilter)

public:
	CPopup_EffectFilter(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopup_EffectFilter();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EH_1226_POPUP_EFFECTFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	void		Set_FilterIndex(_int iIndex) { m_iFilterIndex = iIndex; }
	void		Change_Picture();

private:
	void		Change_Picture_EffectFilter(CString* pTexPath, CStatic* pPicture);

	void		ReLoad();
	void		Load_ResourceData(CString pPath, CListBox* pListBox);
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnBnClicked_Eh_Button_FilterType();
	afx_msg void OnBnClicked_Eh_Delete();
	afx_msg void OnCbnSelchange_Eh_Filter_Combobox();
	afx_msg void OnLbnSelchange_Eh_Filter_Listbox();
	afx_msg void OnBnClicked_Eh_Upadte();

	afx_msg void OnBnClickedOk();

private:
	_int		m_iFilterIndex = 0;
	_int		m_iTypeIndex = 0;
	CString		m_pathFilterTex;

private:
	CString m_pPath_FilterImage;
	CStatic m_pictureFilter;
	CComboBox m_comboBoxType;
	CListBox m_listBoxFilter;
};

#endif // !__POPUPTOOL_EFFECTFILTER_H__