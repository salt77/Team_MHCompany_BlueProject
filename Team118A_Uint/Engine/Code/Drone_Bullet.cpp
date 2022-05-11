#include "Drone_Bullet.h"

#include "Scene.h"
#include "Run_Player.h"
#include "Run_Map.h" 
#include "EffectObject.h"
#include "Bullet_Dead.h"
#include "Camera.h" 

#include "Renderer.h"
#include "ProtoMgr.h"
#include "KeyManager.h"
#include "CollisionManager.h" 
#include "TimeScale_Manager.h"
#include "UI_Manager.h" 
#include "Sound_Manager.h" 

USING(Engine)

CDrone_Bullet::CDrone_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CDrone_Bullet::CDrone_Bullet(const CDrone_Bullet & rhs)
	: CAttackObj(rhs)
{
}

CDrone_Bullet::~CDrone_Bullet()
{
}

CDrone_Bullet * CDrone_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fRunAngle, const _vec3& vToFireDir, E_LAYERTAG eTargetLayerTag)
{
	CDrone_Bullet*	pInstance = new CDrone_Bullet(pGraphicDev, eTargetLayerTag); 

	if (FAILED(pInstance->Ready_Object(fRunAngle, vToFireDir)))
		Safe_Release(pInstance); 

	return pInstance;
}

void CDrone_Bullet::Free()
{
	CAttackObj::Free(); 
}

void CDrone_Bullet::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte); 
}

void CDrone_Bullet::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte); 
}


HRESULT CDrone_Bullet::Ready_Object(const _float& fRunAngle, const _vec3& vToFireDir)
{
	CAttackObj::Ready_Object(); 

	Add_Component(); 

	m_bColUseY = TRUE; 

	//Set_Scale(VEC_ONE * 25.f); 
	m_pEffectObject->Set_Effect_Scale(VEC_ONE * 25.f); 

	m_fRunAngle = fRunAngle; 
	m_vToFireDir = vToFireDir; 

	m_fNowLifeTime = 2.f; 

	return S_OK;
}

_int CDrone_Bullet::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta); 

	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player)); 
	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map)); 

	if (!m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CDrone_Bullet -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음"); 
	}

	Set_Scale(VEC_ONE * m_pRunMap->Get_Scale().x * 5.f); 
	m_pSphereColliderCom->SetScale(Get_Scale() * 2.f); 

	m_fSpeed = m_pPlayer->Get_OriginSpeed() * m_fSpeedInterpol;
	m_fDistanceOriginToPipe = m_pPlayer->Get_DistanceOriginToPipeMid();

	return iExit;
}

_int CDrone_Bullet::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	Movement(fTimeDelta); 
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	Check_Dead(); 

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CDrone_Bullet::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	//0205_alpha_pass_change_byEH_랜더 문제시 문의
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this); 

	//CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereColliderCom, E_LAYERTAG::Character, TRUE);

	return iExit;
}

void CDrone_Bullet::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Alpha, iPass));

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CDrone_Bullet::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest); 

	if (E_LAYERTAG::Character == pDest->Get_LayerTag()) 
	{
		static_cast<CRun_Player*>(pDest)->Set_Damage(-(m_fDamageStandard + CWall512::GetInstance()->GetFloatValue(-m_fDamageRandRange, m_fDamageRandRange)));

		CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText();

		CGameObject*	pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_RED);
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
		pGameObject->Set_Pos(Get_Position());

		CCamera::m_pMainCam->Set_ShakeCamera(3.f, 3.f); 
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, 0.75f); 
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SE_Laser_01.wav", 0.75f); 

		m_bDead = TRUE; 
	} 
}

void CDrone_Bullet::Set_Constantable()
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

void CDrone_Bullet::Set_Constantable_Fog(CShader * pShader)
{
}

void CDrone_Bullet::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet.effect");
	assert(m_pEffectObject); 

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
	m_pSphereColliderCom->SetRadius(0.3f); 
	m_pSphereColliderCom->SetScale(Get_Scale()); 
}

void CDrone_Bullet::Movement(const _float & fTimeDelta)
{
	if (!m_pRunMap)
	{
		return; 
	}

	m_fRunAngle += m_fSpeed; 

	_vec3	vDir = VEC_UP; 

	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir); 
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_fDistanceOriginToPipe;
	_vec3	vTargetPos = vPipeMidPos + m_vToFireDir; 

	Set_Pos(vTargetPos); 
}

void CDrone_Bullet::Check_Dead()
{
	if (!m_pPlayer)
	{
		return;
	}

	if (m_pPlayer->Get_RunAngle() - m_fDeadAngle >= m_fRunAngle)
	{
		m_bDead = TRUE; 

		CGameObject*	pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_RED);
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
		pGameObject->Set_Pos(Get_Position());
	}
}
