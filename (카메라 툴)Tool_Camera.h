#pragma once

#ifndef __Tool_Camera_H__
#define __Tool_Camera_H__

#include "PopupTool_ObjectTree.h"
#include "afxcmn.h"
#include "afxwin.h"

// 헤더 파일에서 사용될 클래스 전방 선언 
class CMFC_CamControl_Eye;
class CMFC_CamControl_At;
class CMFC_Line;
class CStageMap;

// 더 이상 상속 X. final 선언 
class CTool_Camera final : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Camera)

private:		// 해당 클래스에서만 사용할 enum들 
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
	CTool_Camera(CWnd* pParent = NULL);   
	CTool_Camera(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);  
	virtual ~CTool_Camera();

	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_CAMERA };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:				// 툴 작동을 위한 기본 함수 
	virtual BOOL	OnInitDialog();
	afx_msg void	OnTimer(UINT_PTR nIDEvent);

public:				// 툴 동작 함수들 
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

private:			// MFC 버튼이나 이벤트 관련 함수들 
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
	_float				m_fProgressCam = 0.f;
	_float				m_fTime = 3.f;
	
	// _vec3  = #define D3DXVECTOR3
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

#endif // !__Tool_Camera_H__