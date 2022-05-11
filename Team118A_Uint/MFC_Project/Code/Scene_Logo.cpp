#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Engine_Define.h"
#include "Scene_Manager.h"

#include "Scene_Logo.h"
#include "Scene_Combat.h"
#include "Boss_HpBar.h"
#include "XL_ParsingMgr.h"
#include "SkyDome.h"

#include "HeavyTank_Decagram_Cannon.h" 
#include "Char_Aru.h" 

USING(Engine)

_bool CScene_Logo::bUniquePrototype = TRUE;
CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CScene_Logo::~CScene_Logo()
{
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Logo*	pInstance = new CScene_Logo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CScene_Logo::Free()
{
	CScene::Free();
}

HRESULT CScene_Logo::Ready_Scene()
{
	//AfxGetApp()->GetMainWnd()->SetWindowTextW(L"로고");
	CScene::Ready_Scene();
	Ready_Prototype();
	Ready_LightInfo();
	Ready_SceneObject();
	
	CGameManager::GetInstance()->Ready_GameManager();

	CSound_Manager::GetInstance()->PlayBGM(L"BGM_시스템_로딩.ogg", 0.1f);
	return S_OK;
}

_int CScene_Logo::Start_Scene(const _float & fTimeDelta)
{
	_int iExit = CScene::Start_Scene(fTimeDelta); 


	return iExit;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::Update_Scene(fTimeDelta);

	if (Management Key_Down(KEY_F3) && FALSE == bUniquePrototype)
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Lobby);
	}

	if (Management Key_Down(KEY_F4) && FALSE == bUniquePrototype)
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Cafe);
	}

	if (Management Key_Down(KEY_F6))
	{
		PopupTool_PickObject_Clear();
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_Combat);
	}

	if (Management Key_Down(KEY_F8))
	{
		CScene_Manager::GetInstance()->Set_Loading(E_SCENETAG::Scene_RunTest);
	}

	return	iExit;
}

_int CScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int	iExit = CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CScene_Logo::Render_Scene()
{
	// 디버그용
}


HRESULT CScene_Logo::Ready_Prototype()
{
	if(bUniquePrototype)
	{
		// 임시 테스트용 Texture
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_TexForTest", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Texture/Test/TexForTest.png")), E_FAIL);
		
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_RectTex", CRectBuffer::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_SphereTex2", CSphereTex2::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Buffer_CrossRectTex", CCrossRectBuffer::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Effect/BackGrid.png")), E_FAIL);

		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_CharacterMouse", CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Character/Mouth.png")), E_FAIL);

		// 시로 폭탄 메쉬 
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_S", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb/", L"Shiro_Bomb.X")), E_FAIL); 
		FAILED_CHECK_RETURN(Management Ready_Prototype(L"Proto_SMesh_Shiro_Bomb_B", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Shiro/Shiro_Bomb_OL/", L"Shiro_Bomb_OL.X")), E_FAIL);

		bUniquePrototype = FALSE;
	}

	return S_OK;
}

HRESULT CScene_Logo::Ready_LightInfo()
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
	D3DLIGHT9* pLightInfo = Management Get_Light();

	return S_OK;
}

HRESULT CScene_Logo::Ready_SceneObject()
{
	CGameObject*			pGameObject = nullptr;

	// Dynamic Camera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, g_hWnd,
		T_CAMINFO(CAM_DEFAULT, VEC_ZERO, VEC_UP, D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f), E_OBJTAG::OBJ_DynamicCamera);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::GameLogic, pGameObject->Get_ObjTag(), pGameObject);

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround);
	assert(pGameObject);
	Add_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_BackGround, pGameObject);
	//pGameObject->Set_Pos(_vec3(0.f, 2.f, 0.f));	

	// 그림자 테스트용 수치값
	pGameObject->Set_Pos(_vec3(0.f, -0.01f, 0.f));
	pGameObject->Set_Rotation(90.f, 0.f, 0.f);
	pGameObject->Set_Scale(4.f, 4.f, 4.f);

	//pGameObject = CChar_Aru::Create(m_pGraphicDev, E_OBJTAG::OBJ_Player);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player, pGameObject);
	//pGameObject->Set_Pos(_vec3(3.f, 0.f, 3.f));

	//pGameObject = CHeavyTank_Decagram_Cannon::Create(m_pGraphicDev, E_OBJTAG::OBJ_Monster);
	//assert(pGameObject);
	//Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster, pGameObject);
	//pGameObject->Set_Pos(_vec3(3.f, 0.f, 3.f));
	//T_MONSTER_INFO	tInfo; 
	//tInfo.eAtkType = E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION; 
	//tInfo.eDiffType = E_CHAR_DFFTYPE::DFFTYPE_HEAVY; 
	//tInfo.eMonsterType = E_MONSTER_TYPE::TYPE_ELITE; 
	//tInfo.eWeaponType = E_CHAR_WEAPONTYPE::WEAPONTYPE_RG; 
	//tInfo.fRadius = 1.f; 
	//tInfo.iLevel = 1; 
	//tInfo.vScale = VEC_ONE;
	//tInfo.wszName = L""; 
	//static_cast<CMonster*>(pGameObject)->Set_MonsterInfo();

	return S_OK;
}