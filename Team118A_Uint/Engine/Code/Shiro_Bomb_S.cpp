#include "Shiro_Bomb_S.h"

#include "SphereCollider.h"
#include "SMesh.h"
#include "LightMgr.h"
USING(Engine)

CShiro_Bomb_S::CShiro_Bomb_S(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CShiro_Bomb_S::CShiro_Bomb_S(const CShiro_Bomb_S & rhs)
	: CAttackObj(rhs)
{
}

CShiro_Bomb_S::~CShiro_Bomb_S()
{
}

CShiro_Bomb_S * CShiro_Bomb_S::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vStartPos, const _vec3& vTargetPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _bool bNormalBomb)
{
	CShiro_Bomb_S*	pInstance = new CShiro_Bomb_S(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vStartPos, vTargetPos, tUnitInfo, eAtkType, bNormalBomb)))
		Safe_Release(pInstance);

	return pInstance;
}

void CShiro_Bomb_S::Free()
{
	CAttackObj::Free();
}

void CShiro_Bomb_S::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CShiro_Bomb_S::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CShiro_Bomb_S::Ready_Object(const _vec3& vStartPos, const _vec3& vTargetPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _bool bNormalBomb)
{
	CAttackObj::Ready_Object();

	m_bNormalBomb = bNormalBomb; 

	Add_Component();

	m_vStartPos = vStartPos; 
	m_vTargetPos = vTargetPos; 
	m_fDistance = CUtility::Distance_vec3(m_vStartPos, m_vTargetPos);

	D3DXVec3Normalize(&m_vDir, &(vTargetPos - vStartPos));
	Set_Pos(m_vStartPos);

	//m_fSpeed /= (fDistance); 
	////m_fFallSpeed *= (fDistance / 1.5f); 
	//m_fTimeDec *= (fDistance / 6.f); 
	// 위치 2개, 중력가속도, 도착시간 

	m_pSphereColliderCom->SetRadius(0.1f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	return S_OK;
}

_int CShiro_Bomb_S::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CShiro_Bomb_S::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	Movement(fTimeDelta); 
	Movement_Ex(fTimeDelta); 

	return iExit;
}

_int CShiro_Bomb_S::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CShiro_Bomb_S::Render_Object(_int iPass)
{
	if (nullptr == m_pSMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	m_pSMeshCom->Render_Meshes(m_pShaderCom);

	//m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass)); 
	//m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CShiro_Bomb_S::Debug_This_In_Console()
{
}

void CShiro_Bomb_S::Set_Constantable()
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

	// 사망시 필터
	pEffect->SetFloat("g_fFilter", 0.f);
}

void CShiro_Bomb_S::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// buffer
	m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_SMesh, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Shiro_Bomb_S")));

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CShiro_Bomb_S::Movement(const _float & fTimeDelta)
{
	// p = p - (vo * t - 0.5f * GRAVITY * t^2)
	// p = p0 + (vo * t) + (0.5 * a * t ^ 2)
	// v = v0 + (a * t)
	// 위치 2개, 중력가속도, 도착시간 
	if (m_bNormalBomb)
	{
		m_fCountingTime += fTimeDelta; 

		if (0.1f >= Get_Position().y)
		{
			Bomb();

			return;
		}

		_vec3	vMyPos = Get_Position(); 
		_float	fY = vMyPos.y; 

		fY = m_vStartPos.y - (m_fFallSpeed * m_fTime + (0.5f * GR * m_fTime * m_fTime));
		m_fTime -= 3.f * fTimeDelta;

		Set_Pos(vMyPos + (m_vDir * (m_fDistance / m_fArriveTime) * fTimeDelta));

		vMyPos = Get_Position();
		vMyPos.y = fY;
		Set_Pos(vMyPos);
	}
}

void CShiro_Bomb_S::Movement_Ex(const _float & fTimeDelta)
{
	if (!m_bNormalBomb)
	{
		_float	fY = Get_Position().y;

		if (0.1f >= fY)
		{
			m_fCountingTime += fTimeDelta; 

			if (3.5f <= m_fCountingTime)
			{
				Bomb(); 
			}

			return;
		}

		m_pTransformCom->Move_Pos(-VEC_UP, m_fFallSpeed, fTimeDelta); 
	}
}

void CShiro_Bomb_S::Bomb()
{
	// 폭발 이펙트 생성 코드 여기다가 삽입 

	m_bDead = TRUE; 
}

void CShiro_Bomb_S::OnEnable()
{
	CAttackObj::OnEnable();
}

void CShiro_Bomb_S::OnDisable()
{
	CAttackObj::OnDisable();
}

void CShiro_Bomb_S::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CShiro_Bomb_S::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest); 

	//if (E_LAYERTAG::AttackObj_Monster == pDest->Get_LayerTag())
	//{
	//	if (E_ATKOBJ_TYPE::ATKOBJ_SHIRO_BALL == static_cast<CAttackObj*>(pDest)->Get_AtkObjType())
	//	{
	//		_vec3	vAtkObjPos = pDest->Get_Position();
	//		vAtkObjPos.y = Get_Position().y;
	//		_vec3	vDir = Get_Position() - vAtkObjPos;
	//		D3DXVec3Normalize(&vDir, &vDir);

	//		_vec3	vTargetPos = Get_Position() + (vDir * CWall512::GetInstance()->GetFloatValue(1.5f, 3.f));

	//		Set_KnockBack(vTargetPos);
	//	}

	//	m_iTotalDamage += Get_Damage(static_cast<CAttackObj*>(pDest)->Get_UnitInfo(), static_cast<CAttackObj*>(pDest)->Get_AtkType());

	//	Create_HitEffect();

	//	CUtility::Print_Wstring_DMode(L"Player Hitted");
	//}

	Bomb(); 
}
