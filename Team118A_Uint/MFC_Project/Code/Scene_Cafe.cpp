#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_Cafe.h"
#include "Scene_Manager.h"
#include "StageMap.h"
#include "Obstacle.h"
#include "Obstacle_Cafe.h"
#include "Camera_Cafe.h"
#include "Invite_Character.h"
#include "UI_Manager.h"

USING(Engine)

_bool CScene_Cafe::bUniquePrototype = TRUE;
CScene_Cafe::CScene_Cafe(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_Cafe::~CScene_Cafe()
{
}

CScene_Cafe * CScene_Cafe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Cafe*	pInstance = new CScene_Cafe(pGraphicDev);


	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_Cafe::Free()
{
	for (auto& pChar : m_vecInvite_Char)
	{
		Safe_Release(pChar);
	}
	m_vecInvite_Char.clear();
	m_vecInvite_Char.shrink_to_fit();

	CFogManager::GetInstance()->SetCustomFog(T_CustomFog());

	CScene::Free();
}

//최초 Scene들의 정보를 출력
HRESULT CScene_Cafe::Ready_Scene()
{
	AfxGetApp()->GetMainWnd()->SetWindowTextW(L"카페");
	CScene::Ready_Scene();
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();
	Set_UIManagerFunc();

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_시스템_카페.mp3", 0.2f);

	return S_OK;
}

//Scene 시작 시 Cafe_Camara를 사용한다.
_int CScene_Cafe::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta);

	CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_CAFE);

	CUI_Manager::GetInstance()->Create_Ui_Cafe();

	return iExit;
}

//실시간으로 Camera, Scene 전환 및 각종 가구 기능들 수행
_int CScene_Cafe::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F2) && FALSE == bUniquePrototype)
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
		CFogManager::GetInstance()->SetCustomFog(T_CustomFog());
	}

	if (Management Key_Down(KEY_F3) && FALSE == bUniquePrototype)
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
		CFogManager::GetInstance()->SetCustomFog(T_CustomFog());
	}

	if (Management Key_Down(KEY_F6))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}

	if (Management Key_Down(KEY_G))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_DYNAMIC);
	}
	else if (Management Key_Down(KEY_F))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_CAFE);
	}

	if (m_bPresetFog)
	{
		m_fPresetFogTime += fTimeDelta;

		Update_FogEvent(fTimeDelta);

		if (m_fPresetFogTime >= 4.f)
		{
			Set_FogEvent();
			m_fFogNowTime = 0.f;
			m_fPresetFogTime = 0.f;
			m_bPresetFog = false;
		}
	}

	SelectFurniture();
	PickingFurniture();
	DeleteFurniture();
	
	PickChar();
	MoveChar();
	
	UI_AmenityInfo();
	UI_FurnitureInfo();
	UI_UpdateProfit(fTimeDelta);
	UI_Preset(fTimeDelta);
	UI_PresetName();

	if (CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_Active())
	{
		MoveFurniture();
		RotateFurniture();
	}

	return	iExit;
}

//Default Cafe, Furniture 최초 Load
_int CScene_Cafe::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	if (m_bCreate)
	{
		LoadCafeMap(L"../../Reference/Data/Data_Map Save, Load/Cafe.map");
		LoadCafeFurniture(L"../../Reference/Data/Data_Obstacle Save, Load/Cafe.obs");
		m_bCreate = false;
	}

	UI_DeleteFurnitureInfo();
	UI_StartAmenityInfo();

	return iExit;
}

void CScene_Cafe::Render_Scene()
{
}

//가구, 장식용 Raycast = pOutPos로 위치값을 bCol로 광선 충돌 여부를 판단하여 넘겨준다.
//광선 충돌 시 메쉬 행렬 정렬을 따지지 않고 수행한다.
_vec3 CScene_Cafe::Raycast(HWND hWnd, const _matrix* pMatWorld, _vec3* pOutPos, LPD3DXMESH pMesh, _bool* bCol)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 윈도우 좌표를 투영 좌표로 변환
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 좌표를 뷰스페이스 좌표로 변환
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰스페이스 좌표를 월드 좌표로 변환
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	// 광선을 발사하는 위치 vRayPos, 광선이 향하는 방향을 vRayDir
	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.0f, 0.0f, 0.0f);
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 좌표에서 로컬 좌표로 변환
	_matrix	 matWorld = *pMatWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	BOOL bCOL;
	DWORD dwFaceIndex;
	_float fV, fU, fDist;
	LPD3DXBUFFER pBuffer;
	DWORD dwHit;

	D3DXIntersect(pMesh, &vRayPos, &vRayDir, &bCOL, &dwFaceIndex, &fU, &fV, &fDist, &pBuffer, &dwHit);

	if (bCOL)
	{
		*bCol = TRUE;
	}
	else
	{
		*bCol = FALSE;
	}

	_vec3 vPos = vRayPos + vRayDir * fDist;
	D3DXVec3TransformCoord(&vPos, &vPos, pMatWorld);
	*pOutPos = vPos;

	return vPos;
}

//내장 가구 Raycast = pOutDistance로 메쉬 거리값을, bCol로 광선 충돌 여부를 판단하여 넘겨준다.
//광선 충돌 시 메쉬 행렬 정렬을 수행하여 마우스 포인터로부터 가장 가깝게 충돌한 UV값을 출력한다.
_vec3 CScene_Cafe::TriRaycast(HWND hWnd, CSMesh * pMapBufferCom, CTransform * pMapTransCom, _float* pOutDistance, _bool* bCol)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix	 matWorld = pMapTransCom->Get_WorldMatrix();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_vec3 v1, v2, v3;
	ZeroMemory(&v1, sizeof(_vec3));
	ZeroMemory(&v2, sizeof(_vec3));
	ZeroMemory(&v3, sizeof(_vec3));

	LPD3DXMESH pMesh = pMapBufferCom->Get_Mesh();

	float fU = 0.f, fV = 0.f;

	int iTriCount = pMesh->GetNumFaces();

	void* pIndices;
	void* pVertices;

	pMesh->LockVertexBuffer(0, &pVertices);
	pMesh->LockIndexBuffer(0, &pIndices);

	_vec3* VtxBuffer;
	VtxBuffer = pMapBufferCom->Get_VtxPos();

	_float fDistance = 999999.f;
	_float fDist = 0.f;
	_vec3 vResultPos;
	_bool bCOL = false;

	for (int i = 0; i < iTriCount; ++i)
	{
		WORD _1, _2, _3;
		memcpy(&_1, (char*)pIndices + i * sizeof(INDEX16), sizeof(WORD));
		memcpy(&_2, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD), sizeof(WORD));
		memcpy(&_3, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD) * 2, sizeof(WORD));

		memcpy(&v1, VtxBuffer[_1], sizeof(_vec3));
		memcpy(&v2, VtxBuffer[_2], sizeof(_vec3));
		memcpy(&v3, VtxBuffer[_3], sizeof(_vec3));

		if (D3DXIntersectTri(&v1, &v2, &v3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			_vec3 vOut;

			vOut = v1 + (fU * (v2 - v1)) + fV * (v3 - v1);

			//가장 가까운 거리로 레이캐스트(정렬)
			if (fDist < fDistance)
			{
				fDistance = fDist;
				vResultPos = vOut;

				bCOL = true;

				if (bCOL)
					*bCol = bCOL;
			}
		}
	}

	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	//만약 값이 없다면 0,0,0으로 초기화
	if (fDistance > 99999.f)
	{
		bCOL = false;

		*pOutDistance = 99999.f;

		return _vec3{ 0.f, 0.f, 0.f };
	}
	else
	{
		D3DXVec3TransformCoord(&vResultPos, &vResultPos, &pMapTransCom->Get_WorldMatrix());

		*pOutDistance = fDistance;

		return vResultPos;
	}
}

_bool CScene_Cafe::RayChar(_vec3 * pOutPos)
{
	map<E_OBJTAG, vector<CGameObject*>>*  pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Map);

	_float fDistance = 999999.f;
	_vec3 vResultPos;
	for (auto& tPair : *pLayerMap)
	{
		for (auto& pObject : tPair.second)
		{
			CTransform* pTransform = static_cast<CTransform*>(pObject->Get_Component(ComTag_Transform));
			CSMesh* pSMesh = static_cast<CSMesh*>(pObject->Get_Component(ComTag_SMesh));

			_vec3 vCheckPos;
			_float fDist;
			if (pSMesh->PickingMesh(g_hWnd, &vCheckPos, &fDist, pTransform->Get_WorldMatrix()))
			{
				if (fDist < fDistance)
				{
					fDistance = fDist;
					vResultPos = vCheckPos;
				}
			}
		}
	}

	if (fDistance < 999998.f)
	{
		*pOutPos = vResultPos;
		return TRUE;
	}

	return FALSE;
}

void CScene_Cafe::Set_UIManagerFunc()
{
	CUI_Manager::GetInstance()->Set_Func_PickFurniture(bind(&CScene_Cafe::Set_PickFurniture, this, true));
	CUI_Manager::GetInstance()->Set_Func_RotFurniture(bind(&CScene_Cafe::Set_RotFurniture, this, true));
	CUI_Manager::GetInstance()->Set_Func_StoFurniture(bind(&CScene_Cafe::Set_StoFurniture, this, true));
	CUI_Manager::GetInstance()->Set_Func_SelFurnitureInfo(bind(&CScene_Cafe::Set_SelectFurInfo, this, true));
	CUI_Manager::GetInstance()->Set_Func_AllStoFurniture(bind(&CScene_Cafe::Set_AllStoFurniture, this, true));
	CUI_Manager::GetInstance()->Set_Func_CreateFurniturePick(bind(&CScene_Cafe::Set_CreateFurPick, this, true));
	CUI_Manager::GetInstance()->Set_Func_CafeInfoAmenity(bind(&CScene_Cafe::Set_OpenAmenity, this, true));
	CUI_Manager::GetInstance()->Set_Func_OpenFurInfo(bind(&CScene_Cafe::Set_OpenFurInfo, this, true));
	CUI_Manager::GetInstance()->Set_Func_ExitFurInfo(bind(&CScene_Cafe::Set_ExitFurInfo, this, true));
	CUI_Manager::GetInstance()->Set_Func_UpdateAmenity(bind(&CScene_Cafe::Set_UpdateAmenity, this, true));
	CUI_Manager::GetInstance()->Set_Func_ProfitRecieve(bind(&CScene_Cafe::Set_Recieve, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName(bind(&CScene_Cafe::Set_PresetName, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetNameEnd(bind(&CScene_Cafe::Set_PresetNameEnd, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName1(bind(&CScene_Cafe::Set_PresetName1, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName2(bind(&CScene_Cafe::Set_PresetName2, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName3(bind(&CScene_Cafe::Set_PresetName3, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName4(bind(&CScene_Cafe::Set_PresetName4, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetName5(bind(&CScene_Cafe::Set_PresetName5, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetInit1(bind(&CScene_Cafe::Set_PresetInit1, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetSave1(bind(&CScene_Cafe::Set_PresetSave1, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetApply1(bind(&CScene_Cafe::Set_PresetApply1, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetInit2(bind(&CScene_Cafe::Set_PresetInit2, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetSave2(bind(&CScene_Cafe::Set_PresetSave2, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetApply2(bind(&CScene_Cafe::Set_PresetApply2, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetInit3(bind(&CScene_Cafe::Set_PresetInit3, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetSave3(bind(&CScene_Cafe::Set_PresetSave3, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetApply3(bind(&CScene_Cafe::Set_PresetApply3, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetInit4(bind(&CScene_Cafe::Set_PresetInit4, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetSave4(bind(&CScene_Cafe::Set_PresetSave4, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetApply4(bind(&CScene_Cafe::Set_PresetApply4, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetInit5(bind(&CScene_Cafe::Set_PresetInit5, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetSave5(bind(&CScene_Cafe::Set_PresetSave5, this, true));
	CUI_Manager::GetInstance()->Set_Func_PresetApply5(bind(&CScene_Cafe::Set_PresetApply5, this, true));
	CUI_Manager::GetInstance()->Set_Func_Gift1(bind(&CScene_Cafe::Set_Gift1, this, true));
	CUI_Manager::GetInstance()->Set_Func_Gift2(bind(&CScene_Cafe::Set_Gift2, this, true));
	CUI_Manager::GetInstance()->Set_Func_Gift3(bind(&CScene_Cafe::Set_Gift3, this, true));
	CUI_Manager::GetInstance()->Set_Func_Gift4(bind(&CScene_Cafe::Set_Gift4, this, true));
	CUI_Manager::GetInstance()->Set_Func_Gift5(bind(&CScene_Cafe::Set_Gift5, this, true));
}

void CScene_Cafe::Set_FogEvent()
{
	_vec3 vCentor = _vec3(-10.f, 0.f, 10.f);
	_float fDis = 6.f;
	_float fDest = 50.f;

	//Custom Fog [0]
	m_tCustomFog[0].vPos = vCentor;
	m_tCustomFog[0].fDistance_Start = 1.f;
	m_tCustomFog[0].vColor_Start = _vec4(1.f, 0.f, 1.f, 0.4f);
	m_tCustomFog[0].fDistance_End = m_tCustomFog[0].fDistance_Start + fDis;
	m_tCustomFog[0].vColor_End = _vec4(1.f, 0.f, 1.f, 0.4f);
	
	//Custom Fog [1]
	m_tCustomFog[1].vPos = vCentor;
	m_tCustomFog[1].fDistance_Start = m_tCustomFog[0].fDistance_Start + fDest;
	m_tCustomFog[1].vColor_Start = _vec4(1.f, 0.f, 1.f, 0.1f);
	m_tCustomFog[1].fDistance_End = m_tCustomFog[1].fDistance_Start + fDis;
	m_tCustomFog[1].vColor_End = _vec4(1.f, 0.f, 1.f, 0.1f);
}

void CScene_Cafe::Update_FogEvent(const _float & fTimeDelta)
{
	Set_FogEvent();

	m_fFogNowTime += fTimeDelta;

	m_fFogTotalTime = 4.f;
	_float fRatio = m_fFogNowTime / m_fFogTotalTime;
	fRatio = CUtility::Clamp_float(fRatio, 0.f, 1.f);

	CUtility::LerpEase_float(E_EASE::OutQuint, fRatio);

	T_CustomFog tCustomFog = T_CustomFog();
	tCustomFog.vPos				= m_tCustomFog[0].vPos;
	tCustomFog.fDistance_Start	= CUtility::Lerp_float(m_tCustomFog[0].fDistance_Start, m_tCustomFog[1].fDistance_Start, fRatio);
	tCustomFog.fDistance_End	= CUtility::Lerp_float(m_tCustomFog[0].fDistance_End, m_tCustomFog[1].fDistance_End, fRatio);
	tCustomFog.vColor_Start		= CUtility::Lerp_vec4(m_tCustomFog[0].vColor_Start, m_tCustomFog[1].vColor_Start, fRatio);
	tCustomFog.vColor_End		= CUtility::Lerp_vec4(m_tCustomFog[0].vColor_End, m_tCustomFog[1].vColor_End, fRatio);
	CFogManager::GetInstance()->SetCustomFog(tCustomFog);
}

//wstrFilePath를 통해 경로를 받아서 해당 경로에 있는 Load 파일의 정보를 읽어 객체를 생성한다.
void CScene_Cafe::LoadCafeMap(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
		return;

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

//wstrFilePath를 통해 경로를 받아서 해당 경로에 있는 Load 파일의 정보를 읽어 객체를 생성한다.
void CScene_Cafe::LoadCafeFurniture(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int iLoadObstacleSize = 0;
	CUtility::Data_ReadInt(hFile, iLoadObstacleSize, dwByte);

	for (int i = 0; i < iLoadObstacleSize; ++i)
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

//블럭타일 생성
void CScene_Cafe::CreateBlock()
{
	_float fX = 0.f, fZ = 0.f;
	_float fX_L = 0.f, fZ_L = 0.f;
	_float fX_R = 0.f, fZ_R = 0.f;
	_float fOneX = 0.f, fOneZ = 0.f;
	_float fOneX_L = 0.f, fOneZ_L = 0.f;
	_float fOneX_R = 0.f, fOneZ_R = 0.f;
	m_vecBlock.reserve(24 * 24);
	m_vecBlock_L.reserve(24 * 8);
	m_vecBlock_R.reserve(24 * 8);

	for (size_t i = 0; i < 24; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fZ = i * (0.6f);

			fOneX = 0.6f * 11.5f;
			fOneZ = 0.6f * 11.75f;

			m_pBlock = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_BlockTile, L"블럭타일_B.X");
			assert(m_pBlock);
			Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_BlockTile, m_pBlock);

			m_pBlock->Set_Pos(_vec3(fX - fOneX, 0.01f, fZ - fOneZ));

			m_pBlock->Set_Scale(1.f, 1.f, 1.f);

			m_vecBlock.emplace_back(m_pBlock);
		}
	}

	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX_L = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fZ_L = i * (0.6f);

			fOneX_L = 0.6f * 11.5f;
			fOneZ_L = -0.f;

			m_pBlock_L = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_BlockTile_L, L"블럭타일_L.X");
			assert(m_pBlock_L);
			Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_BlockTile_L, m_pBlock_L);

			m_pBlock_L->Set_Pos(_vec3(fX_L - fOneX_L, fZ_L - fOneZ_L, 7.f));
			m_pBlock_L->Set_Rotation(E_ROTATION::ROT_X, 90.f);
			m_pBlock_L->Set_Scale(1.f, 1.f, 1.f);

			m_vecBlock_L.emplace_back(m_pBlock_L);
		}
	}

	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < 24; ++j)
		{
			fX_R = (j * 0.6f) + ((i % 1) * (0.6f * 0.5f));
			fZ_R = i * (0.6f);

			fOneX_R = 0.6f * 11.7f;
			fOneZ_R = -0.f;

			m_pBlock_R = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_BlockTile_R, L"블럭타일_R.X");
			assert(m_pBlock_R);
			Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_BlockTile_R, m_pBlock_R);

			m_pBlock_R->Set_Pos(_vec3(7.15f, fZ_R - fOneZ_R, fX_R - fOneX_R));
			m_pBlock_R->Set_Rotation(E_ROTATION::ROT_Z, 90.f);
			m_pBlock_R->Set_Scale(1.f, 1.f, 1.f);

			m_vecBlock_R.emplace_back(m_pBlock_R);
		}
	}
}

//Tab키를 통해 가구를 편집할지 안할지의 여부를 판단한다.
void CScene_Cafe::SelectFurniture()
{
	if (CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(0)->Get_Active())
	{
		m_bFurniturePick = false;
	}

	if (m_bCreateFurPick)
	{
		UI_CreateFurPick();
		m_bCreateFurPick = false;
	}

	UI_SelFurInfo();

	//if (Management Key_Down(KEY_Tab))
	//{
	//	m_bFurniturePick = !m_bFurniturePick;
	//}

	if (m_bFurniturePick)
	{
		m_bOnefor = true;
	}
	else
	{
		if (m_bOnefor)
		{
			for (auto& pBlock : m_vecBlock)
			{
				pBlock->Set_BlockRender(false);
			}
			for (auto& pBlock_L : m_vecBlock_L)
			{
				pBlock_L->Set_BlockRender(false);
			}
			for (auto& pBlock_R : m_vecBlock_R)
			{
				pBlock_R->Set_BlockRender(false);
			}
			m_bOnefor = false;
		}

		map<E_OBJTAG, vector<CGameObject*>>* pFurniture = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);

		for (auto& pObs : (*pFurniture)[E_OBJTAG::OBJ_Obstacle])
		{
			CObstacle_Cafe* pObstacle = static_cast<CObstacle_Cafe*>(pObs);
			pObstacle->Set_SelRender(false);

			m_pSelFurniture = nullptr;
			m_pSelFurnitureW = nullptr;
		}
	}
}

//선택한 가구의 정보를 레이캐스트에 담아서 충돌여부를 판단 후 멤버변수에 담아준다.
void CScene_Cafe::PickingFurniture()
{
	if (m_bFurniturePick)
	{
		if (Management Key_Down(KEY_RBUTTON))
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_PickFur.wav", 1.f);

			map<E_OBJTAG, vector<CGameObject*>>* pFurniture = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);

			_int iIndex = 0;
			_float fDistance = 999999.f;
			_int   iResultIndex = -1;

			for (auto& pObs : (*pFurniture)[E_OBJTAG::OBJ_Obstacle])
			{
				HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
				SetCursor(hHandCursor);

				_float fDist = 0.f;
				m_bPickCol = false;

				if (static_cast<CSMesh*>(pObs->Get_Component(ComTag_SMesh)) == nullptr)
				{
					return;
				}

				TriRaycast(g_hWnd, static_cast<CSMesh*>(pObs->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pObs->Get_Component(ComTag_Transform)), &fDist, &m_bPickCol);

				if (m_bPickCol)
				{
					if (fDist < fDistance)
					{
						fDistance = fDist;
						iResultIndex = iIndex;
					}
				}

				++iIndex;
			}

			if (iResultIndex >= 0)
			{
				if (!CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_Active())
				{
					CUI_Manager::GetInstance()->Get_Panel_FixMode1Panel()->Set_Active(true);
				}

				CObstacle_Cafe* pObstacle = static_cast<CObstacle_Cafe*>((*pFurniture)[E_OBJTAG::OBJ_Obstacle][iResultIndex]);

				if (static_cast<CObstacle_Cafe*>(pObstacle)->Get_SelRender() == true)
				{
					return;
				}

				pObstacle->Set_SelRender(true);

				if (pObstacle->Get_Name().substr(0, 3) == L"[W]")
				{
					if (m_pSelFurnitureW != nullptr)
					{
						if (static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Get_SelRender() == true)
						{
							static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Set_SelRender(false);
						}
					}

					if (m_pSelFurniture != nullptr)
					{
						if (static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Get_SelRender() == true)
						{
							static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Set_SelRender(false);
						}
					}

					for (auto& pBlock : m_vecBlock)
					{
						pBlock->Set_BlockRender(false);
					}
					for (auto& pBlock_L : m_vecBlock_L)
					{
						pBlock_L->Set_BlockRender(true);
					}
					for (auto& pBlock_R : m_vecBlock_R)
					{
						pBlock_R->Set_BlockRender(true);
					}

					m_pSelFurnitureW = pObstacle;
					m_pSelFurniture = nullptr;
				}
				else
				{
					if (m_pSelFurnitureW != nullptr)
					{
						if (static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Get_SelRender() == true)
						{
							static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Set_SelRender(false);
						}
					}

					if (m_pSelFurniture != nullptr)
					{
						if (static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Get_SelRender() == true)
						{
							static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Set_SelRender(false);
						}
					}

					for (auto& pBlock : m_vecBlock)
					{
						pBlock->Set_BlockRender(true);
					}
					for (auto& pBlock_L : m_vecBlock_L)
					{
						pBlock_L->Set_BlockRender(false);
					}
					for (auto& pBlock_R : m_vecBlock_R)
					{
						pBlock_R->Set_BlockRender(false);
					}

					m_pSelFurniture = pObstacle;
					m_pSelFurnitureW = nullptr;
				}
			}
			else
			{
				if (!CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_Active())
				{
					CUI_Manager::GetInstance()->Get_Panel_FixMode1Panel()->Set_Active(false);
					CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
					m_bFurniturePick = false;
				}
			}
		}
	}
}

//위 선택한 객체를 멤버변수에 담았다면 해당 멤버변수를 토대로 레이캐스트 충돌 여부를 판단하여 충돌 시 Pos를 실시간으로 Set해준다.
void CScene_Cafe::MoveFurniture()
{
	//타일
	if (m_bFurniturePick && m_pSelFurniture != nullptr)
	{
		if (Management Key_Pressing(KEY_RBUTTON) && m_pSelFurniture != nullptr)
		{
			if (Management Key_Pressing(KEY_LBUTTON) && Management Key_Pressing(KEY_RBUTTON))
			{
				return;
			}

			map<E_OBJTAG, vector<CGameObject*>>* pMap = Management Get_LayerMapObject(E_LAYERTAG::Map);
			map<E_OBJTAG, vector<CGameObject*>>* pObs = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);

			_int iIndex = 0;
			_float fDistance = 999999.f;
			_int   iResultIndex = -1;

			for (auto& pTile : (*pMap)[E_OBJTAG::OBJ_BlockTile])
			{
				HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
				SetCursor(hHandCursor);

				CTransform* pFurnitureTrans = static_cast<CTransform*>(m_pSelFurniture->Get_Component(ComTag_Transform));

				_matrix mTile = static_cast<CTransform*>(pTile->Get_Component(ComTag_Transform))->Get_WorldMatrix();
				_vec3 vPos;
				m_bTileCol = false;

				Raycast(g_hWnd, &mTile, &vPos, static_cast<CSMesh*>(pTile->Get_Component(ComTag_SMesh))->Get_Mesh(), &m_bTileCol);

				if (m_bTileCol)
				{
					if (m_pSelFurniture->Get_Name().substr(0, 3) == L"[C]")
					{
						pFurnitureTrans->Set_Pos(pTile->Get_Info(E_INFO::INFO_POS) + _vec3(0.02f, 0.02f, 0.02f));
					}
				}
			}

			for (auto& pFurniture : (*pObs)[E_OBJTAG::OBJ_Obstacle])
			{
				_float fDist = 0.f;
				_bool bCol = false;

				TriRaycast(g_hWnd, static_cast<CSMesh*>(pFurniture->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pFurniture->Get_Component(ComTag_Transform)), &fDist, &bCol);

				if (bCol)
				{
					if (fDist < fDistance)
					{
						fDistance = fDist;
						iResultIndex = iIndex;
					}
				}

				++iIndex;
			}

			if (iResultIndex >= 0)
			{
				return;
			}
			else
			{
				for (auto& pTile : (*pMap)[E_OBJTAG::OBJ_BlockTile])
				{
					HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
					SetCursor(hHandCursor);

					CTransform* pFurnitureTrans = static_cast<CTransform*>(m_pSelFurniture->Get_Component(ComTag_Transform));

					_matrix mTile = static_cast<CTransform*>(pTile->Get_Component(ComTag_Transform))->Get_WorldMatrix();
					_vec3 vPos;
					m_bTileCol = false;

					Raycast(g_hWnd, &mTile, &vPos, static_cast<CSMesh*>(pTile->Get_Component(ComTag_SMesh))->Get_Mesh(), &m_bTileCol);

					if (m_bTileCol)
					{
						pFurnitureTrans->Set_Pos(pTile->Get_Info(E_INFO::INFO_POS));
					}
				}
			}
		}
	}

	//벽면
	if (m_bFurniturePick && m_pSelFurnitureW != nullptr)
	{
		if (Management Key_Pressing(KEY_RBUTTON) && m_pSelFurnitureW != nullptr)
		{
			if (Management Key_Pressing(KEY_LBUTTON) && Management Key_Pressing(KEY_RBUTTON))
			{
				return;
			}

			map<E_OBJTAG, vector<CGameObject*>>* pObs = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);
			map<E_OBJTAG, vector<CGameObject*>>* pTile_L = Management Get_LayerMapObject(E_LAYERTAG::Map);
			map<E_OBJTAG, vector<CGameObject*>>* pTile_R = Management Get_LayerMapObject(E_LAYERTAG::Map);

			_int iIndex = 0;
			_float fDistance = 999999.f;
			_int   iResultIndex = -1;

			for (auto& pFurniture : (*pObs)[E_OBJTAG::OBJ_Obstacle])
			{
				_float fDist = 0.f;
				_bool bCol = false;

				TriRaycast(g_hWnd, static_cast<CSMesh*>(pFurniture->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pFurniture->Get_Component(ComTag_Transform)), &fDist, &bCol);

				if (bCol)
				{
					if (fDist < fDistance)
					{
						fDistance = fDist;
						iResultIndex = iIndex;
					}
				}

				++iIndex;
			}

			if (iResultIndex >= 0)
			{
				return;
			}
			else
			{
				for (auto& pBlock_L : (*pTile_L)[E_OBJTAG::OBJ_BlockTile_L])
				{
					HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
					SetCursor(hHandCursor);

					CTransform* pFurnitureTrans = static_cast<CTransform*>(m_pSelFurnitureW->Get_Component(ComTag_Transform));
					_float fDist = 0.f;
					m_bBlockLCol = false;

					TriRaycast(g_hWnd, static_cast<CSMesh*>(pBlock_L->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pBlock_L->Get_Component(ComTag_Transform)), &fDist, &m_bBlockLCol);

					if (m_bBlockLCol)
					{
						if (pBlock_L->Get_Name() == L"블럭타일_B.X")
						{
							return;
						}

						m_fRot = -90.f;
						m_pSelFurnitureW->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);

						pFurnitureTrans->Set_Pos(pBlock_L->Get_Info(E_INFO::INFO_POS));
					}
				}

				for (auto& pBlock_R : (*pTile_R)[E_OBJTAG::OBJ_BlockTile_R])
				{
					HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
					SetCursor(hHandCursor);

					CTransform* pFurnitureTrans = static_cast<CTransform*>(m_pSelFurnitureW->Get_Component(ComTag_Transform));
					_float fDist = 0.f;
					m_bBlockRCol = false;

					_vec3 vPickFur = TriRaycast(g_hWnd, static_cast<CSMesh*>(pBlock_R->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pBlock_R->Get_Component(ComTag_Transform)), &fDist, &m_bBlockRCol);

					if (m_bBlockRCol)
					{
						if (pBlock_R->Get_Name() == L"블럭타일_B.X")
						{
							return;
						}

						m_fRot = 0.f;
						m_pSelFurnitureW->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);

						pFurnitureTrans->Set_Pos(pBlock_R->Get_Info(E_INFO::INFO_POS));
					}
				}
			}
		}
	}
}

//선택한 가구를 방향키 단축키를 활용하여 회전할 수 있다.
void CScene_Cafe::RotateFurniture()
{
	if (m_bRotON && m_pSelFurniture != nullptr)
	{
		m_pSelFurniture->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);
		m_fRot += 90.f;

		m_bRotON = false;
	}

	if (Management Key_Down(KEY_RIGHT) && m_pSelFurniture != nullptr)
	{
		m_fRot -= 45.f;

		m_pSelFurniture->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);
	}

	if (Management Key_Down(KEY_LEFT) && m_pSelFurniture != nullptr)
	{
		m_fRot += 45.f;

		m_pSelFurniture->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);
	}

	if (Management Key_Down(KEY_UP) && m_pSelFurniture != nullptr)
	{
		m_fRot -= 45.f;

		m_pSelFurniture->Set_Rotation(E_ROTATION::ROT_Z, m_fRot);
	}

	if (Management Key_Down(KEY_DOWN) && m_pSelFurniture != nullptr)
	{
		m_fRot += 45.f;

		m_pSelFurniture->Set_Rotation(E_ROTATION::ROT_Z, m_fRot);
	}
}

//선택한 가구를 Delete 단축키로 지울 수 있다.(추후 인벤토리 수납 기능으로 전환 예정)
void CScene_Cafe::DeleteFurniture()
{
	if (m_bStorageON && m_pSelFurniture != nullptr)
	{
		m_pSelFurniture->Set_Dead(TRUE);
		m_pSelFurniture = nullptr;
		m_bStorageON = false;
		CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
	}

	if (m_bStorageON && m_pSelFurnitureW != nullptr)
	{
		m_pSelFurnitureW->Set_Dead(TRUE);
		m_pSelFurnitureW = nullptr;
		m_bStorageON = false;
		CUI_Manager::GetInstance()->Get_Panel_ParentLeftPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(4)->OnClick();
	}

	if (Management Key_Down(KEY_Delete) && m_pSelFurniture != nullptr)
	{
		m_pSelFurniture->Set_Dead(TRUE);
		m_pSelFurniture = nullptr;
	}
	else if (Management Key_Down(KEY_Delete) && m_pSelFurnitureW != nullptr)
	{
		m_pSelFurnitureW->Set_Dead(TRUE);
		m_pSelFurnitureW = nullptr;
	}

	if (m_bAllStorageON)
	{
		Management Set_Dead_AllObject(E_LAYERTAG::Obstacle);
		m_bAllStorageON = false;
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(0));
	}
}

void CScene_Cafe::PickChar()
{
	if (Management Key_Down(KEY_Tab))
	{
		m_bCharPick = !m_bCharPick;
	}

	if (m_bCharPick)
	{
		if (Management Key_Down(KEY_LBUTTON))
		{
			vector<CGameObject*>* vecChar = Management Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);

			for (auto& pChar : *vecChar)
			{
				CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pChar->Get_Component(ComTag_Collider));
				CGameObject* pColChar = nullptr;

				if (pSphereCol->PickingCollider(g_hWnd, &pColChar))
				{
					CInvite_Character* pInvChar = static_cast<CInvite_Character*>(pChar);
					pInvChar->Set_TimeSet(true);
					m_pSelChar = pInvChar;

					if (m_bGift1)
					{
						CSkill_Manager::GetInstance()->SetSkill(L"Cafe_Interset_Char.skill", E_LAYERTAG::Effect, m_pSelChar->Get_Position(), VEC_ZERO, VEC_ONE);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart.wav", 0.5f);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart_Effect.wav", 0.5f);

						m_bGift1 = false;
					}
					if (m_bGift2)
					{
						CSkill_Manager::GetInstance()->SetSkill(L"Cafe_Interset_Char2.skill", E_LAYERTAG::Effect, m_pSelChar->Get_Position(), VEC_ZERO, VEC_ONE);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart.wav", 0.5f);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart_Effect.wav", 0.5f);

						m_bGift2 = false;
					}
					if (m_bGift3)
					{
						CSkill_Manager::GetInstance()->SetSkill(L"Cafe_Interset_Char3.skill", E_LAYERTAG::Effect, m_pSelChar->Get_Position(), VEC_ZERO, VEC_ONE);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart.wav", 0.5f);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart_Effect.wav", 0.5f);

						m_bGift3 = false;
					}
					if (m_bGift4)
					{
						CSkill_Manager::GetInstance()->SetSkill(L"Cafe_Interset_Char4.skill", E_LAYERTAG::Effect, m_pSelChar->Get_Position(), VEC_ZERO, VEC_ONE);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart.wav", 0.5f);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart_Effect.wav", 0.5f);

						m_bGift4 = false;
					}
					if (m_bGift5)
					{
						CSkill_Manager::GetInstance()->SetSkill(L"Cafe_Interset_Char5.skill", E_LAYERTAG::Effect, m_pSelChar->Get_Position(), VEC_ZERO, VEC_ONE);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart.wav", 0.5f);
						CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Cafe_Heart_Effect.wav", 0.5f);

						m_bGift5 = false;
					}

					CCamera_Cafe* pCamera = static_cast<CCamera_Cafe*>(m_pCamera);
					m_pSelChar->Set_Look(pCamera->Get_CamEye() - _vec3(0.f, 2.3f, 0.f));

					m_pSelChar->Set_Action(E_ACTION::Cafe_Reaction);

					CharSound();
				}
			}
		}
	}
}

void CScene_Cafe::MoveChar()
{
	if (!m_pSelChar)
	{
		return;
	}

	if (m_bCharPick)
	{
		if (Management Key_Pressing(KEY_RBUTTON))
		{
			vector<CGameObject*>* vecChar = Management Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);

			for (auto& pChar : *vecChar)
			{
				CSphereCollider* pSphereCol = static_cast<CSphereCollider*>(pChar->Get_Component(ComTag_Collider));
				CGameObject* pColChar = nullptr;

				if (pSphereCol->PickingCollider(g_hWnd, &pColChar))
				{
					CInvite_Character* pInvChar = static_cast<CInvite_Character*>(pChar);
					m_pSelChar = pInvChar;
				}
			}

			_vec3 vPos;
			if (RayChar(&vPos))
			{
				m_pSelChar->Set_Pos(vPos + _vec3(0.f, 0.3f, 0.f));

				CCamera_Cafe* pCamera = static_cast<CCamera_Cafe*>(m_pCamera);

				m_pSelChar->Set_Look(pCamera->Get_CamEye() - _vec3(0.f, 2.3f, 0.f));

				m_pSelChar->Set_ActionByEnum(E_ACTION::Formation_Pickup);
			}
		}

		if (Management Key_Up(KEY_RBUTTON))
		{
			m_pSelChar->Set_Pos(m_pSelChar->Get_Position() - _vec3(0.f, 0.3f, 0.f));
			m_pSelChar->Set_ActionByEnum(E_ACTION::Cafe_Idle);
		}
	}
}

CUI_Object * CScene_Cafe::Load_UI_Prefab(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
		return nullptr;

	wstring wstrUITag;
	CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

	_int iObjTag = 0;
	E_OBJTAG eTag = E_OBJTAG::OBJ_END;
	CUtility::Data_ReadInt(hFile, iObjTag, dwByte);
	eTag = (E_OBJTAG)iObjTag;

	CUI_Object* pUi = nullptr;
	if (L"CUI_Object" == wstrUITag)
	{
		pUi = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Slider_Vert" == wstrUITag)
	{
		pUi = CUI_Slider_Vert::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI_SLIDER);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Scroll_View" == wstrUITag)
	{
		pUi = CUI_Scroll_View::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
		pUi->Load(hFile, dwByte);
	}
	else if (L"CUI_Damage" == wstrUITag)
	{
		pUi = CUI_Damage::Create(CGraphicDev::GetInstance()->GetDevice(), &g_hWnd, E_OBJTAG::OBJ_UI_NUMBER, 9999999.f, E_NumberFontType::Normal, 0);
		pUi->Load(hFile, dwByte);
	}

	assert(pUi);

	CloseHandle(hFile);

	return pUi;
}

void CScene_Cafe::UI_SelFurInfo()
{
	if (CUI_Manager::GetInstance()->Get_Panel_FixMode1Panel()->Get_Active())
	{
		if (m_pSelFurnitureW != nullptr)
		{
			wstring wstrFurWName = m_pSelFurnitureW->Get_Name();
			_int iCheck = (_int)wstrFurWName.rfind(L".");
			wstrFurWName = wstrFurWName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurWName.find(L"[W]");
			wstrFurWName = wstrFurWName.substr(0, iCheck2) + wstrFurWName.substr(iCheck2 + 3);

			CUI_Manager::GetInstance()->Get_Panel_FixMode1Panel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFurWName);
		}

		if (m_pSelFurniture != nullptr)
		{
			wstring wstrFurName = m_pSelFurniture->Get_Name();
			_int iCheck = (_int)wstrFurName.rfind(L".");
			wstrFurName = wstrFurName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurName.find(L"[C]");

			if (iCheck2 >= 0)
			{
				wstrFurName = wstrFurName.substr(0, iCheck2) + wstrFurName.substr(iCheck2 + 3);
			}

			CUI_Manager::GetInstance()->Get_Panel_FixMode1Panel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFurName);
		}
	}

	if (CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_Active())
	{
		if (m_pSelFurnitureW != nullptr)
		{
			wstring wstrFurWName = m_pSelFurnitureW->Get_Name();
			_int iCheck = (_int)wstrFurWName.rfind(L".");
			wstrFurWName = wstrFurWName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurWName.find(L"[W]");
			wstrFurWName = wstrFurWName.substr(0, iCheck2) + wstrFurWName.substr(iCheck2 + 3);

			CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFurWName);
		}

		if (m_pSelFurniture != nullptr)
		{
			wstring wstrFurName = m_pSelFurniture->Get_Name();
			_int iCheck = (_int)wstrFurName.rfind(L".");
			wstrFurName = wstrFurName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurName.find(L"[C]");

			if (iCheck2 >= 0)
			{
				wstrFurName = wstrFurName.substr(0, iCheck2) + wstrFurName.substr(iCheck2 + 3);
			}

			CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFurName);
		}
	}

	if (m_bSelectFurInfo)
	{
		if (m_pSelFurnitureW != nullptr)
		{
			wstring wstrFurWName = m_pSelFurnitureW->Get_Name();
			_int iCheck = (_int)wstrFurWName.rfind(L".");
			wstrFurWName = wstrFurWName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurWName.find(L"[W]");
			wstrFurWName = wstrFurWName.substr(0, iCheck2) + wstrFurWName.substr(iCheck2 + 3);

			CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(1)->Set_Ui_Text(wstrFurWName);

			for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
			{
				wstring wstrFurWName = m_pSelFurnitureW->Get_Name();

				if (CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName == wstrFurWName)
				{
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Furniture(i)->wstrTexturePath);
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Furniture(i)->iAmenity));

					wstring wstrFullText;
					wstrFullText = CGameManager::GetInstance()->Get_Furniture(i)->wstrDescription;
					wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
				}
			}
		}

		if (m_pSelFurniture != nullptr)
		{
			wstring wstrFurName = m_pSelFurniture->Get_Name();
			_int iCheck = (_int)wstrFurName.rfind(L".");
			wstrFurName = wstrFurName.substr(0, iCheck);

			_int iCheck2 = (_int)wstrFurName.find(L"[C]");

			if (iCheck2 >= 0)
			{
				wstrFurName = wstrFurName.substr(0, iCheck2) + wstrFurName.substr(iCheck2 + 3);
			}

			CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(1)->Set_Ui_Text(wstrFurName);

			for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
			{
				wstring wstrFurName = m_pSelFurniture->Get_Name();

				if (CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName == wstrFurName)
				{
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(0)->Set_Tex1_Tag(CGameManager::GetInstance()->Get_Furniture(i)->wstrTexturePath);
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(2)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Furniture(i)->iAmenity));

					wstring wstrFullText;
					wstrFullText = CGameManager::GetInstance()->Get_Furniture(i)->wstrDescription;
					wstrFullText = CUtility::Auto_Line(wstrFullText, L" ", 20);
					CUI_Manager::GetInstance()->Get_Panel_FurInfoPanel()->Get_ChildByIndex(3)->Set_Ui_Text(wstrFullText);
				}
			}
		}

		m_bSelectFurInfo = false;
	}
}

void CScene_Cafe::UI_CreateFurPick()
{
	if (CUI_Manager::GetInstance()->Get_Obstacle() != nullptr)
	{
		if (!CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Get_Active())
		{
			CUI_Manager::GetInstance()->Get_Panel_FixMode2Panel()->Set_Active(true);
		}

		if (CUI_Manager::GetInstance()->Get_Obstacle()->Get_SelRender() == true)
		{
			return;
		}

		CUI_Manager::GetInstance()->Get_Obstacle()->Set_SelRender(true);

		if (CUI_Manager::GetInstance()->Get_Obstacle()->Get_Name().substr(0, 3) == L"[W]")
		{
			if (m_pSelFurnitureW != nullptr)
			{
				if (static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Get_SelRender() == true)
				{
					static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Set_SelRender(false);
				}
			}

			if (m_pSelFurniture != nullptr)
			{
				if (static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Get_SelRender() == true)
				{
					static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Set_SelRender(false);
				}
			}

			for (auto& pBlock : m_vecBlock)
			{
				pBlock->Set_BlockRender(false);
			}
			for (auto& pBlock_L : m_vecBlock_L)
			{
				pBlock_L->Set_BlockRender(true);
			}
			for (auto& pBlock_R : m_vecBlock_R)
			{
				pBlock_R->Set_BlockRender(true);
			}

			m_pSelFurnitureW = CUI_Manager::GetInstance()->Get_Obstacle();
			m_pSelFurniture = nullptr;
		}
		else
		{
			if (m_pSelFurnitureW != nullptr)
			{
				if (static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Get_SelRender() == true)
				{
					static_cast<CObstacle_Cafe*>(m_pSelFurnitureW)->Set_SelRender(false);
				}
			}

			if (m_pSelFurniture != nullptr)
			{
				if (static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Get_SelRender() == true)
				{
					static_cast<CObstacle_Cafe*>(m_pSelFurniture)->Set_SelRender(false);
				}
			}

			for (auto& pBlock : m_vecBlock)
			{
				pBlock->Set_BlockRender(true);
			}
			for (auto& pBlock_L : m_vecBlock_L)
			{
				pBlock_L->Set_BlockRender(false);
			}
			for (auto& pBlock_R : m_vecBlock_R)
			{
				pBlock_R->Set_BlockRender(false);
			}

			m_pSelFurniture = CUI_Manager::GetInstance()->Get_Obstacle();
			m_pSelFurnitureW = nullptr;
		}
	}
}

void CScene_Cafe::UI_StartAmenityInfo()
{
	if (m_bStartAmenity)
	{
		m_iAmenity = 0;

		m_vecObstacle_Cafe = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

		for (auto& pObs : *m_vecObstacle_Cafe)
		{
			for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
			{
				if (CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName == pObs->Get_Name())
				{
					m_iAmenity += CGameManager::GetInstance()->Get_Furniture(i)->iAmenity;
				}
			}
		}

		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iAmenity));

		m_bStartAmenity = false;
	}
}

void CScene_Cafe::UI_AmenityInfo()
{
	if (CUI_Manager::GetInstance()->Get_Panel_CafeInfoPanel()->Get_Active())
	{
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Set_ChildClickCheck(false);
	}
	else
	{
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Set_ChildClickCheck(true);
	}

	if (m_bOpenAmenity)
	{
		m_iAmenity = 0;

		m_vecObstacle_Cafe = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

		for (auto& pObs : *m_vecObstacle_Cafe)
		{
			for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
			{
				if (CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName == pObs->Get_Name())
				{
					m_iAmenity += CGameManager::GetInstance()->Get_Furniture(i)->iAmenity;
				}
			}
		}

		CUI_Manager::GetInstance()->Get_Panel_CafeInfoPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iAmenity));
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iAmenity));

		m_bOpenAmenity = false;
	}
}

void CScene_Cafe::UI_FurnitureInfo()
{
	if (m_bOpenFurInfo)
	{
		m_vecObstacle_Cafe = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

		for (auto& pObs : *m_vecObstacle_Cafe)
		{
			for (_int i = 0; i < (_int)E_FurnitureTag::Furniture_End; ++i)
			{
				if (CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName == pObs->Get_Name())
				{
					m_pUi_Furniture = Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]카페_설치된 가구 정보.uidat");

					if (CUI_Manager::GetInstance()->Get_Panel_CafeInfoPanel()->Get_ChildByIndex(1)->Get_Active())
					{
						return;
					}

					wstring wstrFurName = CGameManager::GetInstance()->Get_Furniture(i)->wstrMeshName;
					_int iCheck = (_int)wstrFurName.rfind(L".");
					wstrFurName = wstrFurName.substr(0, iCheck);

					_int iCheck2 = (_int)wstrFurName.find(L"[W]");
					if (iCheck2 >= 0)
					{
						wstrFurName = wstrFurName.substr(0, iCheck2) + wstrFurName.substr(iCheck2 + 3);
					}

					_int iCheck3 = (_int)wstrFurName.find(L"[C]");
					if (iCheck3 >= 0)
					{
						wstrFurName = wstrFurName.substr(0, iCheck3) + wstrFurName.substr(iCheck3 + 3);
					}

					m_pUi_Furniture->Set_Ui_Text(wstrFurName);
					m_pUi_Furniture->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(CGameManager::GetInstance()->Get_Furniture(i)->iAmenity));
					CUI_Manager::GetInstance()->Get_Panel_CafeInfoPanel()->Get_ChildByIndex(1)->Get_ChildByIndex(0)->Set_Child(m_pUi_Furniture);
					m_vecUi_Furniture.emplace_back(m_pUi_Furniture);
					m_pUi_Furniture->Set_Active(true);
				}
			}
		}

		m_bOpenFurInfo = false;
	}
}

void CScene_Cafe::UI_DeleteFurnitureInfo()
{
	if (m_bExitFurInfo)
	{
		for (auto& pPrefab : m_vecUi_Furniture)
		{
			pPrefab->Set_Dead(true);
		}
		m_vecUi_Furniture.clear();
		m_vecUi_Furniture.shrink_to_fit();

		m_bExitFurInfo = false;
	}
}

void CScene_Cafe::UI_UpdateProfit(const _float& fTimeDelta)
{
	if (m_bRecieve)
	{
		//상단바 UI에 있는 AP / Credit 정보에 넘겨주기

		m_iProfitIndex = 0;
		m_iCreditIndex = 0;
		m_iAPIndex = 0;

		m_bRecieve = false;
	}

	m_fProfitTime += fTimeDelta;

	if (m_fProfitTime >= 0.05f)
	{
		if (m_iProfitIndex >= 100)
		{
			CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(9)->Set_Active(false);
			CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(10)->Set_Active(true);
			m_iProfitIndex = 99;
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(9)->Set_Active(true);
			CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(10)->Set_Active(false);
		}

		m_iProfitIndex += 1;
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(6)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iProfitIndex) + L"%");

		if (m_iCreditIndex >= 1000)
		{
			m_iCreditIndex = 990;
		}

		m_iCreditIndex += 10;
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(8)->Get_ChildByIndex(0)->Set_Ui_Text(to_wstring(m_iCreditIndex));

		if (m_iAPIndex >= 100)
		{
			m_iAPIndex = 99;
		}

		m_iAPIndex += 1;
		CUI_Manager::GetInstance()->Get_Panel_ParentRightPanel()->Get_ChildByIndex(8)->Get_ChildByIndex(1)->Set_Ui_Text(to_wstring(m_iAPIndex));

		m_fProfitTime = 0.f;
	}
}

void CScene_Cafe::UI_Preset(const _float& fTimeDelta)
{
	if (m_bPresetInit1)
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(L"Preset1");

		m_bPresetLock1 = true;
		m_bPresetInit1 = false;
	}
	if (m_bPresetInit2)
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(L"Preset2");

		m_bPresetLock2 = true;
		m_bPresetInit2 = false;
	}
	if (m_bPresetInit3)
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(L"Preset3");

		m_bPresetLock3 = true;
		m_bPresetInit3 = false;
	}
	if (m_bPresetInit4)
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Ui_Text(L"Preset4");

		m_bPresetLock4 = true;
		m_bPresetInit4 = false;
	}
	if (m_bPresetInit5)
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Ui_Text(L"Preset5");

		m_bPresetLock5 = true;
		m_bPresetInit5 = false;
	}

	if (m_bPresetSave1)
	{
		UI_PresetSave(1);

		m_bPresetLock1 = false;
		m_bPresetSave1 = false;
	}
	if (m_bPresetSave2)
	{
		UI_PresetSave(2);

		m_bPresetLock2 = false;
		m_bPresetSave2 = false;
	}
	if (m_bPresetSave3)
	{
		UI_PresetSave(3);

		m_bPresetLock3 = false;
		m_bPresetSave3 = false;
	}
	if (m_bPresetSave4)
	{
		UI_PresetSave(4);

		m_bPresetLock4 = false;
		m_bPresetSave4 = false;
	}
	if (m_bPresetSave5)
	{
		UI_PresetSave(5);

		m_bPresetLock5 = false;
		m_bPresetSave5 = false;
	}

	if (m_bPresetApply1)
	{
		if (!m_bPresetLock1)
		{
			UI_PresetApply(1, fTimeDelta);
		}

		vector<CGameObject*>* pObj = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);
		if (pObj != nullptr)
		{
			for (auto& pChar : *pObj)
			{
				static_cast<CInvite_Character*>(pChar)->Set_ChagnePreset(true);
			}
		}

		m_bPresetApply1 = false;
	}
	if (m_bPresetApply2)
	{
		if (!m_bPresetLock2)
		{
			UI_PresetApply(2, fTimeDelta);
		}

		vector<CGameObject*>* pObj = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);
		if (pObj != nullptr)
		{
			for (auto& pChar : *pObj)
			{
				static_cast<CInvite_Character*>(pChar)->Set_ChagnePreset(true);
			}
		}

		m_bPresetApply2 = false;
	}
	if (m_bPresetApply3)
	{
		if (!m_bPresetLock3)
		{
			UI_PresetApply(3, fTimeDelta);
		}

		vector<CGameObject*>* pObj = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);
		if (pObj != nullptr)
		{
			for (auto& pChar : *pObj)
			{
				static_cast<CInvite_Character*>(pChar)->Set_ChagnePreset(true);
			}
		}

		m_bPresetApply3 = false;
	}
	if (m_bPresetApply4)
	{
		if (!m_bPresetLock4)
		{
			UI_PresetApply(4, fTimeDelta);
		}

		vector<CGameObject*>* pObj = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);
		if (pObj != nullptr)
		{
			for (auto& pChar : *pObj)
			{
				static_cast<CInvite_Character*>(pChar)->Set_ChagnePreset(true);
			}
		}

		m_bPresetApply4 = false;
	}
	if (m_bPresetApply5)
	{
		if (!m_bPresetLock5)
		{
			UI_PresetApply(5, fTimeDelta);
		}

		vector<CGameObject*>* pObj = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar);
		if (pObj != nullptr)
		{
			for (auto& pChar : *pObj)
			{
				static_cast<CInvite_Character*>(pChar)->Set_ChagnePreset(true);
			}
		}

		m_bPresetApply5 = false;
	}
}

void CScene_Cafe::UI_PresetSave(_int iIndex)
{
	TCHAR szFilePath1[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath1);
	PathRemoveFileSpec(szFilePath1);
	wstring wstrFullPath1 = szFilePath1;
	_int iCheck1 = (_int)wstrFullPath1.rfind(L"Team118A_Uint");
	wstring wstrLocalPath1 = wstrFullPath1.substr(0, iCheck1);

	WCHAR wszPathBuff[256] = L"";
	swprintf_s(wszPathBuff, L"Team118A_Uint\\Reference\\Data\\Data_Map Save, Load\\Map%d.Preset", iIndex);
	wstrLocalPath1 += wszPathBuff;

	wstring wstrFilePath1 = wstrLocalPath1;
	HANDLE hFile1 = CreateFile(wstrFilePath1.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte1 = 0;

	vector<CGameObject*>* vecMap1 = Management Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
	vector<CGameObject*>* vecTile1 = Management Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile);

	if (vecMap1 != nullptr)
	{
		int iSaveMapSize = (int)vecMap1->size();
		CUtility::Data_WriteInt(hFile1, iSaveMapSize, dwByte1);

		for (int i = 0; i < iSaveMapSize; ++i)
		{
			(*vecMap1)[i]->Save(hFile1, dwByte1);
		}
	}

	if (vecTile1 != nullptr)
	{
		int iSaveTileSize = (int)vecTile1->size();
		CUtility::Data_WriteInt(hFile1, iSaveTileSize, dwByte1);

		for (int i = 0; i < iSaveTileSize; ++i)
		{
			(*vecTile1)[i]->Save(hFile1, dwByte1);
		}
	}

	CloseHandle(hFile1);

	TCHAR szFilePath2[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath2);
	PathRemoveFileSpec(szFilePath2);
	wstring wstrFullPath2 = szFilePath2;
	_int iCheck2 = (_int)wstrFullPath2.rfind(L"Team118A_Uint");
	wstring wstrLocalPath2 = wstrFullPath2.substr(0, iCheck2);

	swprintf_s(wszPathBuff, L"Team118A_Uint\\Reference\\Data\\Data_Obstacle Save, Load\\Furniture%d.Preset", iIndex);
	wstrLocalPath2 += wszPathBuff;

	wstring wstrFilePath2 = wstrLocalPath2;
	HANDLE hFile2 = CreateFile(wstrFilePath2.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte2 = 0;

	m_vecObstacle_Cafe = Management Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

	if (m_vecObstacle_Cafe != nullptr)
	{
		int iSaveObstacle_CafeSize = (int)m_vecObstacle_Cafe->size();
		WriteFile(hFile2, &iSaveObstacle_CafeSize, sizeof(int), &dwByte2, nullptr);

		for (int i = 0; i < iSaveObstacle_CafeSize; ++i)
		{
			(*m_vecObstacle_Cafe)[i]->Save(hFile2, dwByte2);
		}
	}

	CloseHandle(hFile2);
}

void CScene_Cafe::UI_PresetApply(_int iIndex, const _float& fTimeDelta)
{
	m_bPresetFog = true;

	TCHAR szFilePath1[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath1);
	PathRemoveFileSpec(szFilePath1);
	wstring wstrFullPath1 = szFilePath1;
	_int iCheck1 = (_int)wstrFullPath1.rfind(L"Team118A_Uint");
	wstring wstrLocalPath1 = wstrFullPath1.substr(0, iCheck1);

	WCHAR wszPathBuff[256] = L"";
	swprintf_s(wszPathBuff, L"Team118A_Uint\\Reference\\Data\\Data_Map Save, Load\\Map%d.Preset", iIndex);
	wstrLocalPath1 += wszPathBuff;

	wstring wstrFilePath1 = wstrLocalPath1;
	HANDLE hFile1 = CreateFile(wstrFilePath1.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte1 = 0;

	if (INVALID_HANDLE_VALUE == hFile1)
		return;

	map<E_OBJTAG, vector<CGameObject*>>* pLayerMap = Management Get_LayerMapObject(E_LAYERTAG::Map);
	for (auto& tPair : *pLayerMap)
	{
		if (((tPair.first == E_OBJTAG::OBJ_BlockTile)  || 
			(tPair.first == E_OBJTAG::OBJ_BlockTile_L) || 
			(tPair.first == E_OBJTAG::OBJ_BlockTile_R)))
		{
			continue;
		}

		for (auto& pObj : tPair.second)
		{
			pObj->Set_Dead(TRUE);
		}
	}

	int iLoadMapSize = 0;
	CUtility::Data_ReadInt(hFile1, iLoadMapSize, dwByte1);

	for (int i = 0; i < iLoadMapSize; ++i)
	{
		_int iObjTag = 0;
		wstring wstrName;
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile1, wstrName, dwByte1);
		CUtility::Data_ReadWString(hFile1, wstrPath, dwByte1);
		CUtility::Data_ReadInt(hFile1, iObjTag, dwByte1);

		m_pMap = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, wstrPath, wstrName);
		m_pMap->Load(hFile1, dwByte1);
		Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, m_pMap);
	}

	int iLoadTileSize = 0;
	CUtility::Data_ReadInt(hFile1, iLoadTileSize, dwByte1);

	for (int i = 0; i < iLoadTileSize; ++i)
	{
		_int iObjTag = 0;
		wstring wstrName;
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile1, wstrName, dwByte1);
		CUtility::Data_ReadWString(hFile1, wstrPath, dwByte1);
		CUtility::Data_ReadInt(hFile1, iObjTag, dwByte1);

		m_pTile = CStageMap::Create(m_pGraphicDev, E_OBJTAG::OBJ_CafeTile, wstrPath, wstrName);
		m_pTile->Load(hFile1, dwByte1);
		Management Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_CafeTile, m_pTile);
	}

	CloseHandle(hFile1);

	TCHAR szFilePath2[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath2);
	PathRemoveFileSpec(szFilePath2);
	wstring wstrFullPath2 = szFilePath2;
	_int iCheck2 = (_int)wstrFullPath2.rfind(L"Team118A_Uint");
	wstring wstrLocalPath2 = wstrFullPath2.substr(0, iCheck2);

	swprintf_s(wszPathBuff, L"Team118A_Uint\\Reference\\Data\\Data_Obstacle Save, Load\\Furniture%d.Preset", iIndex);
	wstrLocalPath2 += wszPathBuff;

	wstring wstrFilePath2 = wstrLocalPath2;
	HANDLE hFile2 = CreateFile(wstrFilePath2.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte2 = 0;

	if (INVALID_HANDLE_VALUE == hFile2)
		return;

	Management Set_Dead_AllObject(E_LAYERTAG::Obstacle);

	int iLoadObstacle_CafeSize = 0;
	CUtility::Data_ReadInt(hFile2, iLoadObstacle_CafeSize, dwByte2);

	for (int i = 0; i < iLoadObstacle_CafeSize; ++i)
	{
		_int iObjTag = 0;
		wstring wstrName;
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile2, wstrName, dwByte2);
		CUtility::Data_ReadWString(hFile2, wstrPath, dwByte2);
		CUtility::Data_ReadInt(hFile2, iObjTag, dwByte2);

		m_pObstacle_Cafe = CObstacle_Cafe::Create(m_pGraphicDev, E_OBJTAG::OBJ_Obstacle, wstrPath, wstrName);
		m_pObstacle_Cafe->Load(hFile2, dwByte2);
		Management Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, m_pObstacle_Cafe);
	}

	CloseHandle(hFile2);
}

void CScene_Cafe::UI_PresetName()
{
	if (CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_Active())
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ChildClickCheck(false);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_ChildClickCheck(false);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_ChildClickCheck(false);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_ChildClickCheck(false);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_ChildClickCheck(false);
	}
	else
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_ChildClickCheck(true);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_ChildClickCheck(true);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_ChildClickCheck(true);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_ChildClickCheck(true);
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_ChildClickCheck(true);
	}

	if (m_bPresetName)
	{
		UI_InputKey();

		if (m_bPresetNameEnd)
		{
			if (m_bPresetName1)
			{
				CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Ui_Text(CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text());
				m_bPresetName1 = false;
			}

			if (m_bPresetName2)
			{
				CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Set_Ui_Text(CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text());
				m_bPresetName2 = false;
			}

			if (m_bPresetName3)
			{
				CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(2)->Set_Ui_Text(CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text());
				m_bPresetName3 = false;
			}

			if (m_bPresetName4)
			{
				CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(3)->Set_Ui_Text(CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text());
				m_bPresetName4 = false;
			}

			if (m_bPresetName5)
			{
				CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(0)->Get_ChildByIndex(4)->Set_Ui_Text(CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text());
				m_bPresetName5 = false;
			}

			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(L"");

			m_bPresetName = false;
			m_bPresetNameEnd = false;
		}
	}
}

//최초 한번 수행. 피킹용 Whitetile을 사전 생성한다.
HRESULT CScene_Cafe::Ready_Prototype()
{
	if (bUniquePrototype)
	{
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"블럭타일_B.X", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Map/x[Cafe]x_Default/", L"블럭타일_B.X")), E_FAIL);
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"블럭타일_L.X", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Map/x[Cafe]x_Default/", L"블럭타일_L.X")), E_FAIL);
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"블럭타일_R.X", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Map/x[Cafe]x_Default/", L"블럭타일_R.X")), E_FAIL);

		bUniquePrototype = FALSE;
	}
	return S_OK;
}

//최초 한번 수행. 조명을 설정한다.
HRESULT CScene_Cafe::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// 빛 정보
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	// 빛 위치	
	tLightInfo.Position = _vec3(0.f, 20.f, -1.f);

	// 빛 방향방향
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);

	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0) < 0) return E_FAIL;
	{
		D3DLIGHT9* pLightInfo = Management Get_Light();
	}

	return S_OK;
}

//최초 한번 수행. 피킹용 타일과 카메라를 생성한다. 카메라의 경우, 원활한 작업을 위해 자유시점 카메라로 전환할 수 있는 다이나믹 카메라도 생성한다.
HRESULT CScene_Cafe::Ready_SceneObject()
{
	CreateBlock();

	//// BackGround
	//pGameObject = CBackGround::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_BackGround, pGameObject);
	//
	//// 그림자 테스트용 수치값
	//pGameObject->Set_Pos(_vec3(0.f, -0.2f, 0.f));
	//pGameObject->Set_Rotation(90.f, 0.f, 0.f);
	//pGameObject->Set_Scale(100.f, 100.f, 100.f);

	//Dynamic Camera
	m_pCamera = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(_vec3(-2.4f, 2.3f, -3.35f),
			VEC_RIGHT - _vec3(0.5f, 0.f, 0.f),
			VEC_UP,
			D3DXToRadian(60.f),
			(_float)WINCX / (_float)WINCY,
			0.1f, 1000.f),
		E_OBJTAG::OBJ_DynamicCamera);

	assert(m_pCamera);
	Add_GameObject(E_LAYERTAG::GameLogic, m_pCamera->Get_ObjTag(), m_pCamera);

	//Cafe Camera
	m_pCamera = CCamera_Cafe::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(_vec3(-2.4f, 2.3f, -3.35f),
			VEC_RIGHT - _vec3(0.5f, 0.f, 0.f),
			VEC_UP,
			D3DXToRadian(60.f),
			(_float)WINCX / (_float)WINCY,
			0.1f, 1000.f),
		E_OBJTAG::OBJ_Camera_Cafe);

	assert(m_pCamera);
	Add_GameObject(E_LAYERTAG::GameLogic, m_pCamera->Get_ObjTag(), m_pCamera);


	//케릭터
	for (_int i = 0; i < 5; ++i)
	{
		_int iCharRandom = CWall512::GetInstance()->GetValue(0, 10000) % (_int)E_CHAR_NAME::Name_End;
		_int iRandomBlock = CWall512::GetInstance()->GetValue(0, 10000) % (_int)(&m_vecBlock)->size();

		wstring wstrChar = CCharacterMgr::GetInstance()->Translation_EnumToString(iCharRandom, E_CHAR_ENUMTYPE::CNUM_NAME);
		if (wstrChar == L"Airi" ||
			wstrChar == L"Akane" ||
			wstrChar == L"Aris" ||
			wstrChar == L"Aru" ||
			wstrChar == L"Azusa" ||
			wstrChar == L"Cherino" ||
			wstrChar == L"Chinatsu" ||
			wstrChar == L"Chise" ||
			wstrChar == L"Eimi" ||
			wstrChar == L"Hasumi" ||
			wstrChar == L"Hibiki" ||
			wstrChar == L"Hina" ||
			wstrChar == L"Iori" ||
			wstrChar == L"Izumi" ||
			wstrChar == L"Juri" ||
			wstrChar == L"Karin" ||
			wstrChar == L"Kayoko" ||
			wstrChar == L"Kirino" ||
			wstrChar == L"Maki" ||
			wstrChar == L"Mashiro" ||
			wstrChar == L"Momoi" ||
			wstrChar == L"Mutsuki" ||
			wstrChar == L"Neru" ||
			wstrChar == L"Nodoka" ||
			wstrChar == L"Pina" ||
			wstrChar == L"Serika" ||
			wstrChar == L"Shun" ||
			wstrChar == L"Sumire" ||
			wstrChar == L"Suzumi" ||
			wstrChar == L"Utaha" ||
			wstrChar == L"Wakamo" ||
			wstrChar == L"Yoshimi" ||
			wstrChar == L"Yuzu"
			)
		{
			if (m_pInvite_Char != nullptr)
			{
				if (wstrChar == m_pInvite_Char->Get_Name())
				{
					continue;
				}
			}

			m_pInvite_Char = CInvite_Character::Create(m_pGraphicDev, E_OBJTAG::OBJ_InviteChar, wstrChar);
			m_pInvite_Char->AddRef();
			m_pInvite_Char->Set_Scale(3.f, 3.f, 3.f);

			_vec3 vBlockPos = m_vecBlock[iRandomBlock]->Get_Info(E_INFO::INFO_POS);
			m_pInvite_Char->Set_Pos(vBlockPos);

			assert(m_pInvite_Char);
			Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_InviteChar, m_pInvite_Char);
			m_vecInvite_Char.emplace_back(m_pInvite_Char);
		}
	}

	return S_OK;
}

void CScene_Cafe::UI_InputKey()
{
	if (CKeyManager::GetInstance()->Key_Down(KEY_0))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"0");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_1))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"1");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_2))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"2");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_3))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"3");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_4))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"4");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_5))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"5");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_6))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"6");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_7))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"7");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_8))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"8");
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_9))
	{
		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"9");
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_Q))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"Q");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"q");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_W))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"W");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"w");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_E))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"E");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"e");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_R))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"R");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"r");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_T))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"T");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"t");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_Y))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"Y");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"y");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_U))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"U");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"u");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_I))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"I");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"i");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_O))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"O");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"o");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_P))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"P");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"p");
		}
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_A))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"A");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"a");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_S))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"S");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"s");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_D))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"D");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"d");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_F))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"F");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"f");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_G))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"G");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"g");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_H))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"H");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"h");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_J))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"J");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"j");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_K))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"K");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"k");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_L))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"L");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"l");
		}
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_Z))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"Z");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"z");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_X))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"X");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"x");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_C))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"C");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"c");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_V))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"V");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"v");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_B))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"B");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"b");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_N))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"N");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"n");
		}
	}
	if (CKeyManager::GetInstance()->Key_Down(KEY_M))
	{
		if (CKeyManager::GetInstance()->Key_Down(KEY_Shift))
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"M");
		}
		else
		{
			CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Add_Ui_Text(L"m");
		}
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_Back))
	{
		wstring wstrBuff = CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Get_Ui_Text();
		wstrBuff = wstrBuff.substr(0, wstrBuff.size() - 1);

		CUI_Manager::GetInstance()->Get_Panel_PresetPanel()->Get_ChildByIndex(2)->Get_ChildByIndex(1)->Set_Ui_Text(wstrBuff);
	}
}

void CScene_Cafe::CharSound()
{
	if (m_pSelChar->Get_Name() == L"Airi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Airi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Akane")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Akane_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Aris")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Aris_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Aru")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Aru_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Azusa")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Azusa_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Cherino")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Cherino_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Chinatsu")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Chinatsu_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Chise")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Chise_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Eimi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Eimi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Hasumi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Hasumi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Hibiki")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Hibiki_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Hina")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Hina_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Iori")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Iori_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Izumi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Izumi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Juri")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Juri_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Karin")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Karin_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Kayoko")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Kayoko_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Kirino")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Kirino_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Maki")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Maki_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Mashiro")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Mashiro_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Momoi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Momoi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Mutsuki")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Mutsuki_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Neru")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Neru_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Nodoka")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Nodoka_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Pina")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Pina_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Serika")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Serika_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Shun")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Shun_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Sumire")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Sumire_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Suzumi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Suzumi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Utaha")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Utaha_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Wakamo")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Shun_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Yoshimi")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Yoshimi_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
	if (m_pSelChar->Get_Name() == L"Yuzu")
	{
		CSound_Manager::GetInstance()->PlaySound_(L"Interaction_Yuzu_Cafe_Act_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, 0.2f);
	}
}