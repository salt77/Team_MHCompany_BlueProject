#include "CombatTile.h"
#include "Renderer.h"
#include "SMesh.h"
#include "ProtoMgr.h"
USING(Engine)

CCombatTile::CCombatTile(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_TILE_OPTION eTileOption)
	: CGameObject(pGraphicDev, eObjTag),
	m_eTileOption(eTileOption)
{
}

CCombatTile::CCombatTile(const CCombatTile& rhs)
	: CGameObject(rhs),
	m_eTileOption(rhs.m_eTileOption)
{
}

CCombatTile::~CCombatTile()
{
}

void CCombatTile::Free()
{
	Safe_Release(m_pWarpParticle);
	CGameObject::Free();
}

void CCombatTile::Save(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CCombatTile::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}

//최초 한번 실행. Componet를 추가한다.
HRESULT CCombatTile::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CCombatTile::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	Update_TileActive(fTimeDelta);

	if (m_eTileOption == E_TILE_OPTION::TILE_WARP_ONE_WAY_START || m_eTileOption == E_TILE_OPTION::TILE_WARP_TWO_WAY)
	{
		if (!m_pWarpParticle)
		{
			m_pWarpParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"WarpTile.particle");
			m_pWarpParticle->Set_Pos(Get_Position());
			m_pWarpParticle->Set_Rotation(_vec3(0.f, 30.f, 0.f));
			m_pWarpParticle->AddRef();
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pWarpParticle);
		}
		else
		{
			if (m_pWarpParticle->Get_Dead())
			{
				Safe_Release(m_pWarpParticle);
				m_pWarpParticle = nullptr;
			}
		}
	}

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

_int CCombatTile::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);
	
	return iExit;
}

//쉐이더를 토대로 그려준다.
void CCombatTile::Render_Object(_int iPass)
{
	//m_iPassNumber = 4;
	
	Set_Constantable();

	m_pShaderCom->BeginShader();
	if (m_bRenderAlpha)
	{
		m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Alpha, iPass));
	}
	else
	{
		m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));
	}


	if (m_pSMeshCom)
	{
		m_pSMeshCom->Render_Meshes(m_pShaderCom);
		if (m_pSMeshCap && !m_bUsed)
		{
			LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

			_matrix		matWorld;
			matWorld = Get_Transform()->Get_WorldMatrix();
			matWorld._42 -= 0.07f;
			pEffect->SetMatrix("g_matWorld", &matWorld);
			pEffect->CommitChanges();
			m_pSMeshCap->Render_Meshes(m_pShaderCom);
		}
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CCombatTile::On_Collision(CGameObject * pDest)
{
}

 
//객체를 생성한다.
CCombatTile * CCombatTile::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag , E_TILE_OPTION eTileOption)
{
	CCombatTile*	pInstance = new CCombatTile(pGraphicDev, eObjTag, eTileOption);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//컴포넌트가 있다면 프로토타입 생성을 거치지않고 바로 생성, 그게 아니라면 추가한다.
void CCombatTile::Add_Component()
{
	CComponent*		pComponent = nullptr;
	
	m_wstrMeshName = L"CombatTile.X";
	m_wstrMeshFullPath = L"../../Reference/Resource/TileMesh/";

	Set_TileSMesh();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

//쉐이더 상수테이블
void CCombatTile::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	
	if (m_bRenderAlpha)
	{
		pEffect->SetFloat("g_fAddAlpha", m_fAlphaRatio);
	}
	else
	{
		pEffect->SetFloat("g_fFilter", 0.f);
	}
}

void CCombatTile::Set_TileActive(_bool bActive)
{
	if (!m_bTileActive && bActive)
	{
		m_bRenderAlpha = TRUE;
		m_bTileActive = bActive;
		m_fAlphaRatio = 0.f;
		Set_Active(TRUE);
	}
	else if (m_bTileActive && !bActive)
	{
		m_bRenderAlpha = TRUE;
		m_bTileActive = bActive;
		m_fAlphaRatio = 1.f;
	}
}

void CCombatTile::Set_TileSMesh()
{
	
	wstring wstrCapMeshName = L"";
	switch (m_eTileOption)
	{
	case Engine::E_TILE_OPTION::TILE_EMPTY:
		m_wstrMeshName = L"CombatTile.X";
		break;
	case Engine::E_TILE_OPTION::TILE_NORMAL:
		m_wstrMeshName = L"CombatTile_Normal.X";
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_START:
		m_wstrMeshName = L"CombatTile_Warp_One_Way.X";
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_ONE_WAY_END:
		m_wstrMeshName = L"CombatTile_Warp_One_Way.X";
		break;
	case Engine::E_TILE_OPTION::TILE_WARP_TWO_WAY:
		m_wstrMeshName = L"CombatTile_Warp_Two_Way.X";
		break;
	case Engine::E_TILE_OPTION::TILE_SPAWN:
		m_wstrMeshName = L"CombatTile_Normal.X";
		wstrCapMeshName = L"CombatTile_Spawn.X";
		break;
	case Engine::E_TILE_OPTION::TILE_REMOVE:
		m_wstrMeshName = L"CombatTile_Normal.X";
		wstrCapMeshName = L"CombatTile_Remove.X";
		break;
	case Engine::E_TILE_OPTION::TILE_VARIABLE_ON:
		m_wstrMeshName = L"CombatTile_Variable.X";
		break;
	case Engine::E_TILE_OPTION::TILE_VARIABLE_OFF:
		m_wstrMeshName = L"CombatTile_Variable.X";
		m_bTileActive = FALSE;
		Set_Active(FALSE);
		break;
	case Engine::E_TILE_OPTION::TILE_WEAK:
		m_wstrMeshName = L"CombatTile_Weak.X";
		break;
	case Engine::E_TILE_OPTION::TILE_START:
		m_wstrMeshName = L"CombatTile_Normal.X";
		break;
	case Engine::E_TILE_OPTION::TILE_END:
		break;
	default:
		break;
	}
	
	m_wstrMeshFullPath = L"../../Reference/Resource/TileMesh/";
	
	CComponent* pComponent = nullptr;
	pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);

	
	if (pComponent)
	{
		m_mapComponent.emplace(ComTag_SMesh, pComponent);
	}
	else
	{
		CProtoMgr::GetInstance()->Ready_Prototype(m_wstrMeshName, CSMesh::Create(m_pGraphicDev, m_wstrMeshFullPath, m_wstrMeshName));
		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);
		m_mapComponent.emplace(ComTag_SMesh, pComponent);
	}

	m_pSMeshCom = static_cast<CSMesh*>(pComponent);

	if (!wstrCapMeshName.empty())
	{

		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrCapMeshName);

		if (pComponent)
		{
			m_mapComponent.emplace(L"SMesh_Cap", pComponent);
		}
		else
		{
			CProtoMgr::GetInstance()->Ready_Prototype(wstrCapMeshName, CSMesh::Create(m_pGraphicDev, m_wstrMeshFullPath, wstrCapMeshName));
			pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrCapMeshName);
			m_mapComponent.emplace(L"SMesh_Cap", pComponent);
		}

		m_pSMeshCap = static_cast<CSMesh*>(pComponent);
	}
}

void CCombatTile::Update_TileActive(_float fTimeDelta)
{
	if (m_bRenderAlpha)
	{
		if (m_bTileActive)
		{
			m_fAlphaRatio += fTimeDelta;
			if (m_fAlphaRatio > 1.f)
			{
				m_fAlphaRatio = 1.f;
				m_bRenderAlpha = FALSE;
			}
			
		}
		else
		{
			m_fAlphaRatio -= fTimeDelta;
			if (m_fAlphaRatio < 0.f)
			{
				m_fAlphaRatio = 0.f;
				m_bRenderAlpha = FALSE;
				Set_Active(FALSE);
			}
		}


	}
}

