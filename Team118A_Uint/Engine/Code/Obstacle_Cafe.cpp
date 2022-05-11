#include "Obstacle_Cafe.h"
#include "LightMgr.h"

USING(Engine)

CObstacle_Cafe::CObstacle_Cafe(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CObstacle(pGraphicDev, eObjTag)
{
}

CObstacle_Cafe::CObstacle_Cafe(const CObstacle_Cafe& rhs)
	: CObstacle(rhs)
{
}

CObstacle_Cafe::~CObstacle_Cafe()
{
}

void CObstacle_Cafe::Free()
{
	CObstacle::Free();
}

void CObstacle_Cafe::Save(HANDLE hFile, DWORD & dwByte)
{
	//무엇을 저장할지 cout으로 표기

	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CObstacle_Cafe::Load(HANDLE hFile, DWORD & dwByte)
{
	//무엇을 로드할지 cout으로 표기

	CGameObject::Load(hFile, dwByte);
}

//최초 한번 실행. Componet를 추가한다.
HRESULT CObstacle_Cafe::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	Set_Scale(1.f, 1.f, 1.f);
	Set_Pos(_vec3(0.f, -2.f, 0.f));

	m_pSphereCollider->SetRadius(0.3f);
	m_pSphereCollider->SetScale(Get_Scale());

	return S_OK;
}

_int CObstacle_Cafe::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	m_fCreateFurShade += fTimeDelta;

	if (m_bCreateNew)
	{
		m_fCreate_Up_Time += fTimeDelta;

		if (m_fCreate_Up_Time <= 2.f)
		{
			if (m_fvecY >= 0.f)
			{
				m_fCreate_Up_Time = 0.f;
				m_bCreateNew = false;
				return 0;
			}

			m_fvecY += fTimeDelta * 1.f;
			Set_Pos(_vec3(0.f, m_fvecY, 0.f));
		}
		
		if(m_fCreate_Up_Time > 2.f)
		{
			m_fCreate_Up_Time = 0.f;
			m_bCreateNew = false;
		}
	}

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

//카페 가구 선택 시 쉐이더 옵션을 다르게 주기 위해 fTimeDelta를 누적해준다.
_int CObstacle_Cafe::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	m_pSphereCollider->SetMatrix(m_pTransformCom->Get_WorldMatrix());

	if (m_bCreateShaderSetting)
	{
		m_fCreateShaderFilter += fTimeDelta * -1.f;
		if (1.5f > m_fCreateShaderFilter)
		{
			CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
			CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
		}
	}

	if (m_bSelRender)
	{
		m_fShaderFilter += fTimeDelta;

		if (1.f <= m_fShaderFilter)
		{
			m_fShaderFilter -= 1.f;
		}
	}
	else
	{
		m_fShaderFilter = 0.f;
	}

	return iExit;
}

//Obstacle 쉐이더를 적용한다. 만약 메쉬 컴객체가 컴포넌트를 클론하고 대입한다.
void CObstacle_Cafe::Render_Object(_int iPass)
{	
	Set_Constantable();

	m_pShaderCom->BeginShader();

	if (m_bCreateShaderSetting)
	{
		//여기 Pass 바꿔서 처음 생성 2초간 서서히 렌더
		m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));
	}
	else
	{
		m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));
	}

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

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

//구 충돌체의 위치를 가져온다.
_vec3 CObstacle_Cafe::Get_ShperePos()
{
	return _vec3(m_pSphereCollider->GetPos().x, 0.f, m_pSphereCollider->GetPos().z);
}
 
//가구를 생성한다.
CObstacle_Cafe * CObstacle_Cafe::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CObstacle_Cafe*	pInstance = new CObstacle_Cafe(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

// 모든 가구를 생성한다. 태그와 문자열, 경로까지 받는다.
CObstacle_Cafe* CObstacle_Cafe::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag)
{
	CObstacle_Cafe*	pInstance = new CObstacle_Cafe(pGraphicDev, eObjTag);

	pInstance->m_wstrMeshName = pMeshTag;
	pInstance->m_wstrMeshFullPath = pMeshFullPath;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}


//거리에 따라서 Sort하기 위한 Compare함수
_bool CObstacle_Cafe::CompareByDistance(CObstacle_Cafe * pSrc, CObstacle_Cafe * pDest)
{
	if (pSrc->m_fDistanceForSort < pDest->m_fDistanceForSort)
		return TRUE;
	return FALSE;
}

//컴포넌트가 있다면 프로토타입 생성을 거치지않고 바로 생성, 그게 아니라면 추가한다.
void CObstacle_Cafe::Add_Component()
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

	//충돌체 추가
	pComponent = m_pSphereCollider = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(ComTag_Collider, pComponent);

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

//쉐이더 상수 테이블
void CObstacle_Cafe::Set_Constantable()
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
	//
	//// 그림자용
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	if (m_fCreateFurShade <= 2.f)
	{
		pEffect->SetFloat("g_fFilter", m_fCreateShaderFilter);
	}
	else
	{
		m_bCreateShaderSetting = false;
		pEffect->SetFloat("g_fFilter", m_fShaderFilter);
	}
}

