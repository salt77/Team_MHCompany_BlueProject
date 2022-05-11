#include "StaticObject.h"
#include "Scene.h"
#include "Renderer.h"
#include "SMesh.h"
#include "ProtoMgr.h"

USING(Engine)


CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath)
	: CGameObject(pGraphicDev, eObjTag),
	m_wstrMeshFullPath(wstrSMeshFullPath)
{
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
	: CGameObject(rhs),
	m_wstrMeshFullPath(rhs.m_wstrMeshFullPath)
{
}

CStaticObject::~CStaticObject()
{
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev, eObjTag, wstrSMeshFullPath);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStaticObject::Free()
{

	CGameObject::Free();
}

void CStaticObject::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte);
}

void CStaticObject::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}

//최초 한번 실행. Componet를 추가한다.
HRESULT CStaticObject::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CStaticObject::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

_int CStaticObject::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);
	
	return iExit;
}

//쉐이더를 토대로 그려준다.
void CStaticObject::Render_Object(_int iPass)
{
	//m_iPassNumber = 4;
	
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Stage, iPass));

	if (m_pSMeshCom)
	{
		m_pSMeshCom->Render_Meshes(m_pShaderCom);
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CStaticObject::On_Collision(CGameObject * pDest)
{
}
 

//컴포넌트가 있다면 프로토타입 생성을 거치지않고 바로 생성, 그게 아니라면 추가한다.
void CStaticObject::Add_Component()
{
	CComponent*		pComponent = nullptr;
	
	Set_SMesh();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

//쉐이더 상수테이블
void CStaticObject::Set_Constantable()
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

void CStaticObject::Set_SMesh()
{
	_int iCheck = (_int)m_wstrMeshFullPath.rfind(L"/");
	m_wstrMeshName = m_wstrMeshFullPath.substr(iCheck + 1);
	m_wstrMeshPath = m_wstrMeshFullPath.substr(0, iCheck+1);

	CComponent* pComponent = nullptr;
	pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);

	
	if (pComponent)
	{
		m_mapComponent.emplace(ComTag_SMesh, pComponent);
	}
	else
	{
		CProtoMgr::GetInstance()->Ready_Prototype(m_wstrMeshName, CSMesh::Create(m_pGraphicDev, m_wstrMeshPath, m_wstrMeshName));
		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName);
		m_mapComponent.emplace(ComTag_SMesh, pComponent);
	}

	m_pSMeshCom = static_cast<CSMesh*>(pComponent);
}

