#include "Shiro_Ball.h"

#include "SphereCollider.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "Unit.h"

USING(Engine)

CShiro_Ball::CShiro_Ball(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CShiro_Ball::CShiro_Ball(const CShiro_Ball & rhs)
	: CAttackObj(rhs)
{
}

CShiro_Ball::~CShiro_Ball()
{
}

CShiro_Ball * CShiro_Ball::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
{
	CShiro_Ball*	pInstance = new CShiro_Ball(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vDir, vStartPos, tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CShiro_Ball::Free()
{
	CAttackObj::Free();
}

void CShiro_Ball::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CShiro_Ball::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CShiro_Ball::Ready_Object(const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType)
{
	CAttackObj::Ready_Object();

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	Add_Component();

	Set_Scale(_vec3(7.f, 7.f, 7.f));

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);
	m_vDir = vDir;

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pSphereColliderCom->SetRadius(5.f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	return S_OK;
}

_int CShiro_Ball::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CShiro_Ball::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	Movement(fTimeDelta);

	return iExit;
}

_int CShiro_Ball::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CShiro_Ball::Render_Object(_int iPass)
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

void CShiro_Ball::Debug_This_In_Console()
{
}

void CShiro_Ball::Set_Constantable()
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

void CShiro_Ball::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// buffer
	m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_DMesh, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Shiro_Bomb_S")));

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CShiro_Ball::Movement(const _float & fTimeDelta)
{
	m_fCountingTime += fTimeDelta;

	if (3.f <= m_fCountingTime)
	{
		m_bDead = TRUE;

		return;
	}

	if (m_bLanding)
	{
		m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
		m_pTransformCom->Add_Rotation(E_ROTATION::ROT_X, D3DXToRadian(360.f));
		m_pTransformCom->Add_Rotation(E_ROTATION::ROT_Z, D3DXToRadian(360.f));
	}
	else
	{
		_vec3	vMyPos = Get_Position();
		_float	fY = vMyPos.y;

		if (0.1f >= vMyPos.y)
		{
			m_bLanding = TRUE;
			Set_Pos(E_POSITION::POS_Y, 1.f);

			return;
		}

		fY = m_vStartPos.y - (m_fFallSpeed * m_fTime + (0.5f * GR * m_fTime * m_fTime));
		m_fTime -= 0.25f;
		m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);

		vMyPos = Get_Position();
		vMyPos.y = fY;
		Set_Pos(vMyPos);
	}
}

void CShiro_Ball::OnEnable()
{
	CAttackObj::OnEnable();
}

void CShiro_Ball::OnDisable()
{
	CAttackObj::OnDisable();
}

void CShiro_Ball::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CShiro_Ball::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest);

	if (E_LAYERTAG::Character == pDest->Get_LayerTag())
	{
		_vec3	vTargetObjPos = pDest->Get_Position();
		_vec3	vMyPos = Get_Position();
		vMyPos.y = vTargetObjPos.y; 
		_vec3	vDir = vTargetObjPos - vMyPos;
		D3DXVec3Normalize(&vDir, &vDir);

		vTargetObjPos = vTargetObjPos + (vDir * CWall512::GetInstance()->GetFloatValue(1.5f, 3.f));

		static_cast<CUnit*>(pDest)->Set_KnockBack(vTargetObjPos); 
		static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

		CUtility::Print_Wstring_DMode(L"Character가 시로 공에 맞음");
	}

	/*if (E_LAYERTAG::AttackObj_Monster == pDest->Get_LayerTag())
	{
		if (E_ATKOBJ_TYPE::ATKOBJ_BULLET_NORMAL == static_cast<CAttackObj*>(pDest)->Get_AtkObjType())
		{
			pDest->Set_Dead(TRUE);
		}
		else if (E_ATKOBJ_TYPE::ATKOBJ_SHIRO_BALL == static_cast<CAttackObj*>(pDest)->Get_AtkObjType())
		{
			_vec3	vAtkObjPos = pDest->Get_Position();
			vAtkObjPos.y = Get_Position().y;
			_vec3	vDir = Get_Position() - vAtkObjPos;
			D3DXVec3Normalize(&vDir, &vDir);

			_vec3	vTargetPos = Get_Position() + (vDir * CWall512::GetInstance()->GetFloatValue(1.5f, 3.f));

			Set_KnockBack(vTargetPos);
		}

		m_iTotalDamage += Get_Damage(static_cast<CAttackObj*>(pDest)->Get_UnitInfo(), static_cast<CAttackObj*>(pDest)->Get_AtkType());

		Create_HitEffect();

		CUtility::Print_Wstring_DMode(L"Player Hitted");
	}*/
}
