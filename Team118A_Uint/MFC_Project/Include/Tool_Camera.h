#pragma once
#ifndef __TOOL_CAMERA_H__
#define __TOOL_CAMERA_H__

#include "PopupTool_ObjectTree.h"
#include "afxcmn.h"
#include "afxwin.h"
// CTool_Camera 대화 상자입니다.

//#define LINESIZE 50

class CMFC_CamControl_Eye;
class CMFC_CamControl_At;
class CMFC_Line;
class CStageMap;

class CTool_Camera final : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Camera)

private:
	enum ActionType
	{
		Action_Normal, Action_Observe, Action_Subject,
		Action_End
	};

	enum ControlPointType
	{
		Control_EyePoint, Control_AtPoint,
		Control_End
	};

	enum DeployType
	{
		Deploy_Normal, Deploy_OneAt,
		Deploy_End
	};

public:
	CTool_Camera(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Camera(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Camera();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_CAMERA };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL	OnInitDialog();
	afx_msg void	OnTimer(UINT_PTR nIDEvent);

public:
	void			Click_Action();

private:
	void			Free();
	HRESULT			Ready_DefaultSettings();
	HRESULT			Ready_Line();
	void			Scene_Change();
	void			Mode_Change();

	void			Create_ControlSphere();
	void			Refresh_ControlListString();
	void			MoveOn_Line();
	void			Eye_Line();
	void			At_Line();
	void			Make_Spline(_vec3* vOutPos, const _vec3& vPos0, const _vec3& vPos1, const _vec3& vPos2, const _vec3& vPos3);

	void			Delete_All();
	void			Delete_MapClear();

private:
	afx_msg void	OnBnClickedCamAdd();
	afx_msg void	OnBnClickedCamDelete();
	afx_msg void	OnBnClickedControlPointDelete();
	afx_msg void	OnBnClickedControlPointPosApply();
	afx_msg void	OnEditChangePosX();
	afx_msg void	OnEditChangePosY();
	afx_msg void	OnEditChangePosZ();

	afx_msg void	OnSelchangeCamList();
	afx_msg void	OnSelchangeControlPointList_Eye();
	afx_msg void	OnSelchangeControlPointList_At();
	afx_msg void	OnCbnSelchangeCamType();
	afx_msg void	OnCbnSelchangeDeployType();

	afx_msg void	OnBnClickedSpeedApply();

	afx_msg void	OnBnClickedActionStart();

	afx_msg void	OnEnChangeCurPointSpeed();
	afx_msg void	OnEnChangeAllChange();
	afx_msg void	OnBnClickedButtonPickObjPosApply();
	afx_msg void	OnBnClickedJyCamCheckfreecam0122();
	afx_msg void	OnEnChangeJyCamMaincamspeed0122();

	// Save, Load 
	afx_msg void	OnBnClickedCamSceneLoad();

	afx_msg void	OnBnClickedCamToolSAVE();
	afx_msg void	OnBnClickedCamToolLOAD();

private:
	_bool				m_bCreateLine = FALSE;

	_int				m_iCamCurSel = -1;
	_int				m_iControlCurSel_Eye = -1;
	_int				m_iControlCurSel_At = -1;
	_int				m_iCurLineIndex = 1;

	_uint				m_iTimerEvent = 1;

	_float				m_fTimeDelta = 0.f;
	//_float				m_fSpeedRatio = 10.f;
	_float				m_fProgressCam = 0.f;
	//_float				m_fLineLength = 0.f;
	_float				m_fTime = 3.f;

	_vec3				m_vLinePos = VEC_ZERO;
	_vec3				m_vLineAtPos = VEC_ZERO;

	ControlPointType	m_eControlPointType = Control_End;

	ActionType			m_eCamActionType = Action_End;
	ActionType			m_ePreCamActionType = Action_End;

	E_SCENETAG			m_eCurScene = E_SCENETAG::Scene_END;
	E_SCENETAG			m_ePreScene = E_SCENETAG::Scene_END;

	DeployType			m_eDeployType = Deploy_End;
	DeployType			m_ePreDeployType = Deploy_End;

	CString				m_cstrCamCurText = L"";

	map<const wstring, vector<_float>>					m_mapSectionSpeed;
	map<const wstring, DeployType>						m_mapDeployType;
	map<const wstring, vector<CMFC_CamControl_Eye*>>	m_mapActionEye;
	map<const wstring, vector<CMFC_CamControl_At*>>		m_mapActionAt;

	////////////////////////////////////////////////////// 외부에서 가져온 변수들 
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	CDynamicCamera*		m_pMainCam = nullptr;
	CScene*				m_pScene = nullptr;
	CStageMap*			m_pMap = nullptr;
	CMFC_Line*			m_pEyeLine = nullptr;
	CMFC_Line*			m_pAtLine = nullptr;

	////////////////////////////////////////////////////// Tool 변수들 
	CString				m_cstrCamNameInsert;
	CString				m_cstrControlSpeed;

	_float				m_fPosX;
	_float				m_fPosY;
	_float				m_fPosZ;

	CEdit				m_EditPosX;
	CEdit				m_EditPosY;
	CEdit				m_EditPosZ;
	CEdit				m_EditSpeed;
	CEdit				m_EditCamName;

	CListBox			m_ListCam;
	CListBox			m_ListControlPoint_Eye;
	CListBox			m_ListControlPoint_At;

	CComboBox			m_ComboCamType;
	CComboBox			m_Combo_Deploy;

	CEdit				m_EditAllSpeed;
	CString				m_cstrAllSpeed;
	CButton				m_ButtonCheckFreeCam;
	_float				m_fMainCamSpeed;
	CEdit				m_EditMainCamSpeed;
};

#endif // !__TOOL_CAMERA_H__