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

//최초 한번 실행. Componet를 추가
HRESULT CStageMap::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	//크기
	Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

//카페 타일의 경우 인스턴싱을 업데이트한다.
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

//맵에 따라 쉐이더옵션을 다르게 설정한다.
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

//Create 오버라이딩. 태그와 문자열만 받는다.
CStageMap * CStageMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshTag)
{
	CStageMap*	pInstance = new CStageMap(pGraphicDev, eObjTag);

	pInstance->m_wstrMeshName = pMeshTag;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//Create 오버라이딩. 태그와 문자열, 경로까지 받는다.
CStageMap* CStageMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag)
{
	CStageMap*	pInstance = new CStageMap(pGraphicDev, eObjTag);
	
	//파일 이름 저장
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
	//무엇을 저장할지 cout으로 표기

	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CStageMap::Load(HANDLE hFile, DWORD & dwByte)
{
	//무엇을 로드할지 cout으로 표기

	CGameObject::Load(hFile, dwByte);
}

//컴포넌트 추가
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
		//인스턴싱
		//Proto_Shader_Instance
		//m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
		m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	}
	else
	{
		m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	}
}

//쉐이더 상수 테이블
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

	// 포그
	_vec4* pCameraPos = (_vec4*)&matView._41;
	pEffect->SetVector("g_vCamera", pCameraPos);
	pEffect->SetVector("g_vFogColor", &CFogManager::GetInstance()->Get_FogColor());
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
	
	// 커스텀 포그
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
