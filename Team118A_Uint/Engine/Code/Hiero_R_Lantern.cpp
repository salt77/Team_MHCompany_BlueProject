#include "Hiero_R_Lantern.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "SMesh.h"
#include "EffectObject.h"
#include "Scene.h"
#include "GameManager.h"
CHiero_R_Lantern::CHiero_R_Lantern(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CHiero_R_Lantern::~CHiero_R_Lantern()
{
}

CHiero_R_Lantern * CHiero_R_Lantern::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHiero_R_Lantern* pInstance = new CHiero_R_Lantern(pGraphicDev, eObjTag);

	if (pInstance->Ready_Object())
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHiero_R_Lantern::Free()
{
	CGameObject::Free();
}

HRESULT CHiero_R_Lantern::Ready_Object()
{
	CGameObject::Ready_Object();

	Add_Component();

	return S_OK;
}

_int CHiero_R_Lantern::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	m_fActiveTime -= fTimeDelta;
	if (0.f < m_fActiveTime)
	{
		m_bLantern_Active = TRUE;

		for (auto& iter : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
		{
			iter->Set_Invn((m_fRadius >= CUtility::Distance_vec3(Get_Position(), iter->Get_Position())));
		}
	}
	else
	{
		if (m_bLantern_Active)
		{
			m_bLantern_Active = FALSE;
			for (auto& iter : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
			{
				iter->Set_Invn(FALSE);
			}
		}
	}


	return iExit;
}

_int CHiero_R_Lantern::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CHiero_R_Lantern::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));
	m_pSMeshCom->Set_Glow_Option(0, 0.5f, _vec3(1.f, 1.f, 1.f), 1.f, 0.f, 1.f, 0.f, 0.5f, _vec3(1.f, 0.f, 0.f));

	if (m_pSMeshCom)
	{
		m_pSMeshCom->Render_Meshes(m_pShaderCom);
	}
	else
	{
		CComponent* pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);

		if (pComponent)
		{
			m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, pComponent));
		}
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CHiero_R_Lantern::Set_ActiveLantern(const _bool & bState)
{
	if (bState == m_bLantern_Active)
		return;

	m_bLantern_Active = bState;
	m_fActiveTime = 10.f;

	if (FALSE == m_bLantern_Active)
	{
		return;
	}

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_HolyRelic_Red.wav", 0.1f);
	for (_int i = 0; i < 3; ++i)
	{
		CEffectObject* pEffect = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), VEC_ZERO, L"Hieronymus_Lantern_R_Wave.effect");
		pEffect->Get_Effect_Info().fDelayTime = 0.2f * i;
		pEffect->Get_Effect_Info().vScale_End = VEC_ONE * m_fRadius * 2;
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pEffect);
	}
}

void CHiero_R_Lantern::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);

	if (pComponent)
	{
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, pComponent));
	}
	else
	{
		CProtoMgr::GetInstance()->Ready_Prototype(m_wstrMeshName, CSMesh::Create(m_pGraphicDev, m_wstrMeshFullPath, m_wstrMeshName));
		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, pComponent));
	}

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CHiero_R_Lantern::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	//0204mat
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);
}
