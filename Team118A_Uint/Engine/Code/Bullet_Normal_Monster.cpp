#include "Bullet_Normal_Monster.h"

#include "EffectObject.h"
#include "ParticleObject.h"
#include "Unit.h"

#include "CollisionManager.h"

CBullet_Normal_Monster::CBullet_Normal_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CBullet_Normal_Monster::CBullet_Normal_Monster(const CBullet_Normal_Monster & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Normal_Monster::~CBullet_Normal_Monster()
{
}

CBullet_Normal_Monster * CBullet_Normal_Monster::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType,
	E_MovementType eMoveType)
{
	CBullet_Normal_Monster*		pInstance = new CBullet_Normal_Monster(pGraphicDev, E_LAYERTAG::Character);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, tUnitInfo, eAtkType, eMoveType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Normal_Monster::Free()
{
	CAttackObj::Free();
}

void CBullet_Normal_Monster::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Normal_Monster::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Normal_Monster::Ready_Object(_vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType, E_MovementType eMoveType)
{
	CAttackObj::Ready_Object();

	m_eMovementType = eMoveType;

	Add_Component();

	Set_Pos(vPos);
	Set_Scale(VEC_ONE);
	m_vDir = vDir;

	if (E_MovementType::Move_FallDown == m_eMovementType)
	{
		m_fSpeed = 25.f;

		if (m_pEffectObject)
		{
			m_pEffectObject->Set_Effect_Rotation(_vec3(90.f, 1.f, 1.f));
		}
	}
	else if (E_MovementType::Move_AutomataBullet == m_eMovementType ||
			 E_MovementType::Move_GoliathEx == m_eMovementType)
	{
		m_fSpeed = 25.f; 

		if (E_MovementType::Move_GoliathEx == m_eMovementType) 
		{
			m_fAttackRange = 5.f; 
		}
	}
	else
	{
		m_fSpeed = 20.f;
	}

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	return S_OK;
}

_int CBullet_Normal_Monster::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CBullet_Normal_Monster::Update_Object(const _float & fTimeDelta)
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

_int CBullet_Normal_Monster::LateUpdate_Object(const _float & fTimeDelta)
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

void CBullet_Normal_Monster::Render_Object(_int iPass)
{
	if (Get_Dead())
	{
		return;
	}

	if ((E_MovementType::Move_FallDown == m_eMovementType || E_MovementType::Move_HeliMissile == m_eMovementType ||
		E_MovementType::Move_AutomataBullet == m_eMovementType) &&
		0.f >= Get_Position().y)
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

void CBullet_Normal_Monster::On_Collision(CGameObject * pDest)
{
	if (static_cast<CUnit*>(pDest)->Get_UnitDead())
	{
		return;
	}

	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

	if (E_MovementType::Move_TankBullet == m_eMovementType ||
		E_MovementType::Move_RocketLauncher == m_eMovementType)
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, pDest->Get_Position(), VEC_ZERO, 2.5f); 
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Bomb_Hit.wav", 0.125f); 
	}

	m_bDead = TRUE;

	return;
}

void CBullet_Normal_Monster::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Collider 
	if (E_MovementType::Move_FallDown != m_eMovementType &&
		E_MovementType::Move_HeliMissile != m_eMovementType && 
		E_MovementType::Move_GoliathEx != m_eMovementType)
	{
		CComponent*		pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
		assert(pComponent);
		m_mapComponent.emplace(ComTag_Collider, pComponent); 
		E_MovementType::Move_AutomataBullet ? m_pSphereColliderCom->SetRadius(0.6f) : m_pSphereColliderCom->SetRadius(0.3f);
		m_pSphereColliderCom->SetScale(Get_Scale());
		m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());
	}

	// 이펙트 또는 파티클 컴포넌트 
	if (E_MovementType::Move_TankBullet == m_eMovementType)
	{
		m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Bullet_Normal.particle");
		assert(m_pParticle);
		m_pParticle->Set_Scale(Get_Scale() * Half);
	}
	else if (E_MovementType::Move_FallDown == m_eMovementType)
	{
		m_pEffectObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Missile.effect");
		assert(m_pEffectObject);
	}
	else if (E_MovementType::Move_RocketLauncher == m_eMovementType ||
			 E_MovementType::Move_HeliMissile == m_eMovementType || 
			 E_MovementType::Move_GoliathEx == m_eMovementType)
	{
		m_pEffectObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Missile.effect");
		assert(m_pEffectObject);

		m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Missile_Fire.particle");
		assert(m_pParticle);
		m_pParticle->Set_Scale(Get_Scale());
	}
	else if (E_MovementType::Move_AutomataBullet == m_eMovementType)
	{
		m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"TerrorAutomata_Bullet.particle");
		assert(m_pParticle);
		//m_pParticle->Set_Scale(Get_Scale()); 
	}

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	//m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CBullet_Normal_Monster::Set_Constantable()
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

void CBullet_Normal_Monster::Movement(const _float & fTimeDelta)
{
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta);

	if (E_MovementType::Move_TankBullet == m_eMovementType ||
		E_MovementType::Move_HeliMissile == m_eMovementType ||
		E_MovementType::Move_AutomataBullet == m_eMovementType ||
		E_MovementType::Move_RocketLauncher == m_eMovementType || 
		E_MovementType::Move_GoliathEx == m_eMovementType)
	{
		m_pParticle->Set_Direction(m_vDir);

		if (m_pEffectObject)
		{
			m_pEffectObject->Set_Direction(m_vDir);
		}
	}

	if (E_MovementType::Move_FallDown == m_eMovementType ||
		E_MovementType::Move_HeliMissile == m_eMovementType ||
		E_MovementType::Move_AutomataBullet == m_eMovementType || 
		E_MovementType::Move_GoliathEx == m_eMovementType)
	{
		if (0.f >= Get_Position().y &&
			!m_bCollisionLand)
		{
			m_bCollisionLand = TRUE;

			_vec3	vPos = Get_Position();
			vPos.y = 0.f;

			if (E_MovementType::Move_FallDown == m_eMovementType)
			{
				CScene::Get_MainScene()->Active_ShakeCam(1.f, 0.1f);
				CSkill_Manager::GetInstance()->SetSkill(L"Test_Explosion.skill", E_LAYERTAG::Effect, vPos, VEC_ZERO, 3.f);
			}
			else if (E_MovementType::Move_HeliMissile == m_eMovementType)
			{
				CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, vPos, VEC_ZERO, 2.5f);
			}
			else if (E_MovementType::Move_GoliathEx == m_eMovementType)
			{
				CScene::Get_MainScene()->Active_ShakeCam(1.5f, 0.2f); 
				CSkill_Manager::GetInstance()->SetSkill(L"Goliath_ChargeEx_Explosion.skill", E_LAYERTAG::Effect, vPos, VEC_ZERO, 5.5f); 
			}

			if (E_MovementType::Move_FallDown == m_eMovementType ||
				E_MovementType::Move_HeliMissile == m_eMovementType || 
				E_MovementType::Move_GoliathEx == m_eMovementType)
			{
				if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
				{
					vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);

					for (_uint i = 0; i < vecObject->size(); ++i)
					{
						if (m_fAttackRange >= D3DXVec3Length(&(vPos - (*vecObject)[i]->Get_Position())))
						{
							static_cast<CUnit*>((*vecObject)[i])->Set_Damage(Get_UnitInfo(), Get_AtkType()); 

							if (E_MovementType::Move_FallDown == m_eMovementType || 
								E_MovementType::Move_GoliathEx == m_eMovementType)
							{
								static_cast<CUnit*>((*vecObject)[i])->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Confusion, 3.f));
							}
						}
					}
				}

				switch (m_eMovementType)
				{
				case Engine::CBullet_Normal_Monster::Move_FallDown:
					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_Explosion_03.wav", 0.7f); 
					break;

				case Engine::CBullet_Normal_Monster::Move_HeliMissile:
					break;

				case Engine::CBullet_Normal_Monster::Move_GoliathEx:
					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Ex_Explosion.wav", 0.8f); 
					break;
				}
			}
			else if (E_MovementType::Move_AutomataBullet == m_eMovementType)
			{
				CGameObject*	pGameObject = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Bullet_GroundDecal.particle");
				assert(pGameObject);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pGameObject);
				pGameObject->Set_Pos(vPos);
			}

			m_bDead = TRUE;
		}
	}

	if (5.f <= m_fCountingTime)
	{
		CUtility::Print_Wstring_DMode(L"Monster_Bullet_Normal Dead");

		m_bDead = true;
	}
}
