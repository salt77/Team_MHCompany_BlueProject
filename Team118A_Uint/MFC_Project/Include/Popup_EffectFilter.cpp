// Popup_EffectFilter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "Popup_EffectFilter.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CPopup_EffectFilter, CDialogEx)

CPopup_EffectFilter::CPopup_EffectFilter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EH_1226_POPUP_EFFECTFILTER, pParent)
	, m_pPath_FilterImage(_T(""))
{
	m_iTypeIndex = 0;
	m_iFilterIndex = 0;
	m_pathFilterTex = L"None";
}

CPopup_EffectFilter::~CPopup_EffectFilter()
{
}

void CPopup_EffectFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EH_1226_EffectFilter_FilePath, m_pPath_FilterImage);
	DDX_Control(pDX, IDC_EH_1226_EffectFilter_Picture, m_pictureFilter);
	DDX_Control(pDX, IDC_EH_1226_EffectFilter_ComboBox, m_comboBoxType);
	DDX_Control(pDX, IDC_EH_1226_EffectFilter_ListBox, m_listBoxFilter);
}

void CPopup_EffectFilter::Change_Picture()
{
	if (0 != m_pathFilterTex.CompareNoCase(L"None"))
	{
		Change_Picture_EffectFilter(&m_pathFilterTex, &m_pictureFilter);
	}
}

void CPopup_EffectFilter::Change_Picture_EffectFilter(CString * pTexPath, CStatic * pPicture)
{
	if (pTexPath && 0 == pTexPath->CompareNoCase(L"None"))
	{
		AfxMessageBox(L"이미지가 선택되지 않았습니다.");
		return;
	}


	Management GetDevice()->Clear(0, nullptr,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		Management Get_LightBackBufferColor(), 1.f, 0);
	Management GetDevice()->BeginScene();

	if (!pTexPath)
	{
		Management GetDevice()->EndScene();
		Management GetDevice()->Present(NULL, NULL, pPicture->m_hWnd, NULL);
		return;
	}

	CTexture*			pTexture = CTexture::Create(Management GetDevice(), pTexPath->operator LPCWSTR());
	CShader*			pShader = static_cast<CShader*>(Management Clone_Prototype(L"Proto_Shader_Effect"));
	CRectBuffer*		pBuffer = static_cast<CRectBuffer*>(Management Clone_Prototype(L"Proto_Buffer_RectTex"));

	if (!pTexture || !pShader || !pBuffer) 
	{
		Management GetDevice()->EndScene();
		Management GetDevice()->Present(NULL, NULL, pPicture->m_hWnd, NULL);

		Safe_Release(pBuffer);
		Safe_Release(pShader);
		Safe_Release(pTexture);

		return;
	}

	////////////////////////////////////////////////////////////////

	_matrix	matView, matProj, matWorld, matScale, matRot, matTrans;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixRotationZ(&matRot, 0.f * Deg2Rad);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	
	matWorld = matScale * matRot * matTrans;
	
	////////////////////////////////////////////////////////////////

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));

	pTexture->Set_Texture(pEffect, "g_BaseTexture", 0);

	pEffect->SetFloat("g_fU", 0.f);
	pEffect->SetFloat("g_fV", 0.f);
	pEffect->SetFloat("g_fU_Scale", 1.f);
	pEffect->SetFloat("g_fV_Scale", 1.f);

	pEffect->SetBool("g_bFilter_1", FALSE);
	pEffect->SetBool("g_bFilter_2", FALSE);
	pEffect->SetBool("g_bFilter_3", FALSE);

	pEffect->SetBool("g_bBlackAlpha", FALSE);
	pEffect->SetBool("g_bBlackWhite", FALSE);

	////////////////////////////////////////////////////////////////
	
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	pShader->BeginShader();
	pShader->BeginPass(0);

	pBuffer->Render_Buffer();

	pShader->EndPass();
	pShader->EndShader();

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	////////////////////////////////////////////////////////////////

	Management GetDevice()->EndScene();
	Management GetDevice()->Present(NULL, NULL, pPicture->m_hWnd, NULL);

	Safe_Release(pBuffer);
	Safe_Release(pShader);
	Safe_Release(pTexture);
}

void CPopup_EffectFilter::ReLoad()
{
	if (0 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (1 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Fade/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (2 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Ramp/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else
	{
		AfxMessageBox(L"유효하지 않은 인덱스");
	}

	m_pPath_FilterImage = L"None";
}

void CPopup_EffectFilter::Load_ResourceData(CString pPath, CListBox * pListBox)
{
	UpdateData(TRUE);

	pListBox->ResetContent();

	CFileFind finder;
	BOOL bExist = finder.FindFile(pPath);

	//경로가 유효하지 않음
	if (!bExist)
	{
		assert(FALSE);
	}
	while (bExist)
	{
		bExist = finder.FindNextFile();
		pListBox->AddString(finder.GetFileTitle());
	}

	pListBox->SetHorizontalExtent(300);

	UpdateData(FALSE);
}

BOOL CPopup_EffectFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboBoxType.AddString(L"Desolve");
	m_comboBoxType.AddString(L"Fade");
	m_comboBoxType.AddString(L"Color");
	m_comboBoxType.SetCurSel(0);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CPopup_EffectFilter, CDialogEx)
	ON_BN_CLICKED(IDC_EH_1226_EffectFilter_ButtonType, &CPopup_EffectFilter::OnBnClicked_Eh_Button_FilterType)
	ON_BN_CLICKED(IDC_EH_1227_EffectFilter_Delete, &CPopup_EffectFilter::OnBnClicked_Eh_Delete)
	ON_CBN_SELCHANGE(IDC_EH_1226_EffectFilter_ComboBox, &CPopup_EffectFilter::OnCbnSelchange_Eh_Filter_Combobox)
	ON_LBN_SELCHANGE(IDC_EH_1226_EffectFilter_ListBox, &CPopup_EffectFilter::OnLbnSelchange_Eh_Filter_Listbox)
	ON_BN_CLICKED(IDOK, &CPopup_EffectFilter::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EH_1227_EffectFilter_Upadte, &CPopup_EffectFilter::OnBnClicked_Eh_Upadte)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// 필터의 이미지 타입을 다음으로 바꿉니다.
void CPopup_EffectFilter::OnBnClicked_Eh_Button_FilterType()
{
	m_iTypeIndex++;

	if (m_iTypeIndex > 2)
	{
		m_iTypeIndex = 0;
	}

	m_comboBoxType.SetCurSel(m_iTypeIndex);

	if (0 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (1 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Fade/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (2 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Ramp/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else
	{
		AfxMessageBox(L"유효하지 않은 인덱스");
	}

	m_pPath_FilterImage = L"None";

	UpdateData(FALSE);
}

// 현재 이미지를 지웁니다.
void CPopup_EffectFilter::OnBnClicked_Eh_Delete()
{
	m_pPath_FilterImage = L"None";
	m_pathFilterTex = L"None";

	UpdateData(FALSE);
}

// 현재 필터 타입을 선택합니다.
void CPopup_EffectFilter::OnCbnSelchange_Eh_Filter_Combobox()
{
	UpdateData(TRUE);
	
	m_iTypeIndex = m_comboBoxType.GetCurSel();

	if (0 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (1 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Fade/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else if (2 == m_iTypeIndex)
	{
		CString pPath = L"../../Reference/Resource/Effect/Filter_Ramp/*.png";

		Load_ResourceData(pPath, &m_listBoxFilter);

		Change_Picture_EffectFilter(nullptr, &m_pictureFilter);
	}
	else
	{
		AfxMessageBox(L"유효하지 않은 인덱스");
	}

	m_pPath_FilterImage = L"None";
	m_pathFilterTex = L"None";

	UpdateData(FALSE);
}

// 필터의 이미지를 선택합니다.
void CPopup_EffectFilter::OnLbnSelchange_Eh_Filter_Listbox()
{
	int iListCursor = m_listBoxFilter.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"리스트 박스의 리스트를 클릭해주세요.");
		return;
	}

	CString wstrFileName;
	m_listBoxFilter.GetText(iListCursor, wstrFileName);

	if (0 == m_iTypeIndex)
	{
		wstrFileName = L"../../Reference/Resource/Effect/Filter/" + wstrFileName + L".png";
	}
	else if (1 == m_iTypeIndex)
	{
		wstrFileName = L"../../Reference/Resource/Effect/Filter_Fade/" + wstrFileName + L".png";
	}
	else if (2 == m_iTypeIndex)
	{
		wstrFileName = L"../../Reference/Resource/Effect/Filter_Ramp/" + wstrFileName + L".png";
	}

	m_pPath_FilterImage = wstrFileName.operator LPCWSTR();
	m_pathFilterTex = wstrFileName.operator LPCWSTR();

	Change_Picture_EffectFilter(&wstrFileName, &m_pictureFilter);

	UpdateData(FALSE);
}

// 이미지의 정보를 툴로 넘깁니다. 
void CPopup_EffectFilter::OnBnClickedOk()
{			
	//Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Filter_EffectBase(m_pathFilterTex, m_iFilterIndex);
	
	CDialogEx::OnOK();
}

// 업데이트하여 이미지를 다시 로드합니다.
void CPopup_EffectFilter::OnBnClicked_Eh_Upadte()
{
	ReLoad();
}

// 창이 켜지면 리로드합니다
void CPopup_EffectFilter::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		ReLoad();
	}
}
