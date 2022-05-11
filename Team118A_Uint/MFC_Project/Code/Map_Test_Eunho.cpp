#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Map_Test_Eunho.h"

USING(Engine)

CMap_Test_Eunho::CMap_Test_Eunho(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{

}

CMap_Test_Eunho::CMap_Test_Eunho(const CMap_Test_Eunho& rhs)
	: CGameObject(rhs)
{

}

CMap_Test_Eunho::~CMap_Test_Eunho()
{

}

HRESULT CMap_Test_Eunho::Ready_Object()
{
	CGameObject::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CMap_Test_Eunho::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	//Management Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

_int CMap_Test_Eunho::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);
	return iExit;
}

void CMap_Test_Eunho::Render_Object()
{
	Set_Constantable();
	Set_Constantable_Fog();


	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(m_iPassNumber);


	m_pSMeshCom->Render_Meshes(m_pShaderCom);


	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CMap_Test_Eunho::Render_Shadow()
{
	Set_Constantable_Shadow(m_pShaderCom);


	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(2); // 2번은 그림자용


	m_pSMeshCom->Render_Meshes(m_pShaderCom);


	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

CMap_Test_Eunho* CMap_Test_Eunho::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CMap_Test_Eunho*	pInstance = new CMap_Test_Eunho(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMap_Test_Eunho::Free()
{
	CGameObject::Free();
}

void CMap_Test_Eunho::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, Management Clone_Prototype(L"Proto_Test_MonsterHunter")));
	
	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Fog")));
}

void CMap_Test_Eunho::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
}

void CMap_Test_Eunho::Set_Constantable_Fog()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetFloat("g_fFogStart", Management Get_FogStart());
	pEffect->SetFloat("g_fFogEnd", Management Get_FogEnd());
	pEffect->SetVector("g_vFogColor", &( (_vec4)Management Get_FogColor() ));
	//pEffect->SetFloat("g_fFogEnd", &matView);
	//pEffect->SetVector("g_vFogColor", &matProj);
}

