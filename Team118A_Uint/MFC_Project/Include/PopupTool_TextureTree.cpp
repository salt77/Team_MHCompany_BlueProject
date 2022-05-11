#include "stdafx.h"
#include "MFC_Project.h"
#include "PopupTool_TextureTree.h"
#include "afxdialogex.h"
#include "Line.h"
#include "MFC_Line.h"

// CPopupTool_TextureTree ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPopupTool_TextureTree, CDialogEx)

CPopupTool_TextureTree::CPopupTool_TextureTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_POPUPTOOL_TEXTURETREE, pParent)
{

}

CPopupTool_TextureTree::~CPopupTool_TextureTree()
{
	CD3DLine::DestroyInstance();
}

void CPopupTool_TextureTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MH_POPUPTOOL_TEXTURETREE, m_tTreeCtrl_Texture);
	DDX_Control(pDX, IDC_MH_TEXTURETREE_TOOL_PICTURE, m_Texture_PictureControl);
}


BEGIN_MESSAGE_MAP(CPopupTool_TextureTree, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MH_POPUPTOOL_TEXTURETREE, &CPopupTool_TextureTree::SelChange_TextureTree)
	ON_STN_CLICKED(IDC_MH_TEXTURETREE_TOOL_PICTURE, &CPopupTool_TextureTree::Button_Click_PictureControl)
	ON_BN_CLICKED(IDC_MH_1220_BUTTON_CREATE_NEW_UV, &CPopupTool_TextureTree::Button_Create_New_UV)
	ON_BN_CLICKED(IDC_MH_1220_BUTTON_SET_UV_MAX, &CPopupTool_TextureTree::Button_UV_Set_Max)
END_MESSAGE_MAP()


// CPopupTool_TextureTree �޽��� ó�����Դϴ�.


// MFC ���� �� �� FindData_ChoiceType�� ù ��° ���� ��ο� ����ִ� 
// ��� �̹����� Tree�� �ִ´�.
BOOL CPopupTool_TextureTree::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TI_Tex.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	_finddata_t findData;
	intptr_t lHandle = 0;

	// /*			Test ����							*/	
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/Test/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/Test/", lHandle, findData);
	// 
	/*			Button ����							*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Button/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Button/", lHandle, findData);
	 
	/*			BaseImage ����						*/
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/BaseImage/", "*.*", &findData);
	Load_Texture_Path(L"Texture/UI/BaseImage/", lHandle, findData);
/*
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/Logo/", "*.*", &findData);
	Load_Texture_Path(L"Texture/Logo/", lHandle, findData);*/
	// 
	// /*			Cafe ����							*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Cafe/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Cafe/", lHandle, findData);

	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Dialog_BackGround/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Dialog_BackGround/", lHandle, findData);

	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Tutorial/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Tutorial/", lHandle, findData);
	 
	// /*			Gift ����							*/
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Gift/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/Gift/", lHandle, findData);
	// 
	// /*			Character_Standing ����				*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Character_Standing/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Character_Standing/", lHandle, findData);
	//// 

	//// /*			Char_Background ����					*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Character_Collection/Char_Background/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Character_Collection/Char_Background/", lHandle, findData);
	// 
	//  /*			Item ����							*/
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Item/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/Item/", lHandle, findData);
	// 
	///*			Font ����							*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Font/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Font/", lHandle, findData);
	// 
	// /*			Combat ����							*/
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Combat/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/Combat/", lHandle, findData);
	// 
	// /*			SecretStone ����						*/
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/SecretStone/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/SecretStone/", lHandle, findData);
	// 
	// /*			Collection ����						*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Character_Collection/Character/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Character_Collection/Character/", lHandle, findData);
	// 
	// /*			Loading ����							*/
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Loading/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/Loading/", lHandle, findData);
	// 
	// /*			Event_BackGround ����				*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Event_BackGround/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Event_BackGround/", lHandle, findData);
	//
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Gacha/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Gacha/", lHandle, findData);
	//// /*			Boss_Portrait ����							*/
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Boss_Portrait/", "*.*", &findData);
	Load_Texture_Path(L"Texture/UI/Boss_Portrait/", lHandle, findData);

	// 
	// ��ȣ�� ��� ������ Effect�� ����
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Effect/Tex_Single/", "*.*", &findData);
	Load_Texture_Path(L"Effect/Tex_Single/", lHandle, findData);
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Effect/Tex_Sprite/", "*.*", &findData);
	//Load_Texture_Path(L"Effect/Tex_Sprite/", lHandle, findData);

	//��ȣ ����
	// lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Shop/", "*.*", &findData);
	// Load_Texture_Path(L"Texture/UI/Shop/", lHandle, findData);
	// /*			Stroy ����				*/
	//lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Texture/UI/Story/", "*.*", &findData);
	//Load_Texture_Path(L"Texture/UI/Story/", lHandle, findData);

	return TRUE; 
}

void CPopupTool_TextureTree::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}

// findData�� ����ִ� ��ε��� ��� Ready_Prototype���ش�.
void CPopupTool_TextureTree::Load_Texture_Path(_tchar* wPath, intptr_t lhandle, _finddata_t& findData)
{
	intptr_t lResult = 0;

	hTempItem = InsertTree(0, wPath, TI_Tex, m_tTreeCtrl_Texture);

	wstring wstrBasePath = L"../../Reference/Resource/";
	wstrBasePath.append(wPath);
	while (-1 != lResult)
	{
		wchar_t szFullPath[MAX_PATH] = L"";
		wchar_t szName[MAX_PATH] = L"";

		lstrcpy(szFullPath, wstrBasePath.c_str());

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);
		lstrcat(szFullPath, szName);

		if (strcmp(".", findData.name) && strcmp("..", findData.name) && strcmp("desktop.ini", findData.name))
		{
			InsertTree(hTempItem, szName, TI_Tex, m_tTreeCtrl_Texture);
			if (FAILED(Management Ready_Prototype(szName, CTexture::Create(Get_MFC_MainFrame()->m_pGraphicDev, szFullPath, E_TEXTURE_TYPE::TEX_NORMAL))))
			{
				assert(FALSE);
			}
		}
		lResult = _findnext(lhandle, &findData);
	}

	_findclose(lhandle);
}

// Ʈ���� ������ �ֱ�
HTREEITEM CPopupTool_TextureTree::InsertTree(HTREEITEM hParent, _tchar* szText, TV_INSERTSTRUCTW & ti, CTreeCtrl & treeCtrl)
{
	ti.item.pszText = szText;
	ti.hParent = hParent;
	return treeCtrl.InsertItem(&ti);
}

// Ʈ������ �ٸ� ���� Ŭ������ �� �θ� �ִ� Ʈ���������� ����������
// �����̸��� ������ ���̹Ƿ� m_wstrPickTexturePath�� �����Ѵ�.
void CPopupTool_TextureTree::SelChange_TextureTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	m_bCreateU = FALSE;
	m_bCreateV = FALSE;
	UINT nFlags = 0;
	POINT  pt = CUtility::Get_MousePoint(m_tTreeCtrl_Texture.m_hWnd);
	HTREEITEM hHitItem = m_tTreeCtrl_Texture.HitTest(pt, &nFlags);
	NULL_CHECK_RETURN(hHitItem, );

	HTREEITEM hParentItem = m_tTreeCtrl_Texture.GetParentItem(hHitItem);
	NULL_CHECK_RETURN(hParentItem, );

	wstring wstrText = m_tTreeCtrl_Texture.GetItemText(hHitItem);
	wstring wstrParentText = m_tTreeCtrl_Texture.GetItemText(hParentItem);
	wstring wstrBasePath = L"../../Reference/Resource/";
	wstrBasePath.append(wstrParentText);
	m_bClick = TRUE;
	// ��οϼ�
	m_wstrPickTexturePath = wstrBasePath;
	m_wstrPickTexturePath.append(wstrText);

	CUtility::Print_Wstring_DMode(L"���õ� Texture �̸� : " + wstrText);
	CUtility::Print_Wstring_DMode(L"���õ� Texture ��� : " + wstrBasePath);

	if ((eToolMenu)Get_MFC_MainFrame()->m_pMenuForm->m_tTabCtrl.GetCurSel() == eToolMenu::Particle)
	{
		Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Particle->m_wstrTexName = wstrText;
		Get_MFC_MainFrame()->m_pMenuForm->m_pTool_Particle->m_wstrTexPath = wstrBasePath;
	}
	m_vMinUV = _vec2(0.f, 0.f);
	m_vMaxUV = _vec2(1.f, 1.f);
	Render_PictureControl();
}

void CPopupTool_TextureTree::Render_Picture_Control(const CStatic & m_PictureControl, CTexture* pTex)
{

}

// ���� ��Ʈ���� ������ UV���� �ٽ� �����Ѵ�.
void CPopupTool_TextureTree::Button_Click_PictureControl()
{
	CStatic* pstatic = (CStatic *)GetDlgItem(IDC_MH_TEXTURETREE_TOOL_PICTURE);
	CRect rect;
	pstatic->GetClientRect(rect); // Static �� ũ�⸦ ����
	CUtility::Print_Wstring_DMode(to_wstring(rect.left) + L",   " + 
		to_wstring(rect.right) + L",   " + 
		to_wstring(rect.top) + L",   " + 
		to_wstring(rect.bottom));

	POINT ptPictureControlSize = { rect.right - rect.left, rect.bottom - rect.top };
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	GetDlgItem(IDC_MH_TEXTURETREE_TOOL_PICTURE)->ScreenToClient(&ptMouse);
	CUtility::Print_Wstring_DMode(L"ptMouse - " + to_wstring(ptMouse.x) + L",   " + to_wstring(ptMouse.y));

	_vec2 vRatio = _vec2((ptMouse.x / (_float)ptPictureControlSize.x), (ptMouse.y / (_float)ptPictureControlSize.y));
	CUtility::Print_Wstring_DMode(L"vRatio - " + to_wstring(vRatio.x) + L",   " + to_wstring(vRatio.y));
	
	static _vec2 vMinUVTemp = VEC_ZERO;
	if (m_bCreateU)
	{
		m_bCreateU = FALSE;
		m_bCreateV = TRUE;
		vMinUVTemp = vRatio;
	}
	else if (m_bCreateV)
	{
		m_bCreateV = FALSE;
		m_vMaxUV = vRatio;
		m_vMinUV = vMinUVTemp;
	}
	Render_PictureControl();
}

// ���ο� UV���� ������ ��ư�� Ŭ���Ѵ�.
void CPopupTool_TextureTree::Button_Create_New_UV()
{
	m_bCreateU = TRUE;
	Render_PictureControl();
}
 
// ������Ʈ���� ��ġ�� �������Ѵ�.
void CPopupTool_TextureTree::Render_PictureControl()
{
 	if (m_wstrPickTexturePath == L"")
		return;

	_matrix		matWorld, matView, matProj;
	_matrix		matOldView, matOldProj;

	Get_MFC_MainFrame()->m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	Get_MFC_MainFrame()->m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj); 
	Get_MFC_MainFrame()->m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Get_MFC_MainFrame()->m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); 
	Get_MFC_MainFrame()->m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);
	D3DXMatrixScaling(&matWorld, 2.f, 2.f, 1.f);

	CGraphicDev::GetInstance()->Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.f));
	CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &matView);
	CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);

	CTexture* pTex = static_cast<CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(m_wstrPickTexturePath));
	if (nullptr == pTex)
	{
		CProtoMgr::GetInstance()->Ready_Prototype(m_wstrPickTexturePath,
			CTexture::Create(Get_MFC_MainFrame()->m_pGraphicDev, m_wstrPickTexturePath, E_TEXTURE_TYPE::TEX_NORMAL));
		pTex = static_cast<CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(m_wstrPickTexturePath));
	}

	pTex->Set_Texture_Device();
	CVIBuffer* pVIBuffer = static_cast<CVIBuffer*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex"));

	pVIBuffer->Render_Buffer();

	_vec2 vVec[5];
	vVec[0] = _vec2(m_vMinUV.x * WINCX, m_vMinUV.y * WINCY);
	vVec[1] = _vec2(m_vMaxUV.x * WINCX, m_vMinUV.y * WINCY);
	vVec[2] = _vec2(m_vMaxUV.x * WINCX, m_vMaxUV.y * WINCY);
	vVec[3] = _vec2(m_vMinUV.x * WINCX, m_vMaxUV.y * WINCY);
	vVec[4] = _vec2(m_vMinUV.x * WINCX, m_vMinUV.y * WINCY);
	CD3DLine::GetInstance()->Render_Line2D(vVec, 5, D3DXCOLOR(0.f, 1.f, 0.f, 1.f), matWorld);
	CGraphicDev::GetInstance()->Render_End(m_Texture_PictureControl.GetSafeHwnd());

	Safe_Release(pVIBuffer);
	Safe_Release(pTex);

	CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &matOldView);
	CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_PROJECTION, &matOldProj);
	Get_MFC_MainFrame()->m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

// ������Ʈ���� UV���� �ʱ���·� �ǵ�����.
void CPopupTool_TextureTree::Button_UV_Set_Max()
{
	m_vMinUV = _vec2(0.f, 0.f);
	m_vMaxUV = _vec2(1.f, 1.f);
	Render_PictureControl();
}
