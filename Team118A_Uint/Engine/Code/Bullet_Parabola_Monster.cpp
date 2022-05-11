#include "Bullet_Parabola_Monster.h" 

#include "EffectObject.h"
#include "ParticleObject.h"
#include "Unit.h"

#include "CollisionManager.h"

CBullet_Parabola_Monster::CBullet_Parabola_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CBullet_Parabola_Monster::CBullet_Parabola_Monster(const CBullet_Parabola_Monster & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Parabola_Monster::~CBullet_Parabola_Monster()
{
}

CBullet_Parabola_Monster * CBullet_Parabola_Monster::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType)
{
	CBullet_Parabola_Monster*		pInstance = new CBullet_Parabola_Monster(pGraphicDev, E_LAYERTAG::Character);

	if (FAILED(pInstance->Ready_Object(vPos, vTargetPos, vDir, tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Parabola_Monster::Free()
{
	CAttackObj::Free();
}

void CBullet_Parabola_Monster::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Parabola_Monster::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Parabola_Monster::Ready_Object(_vec3 vPos, _vec3 vTargetPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType)
{
	CAttackObj::Ready_Object(); 

	Add_Component();

	Set_Pos(vPos);
	Set_Scale(VEC_ONE);

	m_vTargetPos = vTargetPos;
	m_vDir = vDir; 
	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	m_fDistance = D3DXVec3Length(&(m_vTargetPos - vPos)); 

	return S_OK;
}

_int CBullet_Parabola_Monster::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CBullet_Parabola_Monster::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}
	if (m_pParticle)
	{
		m_pParticle->Set_Pos(Get_Position());
		m_pParticle->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CBullet_Parabola_Monster::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}
	if (m_pParticle)
	{
		m_pParticle->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CBullet_Parabola_Monster::Render_Object(_int iPass)
{
	if (Get_Dead() || 0.f >= Get_Position().y)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	if (0 == iPass &&
		m_pSphereColliderCom)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBullet_Parabola_Monster::On_Collision(CGameObject * pDest)
{
	if (static_cast<CUnit*>(pDest)->Get_UnitDead())
	{
		return;
	}

	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType()); 

	m_bDead = TRUE;

	return;
}

void CBullet_Parabola_Monster::Add_Component()
{
	m_pEffectObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Flash_Frag.effect"); 
	assert(m_pEffectObject); 
	m_pEffectObject->Set_Effect_Scale(VEC_ONE * 2.f); 

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	//m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CBullet_Parabola_Monster::Set_Constantable()
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

void CBullet_Parabola_Monster::Movement(const _float & fTimeDelta)
{
	_vec3	vMyPos = Get_Position();
	_float	fY = vMyPos.y;

	fY = m_vStartPos.y - (m_fFallSpeed * m_fTime + (Half * GR * m_fTime * m_fTime));
	m_fTime -= 20.f * fTimeDelta; 

	Set_Pos(vMyPos + (m_vDir * (m_fDistance / m_fArriveTime) * fTimeDelta));

	vMyPos = Get_Position();
	vMyPos.y = fY;

	Set_Pos(vMyPos);

	if (0.f >= vMyPos.y && 
		!m_bCollisionLand)
	{
		m_bCollisionLand = TRUE; 
		vMyPos.y = 0.f; 

		CSkill_Manager::GetInstance()->SetSkill(L"Flash.skill", E_LAYERTAG::Effect, vMyPos, VEC_ZERO, 2.f); 

		if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
		{
			vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player); 

			for (_uint i = 0; i < vecObject->size(); ++i) 
			{
				if (2.f >= CUtility::Distance_vec3(Get_Position(), (*vecObject)[i]->Get_Position()))
				{
					static_cast<CUnit*>((*vecObject)[i])->Set_Damage(m_tUnitInfo, m_eAtkType); 
					static_cast<CUnit*>((*vecObject)[i])->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Stunned, 3.f)); 
				}
			}
		} 
	}

	if (5.f <= m_fCountingTime) 
	{
		CUtility::Print_Wstring_DMode(L"Monster_Bullet_Parabola Dead");

		m_bDead = true;
	}
}
