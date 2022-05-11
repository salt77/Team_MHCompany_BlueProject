#include "Bullet_Run_Normal.h"

#include "EffectObject.h"
#include "ParticleObject.h"
#include "Run_Map.h"
#include "Run_Chesed.h"
#include "Run_Player.h" 
#include "Bullet_Dead.h" 

#include "CollisionManager.h"
#include "UI_Manager.h" 

CBullet_Run_Normal::CBullet_Run_Normal(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CBullet_Run_Normal::CBullet_Run_Normal(const CBullet_Run_Normal & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Run_Normal::~CBullet_Run_Normal()
{
}

CBullet_Run_Normal * CBullet_Run_Normal::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												const _float& fSpeed,
												const _float& fRunAngle,
												const _vec3& vMidToFireDir,
												const _float& fOriToMidDis,
												const _bool& bIsBossExist,
												E_LAYERTAG eTargetLayerTag)
{
	CBullet_Run_Normal*		pInstance = new CBullet_Run_Normal(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(fSpeed, fRunAngle, vMidToFireDir, fOriToMidDis, bIsBossExist)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Run_Normal::Free()
{
	if (m_pParticleBullet)
	{
		m_pParticleBullet->Set_Dead(TRUE); 
		m_pParticleBullet = nullptr; 
	}

	CAttackObj::Free();
}

void CBullet_Run_Normal::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Run_Normal::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Run_Normal::Ready_Object(const _float& fSpeed,
										 const _float& fRunAngle,
										 const _vec3& vMidToFireDir,
										 const _float& fOriToMidDis,
										 const _bool& bIsBossExist)
{
	CAttackObj::Ready_Object();

	Add_Component();

	m_bColUseY = TRUE; 

	m_fScale = 120.f;
	Set_Scale(VEC_ONE * m_fScale);
	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Scale(VEC_ONE * m_fScale);
	}
	m_pSphereColliderCom->SetScale(VEC_ONE * m_fScale * Half);

	m_bBossTarget = bIsBossExist;
	m_fSpeed = fSpeed * m_fSpeedInterpol;
	m_fRunAngle = fRunAngle;
	m_fOriginToPipeMidDistance = fOriToMidDis;
	m_vPipeMidToFireDir = vMidToFireDir * 0.9f;

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture); 

	m_iDamage = m_iDamage + CWall512::GetInstance()->GetValue(-m_iDamageRandRange, m_iDamageRandRange); 

	m_fNowLifeTime = 1.5f;

	return S_OK;
}

_int CBullet_Run_Normal::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)) 
	{
		m_pChesed = static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster));
	}

	if (!m_pRunMap || !m_pPlayer)
	{
		MSG_BOX("CBullet_Run_Normal -> Start_Object() : m_pRunMap 또는 m_pPlayer 없음");
	}

	m_pParticleBullet = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Momoi_Bullet.particle");
	m_pParticleBullet->Set_Pos(Get_Position());
	m_pParticleBullet->Set_Scale(VEC_ONE * m_fScale * 2.f);
	//m_pParticleBullet->AddRef();
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleBullet);

	if (m_pChesed)
	{
		m_fGapAngleToPlayerAndChesed = fabs(m_pPlayer->Get_RunAngle() - m_pChesed->Get_RunAngle());
	}
	
	m_fStartPipeAngle = m_pPlayer->Get_PipeAngle();

	return iExit;
}

_int CBullet_Run_Normal::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta); 

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	if (m_pParticleBullet)
	{
		m_pParticleBullet->Set_Pos(Get_Position());
	} 

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CBullet_Run_Normal::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta); 

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_EFFECT, this); 

	CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereColliderCom, E_LAYERTAG::AttackObj_Monster, m_bColUseY); 

	return iExit;
}

void CBullet_Run_Normal::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBullet_Run_Normal::On_Collision(CGameObject * pDest)
{
	if (E_LAYERTAG::Monster == pDest->Get_LayerTag())
	{
		m_bDead = TRUE;

		if (m_pEffectObject)
		{
			m_pEffectObject->Set_Dead(TRUE); 
		}
		if (m_pParticleBullet)
		{
			m_pParticleBullet->Set_Dead(TRUE); 
		}

		Dead_Bullet();

		CUI_Manager::GetInstance()->Add_RunComboHit(1); 
		if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player)) 
		{
			static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Add_SkillGage(0.015f);
		}
		CCamera::m_pMainCam->Set_ShakeCamera(0.35f, 0.25f); 

		if (E_OBJTAG::OBJ_Run_BossMonster == pDest->Get_ObjTag()) 
		{
			static_cast<CRun_Chesed*>(pDest)->Set_Damage(-(_float)m_iDamage); 
		}
		else if (E_OBJTAG::OBJ_Run_Monster == pDest->Get_ObjTag()) 
		{
			static_cast<CRun_Unit*>(pDest)->Set_Damage(-(_float)m_iDamage);
		}

		if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player)) 
		{
			static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Add_SkillGage(0.08f); 
		}
	} 
}

void CBullet_Run_Normal::Add_Component()
{
	CComponent*		pComponent = nullptr; 

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
	m_pSphereColliderCom->SetRadius(0.3f);
}

void CBullet_Run_Normal::Set_Constantable()
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

void CBullet_Run_Normal::Movement(const _float & fTimeDelta)
{
	if (!m_pRunMap || !m_pPlayer)
	{
		return;
	}

	if (!m_bBossTarget)
	{
		_vec3	vMidPos = VEC_UP;
		_vec3	vBulletPos = VEC_ZERO;

		m_fRunAngle += m_fSpeed;

		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vMidPos);
		vMidPos *= m_fOriginToPipeMidDistance;

		vBulletPos += vMidPos + m_vPipeMidToFireDir;

		Set_Pos(vBulletPos); 
	}
	else
	{
		if (!m_pChesed)
		{
			return;
		}

		_vec3	vDir = VEC_UP;
		CUtility::Rotate_DirVector(m_pChesed->Get_RunAngle() - m_fGapAngleToPlayerAndChesed, VEC_RIGHT, &vDir);
		_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
		_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir);
		_vec3	vVirtualPlayerShotPos = vPipeMidPos + CUtility::Rotate_DirVector(m_fStartPipeAngle, vAxisZ, &vDir) * m_pPlayer->Get_DistancePipeRadius();

		m_fDistanceRatio -= m_fRatioSpeed * fTimeDelta;
		m_vVirtualShotDir = vVirtualPlayerShotPos - m_pChesed->Get_Position();

		Set_Pos(m_pChesed->Get_Position() + m_vVirtualShotDir * m_fDistanceRatio); 

		if (m_pParticleBullet)
		{
			m_pParticleBullet->Set_Look(m_pChesed->Get_Position());
		}
	}

	_vec3	vMyScale = Get_Scale(); 
	vMyScale.z += m_fScaleUpSpeed * fTimeDelta; 
	Set_Scale(vMyScale); 
}

void CBullet_Run_Normal::Dead_Bullet()
{
	CGameObject*	pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_PINK); 
	assert(pGameObject); 
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject); 
	pGameObject->Set_Pos(Get_Position()); 
}
