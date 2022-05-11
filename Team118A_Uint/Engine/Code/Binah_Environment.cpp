#include "Binah_Environment.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "EffectObject.h"
#include "Scene.h"
#include "Unit.h"
#include "GameManager.h"
#include "Binah.h"

CBinah_Environment::CBinah_Environment(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CBinah_Environment::~CBinah_Environment()
{
}

CBinah_Environment * CBinah_Environment::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, const _vec3 vStartPos, const _vec3& vDir)
{
	CBinah_Environment* pInstance = new CBinah_Environment(pGraphicDev, eObjTag);

	if (pInstance->Ready_Object(vStartPos, vDir))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinah_Environment::Free()
{
	CGameObject::Free();
}

HRESULT CBinah_Environment::Ready_Object(const _vec3 vStartPos, const _vec3& vDir)
{
	CGameObject::Ready_Object();

	Add_Component();

	Set_Scale(_vec3(1.f, 1.f, 1.f));

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	Set_Direction(m_vDir);

	m_pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	return S_OK;
}

_int CBinah_Environment::Update_Object(const _float & fTimeDelta)
{	
	_int	iExit = CGameObject::Update_Object(fTimeDelta);

	CBinah* pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (pBinah->Get_UnitDead())
	{
		Set_Dead(true);
	}

	return iExit;
}

_int CBinah_Environment::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	//m_pSMeshCom->Set_Glow_Option(1, 0.5f, _vec3(1.f, 1.f, 1.f), 1.f, 0.f, 1.f, 0.f, 1.f, _vec3(1.f, 1.f, 1.f));

	return iExit;
}

void CBinah_Environment::Render_Object(_int iPass)
{
	if (nullptr == m_pSMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	m_pSMeshCom->Render_Meshes(m_pShaderCom);

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBinah_Environment::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	//0204mat
	//_matrix	matView, matProj;
	//
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	// 그림자용
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망시 필터
	//pEffect->SetFloat("g_fFilter", 0.f);
}

void CBinah_Environment::Add_Component()
{
	CComponent*		pComponent = nullptr;

	wstring wstrMeshName = L"Binah_Environment.X";
	wstring wstrMeshPath = L"../../Reference/Resource/Monster/Binah/Environment/";

	pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrMeshName);
	if (pComponent)
	{
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, pComponent));
	}
	else
	{
		CProtoMgr::GetInstance()->Ready_Prototype(wstrMeshName, CSMesh::Create(m_pGraphicDev, wstrMeshPath, wstrMeshName));
		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrMeshName);
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, pComponent));
	}

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CBinah_Environment::Movement(const _float & fTimeDelta)
{
	m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
}

void CBinah_Environment::OnEnable()
{
	CGameObject::OnEnable();
}

void CBinah_Environment::OnDisable()
{
	CGameObject::OnDisable();
}

void CBinah_Environment::OnDestroy()
{
	CGameObject::OnDestroy();
}