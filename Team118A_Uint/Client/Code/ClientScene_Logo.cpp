#include "stdafx.h"
#include "../Include/stdafx.h"

#include "ClientScene_Manager.h"

#include "ClientScene_Logo.h"
#include "CLoading_Background.h"

USING(Engine)

_bool CClientScene_Logo::bUniquePrototype = TRUE;
CClientScene_Logo::CClientScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CClientScene_Logo::~CClientScene_Logo()
{
}

CClientScene_Logo * CClientScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClientScene_Logo*	pInstance = new CClientScene_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CClientScene_Logo::Free()
{
	CScene::Free();
}

HRESULT CClientScene_Logo::Ready_Scene()
{
	SetWindowText(g_hWnd, L"로고");
	CScene::Ready_Scene();
	Ready_Prototype();

	Ready_SceneObject();
	

	return S_OK;
}

_int CClientScene_Logo::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_시스템_로딩.ogg", 0.5f);
	return iExit;
}

_int CClientScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F3) && FALSE == bUniquePrototype)
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

	return	iExit;
}

_int CClientScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CClientScene_Logo::Render_Scene()
{
	// 디버그용
}

HRESULT CClientScene_Logo::Ready_SceneObject()
{
	CGameObject*			pGameObject = nullptr;

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd, T_CAMINFO(_vec3(0.f, 2.5f, 0.f), _vec3(1.f, 1.f, 1.f), VEC_UP, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_DynamicCamera, pGameObject);

	pGameObject = CLoading_Background::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround);
	Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_BackGround, pGameObject);


	return S_OK;
}

HRESULT CClientScene_Logo::Ready_Prototype()
{
	if (bUniquePrototype)
	{
		// 임시 테스트용 Texture
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_TexForTest", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Texture/Test/TexForTest.png", E_TEXTURE_TYPE::TEX_NORMAL)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_RectTex", CRectBuffer::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_SphereTex2", CSphereTex2::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_CrossRectTex", CCrossRectBuffer::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Texture/Logo/Logo.png", E_TEXTURE_TYPE::TEX_NORMAL, 1)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_CharacterMouse", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Character/Mouth.png")), E_FAIL);


		//// 시로 폭탄 메쉬 
		//FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_S", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb/", L"Shiro_Bomb.X")), E_FAIL);
		//FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_B", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb_OL/", L"Shiro_Bomb_OL.X")), E_FAIL);
		bUniquePrototype = FALSE;
	}

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
