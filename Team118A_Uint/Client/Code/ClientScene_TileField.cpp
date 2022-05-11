#include "stdafx.h"
#include "../Include/stdafx.h"
#include "ClientScene_Lobby.h"
#include "ClientScene_Manager.h"
#include "SMesh.h"
#include "Mascot.h"

#include "ClientScene_TileField.h"
#include "TileField.h"

USING(Engine)

CClientScene_TileField::CClientScene_TileField(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CClientScene_TileField::~CClientScene_TileField()
{
}

CClientScene_TileField * CClientScene_TileField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClientScene_TileField*	pInstance = new CClientScene_TileField(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CClientScene_TileField::Free()
{
	Safe_Release(m_pTileField);
	CScene::Free();
}

HRESULT CClientScene_TileField::Ready_Scene()
{
	CScene::Ready_Scene();
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();

	m_pTileField = CGameManager::GetInstance()->Get_TileField();
	m_pTileField->AddRef();
	return S_OK;
}

_int CClientScene_TileField::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 

	

	m_pTileField->Spawn_FieldTile();


	return iExit;
}

_int CClientScene_TileField::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F3))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

	m_pTileField->Update_TileField(fTimeDelta);
	if (m_pTileField->Get_CombatSceneChange())
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}


	return	iExit;
}

_int CClientScene_TileField::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CClientScene_TileField::Render_Scene()
{
}

HRESULT CClientScene_TileField::Ready_Prototype()
{
	return S_OK;
}

HRESULT CClientScene_TileField::Ready_LightInfo()
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

HRESULT CClientScene_TileField::Ready_SceneObject()
{
	CGameObject* pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(_vec3(CAM_DEFAULT), VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);
	//// Dynamic Camera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
	//	T_CAMINFO(_vec3(CAM_DEFAULT), VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	pGameObject = CBackBoard::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround, L"../../Reference/Resource/Texture/UI/BaseImage/hexa_back_01.png");
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Environment, pGameObject->Get_ObjTag(), pGameObject);

	return S_OK;
}

