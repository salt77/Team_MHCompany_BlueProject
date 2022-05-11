#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Scene_Lobby.h"
#include "Scene_Manager.h"
#include "BackGround.h"
#include "SMesh.h"
#include "Mascot.h"

#include "Scene_TileField.h"
#include "TileField.h"

USING(Engine)

CScene_TileField::CScene_TileField(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_TileField::~CScene_TileField()
{
}

CScene_TileField * CScene_TileField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_TileField*	pInstance = new CScene_TileField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_TileField::Free()
{
	Safe_Release(m_pTileField);
	CScene::Free();
}

HRESULT CScene_TileField::Ready_Scene()
{
	AfxGetApp()->GetMainWnd()->SetWindowTextW(L"Å¸ÀÏ");
	CScene::Ready_Scene();
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();

	m_pTileField = CGameManager::GetInstance()->Get_TileField();
	m_pTileField->AddRef();
	return S_OK;
}

_int CScene_TileField::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 

	

	m_pTileField->Spawn_FieldTile();


	return iExit;
}

_int CScene_TileField::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);



	if (Management Key_Down(KEY_F2))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
	}

	if (Management Key_Down(KEY_F3))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

	if (Management Key_Down(KEY_F4))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Cafe);
	}

	m_pTileField->Update_TileField(fTimeDelta);
	if (m_pTileField->Get_CombatSceneChange())
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}
	return	iExit;
}

_int CScene_TileField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CScene_TileField::Render_Scene()
{
}

HRESULT CScene_TileField::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_TileField::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// ºû Á¤º¸
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

	// ºû À§Ä¡	
	tLightInfo.Position = _vec3(0.f, 20.f, -4.f);
	
	// ºû ¹æÇâ¹æÇâ
	_vec3 vLightDiection = _vec3(0.f, -1.f, 0.1f);

	tLightInfo.Direction = vLightDiection;

	if (Management Ready_Light(m_pGraphicDev, &tLightInfo, 0)< 0) return E_FAIL;
	D3DLIGHT9* pLightInfo = Management Get_Light();

	return S_OK;
}

HRESULT CScene_TileField::Ready_SceneObject()
{
	CGameObject* pGameObject = nullptr;

	// Dynamic Camera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(_vec3(CAM_DEFAULT), VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	pGameObject = CBackBoard::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround, L"../../Reference/Resource/Texture/UI/BaseImage/hexa_back_01.png");
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Environment, pGameObject->Get_ObjTag(), pGameObject);

	return S_OK;
}

