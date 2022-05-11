#include "Run_Obstacle.h"

#include "Camera.h"
#include "Run_Map.h" 
#include "Run_Player.h" 
#include "SMesh.h" 

#include "Renderer.h" 
#include "ProtoMgr.h" 
#include "FogManager.h" 
#include "CollisionManager.h" 
#include "TimeScale_Manager.h" 
#include "UI_Manager.h" 
#include "Sound_Manager.h" 

USING(Engine)

CRun_Obstacle::CRun_Obstacle(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CRun_Obstacle::CRun_Obstacle(const CRun_Obstacle & rhs)
	: CAttackObj(rhs)
{
}

CRun_Obstacle::~CRun_Obstacle()
{
}

CRun_Obstacle * CRun_Obstacle::Create(LPDIRECT3DDEVICE9 pGraphicDev,
									  const _float& fRunAngle,
									  E_LAYERTAG eTargetLayerTag)
{
	CRun_Obstacle*	pInstance = new CRun_Obstacle(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(fRunAngle)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRun_Obstacle::Free()
{
	if (m_pParticleBreak)
	{
		if (!m_pParticleBreak->Get_Dead())
		{
			m_pParticleBreak->Set_Dead(TRUE);
		}
		
		m_pParticleBreak = nullptr;
	}

	CAttackObj::Free();
}

void CRun_Obstacle::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CRun_Obstacle::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}

HRESULT CRun_Obstacle::Ready_Object(const _float& fRunAngle)
{
	CAttackObj::Ready_Object();

	Add_Component();

	m_bColUseY = TRUE; 

	m_fRunAngle = fRunAngle + m_fAngleFront;
	m_fPipeAngle = CWall512::GetInstance()->GetFloatValue(0.f, 360.f);

	return S_OK;
}

_int CRun_Obstacle::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (!m_pRunMap || !m_pPlayer)
	{
		MSG_BOX("CRun_Obstacle -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음");

		return iExit;
	}

	m_fScale = m_pRunMap->Get_Scale().x * 50.f; 
	_vec3	vScale = _vec3(m_fScale, m_fScale, m_fScale * Half); 
	Set_Scale(vScale); 
	m_pSphereColliderCom->SetScale(VEC_ONE * m_fScale * 0.35f); 

	_vec3	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir);
	CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vDir);

	m_vAppearDir = -vDir;

	m_vTargetPos = vPipeMidPos + vDir * m_pPlayer->Get_DistancePipeRadius();
	Set_Pos(vPipeMidPos + vDir * m_pPlayer->Get_DistancePipeRadius() * 1.5f);
	Set_Direction(VEC_UP);

	return iExit;
}

_int CRun_Obstacle::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	Appear(fTimeDelta);

	return iExit;
}

_int CRun_Obstacle::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this); 

	return iExit;
}

void CRun_Obstacle::Render_Object(_int iPass)
{
	CAttackObj::Render_Object(iPass);

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(iPass);

	if (m_pSMeshCom)
	{
		m_pSMeshCom->Render_Meshes(m_pShaderCom);
	}

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CRun_Obstacle::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest);

	if (E_LAYERTAG::Character == pDest->Get_LayerTag())
	{
		static_cast<CRun_Player*>(pDest)->Set_Damage(-(m_fDamageStandard + CWall512::GetInstance()->GetFloatValue(-m_fDamageRandRange, m_fDamageRandRange)));
		CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText(); 

		CCamera::m_pMainCam->Set_ShakeCamera(10.f, 2.5f);
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, 0.75f);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SE_Laser_01.wav", 0.85f);
	}
}

void CRun_Obstacle::Set_Constantable()
{
	CAttackObj::Set_Constantable();

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

	// 포그
	pEffect->SetVector("g_vFogColor", &(_vec4)(CFogManager::GetInstance()->Get_FogColor()));
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	CFogManager::GetInstance()->Set_FogEnd(10000.f);
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
}

void CRun_Obstacle::Set_Constantable_Fog(CShader * pShader)
{
	CAttackObj::Set_Constantable_Fog(pShader);
}

void CRun_Obstacle::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pSMeshCom = static_cast<CSMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Run_Obstacle"));

	if (!m_pSMeshCom)
	{
		if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SMesh_Run_Obstacle", CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Obstacle/School/", L"[S]School_Barigate1.X"))))
		{
			assert(0);
		}

		m_pSMeshCom = static_cast<CSMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Run_Obstacle"));
	}
	m_mapComponent.emplace(ComTag_SMesh, m_pSMeshCom);

	// Shader 
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider 
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
	m_pSphereColliderCom->SetRadius(1.f);
}

void CRun_Obstacle::Appear(const _float & fTimeDelta)
{
	if (5.f < D3DXVec3Length(&(m_vTargetPos - Get_Position())))
	{
		Move_Pos(m_vAppearDir, m_fSpeed, fTimeDelta);
	}
	else if (!m_bShakeOneTime)
	{
		m_bShakeOneTime = TRUE; 

		m_pParticleBreak = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_Obstacle.particle");
		assert(m_pParticleBreak);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleBreak);
		m_pParticleBreak->Set_Pos(Get_Position());
		m_pParticleBreak->Set_Scale(Get_Scale() * Half);
		m_pParticleBreak->Set_Direction(m_vAppearDir);

		CCamera::m_pMainCam->Set_ShakeCamera(10.f, 3.5f); 

		_uint	iRand = CWall512::GetInstance()->GetValue(0, 3); 
		if (0 == iRand)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Obstacles_Explosion_L_01.wav", 1.f);
		}
		else if (0 == iRand)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Obstacles_Explosion_L_02.wav", 1.f);
		}
		else
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Obstacles_Explosion_L_03.wav", 1.f);
		}
	}
}

void CRun_Obstacle::Check_Dead()
{
}
