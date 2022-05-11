#include "stdafx.h"
#include "MFC_Project.h"
#include "MenuForm.h"

IMPLEMENT_DYNCREATE(CMenuForm, CFormView)


CMenuForm::CMenuForm() : CFormView(IDD_MH_MENUFORM)
{
}

CMenuForm::~CMenuForm()
{
	Tool_Release();
}

void CMenuForm::DoDataExchange(CDataExchange * pDX)
{
	DDX_Control(pDX, IDC_MH_MenuTab, m_tTabCtrl);
}

void CMenuForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	UpdateData(TRUE);

	CRect rect;
	m_tTabCtrl.GetWindowRect(rect);

	m_pPopupTool_ObjectTree = new CPopupTool_ObjectTree;
	m_pPopupTool_ObjectTree->Create(IDD_MH_POPUPTOOL_OBJECTTREE);
	m_pPopupTool_ObjectTree->ShowWindow(SW_SHOW);

	m_pPopupTool_TextureTree = new CPopupTool_TextureTree;
	m_pPopupTool_TextureTree->Create(IDD_MH_POPUPTOOL_TEXTURETREE);
	m_pPopupTool_TextureTree->ShowWindow(SW_HIDE);

	m_pPopup_LightOption = new CPopup_LightOption;
	m_pPopup_LightOption->Create(IDD_EH_POPUP_LIGHTOPTION);
	m_pPopup_LightOption->ShowWindow(SW_HIDE);

	m_pPopup_EffectBase = new CPopup_EffectBase;
	m_pPopup_EffectBase->Create(IDD_EH_POPUP_EFFECTBASE);
	m_pPopup_EffectBase->ShowWindow(SW_HIDE);

	m_pPopup_EffectFilter = new CPopup_EffectFilter;
	m_pPopup_EffectFilter->Create(IDD_EH_1226_POPUP_EFFECTFILTER);
	m_pPopup_EffectFilter->ShowWindow(SW_HIDE);

	_int iStack = 0;
	m_tTabCtrl.InsertItem(iStack++, _T(" World "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Camera "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Ui "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Map "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Particle "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Effect "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Animation "));
	m_tTabCtrl.InsertItem(iStack++, _T(" Tile "));
	m_tTabCtrl.SetCurSel(0);

	m_pTool_World = new CTool_World(m_pPopupTool_ObjectTree, 0);
	m_pTool_World->Create(IDD_MH_TOOL_WORLD, &m_tTabCtrl);
	m_pTool_World->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_World->ShowWindow(SW_SHOW);

	m_pTool_Tile = new CTool_Tile(m_pPopupTool_ObjectTree, 0);
	m_pTool_Tile->Create(IDD_MW_TOOL_TILE, &m_tTabCtrl);
	m_pTool_Tile->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Tile->ShowWindow(SW_HIDE);

	m_pTool_Camera = new CTool_Camera(m_pPopupTool_ObjectTree, 0);
	m_pTool_Camera->Create(IDD_MH_TOOL_CAMERA, &m_tTabCtrl);
	m_pTool_Camera->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Camera->ShowWindow(SW_HIDE);

	m_pTool_Ui = new CTool_Ui(m_pPopupTool_ObjectTree, 0);
	m_pTool_Ui->Create(IDD_MH_TOOL_UI, &m_tTabCtrl);
	m_pTool_Ui->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Ui->ShowWindow(SW_HIDE);

	m_pTool_Map = new CTool_Map(m_pPopupTool_ObjectTree, 0);
	m_pTool_Map->Create(IDD_MH_TOOL_MAP, &m_tTabCtrl);
	m_pTool_Map->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Map->ShowWindow(SW_HIDE);

	m_pTool_Particle = new CTool_Particle(m_pPopupTool_ObjectTree, 0);
	m_pTool_Particle->Create(IDD_MH_TOOL_PARTICLE, &m_tTabCtrl);
	m_pTool_Particle->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Particle->ShowWindow(SW_HIDE);

	m_pTool_Effect = new CTool_Effect(m_pPopupTool_ObjectTree, 0);
	m_pTool_Effect->Create(IDD_MH_TOOL_EFFECT, &m_tTabCtrl);
	m_pTool_Effect->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Effect->ShowWindow(SW_HIDE);

	m_pTool_Animation = new CTool_Animation(m_pPopupTool_ObjectTree, 0);
	m_pTool_Animation->Create(IDD_MH_TOOL_ANIMATION, &m_tTabCtrl);
	m_pTool_Animation->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);
	m_pTool_Animation->ShowWindow(SW_HIDE);


	ToolMenuState = eToolMenu::World;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.


	UpdateData(FALSE);
}
BEGIN_MESSAGE_MAP(CMenuForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MH_MenuTab, &CMenuForm::MH_On_Selchange_Menutab)
END_MESSAGE_MAP()


void CMenuForm::Tool_Update(NMHDR *pNMHDR)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IDC_MH_MenuTab == pNMHDR->idFrom)
	{
		int iSelect = m_tTabCtrl.GetCurSel();

		m_pTool_World->ShowWindow(SW_HIDE);
		m_pTool_Tile->ShowWindow(SW_HIDE);
		m_pTool_Camera->ShowWindow(SW_HIDE);
		m_pTool_Ui->ShowWindow(SW_HIDE);
		m_pTool_Map->ShowWindow(SW_HIDE);
		m_pTool_Particle->ShowWindow(SW_HIDE);
		m_pTool_Effect->ShowWindow(SW_HIDE);
		m_pTool_Animation->ShowWindow(SW_HIDE);

		switch (iSelect)
		{
		case (int)eToolMenu::World:		{	m_pTool_World->ShowWindow(SW_SHOW);			break;	}
		case (int)eToolMenu::Camera:	{	m_pTool_Camera->ShowWindow(SW_SHOW);		break;	}
		case (int)eToolMenu::UI:		{	m_pTool_Ui->ShowWindow(SW_SHOW);			break;	}
		case (int)eToolMenu::Map:		{	m_pTool_Map->ShowWindow(SW_SHOW);			break;	}
		case (int)eToolMenu::Particle:	{	m_pTool_Particle->ShowWindow(SW_SHOW);		break;	}
		case (int)eToolMenu::Effect:	{	m_pTool_Effect->ShowWindow(SW_SHOW);		break;	}
		case (int)eToolMenu::Animation: {	m_pTool_Animation->ShowWindow(SW_SHOW);		break;	}
		case (int)eToolMenu::Tile:		{   m_pTool_Tile->ShowWindow(SW_SHOW);			break;  }
		default:						{	assert(FALSE);								break;	}
		}
	}
}

void CMenuForm::Tool_Release()
{
	if (m_pPopupTool_ObjectTree)
	{
		Safe_Delete(m_pPopupTool_ObjectTree);
	}
	if (m_pPopupTool_TextureTree)
	{
		Safe_Delete(m_pPopupTool_TextureTree);
	}
	if (m_pPopup_LightOption)
	{
		Safe_Delete(m_pPopup_LightOption);
	}
	if (m_pPopup_EffectBase)
	{
		Safe_Delete(m_pPopup_EffectBase);
	}
	if (m_pPopup_EffectFilter)
	{
		Safe_Delete(m_pPopup_EffectFilter);
	}
	if (m_pTool_World)
	{
		Safe_Delete(m_pTool_World);
	}
	if (m_pTool_Tile)
	{
		Safe_Delete(m_pTool_Tile);
	}
	if (m_pTool_Camera)
	{
		Safe_Delete(m_pTool_Camera);
	}
	if (m_pTool_Ui)
	{
		Safe_Delete(m_pTool_Ui);
	}
	if (m_pTool_Map)
	{
		Safe_Delete(m_pTool_Map);
	}
	if (m_pTool_Particle)
	{
		Safe_Delete(m_pTool_Particle);
	}
	if (m_pTool_Effect)
	{
		Safe_Delete(m_pTool_Effect);
	}
	if (m_pTool_Animation)
	{
		if (m_pTool_Animation->m_pDMesh)
		{
			Safe_Release(m_pTool_Animation->m_pDMesh);
		}
		Safe_Delete(m_pTool_Animation);
	}
}

_bool CMenuForm::Get_CurTab(eToolMenu eTab)
{
	int iSelect = m_tTabCtrl.GetCurSel();
	if (iSelect == (_int)eTab)
		return TRUE;

	return FALSE;
}

void CMenuForm::Popup_TextureTool()
{
	m_pPopupTool_TextureTree->ShowWindow(SW_SHOW);
}

void CMenuForm::MH_On_Selchange_Menutab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	Tool_Update(pNMHDR);
}
