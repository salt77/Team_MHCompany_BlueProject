#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_Lobby.h"
#include "Scene_Manager.h"
#include "BackGround.h"
#include "SMesh.h"
#include "CharacterMgr.h"
#include "StageMap.h"
#include "TileField.h"

USING(Engine)

CScene_Lobby::CScene_Lobby(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_Lobby::~CScene_Lobby()
{
}

CScene_Lobby * CScene_Lobby::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Lobby*	pInstance = new CScene_Lobby(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_Lobby::Free()
{
	m_pSelObstacle = nullptr;
	CScene::Free();
}

HRESULT CScene_Lobby::Ready_Scene()
{
	AfxGetApp()->GetMainWnd()->SetWindowTextW(L"로비");
	CScene::Ready_Scene();
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();
	
	return S_OK;
}

_int CScene_Lobby::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 
	CUI_Manager::GetInstance()->Goto_Lobby();
	return iExit;
}

_int CScene_Lobby::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F2))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
	}

	if (Management Key_Down(KEY_F4))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Cafe);
	}

	if (Management Key_Down(KEY_F6))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}

	if (Management Key_Down(KEY_F7))
	{
		PopupTool_PickObject_Clear();
		CGameManager::GetInstance()->Set_TileField(CTileField::Create(L"Test.tile"));
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_TileField);
	}

	//클릭 이펙트
	Set_ClickEffect();

	PickingObstacle();
	MoveObstacle();
	RotateFurniture();

	return	iExit;
}

_int CScene_Lobby::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CScene_Lobby::Render_Scene()
{
}

HRESULT CScene_Lobby::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_Lobby::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// 빛 정보
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	// 빛 위치	
	tLightInfo.Position = _vec3(0.f, 20.f, -4.f);
	
	// 빛 방향방향
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);

	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0)< 0) return E_FAIL;

	return S_OK;
}

HRESULT CScene_Lobby::Ready_SceneObject()
{
	CGameObject* pGameObject = nullptr;

	// Dynamic Camera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(CAM_DEFAULT, VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev, E_OBJTAG::OBJ_Test);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Environment, pGameObject->Get_ObjTag(), pGameObject); 
	pGameObject->Set_Pos(_vec3(0.f, -0.005f, 0.f));
	pGameObject->Set_Rotation(90.f, 0.f, 0.f);
	pGameObject->Set_Scale(4.f, 4.f, 4.f);


	return S_OK;
}

//엄폐물을 레이캐스트로 체크해서 충돌한 엄폐물을 m_pSelObstacle 객체에 담는다.(객체 선택)
void CScene_Lobby::PickingObstacle()
{
	if (m_bObstacleMove)
	{
		//Key_Down 전환 예정
		if (Management Key_Pressing(KEY_RBUTTON))
		{
			map<E_OBJTAG, vector<CGameObject*>>* pMap = Management Get_LayerMapObject(E_LAYERTAG::Map);
			map<E_OBJTAG, vector<CGameObject*>>* pFurniture = Management Get_LayerMapObject(E_LAYERTAG::Obstacle);

			for (auto& pObs : (*pFurniture)[E_OBJTAG::OBJ_Obstacle])
			{
				//_vec3 vFurniturePos = pObs->Get_Info(E_INFO::INFO_POS);

				HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
				SetCursor(hHandCursor);

				_vec3 vPos;
				_float fDist = 0.f;
				_bool bCol = false;
				_matrix mMap = static_cast<CTransform*>(pObs->Get_Component(ComTag_Transform))->Get_WorldMatrix();

				_vec3 vPickFur = TriRaycast(g_hWnd, static_cast<CSMesh*>(pObs->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pObs->Get_Component(ComTag_Transform)), &fDist, &bCol);

				//_vec3 vPickFur = Raycast(g_hWnd, &mMap, &vPos, static_cast<CSMesh*>(pObs->Get_Component(ComTag_SMesh))->Get_Mesh(), &bCol);
				//D3DXVec3TransformCoord(&vPos, &vPos, &mMap);

				//메쉬 충돌 했을때
				if (bCol)
				{
					m_pSelObstacle = pObs;
					return;
				}
			}
		}
	}
}

//위에서 선택한 객체를 레이캐스트하여 충돌된 엄폐물을 마우스 우클릭으로 실시간 Set_Pos한다.
void CScene_Lobby::MoveObstacle()
{
	if (m_bObstacleMove && m_pSelObstacle != nullptr)
	{
		if (Management Key_Pressing(KEY_RBUTTON))
		{
			map<E_OBJTAG, vector<CGameObject*>>* pMap = Management Get_LayerMapObject(E_LAYERTAG::Map);

			for (auto& pObj : (*pMap)[E_OBJTAG::OBJ_Map])
			{
				CTransform* pObstacleTrans = static_cast<CTransform*>(m_pSelObstacle->Get_Component(ComTag_Transform));

				HCURSOR hHandCursor = ::LoadCursor(NULL, IDC_HAND);
				SetCursor(hHandCursor);

				_vec3 vPos;
				_float fDist = 0.f;
				_bool bCol = false;
				_matrix mMap = static_cast<CTransform*>(pObj->Get_Component(ComTag_Transform))->Get_WorldMatrix();

				_vec3 vPickFur = TriRaycast(g_hWnd, static_cast<CSMesh*>(pObj->Get_Component(ComTag_SMesh)), static_cast<CTransform*>(pObj->Get_Component(ComTag_Transform)), &fDist, &bCol);

				//메쉬 충돌 했을때
				if (bCol)
				{
					pObstacleTrans->Set_Pos(vPickFur);
				}
			}
		}
	}
}

//pOutDistance로 메쉬 거리값을, bCol로 광선 충돌 여부를 판단하여 넘겨준다.
//광선 충돌 시 메쉬 행렬 정렬을 수행하여 마우스 포인터로부터 가장 가깝게 충돌한 UV값을 출력한다.
_vec3 CScene_Lobby::TriRaycast(HWND hWnd, CSMesh * pMapBufferCom, CTransform * pMapTransCom, _float * pOutDistance, _bool * bCol)
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

//선택한 가구를 방향키 단축키를 활용하여 회전할 수 있다.
void CScene_Lobby::RotateFurniture()
{
	if (Management Key_Down(KEY_RIGHT) && m_pSelObstacle != nullptr)
	{
		m_fRot -= 45.f;

		m_pSelObstacle->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);
	}

	if (Management Key_Down(KEY_LEFT) && m_pSelObstacle != nullptr)
	{
		m_fRot += 45.f;

		m_pSelObstacle->Set_Rotation(E_ROTATION::ROT_Y, m_fRot);
	}

	if (Management Key_Down(KEY_UP) && m_pSelObstacle != nullptr)
	{
		m_fRot -= 45.f;

		m_pSelObstacle->Set_Rotation(E_ROTATION::ROT_Z, m_fRot);
	}

	if (Management Key_Down(KEY_DOWN) && m_pSelObstacle != nullptr)
	{
		m_fRot += 45.f;

		m_pSelObstacle->Set_Rotation(E_ROTATION::ROT_Z, m_fRot);
	}
}