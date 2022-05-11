#include "StageMap.h"
#include "Renderer.h"
#include "ProtoMgr.h"
#include "SMesh.h"
#include "FogManager.h"

USING(Engine)
CStageMap::CStageMap(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CStageMap::CStageMap(const CStageMap& rhs)
	: CGameObject(rhs)
{
}

CStageMap::~CStageMap()
{
}

//���� �ѹ� ����. Componet�� �߰�
HRESULT CStageMap::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	//ũ��
	Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

//ī�� Ÿ���� ��� �ν��Ͻ��� ������Ʈ�Ѵ�.
_int CStageMap::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_bBlockRender)
	{
		if (m_eObjTag == E_OBJTAG::OBJ_BlockTile ||
			m_eObjTag == E_OBJTAG::OBJ_BlockTile_L ||
			m_eObjTag == E_OBJTAG::OBJ_BlockTile_R
			)
		{
			CRenderer::GetInstance()->Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA, this);
		}
	}

	if (m_eObjTag == E_OBJTAG::OBJ_CafeTile)
	{
		CRenderer::GetInstance()->Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA, this);
	}
	else
	{
		if (m_eObjTag == E_OBJTAG::OBJ_BlockTile ||
			m_eObjTag == E_OBJTAG::OBJ_BlockTile_L ||
			m_eObjTag == E_OBJTAG::OBJ_BlockTile_R
			)
		{
			return 0;
		}

		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	}

	return iExit;
}

_int CStageMap::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	return iExit;
}

//�ʿ� ���� ���̴��ɼ��� �ٸ��� �����Ѵ�.
void CStageMap::Render_Object(_int iPass)
{
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

void CStageMap::Debug_This_In_Console()
{
}
 
CStageMap * CStageMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CStageMap*	pInstance = new CStageMap(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//Create �������̵�. �±׿� ���ڿ��� �޴´�.
CStageMap * CStageMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshTag)
{
	CStageMap*	pInstance = new CStageMap(pGraphicDev, eObjTag);

	pInstance->m_wstrMeshName = pMeshTag;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//Create �������̵�. �±׿� ���ڿ�, ��α��� �޴´�.
CStageMap* CStageMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag)
{
	CStageMap*	pInstance = new CStageMap(pGraphicDev, eObjTag);
	
	//���� �̸� ����
	pInstance->m_wstrMeshName = pMeshTag;
	pInstance->m_wstrMeshFullPath = pMeshFullPath;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStageMap::Free()
{
	CGameObject::Free();
}

void CStageMap::Save(HANDLE hFile, DWORD & dwByte)
{
	//������ �������� cout���� ǥ��

	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CStageMap::Load(HANDLE hFile, DWORD & dwByte)
{
	//������ �ε����� cout���� ǥ��

	CGameObject::Load(hFile, dwByte);
}

//������Ʈ �߰�
void CStageMap::Add_Component()
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
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, CProtoMgr::GetInstance()->Clone_Prototype(m_wstrMeshName)));
	}

	if (m_eObjTag == E_OBJTAG::OBJ_CafeTile ||
		m_eObjTag == E_OBJTAG::OBJ_BlockTile ||
		m_eObjTag == E_OBJTAG::OBJ_BlockTile_L ||
		m_eObjTag == E_OBJTAG::OBJ_BlockTile_R
		)
	{
		//�ν��Ͻ�
		//Proto_Shader_Instance
		//m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
		m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	}
	else
	{
		m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	}
}

//���̴� ��� ���̺�
void CStageMap::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix	matView, matProj;
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//0204mat
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	// ����
	_vec4* pCameraPos = (_vec4*)&matView._41;
	pEffect->SetVector("g_vCamera", pCameraPos);
	pEffect->SetVector("g_vFogColor", &CFogManager::GetInstance()->Get_FogColor());
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
	
	// Ŀ���� ����
	T_CustomFog tCustomFog = CFogManager::GetInstance()->GetCustomFog();	
	pEffect->SetVector("g_CF_vPos", &(_vec4)(tCustomFog.vPos));

	pEffect->SetFloat("g_CF_Under", tCustomFog.fView_Under);
	pEffect->SetFloat("g_CF_Over", tCustomFog.fView_Over);

	pEffect->SetFloat("g_CF_Start", tCustomFog.fDistance_Start);
	pEffect->SetFloat("g_CF_End", tCustomFog.fDistance_End);
	pEffect->SetVector("g_CF_vColor_Start", &(_vec4)(tCustomFog.vColor_Start));
	pEffect->SetVector("g_CF_vColor_End", &(_vec4)(tCustomFog.vColor_End));

	pEffect->SetFloat("g_fFilter", 0.f);
}
