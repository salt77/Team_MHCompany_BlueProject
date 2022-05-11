#include "stdafx.h"
#include "MFC_Project.h"
#include "Tool_Map.h"
#include "MainFrm.h"
#include "MainView.h"
#include "Scene_Lobby.h"
#include "afxdialogex.h"

#include "StageMap.h"
#include "Obstacle.h"
#include "Obstacle_Cafe.h"
#include "Obstacle_Combat.h"

IMPLEMENT_DYNAMIC(CTool_Map, CDialogEx)

CTool_Map::CTool_Map(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MH_TOOL_MAP, pParent)
{
}

CTool_Map::CTool_Map(CPopupTool_ObjectTree * pPopup, CWnd * pParent)
	: CDialogEx(IDD_MH_TOOL_UI, pParent)
	, m_pPopupTool(pPopup)
{
}

CTool_Map::~CTool_Map()
{
}

void CTool_Map::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JH_Map_List_Box, m_TreeMap);
	DDX_Control(pDX, IDC_JH_Map_Tree_Obstacle, m_TreeObstacle);
	DDX_Control(pDX, IDC_JH_CHECKBOX_OBSTACLE, m_CheckBox_RayCast);
}

BEGIN_MESSAGE_MAP(CTool_Map, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_JH_Add_Map, &CTool_Map::OnBnClicked_JH_Add_Map)
	ON_BN_CLICKED(IDC_JH_Save_Map, &CTool_Map::OnBnClicked_JH_Save_Map)
	ON_BN_CLICKED(IDC_JH_Load_Map, &CTool_Map::OnBnClicked_JH_Load_Map)
	ON_BN_CLICKED(IDC_JH_Add_Obstacle, &CTool_Map::OnBnClicked_JH_Add_Obstacle)
	ON_BN_CLICKED(IDC_JH_Save_Obstacle, &CTool_Map::OnBnClicked_JH_Save_Obstacle)
	ON_BN_CLICKED(IDC_JH_Load_Obstacle, &CTool_Map::OnBnClicked_JH_Load_Obstacle)
	ON_BN_CLICKED(IDC_JH_CHECKBOX_OBSTACLE, &CTool_Map::OnBnClicked_JH_RayCast)
END_MESSAGE_MAP()

//MFC 트리에 필요한 아이템 정보를 해당 변수들에 담고, 메인프레임에 있는 그래픽 디바이스를 가져온다.
BOOL CTool_Map::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	TI_Map.hInsertAfter = TVI_LAST;
	TI_Map.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	TI_Obstacle.hInsertAfter = TVI_LAST;
	TI_Obstacle.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	CMainFrame* MainFrame = Get_MFC_MainFrame();
	m_pGraphicDev = MainFrame->m_pGraphicDev;

	Loading();

	return TRUE;
}

//리스트박스에 있는 맵들을 상황에 맞게 생성한다. 카페 타일의 경우 한번 생성하면 24x24로 배치된다.
void CTool_Map::OnBnClicked_JH_Add_Map()
{
	UpdateData(TRUE);

	HTREEITEM iCurSel = m_TreeMap.GetSelectedItem();

	wstring	strTemp;
	strTemp = m_TreeMap.GetItemText(iCurSel);

	if (strTemp == L"Backstreet" ||
		strTemp == L"Beach" ||
		strTemp == L"Desertcity" ||
		strTemp == L"DesertTrack" ||
		strTemp == L"Highway[A]" ||
		strTemp == L"Hyakkiyakostreet" ||
		strTemp == L"Mainstreet[A]" ||
		strTemp == L"Mainstreet(Snow)[A]" ||
		strTemp == L"Market" ||
		strTemp == L"Market(Snow)" ||
		strTemp == L"Munitionsfactory" ||
		strTemp == L"Park[A]" ||
		strTemp == L"Redwinterstreet" ||
		strTemp == L"Ruinedschool[A]" ||
		strTemp == L"Ruins[A]" ||
		strTemp == L"School" ||
		strTemp == L"Shanhaijingstreet" ||
		strTemp == L"slumpia" ||
		strTemp == L"Subway" ||
		strTemp == L"Underground" ||
		strTemp == L"BossMap_Hieronymus" ||
		strTemp == L"BossMap_Binah"
		)
	{
		TotalMap();
		Get_MFC_PopupObject_Tree()->bUpdateNextFrame = TRUE;
		return;
	}

	_int iFindIndex = -1;
	for (_int i = 0; i < (_int)m_vecMapPathAndName.size(); ++i)
	{
		if (m_vecMapPathAndName[i].second == strTemp)
		{
			iFindIndex = i;
		}
	}

	if (-1 != iFindIndex)
	{
		if (strTemp == L"내츄럴 나무 바닥.X" ||
			strTemp == L"모눈 타일 바닥.X" ||
			strTemp == L"파란색 폴리싱 바닥.X" ||
			strTemp == L"블루 포인트 포세린 바닥.X" ||
			strTemp == L"발렌타인 분홍색 바닥.X"
			)
		{
			Management Set_Dead_CafeTile(E_LAYERTAG::Map);

			CreateTile(iFindIndex, strTemp);
		}
		else if (strTemp == L"내츄럴 나무 벽_Left.X" ||
				 strTemp == L"패턴 타일 벽_Left.X" ||
				 strTemp == L"파란 타일 벽_Left.X" ||
				 strTemp == L"여름 파도 무늬 벽_Left.X" ||
				 strTemp == L"발렌타인 초콜릿 벽_Left.X"
			)
		{
			Management Set_Dead_CafeWall_L(E_LAYERTAG::Map);

			CreateWall_L(iFindIndex, strTemp);
		}
		else if (strTemp == L"내츄럴 나무 벽_Right.X" ||
			strTemp == L"패턴 타일 벽_Right.X" ||
			strTemp == L"파란 타일 벽_Right.X" ||
			strTemp == L"여름 파도 무늬 벽_Right.X" ||
			strTemp == L"발렌타인 초콜릿 벽_Right.X"
			)
		{
			Management Set_Dead_CafeWall_R(E_LAYERTAG::Map);

			CreateWall_R(iFindIndex, strTemp);
		}
		else
		{
			m_pMap = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, m_vecMapPathAndName[iFindIndex].first, strTemp);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pMap);
		}
	}

	Get_MFC_PopupObject_Tree()->bUpdateNextFrame = TRUE;

	UpdateData(FALSE);
}

//E_LAYERTAG::Map에는 OBJ_Map, OBJ_CafeTile이 같이 존재한다. 고로 저장또한 동시에 진행한다. (각 OBJ태그가 있는 것들만 저장되게 예외처리)
//벡터로 각각 Size만큼 담아서 저장해준다.
void CTool_Map::OnBnClicked_JH_Save_Map()
{
	CFileDialog Dlg(FALSE, L"map", L"*.map", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	wstring wstrFullPath = szFilePath;
	_int iCheck = (_int)wstrFullPath.rfind(L"Team118A_Uint");
	wstring wstrLocalPath = wstrFullPath.substr(0,iCheck);
	wstrLocalPath += L"Team118A_Uint\\Reference\\Data\\Data_Map Save, Load\\";
	Dlg.m_ofn.lpstrInitialDir = wstrLocalPath.c_str();

	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		vector<CGameObject*>* vecMap = Management Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
		vector<CGameObject*>* vecTile = Management Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile);

		if (vecMap != nullptr)
		{
			int iSaveMapSize = (int)vecMap->size();
			CUtility::Data_WriteInt(hFile, iSaveMapSize, dwByte);

			for (int i = 0; i < iSaveMapSize; ++i)
			{
				(*vecMap)[i]->Save(hFile, dwByte);
			}
		}

		if (vecTile != nullptr)
		{
			int iSaveTileSize = (int)vecTile->size();
			CUtility::Data_WriteInt(hFile, iSaveTileSize, dwByte);

			for (int i = 0; i < iSaveTileSize; ++i)
			{
				(*vecTile)[i]->Save(hFile, dwByte);
			}
		}

		CloseHandle(hFile);
	}
}

//기존에 씬에 있던 정보들은 전부 지우고 저장한 정보를 Size만큼 받아와서 객체를 생성해준다.
void CTool_Map::OnBnClicked_JH_Load_Map()
{
	CFileDialog Dlg(TRUE, L"map", L"*.map", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	wstring wstrFullPath = szFilePath;
	_int iCheck = (_int)wstrFullPath.rfind(L"Team118A_Uint");
	wstring wstrLocalPath = wstrFullPath.substr(0, iCheck);
	wstrLocalPath += L"Team118A_Uint\\Reference\\Data\\Data_Map Save, Load\\";
	Dlg.m_ofn.lpstrInitialDir = wstrLocalPath.c_str();

	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;

		Management Set_Dead_AllObject(E_LAYERTAG::Map);

		int iLoadMapSize = 0;
		CUtility::Data_ReadInt(hFile, iLoadMapSize, dwByte);

		for (int i = 0; i < iLoadMapSize; ++i)
		{
			_int iObjTag = 0;
			wstring wstrName;
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrName, dwByte);
			CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			m_pMap = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, wstrPath, wstrName);
			m_pMap->Load(hFile, dwByte);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pMap);
		}

		int iLoadTileSize = 0;
		CUtility::Data_ReadInt(hFile, iLoadTileSize, dwByte);

		for (int i = 0; i < iLoadTileSize; ++i)
		{
			_int iObjTag = 0;
			wstring wstrName;
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrName, dwByte);
			CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			m_pTile = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_CafeTile, wstrPath, wstrName);
			m_pTile->Load(hFile, dwByte);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile, m_pTile);
		}

		Get_MFC_PopupObject_Tree()->bUpdateNextFrame = TRUE;

		CloseHandle(hFile);
	}
}

//리스트박스에 있는 Obstacle를 상황에 맞게 생성한다. (Obstacle = 엄폐물, 카페 가구)
void CTool_Map::OnBnClicked_JH_Add_Obstacle()
{
	UpdateData(TRUE);
	HTREEITEM iCurSel = m_TreeObstacle.GetSelectedItem();
	wstring	strTemp;
	strTemp = m_TreeObstacle.GetItemText(iCurSel);
	
	HTREEITEM hParentItem = m_TreeObstacle.GetNextItem(iCurSel, TVGN_PARENT);

	if (nullptr == hParentItem)
		return;

	_int iFindIndex = -1;
	for (_int i = 0; i < (_int)m_vecObstaclePathAndName.size(); ++i)
	{
		if (m_vecObstaclePathAndName[i].second == strTemp)
		{
			iFindIndex = i;
		}
	}

	if (-1 != iFindIndex)
	{
		m_pObstacle_Cafe = CObstacle_Cafe::Create(m_pGraphicDev, E_OBJTAG::OBJ_Obstacle, m_vecObstaclePathAndName[iFindIndex].first, strTemp);
		Management Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
		m_pObstacle_Cafe->Set_Scale(0.5f, 0.5f, 0.5f);
	}

	Get_MFC_PopupObject_Tree()->bUpdateNextFrame = TRUE;

	UpdateData(FALSE);
}

//Obstacle을 벡터로 Size만큼 담아서 저장해준다.
void CTool_Map::OnBnClicked_JH_Save_Obstacle()
{
	CFileDialog Dlg(FALSE, L"obs", L"*.obs", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	wstring wstrFullPath = szFilePath;
	_int iCheck = (_int)wstrFullPath.rfind(L"Team118A_Uint");
	wstring wstrLocalPath = wstrFullPath.substr(0, iCheck);
	wstrLocalPath += L"Team118A_Uint\\Reference\\Data\\Data_Obstacle Save, Load\\";
	Dlg.m_ofn.lpstrInitialDir = wstrLocalPath.c_str();

	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		m_vecObstacle_Cafe = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

		if (m_vecObstacle_Cafe != nullptr)
		{
			int iSaveObstacle_CafeSize = (int)m_vecObstacle_Cafe->size();
			WriteFile(hFile, &iSaveObstacle_CafeSize, sizeof(int), &dwByte, nullptr);

			for (int i = 0; i < iSaveObstacle_CafeSize; ++i)
			{
				(*m_vecObstacle_Cafe)[i]->Save(hFile, dwByte);
			}
		}

		CloseHandle(hFile);
	}
}

//기존에 씬에 있던 정보들은 전부 지우고 저장한 정보를 Size만큼 받아와서 객체를 생성해준다.
void CTool_Map::OnBnClicked_JH_Load_Obstacle()
{
	CFileDialog Dlg(TRUE, L"obs", L"*.obs", OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	wstring wstrFullPath = szFilePath;
	_int iCheck = (_int)wstrFullPath.rfind(L"Team118A_Uint");
	wstring wstrLocalPath = wstrFullPath.substr(0, iCheck);
	wstrLocalPath += L"Team118A_Uint\\Reference\\Data\\Data_Obstacle Save, Load\\";
	Dlg.m_ofn.lpstrInitialDir = wstrLocalPath.c_str();

	if (IDOK == Dlg.DoModal())
	{
		wstring wstrFilePath = Dlg.GetPathName().GetString();
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		DWORD dwByte = 0;

		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}

		Get_MFC_MainFrame()->m_pMenuForm->m_pPopupTool_ObjectTree->m_pPickObject = nullptr;

		Management Set_Dead_AllObject(E_LAYERTAG::Obstacle);
		
		int iLoadObstacle_CafeSize = 0;
		CUtility::Data_ReadInt(hFile, iLoadObstacle_CafeSize, dwByte);

		for (int i = 0; i < iLoadObstacle_CafeSize; ++i)
		{
			_int iObjTag = 0;
			wstring wstrName;
			wstring wstrPath;
			CUtility::Data_ReadWString(hFile, wstrName, dwByte);
			CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
			CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

			m_pObstacle_Cafe = CObstacle_Cafe::Create(m_pGraphicDev, E_OBJTAG::OBJ_Obstacle, wstrPath, wstrName);
			m_pObstacle_Cafe->Load(hFile, dwByte);
			Management Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
		}

		Get_MFC_PopupObject_Tree()->bUpdateNextFrame = TRUE;

		CloseHandle(hFile);
	}
}

//Tool_Map에 있는 레이캐스트 체크박스의 체크 유무를 판단하여 Obstacle을 움직일지 말지를 Set으로 넘겨준다.
void CTool_Map::OnBnClicked_JH_RayCast()
{
	if (dynamic_cast<CScene_Lobby*>(Management Get_Scene()) == nullptr)
	{
		return;
	}

	if (m_CheckBox_RayCast.GetCheck())
	{
		static_cast<CScene_Lobby*>(Management Get_Scene())->Set_ObstacleMove(true);
	}
	else
	{
		static_cast<CScene_Lobby*>(Management Get_Scene())->Set_ObstacleMove(false);
	}
}

//선택한 문자열의 자식트리를 가지는 문자열(.X)들의 정보를 한번에 생성해준다. 
void CTool_Map::TotalMap()
{
	HTREEITEM iCurSel = m_TreeMap.GetSelectedItem();
	wstring	strTemp;
	strTemp = m_TreeMap.GetItemText(iCurSel);
	int iChildCount = 0;
	_int iFindIndex = -1;
	HTREEITEM iChild = m_TreeMap.GetChildItem(iCurSel);

	while (iChild)
	{
		iChildCount++;
		iChild = m_TreeMap.GetNextSiblingItem(iChild);
	}

	for (_int i = 0; i < (_int)m_vecMapPathAndName.size(); ++i)
	{
		if (m_vecMapPathAndName[i].second == strTemp)
		{
			iFindIndex = i;
		}
	}

	if (-1 == iFindIndex)
		return;

	for (_int i = 1; i < iChildCount + 1; ++i)
	{
		wstring wstrChildName = strTemp + to_wstring(i) + L".X";
		m_pMap = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, m_vecMapPathAndName[iFindIndex].first, wstrChildName);
		assert(m_pMap);
		Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pMap);
	}
}

void CTool_Map::CreateTile(_int Index, wstring MapName)
{
	_float fX = 0.f, fZ = 0.f;
	_float fOneX = 0.f, fOneZ = 0.f;

	for (size_t i = 0; i < 24; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fZ = i * (0.6f);

			fOneX = 0.6f * 11.5f;
			fOneZ = 0.6f * 11.75f;

			m_pTile = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_CafeTile, m_vecMapPathAndName[Index].first, MapName);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile, m_pTile);

			m_pTile->Set_Pos(_vec3(fX - fOneX, 0.f, fZ - fOneZ));
		}
	}
}

void CTool_Map::CreateWall_L(_int Index, wstring pWall_LName)
{
	_float fX_L = 0.f, fY_L = 0.f;
	_float fOneX_L = 0.f, fOneY_L = 0.f;

	for (size_t i = 0; i < 1; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fY_L = i * (0.6f);

			fOneX_L = 0.6f * 11.25f;
			fOneY_L = 0.f;

			m_pWall_L = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, m_vecMapPathAndName[Index].first, pWall_LName);
			assert(m_pWall_L);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_L);

			m_pWall_L->Set_Pos(_vec3(fX_L - fOneX_L, fY_L - fOneY_L, 7.05f));
			m_pWall_L->Set_Scale(0.5f, 1.f, 1.f);
		}
	}
}

void CTool_Map::CreateWall_R(_int Index, wstring pWall_RName)
{
	_float fY_R = 0.f, fZ_R = 0.f;
	_float fOneY_R = 0.f, fOneZ_R = 0.f;

	for (size_t i = 0; i < 1; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fZ_R = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fY_R = i * (0.6f);

			fOneZ_R = 0.6f * 11.5f;
			fOneY_R = 0.f;

			m_pWall_R = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, m_vecMapPathAndName[Index].first, pWall_RName);
			assert(m_pWall_R);
			Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pWall_R);

			m_pWall_R->Set_Pos(_vec3(7.2f, fY_R - fOneY_R, fZ_R - fOneZ_R));
			m_pWall_R->Set_Scale(1.f, 1.f, 0.5f);
		}
	}
}

//맵 생성에 필요한 맵들의 경로 모음
void CTool_Map::Loading()
{
	_finddata_t findData;
	intptr_t lHandle = 0;

	//[Map_Combat]
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Backstreet/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Backstreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Beach/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Beach", lHandle, findData);
	
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Desertcity/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Desertcity", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/DesertTrack/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/DesertTrack", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Highway[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Highway[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Hyakkiyakostreet/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Hyakkiyakostreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Mainstreet[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Mainstreet[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Mainstreet(Snow)[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Mainstreet(Snow)[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Market/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Market", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Market(Snow)/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Market(Snow)", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Munitionsfactory/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Munitionsfactory", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Park[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Park[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Redwinterstreet/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Redwinterstreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Ruinedschool[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Ruinedschool[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Ruins[A]/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Ruins[A]", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/School/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/School", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Shanhaijingstreet/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Shanhaijingstreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/slumpia/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/slumpia", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Subway/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Subway", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/Underground/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/Underground", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/BossMap_Hieronymus/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/BossMap_Hieronymus", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/BossMap_Binah/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/BossMap_Binah", lHandle, findData);

	//[Obstacle_Combat]
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Common/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Common", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Backstreet/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Backstreet", lHandle, findData);
	
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Beach/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Beach", lHandle, findData);
	
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Desertcity/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Desertcity", lHandle, findData);
	
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/DesertTrack/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/DesertTrack", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Hyakkiyakostreet/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Hyakkiyakostreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Munitionsfactory/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Munitionsfactory", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Park/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Park", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Redwinterstreet/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Redwinterstreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Ruinedschool/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Ruinedschool", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Ruins/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Ruins", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/School/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/School", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Shanhaijingstreet/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Shanhaijingstreet", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/slumpia/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/slumpia", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Subway/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Subway", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/Underground/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/Underground", lHandle, findData);

	//[Map_Cafe]
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/x[Cafe]x_Default/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/x[Cafe]x_Default", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/x[Cafe]x_GameCenter/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/x[Cafe]x_GameCenter", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/x[Cafe]x_Momo/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/x[Cafe]x_Momo", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/x[Cafe]x_PoolParty/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/x[Cafe]x_PoolParty", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Map/x[Cafe]x_Valentine/", "*.X", &findData);
	Load_Map_Path(L"../../Reference/Resource/Map/x[Cafe]x_Valentine", lHandle, findData);

	//[Obstacle_Cafe]
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_Default/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_Default", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_Momo/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine", lHandle, findData);

	lHandle = CUtility::FindData_ChoiceType("../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/", "*.X", &findData);
	Load_Obstacle_Path(L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy", lHandle, findData);
}

//Map 폴더 내부에 .X 문자열이 포함되어 있는 정보들을 트리 컨트롤에 사전 로딩하기 위해 경로를 설정한다.
void CTool_Map::Load_Map_Path(_tchar* wPath, intptr_t lhandle, _finddata_t& findData)
{
	intptr_t lResult = 0;
	wstring wstrTemp = wPath;
	_int iCheck = (_int)wstrTemp.find(L"Map");
	wstrTemp = wstrTemp.substr(iCheck + 4);

	_tchar szTemp[MAX_PATH]{};
	swprintf_s(szTemp, L"%s", wstrTemp.c_str());
	TI_Map.item.pszText = szTemp;
	hMapItem = InsertTree(0, szTemp, TI_Map, m_TreeMap);

	wstring wstrFullPath = wPath;
	wstrFullPath.append(L"/");
	m_vecMapPathAndName.push_back((make_pair(wstrFullPath, szTemp)));

	while (-1 != lResult)
	{
		wchar_t szFullPath[MAX_PATH] = L"";
		wchar_t szName[MAX_PATH] = L"";

		lstrcpy(szFullPath, wPath);
		lstrcat(szFullPath, L"/");

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);

		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
			InsertTree(hMapItem, szName, TI_Map, m_TreeMap);
			m_vecMapPathAndName.push_back(make_pair(szFullPath, szName));
		}

		lResult = _findnext(lhandle, &findData);
	}

	_findclose(lhandle);
}

//Obstacle 폴더 내부에 .X 문자열이 포함되어 있는 정보들을 트리 컨트롤에 사전 로딩하기 위해 경로를 설정한다.
void CTool_Map::Load_Obstacle_Path(_tchar * wPath, intptr_t lhandle, _finddata_t & findData)
{
	intptr_t lResult = 0;
	wstring wstrTemp = wPath;
	_int iCheck = (_int)wstrTemp.find(L"Obstacle");
	wstrTemp = wstrTemp.substr(iCheck + 9);

	_tchar szTemp[MAX_PATH]{};
	swprintf_s(szTemp, L"%s", wstrTemp.c_str());
	TI_Obstacle.item.pszText = szTemp;
	hObstacleItem = InsertTree(0, szTemp, TI_Obstacle, m_TreeObstacle);

	wstring wstrFullPath = wPath;
	wstrFullPath.append(L"/");
	m_vecObstaclePathAndName.push_back((make_pair(wstrFullPath, szTemp)));

	while (-1 != lResult)
	{
		wchar_t szFullPath[MAX_PATH] = L"";
		wchar_t szName[MAX_PATH] = L"";

		lstrcpy(szFullPath, wPath);
		lstrcat(szFullPath, L"/");

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);

		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
			InsertTree(hObstacleItem, szName, TI_Obstacle, m_TreeObstacle);
			m_vecObstaclePathAndName.push_back(make_pair(szFullPath, szName));
		}

		lResult = _findnext(lhandle, &findData);
	}

	_findclose(lhandle);
}

//위 경로들을 바탕으로 트리컨트롤에 넣어준다.
HTREEITEM CTool_Map::InsertTree(HTREEITEM hParent, _tchar* wText, TV_INSERTSTRUCTW & ti, CTreeCtrl & treeCtrl)
{
	ti.item.pszText = wText;
	ti.hParent = hParent;
	return treeCtrl.InsertItem(&ti);
}