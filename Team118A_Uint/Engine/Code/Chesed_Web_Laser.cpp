#include "Chesed_Web_Laser.h"

#include "Run_Player.h" 
#include "Run_Map.h" 
#include "Run_Chesed.h"
#include "EffectObject.h"
#include "Scene.h"
#include "Camera.h" 

#include "ProtoMgr.h" 
#include "KeyManager.h" 
#include "TimeScale_Manager.h" 
#include "CollisionManager.h" 
#include "UI_Manager.h" 

USING(Engine)

Chesed_Web_Laser::Chesed_Web_Laser(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

Chesed_Web_Laser::Chesed_Web_Laser(const Chesed_Web_Laser & rhs)
	: CAttackObj(rhs)
{
}

Chesed_Web_Laser::~Chesed_Web_Laser()
{
}

Chesed_Web_Laser * Chesed_Web_Laser::Create(LPDIRECT3DDEVICE9 pGraphicDev, const E_WebLaser_AtkType& eLaserType, E_LAYERTAG eTargetLayerTag)
{
	Chesed_Web_Laser*	pInstance = new Chesed_Web_Laser(pGraphicDev, eTargetLayerTag); 

	if (FAILED(pInstance->Ready_Object(eLaserType)))
		Safe_Release(pInstance); 

	return pInstance;
}

void Chesed_Web_Laser::Free()
{
	if (m_pParticleWebLaser)
	{
		m_pParticleWebLaser->Set_ParticleDead(); 
		m_pParticleWebLaser->Set_Dead(TRUE); 
		m_pParticleWebLaser = nullptr; 
		//Safe_Release(m_pParticleWebLaser);
	}
	if (m_pSparkParticle_1)
	{
		m_pSparkParticle_1->Set_ParticleDead();
		m_pSparkParticle_1->Set_Dead(TRUE);
		m_pSparkParticle_1 = nullptr;
		//Safe_Release(m_pSparkParticle_1); 
	}
	if (m_pSparkParticle_2)
	{
		m_pSparkParticle_2->Set_ParticleDead();
		m_pSparkParticle_2->Set_Dead(TRUE);
		m_pSparkParticle_2 = nullptr;
		//Safe_Release(m_pSparkParticle_2); 
	}

	CAttackObj::Free(); 
}

void Chesed_Web_Laser::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte); 
}

void Chesed_Web_Laser::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte); 
}


HRESULT Chesed_Web_Laser::Ready_Object(const E_WebLaser_AtkType& eLaserType)
{
	CAttackObj::Ready_Object(); 

	Add_Component(); 

	m_bColUseY = TRUE; 
	m_fScale = 50.f; 
	m_fNowLifeTime = 8.f; 
	m_eWebLaserType = eLaserType; 

	if (WebLaser_Rotate == m_eWebLaserType || 
		WebLaser_Cross == m_eWebLaserType)
	{
		m_iRotate = CWall512::GetInstance()->GetValue(0, 2); 
	}

	Set_Scale(VEC_ONE * m_fScale);
	if (m_pSphereColliderCom)
	{
		m_pSphereColliderCom->SetScale(Get_Scale() * Half);
	}
	if (m_pSphereColliderCom_2)
	{
		m_pSphereColliderCom_2->SetScale(Get_Scale() * Half);
	}

	return S_OK;
}

_int Chesed_Web_Laser::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta); 

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)) 
	{
		m_pChesed = static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster));
	}

	if (!m_pChesed || !m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CChesed_Web_Laser -> Start_Object() : m_pChesed 또는 pPlayer 또는 m_pRunMap 없음"); 
	}

	m_iReverse = CWall512::GetInstance()->GetValue(0, 2); 

	m_fRunAngle = m_pChesed->Get_RunAngle() + m_fSpawnInterpolAngle; 
	m_vAxisZ = m_pChesed->Get_VecAxis(E_ROTATION::ROT_Z); 
	m_fPipeAngle = CWall512::GetInstance()->GetFloatValue(0.f, 359.f); 
	m_fPipeOriginAngle = m_fPipeAngle; 
	m_fDistanceFromPipeMidLook = CWall512::GetInstance()->GetFloatValue(0.f, m_pPlayer->Get_DistancePipeRadius() * 0.9f); 

	m_pParticleWebLaser = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Web_Laser.particle");
	//m_pParticleWebLaser->AddRef(); 
	m_pParticleWebLaser->Set_Pos(Get_Position());
	m_pParticleWebLaser->Set_Scale(VEC_ONE * m_fScale);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleWebLaser); 

	m_pSparkParticle_1 = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Laser_Spark_LowQuality_2.particle");
	//m_pSparkParticle_1->AddRef(); 
	m_pSparkParticle_1->Set_Pos(Get_Position());
	m_pSparkParticle_1->Set_Scale(VEC_ONE * m_fScale * 5.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pSparkParticle_1); 

	m_pSparkParticle_2 = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Laser_Spark_LowQuality_2.particle");
	//m_pSparkParticle_2->AddRef(); 
	m_pSparkParticle_2->Set_Pos(Get_Position());
	m_pSparkParticle_2->Set_Scale(VEC_ONE * m_fScale * 5.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pSparkParticle_2);

	_vec3	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir);
	CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vDir);

	Set_Pos(vPipeMidPos + (m_iReverse ? vDir : -vDir) * m_pPlayer->Get_DistancePipeRadius() * 2.f);
	m_pParticleWebLaser->Set_Pos(vPipeMidPos + (m_iReverse ? vDir : -vDir) * m_pPlayer->Get_DistancePipeRadius() * 2.f);
	m_pSparkParticle_1->Set_Pos(VEC_ZERO);
	m_pSparkParticle_2->Set_Pos(VEC_ZERO);

	return iExit;
}

_int Chesed_Web_Laser::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta); 

	Ready_Attack(fTimeDelta); 
	Check_Dead(); 

	//m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	if (m_pEffectObject) 
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	if (m_pPlayer)
	{
		CSound_Manager::GetInstance()->PlaySound_(L"빔1.wav", E_ChannelID::Channel2_25, E_SoundType::FX, (1 / D3DXVec3Length(&(Get_Position() - m_pPlayer->Get_Position()))) + Half);
	}

	return iExit;
}

_int Chesed_Web_Laser::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	//0205_alpha_pass_change_byEH_랜더 문제시 문의
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this); 

	//if (m_pSphereColliderCom_2)
	//{
	//	CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereColliderCom_2, E_LAYERTAG::Character, TRUE);
	//}

	return iExit;
}

void Chesed_Web_Laser::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Alpha, iPass)); 

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		if (m_pSphereColliderCom)
		{
			m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
		}
		if (m_pSphereColliderCom_2)
		{
			m_pSphereColliderCom_2->Render_Collider(m_pShaderCom->Get_EffectHandle());
		}
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void Chesed_Web_Laser::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest); 

	if (E_LAYERTAG::Character == pDest->Get_LayerTag()) 
	{ 
		m_bHitPlayer = TRUE; 
		static_cast<CRun_Player*>(pDest)->Set_Damage(-(m_fDamageStandard + CWall512::GetInstance()->GetFloatValue(-m_fDamageRandRange, m_fDamageRandRange)));
		CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText(); 

		CCamera::m_pMainCam->Set_ShakeCamera(3.f, 3.f);
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, 0.75f); 
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Laser_Hit.wav", 2.f); 
	} 
}

void Chesed_Web_Laser::Set_Constantable()
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

void Chesed_Web_Laser::Set_Constantable_Fog(CShader * pShader)
{
}

void Chesed_Web_Laser::Add_Component()
{
	CComponent*		pComponent = nullptr; 

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	//// Collider
	//pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	//assert(pComponent);
	//m_mapComponent.emplace(ComTag_Collider, pComponent);
	//m_pSphereColliderCom->SetRadius(1.f);

	//pComponent = m_pSphereColliderCom_2 = CSphereCollider::Create(m_pGraphicDev, this);
	//assert(pComponent);
	//m_mapComponent.emplace(L"Com_Collider_2", pComponent);
	//m_pSphereColliderCom_2->SetRadius(1.f);
}

void Chesed_Web_Laser::Ready_Attack(const _float& fTimeDelta)
{
	if (!m_pChesed || !m_pParticleWebLaser || !m_pSparkParticle_1 || !m_pSparkParticle_2)
	{
		return; 
	}

	if (0.f < m_fReadyTime)
	{
		m_fReadyTime -= fTimeDelta;
		m_fRunAngle += m_pPlayer->Get_Speed() * CTimeScale_Manager::GetInstance()->Get_TimeScale();
	}

	_vec3	vStartLaserPos = VEC_ZERO;
	_vec3	vTargetLookPos = VEC_ZERO;

	_vec3	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir);
	_vec3	vMoveDir = vDir;
	CUtility::Rotate_DirVector(m_fPipeOriginAngle, vAxisZ, &vDir);

	if (E_WebLaser_AtkType::WebLaser_Normal == m_eWebLaserType)
	{
		m_fOtherReadyTime -= fTimeDelta;

		if (0.f >= m_fOtherReadyTime)
		{
			m_fPipeAngle += m_fOtherMoveSpeed * fTimeDelta;
		}

		vTargetLookPos = vPipeMidPos + (m_iReverse ? vDir : -vDir) * m_fDistanceFromPipeMidLook;
		vStartLaserPos = vPipeMidPos + (m_iReverse ? vDir : -vDir) * m_fDistanceFromPipeMidLook + *D3DXVec3Cross(&m_vOutTemp, &VEC_RIGHT, &vAxisZ) * m_pPlayer->Get_DistancePipeRadius() * 2.f;
		Set_Pos(vStartLaserPos);
		m_pParticleWebLaser->Set_Pos(vStartLaserPos);
		Set_Look(vTargetLookPos);
		m_pParticleWebLaser->Set_Look(vTargetLookPos);
	}
	else if (E_WebLaser_AtkType::WebLaser_Rotate == m_eWebLaserType)
	{
		m_fOtherReadyTime -= fTimeDelta;

		if (0.f >= m_fOtherReadyTime)
		{
			if (0 == m_iRotate)
			{
				m_fPipeAngle -= m_fOtherMoveSpeed * fTimeDelta * 5.f;
			}
			else
			{
				m_fPipeAngle += m_fOtherMoveSpeed * fTimeDelta * 5.f;
			}
		}

		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vMoveDir);

		vTargetLookPos = vPipeMidPos + vDir * m_fDistanceFromPipeMidLook;
		if (0 == m_iRotate)
		{
			vStartLaserPos = vPipeMidPos + vMoveDir * m_fDistanceFromPipeMidLook + *D3DXVec3Cross(&m_vOutTemp, &VEC_RIGHT, &vAxisZ) * m_pPlayer->Get_DistancePipeRadius() * 2.f;
		}
		else
		{
			vStartLaserPos = vPipeMidPos + vMoveDir * m_fDistanceFromPipeMidLook - *D3DXVec3Cross(&m_vOutTemp, &VEC_RIGHT, &vAxisZ) * m_pPlayer->Get_DistancePipeRadius() * 2.f;
		}

		Set_Pos(vStartLaserPos);
		m_pParticleWebLaser->Set_Pos(vStartLaserPos);
		Set_Look(vTargetLookPos);
		m_pParticleWebLaser->Set_Look(vTargetLookPos);
	}
	else if (E_WebLaser_AtkType::WebLaser_Cross == m_eWebLaserType)
	{
		m_fOtherReadyTime -= fTimeDelta;

		if (0.f >= m_fOtherReadyTime && 
			0.f < m_fReadyTime)
		{
			if (0 == m_iRotate)
			{
				m_fPipeAngle -= m_fOtherMoveSpeed * fTimeDelta * m_fCrossRotateSpeed;
			}
			else
			{
				m_fPipeAngle += m_fOtherMoveSpeed * fTimeDelta * m_fCrossRotateSpeed;
			}
		}

		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vDir);

		vStartLaserPos = vPipeMidPos + (m_iReverse ? vDir : -vDir) * m_pPlayer->Get_DistancePipeRadius() * 2.f;

		vTargetLookPos = vPipeMidPos;

		Set_Pos(vStartLaserPos);
		m_pParticleWebLaser->Set_Pos(vStartLaserPos);
		Set_Look(vTargetLookPos);
		m_pParticleWebLaser->Set_Look(vTargetLookPos);
	}

	vTargetLookPos = *D3DXVec3Normalize(&vTargetLookPos, &(vTargetLookPos - Get_Position()));

	_vec3	vWorldStartPos = vStartLaserPos;
	_vec3	vWorldLaserDir = vTargetLookPos;

	// 맵 레이캐스트 
	_vec3	vReverseLaserStartPos = vWorldStartPos + vTargetLookPos * 99999.f; 
	_vec3	vReverseWorldPos = vReverseLaserStartPos; 

	_matrix		matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_pRunMap->Get_Transform()->Get_WorldMatrix());

	D3DXVec3TransformCoord(&vStartLaserPos, &vStartLaserPos, &matWorld);
	D3DXVec3TransformCoord(&vReverseLaserStartPos, &vReverseLaserStartPos, &matWorld); 
	D3DXVec3TransformNormal(&vTargetLookPos, &vTargetLookPos, &matWorld);

	BOOL	bHit = 0; 
	BOOL	bHit2 = 0;
	_float	fDistance_1 = 0.f; 
	_float	fDistance_2 = 0.f; 
	D3DXIntersect(m_pRunMap->Get_Mesh(), &vStartLaserPos, &vTargetLookPos, &bHit, nullptr, nullptr, nullptr, &fDistance_1, nullptr, nullptr);
	D3DXIntersect(m_pRunMap->Get_Mesh(), &vReverseLaserStartPos, &-vTargetLookPos, &bHit2, nullptr, nullptr, nullptr, &fDistance_2, nullptr, nullptr);

	m_fReverseLaserArriveDelay -= fTimeDelta; 

	if (bHit && bHit2) 
	{
		if (m_pSparkParticle_1 && m_pSparkParticle_2) 
		{
			_vec3	vPos = vWorldStartPos + vWorldLaserDir * fDistance_1; 
			m_pSparkParticle_1->Set_Pos(vPos);

			if (0.f >= m_fReverseLaserArriveDelay)
			{
				vPos = vReverseWorldPos - vWorldLaserDir * fDistance_2; 

				m_pSparkParticle_2->Set_Pos(vPos); 
			} 
		}
	}

	if (!m_bHitPlayer)
	{
		//// 플레이어 콜라이더 레이캐스트 
		//CCollisionManager::GetInstance()->Collision_RayToSphere(this,
		//	vWorldStartPos,
		//	vWorldLaserDir,
		//	99999.f,
		//	E_LAYERTAG::Character,
		//	Collision_Raycast);

		_matrix		matPlayerColWorld;
		CSphereCollider*	pPlayerCol = static_cast<CSphereCollider*>(m_pPlayer->Get_Component(ComTag_Collider));
		D3DXMatrixInverse(&matPlayerColWorld, NULL, &pPlayerCol->Get_WorldMatrix());

		D3DXVec3TransformCoord(&vWorldStartPos, &vWorldStartPos, &matPlayerColWorld);
		D3DXVec3TransformNormal(&vWorldLaserDir, &vWorldLaserDir, &matPlayerColWorld);

		BOOL	bPlayerHit = 0;
		_float	fPlayerDistance = 0.f;
		D3DXIntersect(pPlayerCol->Get_MeshCollider(), &vWorldStartPos, &vWorldLaserDir, &bPlayerHit, nullptr, nullptr, nullptr, &fPlayerDistance, nullptr, nullptr);

		if (bPlayerHit)
		{
			On_Collision(m_pPlayer);
		}
	}
}

void Chesed_Web_Laser::Collision_Raycast(CGameObject * pThis, CGameObject * pPlayer)
{
	static_cast<CRun_Player*>(pPlayer)->Set_Damage(-(CWall512::GetInstance()->GetFloatValue(1.f, 3.5f))); 
	CCamera::m_pMainCam->Set_ShakeCamera(3.f, 1.f); 
	CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.5f, 0.5f); 
	CUI_Manager::GetInstance()->Active_RunHurtScreenUI(); 
	CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText(); 
}

void Chesed_Web_Laser::Check_Dead()
{
	if (!m_pPlayer) 
	{
		return; 
	}

	if (m_pParticleWebLaser &&
		m_fRunAngle <= m_pPlayer->Get_RunAngle() - 10.f)
	{
		Set_Dead(TRUE); 
		m_pParticleWebLaser->Set_ParticleDead();
		m_pParticleWebLaser->Set_Dead(TRUE); 
		m_pParticleWebLaser = nullptr; 
		//Safe_Release(m_pParticleWebLaser);
		m_pSparkParticle_1->Set_ParticleDead(); 
		m_pSparkParticle_1->Set_Dead(TRUE); 
		m_pSparkParticle_1 = nullptr; 
		//Safe_Release(m_pSparkParticle_1);
		m_pSparkParticle_2->Set_ParticleDead(); 
		m_pSparkParticle_2->Set_Dead(TRUE); 
		m_pSparkParticle_2 = nullptr; 
		//Safe_Release(m_pSparkParticle_2);
	}
}
