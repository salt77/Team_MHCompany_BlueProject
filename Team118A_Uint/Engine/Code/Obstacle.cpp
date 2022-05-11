#include "Obstacle.h"
USING(Engine)

CObstacle::CObstacle(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CObstacle::CObstacle(const CObstacle& rhs)
	: CGameObject(rhs)
{
}

CObstacle::~CObstacle()
{
}

void CObstacle::Free()
{

	CGameObject::Free();
}

void CObstacle::Save(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CObstacle::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}

//���� �ѹ� ����. Componet�� �߰��Ѵ�.
HRESULT CObstacle::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CObstacle::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

_int CObstacle::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);
	
	return iExit;
}

//���̴��� ���� �׷��ش�.
void CObstacle::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

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

void CObstacle::On_Collision(CGameObject * pDest)
{
}
 
//��ü�� �����Ѵ�.
CObstacle * CObstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CObstacle*	pInstance = new CObstacle(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//�������̵�. ��ü ������ �ʿ��� �޽��±׿� ��θ� �޾Ƽ� �����Ѵ�.
CObstacle* CObstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag)
{
	CObstacle*	pInstance = new CObstacle(pGraphicDev, eObjTag);

	pInstance->m_wstrMeshName = pMeshTag;
	pInstance->m_wstrMeshFullPath = pMeshFullPath;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//������Ʈ�� �ִٸ� ������Ÿ�� ������ ��ġ���ʰ� �ٷ� ����, �װ� �ƴ϶�� �߰��Ѵ�.
void CObstacle::Add_Component()
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

//���̴� ������̺�
void CObstacle::Set_Constantable()
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