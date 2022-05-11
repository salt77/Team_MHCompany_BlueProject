#pragma once
#include "afxcolorbutton.h"
#include "afxcmn.h"
#ifndef __POPUPTOOL_LIGHTOPTION_H__
#define __POPUPTOOL_LIGHTOPTION_H__

// CPopup_LightOption 대화 상자입니다.

class CPopup_LightOption : public CDialogEx
{
	DECLARE_DYNAMIC(CPopup_LightOption)

public:
	CPopup_LightOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopup_LightOption();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EH_POPUP_LIGHTOPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void Set_Render();
	void Get_Render();


public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	afx_msg void OnBnClicked_Eh_Apply();
	afx_msg void OnBnClicked_Eh_Color_Button();
	afx_msg void OnBnClicked_Eh_Color_Vector();
	afx_msg void OnBnClicked_Eh_Fogcolor_Button();
	afx_msg void OnBnClicked_Eh_Fogcolor_Vector();
	afx_msg void OnBnClicked_Eh_Cancel();
	afx_msg void OnBnClicked_Eh_RenderoptionReset();

	afx_msg void OnNMCustomdraw_Lightoptionview_LumSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw_Lightoptionview_MidgraySlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw_Lightoptionview_WhitecutoffSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw_Lightoptionview_BrightfilterSlider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClicked_Eh_RenderoptionUpdate();
	afx_msg void OnNMCustomdraw_Eh_LightDisSlider(NMHDR *pNMHDR, LRESULT *pResult);

private:
	_uint TIMERID = 7700;
	
private:
	float m_fLightPos_X;
	float m_fLightPos_Y;
	float m_fLightPos_Z;
	float m_fLightDir_X;
	float m_fLightDir_Y;
	float m_fLightDir_Z;

	CMFCColorButton m_vColor_Diffuse;
	float m_f_D_R;
	float m_f_D_G;
	float m_f_D_B;
	float m_f_D_A;

	CMFCColorButton m_vColor_Specular;
	float m_f_S_R;
	float m_f_S_G;
	float m_f_S_B;
	float m_f_S_A;

	CMFCColorButton m_vColor_Ambient;
	float m_f_A_R;
	float m_f_A_G;
	float m_f_A_B;
	float m_f_A_A;

	float m_fFogStart;
	float m_fFogEnd;

	CMFCColorButton m_vColor_Fog;
	float m_fFogColor_R;
	float m_fFogColor_G;
	float m_fFogColor_B;
	float m_fFogColor_A;

	float m_fBufferColor_R;
	float m_fBufferColor_G;
	float m_fBufferColor_B;

	BOOL m_bHDR;
	BOOL m_bBloom;
	float m_fLum;
	float m_fMiddleGray;
	float m_fBrightFilter;
	float m_fWhiteCutOff;

	CSliderCtrl m_Slider_Lum;
	CSliderCtrl m_Slider_MidGray;
	CSliderCtrl m_Slider_WhiteCutOff;
	CSliderCtrl m_Slider_BrightFilter;

	_float	m_fLightDis;
	_int	m_iLightDis = 300;
	CSliderCtrl m_fLightDis_Slider;

public:
};

#endif // !__POPUPTOOL_LIGHTOPTION_H__