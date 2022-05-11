#include "Obstacle_Combat.h"
#include "Unit.h"
USING(Engine)

CObstacle_Combat::CObstacle_Combat(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CObstacle(pGraphicDev, eObjTag)
{
}

CObstacle_Combat::CObstacle_Combat(const CObstacle_Combat& rhs)
	: CObstacle(rhs)
{
}

CObstacle_Combat::~CObstacle_Combat()
{
}

void CObstacle_Combat::Free()
{
	DeleteHideObject();
	CObstacle::Free();
}

void CObstacle_Combat::Save(HANDLE hFile, DWORD & dwByte)
{
	//무엇을 저장할지 cout으로 표기

	CUtility::Data_WriteWString(hFile, m_wstrMeshName, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrMeshFullPath, dwByte);

	CGameObject::Save(hFile, dwByte);
}

void CObstacle_Combat::Load(HANDLE hFile, DWORD & dwByte)
{
	//무엇을 로드할지 cout으로 표기

	CGameObject::Load(hFile, dwByte);
}

//최초 한번 실행. Componet를 추가
HRESULT CObstacle_Combat::Ready_Object()
{
	CGameObject::Ready_Object();

	Add_Component();

	Set_Pos(_vec3(0.f, 0.f, 0.f));
	m_pSphereCollider->SetRadius(0.5f);
	m_pSphereCollider->SetScale(Get_Scale());
	return S_OK;
}

_int CObstacle_Combat::Update_Object(const _float& fTimeDelta)
{
	ChoiceBidObject();
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	m_pSphereCollider->SetMatrix(m_pTransformCom->Get_WorldMatrix());

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

//카페 가구 선택 시 쉐이더 옵션을 다르게 주기 위해 fTimeDelta를 누적해준다.
_int CObstacle_Combat::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	return iExit;
}

//Obstacle 쉐이더를 적용한다. 만약 메쉬 컴객체가 컴포넌트를 클론하고 대입한다.
void CObstacle_Combat::Render_Object(_int iPass)
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

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

//엄폐물을 사용할 수 있는 오브젝트인지 판별하여 후보에 삽입하는 함수
void CObstacle_Combat::Bid_Obstacle(CGameObject * pObject)
{
	if (m_pHideObject)
	{
		return;
	}

	E_OBJTAG eObjTag = pObject->Get_ObjTag();
	_vec3 vPlayDir;

	_float fMinDistance = 999999.f;

	if (eObjTag == E_OBJTAG::OBJ_Player)
	{
		vPlayDir = _vec3(0.f, 0.f, -1.f);
		map<E_OBJTAG, vector<CGameObject*>>* pMapLayerMonster = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Monster);
		for (auto& tPair : *pMapLayerMonster)
		{
			for (auto& pMonster : tPair.second)
			{
				if (static_cast<CUnit*>(pMonster)->Get_UnitDead())
					continue;


				_vec3 vShootDir = pMonster->Get_Info(E_INFO::INFO_POS) - Get_HideSpotPos(pObject);
			
				_vec3 vNormalShootDir;
				D3DXVec3Normalize(&vNormalShootDir, &vShootDir);
				if (D3DXVec3Dot(&vPlayDir, &vNormalShootDir) < 0.f)
				{
					return;
				}

				_float fDistance = D3DXVec3Length(&vShootDir);

				if (fDistance < 1.f)
				{
					return;
				}
				
				if (fDistance < fMinDistance)
				{
					fMinDistance = fDistance;
				}
			}
		}
	}
	else
	{
		vPlayDir = _vec3(0.f, 0.f, 1.f);
		vector<CGameObject*>* pVecPlayer = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
		if (nullptr == pVecPlayer)
			return;

		for (auto& pPlayer : *pVecPlayer)
		{
			if (static_cast<CUnit*>(pPlayer)->Get_UnitDead())
				continue;

			_vec3 vShootDir = pPlayer->Get_Info(E_INFO::INFO_POS) - Get_HideSpotPos(pObject);
			_vec3 vNormalShootDir;
			D3DXVec3Normalize(&vNormalShootDir, &vShootDir);
			if (D3DXVec3Dot(&vPlayDir, &vNormalShootDir) < 0.f)
			{
				return;
			}
			_float fDistance = D3DXVec3Length(&vShootDir);

			if (fDistance < 1.f)
			{
				return;
			}

			if (fDistance < fMinDistance)
			{
				fMinDistance = fDistance;
			}
		}
	}

	if (fMinDistance < static_cast<CUnit*>(pObject)->Get_UnitRange())//나중에 디파인 값으로 바꿔야함
	{
		m_vecBidObject.emplace_back(pObject);
	}
}

void CObstacle_Combat::Check_Obstacle()
{
	if (nullptr == m_pHideObject)
		return;

	E_OBJTAG eObjTag = m_pHideObject->Get_ObjTag();
	_vec3 vPlayDir;

	if (eObjTag == E_OBJTAG::OBJ_Player)
	{
		vPlayDir = _vec3(0.f, 0.f, -1.f);
		map<E_OBJTAG, vector<CGameObject*>>* pMapLayerMonster = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Monster);
		for (auto& tPair : *pMapLayerMonster)
		{
			for (auto& pMonster : tPair.second)
			{
				_vec3 vShootDir = pMonster->Get_Info(E_INFO::INFO_POS) - Get_HideSpotPos(m_pHideObject);
				_vec3 vNormalShootDir;
				D3DXVec3Normalize(&vNormalShootDir, &vShootDir);
				if (D3DXVec3Dot(&vPlayDir, &vNormalShootDir) < 0.f)
				{
					DeleteHideObject();
					return;
				}
			}
		}
	}
	else
	{
		vPlayDir = _vec3(0.f, 0.f, 1.f);
		vector<CGameObject*>* pVecPlayer = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
		if (nullptr == pVecPlayer)
			return;

		for (auto& pPlayer : *pVecPlayer)
		{
			_vec3 vShootDir = pPlayer->Get_Info(E_INFO::INFO_POS) - Get_HideSpotPos(m_pHideObject);
			_vec3 vNormalShootDir;
			D3DXVec3Normalize(&vNormalShootDir, &vShootDir);
			if (D3DXVec3Dot(&vPlayDir, &vNormalShootDir) < 0.f)
			{
				DeleteHideObject();
				return;
			}
		}
	}

}


void CObstacle_Combat::DeleteHideObject()
{
	if (m_pHideObject)
	{
		static_cast<CUnit*>(m_pHideObject)->CutHideObstacle();
		m_pHideObject = nullptr;
	}
}

//엄폐물을 이용 가능한 오브젝트 중에서 가장 가까이 있는 오브젝트를 골라서 연결하는 함수
void CObstacle_Combat::ChoiceBidObject()
{
	if (m_pHideObject)
	{
		m_vecBidObject.clear();
		return;
	}
	if (m_vecBidObject.empty())
	{
		return;
	}


	_float fMinDistance = 999999.f;
	CGameObject* pSelectObj = nullptr;
	for (auto& pObj : m_vecBidObject)
	{
		CUnit* pUnit = static_cast<CUnit*>(pObj);
		if (pUnit->Get_HasHideObstacle())
		{
			continue;
		}

		_float fDistance = D3DXVec3Length(&(pUnit->Get_Info(E_INFO::INFO_POS) - Get_HideSpotPos(pUnit)));
		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			pSelectObj = pObj;
		}
	}

	if (pSelectObj)
	{
		m_pHideObject = pSelectObj;

		static_cast<CUnit*>(m_pHideObject)->Set_HideObstacle(this);
	}
	m_vecBidObject.clear();
}

//캐릭터 엄폐장소를 반환하는 함수
_vec3 CObstacle_Combat::Get_HideSpotPos(CGameObject* pHideObject)
{
	//예외 처리
	if (!pHideObject->Get_Component(ComTag_Collider))
	{
		return _vec3();
	}

	CCollider* pCol = static_cast<CCollider*>(pHideObject->Get_Component(ComTag_Collider));

	CSphereCollider* pSphereCol = nullptr;
	if (E_COLTAG::COL_SPHERE == pCol->GetColliderTag())
	{
		pSphereCol = static_cast<CSphereCollider*>(pCol);
	}
	_vec3 vHideDir;
	if (pHideObject->Get_ObjTag() == E_OBJTAG::OBJ_Player)
	{
		vHideDir = { 0.f,0.f, 1.f };
	}
	else
	{
		vHideDir = { 0.f,0.f,-1.f };
	}

	_vec3 vPos = Get_Info(E_INFO::INFO_POS);
	_float fObstacle = m_pSphereCollider->GetRadius();
	_float fUnitRadius = pSphereCol->GetRadius();

	_vec3 vReturn = vPos + vHideDir * (fObstacle + fUnitRadius);
	return vReturn;
}

void CObstacle_Combat::CutHideObject()
{
	m_pHideObject = nullptr;
}

CObstacle_Combat * CObstacle_Combat::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CObstacle_Combat*	pInstance = new CObstacle_Combat(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//모든 가구를 생성한다. 태그와 문자열, 경로까지 받는다.
CObstacle_Combat* CObstacle_Combat::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag)
{
	CObstacle_Combat*	pInstance = new CObstacle_Combat(pGraphicDev, eObjTag);

	pInstance->m_wstrMeshName = pMeshTag;
	pInstance->m_wstrMeshFullPath = pMeshFullPath;

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

//컴포넌트 추가
void CObstacle_Combat::Add_Component()
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
	}
	pComponent = m_pSphereCollider = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(ComTag_Collider, pComponent);

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

//쉐이더 상수 테이블
void CObstacle_Combat::Set_Constantable()
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

	pEffect->SetFloat("g_fFilter", 0.f);
}

