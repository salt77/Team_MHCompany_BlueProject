// Popup_EffectBase.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "Popup_EffectBase.h"
#include "afxdialogex.h"


// Popup_EffectBase ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPopup_EffectBase, CDialogEx)

CPopup_EffectBase::CPopup_EffectBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EH_POPUP_EFFECTBASE, pParent)
	, m_bCheckBox_UVTex(TRUE)
	, m_iSpriteIndex(0)
	, m_fSpriteTime(0.2f)
	, m_iSprite_Row(1) 
	, m_iSprite_Column(1)
	, m_bBlackWhite(FALSE)
	, m_bBlackAlpha(FALSE)
	, m_bIsSprite(FALSE)
	, m_textBaseTexturePath(_T(""))
	, m_pathView_Base(_T("None"))
	, m_pathView_Filter_1(_T("None"))
	, m_pathView_Filter_2(_T("None"))
	, m_pathView_Filter_4(_T("None"))
{
	m_pathDefualtTex = L"../../Reference/Resource/Effect/BackGrid.png";
	m_pathBaseTex = L"../../Reference/Resource/Effect/Tex_Single/*.png";
}

CPopup_EffectBase::~CPopup_EffectBase()
{
}

void CPopup_EffectBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EH_1220_EffectBaseView_ListBox_EffectBase, m_listBox_EffectBase);
	DDX_Control(pDX, IDC_EH_1220_EffectBaseView_Picture_EffectBase, m_pictureEffectBase);
	DDX_Control(pDX, IDC_EH_1222_EffectBaseView_Picture_EffectBase_EMesh, m_pictureEMeshView);
	DDX_Control(pDX, IDC_EH_1222_EffectBaseView_ListBox_EffectBase_EMesh, m_listBox_EMesh);
	DDX_Check(pDX, IDC_EH_1222_EfectBase_CheckBox_UVTex, m_bCheckBox_UVTex);
	DDX_Control(pDX, IDC_EH_0115_EffectBaseView_TextureFolder, m_comboBox_TextureFolder);
	DDX_Text(pDX, IDC_EH_1223_EffectBaseView_SpriteIndex, m_iSpriteIndex);
	DDX_Text(pDX, IDC_EH_1223_EffectBaseView_PlaySpeed, m_fSpriteTime);
	DDX_Text(pDX, IDC_EH_1223_EffectBaseView_Row, m_iSprite_Row);
	DDX_Text(pDX, IDC_EH_1223_EffectBaseView_Column, m_iSprite_Column);
	DDX_Control(pDX, IDC_EH_1223_EffectBaseView_Picture_SpriteTex, m_pictureSpriteView);
	DDX_Check(pDX, IDC_EH_1228_EffecBase_BlackWhite, m_bBlackWhite);
	DDX_Check(pDX, IDC_EH_1228_EffecBase_BlackAlpha, m_bBlackAlpha);
	DDX_Check(pDX, IDC_EH_0115_IsSpriteCheckBox, m_bIsSprite);
	DDX_Text(pDX, IDC_EH_0115_Pick_ImageText, m_textBaseTexturePath);
	DDX_Text(pDX, IDC_EH_0117_Base_ImageText, m_pathView_Base);
	DDX_Text(pDX, IDC_EH_0117_Filter_1_ImageText, m_pathView_Filter_1);
	DDX_Text(pDX, IDC_EH_0117_Filter_2_ImageText, m_pathView_Filter_2);
	DDX_Text(pDX, IDC_EH_0117_Filter_4_ImageText, m_pathView_Filter_4);
}

// ����Ʈ ������ �� �並 ������ �� ���� �⺻ �׸����� ������
void CPopup_EffectBase::Change_Picture()
{
	m_fMeshAngle = 0.f;

	CString pTexPath = m_tEffectTexture.textPath.c_str();
	if (0 != pTexPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&pTexPath, &m_pictureEffectBase);

		if (m_tEffectTexture.bIsSprite)
		{
			Change_Picture_EffectBase(&pTexPath, &m_pictureSpriteView, nullptr, m_tEffectTexture.bIsSprite);
		}
		else
		{
			Change_Picture_EffectBase(nullptr, &m_pictureSpriteView);
		}
	}

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath);
	}
}

void CPopup_EffectBase::Set_TexPath(T_EffectInfo tInfo)
{
	UpdateData(TRUE);

	CString strPath = L"path";

	m_pathView_Base = tInfo.textPath.c_str();
	AfxExtractSubString(m_pathView_Base, tInfo.textPath.c_str(), 5, '/');
	AfxExtractSubString(strPath, tInfo.textPath.c_str(), 6, '/');
	m_pathView_Base = m_pathView_Base + L"/" + strPath;

	m_pathView_Filter_1 = tInfo.filterPath_1.c_str();
	AfxExtractSubString(m_pathView_Filter_1, tInfo.filterPath_1.c_str(), 5, '/');
	AfxExtractSubString(strPath, tInfo.filterPath_1.c_str(), 6, '/');
	m_pathView_Filter_1 = m_pathView_Filter_1 + L"/" + strPath;

	m_pathView_Filter_2 = tInfo.filterPath_2.c_str();
	AfxExtractSubString(m_pathView_Filter_2, tInfo.filterPath_2.c_str(), 5, '/');
	AfxExtractSubString(strPath, tInfo.filterPath_2.c_str(), 6, '/');
	m_pathView_Filter_2 = m_pathView_Filter_2 + L"/" + strPath;

	m_pathView_Filter_4 = tInfo.filterPath_3.c_str();
	AfxExtractSubString(m_pathView_Filter_4, tInfo.filterPath_3.c_str(), 5, '/');
	AfxExtractSubString(strPath, tInfo.filterPath_3.c_str(), 6, '/');
	m_pathView_Filter_4 = m_pathView_Filter_4 + L"/" + strPath;


	UpdateData(FALSE);
}

// �޽��� ������ �޺� ��ư�� �ɼǵ��� ������
BOOL CPopup_EffectBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	m_comboBox_TextureFolder.AddString(L"Single");
	m_comboBox_TextureFolder.AddString(L"Sprite");
	m_comboBox_TextureFolder.AddString(L"Filter");
	m_comboBox_TextureFolder.AddString(L"Filter_Fade");
	m_comboBox_TextureFolder.AddString(L"Filter_Ramp");
	m_comboBox_TextureFolder.AddString(L"Etc");
	m_comboBox_TextureFolder.SetCurSel(0);
	m_iTextureFolder = 0;
	
	m_fMeshAngle = 0.f;

	return TRUE;
}

// �ش� ����Ʈ�ڽ�(pListBox)�� �ʱ�ȭ�ϰ� pPath(����� ���� ����)�� �ҷ���
void CPopup_EffectBase::Load_ResourceData(CString pPath, CListBox* pListBox)
{
	UpdateData(TRUE);
	
	pListBox->ResetContent();

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
		pListBox->AddString(finder.GetFileTitle());
	}

	pListBox->SetHorizontalExtent(300);

	UpdateData(FALSE);
}

// pPicture(��ó��Ʈ��)��  pTexPath(�Ǵ� pMeshPath)�� �̹���(�Ǵ� �𵨸�)�� ǥ����
void CPopup_EffectBase::Change_Picture_EffectBase(CString* pTexPath, CStatic* pPicture, CString* pMeshPath, _bool bSprtiteDraw)
{
	if (pTexPath && 0 == pTexPath->CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� ���õ��� �ʾҽ��ϴ�.");
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
	CRectBuffer*		pBuffer = nullptr;
	CEMesh*				pEMesh = nullptr;

	_matrix	matView, matProj, matWorld, matScale, matRot, matTrans;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	if (!pMeshPath)
	{
		// �ؽ���
		pBuffer = static_cast<CRectBuffer*>(Management Clone_Prototype(L"Proto_Buffer_RectTex"));

		D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
		D3DXMatrixRotationZ(&matRot, 0.f * Deg2Rad);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	}
	else
	{
		// �޽�
		pEMesh = CEMesh::Create(Management GetDevice(), pMeshPath->operator LPCWSTR());

		float fSize = 0.5f * m_fMeshScale;
		D3DXMatrixScaling(&matScale, fSize, fSize, fSize);
		D3DXMatrixRotationY(&matRot, m_fMeshAngle * -Deg2Rad);
		D3DXMatrixTranslation(&matTrans, 0.f, -0.2f, 1.8f);

		D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), WINCX / (_float)WINCY, 0.1f, 1000.f);
	}

	matWorld = matScale * matRot * matTrans;

	_int iPosAddX = 0;
	_int iPosAddY = 0;
	_float fScaleAddX = 1.f;
	_float fScaleAddY = 1.f;

	if (bSprtiteDraw)
	{
		iPosAddX = m_iSpriteIndex % m_iSprite_Column;

		if (0 > iPosAddX || m_iSprite_Column <= iPosAddX)
		{
			AfxMessageBox(L"��������Ʈ�� ���� �߸��Ǿ����ϴ�.");

			m_iSpriteIndex = 0;
			UpdateData(FALSE);

			Management GetDevice()->EndScene();
			Management GetDevice()->Present(NULL, NULL, m_pictureSpriteView.m_hWnd, NULL);

			Safe_Release(pBuffer);
			Safe_Release(pShader);
			Safe_Release(pTexture);

			return;
		}

		iPosAddY = (m_iSpriteIndex - iPosAddX) / m_iSprite_Column;

		if (0 > iPosAddY || m_iSprite_Row <= iPosAddY)
		{
			AfxMessageBox(L"��������Ʈ�� ���� �߸��Ǿ����ϴ�.");

			m_iSpriteIndex = 0;
			UpdateData(FALSE);

			Management GetDevice()->EndScene();
			Management GetDevice()->Present(NULL, NULL, m_pictureSpriteView.m_hWnd, NULL);

			Safe_Release(pBuffer);
			Safe_Release(pShader);
			Safe_Release(pTexture);

			return;
		}

		fScaleAddX = 1.f / (_float)m_iSprite_Column;
		fScaleAddY = 1.f / (_float)m_iSprite_Row;
	}

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));

	pTexture->Set_Texture(pEffect, "g_BaseTexture", 0);

	pEffect->SetFloat("g_fU", (_float)iPosAddX);
	pEffect->SetFloat("g_fV", (_float)iPosAddY);
	pEffect->SetFloat("g_fU_Scale", fScaleAddX);
	pEffect->SetFloat("g_fV_Scale", fScaleAddY);

	pEffect->SetBool("g_bFilter_1", FALSE);
	pEffect->SetBool("g_bFilter_2", FALSE);
	pEffect->SetBool("g_bFilter_3", FALSE);

	pEffect->SetBool("g_bBlackAlpha", m_bBlackAlpha);
	pEffect->SetBool("g_bBlackWhite", m_bBlackWhite);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pShader->BeginShader();
	pShader->BeginPass(0);

	if (pEMesh)
	{
		pEMesh->Render_Meshes();
	}
	else
	{
		pBuffer->Render_Buffer();
	}

	pShader->EndPass();
	pShader->EndShader();
	
	Management GetDevice()->EndScene();
	Management GetDevice()->Present(NULL, NULL, pPicture->m_hWnd, NULL);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	Safe_Release(pBuffer);
	Safe_Release(pShader);
	Safe_Release(pTexture);
	Safe_Release(pEMesh);
}

BEGIN_MESSAGE_MAP(CPopup_EffectBase, CDialogEx)
	ON_BN_CLICKED(IDC_EH_1219_EffectBaseView_ResourceLoad, &CPopup_EffectBase::OnBnClicked_Eh_Load_Resource)
	ON_LBN_SELCHANGE(IDC_EH_1220_EffectBaseView_ListBox_EffectBase, &CPopup_EffectBase::OnLbnSelchange_Eh_Listbox_Effectbase)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshLoad, &CPopup_EffectBase::OnBnClicked_Eh_Load_EMesh)
	ON_LBN_SELCHANGE(IDC_EH_1222_EffectBaseView_ListBox_EffectBase_EMesh, &CPopup_EffectBase::OnLbnSelchange_Eh_Listbox_Emesh)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshRot2, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_1)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshCenter, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_Reset)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshRot, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_2)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshScale10, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_1)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshScale15, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_Reset)
	ON_BN_CLICKED(IDC_EH_1222_EffectBaseView_EMeshScale20, &CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_2)
	ON_BN_CLICKED(IDC_EH_1222_EfectBase_CheckBox_UVTex, &CPopup_EffectBase::OnBnClicked_Eh_CheckBox_UVTex)
	ON_CBN_SELCHANGE(IDC_EH_0115_EffectBaseView_TextureFolder, &CPopup_EffectBase::OnCbnSelchange_Eh_TextureFoler)
	ON_BN_CLICKED(IDC_EH_1223_EffectBaseView_Play, &CPopup_EffectBase::OnBnClicked_Eh_Play)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_EH_1226_EffectBaseView_Update, &CPopup_EffectBase::OnBnClicked_Eh_Update)
	ON_BN_CLICKED(IDC_EH_1228_EffecBase_BlackWhite, &CPopup_EffectBase::OnBnClicked_Eh_BlackWhite)
	ON_BN_CLICKED(IDC_EH_1228_EffecBase_BlackAlpha, &CPopup_EffectBase::OnBnClicked_Eh_BlackAlpha)
	ON_BN_CLICKED(IDC_EH_0115_IsSpriteCheckBox, &CPopup_EffectBase::OnBnClicked_EH_Set_CheckBox_IsSprite)
	ON_BN_CLICKED(IDC_EH_0116_EffectBaseView_SetBase, &CPopup_EffectBase::OnBnClicked_Eh_SetBase)
	ON_BN_CLICKED(IDC_EH_0116_EffectBaseView_SetFilter_1, &CPopup_EffectBase::OnBnClicked_Eh_SetFilter_1)
	ON_BN_CLICKED(IDC_EH_0116_EffectBaseView_SetFilter_2, &CPopup_EffectBase::OnBnClicked_Eh_SetFilter_2)
	ON_BN_CLICKED(IDC_EH_0116_EffectBaseView_SetFilter_3, &CPopup_EffectBase::OnBnClicked_Eh_SetFilter_3)
	ON_BN_CLICKED(IDC_EH_0116_EffectBaseView_SetEMesh, &CPopup_EffectBase::OnBnClicked_Eh_SetEMesh)
END_MESSAGE_MAP()


// ���� �̹����� ���ҽ��� �ҷ���
void CPopup_EffectBase::OnBnClicked_Eh_Load_Resource()
{	
	Load_ResourceData(m_pathBaseTex, &m_listBox_EffectBase);
}

// ���� �̹����� ����Ʈ�ڽ��� �����ϸ� - �̹��� ǥ�� + �̹��� ��� ����
void CPopup_EffectBase::OnLbnSelchange_Eh_Listbox_Effectbase()
{
	m_bPlaySprite = FALSE;

	int iListCursor = m_listBox_EffectBase.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	CString wstrFileName;
	m_listBox_EffectBase.GetText(iListCursor, wstrFileName);

	if (0 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Tex_Single/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Tex_Single/" + wstrFileName + L".png";
	}
	else if (1 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Tex_Sprite/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Tex_Sprite/" + wstrFileName + L".png";
	}
	else if (2 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Filter/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Filter/" + wstrFileName + L".png";
	}
	else if (3 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Filter_Fade/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Filter_Fade/" + wstrFileName + L".png";
	}
	else if (4 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Filter_Ramp/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Filter_Ramp/" + wstrFileName + L".png";
	}
	else if (5 == m_iTextureFolder)
	{
		m_textBaseTexturePath = L"Tex_Etc/" + wstrFileName + L".png";
		wstrFileName = L"../../Reference/Resource/Effect/Tex_Etc/" + wstrFileName + L".png";
	}

	UpdateData(FALSE);

	m_tEffectTexture.textPath = wstrFileName.operator LPCWSTR();

	Change_Picture_EffectBase(&wstrFileName, &m_pictureEffectBase);
	
	//////////////////////////////////////////////////////////
	if (!m_bCheckBox_UVTex)
	{
		CString pTexPath = m_tEffectTexture.textPath.c_str();
		m_pathDefualtTex = pTexPath;

		CString pMeshPath = m_tEffectTexture.meshPath.c_str();
		if (0 != pMeshPath.CompareNoCase(L"None"))
		{
			Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath);
		}
	}
	//////////////////////////////////////////////////////////
	if (m_tEffectTexture.bIsSprite)
	{
		Change_Picture_EffectBase(&wstrFileName, &m_pictureSpriteView, nullptr, m_tEffectTexture.bIsSprite);
	}
	else
	{
		Change_Picture_EffectBase(nullptr, &m_pictureSpriteView);
	}
}

// EMesh �𵨸� ���ҽ����� �ҷ���
void CPopup_EffectBase::OnBnClicked_Eh_Load_EMesh()
{
	// EMesh �ε� ��ư		
	m_fMeshAngle = 0.f;

	Load_ResourceData(L"../../Reference/Resource/Effect/Mesh/*.X", &m_listBox_EMesh);
	
	UpdateData(FALSE);
}

// EMesh�� EMesh����Ʈ �ڽ����� ���������� - �޽� �𵨸� �̹��� ǥ�� + �޽� ���� ���� + (�޽� ����Ʈ ���δ� ������� ����!!!)
void CPopup_EffectBase::OnLbnSelchange_Eh_Listbox_Emesh()
{
	m_fMeshAngle = 0.f;
	
	// �޽� ����Ʈ�ڽ� ����
	int iListCursor = m_listBox_EMesh.GetCurSel();

	if (0 > iListCursor)
	{
		AfxMessageBox(L"����Ʈ �ڽ��� ����Ʈ�� Ŭ�����ּ���.");
		return;
	}

	//////////////////////////////////////////////////////////

	CString wstrFileName;
	m_listBox_EMesh.GetText(iListCursor, wstrFileName);
	wstrFileName = L"../../Reference/Resource/Effect/Mesh/" + wstrFileName + L".X";

	m_tEffectTexture.meshPath = wstrFileName.operator LPCWSTR();

	Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &wstrFileName, m_tEffectTexture.bIsSprite);
}

// �޽� �𵨸� �̹����� ȸ�� +40
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_1()
{
	m_fMeshAngle += 40.f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// �޽� �𵨸� �̹����� ȸ�� -40
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_2()
{
	m_fMeshAngle -= 40.f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}


// �޽� �𵨸� �̹����� ȸ������ ������ �ǵ���
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Rotation_Reset()
{
	m_fMeshAngle = 0.f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None")) 
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// �޽� �𵨸��� �̹����� ǥ�õǴ� ũ�� x0.8
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_1()
{
	m_fMeshScale *= 0.8f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// �޽� �𵨸��� �̹����� ǥ�õǴ� ũ�⸦ ������ ��
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_Reset()
{
	m_fMeshScale = 1.f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// �޽� �𵨸��� �̹����� ǥ�õǴ� ũ�� x1.2
void CPopup_EffectBase::OnBnClicked_Eh_EMesh_Scale_2()
{
	m_fMeshScale *= 1.2f;

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// ����Ʈ �޽��� �𵨸� �̹������� �ؽ��ĸ� ����ƮUV�̹����� �������, ���õ� ����Ʈ �̹����� ������� ����
void CPopup_EffectBase::OnBnClicked_Eh_CheckBox_UVTex()
{
	if (m_bCheckBox_UVTex)
	{
		// �̹����� ������ ����Ʈ �ؽ�ó�� ���� ����.
		CString pTexPath = m_tEffectTexture.textPath.c_str();
		if (0 == pTexPath.CompareNoCase(L"None"))
		{
			AfxMessageBox(L"�̹����� ������ ���� �����ϴ�.");
			return;
		}
		m_bCheckBox_UVTex = FALSE;
		m_pathDefualtTex = pTexPath;
	}
	else
	{
		// ����Ʈ �ؽ��� �ѱ�
		m_pathDefualtTex = L"../../Reference/Resource/Effect/BackGrid.png";
		m_bCheckBox_UVTex = TRUE;
	}

	UpdateData(FALSE);

	CString pMeshPath = m_tEffectTexture.meshPath.c_str();
	if (0 != pMeshPath.CompareNoCase(L"None"))
	{
		Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
	}
}

// ����Ʈ �̹����� Ÿ���� ������ (��������Ʈ <-> �̱�)
void CPopup_EffectBase::OnCbnSelchange_Eh_TextureFoler()
{

	_int iIndex = m_comboBox_TextureFolder.GetCurSel();	
	if (0 == iIndex)
	{	
		m_pathBaseTex = L"../../Reference/Resource/Effect/Tex_Single/*.png";	
	}
	else if (1 == iIndex)
	{	
		m_pathBaseTex = L"../../Reference/Resource/Effect/Tex_Sprite/*.png";	
	}
	else if (2 == iIndex)
	{
		m_pathBaseTex = L"../../Reference/Resource/Effect/Filter/*.png";
	}
	else if (3 == iIndex)
	{
		m_pathBaseTex = L"../../Reference/Resource/Effect/Filter_Fade/*.png";
	}
	else if (4 == iIndex)
	{
		m_pathBaseTex = L"../../Reference/Resource/Effect/Filter_Ramp/*.png";
	}
	else if (5 == iIndex)
	{
		m_pathBaseTex = L"../../Reference/Resource/Effect/Tex_Etc/*.png";
	}
	else
	{
		AfxMessageBox(L"��ȿ���� ���� �ε���");
	}
	
	m_iTextureFolder = iIndex;

	Load_ResourceData(m_pathBaseTex, &m_listBox_EffectBase);
	Change_Picture_EffectBase(nullptr, &m_pictureEffectBase);

	m_textBaseTexturePath = L"";

	UpdateData(FALSE);
}

// ��������Ʈ �̹����� ��� ����
void CPopup_EffectBase::OnBnClicked_Eh_Play()
{
	//��������Ʈ ���
	m_bPlaySprite = !m_bPlaySprite;
}

// ��������Ʈ ����Ʈ�� �ִϸ��̼� �����
void CPopup_EffectBase::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	if (TIMERID == nIDEvent) 
	{
		if (m_tEffectTexture.bIsSprite && m_bPlaySprite)
		{
			CString pTexPath = m_tEffectTexture.textPath.c_str();
			if (0 == pTexPath.CompareNoCase(L"None")) 
			{
				return;
			}

			// �ִϸ��̼� ������̸鼭(m_bPlaySprite) , �ؽ��İ� ������
			m_fSpriteTimeAcc += 1.f / 60.f;

			UpdateData(TRUE);

			if (m_fSpriteTime <= m_fSpriteTimeAcc)
			{
				m_fSpriteTimeAcc -= m_fSpriteTime;

				m_iSpriteIndex += 1;

				if (m_iSpriteIndex >= m_iSprite_Row * m_iSprite_Column)
				{
					m_iSpriteIndex = 0;
				}

				UpdateData(FALSE);

				Change_Picture_EffectBase(&pTexPath, &m_pictureSpriteView, nullptr, m_tEffectTexture.bIsSprite);
				
				CString pMeshPath = m_tEffectTexture.meshPath.c_str();
				if (0 != pMeshPath.CompareNoCase(L"None"))
				{
					Change_Picture_EffectBase(&m_pathDefualtTex, &m_pictureEMeshView, &pMeshPath, m_tEffectTexture.bIsSprite);
				}
			}
		}
	}
}

// ȭ���� ������ ��Ÿ�̸� ������Ʈ�� ���ư�
void CPopup_EffectBase::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		CString pTexPath = m_tEffectTexture.textPath.c_str();
		if (0 == pTexPath.CompareNoCase(L"None")) 
		{
			Load_ResourceData(m_pathBaseTex, &m_listBox_EffectBase);
		}
		Change_Picture();

		SetTimer(TIMERID, (_uint)(1.f / 60.f), NULL);
	}
	else
	{
		KillTimer(TIMERID);
	}
}

// ��������Ʈ ����Ʈ�� �ɼǰ� ���� ��ư
void CPopup_EffectBase::OnBnClicked_Eh_Update()
{	
	UpdateData(TRUE);
		
	if (m_iSpriteIndex >= m_iSprite_Row * m_iSprite_Column)
	{
		m_iSpriteIndex = 0;
	}
	
	CString pTexPath = m_tEffectTexture.textPath.c_str();
	if (0 != pTexPath.CompareNoCase(L"None") && m_bIsSprite)
	{
		Change_Picture_EffectBase(&pTexPath, &m_pictureSpriteView, nullptr, m_tEffectTexture.bIsSprite);
	}
	else
	{
		Change_Picture_EffectBase(nullptr, &m_pictureSpriteView);
	}

	UpdateData(FALSE);
}


// ����̹��� ���
void CPopup_EffectBase::OnBnClicked_Eh_BlackWhite()
{
	UpdateData(TRUE);

	Change_Picture();
}


// ����̹��� ���� ���
void CPopup_EffectBase::OnBnClicked_Eh_BlackAlpha()
{
	UpdateData(TRUE);

	Change_Picture();
}


// �̹����� ��������Ʈ ���� üũ
void CPopup_EffectBase::OnBnClicked_EH_Set_CheckBox_IsSprite()
{
	// �ؽ�ó��� ����(��������Ʈ <-> �̱�)
	UpdateData(TRUE);

	m_tEffectTexture.bIsSprite = BOOL2bool(m_bIsSprite);

	// ��������Ʈ ���� ����
	m_bPlaySprite = FALSE;
	
	CString pTexPath = m_tEffectTexture.textPath.c_str();
	if (0 != pTexPath.CompareNoCase(L"None") && m_bIsSprite)
	{
		Change_Picture_EffectBase(&pTexPath, &m_pictureSpriteView, nullptr, m_tEffectTexture.bIsSprite);
	}
	else
	{
		Change_Picture_EffectBase(nullptr, &m_pictureSpriteView);
	}

	UpdateData(FALSE);
}

void CPopup_EffectBase::OnBnClicked_Eh_SetBase()
{
	// �ؽ�ó ��ȿ�˻�
	CString pPath = m_tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� �����ϰ� �����ּ���.");

		return;
	}

	T_EffectTexture tEffectTexture = m_tEffectTexture;

	m_tEffectTexture.bIsMesh = FALSE;
	m_tEffectTexture.meshPath = L"None";

	tEffectTexture.iRow = m_iSprite_Row;
	tEffectTexture.iColumn = m_iSprite_Column;
	tEffectTexture.iTextureType = 0;

	Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Tex_EffectBase(tEffectTexture);

	CDialogEx::OnOK();
}


void CPopup_EffectBase::OnBnClicked_Eh_SetFilter_1()
{
	// �ؽ�ó ��ȿ�˻�
	CString pPath = m_tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� �����ϰ� �����ּ���.");

		return;
	}

	T_EffectTexture tEffectTexture = m_tEffectTexture;

	m_tEffectTexture.bIsMesh = FALSE;
	m_tEffectTexture.meshPath = L"None";

	tEffectTexture.iRow = m_iSprite_Row;
	tEffectTexture.iColumn = m_iSprite_Column;
	tEffectTexture.iTextureType = 1;

	Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Tex_EffectBase(tEffectTexture);

	CDialogEx::OnOK();
}


void CPopup_EffectBase::OnBnClicked_Eh_SetFilter_2()
{
	// �ؽ�ó ��ȿ�˻�
	CString pPath = m_tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� �����ϰ� �����ּ���.");

		return;
	}

	T_EffectTexture tEffectTexture = m_tEffectTexture;

	m_tEffectTexture.bIsMesh = FALSE;
	m_tEffectTexture.meshPath = L"None";

	tEffectTexture.iRow = m_iSprite_Row;
	tEffectTexture.iColumn = m_iSprite_Column;
	tEffectTexture.iTextureType = 2;

	Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Tex_EffectBase(tEffectTexture);

	CDialogEx::OnOK();
}


void CPopup_EffectBase::OnBnClicked_Eh_SetFilter_3()
{
	// �ؽ�ó ��ȿ�˻�
	CString pPath = m_tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� �����ϰ� �����ּ���.");

		return;
	}

	T_EffectTexture tEffectTexture = m_tEffectTexture;

	m_tEffectTexture.bIsMesh = FALSE;
	m_tEffectTexture.meshPath = L"None";

	tEffectTexture.iRow = m_iSprite_Row;
	tEffectTexture.iColumn = m_iSprite_Column;
	tEffectTexture.iTextureType = 3;

	Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Tex_EffectBase(tEffectTexture);

	CDialogEx::OnOK();
}


void CPopup_EffectBase::OnBnClicked_Eh_SetEMesh()
{
	// �ؽ�ó ��ȿ�˻�
	CString pPath = m_tEffectTexture.textPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�̹����� �����ϰ� �����ּ���.");

		return;
	}
	
	T_EffectTexture tEffectTexture = m_tEffectTexture;
	
	pPath = tEffectTexture.meshPath.c_str();
	if (0 == pPath.CompareNoCase(L"None"))
	{
		AfxMessageBox(L"�޽��� �����ϰ� �����ּ���.");

		return;
	}

	tEffectTexture.bIsMesh = TRUE;
	
	tEffectTexture.iRow = m_iSprite_Row;
	tEffectTexture.iColumn = m_iSprite_Column;
	tEffectTexture.iTextureType = 0;
	
	Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Effect->Apply_Tex_EffectBase(tEffectTexture);

	CDialogEx::OnOK();
}