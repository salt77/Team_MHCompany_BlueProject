#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Tile.h"
#include "afxdialogex.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "SMesh_Test.h"
#include "CombatTile.h"

// CTool_Tile 대화 상자입니다.

USING(Engine)

IMPLEMENT_DYNAMIC(CTool_Tile, CDialogEx)

CTool_Tile::CTool_Tile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MW_TOOL_TILE, pParent)
	, m_strWorldCreaterPath(_T(""))
	, m_strLinkTileIndex(_T(""))
{
	ZeroMemory(m_arrayCombatTile, sizeof(m_arrayCombatTile));
}

CTool_Tile::CTool_Tile(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Tile::~CTool_Tile()
{
}

void CTool_Tile::ToolTileUpdate(_float fDeltaTime)
{
	if (!m_bShow)
	{
		return;
	}

	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		m_arrayCombatTile[i]->Update_Object(fDeltaTime);
	}
	for (auto& pMascot : m_vecMascot)
	{
		pMascot->Update_Object(fDeltaTime);
		pMascot->LateUpdate_Object(fDeltaTime);
	}

	for (auto& tPair : m_mapStartWorldUI)
	{
		tPair.second->Update_Object(fDeltaTime);
		tPair.second->LateUpdate_Object(fDeltaTime);
	}
	if (Management Key_Down(KEY_LBUTTON))
	{
		Raycast_Mascot();	
	}
	if (Management Key_Pressing(KEY_LBUTTON))
	{
		Raycast_TileSelect();
	}
	if (Management Key_Down(KEY_LEFT))
	{
		if (!m_pPickMascot)
		{
			return;
		}
		m_pPickMascot->Add_Rotation(_vec3(0.f, 60.f, 0.f));
	}
	if (Management Key_Down(KEY_RIGHT))
	{
		if (!m_pPickMascot)
		{
			return;
		}
		m_pPickMascot->Add_Rotation(_vec3(0.f, -60.f, 0.f));
	}
}

//콤보박스 설정
BOOL CTool_Tile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	Set_Tile();

	m_cComboTileOption.AddString(L"TILE_EMPTY");
	m_cComboTileOption.AddString(L"TILE_NORMAL");
	m_cComboTileOption.AddString(L"TILE_WARP_ONE_WAY_START");
	m_cComboTileOption.AddString(L"TILE_WARP_ONE_WAY_END");
	m_cComboTileOption.AddString(L"TILE_WARP_TWO_WAY");
	m_cComboTileOption.AddString(L"TILE_SPAWN");
	m_cComboTileOption.AddString(L"TILE_VARIABLE_OFF");
	m_cComboTileOption.AddString(L"TILE_REMOVE");
	m_cComboTileOption.AddString(L"TILE_VARIABLE_ON");
	m_cComboTileOption.AddString(L"TILE_WEAK");
	m_cComboTileOption.AddString(L"TILE_START");

	m_cComboPlayType.AddString(L"Defensive");
	m_cComboPlayType.AddString(L"Aggresive");
	m_cComboPlayType.AddString(L"Tracking");
	Load_World();
	return 0;
}

void CTool_Tile::Set_Tile()
{
	

	WCHAR wszListName[32];

	for (_int i = 0; i < m_iMaxZ; ++i)
	{
		for (_int j = 0; j < m_iMaxX; ++j)
		{
			_float fX = j * m_fIntervalX - m_fIntervalX*(m_iMaxX-1)*0.5f;
			_float fZ = i * m_fIntervalZ - m_fIntervalZ*(m_iMaxZ-1)*0.5f;

			if (i % 2 == 1)
			{
				fX += m_fIntervalX * 0.5f;
			}

			CCombatTile* pCombatTile = CCombatTile::Create(Management GetDevice(), E_OBJTAG::OBJ_Test,E_TILE_OPTION::TILE_EMPTY);
			_int iIndex = i*m_iMaxX + j;
			m_arrayCombatTile[iIndex] = pCombatTile;
			pCombatTile->Set_Pos(_vec3(fX, -0.3f, fZ));
			
			swprintf_s(wszListName, L"Combat Tile %d", iIndex);
			m_cListTile.AddString(wszListName);
		}
	}
}

void CTool_Tile::Load_World()
{
	m_cListWorld.ResetContent();
	
	string searching = "../../Reference/Data/Data_World/";
	searching += "*.*";
	_finddata_t findData;
	intptr_t handle = _findfirst(searching.c_str(), &findData);  //현재 폴더 내 모든 파일을 찾는다.


	if (handle == -1)
	{
		return;
	}

	int result = 0;

	do
	{
		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
			wstring wstr = CUtility::CHAR_TO_WSTRING(findData.name);
			m_cListWorld.AddString(wstr.c_str());
		}
		result = _findnext(handle, &findData);

	} while (result != -1);

	_findclose(handle);
}

void CTool_Tile::Raycast_TileSelect()
{
	_int iSelect = -1;
	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		CCombatTile* pCombatTile = m_arrayCombatTile[i];
		if (!pCombatTile)
		{
			continue;
		}

		CSMesh* pSMesh = static_cast<CSMesh*>(pCombatTile->Get_Component(ComTag_SMesh));
		if (!pSMesh)
		{
			continue;
		}

		_float fDis;
		_vec3 vPos;
		_matrix matWorld = static_cast<CTransform*>(pCombatTile->Get_Component(ComTag_Transform))->Get_WorldMatrix();
		if (pSMesh->PickingMesh(g_hWnd, &vPos, &fDis, matWorld))
		{
			iSelect = i;
			break;
		}

	}

	if (iSelect < 0)
	{
		return;
	}
	if (!m_pPickMascot)
	{
		m_cListTile.SetCurSel(iSelect);
		Reload_TileInfo();
	}
	else
	{
		for (_int i = 0; i<(_int)m_vecMascot.size(); ++i)
		{
			if (iSelect == m_vecMascot[i]->Get_CurrentTileIndex())
			{
				return;
			}
		}

		m_pPickMascot->Set_Pos(m_arrayCombatTile[iSelect]->Get_Position());
	
		m_pPickMascot->Set_CurrentTileIndex(iSelect);
	}
}

void CTool_Tile::Raycast_Mascot()
{
	_int iIndex = 0;
	for (auto& pMascot : m_vecMascot)
	{
		CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pMascot->Get_Component(ComTag_Collider));
		if (!pSphereCol)
		{
			continue;
		}

		CGameObject* pPickObj = nullptr;
		if (pSphereCol->PickingCollider(g_hWnd, &pPickObj))
		{
			m_pPickMascot = static_cast<CMascot*>(pPickObj);
			m_cListMascot.SetCurSel(iIndex);
			OnLbnSelchangeMw0109ListMascot();
			return;
		}
		++iIndex;
	}

	m_pPickMascot = nullptr;
}

void CTool_Tile::Reload_TileInfo()
{
	_int iSelect = m_cListTile.GetCurSel();
	if (iSelect < 0 || iSelect >= m_iMaxArray)
	{
		return;
	}

	CCombatTile* pCombatTile = m_arrayCombatTile[iSelect];

	UpdateData(TRUE);

	m_cComboTileOption.SetCurSel((_int)pCombatTile->Get_TileOption());


	m_strLinkTileIndex = to_wstring(pCombatTile->Get_LinkTileIndex()).c_str();
	UpdateData(FALSE);
}

_vec3 CTool_Tile::Get_IndexTilePos(_int iTileIndex)
{
	
	if (iTileIndex < 0 || iTileIndex >= m_iMaxArray)
	{
		return _vec3(0.f, 0.f, 0.f);
	}
	_int iZ = iTileIndex / m_iMaxX;
	_int iX = iTileIndex % m_iMaxX;
	_float fX = _float(iX) * m_fIntervalX - m_fIntervalX*(m_iMaxX - 1)*0.5f;
	_float fZ = _float(iZ) * m_fIntervalZ - m_fIntervalZ*(m_iMaxZ - 1)*0.5f;

	return _vec3(fX, 0.f, fZ);
}

_bool CTool_Tile::Check_TileField()
{
	_int iStartTileCount = 0;

	vector<_int> vecRemove;
	vector<_int> vecVariableOn;

	vector<_int> vecSpawn;
	vector<_int> vecVariableOff;

	vector<_int> vecWarpOneWayStart;
	vector<_int> vecWarpOneWayEnd;

	vector<_int> vecWarpTwoWay;

	//타일 검사
	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		E_TILE_OPTION eTileOption = m_arrayCombatTile[i]->Get_TileOption();
		switch (eTileOption)
		{
		case Engine::E_TILE_OPTION::TILE_EMPTY:
			break;
		case Engine::E_TILE_OPTION::TILE_NORMAL:
			break;
		case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_START:
			vecWarpOneWayStart.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_END:
			vecWarpOneWayEnd.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_WARP_TWO_WAY:
			vecWarpTwoWay.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_SPAWN:
			vecSpawn.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_VARIABLE_OFF:
			vecVariableOff.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_REMOVE:
			vecRemove.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_VARIABLE_ON:
			vecVariableOn.emplace_back(i);
			break;
		case Engine::E_TILE_OPTION::TILE_WEAK:
			break;
		case Engine::E_TILE_OPTION::TILE_START:
			++iStartTileCount;
			break;
		case Engine::E_TILE_OPTION::TILE_END:
			break;
		default:
			break;
		}
	}

	if (iStartTileCount < 1)
	{
		MessageBox(L"시작 타일이 없습니다.", L"실패", MB_OK);
		return FALSE;
	}

	if (m_vecMascot.empty())
	{
		MessageBox(L"몬스터가 없습니다.", L"실패", MB_OK);
		return FALSE;
	}

	_int iBossCount = 0;
	for (auto& pMascot : m_vecMascot)
	{
		_int iCurIndex = pMascot->Get_CurrentTileIndex();
		E_TILE_OPTION eTileOption = m_arrayCombatTile[iCurIndex]->Get_TileOption();
		if (pMascot->Get_Boss())
		{
			++iBossCount;
		}
		if (!((eTileOption == E_TILE_OPTION::TILE_NORMAL) || (eTileOption == E_TILE_OPTION::TILE_VARIABLE_ON)))
		{
			MessageBox(L"몬스터가 시작할 수 없는 타일에 있습니다.", L"실패", MB_OK);
			return FALSE;
		}
	}
	if (iBossCount < 1)
	{
		MessageBox(L"보스가 없습니다", L"실패",MB_OK);
		return FALSE;
	}

	for (auto& iIndex : vecSpawn)
	{
		_int iLinkIndex = m_arrayCombatTile[iIndex]->Get_LinkTileIndex();

		_bool bCheck = FALSE;

		auto iter = vecVariableOff.begin();
		while (iter!=vecVariableOff.end())
		{
			if ((*iter) == iLinkIndex)
			{
				bCheck = TRUE;
				vecVariableOff.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}
		
		if (!bCheck)
		{
			MessageBox(L"스폰타일과 VARIABLE_OFF 타일이 연결간의 일치하지 않습니다.", L"실패", MB_OK);
			return FALSE;
		}
	}
	if (!vecVariableOff.empty())
	{

		MessageBox(L"스폰타일과 VARIABLE_OFF 타일이 연결간의 일치하지 않습니다.", L"실패", MB_OK);
		return FALSE;
	}
	
	for (auto& iIndex : vecRemove)
	{
		_int iLinkIndex = m_arrayCombatTile[iIndex]->Get_LinkTileIndex();

		_bool bCheck = FALSE;

		auto iter = vecVariableOn.begin();
		while (iter!=vecVariableOn.end())
		{
			if ((*iter) == iLinkIndex)
			{
				bCheck = TRUE;
				vecVariableOn.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}

		if (!bCheck)
		{
			MessageBox(L"리무브타일과 VARIABLE_ON 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
			return FALSE;
		}
	}
	if (!vecVariableOn.empty())
	{

		MessageBox(L"리무브타일과 VARIABLE_ON 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
		return FALSE;
	}

	for (auto& iIndex : vecWarpOneWayStart)
	{
		_int iLinkIndex = m_arrayCombatTile[iIndex]->Get_LinkTileIndex();

		_bool bCheck = FALSE;

		auto iter = vecWarpOneWayEnd.begin();
		while (iter!=vecWarpOneWayEnd.end())
		{
			if ((*iter) == iLinkIndex)
			{
				bCheck = TRUE;
				vecWarpOneWayEnd.erase(iter);
				break;
			}
			else
			{
				++iter;
			}
		}

		if (!bCheck)
		{
			MessageBox(L"WARP ONE WAY START타일과 WARP ONE WAY END 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
			return FALSE;
		}

		
	}
	if (!vecWarpOneWayEnd.empty())
	{
		MessageBox(L"WARP ONE WAY START타일과 WARP ONE WAY END 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
		return FALSE;
	}

	while (!vecWarpTwoWay.empty())
	{
		_int iResultIndex1 = -1;
		_int iResultIndex2 = -1;
		for (auto& iIndex : vecWarpTwoWay)
		{
			_int iLinkIndex = m_arrayCombatTile[iIndex]->Get_LinkTileIndex();

			_bool bCheck = FALSE;
			for (auto& iCheckIndex : vecWarpTwoWay)
			{
				if (iIndex == iCheckIndex)
				{
					continue;
				}

				if (iLinkIndex == iCheckIndex)
				{
					_int iCheckLinkIndex = m_arrayCombatTile[iCheckIndex]->Get_LinkTileIndex();
					if (iCheckLinkIndex != iIndex)
					{
						MessageBox(L"WARP TWO WAY 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
						return FALSE;
					}

					iResultIndex1 = iIndex;
					iResultIndex2 = iCheckIndex;

					bCheck = TRUE;
					break;
				}
			}
			if (bCheck)
			{
				break;
			}
			else
			{
				MessageBox(L"WARP TWO WAY 타일간의 연결이 일치하지 않습니다.", L"실패", MB_OK);
				return FALSE;
			}
		}

		if (iResultIndex1 >= 0 && iResultIndex2 >= 0)
		{
			_int iCount = 0;
			auto iter = vecWarpTwoWay.begin();
			while (iter!=vecWarpTwoWay.end())
			{
				if (iCount > 1)
				{
					break;
				}

				if ((*iter) == iResultIndex1 || (*iter) == iResultIndex2)
				{
					++iCount;
					iter = vecWarpTwoWay.erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	} 

	return TRUE;
}


//불러오기 전 기존 데이터를 모두 지우는 함수

void CTool_Tile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MW_0108_LIST_TILE, m_cListTile);
	DDX_Control(pDX, IDC_MW_0108_LIST_WORLD, m_cListWorld);
	DDX_Control(pDX, IDC_MW_0109_COMBO_TILE_OPTION, m_cComboTileOption);
	DDX_Text(pDX, IDC_MW_0108_EDIT_WORLD_CREATER_PATH, m_strWorldCreaterPath);
	DDX_Control(pDX, IDC_MW_0109_LIST_MASCOT, m_cListMascot);
	DDX_Control(pDX, IDC_MW_0109_COMBO_PLAY_TYPE, m_cComboPlayType);
	DDX_Text(pDX, IDC_MW_0110_EDIT_LINK_TILE_INDEX, m_strLinkTileIndex);
}


BEGIN_MESSAGE_MAP(CTool_Tile, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_MW_0108_LIST_TILE, &CTool_Tile::OnLbnSelchangeMw0108ListTile)
	ON_LBN_SELCHANGE(IDC_MW_0108_LIST_WORLD, &CTool_Tile::OnLbnSelchangeMw0108ListWorld)
	ON_BN_CLICKED(IDC_MW_0108_BUTTON_BIND_WORLD, &CTool_Tile::OnBnClickedMw0108ButtonBindWorld)
	ON_BN_CLICKED(IDC_MW_0108_BUTTON_DELETE_WORLD, &CTool_Tile::OnBnClickedMw0108ButtonDeleteWorld)
	ON_CBN_SELCHANGE(IDC_MW_0109_COMBO_TILE_OPTION, &CTool_Tile::OnCbnSelchangeMw0109ComboTileOption)
	ON_LBN_SELCHANGE(IDC_MW_0109_LIST_MASCOT, &CTool_Tile::OnLbnSelchangeMw0109ListMascot)
	ON_CBN_SELCHANGE(IDC_MW_0109_COMBO_PLAY_TYPE, &CTool_Tile::OnCbnSelchangeMw0109ComboPlayType)
	ON_BN_CLICKED(IDC_MW_0109_BUTTON_SAVE, &CTool_Tile::OnBnClickedMw0109ButtonSave)
	ON_BN_CLICKED(IDC_MW_0109_BUTTON_LOAD, &CTool_Tile::OnBnClickedMw0109ButtonLoad)
	ON_EN_CHANGE(IDC_MW_0110_EDIT_LINK_TILE_INDEX, &CTool_Tile::OnEnChangeMw0110EditLinkTileIndex)
END_MESSAGE_MAP()



void CTool_Tile::OnDestroy()
{
	CDialogEx::OnDestroy();

	for (_int i = 0; i < m_iMaxArray; ++i)
	{
		Safe_Release(m_arrayCombatTile[i]);
	}
	for (auto& pMascot : m_vecMascot)
	{
		Safe_Release(pMascot);
	}

	for (auto& tPair : m_mapStartWorldUI)
	{
		Safe_Release(tPair.second);
	}
	m_mapStartWorldUI.clear();
}


void CTool_Tile::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_bShow = TRUE;
	}
	else
	{
		m_bShow = FALSE;
	}
}


void CTool_Tile::OnLbnSelchangeMw0108ListTile()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(FALSE);

	Reload_TileInfo();
}


void CTool_Tile::OnLbnSelchangeMw0108ListWorld()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CTool_Tile::OnBnClickedMw0108ButtonBindWorld()
{
	
	UpdateData(TRUE);
	UpdateData(FALSE);

	_int iSelectWorld = m_cListWorld.GetCurSel();
	if (iSelectWorld < 0)
	{
		return;
	}
	WCHAR wszWorldName[64];
	m_cListWorld.GetText(iSelectWorld, wszWorldName);

	CMascot* pMascot = CMascot::Create(Management GetDevice(), E_OBJTAG::OBJ_Monster, wszWorldName);
	if (!pMascot)
	{
		return;
	}
	if (pMascot->Get_Boss())
	{
		for (auto& pMonster : m_vecMascot)
		{
			if (pMonster->Get_Boss())
			{
				MessageBox(L"이미 보스가 존재합니다.", L"실패", MB_OK);
				Safe_Release(pMascot);
				return;
			}
		}
	}

	_int iIndex = 0;
	for (_int i=0; i<(_int)m_vecMascot.size(); ++i)
	{
		if (iIndex == m_vecMascot[i]->Get_CurrentTileIndex())
		{
			i = -1;
			++iIndex;
		}
	}
	
	pMascot->Set_CurrentTileIndex(iIndex);
	pMascot->Set_Pos(m_arrayCombatTile[iIndex]->Get_Position());
	pMascot->Set_Rotation(_vec3(0.f, 150.f, 0.f));

	m_vecMascot.emplace_back(pMascot);
	m_cListMascot.AddString(wszWorldName);


}


void CTool_Tile::OnBnClickedMw0108ButtonDeleteWorld()
{
	UpdateData(TRUE);
	UpdateData(FALSE);

	_int iSelectMascot = m_cListMascot.GetCurSel();

	if (iSelectMascot < 0 || iSelectMascot >= (_int)m_vecMascot.size())
	{
		return;
	}

	auto iter = m_vecMascot.begin();
	for (_int i = 0; i < iSelectMascot; ++i)
	{
		++iter;
	}

	m_cListMascot.DeleteString(iSelectMascot);
	Safe_Release(*iter);
	m_vecMascot.erase(iter);
}


void CTool_Tile::OnCbnSelchangeMw0109ComboTileOption()
{
	UpdateData(TRUE);
	UpdateData(FALSE);

	_int iSelect = m_cListTile.GetCurSel();
	if (iSelect < 0 || iSelect >= m_iMaxArray)
	{
		return;
	}
	E_TILE_OPTION ePrevTileOption = m_arrayCombatTile[iSelect]->Get_TileOption();
	_vec3 vPrevPos = m_arrayCombatTile[iSelect]->Get_Position();
	Safe_Release(m_arrayCombatTile[iSelect]);

	E_TILE_OPTION eSelectTileOption = E_TILE_OPTION(m_cComboTileOption.GetCurSel());
	

	m_arrayCombatTile[iSelect] = CCombatTile::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, eSelectTileOption);
	m_arrayCombatTile[iSelect]->Set_Pos(vPrevPos);
	Reload_TileInfo();


	if (eSelectTileOption == E_TILE_OPTION::TILE_START && ePrevTileOption != E_TILE_OPTION::TILE_START)
	{
		CWorldUI* pWorldUI = CWorldUI::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, L"StartUI.uidat");
		assert(pWorldUI);

		pWorldUI->Set_Pos(m_arrayCombatTile[iSelect]->Get_Position());
		_int iMapSize = (_int)m_mapStartWorldUI.size();
		m_mapStartWorldUI.emplace(iSelect, pWorldUI);
		if (iMapSize == (_int)m_mapStartWorldUI.size())
		{
			Safe_Release(pWorldUI);
		}
	}
	else if (ePrevTileOption == E_TILE_OPTION::TILE_START && eSelectTileOption != E_TILE_OPTION::TILE_START)
	{
		Safe_Release(m_mapStartWorldUI[iSelect]);
		m_mapStartWorldUI.erase(iSelect);
	}
}


void CTool_Tile::OnLbnSelchangeMw0109ListMascot()
{
	UpdateData(TRUE);
	_int iSelectMascot = m_cListMascot.GetCurSel();

	if (iSelectMascot < 0 || iSelectMascot >= (_int)m_vecMascot.size())
	{
		return;
	}


	m_strWorldCreaterPath = m_vecMascot[iSelectMascot]->Get_WorldName().c_str();
	m_cComboPlayType.SetCurSel((_int)m_vecMascot[iSelectMascot]->Get_PlayType());

	UpdateData(FALSE);
}


void CTool_Tile::OnCbnSelchangeMw0109ComboPlayType()
{
	UpdateData(TRUE);
	UpdateData(FALSE);

	_int iSelectMascot = m_cListMascot.GetCurSel();

	if (iSelectMascot < 0 || iSelectMascot >= (_int)m_vecMascot.size())
	{
		return;
	}

	_int iSelectType = m_cComboPlayType.GetCurSel();
	m_vecMascot[iSelectMascot]->Set_PlayType(E_ENEMY_PLAY_TYPE(iSelectType));
}


void CTool_Tile::OnBnClickedMw0109ButtonSave()
{
	if (!Check_TileField())
	{
		return;
	}

	CFileDialog Dlg(FALSE,
		L"tile",
		L"*.tile",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"..\\..\\Reference\\Data\\Data_Tile");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		DWORD dwByte;
		_int iOption;
		_int iLink;
		_vec3 vPos;
		
		for (_int i = 0; i < m_iMaxArray; ++i)
		{
			iOption = (_int)m_arrayCombatTile[i]->Get_TileOption();
			vPos = m_arrayCombatTile[i]->Get_Position();
			iLink = m_arrayCombatTile[i]->Get_LinkTileIndex();
			CUtility::Data_WriteInt(hFile, iOption, dwByte);
			CUtility::Data_WriteVec3(hFile, vPos, dwByte);
			CUtility::Data_WriteInt(hFile, iLink, dwByte);
		}
	
		_int iSize = (_int)m_vecMascot.size();
		CUtility::Data_WriteInt(hFile, iSize, dwByte);

		for (auto& pMascot : m_vecMascot)
		{
			CUtility::Data_WriteWString(hFile,pMascot->Get_WorldName(),dwByte);
			CUtility::Data_WriteInt(hFile, pMascot->Get_CurrentTileIndex(), dwByte);
			CUtility::Data_WriteInt(hFile, (_int)pMascot->Get_PlayType(), dwByte);
			CUtility::Data_WriteVec3(hFile, pMascot->Get_Position(), dwByte);
			CUtility::Data_WriteVec3(hFile, pMascot->Get_Angle(), dwByte);
		}

		CloseHandle(hFile);
	}
}


void CTool_Tile::OnBnClickedMw0109ButtonLoad()
{
	CFileDialog Dlg(TRUE,
		L"tile",
		L"*.tile",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Reference\\Data\\Data_Tile");

	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		for (auto& tPair : m_mapStartWorldUI)
		{
			Safe_Release(tPair.second);
		}
		m_mapStartWorldUI.clear();

		m_cListMascot.ResetContent();
		for (auto& pMascot : m_vecMascot)
		{
			Safe_Release(pMascot);
		}
		m_vecMascot.clear(); 

		for (_int i = 0; i < m_iMaxArray; ++i)
		{
			Safe_Release(m_arrayCombatTile[i]);

		}

		DWORD dwByte;
		for (_int i = 0; i < m_iMaxArray; ++i)
		{
		
			_int iOption;
			CUtility::Data_ReadInt(hFile, iOption, dwByte);
			m_arrayCombatTile[i] = CCombatTile::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, E_TILE_OPTION(iOption));
			_vec3 vPos;
			CUtility::Data_ReadVec3(hFile, vPos, dwByte);
			m_arrayCombatTile[i]->Set_Pos(vPos);
			_int iLink;
			CUtility::Data_ReadInt(hFile, iLink, dwByte);
			m_arrayCombatTile[i]->Set_LinkTileIndex(iLink);

			if (iOption == (_int)E_TILE_OPTION::TILE_START)
			{
				CWorldUI* pWorldUI = CWorldUI::Create(Management GetDevice(), E_OBJTAG::OBJ_Test, L"StartUI.uidat");
				assert(pWorldUI);

				pWorldUI->Set_Pos(vPos);
				m_mapStartWorldUI.emplace(i, pWorldUI);
			}
		}
		

		_int iSize;
		CUtility::Data_ReadInt(hFile, iSize, dwByte);

		CMascot* pMascot = nullptr;
		for (_int i=0; i<iSize; ++i)
		{
			wstring wstrWorldName;
			CUtility::Data_ReadWString(hFile, wstrWorldName, dwByte);
			_int iCurrentIndex;
			CUtility::Data_ReadInt(hFile, iCurrentIndex, dwByte);
			_int iPlayType;
			CUtility::Data_ReadInt(hFile, iPlayType, dwByte);
			
			pMascot = CMascot::Create(Management GetDevice(), E_OBJTAG::OBJ_Monster,wstrWorldName);
			if (!pMascot)
			{
				CloseHandle(hFile);
				return;
			}

			pMascot->Set_CurrentTileIndex(iCurrentIndex);
			_vec3 vPos;
			CUtility::Data_ReadVec3(hFile,vPos,dwByte);

			_vec3 vRot;
			CUtility::Data_ReadVec3(hFile, vRot, dwByte);

			pMascot->Set_Pos(vPos);
			pMascot->Set_Rotation(vRot);

			pMascot->Set_PlayType(E_ENEMY_PLAY_TYPE(iPlayType));

			m_vecMascot.emplace_back(pMascot);
			m_cListMascot.AddString(wstrWorldName.c_str());
		}

		CloseHandle(hFile);
	}
}


void CTool_Tile::OnEnChangeMw0110EditLinkTileIndex()
{
	UpdateData(TRUE);
	UpdateData(FALSE);

	_int iSelectTile = m_cListTile.GetCurSel();
	if (iSelectTile < 0 || iSelectTile >= m_iMaxArray)
	{
		return;
	}
	
	m_arrayCombatTile[iSelectTile]->Set_LinkTileIndex(_wtoi(m_strLinkTileIndex));
}
