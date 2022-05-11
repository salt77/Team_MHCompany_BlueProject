#include "stdafx.h"
#include "../Include/stdafx.h"
#include "SMesh_Test.h"

USING(Engine)

CSMesh_Test::CSMesh_Test(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{

}

CSMesh_Test::CSMesh_Test(const CSMesh_Test& rhs)
	: CGameObject(rhs)
{

}

CSMesh_Test::~CSMesh_Test()
{
}

HRESULT CSMesh_Test::Ready_Object()
{
	CGameObject::Ready_Object(); 
	Add_Component();
	
	return S_OK;
}

_int CSMesh_Test::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	/*
	if (Management Key_Down(KEY_G))
	{
#ifdef _DEBUG
		cout << "나는 스태틱 메쉬야" << endl;
#endif
	}
	*/

	//Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	Management Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA, this);
	//Management Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

_int CSMesh_Test::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CSMesh_Test::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(m_iPassNumber);

	m_pSMeshCom->Render_Meshes(m_pShaderCom);

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CSMesh_Test::Debug_This_In_Console()
{
#ifdef _DEBUG
	cout << "나는 스태틱 메쉬야" << endl;
#endif
}

 
CSMesh_Test* CSMesh_Test::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSMesh_Test*	pInstance = new CSMesh_Test(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSMesh_Test::Free()
{
	CGameObject::Free();
}

void CSMesh_Test::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Mesh
	m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, Management Clone_Prototype(L"Proto_Map_BackStreet_Prop15")));
	//m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, Management Clone_Prototype(L"Backstreet.X")));

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Instance")));
}

void CSMesh_Test::Set_Constantable()
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

