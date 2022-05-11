#include "Chesed_Direct_Laser.h"

#include "Run_Player.h" 
#include "Run_Map.h" 
#include "Run_Chesed.h"
#include "EffectObject.h"
#include "Scene.h"

#include "KeyManager.h" 
#include "CollisionManager.h" 
#include "UI_Manager.h"
#include "TimeScale_Manager.h" 

USING(Engine)

Chesed_Direct_Laser::Chesed_Direct_Laser(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

Chesed_Direct_Laser::Chesed_Direct_Laser(const Chesed_Direct_Laser & rhs)
	: CAttackObj(rhs)
{
}

Chesed_Direct_Laser::~Chesed_Direct_Laser()
{
}

Chesed_Direct_Laser * Chesed_Direct_Laser::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
{
	Chesed_Direct_Laser*	pInstance = new Chesed_Direct_Laser(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void Chesed_Direct_Laser::Free()
{
	if (m_pDirectLaserParticle)
	{
		m_pDirectLaserParticle->Set_Dead(TRUE); 
		m_pDirectLaserParticle = nullptr; 
	}
	if (m_pSparkParticle)
	{
		m_pSparkParticle->Set_ParticleDead();
		m_pSparkParticle->Set_Dead(TRUE); 
		m_pSparkParticle = nullptr; 
	}

	CAttackObj::Free();
}

void Chesed_Direct_Laser::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void Chesed_Direct_Laser::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT Chesed_Direct_Laser::Ready_Object()
{
	CAttackObj::Ready_Object();

	Add_Component();

	m_bColUseY = TRUE; 

	m_fScale = 50.f;
	Set_Scale(VEC_ONE * m_fScale);
	m_pSphereColliderCom->SetScale(Get_Scale()); 

	m_fNowLifeTime = 1.f;

	return S_OK;
}

_int Chesed_Direct_Laser::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	// 추후 몬스터 늘어날 때 대비해서 수정 필요함 
	m_pChesed = static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster));
	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (!m_pChesed || !m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CChesed_Direct_Laser -> Start_Object() : m_pChesed 또는 pPlayer 또는 m_pRunMap 없음");
	}

	m_pDirectLaserParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Laser.particle"); 
	m_pDirectLaserParticle->Set_Pos(Get_Position());
	m_pDirectLaserParticle->Set_Scale(VEC_ONE * m_fScale);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pDirectLaserParticle);

	m_pSparkParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Laser_Spark.particle"); 
	m_pSparkParticle->Set_Pos(VEC_ZERO);
	m_pSparkParticle->Set_Scale(VEC_ONE * m_fScale * 3.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pSparkParticle);

	m_fTargetRunAngle = m_pPlayer->Get_RunAngle() + m_fFrontTargetRunAngle;
	m_fTargetPipeAngle = m_pPlayer->Get_PipeAngle();

	return iExit;
}

_int Chesed_Direct_Laser::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta); 

	Movement(fTimeDelta);
	Toward_Player(fTimeDelta);

	if (m_pDirectLaserParticle)
	{
		m_pDirectLaserParticle->Set_Pos(m_pChesed->Get_Position()); 
	} 
	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int Chesed_Direct_Laser::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_EFFECT, this); 

	return iExit;
}

void Chesed_Direct_Laser::Render_Object(_int iPass)
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

void Chesed_Direct_Laser::On_Collision(CGameObject * pDest) 
{
	CAttackObj::On_Collision(pDest); 

	if (E_LAYERTAG::Character == pDest->Get_LayerTag()) 
	{
		static_cast<CRun_Player*>(pDest)->Set_Damage(-(m_fDamageStandard + CWall512::GetInstance()->GetFloatValue(-m_fDamageRandRange, m_fDamageRandRange))); 
		CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText(); 
		CCamera::m_pMainCam->Set_ShakeCamera(3.f, 3.f);
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, 0.75f);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Laser_Hit.wav", 2.f);
	}
}

void Chesed_Direct_Laser::Set_Constantable()
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

void Chesed_Direct_Laser::Set_Constantable_Fog(CShader * pShader)
{
}

void Chesed_Direct_Laser::Add_Component()
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

void Chesed_Direct_Laser::Movement(const _float & fTimeDelta)
{
	if (!m_pChesed)
	{
		return;
	}

	Set_Pos(m_pChesed->Get_WeaponPos());
}

void Chesed_Direct_Laser::Toward_Player(const _float& fTimeDelta)
{
	if (!m_pPlayer)
	{
		return;
	}

	_vec3	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fTargetRunAngle, VEC_RIGHT, &vDir);
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir);
	CUtility::Rotate_DirVector(m_fTargetPipeAngle, vAxisZ, &vDir);

	_vec3	vTargetPos = vPipeMidPos + vDir * m_pPlayer->Get_DistancePipeRadius();

	Set_Look(vTargetPos);

	if (m_pDirectLaserParticle)
	{
		m_pDirectLaserParticle->Set_Look(vTargetPos);
	}

	_matrix		matScale, matTrans, matTargetWorld;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixScaling(&matScale, Get_Scale().x, Get_Scale().y, Get_Scale().z);
	D3DXMatrixTranslation(&matTrans, vTargetPos.x, vTargetPos.y, vTargetPos.z);

	matTargetWorld = matScale * matTrans;

	if (m_pSparkParticle)
	{
		m_pSparkParticle->Set_Pos(vTargetPos);
	}

	m_pSphereColliderCom->SetMatrix(matTargetWorld);

	if (m_pPlayer->Get_RunAngle() + m_fBackTargetRunAngle <= m_fTargetPipeAngle)
	{
		m_fTargetPipeAngle -= 5.f * fTimeDelta;
	} 
}
