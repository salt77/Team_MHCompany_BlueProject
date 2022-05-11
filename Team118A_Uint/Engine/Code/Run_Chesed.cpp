#include "Run_Chesed.h"

#include "Run_Player.h" 
#include "Run_Map.h"
#include "Chesed_Normal_Bullet.h" 
#include "Chesed_Circle_Bullet.h" 
#include "Chesed_Direct_Laser.h" 
#include "Chesed_Web_Laser.h"
#include "Run_Drone.h" 
#include "Bullet_Run_Normal.h"

#include "Scene.h"
#include "KeyManager.h"
#include "DMesh.h"
#include "UI_Manager.h"
#include "Wall512.h" 
#include "TimeScale_Manager.h" 
#include "ActionCam_Mgr.h" 

USING(Engine)

CRun_Chesed::CRun_Chesed(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag)
	: CRun_Unit(pGraphicDev, wstrUnitName, eObjTag)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CRun_Chesed::CRun_Chesed(const CRun_Chesed & rhs)
	: CRun_Unit(rhs)
{
}

CRun_Chesed::~CRun_Chesed()
{
}

CRun_Chesed * CRun_Chesed::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag, _bool bCutSceneView)
{
	CRun_Chesed*	pInstance = new CRun_Chesed(pGraphicDev, wstrUnitName, eObjTag);

	if (FAILED(pInstance->Ready_Object(bCutSceneView)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRun_Chesed::Free()
{
	FSM_FunctionRelease();

	CRun_Unit::Free();
}

void CRun_Chesed::Save(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Save(hFile, dwByte);
}

void CRun_Chesed::Load(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Load(hFile, dwByte);
}

HRESULT CRun_Chesed::Ready_Object(_bool bCutSceneView)
{
	CRun_Unit::Ready_Object();

	m_bCutSceneView = bCutSceneView;
	m_bCutSceneView ? Set_Pos(_vec3(0.f, 100.f, 0.f)) : VEC_ZERO;

	Add_Component();
	Ready_UnitInfo();

	m_vOriginAxisY = m_vAxis[(_uint)E_ROTATION::ROT_Y];

	m_szBodyBoneName = "bone_move_main";
	m_ePatternType = E_Chesed_Pattern::Pattern_Spawn;

	m_fDeadMoveDelay = 1.f;

	return S_OK;
}

_int CRun_Chesed::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Start_Object(fTimeDelta);

	OnEnable();

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));
	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));

	if (!m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CRun_Chesed -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음");
	}

	m_fScale = 65.f;
	m_bCutSceneView ? Set_Scale(VEC_ONE * 2.5f) : Set_Scale(VEC_ONE * m_fScale);
	m_pColliderCom->SetScale(Get_Scale());

	m_fRunAngle = m_pPlayer->Get_RunAngle() + m_fSpawnFrontAngle;
	m_fDistancePlayerDeadLine = m_pRunMap->Get_Scale().x * 65.f;

	Set_GroggyPercent(40.f);

	return iExit;
}

_int CRun_Chesed::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Update_Object(fTimeDelta);

	Update_FSM(fTimeDelta);
	Update_RotateInfo();
	Update_Trigger(fTimeDelta);

	Pattern_Change();
	Move_InPipe();
	LookAt_Player();

	//Update_WorldMatrix();
	Update_Collider(fTimeDelta);
	Update_Particle();

	Animation_Change();

	m_pDMeshCom->Set_AnimSpeed(1.f * CTimeScale_Manager::GetInstance()->Get_TimeScale());
	m_pDMeshCom->Play_Animation(fTimeDelta);
	m_pColliderCom->SetMatrix(m_matWorld);

	return iExit;
}

_int CRun_Chesed::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CRun_Chesed::Render_Object(_int iPass)
{
	CRun_Unit::Render_Object(iPass);
}

void CRun_Chesed::Debug_This_In_Console()
{
}

void CRun_Chesed::Set_Constantable()
{
	CRun_Unit::Set_Constantable();
}

void CRun_Chesed::Set_Constantable_Fog(CShader * pShader)
{
	CRun_Unit::Set_Constantable_Fog(pShader);
}

void CRun_Chesed::Set_Damage(const _float & fDamage)
{
	if (!m_bSpawn)
	{
		return;
	}

	CRun_Unit::Set_Damage(fDamage);

	if (m_pBossHpBar)
	{
		m_pBossHpBar->Add_Damage_ForUI((_int)fDamage);
		Add_GroggyGage(-fDamage);

		if (0 >= m_tUnitInfo.iNowHp)
		{
			if (!m_pBossHpBar->Get_Dead())
			{
				m_pBossHpBar->Set_Dead(TRUE);
			}

			m_pBossHpBar = nullptr;
		}
	}

	if (0.f >= m_tUnitInfo.iNowHp && 
		!m_bJustOnceCheck)
	{
		m_bJustOnceCheck = TRUE; 

		static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Set_EndGame_Player(m_fEndGameDelay);
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, m_fEndGameDelay);
	}
}

void CRun_Chesed::Set_GroggyPercent(const _float & fPercentigy)
{
	m_fGroggy_MaxGage = m_tUnitInfo.iMaxHp * fPercentigy * 0.01f;
}

void CRun_Chesed::Add_GroggyGage(const _float & fDamage)
{
	// 받은 데미지 / 그로기 MaxHealth
	m_fGroggy_Gage += fDamage;
	m_pBossHpBar->Set_Gage_Groggy(m_fGroggy_Gage / m_fGroggy_MaxGage);
}

void CRun_Chesed::Set_ClearBossHpBar()
{
	if (m_pBossHpBar)
	{
		m_pBossHpBar->Set_Dead(TRUE);
		m_pBossHpBar = nullptr;
	}
}

void CRun_Chesed::OnEnable()
{
	CRun_Unit::OnEnable();

	FSM_FunctionSetting(&CRun_Chesed::Begin_Battle, &CRun_Chesed::Idle);
}

void CRun_Chesed::OnDisable()
{
	CRun_Unit::OnDisable();
}

void CRun_Chesed::OnDestroy()
{
	CRun_Unit::OnDestroy();
}

void CRun_Chesed::On_Collision(CGameObject * pDest)
{
	CRun_Unit::On_Collision(pDest);
}

void CRun_Chesed::Animation_Change()
{
	if (m_ePreAction != m_eAction)
	{
		m_pDMeshCom->Set_AnimSpeed(1.25f);
		Set_ActionByEnum(m_eAction);
		m_ePreAction = m_eAction;

		if (E_ACTION::Normal_Shoot != m_eAction)
		{
			m_fAttackCountingTime = 0.f;
		}

		switch (m_eAction)
		{
			// Idle 
		case E_ACTION::Normal_Idle:
			m_fIdleWaitingTime = CWall512::GetInstance()->GetFloatValue(m_fIdleWaitingMinTime, 5.f);
			break;

		case E_ACTION::Sit_Idle:
			m_fWebLaserTimeCount = m_fWebLaserDelay;
			break;

		case E_ACTION::Appear:
			Set_ActionByEnum(m_eAction);
			m_pDMeshCom->Play_Animation(0.f);
			break;
		}
	}
}

void CRun_Chesed::Update_Collider(const _float & fTimeDelta)
{
	Collider_SetMatrix(m_pColliderCom, m_szBodyBoneName);

	m_vWeaponPos = Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Z] * 5.f;
}

void CRun_Chesed::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader 
	m_pShaderCom = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base"));
	m_mapComponent.emplace(ComTag_Shader, m_pShaderCom);

	Create_Collider(&m_pColliderCom, ComTag_Collider, 1.f);

	// DMesh 
	m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Chesed"));

	if (!m_pDMeshCom)
	{
		if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DMesh_Run_Chesed",
			CDMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Monster/Chesed/", L"Chesed.X"))))
		{
			assert(0);
		}

		m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Chesed"));
	}

	m_mapComponent.emplace(ComTag_DMesh, m_pDMeshCom);

	Initialize_Trigger();
	Load_Animation();
}

void CRun_Chesed::Ready_UnitInfo()
{
	m_tUnitInfo.iMaxHp = 800000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
}

void CRun_Chesed::Update_RotateInfo()
{
	if (!m_pPlayer || !m_pRunMap)
	{
		return;
	}

	if (0.f == m_fDistanceToPipeMid)
	{
		m_fDistanceToPipeMid = m_pPlayer->Get_DistanceOriginToPipeMid();
	}

	m_fSpeed = m_pPlayer->Get_Speed();

	if (E_Chesed_Pattern::Pattern_Spawn != m_ePatternType)
	{
		m_fRunAngle = m_pPlayer->Get_RunAngle() + m_fAngleFront;
	}
}

void CRun_Chesed::Move_InPipe()
{
	if (!m_pPlayer || !m_pRunMap || m_bCutSceneView)
	{
		return;
	}

	if (E_Chesed_Pattern::Pattern_Spawn != m_ePatternType &&
		E_Chesed_Pattern::Pattern_Retreat != m_ePatternType)
	{
		if (&CRun_Chesed::Dead == m_fpLockFunction &&
			0.f >= m_fDeadMoveDelay)
		{
			return;
		}

		_vec3	vToPipeMid = VEC_UP;
		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vToPipeMid);
		vToPipeMid *= m_fDistanceToPipeMid;

		Set_Pos(m_pRunMap->Get_Position() + vToPipeMid);
	}
	else if (E_Chesed_Pattern::Pattern_Spawn == m_ePatternType)
	{
		_float		fPlayerAngle = m_pPlayer->Get_RunAngle();

		if (fPlayerAngle + m_fAngleFront < fPlayerAngle + m_fSpawnFrontAngle + m_fDecRunAngle)
		{
			_vec3	vDir = VEC_UP;
			CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
			_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_fDistanceToPipeMid;

			Set_Pos(vPipeMidPos);

			m_fDecRunAngle -= D3DXVec3Length(&(m_pPlayer->Get_Position() - Get_Position())) * Hundredth * 0.035f;
			m_fRunAngle = fPlayerAngle + m_fSpawnFrontAngle + m_fDecRunAngle;
		}
		else
		{
			m_ePatternType = Pattern_Normal;
		}
	}
	else if (E_Chesed_Pattern::Pattern_Retreat == m_ePatternType)
	{
		if (m_fDecRunAngle <= m_fSpawnFrontAngle)
		{
			_vec3	vDir = VEC_UP;
			CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
			_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_fDistanceToPipeMid;

			Set_Pos(vPipeMidPos);

			m_fDecRunAngle += D3DXVec3Length(&(m_pPlayer->Get_Position() - Get_Position())) * Hundredth * 0.1f;
			m_fRunAngle = m_pPlayer->Get_RunAngle() + m_fDecRunAngle;
		}
		else
		{
			Set_Pos(VEC_ZERO);
		}
	}
}

void CRun_Chesed::LookAt_Player()
{
	if (!m_pPlayer || !m_pRunMap || m_bCutSceneView)
	{
		return;
	}

	_vec3		vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_pPlayer->Get_RunAngle() - 15.f, VEC_RIGHT, &vDir);
	_vec3		vTargetLookPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	_vec3		vLookDir = m_vLookDir = *D3DXVec3Normalize(&vLookDir, &(vTargetLookPos - Get_Position()));
	_vec3		vRightDir = -m_pPlayer->Get_VecAxis(E_ROTATION::ROT_X);

	_vec3		vUpDir = -*D3DXVec3Cross(&vUpDir, &vLookDir, &vRightDir);
	_matrix		matScale, matTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixScaling(&matScale, Get_Scale().x, Get_Scale().y, Get_Scale().z);
	D3DXMatrixTranslation(&matTrans, Get_Position().x, Get_Position().y, Get_Position().z);
	memcpy(&m_matRot._11, &vRightDir, sizeof(_vec3));
	memcpy(&m_matRot._21, &vUpDir, sizeof(_vec3));
	memcpy(&m_matRot._31, &vLookDir, sizeof(_vec3));

	m_matWorld = matScale * m_matRot * matTrans;
}

void CRun_Chesed::Update_Particle()
{
	if (m_pParticleChargeLaser)
	{
		if (m_pParticleChargeLaser->Get_Dead())
		{
			m_pParticleChargeLaser = nullptr;

			return;
		}

		m_pParticleChargeLaser->Set_Pos(m_vWeaponPos);
	}
}

void CRun_Chesed::FSM_FunctionSetting(void(CRun_Chesed::* pCol)(const _float&), void(CRun_Chesed::* pDestFunc)(const _float&))
{
	if (&CRun_Chesed::Idle != pCol)
	{
		m_bStateLock = TRUE;
	}

	if (&CRun_Chesed::Attack == pCol)
	{
		m_fBulletDelay = m_fNormalBulletDelay;
		m_fBulletDelay = m_fCircleBulletDelay;
		m_fBulletDelay = m_fSpreeBulletDelay;
		m_fBulletDelay = m_fFourthSpreeDelay;
		m_fDirectLaserTimeCount = m_fDirectLaserDelay;

		if (0 == m_iPhase || 1 == m_iPhase)
		{
			m_eMinPatternType = Pattern_AttackSpree;
			m_eMaxPatternType = Pattern_AttackFourthDir;
		}
		else
		{
			m_eMinPatternType = Pattern_AttackRounded;
			m_eMaxPatternType = Pattern_AttackCircle;
		}

		Decide_Pattern();
	}

	m_fpLockFunction = pCol;
	m_fpDestFunction = pDestFunc;
}

void CRun_Chesed::FSM_FunctionRelease()
{
	m_bStateLock = FALSE;

	m_fpLockFunction = nullptr;
	m_fpDestFunction = nullptr;
}

void CRun_Chesed::Update_FSM(const _float & fTimeDelta)
{
	if (!m_pPlayer || !m_pRunMap)
	{
		return;
	}

	if (0 >= m_tUnitInfo.iNowHp)
	{
		FSM_FunctionSetting(&CRun_Chesed::Dead, nullptr);

		m_fDeadBombDelay -= fTimeDelta;

		if (0.f >= m_fDeadBombDelay)
		{
			m_fDeadBombDelay = 0.35f;

			_vec3	vRandPos = _vec3(CWall512::GetInstance()->GetFloatValue(0.f, 50.f), CWall512::GetInstance()->GetFloatValue(0.f, 50.f), CWall512::GetInstance()->GetFloatValue(0.f, 50.f));
			CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, Get_Position() + vRandPos, Get_Angle(), Get_Scale());
		}
	}

	m_fCountingTime += fTimeDelta;

	if (0.2f < ((_float)m_tUnitInfo.iNowHp / m_tUnitInfo.iMaxHp) &&
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster))
	{
		m_fDroneSpawnDelay -= fTimeDelta;

		if (0.f >= m_fDroneSpawnDelay)
		{
			m_fDroneSpawnDelay = 25.f;

			for (_uint i = 0; i < 6; ++i)
			{
				CGameObject*	pGameObject = CRun_Drone::Create(m_pGraphicDev, L"Drone_Decagram_Vulcan", i * 360.f / 6, E_DRONE_ATKTYPE::DRONE_ROUNDED, 6, E_OBJTAG::OBJ_Run_Monster);
				assert(pGameObject);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, pGameObject);
			}
		}
	}

	if (1.f <= (_float)m_fGroggy_Gage / m_fGroggy_MaxGage)
	{
		if (0 == m_iPhase ||
			1 == m_iPassNumber)
		{
			m_fpLockFunction = &CRun_Chesed::Attack_WebLaser;
		}

		m_fGroggy_Gage = 0.f;
		++m_iPhase;
	}

	if (m_fpLockFunction)
	{
		((*this).*m_fpLockFunction)(fTimeDelta);
	}
	else
	{
		FSM_FunctionSetting(&CRun_Chesed::Idle, &CRun_Chesed::Attack);
	}
}

void CRun_Chesed::Begin_Battle(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Normal_Idle;

	Animation_Change();

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE;
		m_bSpawn = TRUE;
		m_ePatternType = Pattern_Attack;

		FSM_FunctionSetting(&CRun_Chesed::Attack, &CRun_Chesed::Idle);

		CGameObject*	pGameObject = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Chesed", m_tUnitInfo.iMaxHp, 50, L"[Fixed]Boss_Hp_Bar_2.uidat");
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, pGameObject->Get_ObjTag(), pGameObject);
		static_cast<CUI_Object*>(pGameObject)->Set_ImageAlpha(0.f);

		m_pBossHpBar = static_cast<CBoss_HpBar*>(pGameObject);
	}
}

void CRun_Chesed::Idle(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Normal_Idle;

	Animation_Change();

	m_fIdleWaitingTime -= fTimeDelta;

	if (0.f >= m_fIdleWaitingTime)
	{
		m_bStateLock = FALSE;
		m_fIdleWaitingTime = CWall512::GetInstance()->GetFloatValue(m_fIdleWaitingMinTime, 3.5f);

		if (1 <= m_iPhase)
		{
			if (CWall512::GetInstance()->GetValue(0, 4))
			{
				FSM_FunctionSetting(&CRun_Chesed::Attack, &CRun_Chesed::Idle);
			}
			else
			{
				FSM_FunctionSetting(&CRun_Chesed::Attack_WebLaser, &CRun_Chesed::Idle);
			}
		}
		else
		{
			FSM_FunctionSetting(&CRun_Chesed::Attack, &CRun_Chesed::Idle);
		}
	}
}

void CRun_Chesed::Attack(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Normal_Shoot;

	m_fBulletDelay -= fTimeDelta;
	m_fSkillAttackDelay -= fTimeDelta;
	m_fDirectLaserTimeCount -= fTimeDelta;
	m_fAttackCountingTime += fTimeDelta;

	if (m_fAttackEndDelay <= m_fAttackCountingTime)
	{
		m_bAttack = FALSE;
	}
	else if (m_fAttackStartDelay <= m_fAttackCountingTime)
	{
		m_bAttack = TRUE;
	}

	Animation_Change();
	Pattern_Change();

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE;
		m_bLaserAttack = FALSE;
		m_fAttackCountingTime = 0.f;

		FSM_FunctionSetting(&CRun_Chesed::Idle, &CRun_Chesed::Attack);
	}

	if (!m_bAttack && !m_bLaserAttack)
	{
		return;
	}

	if (0 >= m_fBulletDelay)
	{
		switch (m_ePatternType)
		{
		case Engine::CRun_Chesed::Pattern_Attack:
			m_fBulletDelay = m_fNormalBulletDelay;
			Create_ChesedBullet(E_CHESED_ATKTYPE::CHESED_ATK_DEPLOY, E_TANMAKTYPE::TANMAK_DESTROY, m_pPlayer->Get_PipeAngle(), 30.f);
			break;

		case Engine::CRun_Chesed::Pattern_AttackSpree:
			m_fBulletDelay = m_fSpreeBulletDelay;
			Create_ChesedBullet(E_CHESED_ATKTYPE::CHESED_ATK_SPREE, E_TANMAKTYPE::TANMAK_SLOW, m_pPlayer->Get_PipeAngle(), 65.f);
			break;

		case Engine::CRun_Chesed::Pattern_AttackFourthDir:
			for (_uint i = 0; i < 4; ++i)
			{
				m_fBulletDelay = m_fFourthSpreeDelay;
				m_fRoundedFireAngle += 10.f;
				Create_ChesedBullet(E_CHESED_ATKTYPE::CHESED_ATK_SPREE, E_TANMAKTYPE::TANMAK_SLOW, i * m_fRoundedFireAngle + 90.f, 10.f);
			}
			break;

		case Engine::CRun_Chesed::Pattern_AttackRounded:
			m_fBulletDelay = m_fRoundedBulletDelay;
			m_fRoundedFireAngle += m_fRoundedFireAngleChange;
			Create_ChesedBullet(E_CHESED_ATKTYPE::CHESED_ATK_ROUNDED, E_TANMAKTYPE::TANMAK_NORMAL, m_fRoundedFireAngle);
			break;

		case Engine::CRun_Chesed::Pattern_AttackCircle:
			m_fBulletDelay = m_fCircleBulletDelay;
			Create_ChesedCircleBullet(CWall512::GetInstance()->GetValue(10, 13));
			break;

		case Engine::CRun_Chesed::Pattern_DirectLaser:
			if (!m_bLaserAttack)
			{
				m_bLaserAttack = TRUE;

				m_pParticleChargeLaser = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Chesed_ChargeLaser_2.particle");
				assert(m_pParticleChargeLaser);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleChargeLaser);
				m_pParticleChargeLaser->Set_Pos(m_vWeaponPos);
				m_pParticleChargeLaser->Set_Scale(Get_Scale());

				CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Chesed_Skill_01.wav", 0.5f);
			}
			else if (m_bLaserAttack &&
				0.f >= m_fDirectLaserTimeCount)
			{
				m_bLaserAttack = FALSE;
				m_fDirectLaserTimeCount = m_fDirectLaserDelay;

				Create_ChesedDirectLaser();

				if (m_pParticleChargeLaser)
				{
					m_pParticleChargeLaser->Set_ParticleDead();
					m_pParticleChargeLaser->Set_Dead(TRUE);
					m_pParticleChargeLaser = nullptr;
				}
			}
			break;

		default:
			break;
		}
	}
}

void CRun_Chesed::Attack_WebLaser(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Sit_Idle;
	m_ePatternType = Pattern_AttackWebLaser;

	Animation_Change();
	Pattern_Change();

	m_fWebLaserTimeCount -= fTimeDelta;

	if (0.f >= m_fWebLaserTimeCount)
	{
		if (7 < m_iLaserPhase)
		{
			m_bStateLock = FALSE;

			FSM_FunctionSetting(&CRun_Chesed::Idle, &CRun_Chesed::Attack);

			if (0 == m_iPhase)
			{
				m_eMinPatternType = Pattern_AttackSpree;
				m_eMaxPatternType = Pattern_AttackFourthDir;
				//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackSpree, (_int)Pattern_AttackFourthDir + 1);
			}
			else if (1 == m_iPhase)
			{
				m_eMinPatternType = Pattern_AttackSpree;
				m_eMaxPatternType = Pattern_AttackFourthDir;
				//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackSpree, (_int)Pattern_AttackFourthDir + 1);
			}
			else
			{
				m_eMinPatternType = Pattern_AttackCircle;
				m_eMaxPatternType = Pattern_AttackRounded;
				//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackSpree, (_int)Pattern_AttackFourthDir + 1);
			}

			return;
		}

		m_fWebLaserTimeCount = m_fWebLaserDelay;

		Create_ChesedWebLaser(CWall512::GetInstance()->GetValue(10, 13), 3 > m_iLaserPhase++ ? Chesed_Web_Laser::WebLaser_Normal : Chesed_Web_Laser::WebLaser_Cross);
	}
}

void CRun_Chesed::Groggy_SpawnDrone(const _float & fTimeDelta)
{
	//m_eAction = E_ACTION::Temp2;
	//m_ePatternType = Pattern_Groggy;

	//Animation_Change();
	//Pattern_Change();

	//if (!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster) &&
	//	2 > m_iDroneSpawnPhase)
	//{
	//	m_fDroneSpawnDelay -= fTimeDelta;

	//	if (0.f >= m_fDroneSpawnDelay)
	//	{
	//		m_fDroneSpawnDelay = m_fDroneSpawnOriginDelay;

	//		Spawn_Drone();
	//	}
	//}
	//else if (!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster) &&
	//	2 <= m_iDroneSpawnPhase)
	//{
	//	m_bStateLock = FALSE;

	//	FSM_FunctionSetting(&CRun_Chesed::Attack, &CRun_Chesed::Idle);
	//}
}

void CRun_Chesed::Dead(const _float & fTimeDelta)
{
	m_eAction = E_ACTION::Vital_Death;

	Animation_Change();

	m_fDeadMoveDelay -= fTimeDelta;

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = TRUE;
		m_bDead = TRUE;

		_matrix		matWorld = Get_Transform()->Get_WorldMatrix();
		matWorld._42 = (_float)INT_MAX;
		m_pColliderCom->SetMatrix(matWorld);
	}
}

void CRun_Chesed::Pattern_Change()
{
	if (m_ePatternType != m_ePrePatternType)
	{
		switch (m_ePatternType)
		{
		case CRun_Chesed::Pattern_Attack:
			m_fBulletDelay = m_fNormalBulletDelay;
			break;

		case CRun_Chesed::Pattern_AttackCircle:
			m_fBulletDelay = m_fCircleBulletDelay;
			break;

		case CRun_Chesed::Pattern_AttackRounded:
			break;

		case CRun_Chesed::Pattern_AttackSpree:
			m_fBulletDelay = m_fSpreeBulletDelay;
			break;

		case CRun_Chesed::Pattern_AttackFourthDir:
			m_fBulletDelay = m_fFourthSpreeDelay;
			break;

		case CRun_Chesed::Pattern_DroneSpawn:
			break;

		case CRun_Chesed::Pattern_DirectLaser:
			m_fDirectLaserTimeCount = m_fDirectLaserDelay;
			break;

		case CRun_Chesed::Pattern_AttackWebLaser:
			break;

		default:
			break;
		}

		m_fDecRunAngle = 0.f;
		m_ePrePatternType = m_ePatternType;
	}
}

void CRun_Chesed::Create_ChesedBullet(const E_CHESED_ATKTYPE& eAtkType, const E_TANMAKTYPE& eTanmakType, const _float& fPipeAngle, const _float& fPipeAngleRand)
{
	if (!m_pPlayer)
	{
		return;
	}

	CGameObject*	pGameObject = Chesed_Normal_Bullet::Create(m_pGraphicDev, m_pColliderCom->GetPos() - m_pPlayer->Get_Position(), m_fRunAngle, fPipeAngle, eAtkType, fPipeAngleRand, eTanmakType);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
	pGameObject->Set_Pos(m_pColliderCom->GetPos());
}

void CRun_Chesed::Create_ChesedCircleBullet(const _uint& iBulletCount)
{
	if (!m_pPlayer)
	{
		return;
	}

	for (_uint i = 0; i < iBulletCount; ++i)
	{
		CGameObject*	pGameObject =
			Chesed_Circle_Bullet::Create(m_pGraphicDev,
				i * 360.f / iBulletCount,
				E_TANMAKTYPE::TANMAK_NORMAL,
				E_LAYERTAG::Character);

		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
		pGameObject->Set_Pos(m_pColliderCom->GetPos());
	}
}

void CRun_Chesed::Create_ChesedDirectLaser()
{
	CGameObject*	pGameObject = Chesed_Direct_Laser::Create(m_pGraphicDev, E_LAYERTAG::Character);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CRun_Chesed::Create_ChesedWebLaser(const _uint& iLaserCount, const Chesed_Web_Laser::E_WebLaser_AtkType& eLaserType)
{
	for (_uint i = 0; i < iLaserCount; ++i)
	{
		CGameObject*	pGameObject = Chesed_Web_Laser::Create(m_pGraphicDev, eLaserType, E_LAYERTAG::Character);
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
	}
}

void CRun_Chesed::Spawn_Drone()
{
	//++m_iDroneSpawnPhase;
	//m_iDroneSpawnCount += 2;

	//for (_uint i = 0; i < m_iDroneSpawnCount; ++i)
	//{
	//	CGameObject*	pGameObject = CRun_Drone::Create(m_pGraphicDev, L"Drone_Decagram_Vulcan", i * 360.f / m_iDroneSpawnCount, E_DRONE_ATKTYPE::DRONE_CHANGE, m_iDroneSpawnCount, E_OBJTAG::OBJ_Run_Monster);
	//	assert(pGameObject);
	//	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, pGameObject);
	//}
}

void CRun_Chesed::Decide_Pattern()
{
	m_eMaxPatternType != m_ePatternType ? m_ePatternType = E_Chesed_Pattern((_uint)m_ePatternType + 1) :
		m_ePatternType = m_eMinPatternType;

	m_ePatternType = (E_Chesed_Pattern)CUtility::Clamp_int((_int)m_ePatternType, (_int)m_eMinPatternType, (_int)m_eMaxPatternType);
}

void CRun_Chesed::Trigger_Attack_1()
{
}

void CRun_Chesed::Trigger_Attack_2()
{
	//if (0 == m_iPhase)
	//{
	//	m_eMinPatternType = Pattern_Attack;
	//	m_eMaxPatternType = Pattern_AttackFourthDir;
	//	//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackSpree, (_int)Pattern_AttackFourthDir + 1);
	//}
	//else if (1 == m_iPhase)
	//{
	//	m_eMinPatternType = Pattern_AttackSpree;
	//	m_eMaxPatternType = Pattern_AttackFourthDir;
	//	//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackSpree, (_int)Pattern_AttackFourthDir + 1);
	//}
	//else
	//{
	//	m_eMinPatternType = Pattern_AttackRounded;
	//	m_eMaxPatternType = Pattern_AttackCircle;
	//	//m_ePatternType = (E_Chesed_Pattern)CWall512::GetInstance()->GetValue((_int)Pattern_AttackRounded, (_int)Pattern_AttackCircle + 1);
	//}

	//Decide_Pattern();
}

void CRun_Chesed::Trigger_1()
{
	if (m_bCutSceneView)
	{
		CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Chesed_1.camdat", L"Chesed_1", Get_Position(), Get_Angle().y);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Chesed_Cutin.wav", 0.5f);
	}
}

void CRun_Chesed::Trigger_2()
{
	if (m_bCutSceneView)
	{
		CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Chesed_1.camdat", L"Chesed_2", Get_Position(), Get_Angle().y);
	}
}

void CRun_Chesed::Trigger_3()
{
	if (m_bCutSceneView)
	{
		CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Chesed_1.camdat", L"Chesed_3", Get_Position(), Get_Angle().y);
	}
}

void CRun_Chesed::Trigger_4()
{
	if (m_bCutSceneView)
	{
		CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Chesed_1.camdat", L"Chesed_4", Get_Position(), Get_Angle().y);
	}
}

void CRun_Chesed::Trigger_5()
{
	if (m_bCutSceneView)
	{
		CActionCam_Mgr::GetInstance()->Stop_ActionCam();
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_QUAT);
	}
}

