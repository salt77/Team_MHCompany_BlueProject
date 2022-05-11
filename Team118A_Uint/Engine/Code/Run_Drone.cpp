#include "Run_Drone.h"

#include "Run_Player.h" 
#include "Run_Map.h"
#include "Drone_Bullet.h" 
#include "Chesed_Normal_Bullet.h" 
#include "Bullet_Run_Normal.h"

#include "Scene.h"
#include "KeyManager.h"
#include "DMesh.h"
#include "UI_Manager.h"
#include "Wall512.h"
#include "TimeScale_Manager.h"

USING(Engine)

CRun_Drone::CRun_Drone(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag)
	: CRun_Unit(pGraphicDev, wstrUnitName, eObjTag)
{
}

CRun_Drone::CRun_Drone(const CRun_Drone & rhs)
	: CRun_Unit(rhs)
{
}

CRun_Drone::~CRun_Drone()
{
}

CRun_Drone * CRun_Drone::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const wstring& wstrUnitName,
								const _float& fPipeAngle,
								const E_DRONE_ATKTYPE& eAtkType,
								const _uint& iSpawnCount,
								E_OBJTAG eObjTag)
{
	CRun_Drone*	pInstance = new CRun_Drone(pGraphicDev, wstrUnitName, eObjTag);

	if (FAILED(pInstance->Ready_Object(fPipeAngle, eAtkType, iSpawnCount)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRun_Drone::Free()
{
	FSM_FunctionRelease(); 

	CRun_Unit::Free();
}

void CRun_Drone::Save(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Save(hFile, dwByte);
}

void CRun_Drone::Load(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Load(hFile, dwByte);
}


HRESULT CRun_Drone::Ready_Object(const _float& fPipeAngle, const E_DRONE_ATKTYPE& eAtkType, const _uint& iSpawnCount)
{
	CRun_Unit::Ready_Object();

	Add_Component();
	Ready_UnitInfo();

	//Set_Scale(VEC_ONE * 100.f);

	m_fPipeAngle = fPipeAngle;
	m_eDroneType = eAtkType;

	switch (m_eDroneType)
	{
	case Engine::E_DRONE_ATKTYPE::DRONE_ROTATE:
		m_fRotatePipeAngle = 360.f / iSpawnCount;
		break;

	case Engine::E_DRONE_ATKTYPE::DRONE_CHANGE:
		m_fRotatePipeAngle = 360.f / iSpawnCount * 4.f; 
		break;

	case Engine::E_DRONE_ATKTYPE::DRONE_ROUNDED:
		break; 

	default:
		assert(0);
		break;
	}

	m_fNextRotateTargetAngle = m_fRotatePipeAngle;

	m_szBodyBoneName = "bone_move_main";

	m_iRotateType = CWall512::GetInstance()->GetValue(0, 2);

	return S_OK;
}

_int CRun_Drone::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Start_Object(fTimeDelta);

	OnEnable();

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (!m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CRun_Drone -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음");
	}

	Set_Scale(VEC_ONE * m_pRunMap->Get_Scale().x * 20.f);

	m_fRunAngle = m_pPlayer->Get_RunAngle() + m_fFrontSpawnAngle;
	m_fDistanceToPipeMid = m_pPlayer->Get_DistanceOriginToPipeMid();
	m_fDistanceFromPipeMid = m_pPlayer->Get_DistancePipeRadius() * 0.8f;

	m_fSpeedInterpol = m_pPlayer->Get_OriginSpeed() * 12.f; 
	m_fFrontSpawnAngle = m_pRunMap->Get_Scale().x * 24.f; 
	m_fDistancePlayerDeadline = m_pRunMap->Get_Scale().x * 65.f; 

	Set_ActionByEnum(E_ACTION::Move_Ing); 

	return iExit;
}

_int CRun_Drone::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Update_Object(fTimeDelta);

	Update_FSM(fTimeDelta);
	Update_Trigger(fTimeDelta);

	Movement(fTimeDelta);
	Interpolate_Rotation();
	Move_InPipe(fTimeDelta);

	Update_WorldMatrix();
	Update_Collider(fTimeDelta);

	Animation_Change(); 
	
	m_pDMeshCom->Set_AnimSpeed(1.f * CTimeScale_Manager::GetInstance()->Get_TimeScale());
	m_pDMeshCom->Play_Animation(fTimeDelta);

	if (&CRun_Drone::Dead == m_fpLockFunction)
	{
		_matrix		matWorld = Get_Transform()->Get_WorldMatrix();
		matWorld._41 = (_float)INT_MAX;
		m_pColliderCom->SetMatrix(matWorld);
	}
	else
	{
		m_pColliderCom->SetMatrix(m_matWorld);
	}
	
	return iExit;
}

_int CRun_Drone::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CRun_Drone::Render_Object(_int iPass)
{
	CRun_Unit::Render_Object(iPass);
}

void CRun_Drone::Debug_This_In_Console()
{
}

void CRun_Drone::Set_Constantable()
{
	CRun_Unit::Set_Constantable();
}

void CRun_Drone::Set_Constantable_Fog(CShader * pShader)
{
	CRun_Unit::Set_Constantable_Fog(pShader);
}

void CRun_Drone::OnEnable()
{
	CRun_Unit::OnEnable();
}

void CRun_Drone::OnDisable()
{
	CRun_Unit::OnDisable();
}

void CRun_Drone::OnDestroy()
{
	CRun_Unit::OnDestroy();
}

void CRun_Drone::On_Collision(CGameObject * pDest)
{
	CRun_Unit::On_Collision(pDest);

	//if (E_LAYERTAG::AttackObj_Player == pDest->Get_LayerTag())
	//{
	//	Set_Damage(static_cast<CBullet_Run_Normal*>(pDest)->Get_Damage());
	//}
}

void CRun_Drone::Update_Collider(const _float & fTimeDelta)
{
	//Collider_SetMatrix(m_pColliderCom, m_szBodyBoneName);
}

void CRun_Drone::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader 
	m_pShaderCom = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base"));
	m_mapComponent.emplace(ComTag_Shader, m_pShaderCom);

	Create_Collider(&m_pColliderCom, ComTag_Collider, 1.f);
	m_pColliderCom->SetScale(Get_Scale() * 20.f); 

	// DMesh 
	m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Drone"));

	if (!m_pDMeshCom)
	{
		if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DMesh_Run_Drone",
			CDMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Monster/Drone_Decagram_Vulcan/", L"Drone_Decagram_Vulcan.X"))))
		{
			assert(0);
		}

		m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Drone"));
	}

	m_mapComponent.emplace(ComTag_DMesh, m_pDMeshCom);

	Initialize_Trigger();
	Load_Animation();
}

void CRun_Drone::Ready_UnitInfo()
{
	m_tUnitInfo.iMaxHp = 1000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
}

void CRun_Drone::Move_InPipe(const _float& fTimeDelta)
{
	if (!m_pPlayer || !m_pRunMap)
	{
		return;
	}

	if (&CRun_Drone::Dead == m_fpLockFunction)
	{
		return;
	}

	m_fSpeed = m_pPlayer->Get_Speed();
	m_fRunAngle += m_fSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();

	if (m_fDistancePlayerDeadline <= m_fDistanceFromPlayer)
	{
		_vec3	vDir = VEC_UP;

		m_fPlayerRunAngle = m_pPlayer->Get_RunAngle();
		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
		_vec3	vToPipeMidDir = vDir * m_fDistanceToPipeMid;
		m_vPipeMidPos = m_pRunMap->Get_Position() + vToPipeMidDir;

		m_fDistanceFromPlayer = D3DXVec3Length(&(m_pPlayer->Get_Position() - m_vPipeMidPos));

		_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vToPipeMidDir); 
		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vToPipeMidDir);

		Set_Pos(m_vPipeMidPos + vToPipeMidDir * m_fDistanceFromPipeMid);

		m_fDecRunAngle -= D3DXVec3Length(&(m_pPlayer->Get_Position() - Get_Position())) * Hundredth * 0.3f;
		m_fRunAngle = m_fPlayerRunAngle + m_fFrontSpawnAngle + m_fDecRunAngle;
	}
	else
	{
		if (!m_bUpdateFSM)
		{
			m_bUpdateFSM = TRUE; 

			FSM_FunctionSetting(&CRun_Drone::MoveEnd, &CRun_Drone::Idle); 
		}

		_vec3	vDir = VEC_UP;

		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
		_vec3	vToPipeMidDir = vDir * m_fDistanceToPipeMid;
		m_vPipeMidPos = m_pRunMap->Get_Position() + vToPipeMidDir;

		_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vToPipeMidDir);
		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vToPipeMidDir);

		m_vTargetPos = m_vPipeMidPos + vToPipeMidDir * m_fDistanceFromPipeMid;
		_vec3	vTargetDir = *D3DXVec3Normalize(&vTargetDir, &(m_vTargetPos - Get_Position()));

		Move_Pos(vTargetDir, D3DXVec3Length(&(m_vTargetPos - Get_Position())) * m_fSpeedInterpol, fTimeDelta);
	}
}

void CRun_Drone::Movement(const _float & fTimeDelta)
{
	if (&CRun_Drone::Dead == m_fpLockFunction)
	{
		return; 
	}

	m_fMovementDelay -= fTimeDelta;

	if (E_DRONE_ATKTYPE::DRONE_ROUNDED == m_eDroneType)
	{
		m_fPipeAngle += m_fSpeed * Half;
	}
	else if (0.f >= m_fMovementDelay)
	{
		if (E_DRONE_ATKTYPE::DRONE_ROTATE == m_eDroneType)
		{
			if (m_iRotateType)
			{
				m_fPipeAngle += m_fNextRotateTargetAngle * Tenth;
			}
			else
			{
				m_fPipeAngle -= m_fNextRotateTargetAngle * Tenth;
			}

			m_fNextRotateTargetAngle -= m_fNextRotateTargetAngle * Tenth;

			if (Tenth >= m_fNextRotateTargetAngle)
			{
				m_fMovementDelay = m_fMovementOriginDelay;
				m_fNextRotateTargetAngle = m_fRotatePipeAngle;
			}
		}
		else if (E_DRONE_ATKTYPE::DRONE_CHANGE == m_eDroneType)
		{
			m_fPipeAngle += m_fRotatePipeAngle; 

			//_vec3	vDir = VEC_UP;

			//CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
			//_vec3	vToPipeMidDir = vDir * m_fDistanceToPipeMid;
			//_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vToPipeMidDir;

			//CUtility::Rotate_DirVector(m_fPipeAngle + m_fRotatePipeAngle, m_pPlayer->Get_VecAxis(E_ROTATION::ROT_Z), &vToPipeMidDir);

			//m_vTargetPos = vPipeMidPos + vToPipeMidDir * m_fDistanceFromPipeMid;

			//// 바로 초기화 
			m_fMovementDelay = m_fMovementOriginDelay;
		}
		else
		{
			assert(0);
		}
	}
}

void CRun_Drone::Interpolate_Rotation()
{
	if (!m_pRunMap || !m_pPlayer)
	{
		return;
	}

	_vec3	vDir = VEC_UP;

	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_fDistanceFromPipeMid;

	_float		fAngleX = D3DXToDegree(acos(D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &-m_pPlayer->Get_VecAxis(E_ROTATION::ROT_Z))));
	_vec3		vOutVec = VEC_ZERO;

	if (0.f > D3DXVec3Dot(&-m_vAxis[(_uint)E_ROTATION::ROT_Y], &-m_pPlayer->Get_VecAxis(E_ROTATION::ROT_Z)))
	{
		fAngleX = -fAngleX;
	}

	Add_Rotation_Quat(E_ROTATION::ROT_X, fAngleX);


	//_float	fAngleZ = m_pPlayer->Get_vQuatAngle().z - m_vQuatAngle.z; 
	//Add_Rotation_Quat(E_ROTATION::ROT_Z, fAngleZ); 
}

void CRun_Drone::Create_DroneBullet(const _float& fTimeDelta)
{
	m_fFireDelay -= fTimeDelta;

	if (0.f >= m_fFireDelay)
	{
		m_fFireDelay = m_fFireOriginDelay;

		CGameObject*	pGameObject = Chesed_Normal_Bullet::Create(m_pGraphicDev, 
																   Get_Position() - m_pPlayer->Get_Position(), 
																   m_fRunAngle, 
																   m_fPipeAngle, 
																   E_CHESED_ATKTYPE::CHESED_ATK_DEPLOY, 
																   0.f, 
																   E_TANMAKTYPE::TANMAK_DESTROY, 
																   E_LAYERTAG::Character);
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
		pGameObject->Set_Pos(m_pColliderCom->GetPos());
	}
}

void CRun_Drone::Animation_Change()
{
	if (m_eAction != m_ePreAction) 
	{
		if (E_ACTION::Vital_Death == m_eAction) 
		{
			//CGameObject*	pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Explosion_Particle.particle"); 
			//assert(pParticle); 
			//CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle); 
			//pParticle->Set_Pos(Get_Position()); 
			//pParticle->Set_Scale(Get_Scale()); 

			_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
			iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_01.wav", 1.5f) :
				CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_02.wav", 1.5f); 
		}
	}

	CRun_Unit::Animation_Change(); 
}

void CRun_Drone::FSM_FunctionSetting(void(CRun_Drone::* pCol)(const _float &), void(CRun_Drone::* pDestFunc)(const _float &))
{
	m_bStateLock = TRUE; 

	m_fpLockFunction = pCol; 
	m_fpDestFunction = pDestFunc; 
}

void CRun_Drone::FSM_FunctionRelease()
{
	m_bStateLock = FALSE; 

	m_fpLockFunction = nullptr;
	m_fpDestFunction = nullptr; 
}

void CRun_Drone::Update_FSM(const _float & fTimeDelta)
{
	if (!m_pPlayer || !m_pRunMap)
	{
		return;
	}

	if (0 >= m_tUnitInfo.iNowHp)
	{
		FSM_FunctionSetting(&CRun_Drone::Dead, nullptr);
		Dead(fTimeDelta); 
	}

	if (m_bUpdateFSM)
	{
		m_fCountingTime += fTimeDelta; 

		if (m_fpLockFunction) 
		{
			((*this).*m_fpLockFunction)(fTimeDelta); 
		}
		else
		{
			FSM_FunctionSetting(&CRun_Drone::Idle, &CRun_Drone::Reload); 
		}
	}
}

void CRun_Drone::MoveEnd(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Move_End_Normal; 

	Animation_Change(); 

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE; 

		FSM_FunctionSetting(&CRun_Drone::Idle, &CRun_Drone::Reload);
	}
}

void CRun_Drone::Idle(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Normal_Idle; 

	Animation_Change(); 

	if (m_bFireTrigger)
	{
		Create_DroneBullet(fTimeDelta); 
	}

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE; 

		FSM_FunctionSetting(&CRun_Drone::Reload, &CRun_Drone::Idle); 
	}
} 

void CRun_Drone::Reload(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Normal_Reload; 

	Animation_Change(); 

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE; 

		FSM_FunctionSetting(&CRun_Drone::Idle, &CRun_Drone::Reload);
	}
}

void CRun_Drone::Dead(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Vital_Death; 

	Animation_Change(); 

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bDead = TRUE; 
	}
}

void CRun_Drone::Trigger_1()
{
	m_bFireTrigger = TRUE; 
}

void CRun_Drone::Trigger_2()
{
	m_bFireTrigger = FALSE; 
}
