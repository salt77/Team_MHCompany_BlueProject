// Popup_LightOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "Popup_LightOption.h"
#include "afxdialogex.h"
#include "LightMgr.h"


// CPopup_LightOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopup_LightOption, CDialogEx)

CPopup_LightOption::CPopup_LightOption(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EH_POPUP_LIGHTOPTION, pParent)
	, m_fLightPos_X(0)
	, m_fLightPos_Y(0)
	, m_fLightPos_Z(0)
	, m_fLightDir_X(0)
	, m_fLightDir_Y(0)
	, m_fLightDir_Z(0)
	, m_f_D_R(0)
	, m_f_D_G(0)
	, m_f_D_B(0)
	, m_f_D_A(0)
	, m_f_S_R(0)
	, m_f_S_G(0)
	, m_f_S_B(0)
	, m_f_S_A(0)
	, m_f_A_R(0)
	, m_f_A_G(0)
	, m_f_A_B(0)
	, m_f_A_A(0)
	, m_fFogStart(0)
	, m_fFogEnd(0)
	, m_fFogColor_R(0)
	, m_fFogColor_G(0)
	, m_fFogColor_B(0)
	, m_fFogColor_A(0)
	, m_fBufferColor_R(0)
	, m_fBufferColor_G(0)
	, m_fBufferColor_B(0)
	, m_bHDR(TRUE)
	, m_bBloom(TRUE)
	, m_fLum(0.08f)
	, m_fMiddleGray(0.f)
	, m_fWhiteCutOff(0.f)
	, m_fBrightFilter(0.f)
	, m_fLightDis(0)
{

}

CPopup_LightOption::~CPopup_LightOption()
{
}

void CPopup_LightOption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightPos_X, m_fLightPos_X);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightPos_Y, m_fLightPos_Y);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightPos_Z, m_fLightPos_Z);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightDir_X, m_fLightDir_X);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightDir_Y, m_fLightDir_Y);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightDir_Z, m_fLightDir_Z);
	DDX_Control(pDX, IDC_EH_LightOptionView_Color_Diffuse, m_vColor_Diffuse);
	DDX_Control(pDX, IDC_EH_LightOptionView_Color_Specular, m_vColor_Specular);
	DDX_Control(pDX, IDC_EH_LightOptionView_Color_Ambient, m_vColor_Ambient);
	DDX_Text(pDX, IDC_EH_LightOptionView_D_R, m_f_D_R);
	DDX_Text(pDX, IDC_EH_LightOptionView_D_G, m_f_D_G);
	DDX_Text(pDX, IDC_EH_LightOptionView_D_B, m_f_D_B);
	DDX_Text(pDX, IDC_EH_LightOptionView_D_A, m_f_D_A);
	DDX_Text(pDX, IDC_EH_LightOptionView_S_R, m_f_S_R);
	DDX_Text(pDX, IDC_EH_LightOptionView_S_G, m_f_S_G);
	DDX_Text(pDX, IDC_EH_LightOptionView_S_B, m_f_S_B);
	DDX_Text(pDX, IDC_EH_LightOptionView_S_A, m_f_S_A);
	DDX_Text(pDX, IDC_EH_LightOptionView_A_R, m_f_A_R);
	DDX_Text(pDX, IDC_EH_LightOptionView_A_G, m_f_A_G);
	DDX_Text(pDX, IDC_EH_LightOptionView_A_B, m_f_A_B);
	DDX_Text(pDX, IDC_EH_LightOptionView_A_A, m_f_A_A);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightDir_X2, m_fFogStart);
	DDX_Text(pDX, IDC_EH_LightOptionView_LightDir_Y2, m_fFogEnd);
	DDX_Control(pDX, IDC_EH_LightOptionView_Color_Fog, m_vColor_Fog);
	DDX_Text(pDX, IDC_EH_LightOptionView_Fog_R, m_fFogColor_R);
	DDX_Text(pDX, IDC_EH_LightOptionView_Fog_G, m_fFogColor_G);
	DDX_Text(pDX, IDC_EH_LightOptionView_Fog_B, m_fFogColor_B);
	DDX_Text(pDX, IDC_EH_LightOptionView_Fog_A, m_fFogColor_A);
	DDX_Text(pDX, IDC_EH_LightOptionView_Backbuffer_R, m_fBufferColor_R);
	DDX_Text(pDX, IDC_EH_LightOptionView_Backbuffer_G, m_fBufferColor_G);
	DDX_Text(pDX, IDC_EH_LightOptionView_Backbuffer_B, m_fBufferColor_B);

	DDX_Check(pDX, IDC_EH_0120_LightOptionView_CheckBox_Bloom, m_bBloom);
	DDX_Check(pDX, IDC_EH_0120_LightOptionView_CheckBox_HDR, m_bHDR);

	DDX_Text(pDX, IDC_EH_0120_LightOptionView_RenderOption_Lum, m_fLum);
	DDX_Text(pDX, IDC_EH_0120_LightOptionView_RenderOption_MidGray, m_fMiddleGray);
	DDX_Text(pDX, IDC_EH_0120_LightOptionView_RenderOption_WhiteCutOff, m_fBrightFilter);
	DDX_Text(pDX, IDC_EH_0120_LightOptionView_RenderOption_BrightFilter, m_fWhiteCutOff);
	DDX_Control(pDX, IDC_EH_0120_LightOptionView_RenderOption_Lum_Slider, m_Slider_Lum);
	DDX_Control(pDX, IDC_EH_0120_LightOptionView_RenderOption_MidGray_Slider, m_Slider_MidGray);
	DDX_Control(pDX, IDC_EH_0120_LightOptionView_RenderOption_WhiteCutOff_Slider, m_Slider_BrightFilter);
	DDX_Control(pDX, IDC_EH_0120_LightOptionView_RenderOption_BrightFilter_Slider, m_Slider_WhiteCutOff);
	DDX_Text(pDX, IDC_EH_0125_LightOptionView_RenderOption_fLightDis, m_fLightDis);
	DDX_Control(pDX, IDC_EH_0125_LightOptionView_RenderOption_fLightDis_Slider, m_fLightDis_Slider);
}


BEGIN_MESSAGE_MAP(CPopup_LightOption, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EH_LightOptionView_TransformApply, &CPopup_LightOption::OnBnClicked_Eh_Apply)
	ON_BN_CLICKED(IDC_EH_LightOptionView_TransformApply_ColorButton, &CPopup_LightOption::OnBnClicked_Eh_Color_Button)
	ON_BN_CLICKED(IDC_EH_LightOptionView_TransformApply_ColorVector, &CPopup_LightOption::OnBnClicked_Eh_Color_Vector)
	ON_BN_CLICKED(IDC_EH_LightOptionView_TransformApply_FogColorButton, &CPopup_LightOption::OnBnClicked_Eh_Fogcolor_Button)
	ON_BN_CLICKED(IDC_EH_LightOptionView_TransformApply_FogColorVector, &CPopup_LightOption::OnBnClicked_Eh_Fogcolor_Vector)
	ON_BN_CLICKED(IDCANCEL, &CPopup_LightOption::OnBnClicked_Eh_Cancel)
	ON_BN_CLICKED(IDC_EH_0120_LightOptionView_RenderOption_Reset, &CPopup_LightOption::OnBnClicked_Eh_RenderoptionReset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EH_0120_LightOptionView_RenderOption_Lum_Slider, &CPopup_LightOption::OnNMCustomdraw_Lightoptionview_LumSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EH_0120_LightOptionView_RenderOption_MidGray_Slider, &CPopup_LightOption::OnNMCustomdraw_Lightoptionview_MidgraySlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EH_0120_LightOptionView_RenderOption_WhiteCutOff_Slider, &CPopup_LightOption::OnNMCustomdraw_Lightoptionview_BrightfilterSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EH_0120_LightOptionView_RenderOption_BrightFilter_Slider, &CPopup_LightOption::OnNMCustomdraw_Lightoptionview_WhitecutoffSlider)
	ON_BN_CLICKED(IDC_EH_0121_LightOptionView_RenderOption_Update, &CPopup_LightOption::OnBnClicked_Eh_RenderoptionUpdate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EH_0125_LightOptionView_RenderOption_fLightDis_Slider, &CPopup_LightOption::OnNMCustomdraw_Eh_LightDisSlider)
END_MESSAGE_MAP()


// CPopup_LightOption 메시지 처리기입니다.


void CPopup_LightOption::Set_Render()
{
	T_RenderOption tRenderOption = T_RenderOption();

	tRenderOption.bHDR = BOOL2bool(m_bHDR);
	tRenderOption.fLuminance = m_fLum;
	tRenderOption.fMiddleGray = m_fMiddleGray;
	tRenderOption.fWhiteCutoff = m_fWhiteCutOff;

	tRenderOption.bBloom = BOOL2bool(m_bBloom);
	tRenderOption.fBrightFilter = m_fBrightFilter;

	Management Set_RendererOption(tRenderOption);

	CLightMgr::GetInstance()->Set_LightDis(m_fLightDis);
}

void CPopup_LightOption::Get_Render()
{
	T_RenderOption tRenderOption = Management Get_RendererOption();

	m_bHDR = tRenderOption.bHDR;
	m_fLum = tRenderOption.fLuminance;
	m_fMiddleGray = tRenderOption.fMiddleGray;
	m_fWhiteCutOff = tRenderOption.fWhiteCutoff;

	m_bBloom = tRenderOption.bBloom;
	m_fBrightFilter = tRenderOption.fBrightFilter;

	m_Slider_Lum.SetPos((_int)(m_fLum * 1000.f));
	m_Slider_MidGray.SetPos((_int)(m_fMiddleGray * 100.f));
	m_Slider_WhiteCutOff.SetPos((_int)(m_fWhiteCutOff * 100.f));

	m_Slider_BrightFilter.SetPos((_int)(m_fBrightFilter * 100.f));

	// 빛 거리
	m_fLightDis = CLightMgr::GetInstance()->Get_LightDis();

	_int iLightDis = (_int)(m_fLightDis * 10.f) + m_iLightDis;
	m_fLightDis_Slider.SetPos(iLightDis);

	UpdateData(FALSE);
}

// 창이 켜지면 빛(+@)의 기본 정보들을 가져옴 
void CPopup_LightOption::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	D3DXCOLOR vBufferColor = Management Get_LightBackBufferColor();

	m_fBufferColor_R = vBufferColor.r;
	m_fBufferColor_G = vBufferColor.g;
	m_fBufferColor_B = vBufferColor.b;

	UpdateData(FALSE);

	if (!Management IsLightExist()) 
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}

	_vec3 vPos = Management Get_LightPos();
	_vec3 vDir = Management Get_LightDir();
	
	m_fLightPos_X = vPos.x;
	m_fLightPos_Y = vPos.y;
	m_fLightPos_Z = vPos.z;

	m_fLightDir_X = vDir.x;
	m_fLightDir_Y = vDir.y;
	m_fLightDir_Z = vDir.z;

	D3DXCOLOR vDiffuse = Management Get_LightDiffuse();
	D3DXCOLOR vSpecular = Management Get_LightSpecular();
	D3DXCOLOR vAmbient = Management Get_LightAmbient();
	
	m_vColor_Diffuse.SetColor(D3DCOLORTOCOLORREF(vDiffuse));
	m_vColor_Specular.SetColor(D3DCOLORTOCOLORREF(vSpecular));
	m_vColor_Ambient.SetColor(D3DCOLORTOCOLORREF(vAmbient));

	m_f_D_R = vDiffuse.r;
	m_f_D_G = vDiffuse.g;
	m_f_D_B = vDiffuse.b;
	m_f_D_A = vDiffuse.a;

	m_f_S_R = vSpecular.r;
	m_f_S_G = vSpecular.g;
	m_f_S_B = vSpecular.b;
	m_f_S_A = vSpecular.a;

	m_f_A_R = vAmbient.r;
	m_f_A_G = vAmbient.g;
	m_f_A_B = vAmbient.b;
	m_f_A_A = vAmbient.a;

	m_fFogStart = Management Get_FogStart();
	m_fFogEnd = Management Get_FogEnd();

	_vec4 vFogColor = Management Get_FogColor();

	D3DXCOLOR vColor = D3DXCOLOR(vFogColor.x, vFogColor.y, vFogColor.z, vFogColor.w);

	m_vColor_Fog.SetColor(D3DCOLORTOCOLORREF(vColor));

	m_fFogColor_R = vFogColor.x;
	m_fFogColor_G = vFogColor.y;
	m_fFogColor_B = vFogColor.z;
	m_fFogColor_A = vFogColor.w;
	
	// Slider
	m_Slider_Lum.SetRange(0,1000);
	m_Slider_MidGray.SetRange(0, 100);
	m_Slider_WhiteCutOff.SetRange(0, 100);
	m_Slider_BrightFilter.SetRange(0, 100);
	m_fLightDis_Slider.SetRange(0, 1000);
	
	Get_Render();
	
	UpdateData(FALSE);
}


void CPopup_LightOption::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


// apply(적용) 버튼을 누르면 - 옵션 정보들을 적용한다.
void CPopup_LightOption::OnBnClicked_Eh_Apply()
{
	UpdateData(TRUE);

	Management Set_LightBackBufferColor(D3DXCOLOR(m_fBufferColor_R, m_fBufferColor_G, m_fBufferColor_B, 1.f));

	if (!Management IsLightExist())
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}


	Management Set_LightPos(_vec3(m_fLightPos_X, m_fLightPos_Y, m_fLightPos_Z));
	Management Set_LightDir(_vec3(m_fLightDir_X, m_fLightDir_Y, m_fLightDir_Z));
	
	Management Set_LightDiffuse(D3DXCOLOR(m_f_D_R, m_f_D_G, m_f_D_B, m_f_D_A));
	Management Set_LightSpecular(D3DXCOLOR(m_f_S_R, m_f_S_G, m_f_S_B, m_f_S_A));
	Management Set_LightAmbient(D3DXCOLOR(m_f_A_R, m_f_A_G, m_f_A_B, m_f_A_A));

	Management Set_FogStart(m_fFogStart);
	Management Set_FogEnd(m_fFogEnd);

	Management Set_FogColor(_vec4(m_fFogColor_R, m_fFogColor_G, m_fFogColor_B, m_fFogColor_A));

	Set_Render();

}

// 이 버튼을 누르면 해당 칼라버튼의 색을 벡터 값들에 적용해줌 
void CPopup_LightOption::OnBnClicked_Eh_Color_Button()
{
	if (!Management IsLightExist())
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}

	UpdateData(TRUE);

	// 칼라버튼 -> 칼라 벡터

	COLORREF dwDiffuse = m_vColor_Diffuse.GetColor();
	COLORREF dwSpecular = m_vColor_Specular.GetColor();
	COLORREF dwAmbient = m_vColor_Ambient.GetColor();

	m_f_D_R = GetRGB255_TO_RGB1f(GetRValue(dwDiffuse));
	m_f_D_R = SetFloatColor(m_f_D_R);
	m_f_D_G = GetRGB255_TO_RGB1f(GetGValue(dwDiffuse));
	m_f_D_G = SetFloatColor(m_f_D_G);
	m_f_D_B = GetRGB255_TO_RGB1f(GetBValue(dwDiffuse));
	m_f_D_B = SetFloatColor(m_f_D_B);

	m_f_S_R = GetRGB255_TO_RGB1f(GetRValue(dwSpecular));
	m_f_S_R = SetFloatColor(m_f_S_R);
	m_f_S_G = GetRGB255_TO_RGB1f(GetGValue(dwSpecular));
	m_f_S_G = SetFloatColor(m_f_S_G);
	m_f_S_B = GetRGB255_TO_RGB1f(GetBValue(dwSpecular));
	m_f_S_B = SetFloatColor(m_f_S_B);

	m_f_A_R = GetRGB255_TO_RGB1f(GetRValue(dwAmbient));
	m_f_A_R = SetFloatColor(m_f_A_R);
	m_f_A_G = GetRGB255_TO_RGB1f(GetGValue(dwAmbient));
	m_f_A_G = SetFloatColor(m_f_A_G);
	m_f_A_B = GetRGB255_TO_RGB1f(GetBValue(dwAmbient));
	m_f_A_B = SetFloatColor(m_f_A_B);

	UpdateData(FALSE);
}


// 이 버튼을 누르면 해당 칼라 벡터 값들을 버튼의 색에 적용해줌 
void CPopup_LightOption::OnBnClicked_Eh_Color_Vector()
{
	if (!Management IsLightExist())
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}

	UpdateData(TRUE);

	// 칼라벡터 -> 칼라 버튼

	D3DXCOLOR vDiffuse	= D3DXCOLOR(m_f_D_R, m_f_D_G, m_f_D_B, m_f_D_A);
	D3DXCOLOR vSpecular	= D3DXCOLOR(m_f_S_R, m_f_S_G, m_f_S_B, m_f_S_A);
	D3DXCOLOR vAmbient	= D3DXCOLOR(m_f_A_R, m_f_A_G, m_f_A_B, m_f_A_A);

	m_vColor_Diffuse.SetColor(D3DCOLORTOCOLORREF(vDiffuse));
	m_vColor_Specular.SetColor(D3DCOLORTOCOLORREF(vSpecular));
	m_vColor_Ambient.SetColor(D3DCOLORTOCOLORREF(vAmbient));

	UpdateData(FALSE);
}


// 이 버튼을 누르면 해당 칼라버튼의 색을 벡터 값들에 적용해줌 
void CPopup_LightOption::OnBnClicked_Eh_Fogcolor_Button()
{
	if (!Management IsLightExist())
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}

	UpdateData(TRUE);

	// 포그 칼라버튼 -> 포그 칼라 벡터

	COLORREF dwColor = m_vColor_Fog.GetColor();

	m_fFogColor_R = GetRGB255_TO_RGB1f(GetRValue(dwColor));
	m_fFogColor_R = SetFloatColor(m_fFogColor_R);
	m_fFogColor_G = GetRGB255_TO_RGB1f(GetGValue(dwColor));
	m_fFogColor_G = SetFloatColor(m_fFogColor_G);
	m_fFogColor_B = GetRGB255_TO_RGB1f(GetBValue(dwColor));
	m_fFogColor_B = SetFloatColor(m_fFogColor_B);

	UpdateData(FALSE);
}


// 이 버튼을 누르면 해당 칼라 벡터 값들을 버튼의 색에 적용해줌 
void CPopup_LightOption::OnBnClicked_Eh_Fogcolor_Vector()
{
	if (!Management IsLightExist())
	{
		// 빛이 하나도 없으므로 리턴스~
		return;
	}

	UpdateData(TRUE);

	// 포그 칼라벡터 -> 포그 칼라 버튼

	D3DXCOLOR vColor = D3DXCOLOR(m_fFogColor_R, m_fFogColor_G, m_fFogColor_B, m_fFogColor_A);

	m_vColor_Fog.SetColor(D3DCOLORTOCOLORREF(vColor));

	UpdateData(FALSE);
}

// 취소
void CPopup_LightOption::OnBnClicked_Eh_Cancel()
{
	CDialogEx::OnCancel();
}


void CPopup_LightOption::OnBnClicked_Eh_RenderoptionReset()
{
	Management Reset_RendererOption();

	Get_Render();

	UpdateData(FALSE);
}


void CPopup_LightOption::OnNMCustomdraw_Lightoptionview_LumSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);
	
	_float fMulti = 1.f;
	m_fLum = (float)m_Slider_Lum.GetPos() * 0.001f * fMulti;

	*pResult = 0;

	Set_Render();


	UpdateData(false);
}


void CPopup_LightOption::OnNMCustomdraw_Lightoptionview_MidgraySlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);

	m_fMiddleGray = (float)m_Slider_MidGray.GetPos() * 0.01f;

	*pResult = 0;

	Set_Render();

	UpdateData(false);
}


void CPopup_LightOption::OnNMCustomdraw_Lightoptionview_WhitecutoffSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);

	m_fWhiteCutOff = (float)m_Slider_WhiteCutOff.GetPos() * 0.01f;

	*pResult = 0;

	Set_Render();

	UpdateData(false);
}


void CPopup_LightOption::OnNMCustomdraw_Lightoptionview_BrightfilterSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);

	m_fBrightFilter = (float)m_Slider_BrightFilter.GetPos() * 0.01f;

	*pResult = 0;

	Set_Render();

	UpdateData(false);
}


void CPopup_LightOption::OnBnClicked_Eh_RenderoptionUpdate()
{
	Get_Render();

	_vec3 vPos_Light = Management Get_LightPos();
	m_fLightPos_X = vPos_Light.x;
	m_fLightPos_Y = vPos_Light.y;
	m_fLightPos_Z = vPos_Light.z;

	_vec3 vDir_Light = Management Get_LightDir();
	m_fLightDir_X = vDir_Light.x;
	m_fLightDir_Y = vDir_Light.y;
	m_fLightDir_Z = vDir_Light.z;

	UpdateData(FALSE);
}


void CPopup_LightOption::OnNMCustomdraw_Eh_LightDisSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);

	_float fDis = (_float)m_fLightDis_Slider.GetPos() * 0.1f;

	m_fLightDis = fDis - ((_float)m_iLightDis * 0.1f);

	*pResult = 0;

	Set_Render();

	UpdateData(false);
}
