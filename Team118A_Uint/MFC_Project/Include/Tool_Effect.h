#pragma once
#ifndef __TOOL_EFFECT_H__
#define __TOOL_EFFECT_H__

#include "PopupTool_ObjectTree.h"
#include "afxwin.h"
// CTool_Effect 대화 상자입니다.

#include "EffectObject.h"
#include "afxcolorbutton.h"
//class CEffectObject;

class CTool_Effect : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Effect)

public:
	CTool_Effect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Effect(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Effect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_EFFECT};
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void	Apply_Tex_EffectBase(T_EffectTexture tEffectTexture);

private:
	void	ReUpdate_Tool();
	void	DrawPicture(HWND hDestWindowOverride, CString& strTextureName, _bool bFilter = TRUE, _bool bBlakcOption = FALSE);
	void	Set_Info();
	void	Get_Info();
	void	Reset_Info();

	void	LoadEffectInfo(CString pPath, T_EffectInfo* pInfo, _bool bFullPath = false);
	void	SaveEffectInfo(CString pPath, _bool bFullPath = false);
	void	Edit_Effect(CString pPath, _bool bFullPath = false);

	void	LoadEffectList();
	void	LoadParticleList();
	void	LoadSkillList();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual BOOL OnInitDialog();

private:
	afx_msg void OnBnClicked_Eh_Open_Light();
	afx_msg void OnBnClicked_Eh_Create();
	afx_msg void OnBnClicked_Eh_Apply();
	
	afx_msg void OnBnClicked_Eh_ColorStart_Vec();
	afx_msg void OnBnClicked_Eh_ColorStart_Button();

	afx_msg void OnBnClicked_Eh_ColorEnd_Vec();
	afx_msg void OnBnClicked_Eh_ColorEnd_Button();

	afx_msg void OnBnClicked_Eh_Reset();
	afx_msg void OnBnClicked_Eh_Base2move();
	afx_msg void OnBnClicked_Eh_ReUpdate();
	afx_msg void OnCbnSelchange_Eh_Type();
	
	afx_msg void OnBnClicked_Eh_Save();
	afx_msg void OnBnClicked_Eh_Load();
	afx_msg void OnBnClicked_Eh_SaveFix();
	
	afx_msg void OnBnClicked_Eh_Open_EffectTexture();

	afx_msg void OnBnClicked_Eh_LoadEffectList();
	afx_msg void OnBnClicked_Eh_LoadParticleList();

	afx_msg void OnLbnSelchange_Eh_ListBox_EffectLoad();
	afx_msg void OnLbnSelchange_Eh_ListBox_ParticleLoad();

	afx_msg void OnBnClicked_Eh_PlayList_Effect();
	afx_msg void OnBnClicked_Eh_PlayList_Particle();
	afx_msg void OnBnClicked_Eh_AddSkill_Effect();
	afx_msg void OnBnClicked_Eh_AddSkill_Particle();

	afx_msg void OnBnClicked_Eh_SkillList_Save();
	afx_msg void OnBnClicked_Eh_SkillList_Load();
	afx_msg void OnBnClicked_Eh_SkillList_Play();
	
	afx_msg void OnBnClicked_Eh_Skill_List_Effect_DeleteAll();
	afx_msg void OnBnClicked_Eh_Skill_List_Effect_Delete();
	afx_msg void OnBnClicked_Eh_Skill_List_Particle_DeleteAll();
	afx_msg void OnBnClicked_Eh_Skill_List_Particle_Delete();

	afx_msg void OnBnClicked_Eh_Skill_List_Update();
	afx_msg void OnBnClicked_Eh_Skill_List_DeleteAll();
	afx_msg void OnLbnSelchangeOn_Eh_Listbox_Skill_Save();
	afx_msg void OnBnClicked_Eh_TestSkillCreate();

	afx_msg void OnBnClicked_Eh_BlackAplha();
	afx_msg void OnBnClicked_Eh_BlackWhite();
	
	afx_msg void OnBnClicked_Eh_Reset_Image();

	afx_msg void OnBnClicked_Eh_DeleteBase();
	afx_msg void OnBnClicked_Eh_DeleteFilter1();
	afx_msg void OnBnClicked_Eh_DeleteFilter2();
	afx_msg void OnBnClicked_Eh_DeleteFilter3();

	afx_msg void OnBnClicked_Eh_Skilllist_EffectUp();
	afx_msg void OnBnClicked_Eh_Skilllist_EffectDown();
	afx_msg void OnBnClicked_Eh_Search();
	afx_msg void OnBnClicked_Eh_SearchReset();

private:
	_uint			TIMERID = 7800;
	T_EffectInfo	m_tEffectInfo;
	
	CString			m_pPathEffect = L"None";;
	CString			m_pPathParticle = L"None";;

	_float			m_fEffectSaveSelTime = 0.f;
	_int			m_iEffectSaveSel = -1;

private:
	CStatic m_pictureEffect_Base;
	CStatic m_pictureEffect_Filter_1;
	CStatic m_pictureEffect_Filter_2;
	CStatic m_pictureEffect_Filter_3;

	float m_fDelayTime;
	float m_fLifeTime;

	CComboBox m_eEffectLoopType;
	float m_fSpeedCount;

	float m_fU_Start;
	float m_fV_Start;
	float m_fU_End;
	float m_fV_End;
	CString m_EffectName;

	float m_fPos_Start_X;
	float m_fPos_Start_Y;
	float m_fPos_Start_Z;
	float m_fPos_End_X;
	float m_fPos_End_Y;
	float m_fPos_End_Z;

	float m_fRot_Start_X;
	float m_fRot_Start_Y;
	float m_fRot_Start_Z;
	float m_fRot_End_X;
	float m_fRot_End_Y;
	float m_fRot_End_Z;

	float m_fScale_Start_X;
	float m_fScale_Start_Y;
	float m_fScale_Start_Z;
	float m_fScale_End_X;
	float m_fScale_End_Y;
	float m_fScale_End_Z;

	CMFCColorButton m_vColor_Start_Button;
	float m_fColor_Start_R;
	float m_fColor_Start_G;
	float m_fColor_Start_B;
	float m_fColor_Start_A;

	CMFCColorButton m_vColor_End_Button;
	float m_fColor_End_R;
	float m_fColor_End_G;
	float m_fColor_End_B;
	float m_fColor_End_A;

	CListBox m_listBoxEffectSave;
	CListBox m_listBoxParticleSave;
	CListBox m_listBox_Skill_Effect;
	CListBox m_listBox_Skill_Particle;
	CListBox m_listBox_Skill;

	CString m_testSkill = L"None";

	BOOL m_bBlackAlpha;
	BOOL m_bBlackWhite;

	BOOL m_bBlur;
	BOOL m_bBillboard;
	BOOL m_bZenable;

	CComboBox m_comboAlphaOption;
	CComboBox m_comboCull;

	// Filter_1 
	float m_fBegin_Filter_1;
	float m_fEnd_Filter_1;
	float m_fMin_Filter_1;
	float m_fMax_Filter_1;

	float m_fB_Filter_1;
	float m_fR_Filter_1;
	float m_fG_Filter_1;

	BOOL m_bReserve_Filter_1;

	// Filter_2 
	float m_fBegin_Filter_2;
	float m_fEnd_Filter_2;
	float m_fMin_Filter_2;
	float m_fMax_Filter_2;

	float m_fB_Filter_2;
	float m_fR_Filter_2;
	float m_fG_Filter_2;

	BOOL m_bReserve_Filter_2;

	// Filter_4 칼러 필터
	float m_fR_Filter_4;
	float m_fG_Filter_4;
	float m_fB_Filter_4;
	float m_fA_Filter_4;
	float m_fU_Filter_4;
	float m_fV_Filter_4;

	CString m_strSearch;

	_bool m_bSearch = FALSE;

public:
};
#endif // !__TOOL_EFFECT_H__