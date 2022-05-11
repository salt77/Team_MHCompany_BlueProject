// Tool_Effect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Effect.h"
#include "afxdialogex.h"

// CTool_Effect ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTool_Effect, CDialogEx)

CTool_Effect::CTool_Effect(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_EFFECT, pParent)
	, m_fDelayTime(0)
	, m_fLifeTime(0)
	, m_fPos_Start_X(0)
	, m_fPos_Start_Y(0)
	, m_fPos_Start_Z(0)
	, m_fPos_End_X(0)
	, m_fPos_End_Y(0)
	, m_fPos_End_Z(0)
	, m_fRot_Start_X(0)
	, m_fRot_Start_Y(0)
	, m_fRot_Start_Z(0)
	, m_fRot_End_X(0)
	, m_fRot_End_Y(0)
	, m_fRot_End_Z(0)
	, m_fScale_Start_X(0)
	, m_fScale_Start_Y(0)
	, m_fScale_Start_Z(0)
	, m_fScale_End_X(0)
	, m_fScale_End_Y(0)
	, m_fScale_End_Z(0)
	, m_fColor_Start_R(0)
	, m_fColor_Start_G(0)
	, m_fColor_Start_B(0)
	, m_fColor_Start_A(0)
	, m_fColor_End_R(0)
	, m_fColor_End_G(0)
	, m_fColor_End_B(0)
	, m_fColor_End_A(0)
	, m_fSpeedCount(0)
	, m_fU_Start(0)
	, m_fV_Start(0)
	, m_fU_End(0)
	, m_fV_End(0)
	, m_bBlackAlpha(FALSE)
	, m_bBlackWhite(FALSE)
	, m_bBlur(FALSE)
	, m_bBillboard(FALSE)
	, m_bZenable(TRUE)
	, m_EffectName(_T(""))
	, m_fBegin_Filter_1(0)
	, m_fEnd_Filter_1(0)
	, m_fMin_Filter_1(0)
	, m_fMax_Filter_1(0)
	, m_fB_Filter_1(1.f)
	, m_fR_Filter_1(1.f)
	, m_fG_Filter_1(1.f)
	, m_bReserve_Filter_1(FALSE)
	, m_fBegin_Filter_2(0)
	, m_fEnd_Filter_2(0)
	, m_fMin_Filter_2(0)
	, m_fMax_Filter_2(0)
	, m_fB_Filter_2(1.f)
	, m_fR_Filter_2(1.f)
	, m_fG_Filter_2(1.f)
	, m_bReserve_Filter_2(FALSE)
	, m_fR_Filter_4(1.f)
	, m_fG_Filter_4(1.f)
	, m_fB_Filter_4(1.f)
	, m_fA_Filter_4(1.f)
	, m_fU_Filter_4(0.f)
	, m_fV_Filter_4(0.f)
	, m_strSearch(_T(""))
{
	m_pPathEffect = L"None";
	m_pPathParticle = L"None";
	m_testSkill = L"None";
}

CTool_Effect::CTool_Effect(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_PARTICLE, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Effect::~CTool_Effect()
{
}

void CTool_Effect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Picture_Base, m_pictureEffect_Base);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Picture_Filter_1, m_pictureEffect_Filter_1);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Picture_Filter_2, m_pictureEffect_Filter_2);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Picture_Filter_3, m_pictureEffect_Filter_3);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_fDelayTime, m_fDelayTime);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_fLifeTime, m_fLifeTime);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_Start_X, m_fPos_Start_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_Start_Y, m_fPos_Start_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_Start_Z, m_fPos_Start_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_End_X, m_fPos_End_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_End_Y, m_fPos_End_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Pos_End_Z, m_fPos_End_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_Start_X, m_fRot_Start_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_Start_Y, m_fRot_Start_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_Start_Z, m_fRot_Start_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_End_X, m_fRot_End_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_End_Y, m_fRot_End_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Rot_End_Z, m_fRot_End_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_Start_X, m_fScale_Start_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_Start_Y, m_fScale_Start_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_Start_Z, m_fScale_Start_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_End_X, m_fScale_End_X);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_End_Y, m_fScale_End_Y);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Scale_End_Z, m_fScale_End_Z);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_Start_R, m_fColor_Start_R);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_Start_G, m_fColor_Start_G);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_Start_B, m_fColor_Start_B);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_Start_A, m_fColor_Start_A);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_End_R, m_fColor_End_R);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_End_G, m_fColor_End_G);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_End_B, m_fColor_End_B);
	DDX_Text(pDX, IDC_EH_1220_EffectTool_Color_End_A, m_fColor_End_A);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Color_Start_Button, m_vColor_Start_Button);
	DDX_Control(pDX, IDC_EH_1220_EffectTool_Color_End_Button, m_vColor_End_Button);
	DDX_Control(pDX, IDC_EH_1221_EffectTool_TypeCombo, m_eEffectLoopType);
	DDX_Text(pDX, IDC_EH_1221_EffectTool_fSpeedCount, m_fSpeedCount);
	DDX_Text(pDX, IDC_EH_1221_EffectTool_U_Start, m_fU_Start);
	DDX_Text(pDX, IDC_EH_1221_EffectTool_V_Start, m_fV_Start);
	DDX_Text(pDX, IDC_EH_1221_EffectTool_U_End, m_fU_End);
	DDX_Text(pDX, IDC_EH_1221_EffectTool_V_End, m_fV_End);
	DDX_Control(pDX, IDC_EH_1227_EffectTool_ListBox_EffectSave, m_listBoxEffectSave);
	DDX_Control(pDX, IDC_EH_1227_EffectTool_ListBox_ParticleSave, m_listBoxParticleSave);
	DDX_Control(pDX, IDC_EH_1227_EffectTool_ListBox_Skill_Effect, m_listBox_Skill_Effect);
	DDX_Control(pDX, IDC_EH_1227_EffectTool_ListBox_Skill_Particle, m_listBox_Skill_Particle);
	DDX_Control(pDX, IDC_EH_1228_EffectTool_ListBox_Skill_Save, m_listBox_Skill);
	DDX_Text(pDX, IDC_EH_1228_EffectTool_Test_Skill, m_testSkill);
	DDX_Check(pDX, IDC_EH_1228_BlackAlpha, m_bBlackAlpha);
	DDX_Check(pDX, IDC_EH_1228_BlackWhite, m_bBlackWhite);
	DDX_Check(pDX, IDC_EH_0106_EffectTool_Blur, m_bBlur);
	DDX_Control(pDX, IDC_EH_0106_EffectTool_TypeCombo_AlphaOPtion, m_comboAlphaOption);
	DDX_Control(pDX, IDC_EH_0106_EffectTool_Cull, m_comboCull);
	DDX_Check(pDX, IDC_EH_0107_Billboard, m_bBillboard);
	DDX_Check(pDX, IDC_EH_0107_Zenable, m_bZenable);
	DDX_Text(pDX, IDC_EH_0108_EffectTool_EffectName, m_EffectName);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fBegin_Filter_1, m_fBegin_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fEnd_Filter_1, m_fEnd_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fMin_Filter_1, m_fMin_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fMax_Filter_1, m_fMax_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fB_Filter_1, m_fB_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fR_Filter_1, m_fR_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fG_Filter_1, m_fG_Filter_1);
	DDX_Check(pDX, IDC_EH_0117_bReserve_Filter_1, m_bReserve_Filter_1);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fBegin_Filter_2, m_fBegin_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fEnd_Filter_2, m_fEnd_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fMin_Filter_2, m_fMin_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fMax_Filter_2, m_fMax_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fB_Filter_2, m_fB_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fR_Filter_2, m_fR_Filter_2);
	DDX_Text(pDX, IDC_EH_0116_EffectTool_fG_Filter_2, m_fG_Filter_2);
	DDX_Check(pDX, IDC_EH_0117_bReserve_Filter_2, m_bReserve_Filter_2);

	DDX_Text(pDX, IDC_EH_0117_EffectTool_fR_Filter_4, m_fR_Filter_4);
	DDX_Text(pDX, IDC_EH_0117_EffectTool_fA_Filter_4, m_fA_Filter_4);
	DDX_Text(pDX, IDC_EH_0117_EffectTool_fG_Filter_4, m_fG_Filter_4);
	DDX_Text(pDX, IDC_EH_0117_EffectTool_fB_Filter_4, m_fB_Filter_4);
	DDX_Text(pDX, IDC_EH_0117_EffectTool_fU_Filter_4, m_fU_Filter_4);
	DDX_Text(pDX, IDC_EH_0117_EffectTool_fV_Filter_4, m_fV_Filter_4);
	DDX_Text(pDX, IDC_EH_0126_EffectTool_Search_Edit, m_strSearch);
}

// ��ư�� ������ - ����Ʈ ���� ������
void CTool_Effect::ReUpdate_Tool()
{
	LoadEffectList();

	LoadParticleList();

	LoadSkillList();

	Get_Info();
}

// ����Ʈ ���� �̹����� ������
void CTool_Effect::Apply_Tex_EffectBase(T_EffectTexture tEffectTexture)
{

	// ����Ʈ �ؽ�ó ��ȿ �˻�
	CString pPath = tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		// ����Ʈ�� �ؽ�ó�� ����
		AfxMessageBox(L"�ؽ�ó ��� ����.");
		return;
	}
	
	T_EffectInfo tEffectInfo = m_tEffectInfo;

	// ���� �ؽ��� ����
	if (0 == tEffectTexture.iTextureType) 
	{
		// �ؽ�ó ���
		tEffectInfo.textPath = tEffectTexture.textPath;
		pPath = tEffectInfo.textPath.c_str();

		// ��������Ʈ �ؽ��� ����
		tEffectInfo.bIsSprite = tEffectTexture.bIsSprite;
		tEffectInfo.iRow = tEffectTexture.iRow;
		tEffectInfo.iColumn = tEffectTexture.iColumn;


		// �޽� ����Ʈ ����
		tEffectInfo.eEffectRenderType = (E_EffectRenderType)tEffectTexture.bIsMesh;

		if ((_int)tEffectInfo.eEffectRenderType)
		{
			// ����Ʈ �ؽ�ó ��ȿ �˻�
			CString pMeshPath = tEffectTexture.meshPath.c_str();
			if (0 == pMeshPath.CompareNoCase(L"None"))
			{
				// �޽� ����Ʈ�� �޽ð�ΰ� ����
				AfxMessageBox(L"�޽� ��� ����.");
				return;
			}

			tEffectInfo.meshPath = tEffectTexture.meshPath;
		}
		else
		{
			tEffectInfo.meshPath = L"None";
		}

		DrawPicture(m_pictureEffect_Base.m_hWnd, pPath, FALSE, TRUE);
	}
	else if (1 == tEffectTexture.iTextureType)
	{
		tEffectInfo.filterPath_1 = tEffectTexture.textPath;
		pPath = tEffectInfo.filterPath_1.c_str();
		DrawPicture(m_pictureEffect_Filter_1.m_hWnd, pPath);

	}
	else if (2 == tEffectTexture.iTextureType)
	{
		tEffectInfo.filterPath_2 = tEffectTexture.textPath;
		pPath = tEffectInfo.filterPath_2.c_str();
		DrawPicture(m_pictureEffect_Filter_2.m_hWnd, pPath);

	}
	else if (3 == tEffectTexture.iTextureType)
	{
		tEffectInfo.filterPath_3 = tEffectTexture.textPath;
		pPath = tEffectInfo.filterPath_3.c_str();
		DrawPicture(m_pictureEffect_Filter_3.m_hWnd, pPath);

	}
	
	m_tEffectInfo = tEffectInfo;

}


// HWND�� strTextureName �̹����� �׸�
void CTool_Effect::DrawPicture(HWND hDestWindowOverride, CString & strTextureName, _bool bFilter/*=TRUE*/, _bool bBlakcOption/*=FALSE*/)
{
	CTexture*			pTextureCom = CTexture::Create(Management GetDevice(), strTextureName.operator LPCWSTR());
	CRectBuffer*		pBufferCom = static_cast<CRectBuffer*>(Management Clone_Prototype(L"Proto_Buffer_RectTex"));
	CShader*			pShaderCom = static_cast<CShader*>(Management Clone_Prototype(L"Proto_Shader_Effect"));

	Management GetDevice()->Clear(0, nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		Management Get_LightBackBufferColor()
		, 1.f, 0);
	Management GetDevice()->BeginScene();

	if (!pTextureCom || !pBufferCom || !pShaderCom)
	{
		Management GetDevice()->EndScene();
		Management GetDevice()->Present(NULL, NULL, hDestWindowOverride, NULL);
		
		Safe_Release(pBufferCom);
		Safe_Release(pShaderCom);
		Safe_Release(pTextureCom);

		return;
	}

	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;

	_matrix		matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matIdentity);
	pEffect->SetMatrix("g_matProj", &matIdentity);
	pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));

	pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);

	pEffect->SetFloat("g_fU", 0.f);
	pEffect->SetFloat("g_fV", 0.f);
	pEffect->SetFloat("g_fU_Scale", 1.f);
	pEffect->SetFloat("g_fV_Scale", 1.f);

	pEffect->SetBool("g_bFilter_1", FALSE);
	pEffect->SetBool("g_bFilter_2", FALSE);
	pEffect->SetBool("g_bFilter_3", FALSE);
	
	if (bBlakcOption && !bFilter)
	{
		pEffect->SetBool("g_bBlackAlpha", m_bBlackAlpha);
		pEffect->SetBool("g_bBlackWhite", m_bBlackWhite);
	}
	else
	{
		pEffect->SetBool("g_bBlackAlpha", FALSE);
		pEffect->SetBool("g_bBlackWhite", FALSE);
	}
	
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	pShaderCom->BeginShader();
	pShaderCom->BeginPass(0);

	pBufferCom->Render_Buffer();

	pShaderCom->EndPass();
	pShaderCom->EndShader();


	Management GetDevice()->EndScene();
	Management GetDevice()->Present(NULL, NULL, hDestWindowOverride, NULL);
	

	Safe_Release(pBufferCom);
	Safe_Release(pShaderCom);
	Safe_Release(pTextureCom);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

// ����Ʈ ������ ������
void CTool_Effect::Set_Info()
{
	UpdateData(TRUE);

	m_tEffectInfo.fDelayTime = m_fDelayTime;
	m_tEffectInfo.fLifeTime = m_fLifeTime;

	m_tEffectInfo.vPos_Start	= _vec3(m_fPos_Start_X, m_fPos_Start_Y, m_fPos_Start_Z);
	m_tEffectInfo.vPos_End		= _vec3(m_fPos_End_X, m_fPos_End_Y, m_fPos_End_Z);

	m_tEffectInfo.vRot_Start	= _vec3(m_fRot_Start_X, m_fRot_Start_Y, m_fRot_Start_Z);
	m_tEffectInfo.vRot_End		= _vec3(m_fRot_End_X, m_fRot_End_Y, m_fRot_End_Z);

	m_tEffectInfo.vScale_Start	= _vec3(m_fScale_Start_X, m_fScale_Start_Y, m_fScale_Start_Z);
	m_tEffectInfo.vScale_End	= _vec3(m_fScale_End_X, m_fScale_End_Y, m_fScale_End_Z);

	m_tEffectInfo.vColor_Start	= _vec4(m_fColor_Start_R, m_fColor_Start_G, m_fColor_Start_B, m_fColor_Start_A);
	m_tEffectInfo.vColor_End	= _vec4(m_fColor_End_R, m_fColor_End_G, m_fColor_End_B, m_fColor_End_A);
	
	m_vColor_Start_Button.SetColor(D3DCOLORTOCOLORREF(D3DXCOLOR(m_fColor_Start_R, m_fColor_Start_G, m_fColor_Start_B, m_fColor_Start_A)));
	m_vColor_End_Button.SetColor(D3DCOLORTOCOLORREF(D3DXCOLOR(m_fColor_End_R, m_fColor_End_G, m_fColor_End_B, m_fColor_End_A)));

	m_tEffectInfo.eEffectType = (E_EffectType)m_eEffectLoopType.GetCurSel();
	m_tEffectInfo.fSpeedCount = m_fSpeedCount;

	m_tEffectInfo.fU_Start = m_fU_Start;
	m_tEffectInfo.fV_Start = m_fV_Start;
	m_tEffectInfo.fU_End = m_fU_End;
	m_tEffectInfo.fV_End = m_fV_End;

	//m_tEffectInfo.iPass = m_iPass;

	m_tEffectInfo.bBlackAlpha = BOOL2bool(m_bBlackAlpha);
	m_tEffectInfo.bBlackWhite = BOOL2bool(m_bBlackWhite);

	/////////////////////////////////////////////////////////////////
	
	_int iCount_iTemp = 0;

	// iTemp[0] -----------------------------------------------------
	m_tEffectInfo.iTemp[iCount_iTemp++] = m_comboAlphaOption.GetCurSel();

	// iTemp[1] -----------------------------------------------------

	m_tEffectInfo.iTemp[iCount_iTemp++] = m_comboCull.GetCurSel();

	/////////////////////////////////////////////////////////////////
	
	_int iCount_bTemp = 0;

	// bTemp[0] -----------------------------------------------------
	m_tEffectInfo.bTemp[iCount_bTemp++] = BOOL2bool(m_bBlur);
	// bTemp[1] -----------------------------------------------------
	m_tEffectInfo.bTemp[iCount_bTemp++] = BOOL2bool(m_bBillboard);
	// bTemp[2] -----------------------------------------------------
	m_tEffectInfo.bTemp[iCount_bTemp++] = BOOL2bool(m_bZenable);
	// bTemp[3] -----------------------------------------------------
	m_tEffectInfo.bTemp[iCount_bTemp++] = BOOL2bool(m_bReserve_Filter_1);
	// bTemp[4] -----------------------------------------------------
	m_tEffectInfo.bTemp[iCount_bTemp++] = BOOL2bool(m_bReserve_Filter_2);

	/////////////////////////////////////////////////////////////////

	_int iCount_fTemp = 0;
	// fTemp[0] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fBegin_Filter_1;
	// fTemp[1] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fEnd_Filter_1;
	// fTemp[2] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fMin_Filter_1;
	// fTemp[3] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fMax_Filter_1;
	// fTemp[4] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fR_Filter_1;
	// fTemp[5] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fG_Filter_1;
	// fTemp[6] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fB_Filter_1;

	// fTemp[7] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fR_Filter_4;
	// fTemp[8] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fG_Filter_4;
	// fTemp[9] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fB_Filter_4;
	// fTemp[10] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fA_Filter_4;
	// fTemp[11] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fU_Filter_4;
	// fTemp[12] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fV_Filter_4;

	// fTemp[13] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fBegin_Filter_2;
	// fTemp[14] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fEnd_Filter_2;
	// fTemp[15] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fMin_Filter_2;
	// fTemp[16] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fMax_Filter_2;
	// fTemp[17] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fR_Filter_2;
	// fTemp[18] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fG_Filter_2;
	// fTemp[19] -----------------------------------------------------
	m_tEffectInfo.fTemp[iCount_fTemp++] = m_fB_Filter_2;

	UpdateData(FALSE);
}

// ����Ʈ ������ ������
void CTool_Effect::Get_Info()
{
	m_fDelayTime		= m_tEffectInfo.fDelayTime;
	m_fLifeTime			= m_tEffectInfo.fLifeTime;

	m_fPos_Start_X		= m_tEffectInfo.vPos_Start.x;
	m_fPos_Start_Y		= m_tEffectInfo.vPos_Start.y;
	m_fPos_Start_Z		= m_tEffectInfo.vPos_Start.z;
	m_fPos_End_X		= m_tEffectInfo.vPos_End.x;
	m_fPos_End_Y		= m_tEffectInfo.vPos_End.y;
	m_fPos_End_Z		= m_tEffectInfo.vPos_End.z;

	m_fRot_Start_X		= m_tEffectInfo.vRot_Start.x;
	m_fRot_Start_Y		= m_tEffectInfo.vRot_Start.y;
	m_fRot_Start_Z		= m_tEffectInfo.vRot_Start.z;
	m_fRot_End_X		= m_tEffectInfo.vRot_End.x;
	m_fRot_End_Y		= m_tEffectInfo.vRot_End.y;
	m_fRot_End_Z		= m_tEffectInfo.vRot_End.z;

	m_fScale_Start_X	= m_tEffectInfo.vScale_Start.x;
	m_fScale_Start_Y	= m_tEffectInfo.vScale_Start.y;
	m_fScale_Start_Z	= m_tEffectInfo.vScale_Start.z;
	m_fScale_End_X		= m_tEffectInfo.vScale_End.x;
	m_fScale_End_Y		= m_tEffectInfo.vScale_End.y;
	m_fScale_End_Z		= m_tEffectInfo.vScale_End.z;

	m_fColor_Start_R	= m_tEffectInfo.vColor_Start.x;
	m_fColor_Start_G	= m_tEffectInfo.vColor_Start.y;
	m_fColor_Start_B	= m_tEffectInfo.vColor_Start.z;
	m_fColor_Start_A	= m_tEffectInfo.vColor_Start.w;

	m_fColor_End_R		= m_tEffectInfo.vColor_End.x;
	m_fColor_End_G		= m_tEffectInfo.vColor_End.y;
	m_fColor_End_B		= m_tEffectInfo.vColor_End.z;
	m_fColor_End_A		= m_tEffectInfo.vColor_End.w;

	m_vColor_Start_Button;
	m_vColor_End_Button;

	D3DXCOLOR vColor_Start	= D3DXCOLOR(m_fColor_Start_R, m_fColor_Start_G, m_fColor_Start_B, m_fColor_Start_A); 
	D3DXCOLOR vColor_End	= D3DXCOLOR(m_fColor_End_R, m_fColor_End_G, m_fColor_End_B, m_fColor_End_A);

	m_vColor_Start_Button.SetColor(D3DCOLORTOCOLORREF(vColor_Start));
	m_vColor_End_Button.SetColor(D3DCOLORTOCOLORREF(vColor_End));
	
	m_eEffectLoopType.SetCurSel((_int)m_tEffectInfo.eEffectType);
	m_fSpeedCount = m_tEffectInfo.fSpeedCount;

	m_fU_Start = m_tEffectInfo.fU_Start;
	m_fV_Start = m_tEffectInfo.fV_Start;
	m_fU_End = m_tEffectInfo.fU_End;
	m_fV_End = m_tEffectInfo.fV_End;

	//m_iPass = m_tEffectInfo.iPass;

	m_bBlackAlpha = m_tEffectInfo.bBlackAlpha;
	m_bBlackWhite = m_tEffectInfo.bBlackWhite;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	_int iCount_iTemp = 0;

	//  iTemp[0]
	_int iTemp = 0;
	iTemp = m_tEffectInfo.iTemp[iCount_iTemp++];
	if (3 <= iTemp) // m_comboAlphaOption�� ����Ʈ����(0),���ĺ���(1),����� (2)�� ���� ����
	{
		AfxMessageBox(L"m_tEffectInfo.iTemp[0] �ҷ����� ����. 3�̻��� ��(����) �� ȣ���");
	}
	m_comboAlphaOption.SetCurSel(iTemp);

	//  iTemp[1]
	iTemp = m_tEffectInfo.iTemp[iCount_iTemp++];
	if (3 <= iTemp) // m_comboCull�� ��� ǥ��(0), �ĸ� �߷�����(1), ���� �߷����� (2)�� ���� ����
	{
		AfxMessageBox(L"m_tEffectInfo.iTemp[1] �ҷ����� ����. 3�̻��� ��(����) �� ȣ���");
	}
	m_comboCull.SetCurSel(iTemp);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	_int iCount_bTemp = 0;

	// bTemp[0]
	m_bBlur = m_tEffectInfo.bTemp[iCount_bTemp++];
	// bTemp[1]
	m_bBillboard = m_tEffectInfo.bTemp[iCount_bTemp++];
	// bTemp[2]
	m_bZenable = m_tEffectInfo.bTemp[iCount_bTemp++];
	// bTemp[3] -----------------------------------------------------
	m_bReserve_Filter_1 = m_tEffectInfo.bTemp[iCount_bTemp++];
	// bTemp[4] -----------------------------------------------------
	m_bReserve_Filter_2 = m_tEffectInfo.bTemp[iCount_bTemp++];

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	_int iCount_fTemp = 0;

	// fTemp[0] -----------------------------------------------------
	m_fBegin_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[1] -----------------------------------------------------
	m_fEnd_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[2] -----------------------------------------------------
	m_fMin_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[3] -----------------------------------------------------
	m_fMax_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[4] -----------------------------------------------------
	m_fR_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[5] -----------------------------------------------------
	m_fG_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[6] -----------------------------------------------------
	m_fB_Filter_1 = m_tEffectInfo.fTemp[iCount_fTemp++];

	// fTemp[7] -----------------------------------------------------
	m_fR_Filter_4 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[8] -----------------------------------------------------
	m_fG_Filter_4; m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[9] -----------------------------------------------------
	m_fB_Filter_4 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[10] -----------------------------------------------------
	m_fA_Filter_4 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[11] -----------------------------------------------------
	m_fU_Filter_4 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[12] -----------------------------------------------------
	m_fV_Filter_4 = m_tEffectInfo.fTemp[iCount_fTemp++];

	// fTemp[13] -----------------------------------------------------
	m_fBegin_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[14] -----------------------------------------------------
	m_fEnd_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[15] -----------------------------------------------------
	m_fMin_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[16] -----------------------------------------------------
	m_fMax_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[17] -----------------------------------------------------
	m_fR_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[18] -----------------------------------------------------
	m_fG_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];
	// fTemp[19] -----------------------------------------------------
	m_fB_Filter_2 = m_tEffectInfo.fTemp[iCount_fTemp++];

	/////////////////////////////////////////////////////////////////////////
	// �׸� ����

	CString pPath = m_tEffectInfo.textPath.c_str();
	DrawPicture(m_pictureEffect_Base.m_hWnd, pPath, FALSE, TRUE);
	pPath = m_tEffectInfo.filterPath_1.c_str();
	DrawPicture(m_pictureEffect_Filter_1.m_hWnd, pPath);
	pPath = m_tEffectInfo.filterPath_2.c_str();
	DrawPicture(m_pictureEffect_Filter_2.m_hWnd, pPath);
	pPath = m_tEffectInfo.filterPath_3.c_str();
	DrawPicture(m_pictureEffect_Filter_3.m_hWnd, pPath);


	UpdateData(FALSE);
}

// ����Ʈ ������ �ʱ�ȭ��
void CTool_Effect::Reset_Info()
{
	m_tEffectInfo = T_EffectInfo();
	m_tEffectInfo.bTemp[2] = TRUE;

	Get_Info();
	Set_Info();
}

void CTool_Effect::LoadEffectInfo(CString pPath, T_EffectInfo* pInfo, _bool bFullPath)
{
	T_EffectInfo tEffectInfo = T_EffectInfo();

	CString pFilePath;
	if (bFullPath) 
	{
		pFilePath = pPath;
	}
	else
	{
		pFilePath = L"../../Reference/Data/Data_Effect/" + pPath;
	} 
		
	HANDLE hFile = CreateFileW(pFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD dwbyte = 0;

	CUtility::Data_ReadWString(hFile, tEffectInfo.textPath, dwbyte);
	int iEnumTemp = 0;
	CUtility::Data_ReadInt(hFile, iEnumTemp, dwbyte);
	tEffectInfo.eEffectRenderType = (E_EffectRenderType)iEnumTemp;
	int iEnumTemp2 = 0;
	CUtility::Data_ReadWString(hFile, tEffectInfo.meshPath, dwbyte);
	CUtility::Data_ReadInt(hFile, iEnumTemp2, dwbyte);
	tEffectInfo.eEffectType = (E_EffectType)iEnumTemp2;

	CUtility::Data_ReadFloat(hFile, tEffectInfo.fSpeedCount, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fDelayTime, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fLifeTime, dwbyte);

	CUtility::Data_ReadInt(hFile, tEffectInfo.iPass, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fU_Start, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fU_End, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fV_Start, dwbyte);
	CUtility::Data_ReadFloat(hFile, tEffectInfo.fV_End, dwbyte);

	CUtility::Data_ReadVec3(hFile, tEffectInfo.vPos_Start, dwbyte);
	CUtility::Data_ReadVec3(hFile, tEffectInfo.vPos_End, dwbyte);
	CUtility::Data_ReadVec3(hFile, tEffectInfo.vRot_Start, dwbyte);
	CUtility::Data_ReadVec3(hFile, tEffectInfo.vRot_End, dwbyte);
	CUtility::Data_ReadVec3(hFile, tEffectInfo.vScale_Start, dwbyte);
	CUtility::Data_ReadVec3(hFile, tEffectInfo.vScale_End, dwbyte);

	ReadFile(hFile, &tEffectInfo.vColor_Start, sizeof(D3DXVECTOR4), &dwbyte, nullptr);
	ReadFile(hFile, &tEffectInfo.vColor_End, sizeof(D3DXVECTOR4), &dwbyte, nullptr);

	CUtility::Data_ReadBoolean(hFile, tEffectInfo.bIsSprite, dwbyte);
	CUtility::Data_ReadInt(hFile, tEffectInfo.iRow, dwbyte);
	CUtility::Data_ReadInt(hFile, tEffectInfo.iColumn, dwbyte);

	CUtility::Data_ReadWString(hFile, tEffectInfo.filterPath_1, dwbyte);
	CUtility::Data_ReadWString(hFile, tEffectInfo.filterPath_2, dwbyte);
	CUtility::Data_ReadWString(hFile, tEffectInfo.filterPath_3, dwbyte);

	CUtility::Data_ReadBoolean(hFile, tEffectInfo.bBlackAlpha, dwbyte);
	CUtility::Data_ReadBoolean(hFile, tEffectInfo.bBlackWhite, dwbyte);
	
	for (_int i = 0; i < 10; i++)
	{
		CUtility::Data_ReadInt(hFile, tEffectInfo.iTemp[i], dwbyte);
	}

	for (_int i = 0; i < 10; i++)
	{
		CUtility::Data_ReadBoolean(hFile, tEffectInfo.bTemp[i], dwbyte);
	}

	if (9 == tEffectInfo.iTemp[9])
	{
		// ���� ���̺� ����
		// �߰��� �� ���� "float[20]"�� �о�´�.
		for (_int i = 0; i < 20; i++)
		{
			CUtility::Data_ReadFloat(hFile, tEffectInfo.fTemp[i], dwbyte);
		}
	}

	CloseHandle(hFile);

	CString pInfoPath = tEffectInfo.textPath.c_str();
	if (0 == pInfoPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Դϴ�.");
		
		return;
	}

	*pInfo = tEffectInfo;

	return;
}

void CTool_Effect::SaveEffectInfo(CString pPath, _bool bFullPath)
{
	CString pFilePath;
	if (bFullPath)
	{
		pFilePath = pPath;
	}
	else
	{
		pFilePath = L"../../Reference/Data/Data_Effect/" + pPath;
	}

	HANDLE hFile = CreateFile(pFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD dwbyte = 0;

	CUtility::Data_WriteWString(hFile, m_tEffectInfo.textPath, dwbyte);
	CUtility::Data_WriteInt(hFile, (_int)m_tEffectInfo.eEffectRenderType, dwbyte);
	CUtility::Data_WriteWString(hFile, m_tEffectInfo.meshPath, dwbyte);
	CUtility::Data_WriteInt(hFile, (_int)m_tEffectInfo.eEffectType, dwbyte);

	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fSpeedCount, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fDelayTime, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fLifeTime, dwbyte);

	CUtility::Data_WriteInt(hFile, m_tEffectInfo.iPass, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fU_Start, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fU_End, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fV_Start, dwbyte);
	CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fV_End, dwbyte);

	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vPos_Start, dwbyte);
	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vPos_End, dwbyte);
	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vRot_Start, dwbyte);
	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vRot_End, dwbyte);
	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vScale_Start, dwbyte);
	CUtility::Data_WriteVec3(hFile, m_tEffectInfo.vScale_End, dwbyte);

	WriteFile(hFile, &m_tEffectInfo.vColor_Start, sizeof(D3DXVECTOR4), &dwbyte, nullptr);
	WriteFile(hFile, &m_tEffectInfo.vColor_End, sizeof(D3DXVECTOR4), &dwbyte, nullptr);

	CUtility::Data_WriteBoolean(hFile, m_tEffectInfo.bIsSprite, dwbyte);
	CUtility::Data_WriteInt(hFile, m_tEffectInfo.iRow, dwbyte);
	CUtility::Data_WriteInt(hFile, m_tEffectInfo.iColumn, dwbyte);

	CUtility::Data_WriteWString(hFile, m_tEffectInfo.filterPath_1, dwbyte);
	CUtility::Data_WriteWString(hFile, m_tEffectInfo.filterPath_2, dwbyte);
	CUtility::Data_WriteWString(hFile, m_tEffectInfo.filterPath_3, dwbyte);

	CUtility::Data_WriteBoolean(hFile, m_tEffectInfo.bBlackAlpha, dwbyte);
	CUtility::Data_WriteBoolean(hFile, m_tEffectInfo.bBlackWhite, dwbyte);

	for (_int i = 0; i < 9; i++)
	{
		CUtility::Data_WriteInt(hFile, m_tEffectInfo.iTemp[i], dwbyte);
	}

	CUtility::Data_WriteInt(hFile, 9, dwbyte);

	for (_int i = 0; i < 10; i++)
	{
		CUtility::Data_WriteBoolean(hFile, m_tEffectInfo.bTemp[i], dwbyte);
	}

	for (_int i = 0; i < 20; i++)
	{
		CUtility::Data_WriteFloat(hFile, m_tEffectInfo.fTemp[i], dwbyte);
	}

	CloseHandle(hFile);
}

void CTool_Effect::Edit_Effect(CString pPath, _bool bFullPath)
{
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Դϴ�.");
		return;
	}

	T_EffectInfo tEffectInfo = T_EffectInfo();

	LoadEffectInfo(pPath, &tEffectInfo, bFullPath);

	CString pErrorPath = tEffectInfo.textPath.c_str();
	if (0 == pErrorPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Ʈ ���� �ҷ����� �Դϴ�.");
		return;
	}

	m_tEffectInfo = tEffectInfo;

	Get_Info();

	m_EffectName = pPath;

	UpdateData(FALSE);
}

void CTool_Effect::LoadEffectList()
{
	UpdateData(TRUE);

	m_listBoxEffectSave.ResetContent();

	CString pPath = L"../../Reference/Data/Data_Effect/*.effect";

	CFileFind finder;
	BOOL bExist = finder.FindFile(pPath);

	//��ΰ� ��ȿ���� ����
	if (!bExist)
	{
		//assert(FALSE);
		return;
	}

	while (bExist)
	{
		bExist = finder.FindNextFile();

		if (m_bSearch) 
		{
			CString strFindFile = finder.GetFileTitle();
			CString strSearch = m_strSearch;

			strFindFile.MakeLower();
			strSearch.MakeLower();

			if (-1 != strFindFile.Find(strSearch))
			{				
				m_listBoxEffectSave.AddString(finder.GetFileTitle());
			}

			//if (0 == m_strSearch.CompareNoCase(strFindFile))
			//{
			//	m_listBoxEffectSave.AddString(strFindFile);
			//}
		}
		else
		{
			m_listBoxEffectSave.AddString(finder.GetFileTitle());
		}
	}

	m_listBoxEffectSave.SetHorizontalExtent(160);

	UpdateData(FALSE);
}

void CTool_Effect::LoadParticleList()
{
	UpdateData(TRUE);

	m_listBoxParticleSave.ResetContent();

	CString pPath = L"../../Reference/Data/Data_Particle/*.particle";

	CFileFind finder;
	BOOL bExist = finder.FindFile(pPath);

	//��ΰ� ��ȿ���� ����
	if (!bExist)
	{
		return;
	}
	while (bExist)
	{
		bExist = finder.FindNextFile();
		m_listBoxParticleSave.AddString(finder.GetFileTitle());
	}

	m_listBoxParticleSave.SetHorizontalExtent(160);

	UpdateData(FALSE);
}

void CTool_Effect::LoadSkillList()
{
	UpdateData(TRUE);

	m_listBox_Skill.ResetContent();

	CString pPath = L"../../Reference/Data/Data_Skill/*.skill";

	CFileFind finder;
	BOOL bExist = finder.FindFile(pPath);

	//��ΰ� ��ȿ���� ����
	if (!bExist)
	{
		assert(FALSE);
	}

	while (bExist)
	{
		bExist = finder.FindNextFile();

		CString pFile = finder.GetFileTitle() + L".skill";
		m_listBox_Skill.AddString(pFile);
	}

	m_listBox_Skill.SetHorizontalExtent(300);

	UpdateData(FALSE);
}

// �����ϸ� ����Ʈ Ÿ���� �ɼǰ��� �־���
BOOL CTool_Effect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_eEffectLoopType.AddString(L"Once");
	m_eEffectLoopType.AddString(L"Loop");
	m_eEffectLoopType.AddString(L"Once_PingPong");
	m_eEffectLoopType.AddString(L"Loop_PingPong");
	m_eEffectLoopType.SetCurSel(0);

	m_comboAlphaOption.AddString(L"AplhaTrans");
	m_comboAlphaOption.AddString(L"AplhaBlend");
	m_comboAlphaOption.AddString(L"NonAplha");
	m_comboAlphaOption.SetCurSel(0);

	m_comboCull.AddString(L"��� ǥ��");
	m_comboCull.AddString(L"�ĸ� �߷�����");
	m_comboCull.AddString(L"���� �߷�����");
	m_comboCull.SetCurSel(0);

	Reset_Info();
	
	return TRUE;
}


BEGIN_MESSAGE_MAP(CTool_Effect, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_EH_EffectTool_LightView, &CTool_Effect::OnBnClicked_Eh_Open_Light)
	ON_BN_CLICKED(IDC_EH_1219_EffectTool_EffecyBaseView, &CTool_Effect::OnBnClicked_Eh_Open_EffectTexture)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Create, &CTool_Effect::OnBnClicked_Eh_Create)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_LoadParticleList, &CTool_Effect::OnBnClicked_Eh_LoadParticleList)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_LoadEffectList, &CTool_Effect::OnBnClicked_Eh_LoadEffectList)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Apply, &CTool_Effect::OnBnClicked_Eh_Apply)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Color_End_Vec2Color, &CTool_Effect::OnBnClicked_Eh_ColorEnd_Vec)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Color_Start_Vec2Color, &CTool_Effect::OnBnClicked_Eh_ColorStart_Vec)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Color_Start_Button, &CTool_Effect::OnBnClicked_Eh_ColorStart_Button)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Color_End_Button, &CTool_Effect::OnBnClicked_Eh_ColorEnd_Button)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Reset, &CTool_Effect::OnBnClicked_Eh_Reset)
	ON_CBN_SELCHANGE(IDC_EH_1221_EffectTool_TypeCombo, &CTool_Effect::OnCbnSelchange_Eh_Type)
	ON_BN_CLICKED(IDC_EH_1222_EffectTool_Base2Move, &CTool_Effect::OnBnClicked_Eh_Base2move)
	ON_BN_CLICKED(IDC_EH_EffectTool_ReUpdate, &CTool_Effect::OnBnClicked_Eh_ReUpdate)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Save, &CTool_Effect::OnBnClicked_Eh_Save)
	ON_BN_CLICKED(IDC_EH_1220_EffectTool_Load, &CTool_Effect::OnBnClicked_Eh_Load)
	ON_LBN_SELCHANGE(IDC_EH_1227_EffectTool_ListBox_EffectSave, &CTool_Effect::OnLbnSelchange_Eh_ListBox_EffectLoad)
	ON_LBN_SELCHANGE(IDC_EH_1227_EffectTool_ListBox_ParticleSave, &CTool_Effect::OnLbnSelchange_Eh_ListBox_ParticleLoad)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_PlayList_Effect, &CTool_Effect::OnBnClicked_Eh_PlayList_Effect)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_PlayList_Particle, &CTool_Effect::OnBnClicked_Eh_PlayList_Particle)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_AddSkill_Effect, &CTool_Effect::OnBnClicked_Eh_AddSkill_Effect)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_AddSkill_Particle, &CTool_Effect::OnBnClicked_Eh_AddSkill_Particle)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Save, &CTool_Effect::OnBnClicked_Eh_SkillList_Save)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Load, &CTool_Effect::OnBnClicked_Eh_SkillList_Load)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Play, &CTool_Effect::OnBnClicked_Eh_SkillList_Play)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Effect_DeleteAll, &CTool_Effect::OnBnClicked_Eh_Skill_List_Effect_DeleteAll)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Effect_Delete, &CTool_Effect::OnBnClicked_Eh_Skill_List_Effect_Delete)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Particle_DeleteAll, &CTool_Effect::OnBnClicked_Eh_Skill_List_Particle_DeleteAll)
	ON_BN_CLICKED(IDC_EH_1227_EffectTool_SkillList_Particle_Delete, &CTool_Effect::OnBnClicked_Eh_Skill_List_Particle_Delete)
	ON_BN_CLICKED(IDC_EH_1228_EffectTool_SkillList_Update, &CTool_Effect::OnBnClicked_Eh_Skill_List_Update)
	ON_BN_CLICKED(IDC_EH_1228_EffectTool_SkillList_DeleteAll, &CTool_Effect::OnBnClicked_Eh_Skill_List_DeleteAll)
	ON_LBN_SELCHANGE(IDC_EH_1228_EffectTool_ListBox_Skill_Save, &CTool_Effect::OnLbnSelchangeOn_Eh_Listbox_Skill_Save)
	ON_BN_CLICKED(IDC_EH_1228_EffectTool_Test_Skill_Create, &CTool_Effect::OnBnClicked_Eh_TestSkillCreate)
	ON_BN_CLICKED(IDC_EH_1228_BlackAlpha, &CTool_Effect::OnBnClicked_Eh_BlackAplha)
	ON_BN_CLICKED(IDC_EH_1228_BlackWhite, &CTool_Effect::OnBnClicked_Eh_BlackWhite)
	ON_BN_CLICKED(IDC_EH_0106_EffectTool_ResetImage, &CTool_Effect::OnBnClicked_Eh_Reset_Image)
	ON_BN_CLICKED(IDC_EH_0106_EffectTool_SaveFix, &CTool_Effect::OnBnClicked_Eh_SaveFix)
	ON_BN_CLICKED(IDC_EH_0117_EffectTool_Delete_Base, &CTool_Effect::OnBnClicked_Eh_DeleteBase)
	ON_BN_CLICKED(IDC_EH_0117_EffectTool_Delete_Filter_3, &CTool_Effect::OnBnClicked_Eh_DeleteFilter3)
	ON_BN_CLICKED(IDC_EH_0117_EffectTool_Delete_Filter_1, &CTool_Effect::OnBnClicked_Eh_DeleteFilter1)
	ON_BN_CLICKED(IDC_EH_0117_EffectTool_Delete_Filter_2, &CTool_Effect::OnBnClicked_Eh_DeleteFilter2)
	ON_BN_CLICKED(IDC_EH_0118_EffectTool_SkillList_Effect_Up, &CTool_Effect::OnBnClicked_Eh_Skilllist_EffectUp)
	ON_BN_CLICKED(IDC_EH_0118_EffectTool_SkillList_Effect_Down, &CTool_Effect::OnBnClicked_Eh_Skilllist_EffectDown)
	ON_BN_CLICKED(IDC_EH_0126_EffectTool_Effect_Search, &CTool_Effect::OnBnClicked_Eh_Search)
	ON_BN_CLICKED(IDC_EH_0126_EffectTool_Effect_Search_Reset, &CTool_Effect::OnBnClicked_Eh_SearchReset)
END_MESSAGE_MAP()


// CTool_Effect �޽��� ó�����Դϴ�.

// ȭ���� ���̸� ��Ÿ�̸� ������Ʈ�� �۵�, ������ ����Ʈ �˾��� ����
void CTool_Effect::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetTimer(TIMERID, (_uint)(1.f / 60.f), NULL);
		
		ReUpdate_Tool();
	}
	else
	{
		KillTimer(TIMERID);

		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectBase->ShowWindow(SW_HIDE);
		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectFilter->ShowWindow(SW_HIDE);
	}
}

// ����Ʈ ���� â�� ���� ��
void CTool_Effect::OnBnClicked_Eh_Open_Light()
{
	if (Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_LightOption->ShowWindow(SW_SHOW)) 
	{
		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_LightOption->ShowWindow(SW_HIDE);
	}
}


// ����Ʈ�� ���̾ �����Ѵ�
void CTool_Effect::OnBnClicked_Eh_Create()
{	
	// �̹��� �˻� 

	CString pPath = m_tEffectInfo.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"���õ� �̹����� �����ϴ�.");
		return;
	}

	Set_Info();
	
	CGameObject* pGameObject = CEffectObject::Create(Management GetDevice(), E_OBJTAG::OBJ_Effect, m_tEffectInfo);
	assert(pGameObject);
	Management Get_Scene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
}

// ��Ÿ�� ������Ʈ
void CTool_Effect::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (TIMERID != nIDEvent) 
	{
		return;
	}

	if (0.f < m_fEffectSaveSelTime) 
	{
		m_fEffectSaveSelTime -= 0.016f;
		if (0.f >= m_fEffectSaveSelTime)
		{
			m_fEffectSaveSelTime = 0.f;
			m_iEffectSaveSel = -1;
		}
	}


	if (Management Key_Pressing(KEY_A) && Management Key_Pressing(KEY_S) && Management Key_Pressing(KEY_D) && Management Key_Down(KEY_E))
	{
		AfxMessageBox(L"������ �ٺ�.");
	}
}

// ����Ʈ ������ �ݿ���
void CTool_Effect::OnBnClicked_Eh_Apply()
{
	Set_Info();
}

// ����Ʈ�� ��ǥ���� ���� ���͸� ��ư�� �ݿ���
void CTool_Effect::OnBnClicked_Eh_ColorEnd_Vec()
{
	// Į�� -> ��ư

	UpdateData(TRUE);

	D3DXCOLOR vColor = D3DXCOLOR(m_fColor_End_R, m_fColor_End_G, m_fColor_End_B, m_fColor_End_A);

	m_vColor_End_Button.SetColor(D3DCOLORTOCOLORREF(vColor));

	UpdateData(FALSE);
}

// ����Ʈ�� ���۰��� ���� ���͸� ��ư�� �ݿ���
void CTool_Effect::OnBnClicked_Eh_ColorStart_Vec()
{
	// Į�� -> ��ư

	UpdateData(TRUE);
	
	D3DXCOLOR vColor = D3DXCOLOR(m_fColor_Start_R, m_fColor_Start_G, m_fColor_Start_B, m_fColor_Start_A);

	m_vColor_Start_Button.SetColor(D3DCOLORTOCOLORREF(vColor));

	UpdateData(FALSE);
}

// ����Ʈ�� ���۰��� ���� ��ư�� ���� ���Ϳ� �ݿ���
void CTool_Effect::OnBnClicked_Eh_ColorStart_Button()
{
	// ��ư -> Į��

	UpdateData(TRUE);

	COLORREF dwColor = m_vColor_Start_Button.GetColor();
	
	m_fColor_Start_R = GetRGB255_TO_RGB1f(GetRValue(dwColor));
	m_fColor_Start_R = SetFloatColor(m_fColor_Start_R);
	m_fColor_Start_G = GetRGB255_TO_RGB1f(GetGValue(dwColor));
	m_fColor_Start_G = SetFloatColor(m_fColor_Start_G);
	m_fColor_Start_B = GetRGB255_TO_RGB1f(GetBValue(dwColor));
	m_fColor_Start_B = SetFloatColor(m_fColor_Start_B);

	UpdateData(FALSE);
}

// ����Ʈ�� ��ǥ���� ���� ��ư�� ���� ���Ϳ� �ݿ���
void CTool_Effect::OnBnClicked_Eh_ColorEnd_Button()
{
	// ��ư -> Į��

	UpdateData(TRUE);

	COLORREF dwColor = m_vColor_End_Button.GetColor();

	m_fColor_End_R = GetRGB255_TO_RGB1f(GetRValue(dwColor));
	m_fColor_End_R = SetFloatColor(m_fColor_End_R);
	m_fColor_End_G = GetRGB255_TO_RGB1f(GetGValue(dwColor));
	m_fColor_End_G = SetFloatColor(m_fColor_End_G);
	m_fColor_End_B = GetRGB255_TO_RGB1f(GetBValue(dwColor));
	m_fColor_End_B = SetFloatColor(m_fColor_End_B);

	UpdateData(FALSE);
}

// ����Ʈ ���� �� �ʱ�ȭ
void CTool_Effect::OnBnClicked_Eh_Reset()
{
	Reset_Info();

	LoadEffectList();

	LoadParticleList();

	LoadSkillList();
}

// ����Ʈ Ÿ�� ����
void CTool_Effect::OnCbnSelchange_Eh_Type()
{
	UpdateData(TRUE);

	_int iIndex = m_eEffectLoopType.GetCurSel();

	if (0 > iIndex || (_int)(E_EffectType::End) <= iIndex)
	{
		AfxMessageBox(L"��ȿ���� ���� �ε���");
		return;
	}

	m_tEffectInfo.eEffectType = (E_EffectType)iIndex;
}

// ����Ʈ�� ���۰��� ��� ������ ��ǥ���� ������
void CTool_Effect::OnBnClicked_Eh_Base2move()
{
	UpdateData(TRUE);
	
	m_fPos_End_X = m_fPos_Start_X;
	m_fPos_End_Y = m_fPos_Start_Y;
	m_fPos_End_Z = m_fPos_Start_Z;

	m_fRot_End_X = m_fRot_Start_X;
	m_fRot_End_Y = m_fRot_Start_Y;
	m_fRot_End_Z = m_fRot_Start_Z;

	m_fScale_End_X = m_fScale_Start_X;
	m_fScale_End_Y = m_fScale_Start_Y;
	m_fScale_End_Z = m_fScale_Start_Z;
	
	m_fColor_End_R = m_fColor_Start_R;
	m_fColor_End_G = m_fColor_Start_G;
	m_fColor_End_B = m_fColor_Start_B;
	m_fColor_End_A = m_fColor_Start_A;

	m_vColor_Start_Button;
	m_vColor_End_Button;

	D3DXCOLOR vColor_Start = D3DXCOLOR(m_fColor_Start_R, m_fColor_Start_G, m_fColor_Start_B, m_fColor_Start_A);
	m_vColor_Start_Button.SetColor(D3DCOLORTOCOLORREF(vColor_Start));
	m_vColor_End_Button.SetColor(D3DCOLORTOCOLORREF(vColor_Start));

	UpdateData(FALSE);
}

// ��ư�� ������ - ����Ʈ ���� ������
void CTool_Effect::OnBnClicked_Eh_ReUpdate()
{
	Management PrefabReload();

	UpdateData(TRUE);

	ReUpdate_Tool();

}

// ����Ʈ���̽� ���� â�� ���� ��
void CTool_Effect::OnBnClicked_Eh_Open_EffectTexture()
{
	if (Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectBase->ShowWindow(SW_SHOW))
	{
		// �̹� �������� �� -> ����
		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectBase->ShowWindow(SW_HIDE);
	}
	else
	{
		// �������� �� -> Ų�� + ��ó ����
		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectBase->Change_Picture();
		Get_MFC_MainFrame()->m_pMenuForm->m_pPopup_EffectBase->Set_TexPath(m_tEffectInfo);
	}
}

// ����Ʈ ����ü ���̺�
void CTool_Effect::OnBnClicked_Eh_Save()
{
	Set_Info();
		
	CFileDialog Dlg(FALSE, L"effect", m_EffectName, OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Effect\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();

		SaveEffectInfo(strFilePath, TRUE);
		
	}

	LoadEffectList();

	m_EffectName = L"None";

	UpdateData(FALSE);
}

// ����Ʈ ����ü �ε�
void CTool_Effect::OnBnClicked_Eh_Load()
{
	// TRUE�� ����
	CFileDialog Dlg(TRUE, L"effect", L"*.effect", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Effect\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		
		LoadEffectInfo(strFilePath, &m_tEffectInfo, TRUE);
				
		CFileFind finder;
		if (!finder.FindFile(strFilePath))
		{
			assert(FALSE);
		}
		finder.FindNextFile();
		m_EffectName = finder.GetFileTitle() + L".effect";

		UpdateData(FALSE);
	}

	m_EffectName = L"None";

	UpdateData(FALSE);
}


// ����Ʈ�� ������ �����մϴ�.
void CTool_Effect::OnBnClicked_Eh_SaveFix()
{	
	Set_Info();

	SaveEffectInfo(m_EffectName);	
}

// ����Ʈ ���̺� ���ϵ��� �ҷ��´�
void CTool_Effect::OnBnClicked_Eh_LoadEffectList()
{
	LoadEffectList();
}

// ��ƼŬ ���̺� ���ϵ��� �ҷ��´�
void CTool_Effect::OnBnClicked_Eh_LoadParticleList()
{
	LoadParticleList();
}

// ����Ʈ ������ ����Ʈ���� ����
void CTool_Effect::OnLbnSelchange_Eh_ListBox_EffectLoad()
{
	int iListCursor = m_listBoxEffectSave.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	// �ֱ��� ���� ����Ʈ�ڽ��� ��ġ�ϸ�
	if (iListCursor == m_iEffectSaveSel)
	{
		Edit_Effect(m_pPathEffect);
	}
	else
	{
		m_iEffectSaveSel = iListCursor;
		m_fEffectSaveSelTime = 0.5f;
	}

	CString wstrFileName;
	m_listBoxEffectSave.GetText(iListCursor, wstrFileName);

	m_pPathEffect = wstrFileName + L".effect";

	if (0 == m_pPathEffect.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Դϴ�.");
		return;
	}
}

// ��ƼŬ ������ ����Ʈ���� ����
void CTool_Effect::OnLbnSelchange_Eh_ListBox_ParticleLoad()
{
	int iListCursor = m_listBoxParticleSave.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	CString wstrFileName;
	m_listBoxParticleSave.GetText(iListCursor, wstrFileName);

	m_pPathParticle = wstrFileName + L".particle";

	if (0 == m_pPathParticle.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Դϴ�.");
		return;
	}
}

// ����Ʈ���� ������ ����Ʈ�� ���� �ݿ�
void CTool_Effect::OnBnClicked_Eh_PlayList_Effect()
{
	Edit_Effect(m_pPathEffect);
}

// ����Ʈ���� ������ ��ƼŬ�� ����
void CTool_Effect::OnBnClicked_Eh_PlayList_Particle()
{
	if (0 == m_pPathParticle.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"��ƼŬ�� ������ �ּ���.");
		return;
	}

	CGameObject* pGameObject = CParticleObject::Create(Management GetDevice(), E_OBJTAG::OBJ_Particle, m_pPathParticle.GetString());
	assert(pGameObject);

	Management Get_Scene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pGameObject);
}

// ����Ʈ���� ������ ����Ʈ�� ��ų�� �߰�
void CTool_Effect::OnBnClicked_Eh_AddSkill_Effect()
{
	m_listBox_Skill_Effect.AddString(m_pPathEffect);

	UpdateData(FALSE);
}

// ����Ʈ���� ������ ��ƼŬ�� ��ų�� �߰�
void CTool_Effect::OnBnClicked_Eh_AddSkill_Particle()
{
	m_listBox_Skill_Particle.AddString(m_pPathParticle);

	UpdateData(FALSE);
}

// ��ų ��ü ����
void CTool_Effect::OnBnClicked_Eh_SkillList_Play()
{
	int iMaxParticle = m_listBox_Skill_Particle.GetCount();
	int iMaxEffect = m_listBox_Skill_Effect.GetCount();

	for (_int i = 0; i < iMaxEffect; i++)
	{
		CString pPath;
		m_listBox_Skill_Effect.GetText(i, pPath);

		T_EffectInfo tInfo = T_EffectInfo();
		LoadEffectInfo(pPath, &tInfo);
		
		CGameObject* pGameObject = CEffectObject::Create(Management GetDevice(), E_OBJTAG::OBJ_Effect, tInfo);
		assert(pGameObject);

		Management Get_Scene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	}

	for (_int i = 0; i < iMaxParticle; i++)
	{
		CString pPath;
		m_listBox_Skill_Particle.GetText(i, pPath);

		CGameObject* pGameObject = CParticleObject::Create(Management GetDevice(), E_OBJTAG::OBJ_Particle, pPath.GetString());
		assert(pGameObject);

		Management Get_Scene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pGameObject);
	}
}

//��ų ���̺�
void CTool_Effect::OnBnClicked_Eh_SkillList_Save()
{
	// FALSE�� �ٸ� �̸����� ����
	//CFileDialog Dlg(FALSE, L"skill", L"*.skill", OFN_OVERWRITEPROMPT);
	CFileDialog Dlg(FALSE, L"skill", m_testSkill, OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Skill\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD dwbyte = 0;
				
		_int iSizeEffect = m_listBox_Skill_Effect.GetCount();
		CUtility::Data_WriteInt(hFile, iSizeEffect, dwbyte);
		for (_int i = 0; i < iSizeEffect; i++)
		{
			CString pPath;
			m_listBox_Skill_Effect.GetText(i, pPath);
			CUtility::Data_WriteWString(hFile, pPath.GetString(), dwbyte);
		}

		_int iSizeParticle = m_listBox_Skill_Particle.GetCount();
		CUtility::Data_WriteInt(hFile, iSizeParticle, dwbyte);
		for (_int i = 0; i < iSizeParticle; i++)
		{
			CString pPath;
			m_listBox_Skill_Particle.GetText(i, pPath);
			CUtility::Data_WriteWString(hFile, pPath.GetString(), dwbyte);
		}

		CloseHandle(hFile);
	}

	LoadSkillList();
}

//��ų �ε�
void CTool_Effect::OnBnClicked_Eh_SkillList_Load()
{
	m_listBox_Skill_Effect.ResetContent();
	m_listBox_Skill_Particle.ResetContent();

	UpdateData(FALSE);

	// TRUE�� ����
	CFileDialog Dlg(TRUE, L"skill", L"*.skill", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Skill\\");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		DWORD dwbyte = 0;
		_int iSize = 0;

		CUtility::Data_ReadInt(hFile, iSize, dwbyte);
		for (_int i = 0; i < iSize; i++)
		{
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrPath, dwbyte);
			CString pPath = wstrPath.c_str();
			m_listBox_Skill_Effect.AddString(pPath);
		}

		CUtility::Data_ReadInt(hFile, iSize, dwbyte);
		for (_int i = 0; i < iSize; i++)
		{
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrPath, dwbyte);
			CString pPath = wstrPath.c_str();
			m_listBox_Skill_Particle.AddString(pPath);
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}

// ��ų����Ʈ_����Ʈ ��ü����
void CTool_Effect::OnBnClicked_Eh_Skill_List_Effect_DeleteAll()
{
	m_listBox_Skill_Effect.ResetContent();
}

// ��ų���� _����Ʈ ���û���
void CTool_Effect::OnBnClicked_Eh_Skill_List_Effect_Delete()
{
	int iListCursor = m_listBox_Skill_Effect.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	m_listBox_Skill_Effect.DeleteString(iListCursor);

	UpdateData(FALSE);
}

// ��ų����Ʈ_��ƼŬ ��ü����
void CTool_Effect::OnBnClicked_Eh_Skill_List_Particle_DeleteAll()
{
	m_listBox_Skill_Particle.ResetContent();
}

// ��ų����Ʈ_��ƼŬ ���û���
void CTool_Effect::OnBnClicked_Eh_Skill_List_Particle_Delete()
{
	int iListCursor = m_listBox_Skill_Particle.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	m_listBox_Skill_Particle.DeleteString(iListCursor);

	UpdateData(FALSE);
}

// ��ų ����Ʈ�ڽ� ����
void CTool_Effect::OnBnClicked_Eh_Skill_List_Update()
{
	LoadSkillList();
}

// ����Ʈ, ��ƼŬ ����Ʈ �ڽ� ���� ����
void CTool_Effect::OnBnClicked_Eh_Skill_List_DeleteAll()
{
	m_listBox_Skill_Effect.ResetContent();
	m_listBox_Skill_Particle.ResetContent();
	UpdateData(FALSE);
}

// ��ų ����
void CTool_Effect::OnLbnSelchangeOn_Eh_Listbox_Skill_Save()
{
	int iListCursor = m_listBox_Skill.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	m_listBox_Skill_Effect.ResetContent();
	m_listBox_Skill_Particle.ResetContent();

	UpdateData(FALSE);

	CString wstrFileName = L"None";
	m_listBox_Skill.GetText(iListCursor, wstrFileName);
	m_testSkill = wstrFileName;

	wstrFileName = L"../../Reference/Data/Data_Skill/" + wstrFileName;


	UpdateData(FALSE);

	HANDLE hFile = CreateFile(wstrFileName.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}

	DWORD dwbyte = 0;
	_int iSize = 0;

	CUtility::Data_ReadInt(hFile, iSize, dwbyte);
	for (_int i = 0; i < iSize; i++)
	{
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile, wstrPath, dwbyte);
		CString pPath = wstrPath.c_str();
		m_listBox_Skill_Effect.AddString(pPath);
	}

	CUtility::Data_ReadInt(hFile, iSize, dwbyte);
	for (_int i = 0; i < iSize; i++)
	{
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile, wstrPath, dwbyte);
		CString pPath = wstrPath.c_str();
		m_listBox_Skill_Particle.AddString(pPath);
	}

	CloseHandle(hFile);

	UpdateData(FALSE);
}

// �׽�Ʈ�� ��ų�� �ʿ� ���� ����ϴ�. 
// Ŭ���̾�Ʈ ��ų ������ ����
void CTool_Effect::OnBnClicked_Eh_TestSkillCreate()
{
	// ����� ��ų�� ����մϴ�.
	// mfc������ CString���� üũ�Ͽ����ϴ�. ������ ���ڿ��� ��ȿ�� �Ǵ��մϴ�.
	if (0 == m_testSkill.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�߸��� ����Դϴ�.\n����Ʈ���� ����� ��ų�� �������ּ���.");
		return;
	}

	// ��ų �̸��� �޾ƿ´�.
	wstring tagSkill = m_testSkill.GetString();
	_vec3 vPos = _vec3(0.f, 0.f, 0.f);
	_vec3 vRot = _vec3(0.f, 0.f, 0.f);
	
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	// ��ų ���� ����
	
	// ��ų �̸��� wstring(const wstring ����)�϶�
	Management SetSkill(tagSkill, E_LAYERTAG::Effect, vPos, vRot);
	

	// ��ų�̸��� ���ͳ� ��� �϶� .ex) L"3.skill"  
	//Management SetSkill(L"3.skill", E_LAYERTAG::Effect, vPos, vRot);


	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
}

// �̹����� ���� ����ȭ�Ѵ�
void CTool_Effect::OnBnClicked_Eh_BlackAplha()
{
	UpdateData(TRUE);

	if (m_bBlackAlpha)
	{
		m_tEffectInfo.bBlackAlpha = TRUE;
	}
	else
	{
		m_tEffectInfo.bBlackAlpha = FALSE;
	}

	Get_Info();
}

// �̹����� ���ȭ �Ѵ�
void CTool_Effect::OnBnClicked_Eh_BlackWhite()
{
	UpdateData(TRUE);

	if (m_bBlackWhite)
	{
		m_tEffectInfo.bBlackWhite = TRUE;
	}
	else
	{
		m_tEffectInfo.bBlackWhite = FALSE;
	}

	Get_Info();
}

// �̹��� â�� ���ϴ�.
void CTool_Effect::OnBnClicked_Eh_Reset_Image()
{
	UpdateData(TRUE);

	m_tEffectInfo.textPath = L"None";
	m_tEffectInfo.filterPath_1 = L"None";
	m_tEffectInfo.filterPath_2 = L"None";
	m_tEffectInfo.filterPath_3 = L"None";

	CString pPath = m_tEffectInfo.textPath.c_str();
	DrawPicture(m_pictureEffect_Base.m_hWnd, pPath, FALSE, TRUE);
	pPath = m_tEffectInfo.filterPath_1.c_str();
	DrawPicture(m_pictureEffect_Filter_1.m_hWnd, pPath);
	pPath = m_tEffectInfo.filterPath_2.c_str();
	DrawPicture(m_pictureEffect_Filter_2.m_hWnd, pPath);
	pPath = m_tEffectInfo.filterPath_3.c_str();
	DrawPicture(m_pictureEffect_Filter_3.m_hWnd, pPath);

	UpdateData(FALSE);
}

void CTool_Effect::OnBnClicked_Eh_DeleteBase()
{
	UpdateData(TRUE);

	m_tEffectInfo.textPath = L"None";
	CString pPath = m_tEffectInfo.textPath.c_str();
	DrawPicture(m_pictureEffect_Base.m_hWnd, pPath, FALSE, TRUE);

	UpdateData(FALSE);
}

void CTool_Effect::OnBnClicked_Eh_DeleteFilter1()
{
	UpdateData(TRUE);

	m_tEffectInfo.filterPath_1 = L"None";
	CString pPath = m_tEffectInfo.filterPath_1.c_str();
	DrawPicture(m_pictureEffect_Filter_1.m_hWnd, pPath);

	UpdateData(FALSE);
}


void CTool_Effect::OnBnClicked_Eh_DeleteFilter2()
{
	UpdateData(TRUE);

	m_tEffectInfo.filterPath_2 = L"None";
	CString pPath = m_tEffectInfo.filterPath_2.c_str();
	DrawPicture(m_pictureEffect_Filter_2.m_hWnd, pPath);

	UpdateData(FALSE);
}


void CTool_Effect::OnBnClicked_Eh_DeleteFilter3()
{
	UpdateData(TRUE);

	m_tEffectInfo.filterPath_3 = L"None";
	CString pPath = m_tEffectInfo.filterPath_3.c_str();
	DrawPicture(m_pictureEffect_Filter_3.m_hWnd, pPath);

	UpdateData(FALSE);
}



void CTool_Effect::OnBnClicked_Eh_Skilllist_EffectUp()
{
	UpdateData(TRUE);

	_int iSel = m_listBox_Skill_Effect.GetCurSel();
	
	if (-1 == iSel || 0 == iSel)
	{
		return;
	}
	
	CString strTemp = L"None";
	m_listBox_Skill_Effect.GetText(iSel, strTemp);
	m_listBox_Skill_Effect.DeleteString(iSel);

	m_listBox_Skill_Effect.InsertString(iSel - 1,strTemp);

	m_listBox_Skill_Effect.SetCurSel(iSel - 1);

	UpdateData(FALSE);
}


void CTool_Effect::OnBnClicked_Eh_Skilllist_EffectDown()
{
	UpdateData(TRUE);

	_int iSel = m_listBox_Skill_Effect.GetCurSel();


	if (-1 == iSel || m_listBox_Skill_Effect.GetCount() == iSel)
	{
		return;
	}
	
	CString strTemp = L"None";
	m_listBox_Skill_Effect.GetText(iSel, strTemp);
	m_listBox_Skill_Effect.DeleteString(iSel);

	m_listBox_Skill_Effect.InsertString(iSel + 1, strTemp);

	m_listBox_Skill_Effect.SetCurSel(iSel + 1);

	UpdateData(FALSE);
}


void CTool_Effect::OnBnClicked_Eh_Search()
{
	UpdateData(TRUE);

	if (0 == m_strSearch.CompareNoCase(L""))
	{
		m_bSearch = FALSE;
	}
	else
	{
		m_bSearch = TRUE;
	}
	
	LoadEffectList();
}


void CTool_Effect::OnBnClicked_Eh_SearchReset()
{
	m_bSearch = FALSE;

	m_strSearch = L"";

	UpdateData(FALSE);

	LoadEffectList();
}
