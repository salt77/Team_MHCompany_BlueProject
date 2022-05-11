#pragma once
#ifndef __TOOL_PARTICLE_H__
#define __TOOL_PARTICLE_H__


#include "PopupTool_ObjectTree.h"
#include "afxwin.h"
#include "afxcolorbutton.h"
// CTool_Particle 대화 상자입니다.

class CTool_Particle : public CDialogEx
{
	DECLARE_DYNAMIC(CTool_Particle)

public:
	CTool_Particle(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CTool_Particle(CPopupTool_ObjectTree* pPopup, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTool_Particle();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MH_TOOL_PARTICLE };
#endif

protected:
	CPopupTool_ObjectTree*	m_pPopupTool = nullptr;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMwButtonAddSample();
	afx_msg void OnBnClickedMwButtonDeleteSample();
	afx_msg void OnBnClickedMwDeleteSpawner();
	afx_msg void OnBnClickedMwAddSpawner();
	afx_msg void OnBnClickedMwDeleteTexture();
	afx_msg void OnBnClickedMwButtonAddTexture();
	afx_msg void OnBnClickedMwButtonSaveParticle();
	afx_msg void OnBnClickedMwLoadParticle();
public:
	virtual BOOL OnInitDialog();

	void	DataReLoad();
	void	WaitSpanwerReLoad();
	void	ViewRender(_float fDeltaTime);
	void	TestSave();
	void	TestLoad();
	void	DebugRender();
	
	
public:
	wstring m_wstrTexName;
	wstring m_wstrTexPath;

private:
	CListBox m_cListParticleSample;
	CListBox m_cListWaitSpawner;
	CListBox m_cListTexture;

	CString m_strSampleAddMinPosX;
	CString m_strSampleAddMinPosY;
	CString m_strSampleAddMinPosZ;

	CString m_strSampleAddMaxPosX;
	CString m_strSampleAddMaxPosY;
	CString m_strSampleAddMaxPosZ;

	CString m_strSampleAddMinRotX;
	CString m_strSampleAddMinRotY;
	CString m_strSampleAddMinRotZ;

	CString m_strSampleAddMaxRotX;
	CString m_strSampleAddMaxRotY;
	CString m_strSampleAddMaxRotZ;


	CString m_strSampleScaleX;
	CString m_strSampleScaleY;
	CString m_strSampleScaleZ;

	CString m_strSampleAddMinScaleX;
	CString m_strSampleAddMinScaleY;
	CString m_strSampleAddMinScaleZ;

	CString m_strSampleColorR;
	CString m_strSampleColorG;
	CString m_strSampleColorB;
	CString m_strSampleColorA;

	CString m_strSampleAddMinColorR;
	CString m_strSampleAddMinColorG;
	CString m_strSampleAddMinColorB;
	CString m_strSampleAddMinColorA;

	CString m_strSampleAddMaxColorR;
	CString m_strSampleAddMaxColorG;
	CString m_strSampleAddMaxColorB;
	CString m_strSampleAddMaxColorA;

	CString m_strSampleLifeTime;

	CString m_strSampleGravity;
	CString m_strSampleMaxGravity;
	// Spawner



	CString m_strSpawnerPosX;
	CString m_strSpawnerPosY;
	CString m_strSpawnerPosZ;

	CString m_strSpawnerAddMinPosX;
	CString m_strSpawnerAddMinPosY;
	CString m_strSpawnerAddMinPosZ;
	CString m_strSpawnerAddMaxPosX;
	CString m_strSpawnerAddMaxPosY;
	CString m_strSpawnerAddMaxPosZ;

	CString m_strSpawnerRotX;
	CString m_strSpawnerRotY;
	CString m_strSpawnerRotZ;

	CString m_strSpawnerAddMinRotX;
	CString m_strSpawnerAddMinRotY;
	CString m_strSpawnerAddMinRotZ;
	CString m_strSpawnerAddMaxRotX;
	CString m_strSpawnerAddMaxRotY;
	CString m_strSpawnerAddMaxRotZ;

	CString m_strSpawnerSpawnMinPosX;
	CString m_strSpawnerSpawnMinPosY;
	CString m_strSpawnerSpawnMinPosZ;

	CString m_strSpawnerSpawnMaxPosX;
	CString m_strSpawnerSpawnMaxPosY;
	CString m_strSpawnerSpawnMaxPosZ;

	CString m_strSpawnerLifeTime;
	CString m_strSpawnerEntranceTime;

	//Particle Object
	CString m_strParticleObjectLifeTime;

	CStatic m_cPicture;

private:
	_bool			 m_bPasteSample = FALSE;
	_bool			 m_bPasteSpawner = FALSE;
	CParticleObject* m_pParticleObject = nullptr;
	CParticleObject* m_pViewParticle = nullptr;

	_int			 m_iSampleSelect = -1;
	_int			 m_iSpawnerSelect = -1;
	_int			 m_iTextureSelect = -1;

	LPD3DXMESH m_pDebugMesh = nullptr;
	CShader*	m_pShaderCom = nullptr;

	T_PARTICLE_SPAWNER m_tCopyParticleSpawner;
	T_PARTICLE m_tCopyParticleSample;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLbnSelchangeMwSampleList();
	afx_msg void OnEnChangeMwAddMinPosx();
	afx_msg void OnEnChangeMwAddMinPosy();
	afx_msg void OnEnChangeMwAddMinPosz();
	afx_msg void OnEnChangeMwAddMaxPosx();
	afx_msg void OnEnChangeMwAddMaxPosy();
	afx_msg void OnEnChangeMwAddMaxPosz();
	afx_msg void OnEnChangeMwAddMinRotx();
	afx_msg void OnEnChangeMwAddMinRoty();
	afx_msg void OnEnChangeMwAddMinRotz();
	afx_msg void OnEnChangeMwAddMaxRotx();
	afx_msg void OnEnChangeMwAddMaxRoty();
	afx_msg void OnEnChangeMwAddMaxRotz();
	afx_msg void OnEnChangeMwScalex();
	afx_msg void OnEnChangeMwScaley();
	afx_msg void OnEnChangeMwScalez();
	afx_msg void OnEnChangeMwAddMinScalex();
	afx_msg void OnEnChangeMwAddMinScaley();
	afx_msg void OnEnChangeMwAddMinScalez();
	afx_msg void OnEnChangeMwEditSampleA();
	afx_msg void OnEnChangeMwEditSampleAddMinColorA();
	afx_msg void OnEnChangeMwEditSampleLifeTime();
	afx_msg void OnEnChangeMwEditSpawnerPosZ();
	afx_msg void OnEnChangeMwEditSpawnerPosY();
	afx_msg void OnEnChangeMwEditSpawnerPosX();
	afx_msg void OnEnChangeMwEditSpawnerRotX();
	afx_msg void OnEnChangeMwEditSpawnerRotY();
	afx_msg void OnEnChangeMwEditSpawnerRotZ();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMinPosX();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMinPosY();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMinPosZ();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMaxPosX();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMaxPosY();
	afx_msg void OnEnChangeMwEditSpawnerSpawnMaxPosZ();
	afx_msg void OnEnChangeMwEditSpawnerLifeTime();
	afx_msg void OnEnChangeMwEditSpawnerEntranceTime();
	afx_msg void OnEnChangeMwEditParticleObjectLifeTime();
	afx_msg void OnEnChangeMw1216SpawnerAddMinPosx();
	afx_msg void OnEnChangeMw1216SpawnerAddMinPosY();
	afx_msg void OnEnChangeMw1216SpawnerAddMinPosZ();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxPosX();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxPosY();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxPosZ();
	afx_msg void OnEnChangeMw1216SpawnerAddMinRotX();
	afx_msg void OnEnChangeMw1216SpawnerAddMinRotY();
	afx_msg void OnEnChangeMw1216SpawnerAddMinRotZ();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxRotX();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxRotY();
	afx_msg void OnEnChangeMw1216SpawnerAddMaxRotZ();

	afx_msg void OnLbnSelchangeMwWaitSpawnerList();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
	CString m_strSpawnerSpawnInterval;
	CString m_strSpawnerSampleIndex;
public:
	afx_msg void OnEnChangeMw1217EditSpawnerSpawnInterval();
	afx_msg void OnEnChangeMw1217EditSpawnerSampleIndex();
	afx_msg void OnLbnSelchangeMwTextureList();
private:
	CString m_strSampleTexIndex;
public:
	afx_msg void OnEnChangeMw1217EditSampleIndex();
private:
	CComboBox m_cComboSampleType;
	CComboBox m_cComboSpawnerType;
public:
	afx_msg void OnCbnSelchangeMwComboSpawnerSystem();
	afx_msg void OnCbnSelchangeMwSampleSystemCombo();

public:
	afx_msg void OnEnChangeMw1219EditGravity();
	afx_msg void OnEnChangeMw1219EditMaxGravity();
private:
	BOOL m_bSampleBillBoard;
public:
	afx_msg void OnBnClickedMw1219CheckSampleBillboard();
private:
	BOOL m_bSpawnerMultiSpawn;
	CString m_strSpawnerMultiStartPosX;
	CString m_strSpawnerMultiStartPosY;
	CString m_strSpawnerMultiStartPosZ;
	CString m_strSpawnerMultiStartRotX;
	CString m_strSpawnerMultiStartRotY;
	CString m_strSpawnerMultiStartRotZ;
	CString m_strSpawnerMultiAddPosX;
	CString m_strSpawnerMultiAddPosY;
	CString m_strSpawnerMultiAddPosZ;
	CString m_strSpawnerMultiAddRotX;
	CString m_strSpawnerMultiAddRotY;
	CString m_strSpawnerMultiAddRotZ;
	CString m_strSpawnerMultiCount;
public:
	afx_msg void OnBnClickedMw1220CehckMultiple();
	afx_msg void OnEnChangeMw1220EditMultiCount();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartPosX();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartPosY();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartPosZ();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartRotX();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartRotY();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiStartRotZ();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddPosX();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddPosY();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddPosZ();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddRotX();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddRotY();
	afx_msg void OnEnChangeMw1220EditSpawnerMultiAddRotZ();
	afx_msg void OnEnChangeMw0107EditParticleObjectScale();
private:
	CString m_strParticleObjectScale;
public:
	afx_msg void OnBnClickedMw0107ButtonSpawnerCopy();
	afx_msg void OnBnClickedMw0107PasteSpawner();
	afx_msg void OnBnClickedMw0107ButtonCopySample();
	afx_msg void OnBnClickedMw0107ButtonPasteSample();
private:

public:
	afx_msg void OnBnClickedMw0107ColorStart();
	afx_msg void OnBnClickedMw0107ColorEnd();
private:
	CMFCColorButton m_cColorStart;
	CMFCColorButton m_cColorEnd;
	CString m_strParticleObjectRotX;
	CString m_strParticleObjectRotY;
	CString m_strParticleObjectRotZ;
public:
	afx_msg void OnEnChangeMw0107EditParticleObjectRotX();
	afx_msg void OnEnChangeMw0107EditParticleObjectRotY();
	afx_msg void OnEnChangeMw0107EditParticleObjectRotZ();
private:
	BOOL m_bIndependent;
public:
	afx_msg void OnBnClickedMw0107CheckIndependent();
private:
	BOOL m_bBlackBackGroundImage;
public:
	afx_msg void OnBnClickedMw0114CheckBlackImage();
private:
	CString m_strSpawnMinRotX;
	CString m_strSpawnMinRotY;
	CString m_strSpawnMinRotZ;
	CString m_strSpawnMaxRotX;
	CString m_strSpawnMaxRotY;
	CString m_strSpawnMaxRotZ;
public:
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMinRotX();
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMinRotY();
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMinRotZ();
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMaxRot();
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMaxRotY();
	afx_msg void OnEnChangeMw0119EditSpawnerSpawnMaxRotZ();
private:
	CString m_strSampleMaxScaleX;
	CString m_strSampleMaxScaleY;
	CString m_strSampleMaxScaleZ;
public:
	afx_msg void OnEnChangeMw0119MaxScalex();
	afx_msg void OnEnChangeMw0119MaxScaley();
	afx_msg void OnEnChangeMw0119MaxScalez();
private:
	CString m_strSampleOffsetRotX;
	CString m_strSampleOffsetRotY;
	CString m_strSampleOffsetRotZ;
public:
	afx_msg void OnEnChangeMw0119EditOffsetRotX();
	afx_msg void OnEnChangeMw0119EditOffsetRotY();
	afx_msg void OnEnChangeMw0119EditOffsetRotZ();
private:
	CString m_strBillBoardZRot;
public:
	afx_msg void OnEnChangeMw0119EditBillboardZRot();
private:
	CString m_strSampleMaxAlpha;
public:
	afx_msg void OnEnChangeMw0119EditMaxAlpha();
private:
	CMFCColorButton m_cColorStartMax;
public:
	afx_msg void OnBnClickedMw0119ColorStartMax();
private:
	BOOL m_bUseCross;
	BOOL m_bBillBoardLookDir;
public:
	afx_msg void OnBnClickedMw0119CheckUseCross();
	afx_msg void OnBnClickedMw0119CheckBillboardLookDir();

private:
	int m_iRadioAlphaOption;
public:
	afx_msg void OnBnClickedMw0120RadioAlphaTrans();
	afx_msg void OnBnClickedMw0120RadioAlphaBlend();
	afx_msg void OnBnClickedMw0120RadioNonAlpha();
private:
	BOOL m_bSampleZEnable;
	BOOL m_bSampleBlur;
public:
	afx_msg void OnBnClickedMw0120CheckBlur();
	afx_msg void OnBnClickedMw0120CheckZenable();
private:
	CString m_strSpriteX;
	CString m_strSpriteY;
	CString m_strCycleNum;
public:
	afx_msg void OnEnChangeMw0120EditSpriteX();
	afx_msg void OnEnChangeMw0120EditSpriteY();
	afx_msg void OnEnChangeMw0120EditSpriteCycle();
private:
	CString m_strSampleLifeTimeMax;
public:
	afx_msg void OnEnChangeMw0121EditSampleLifeTimeMax();
};
#endif // !__TOOL_PARTICLE_H__