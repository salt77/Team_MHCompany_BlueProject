#include "Binah_Halo.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "EffectObject.h"
#include "Scene.h"
#include "Unit.h"
#include "GameManager.h"
#include "Binah.h"

CBinah_Halo::CBinah_Halo(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CBinah_Halo::~CBinah_Halo()
{
}

CBinah_Halo * CBinah_Halo::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, const _vec3 vStartPos, const _vec3& vDir)
{
	CBinah_Halo* pInstance = new CBinah_Halo(pGraphicDev, eObjTag);

	if (pInstance->Ready_Object(vStartPos, vDir))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinah_Halo::Free()
{
	CGameObject::Free();
}

HRESULT CBinah_Halo::Ready_Object(const _vec3 vStartPos, const _vec3& vDir)
{
	CGameObject::Ready_Object();

	Add_Component();

	Set_Scale(_vec3(0.8f, 0.8f, 0.8f));

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	Set_Direction(m_vDir);

	m_pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	return S_OK;
}

_int CBinah_Halo::Update_Object(const _float & fTimeDelta)
{	
	_int	iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pBinah != nullptr)
	{
		Set_Pos(m_pBinah->Get_Position() - _vec3(3.f, 0.45f, -2.2f));
		Set_Rotation(m_pBinah->Get_Angle() + _vec3(-86.f, -55.f, 92.f));

		Attach_Bone();
	}

	return iExit;
}

_int CBinah_Halo::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	//m_pSMeshCom->Set_Glow_Option(0, 0.8f, _vec3(1.f, 1.f, 1.f), 1.f, 0.f, 1.f, 0.f, 1.f, _vec3(1.f, 1.f, 1.f));

	return iExit;
}

void CBinah_Halo::Render_Object(_int iPass)
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

void CBinah_Halo::Set_Constantable()
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

void CBinah_Halo::Add_Component()
{
	CComponent*		pComponent = nullptr;

	wstring wstrMeshName = L"Binah_Halo.X";
	wstring wstrMeshPath = L"../../Reference/Resource/Monster/Binah/Halo/";

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

void CBinah_Halo::Movement(const _float & fTimeDelta)
{
	m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
}

void CBinah_Halo::Attach_Bone()
{
	CDMesh* pMeshCom = static_cast<CDMesh*>(m_pBinah->Get_Component(ComTag_DMesh));
	const D3DXFRAME_DERIVED* pFrame = pMeshCom->Get_FrameByName("Head_cap_01");
	const _matrix* pBoneMatrix = &pFrame->CombinedTransformMatrix;
	CTransform* pBinahTrans = static_cast<CTransform*>(m_pBinah->Get_Transform());
	const _matrix* pWorldMatrix = &pBinahTrans->Get_WorldMatrix();
	m_pTransformCom->Set_ParentMatrix(&(*pBoneMatrix * *pWorldMatrix));
}

void CBinah_Halo::OnEnable()
{
	CGameObject::OnEnable();
}

void CBinah_Halo::OnDisable()
{
	CGameObject::OnDisable();
}

void CBinah_Halo::OnDestroy()
{
	CGameObject::OnDestroy();
}