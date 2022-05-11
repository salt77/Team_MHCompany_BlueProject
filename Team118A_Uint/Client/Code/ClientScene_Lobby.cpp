#include "stdafx.h"
#include "../Include/stdafx.h"
#include "ClientScene_Lobby.h"

#include "ClientScene_Manager.h"
#include "ActionCam_Mgr.h"

#include "Mutsuki.h"
#include "Sukeban_SMG.h"
#include "Ray_Line.h"

USING(Engine)

CClientScene_Lobby::CClientScene_Lobby(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CClientScene_Lobby::~CClientScene_Lobby()
{

}

CClientScene_Lobby * CClientScene_Lobby::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClientScene_Lobby*	pInstance = new CClientScene_Lobby(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CClientScene_Lobby::Free()
{
	CScene::Free();
}

HRESULT CClientScene_Lobby::Ready_Scene()
{
	CScene::Ready_Scene();
	SetWindowText(g_hWnd, L"·Îºñ");
	Ready_Prototype();
	Ready_SceneObject();
	Ready_LightInfo();


	return S_OK;
}

_int CClientScene_Lobby::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta);
	CUI_Manager::GetInstance()->Goto_Lobby();

	CGameManager::GetInstance()->Set_BattleSpeed(E_BattleSpeed::_X1);
	return iExit;
}

_int CClientScene_Lobby::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F2))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Logo);
	}

	if (Management Key_Down(KEY_F4))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Cafe);
	}

	if (Management Key_Down(KEY_F6))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}

	if (Management Key_Down(KEY_F8))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_RunTest);
	}

	//Å¬¸¯ ÀÌÆåÆ®
	Set_ClickEffect();

	return	iExit;
}

_int CClientScene_Lobby::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CClientScene_Lobby::Render_Scene()
{
}

HRESULT CClientScene_Lobby::Ready_SceneObject()
{
	CGameObject*			pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd, T_CAMINFO(_vec3(0.f, 2.f, 0.f), _vec3(1.f, 1.f, 1.f), VEC_UP, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, pGameObject);

	//pGameObject = CCamera_TPS::Create(m_pGraphicDev, g_hWnd, T_CAMINFO(_vec3(0.f, 2.f, 0.f), _vec3(0.f, 0.f, 0.f), VEC_UP, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_Camera_TPS);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_TPS, pGameObject);

	return S_OK;
}

HRESULT CClientScene_Lobby::Ready_Prototype()
{
	return S_OK;
}

HRESULT CClientScene_Lobby::Ready_LightInfo()
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

	return S_OK;
}