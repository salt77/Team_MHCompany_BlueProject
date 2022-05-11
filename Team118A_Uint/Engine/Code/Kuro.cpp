#include "Kuro.h"
#include "UI_Damage.h"

#include "UI_Manager.h"
#include "Boss_HpBar.h"

#include "StaticObject.h"

#include "Kuro_Cup.h"
#include "Kuro_Horse.h"
#include "Kuro_Car.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "KuroGround.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKuro::CKuro(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CBossMonster(pGraphicDev, eObjTag, L"Kuro")
{
}

CKuro::CKuro(const CKuro & rhs)
	:CBossMonster(rhs)
{
}

CKuro::~CKuro()
{
}

CKuro * CKuro::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKuro* pInstance = new CKuro(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKuro::Free()
{
	if (m_pFadeUI)
	{
		m_pFadeUI->Set_Dead(TRUE);
		m_pFadeUI = nullptr;
	}
	Safe_Release(m_pWarpParticle);
	Safe_Release(m_pStaffChargeParticle);
	
	for (auto& pLaser : m_vecLaserParticle)
	{
		if (pLaser)
		{
			pLaser->Set_Dead(TRUE);
			Safe_Release(pLaser);
		}
	}
	m_vecLaserParticle.clear();

	if (m_pAuraObject)
	{
		m_pAuraObject->Set_Dead(TRUE);
		Safe_Release(m_pAuraObject);
	}
	CBossMonster::Free();

	
}

HRESULT CKuro::Ready_Object()
{
	
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Prop1";

	CBossMonster::Ready_Object();


	m_tUnitInfo.iAttack = 200;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 350000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 2000000;
	m_tUnitInfo.iStability;

	m_fMoveSpeed = 2.f;

	m_vecLaserParticle.reserve(4);
	m_vecExplosionPos.resize(12);

	CActionCam_Mgr::GetInstance()->Load_ActionCam(L"KuroCam.camdat");

	CScene::Get_MainScene()->Set_SkySphere(L"../../Reference/Resource/Texture/Test/fullmoon.jpg");
	return S_OK;
}

_int CKuro::Start_Object(const _float & fTimeDelta)
{
	CBossMonster::Start_Object(fTimeDelta);
	m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Kuro", m_tUnitInfo.iMaxHp, 100);
	assert(m_pBossHpBar);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, m_pBossHpBar->Get_ObjTag(), m_pBossHpBar);
	static_cast<CUI_Object*>(m_pBossHpBar)->Set_ImageAlpha(0.f);

	Set_GroggyPercent(38.f);

	m_bPlay = FALSE;

	Add_Pos(_vec3(0.f, 0.f, -132.f));
	CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"KuroCam.camdat", L"Intro", Get_Position(), Get_Angle().y);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Kuro_Op.wav", 1.f);
	


	for (auto& pChar : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
	{
		pChar->Set_Active(FALSE);
	}
	return RETURN_OK;
}

_int CKuro::Update_Object(const _float & fTimeDelta)
{
	CBossMonster::Update_Object(fTimeDelta);
	if (m_bPlay)
	{
		Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	}
	else
	{
		if (m_ePrevAction == E_ACTION::Appear && Is_AnimFinished())
		{
			m_bPlay = TRUE;
			Add_Pos(_vec3(0.f, 0.f, 132.f));
			CActionCam_Mgr::GetInstance()->Stop_ActionCam();

			for (auto& pChar : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
			{
				pChar->Set_Active(TRUE);
				pChar->Set_UseGravity(TRUE);

				
			}
			CSound_Manager::GetInstance()->PlayBGM(L"BGM_보스전_시로쿠로.mp3", 0.4f);
			m_eCurrentAction = E_ACTION::Normal_Idle;
			/*if (m_pAuraObject)
			{
				m_pAuraObject->Set_Dead(TRUE);
				Safe_Release(m_pAuraObject);
				m_pAuraObject = nullptr;
			}*/
	
		}
		else
		{
			m_eCurrentAction = E_ACTION::Appear;
		}


	}
	Update_Shake(fTimeDelta);
	Update_Fade(fTimeDelta);
	Animation_Change(fTimeDelta);


	SpinLaserUpdate(fTimeDelta);

	if (m_pAuraObject)
	{
		if (m_pAuraObject->Get_Dead())
		{
			Safe_Release(m_pAuraObject);
			m_pAuraObject = CSkill_Manager::GetInstance()->SetSkill(L"Kuro_Aura.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 2.f);
			m_pAuraObject->AddRef();
		}
		_vec3 vAuraPos = m_pSphereCollider->GetPos();
		vAuraPos.y -= 0.5f;
		m_pAuraObject->Set_Pos(vAuraPos);
	}
	// 테스트용 코드 
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_1))
	{
		//Ex1_Pattern(0, FALSE);
		//FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
		DoubleBumpCar();
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_2))
	{
		BumpCar();
		//Ex2_Pattern(0, FALSE);
		//FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_3))
	{
		SpinHorse();
		//Ex3_Pattern(0, FALSE);
		//FSM_FunctionSetting(&CUnit::Boss_Attack_3, nullptr);
		
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_4))
	{
		DoubleDonut();
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_5))
	{
		SpinCup();
	}
	//if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_6))
	//{
	//	_int iWarn = (_int)m_eRandWarn;
	//	iWarn++;

	//	if (iWarn == (_int)E_RandWarn::RandWarn_END)
	//	{
	//		iWarn = 0;
	//	}

	//	m_eRandWarn = (E_RandWarn)iWarn;
	//}
	//if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_7))
	//{
	//	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
	//}

	return RETURN_OK;
}

_int CKuro::LateUpdate_Object(const _float & fTimeDelta)
{
	CBossMonster::LateUpdate_Object(fTimeDelta);

	
	return RETURN_OK;
}

void CKuro::Render_Object(_int iPass)
{
	if (!m_bRender)
	{
		return;
	}

	m_pDMeshCom->Set_Glow_Option(2, 0.f, VEC_LOOK, 1.f, 0.f, 1.f, 0.f, 1.f);
	CBossMonster::Render_Object(iPass);
}

void CKuro::Set_Constantable()
{
	CBossMonster::Set_Constantable();
}

void CKuro::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (!m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
	}

	if (nullptr == m_pTarget || m_pTarget->Get_UnitDead())
	{
		Find_Target_InRange(eTargetLayer, eObjTag);
	}

	if (nullptr == m_pTarget)
	{
		FSM_FunctionRelease();
	}

	if (FALSE == m_bStateLock)
	{
		if (nullptr != m_pTarget)
		{
			FSM_FunctionSetting(&CUnit::Idle, nullptr);
		}
	}
	else
	{
		if (m_fpLockFunction)
		{
			((*this).*m_fpLockFunction)(fDeltaTime);
		}
		else
		{
			if (Is_AnimFinished())
			{
				m_bStateLock = FALSE;
			}
		}
	}
}

void CKuro::FSM_FunctionSetting(void(CUnit::* pCol)(_float), void(CUnit::* pDestFunc)(_float))
{
	CUnit::FSM_FunctionSetting(pCol, pDestFunc);
	m_bFirst = TRUE;
	m_fTimer = CScene::Get_PlayTime();
}

void CKuro::Boss_Groggy(_float fDeltaTime)
{
	if (m_bFirst)
	{
		if (m_pWarpParticle)
		{
			if (!m_pWarpParticle->Get_Dead())
			{
				m_pWarpParticle->Set_ParticleDead();
			}
			Safe_Release(m_pWarpParticle);
			m_pWarpParticle = nullptr;
		}

		if (m_pStaffChargeParticle)
		{
			if (!m_pStaffChargeParticle->Get_Dead())
			{
				m_pStaffChargeParticle->Set_ParticleDead();
			}
			Safe_Release(m_pStaffChargeParticle);
			m_pStaffChargeParticle = nullptr;
		}

		m_bRender = TRUE;
		m_bAnimPlay = TRUE;
		m_bFirst = FALSE;

		Release_Invoke();
	}

	m_eCurrentAction = E_ACTION::Vital_Panic;
	m_fGroggy_Gage = 0.f;
	m_fExSkill2_Gage = 0.f;
	if (m_ePrevAction == E_ACTION::Vital_Panic)
	{
		if (Is_AnimFinished())
		{
			m_fpDestFunction = nullptr;
			FSM_FunctionRelease();
			
		}
	}

}

void CKuro::Idle(_float fDeltaTime)
{
	m_eMainAction = E_ACTION::Normal_Idle;

	if (m_pTarget->Get_UnitDead())
	{
		FSM_FunctionRelease();
		return;
	}

	Set_Look(m_pTarget->Get_Position());

	m_fGage += fDeltaTime;


	
	
	if (m_fGage >= 5.f)
	{
		if (!HPEventCheck())
		{
			if (m_fExSkill2_MaxGage <= m_fExSkill2_Gage)
			{
				if (m_iPhase == 1)
				{
					FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
				}
				else
				{
					_int iRand = CWall512::GetInstance()->GetValue(0, 100);

					if (iRand < 30)
					{
						FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
					}
					else if (iRand < 60)
					{
						FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Boss_Attack_1);
					}
					else
					{
						FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Boss_Attack_2);
					}
				}
				m_fExSkill2_Gage -= m_fExSkill2_MaxGage;
			}
			else
			{
				_int iRand = CWall512::GetInstance()->GetValue(0, 100);

				switch (m_iPhase)
				{
				case 1:
					FSM_FunctionSetting(&CUnit::Shoot, nullptr);
					break;

				case 2:
					if (iRand < 50)
					{
						FSM_FunctionSetting(&CUnit::Shoot, nullptr);
					}
					else
					{
						FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Shoot);
					}
					break;

				case 3:
					if (iRand < 50)
					{
						FSM_FunctionSetting(&CUnit::Shoot, nullptr);
					}
					else
					{
						FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Shoot);
					}
					break;

				default:
					break;
				}
			}
		}
	
		m_fGage = 0.f;
	}

	m_eCurrentAction = m_eMainAction;
}

void CKuro::Shoot(_float fDeltaTime)
{


	if (m_pStaffChargeParticle)
	{
		_vec3 vStaffPos = m_pWeaponCollider->GetPos() + m_pWeaponCollider->GetUp() * 0.9f;
		m_pStaffChargeParticle->Set_Pos(vStaffPos);
	}
	else 
	{
		if (m_bFirst)
		{
			m_eCurrentAction = E_ACTION::Normal_Idle;

			m_pStaffChargeParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Charge.particle");
			m_pStaffChargeParticle->AddRef();
			m_pStaffChargeParticle->Set_Scale(0.5f, 0.5f, 0.5f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pStaffChargeParticle);

			Normal_AttackPattern();
		
			m_fShootDelay = 2.f;

			m_bFirst = FALSE;
			return;
		}
	}

	switch (m_ePrevAction)
	{
	case E_ACTION::Normal_Shoot_Start:
		if (Is_AnimFinished())
		{
			m_eCurrentAction = E_ACTION::Normal_Delay;
		}
		break;
	case E_ACTION::Normal_Delay:
		if (Is_AnimFinished())
		{
			m_pStaffChargeParticle->Set_Dead(TRUE);
			Safe_Release(m_pStaffChargeParticle);
			m_pStaffChargeParticle = nullptr;
			m_eCurrentAction = E_ACTION::Normal_Shoot;

			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SE_Magic_01.wav", 1.f);
		}
		break;
	case E_ACTION::Normal_Shoot:
		if (Is_AnimFinished())
		{
			m_eCurrentAction = E_ACTION::Normal_Shoot_End;
		}
		break;
	case E_ACTION::Normal_Shoot_End:
		if (Is_AnimFinished())
		{
			FSM_FunctionRelease();
		}
		break;
	case E_ACTION::Normal_Idle:
		m_fShootDelay -= fDeltaTime;
		if (m_fShootDelay < 0.f)
		{	
			m_eCurrentAction = E_ACTION::Normal_Shoot_Start;
		}
		break;
	default:
		break;
	}

	
}

//아무위치에서나 나가는 패턴
void CKuro::Boss_Attack_1(_float fDeltaTime)
{

	m_eCurrentAction = E_ACTION::ExSkill;
	if (m_ePrevAction == E_ACTION::ExSkill)
	{
		if (Is_AnimFinished())
		{
			FSM_FunctionRelease();
		}
	}
}

//중앙에서만 나가는 패턴
void CKuro::Boss_Attack_2(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill;
	if (m_ePrevAction == E_ACTION::ExSkill)
	{
		if (Is_AnimFinished())
		{
			FSM_FunctionRelease();
		}
	}
}


void CKuro::Boss_Attack_3(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill;
	if (m_ePrevAction == E_ACTION::ExSkill)
	{
		if (Is_AnimFinished())
		{
			m_bInvn = FALSE;
			FSM_FunctionRelease();
		}
	}
}

//텔레포트
void CKuro::Boss_Attack_4(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill_CutIn;
	if (m_bFirst)
	{
		if (m_fpDestFunction == &CUnit::Shoot || m_fpDestFunction == &CUnit::Boss_Attack_1)
		{
			m_vWarpPos = Get_RandomWarpPos();
		}
		else if (m_fpDestFunction == &CUnit::Boss_Attack_2 || m_fpDestFunction == &CUnit::Boss_Attack_3)
		{
			m_vWarpPos = m_vCenterPos;

			if (D3DXVec3Length(&(m_vWarpPos - Get_Position())) < 0.5f)
			{
				if (m_fpDestFunction)
				{
					FSM_FunctionSetting(m_fpDestFunction, nullptr);
					m_fpDestFunction = nullptr;
					
				}
				else
				{
					FSM_FunctionRelease();
				}

				return;
			}
		}
		m_bInvn = TRUE;
		m_bFirst = FALSE;
	}
	


	if (m_pWarpParticle)
	{
		m_pWarpParticle->Set_Pos(m_pSphereCollider->GetPos());
		if (m_bRender)
		{
			//Move_Pos(VEC_UP, m_fMoveSpeed*5.f, fDeltaTime);
		}
	}

	if (!m_bRender)
	{
		_vec3 vDir = m_vWarpPos - Get_Position();
		if (D3DXVec3Length(&vDir) < 0.3f)
		{
			m_bRender = TRUE;
			m_bAnimPlay = TRUE;
			m_pWarpParticle->Set_ParticleDead();
			Safe_Release(m_pWarpParticle);
			m_pWarpParticle = nullptr;

			CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Crow.particle");
			pParticle->Set_Pos(m_pSphereCollider->GetPos());
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

			if (m_pTarget)
			{
				Set_Look(m_pTarget->Get_Position());
			}
		}
		else
		{
			D3DXVec3Normalize(&vDir, &vDir);
			Move_Pos(vDir, m_fMoveSpeed*5.f, fDeltaTime);
		}
	}

	//if (!m_pWarpParticle)
	//{
	//	if (Get_Position().y > 0.f)
	//	{
	//		Move_Pos(VEC_DOWN, m_fMoveSpeed*8.f, fDeltaTime);
	//	}

	//	if (Get_Position().y < 0.f)
	//	{
	//		Set_Pos(E_POSITION::POS_Y, 0.f);
	//	}
	//}

	if (m_ePrevAction == E_ACTION::ExSkill_CutIn)
	{
		
		if (Is_AnimFinished())
		{
			m_bInvn = FALSE;
			if (m_fpDestFunction)
			{
				FSM_FunctionSetting(m_fpDestFunction, nullptr);
				m_fpDestFunction = nullptr;
			}
			else
			{
				FSM_FunctionRelease();
			}
		}
	}
}

void CKuro::ExSkill(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill;
	if (m_ePrevAction == E_ACTION::ExSkill)
	{
		if (Is_AnimFinished())
		{
			m_bInvn = FALSE;
			FSM_FunctionRelease();
		}
	}
}


void CKuro::OnEnable()
{
	CBossMonster::OnEnable();

	//CGameObject*	pGameObject = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Kuro", m_tUnitInfo.iMaxHp, 25);
	//assert(pGameObject);
	//Management Add_GameObject(E_LAYERTAG::UI_Combat, pGameObject->Get_ObjTag(), pGameObject);
}

void CKuro::OnDisable()
{
	CBossMonster::OnDisable();
}

void CKuro::OnDestroy()
{
	CBossMonster::OnDestroy();
}
 
void CKuro::Trigger_Attack_1()
{
	//맵 변경
	if (m_fpLockFunction == &CUnit::ExSkill)
	{
		Set_Fade();
		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 1.f);
		Invoke_Add(1.f, bind(&CKuro::ChangePhase2, this));
	}

	if (m_fpLockFunction == &CUnit::Boss_Attack_3)
	{
		GroundBreak();
	}

	//아무데나 패턴
	if (m_fpLockFunction == &CUnit::Boss_Attack_1)
	{
		switch (m_iPhase)
		{
		case 1:
			Phase1_EX();
			break;
		case 2:
			Phase2_EX();
			break;
		case 3:
			Phase3_EX();
			break;
		default:
			break;
		}
	}

	//중앙 패턴
	if (m_fpLockFunction == &CUnit::Boss_Attack_2)
	{
		switch (m_iPhase)
		{
		case 1:
			Phase1_Center_EX();
			break;
		case 2:
			Phase2_Center_EX();
			break;
		case 3:
			Phase3_Center_EX();
			break;
		default:
			break;
		}
		
	}
}

void CKuro::Trigger_Attack_2()
{
	
}

void CKuro::Trigger_Reload()
{
	if (m_fpLockFunction == &CUnit::Boss_Attack_4)
	{
	
		CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Crow.particle");
		pParticle->Set_Pos(m_pSphereCollider->GetPos());
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
		m_bRender = FALSE;
		m_bAnimPlay = FALSE;

		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Crows_Sound.mp3", 1.f);
	}
}

void CKuro::Trigger_Buff_1()
{
	if (m_fpLockFunction == &CUnit::Boss_Attack_4)
	{
		CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Warp.particle");
		pParticle->Set_Pos(m_pSphereCollider->GetPos());
		m_pWarpParticle = pParticle;
		pParticle->AddRef();

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
	}
}

void CKuro::Trigger_Buff_2()
{
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_EXSkill.particle");
	pParticle->Set_Pos(m_pSphereCollider->GetPos());

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SE_Magic_02.wav", 1.f);

	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
}

void CKuro::Trigger_1()
{

}

void CKuro::Trigger_2()
{
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Crow.particle");
	pParticle->Set_Pos(m_pSphereCollider->GetPos());
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, m_pSphereCollider->GetPos(), Get_Angle(), 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Crows_Sound.mp3", 1.f);
}

void CKuro::Trigger_3()
{
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_EXSkill.particle");
	pParticle->Set_Pos(m_pSphereCollider->GetPos());

	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	//m_pAuraObject = CSkill_Manager::GetInstance()->SetSkill(L"Kuro_Aura.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 2.f);
	//m_pAuraObject->AddRef();
}

void CKuro::Trigger_4()
{

}


void CKuro::Phase1_EX()
{
	TripleExplosion();
}

void CKuro::Phase2_EX()
{
	
	_int iRand = CWall512::GetInstance()->GetValue(0, 10000) % 3;

	if (m_iUseSkillIndex < 3)
	{
		iRand = m_iUseSkillIndex;
	}

	switch (iRand)
	{
	case 0:
		TripleExplosion();
		break;
	case 1:
		BumpCar();
		break;
	case 2:
		DoubleDonut();
		break;
	default:
		break;
	}

	++m_iUseSkillIndex;
}

void CKuro::Phase3_EX()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 10000) % 2;

	if (m_iUseSkillIndex < 2)
	{
		iRand = m_iUseSkillIndex;
	}

	switch (iRand)
	{
	case 0:
		SpinCup();
		break;
	case 1:
		BumpCar();
		break;
	default:
		break;
	}

	++m_iUseSkillIndex;
}

void CKuro::Phase1_Center_EX()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 10000) % 1;

	switch (iRand)
	{
	case 0:
		SpinLaser();
		break;
	default:
		break;
	}
}

void CKuro::Phase2_Center_EX()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 10000) % 2;

	if (m_iUseCenterSkillIndex < 2)
	{
		iRand = m_iUseCenterSkillIndex;
	}

	switch (iRand)
	{
	case 0:
		SpinLaser();
		break;
	case 1:
		DoubleDonut();
		break;
	default:
		break;
	}

	++m_iUseCenterSkillIndex;
}

void CKuro::Phase3_Center_EX()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 10000) % 3;

	if (m_iUseCenterSkillIndex < 3)
	{
		iRand = m_iUseCenterSkillIndex;
	}

	switch (iRand)
	{
	case 0:
		SpinHorse();
		break;
	case 1:
		FireShoot();
		break;
	case 2:
		DoubleBumpCar();
		break;
	default:
		break;
	}

	++m_iUseCenterSkillIndex;
}

void CKuro::Invoke_AttackFan()
{
	m_fDamageRatio = 2.f;

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_FanExplosion.particle");
	pParticle->Set_Pos(Get_Position());
	pParticle->Set_Rotation(Get_Angle());
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CCollisionManager::GetInstance()->Collision_FanToCircle(this, Get_Position(), Get_Info(E_INFO::INFO_LOOK), 45.f, 10.f, E_LAYERTAG::Character, ColPlayer);
	
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_L_03.wav", 1.f);
	Set_Shake(0.3f, 0.6f, 0.1f);
}

void CKuro::Invoke_AttackStraight()
{
	m_fDamageRatio = 2.f;

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Straight.particle");
	pParticle->Set_Pos(Get_Position());
	pParticle->Set_Rotation(Get_Angle());
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position(), Get_Info(E_INFO::INFO_LOOK), 4.f, 10.f, E_LAYERTAG::Character, ColPlayer);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_L_03.wav", 1.f);
	Set_Shake(0.3f, 0.6f, 0.1f);
}

void CKuro::Invoke_AttackDonut()
{
	m_fDamageRatio = 2.f;

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_DonutExplosion.particle");
	pParticle->Set_Pos(Get_Position());
	pParticle->Set_Rotation(Get_Angle());
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CCollisionManager::GetInstance()->Collision_DonutToCircle(this, Get_Position(), 3.f, 5.f, E_LAYERTAG::Character, ColPlayer);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_L_03.wav", 1.f);
	Set_Shake(0.3f, 0.6f, 0.1f);
}

void CKuro::Invoke_AttackOutDonut()
{
	m_fDamageRatio = 2.f;

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_OutDonutExplosion.particle");
	pParticle->Set_Pos(Get_Position());
	pParticle->Set_Rotation(Get_Angle());
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CCollisionManager::GetInstance()->Collision_DonutToCircle(this, Get_Position(), 5.f, 10.f, E_LAYERTAG::Character, ColPlayer);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_L_03.wav", 1.f);
	Set_Shake(0.3f, 0.6f, 0.1f);
}

void CKuro::Invoke_ExplosionCircle(_int iIndex)
{
	m_fDamageRatio = 3.f;

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Explosion_Circle.particle");
	pParticle->Set_Pos(m_vecExplosionPos[iIndex]);
	pParticle->Set_Scale(VEC_ONE * 1.5f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
	CCollisionManager::GetInstance()->Collision_SpotToCircle(this, m_vecExplosionPos[iIndex], 1.f, E_LAYERTAG::Character, ColPlayer);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Blue_L_01.wav", 1.f);
	Set_Shake(0.3f, 0.6f, 0.1f);

}

void CKuro::Invoke_ExplosionCircleGuide(_int iIndex)
{

	CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vecExplosionPos[iIndex], 1.f, 3.f, 30, TRUE);

	Invoke_Add(3.f, bind(&CKuro::Invoke_ExplosionCircle, this, iIndex), iIndex);
}

void CKuro::Invoke_FireShoot()
{
	
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Fire.particle");
	pParticle->Set_Pos(m_tFireShootGuide.vPosition);
	pParticle->Set_Rotation(m_tFireShootGuide.vRotation);
	pParticle->Set_Scale(1.f, 1.f, 1.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
	
	
}

void CKuro::Invoke_FireShootCol()
{
	m_fDamageRatio = 0.5f;

	CCollisionManager::GetInstance()->Collision_FanToCircle_Guide(this, m_tFireShootGuide, E_LAYERTAG::Character, ColPlayer);
}

void CKuro::Invoke_FadeIn()
{
	m_bFadeOut = FALSE;
}

void CKuro::Normal_AttackPattern()
{

	//CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position(), Get_Angle(), 10.f, 4.f, 3.f, TRUE);
	//CSkill_Manager::GetInstance()->SetSkillGuideDonut(Get_Position(), 3.f, 5.f, 3.f, 100, TRUE);
	//CSkill_Manager::GetInstance()->SetSkillGuideCircle(Get_Position(), 10.f, 3.f, 100, TRUE);
	//CSkill_Manager::GetInstance()->SetSkillGuideCutDonut(Get_Position(), Get_Angle(), 3.f, 5.f, 180.f, 3.f, 50, TRUE);

	switch (m_iPhase)
	{
	case 1:
		Normal_Pattern_Phase1();
		break;
	case 2:
		Normal_Pattern_Phase2();
		break;
	case 3:
		Normal_Pattern_Phase3();
		break;
	default:
		break;
	}
	
}

void CKuro::Normal_Pattern_Phase1()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 100);
	iRand = iRand % 4;

	switch (iRand)
	{
	case 0:
		Normal_AttackStraight();
		break;
	case 1:
		Normal_AttackFan();
		break;
	case 2:
		Normal_AttackDonut();
		break;
	case 3:
		Normal_AttackOutDonut();
		break;
	default:
		break;
	}
}

void CKuro::Normal_Pattern_Phase2()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 100);
	iRand = iRand % 4;

	switch (iRand)
	{
	case 0:
		Normal_AttackStraight();
		break;
	case 1:
		Normal_AttackFan();
		break;
	case 2:
		Normal_AttackDonut();
		break;
	case 3:
		Normal_AttackOutDonut();
		break;
	default:
		break;
	}
}

void CKuro::Normal_Pattern_Phase3()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 100);
	iRand = iRand % 5;

	switch (iRand)
	{
	case 0:
		Normal_AttackStraight();
		break;
	case 1:
		Normal_AttackFan();
		break;
	case 2:
		Normal_AttackDonut();
		break;
	case 3:
		Normal_AttackOutDonut();
		break;
	case 4:
		TripleExplosion();
	default:
		break;
	}
}



void CKuro::AttackGround(_int iIndex)
{
	_float fAngle = 15.f;
	fAngle += (_float)iIndex * 30.f;
	
	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);

	_vec3 vDir;
	D3DXVec3TransformNormal(&vDir, &VEC_RIGHT, &matRotY);

	CSkill_Manager::GetInstance()->SetSkillGuideCutDonut(m_vCenterPos, CMyMath::GetLookRotation(VEC_ZERO, vDir), 7.8f, 13.f, 30.f, 3.f, 10, TRUE);
	Invoke_Add(3.f, bind(&CKuro::InvokeAttackGround, this, iIndex), iIndex);

}

void CKuro::InvokeAttackGround(_int iIndex)
{
	_float fAngle = 15.f;
	fAngle += (_float)iIndex * 30.f;

	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);

	_vec3 vDir;
	D3DXVec3TransformNormal(&vDir, &VEC_RIGHT, &matRotY);

	_int iGroundIndex = iIndex;
	_bool bRight = TRUE;

	if (iIndex >= 6)
	{
		iGroundIndex -= 6;
		bRight = FALSE;
	}

	if (bRight)
	{
		static_cast<CKuroGround*>(m_vecRightGround[iGroundIndex])->Set_GroundDead();
		m_vecRightGround[iGroundIndex] = nullptr;
	}
	else
	{
		static_cast<CKuroGround*>(m_vecLeftGround[iGroundIndex])->Set_GroundDead();
		m_vecLeftGround[iGroundIndex] = nullptr;
	}

	_vec3 vPos = vDir * 9.5f + m_vCenterPos;
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"GroundBreak.particle");
	pParticle->Set_Pos(vPos);
	pParticle->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Explosion.particle");
	pParticle->Set_Pos(vPos);
	pParticle->Set_Scale(_vec3(3.f, 3.f, 3.f));
	pParticle->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Grenade_L_A_03.wav", 1.f);
	

	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	Set_Shake(0.5, 1.f, 0.2f);
}

void CKuro::SpinHorse()
{
	wstring wstrGuidePath = L"../../Reference/Resource/Texture/Test/GuideTexYellow.png";
	_bool m_bInside = FALSE;
	_float fAngle = 36.f;
	_float fAddAngle = 72.f;
	_float fDistance;

	_matrix matRotY;
	_vec3 vDir;
	for (_int i = 0; i < 4; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);
		D3DXVec3TransformNormal(&vDir, &VEC_LOOK, &matRotY);
		D3DXVec3Normalize(&vDir, &vDir);
		if (m_bInside)
		{
			fDistance = 3.f;
		}
		else
		{
			fDistance = 6.f;
		}
		CKuro_Horse* pHorse = CKuro_Horse::Create(m_pGraphicDev, Get_Position(), Get_Position() + vDir * fDistance, m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::AttackObj_Monster, 3.f);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, pHorse->Get_ObjTag(), pHorse);

		m_bInside = !m_bInside;
		fAngle += fAddAngle;
	}

	CSkill_Manager::GetInstance()->SetSkillGuideDonut(Get_Position(), 2.f, 4.9f, 3.f, 100, TRUE, wstrGuidePath);
	CSkill_Manager::GetInstance()->SetSkillGuideDonut(Get_Position(), 5.1f, 8.f, 3.f, 100, TRUE, wstrGuidePath);
}

void CKuro::DoubleBumpCar()
{
	wstring wstrGuidePath = L"../../Reference/Resource/Texture/Test/GuideTexYellow.png";
	for (_int i = 0; i < 2; ++i)
	{

		_float fValueLong = 20.f;
		_float fValueShort = 4.f;


		if (CWall512::GetInstance()->GetRandBoolean())
		{
			fValueLong = -fValueLong;
		}
		if (CWall512::GetInstance()->GetRandBoolean())
		{
			fValueShort = -fValueShort;
		}

		_vec3 vStartPos;
		_vec3 vTargetPos;
		vStartPos.y = 0.f;
		if (CWall512::GetInstance()->GetRandBoolean())
		{
			vStartPos.x = Get_Position().x + fValueLong;
			vStartPos.z = Get_Position().z + fValueShort;
			vTargetPos = Get_Position() + _vec3(0.f, 0.f, fValueShort);
		}
		else
		{
			vStartPos.x = Get_Position().x + fValueShort;
			vStartPos.z = Get_Position().z + fValueLong;
			vTargetPos = Get_Position() + _vec3(fValueShort, 0.f, 0.f);
		}

		_vec3 vLook = vTargetPos - vStartPos;
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);

		CKuro_Car* pCar = CKuro_Car::Create(m_pGraphicDev, vLook, vStartPos, m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::AttackObj_Monster, 3.f);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, pCar->Get_ObjTag(), pCar);

		CSkill_Manager::GetInstance()->SetSkillGuideStraight(vStartPos, vRot, 40.f, 4.f, 3.f, TRUE, wstrGuidePath);
	}
}

void CKuro::SpinLaser()
{
	_matrix matRotY;
	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	D3DXMatrixRotationY(&matRotY, 45.f *Deg2Rad);

	D3DXVec3TransformNormal(&vLook, &vLook, &matRotY);

	D3DXMatrixRotationY(&matRotY, 90.f * Deg2Rad);

	for (_int i = 0; i < 4; ++i)
	{
		CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Laser.particle");
		pParticle->Set_Pos(Get_Position() + vLook);
		pParticle->Add_Pos(_vec3(0.f, 0.3f, 0.f));
		_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);
		pParticle->Set_Rotation(vRot);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

		CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position() + vLook, vRot, 10.f, 1.f, 2.f, TRUE);

		pParticle->AddRef();
		m_vecLaserParticle.emplace_back(pParticle);

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotY);
	}

	m_fLaserSpawnTime = CScene::Get_PlayTime();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Beam.wav", 1.f);
	
}

void CKuro::FireShoot()
{
	m_tFireShootGuide = CSkill_Manager::GetInstance()->SetSkillGuideFan(Get_Position(), Get_Angle(), 8.f, 240.f, 3.f, 50, TRUE)->Get_GuideOption();
	Invoke_Add(3.f, bind(&CKuro::Invoke_FireShoot, this));

	_float fStartTime = 0.5f;
	_float fAddTime = 0.2f;
	for (_int i = 0; i < 10; ++i)
	{
		Invoke_Add(3.f + fStartTime, bind(&CKuro::Invoke_FireShootCol, this));
		fStartTime += fAddTime;
	}
}

void CKuro::Pizza()
{
}

void CKuro::TripleExplosion()
{
	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);



	_float fDistance = 2.f;
	_float fAddDistance = 2.5f;
	_float fStartAngle = -10.f;
	_float fAddAngle = 10.f;
	_matrix matRotY;

	_float fStartTime = 0.1f;
	_float fAddTime = 0.3f;

	for (_int i = 0; i < 4; ++i)
	{

		for (_int j = 0; j < 3; ++j)
		{
			_vec3 vExplosionPos;

			D3DXMatrixRotationY(&matRotY, (fStartAngle + j * fAddAngle) * Deg2Rad);
			D3DXVec3TransformNormal(&vExplosionPos, &vLook, &matRotY);
			vExplosionPos = vExplosionPos * fDistance + Get_Position();
			
			_int iIndex = i * 3 + j;
			m_vecExplosionPos[iIndex] = vExplosionPos;
			
			Invoke_Add(fStartTime, bind(&CKuro::Invoke_ExplosionCircleGuide, this, iIndex), iIndex);
		}
		fStartTime += fAddTime;
		fDistance += fAddDistance;
	}
}

void CKuro::BumpCar()
{
	_float fValueLong = 20.f;
	_float fValueShort = 4.f;

	wstring wstrGuidePath = L"../../Reference/Resource/Texture/Test/GuideTexYellow.png";
	if (CWall512::GetInstance()->GetRandBoolean())
	{
		fValueLong = -fValueLong;
	}
	if (CWall512::GetInstance()->GetRandBoolean())
	{
		fValueShort = -fValueShort;
	}

	_vec3 vStartPos;
	_vec3 vTargetPos;
	vStartPos.y = 0.f;
	if (CWall512::GetInstance()->GetRandBoolean())
	{
		vStartPos.x = Get_Position().x + fValueLong;
		vStartPos.z = Get_Position().z + fValueShort;
		vTargetPos = m_pTarget->Get_Position();
	}
	else
	{
		vStartPos.x = Get_Position().x + fValueShort;
		vStartPos.z = Get_Position().z + fValueLong;
		vTargetPos = m_pTarget->Get_Position();
	}

	_vec3 vLook = vTargetPos - vStartPos;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);

	CKuro_Car* pCar = CKuro_Car::Create(m_pGraphicDev, vLook, vStartPos, m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::AttackObj_Monster, 3.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, pCar->Get_ObjTag(), pCar);

	CSkill_Manager::GetInstance()->SetSkillGuideStraight(vStartPos, vRot, 40.f, 4.f, 3.f, TRUE, wstrGuidePath);
}

void CKuro::SpinCup()
{
	_vec3 vMinPos;
	_vec3 vMaxPos;

	vMinPos.y = 0.f;
	vMinPos.x = 999999.f;
	vMinPos.z = 999999.f;
	vMaxPos.y = 0.f;
	vMaxPos.x = -999999.f;
	vMaxPos.z = -999999.f;


	for (auto& pChar : *CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
	{
		if (static_cast<CCharacter*>(pChar)->Get_UnitDead())
		{
			continue;
		}

		_vec3 vPlayerPos = pChar->Get_Position();
		if (vPlayerPos.x < vMinPos.x)
		{
			vMinPos.x = vPlayerPos.x;
		}
		if (vPlayerPos.z < vMinPos.z)
		{
			vMinPos.z = vPlayerPos.z;
		}
		if (vPlayerPos.x > vMaxPos.x)
		{
			vMaxPos.x = vPlayerPos.x;
		}
		if (vPlayerPos.z > vMaxPos.z)
		{
			vMaxPos.z = vPlayerPos.z;
		}
	}


	for (_int i = 0; i < 3; ++i)
	{
		_vec3 vRandomPos = Get_Position();
		_vec3 vRight = Get_Info(E_INFO::INFO_RIGHT);
		_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vLook, &vLook);

		if (CWall512::GetInstance()->GetRandBoolean())
		{
			vRight = -vRight;
		}
		vRight *= CWall512::GetInstance()->GetFloatValue(3.f, 8.f);
		vLook *= CWall512::GetInstance()->GetFloatValue(-1.f, 2.f);
		vRandomPos += vRight + vLook;

		_vec3 vRandomTarget;
		vRandomTarget.y = 0.f;
		vRandomTarget.x = CWall512::GetInstance()->GetFloatValue(vMinPos.x, vMaxPos.x);
		vRandomTarget.z = CWall512::GetInstance()->GetFloatValue(vMinPos.z, vMaxPos.z);

		_vec3 vCupLook = vRandomTarget - vRandomPos;
		D3DXVec3Normalize(&vCupLook, &vCupLook);

		_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vCupLook);

		CKuro_Cup* pCup = CKuro_Cup::Create(m_pGraphicDev, vCupLook, vRandomPos, m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::AttackObj_Monster, 3.f);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, pCup->Get_ObjTag(), pCup);

		wstring wstrGuidePath = L"../../Reference/Resource/Texture/Test/GuideTexYellow.png";
		CSkill_Manager::GetInstance()->SetSkillGuideStraight(vRandomPos, vRot, 15.f, 2.f, 3.f, TRUE, wstrGuidePath);
	}
	
}

void CKuro::DoubleDonut()
{
	if (CWall512::GetInstance()->GetRandBoolean())
	{
		Normal_AttackOutDonut();
		Invoke_Add(2.f, bind(&CKuro::Normal_AttackDonut,this));
	}
	else
	{
		Normal_AttackDonut();
		Invoke_Add(2.f, bind(&CKuro::Normal_AttackOutDonut, this));
	}
}


void CKuro::Normal_AttackFan()
{

	CSkill_Manager::GetInstance()->SetSkillGuideFan(Get_Position(), Get_Angle(), 10.f, 45.f, 3.f, 10, TRUE);
	Invoke_Add(3.f, bind(&CKuro::Invoke_AttackFan, this));

}

void CKuro::Normal_AttackStraight()
{
	CSkillGuideObject* pSkill = CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position(), Get_Angle(), 10.f, 4.f, 3.f, TRUE);

	Invoke_Add(3.f, bind(&CKuro::Invoke_AttackStraight, this));
}

void CKuro::Normal_AttackDonut()
{
	CSkill_Manager::GetInstance()->SetSkillGuideDonut(Get_Position(), 3.f, 5.f, 3.f, 100, TRUE);
	Invoke_Add(3.f, bind(&CKuro::Invoke_AttackDonut, this));
}

void CKuro::Normal_AttackOutDonut()
{
	CSkill_Manager::GetInstance()->SetSkillGuideDonut(Get_Position(), 5.f, 10.f, 3.f, 100, TRUE);
	Invoke_Add(3.f, bind(&CKuro::Invoke_AttackOutDonut, this));
}

void CKuro::ChangePhase2()
{
	_matrix matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	m_vPrevCamPos = _vec3(matInvView._41, matInvView._42, matInvView._43);

	/*CScene::Get_MainScene()->Active_AutoCam();*/
	
	m_pAuraObject = CSkill_Manager::GetInstance()->SetSkill(L"Kuro_Aura.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 2.f);
	m_pAuraObject->AddRef();

	m_vCenterPos = Get_Position();

	CGameObject* pObj = nullptr;

	CScene::Get_MainScene()->Set_Dead_AllObject(E_LAYERTAG::Map);

	pObj = CStaticObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/slumpia/CenterMap.X");
	pObj->Set_Scale(2.f, 2.f, 2.f);
	pObj->Set_Pos(m_vCenterPos);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, pObj);
	
	_vec3 vLook;
	_matrix matRotY;

	_float fAngle = 15.f;

	m_vecLeftGround.reserve(6);
	m_vecRightGround.reserve(6);

	for (_int i = 0; i < 12; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);
		D3DXVec3TransformNormal(&vLook, &VEC_RIGHT, &matRotY);

		pObj = CKuroGround::Create(m_pGraphicDev, E_OBJTAG::OBJ_Map, L"../../Reference/Resource/Map/slumpia/SideMap.X");
		_vec3 vPos = vLook * 7.8f + m_vCenterPos;
		_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);
	
		pObj->Set_Pos(vPos);

		pObj->Set_Scale(2.f, 2.f, 2.f);
		pObj->Set_Rotation(vRot);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, pObj);

		if (i < 6)
		{
			m_vecRightGround.emplace_back(pObj);
		}
		else
		{
			m_vecLeftGround.emplace_back(pObj);
		}

		fAngle += 30.f;
	}

	for (auto& pChar : *CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
	{
		if (pChar)
		{
			_vec3 vDir = pChar->Get_Position() - m_vCenterPos;
			if (10.f < D3DXVec3Length(&vDir))
			{
				D3DXVec3Normalize(&vDir, &vDir);
				pChar->Add_Pos(m_vCenterPos + vDir * 10.f);
			}
			
		}
	}

	Set_Pos(m_vCenterPos);
	m_iPhase = 2;
	m_iUseSkillIndex = 0;
	m_iUseCenterSkillIndex = 0;
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_UnderGround.particle");
	pParticle->Set_Pos(m_vCenterPos);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
	
	pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Rain.particle");
	pParticle->Set_Pos(m_vCenterPos);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Far_Barrier.particle");
	pParticle->Set_Pos(m_vCenterPos);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	CScene::Get_MainScene()->Set_SkySphere(L"../../Reference/Resource/Texture/Test/fullmoon.jpg");

	m_vPhase2CamPos += m_vCenterPos;
	m_vPhase3CamPos += m_vCenterPos;

	m_bFadeOut = FALSE;
}

void CKuro::GroundBreak()
{
	_bool m_bLeft = TRUE;
	if (!m_vecLeftGround.front())
	{
		m_bLeft = FALSE;
		m_fRatio = 0.f;
		m_iPhase = 3;
		m_iUseSkillIndex = 0;
		m_iUseCenterSkillIndex = 0;
	}

	if (!m_vecRightGround.front())
	{
		m_fRatio = 0.f;
		m_iPhase = 3;
		m_iUseSkillIndex = 0;
		m_iUseCenterSkillIndex = 0;
		if (!m_bLeft)
		{
			return;
		}
	}
	else
	{
		if (m_bLeft)
		{
			if (CWall512::GetInstance()->GetRandBoolean())
			{
				m_bLeft = FALSE;
			}
		}
	}
	_int iAddIndex = 0;
	if (m_bLeft)
	{
		iAddIndex = 6;
	}

	AttackGround(2 + iAddIndex);
	AttackGround(3 + iAddIndex);
	
	Invoke_Add(0.3f, bind(&CKuro::AttackGround, this, 1 + iAddIndex), 1 + iAddIndex);
	Invoke_Add(0.3f, bind(&CKuro::AttackGround, this, 4 + iAddIndex), 4 + iAddIndex);

	Invoke_Add(0.6f, bind(&CKuro::AttackGround, this, 0 + iAddIndex), 0 + iAddIndex);
	Invoke_Add(0.6f, bind(&CKuro::AttackGround, this, 5 + iAddIndex), 5 + iAddIndex);
}

_vec3 CKuro::Get_RandomWarpPos()
{
	_float fLongDistance = 10.f;
	_float fShortDistance = 7.f;

	while (TRUE)
	{
		_float fRandomX = CWall512::GetInstance()->GetFloatValue(-fLongDistance, fLongDistance);
		_float fRandomZ = CWall512::GetInstance()->GetFloatValue(-fLongDistance, fLongDistance);

		_vec3 vPos = _vec3(fRandomX, 0.f, fRandomZ);

	

		if (D3DXVec3Length(&(vPos + m_vCenterPos - Get_Position())) < 1.f)
		{
			continue;
		}

		if (CCollisionManager::GetInstance()->Collision_SpotToCircle(this, vPos + m_vCenterPos, 2.f, E_LAYERTAG::Character, nullptr))
		{
			continue;
		}

		_float fAngle = CMyMath::GetAngleToTarget_vec3(VEC_RIGHT, vPos);
		_float fDis = D3DXVec3Length(&vPos);
		if (vPos.z > 0.f)
		{
			fAngle = -fAngle;
		}

		if (fAngle > 0.f)
		{
			if (m_vecRightGround.front())
			{
				if (fDis > fLongDistance)
				{
					continue;
				}
			}
			else
			{
				if (fDis > fShortDistance)
				{
					continue;
				}
			}
		}
		else
		{
			if (m_vecLeftGround.front())
			{
				if (fDis > fLongDistance)
				{
					continue;
				}
			}
			else
			{
				if (fDis > fShortDistance)
				{
					continue;
				}
			}
		}

		return (vPos + m_vCenterPos);
	}
}

void CKuro::SpinLaserUpdate(_float fTimeDelta)
{
	if (m_vecLaserParticle.empty())
	{
		return;
	}

	if (m_fLaserSpawnTime + 2.f < CScene::Get_PlayTime())
	{
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY, fTimeDelta * 30.f * Deg2Rad);

		for (auto& pLaser : m_vecLaserParticle)
		{
			_vec3 vPos = pLaser->Get_Position();
			vPos.y = 0.f;
			_vec3 vDir = vPos - Get_Position();
			D3DXVec3TransformNormal(&vDir, &vDir, &matRotY);
			pLaser->Set_Pos(vDir + Get_Position());
			pLaser->Add_Pos(_vec3(0.f, 0.3f, 0.f));
			pLaser->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));

			D3DXVec3Normalize(&vDir, &vDir);
			//레이 충돌
			vPos = pLaser->Get_Position();
			vPos.y = 0.15f;
			if (CCollisionManager::GetInstance()->Collision_RayToSphere(this, vPos, vDir, 10.f, E_LAYERTAG::Character, ColRay))
			{
				_int iA = 123;
			}
		}
	}

	if (m_fLaserSpawnTime + 5.f < CScene::Get_PlayTime())
	{
		for (auto& pLaser : m_vecLaserParticle)
		{
			pLaser->Set_ParticleDead();
			Safe_Release(pLaser);
		}
		m_vecLaserParticle.clear();

	}

	
}

void CKuro::Set_Shake(_float fMinShake, _float fMaxShake, _float fShakeDelay)
{
	m_bShake = TRUE;
	m_fShakeTimer = 0.f;
	m_fMinShake = fMinShake;
	m_fMaxShake = fMaxShake;
	m_fShakeDelay = fShakeDelay;
}

void CKuro::Update_Shake(_float fTiemDelta)
{
	if (m_bShake)
	{
		if (m_fShakeTimer > m_fShakeDelay)
		{
			m_bShake = FALSE;
		}
		else
		{
			m_fShakeTimer += fTiemDelta;
			CScene::Get_MainScene()->Active_ShakeCam(m_fMaxShake, m_fMinShake);
		}
	
	}
}

void CKuro::Update_Fade(_float fTimeDelta)
{
	if (!m_pFadeUI)
	{
		return;
	}

	if (m_bFadeOut)
	{
		m_fFadeAlpha += fTimeDelta;
		if (m_fFadeAlpha > 1.f)
		{
			m_fFadeAlpha = 1.f;
		}
	}
	else
	{
		m_fFadeAlpha -= fTimeDelta;
		if (m_fFadeAlpha < 0.f)
		{
			m_pFadeUI->Set_Dead(TRUE);
			m_pFadeUI = nullptr;
		}
	}

	if (m_pFadeUI)
	{
		m_pFadeUI->Set_ImageAlpha(m_fFadeAlpha);
	}
}

void CKuro::Set_Fade()
{
	m_bFadeOut = TRUE;
	m_pFadeUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Fade.uidat");
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pFadeUI);
	m_pFadeUI->Set_ImageAlpha(0.f);
}

_bool CKuro::Update_KuroCam(_float fTimeDelta)
{
	if (m_iPhase < 2)
	{
		return FALSE;
	}
	_vec3 vStartPoint;
	_vec3 vCamDiff;
	if (m_iPhase == 2)
	{
		vStartPoint = m_vPrevCamPos;
		vCamDiff = m_vPhase2CamPos - m_vPrevCamPos;
	}
	else
	{
		vStartPoint = m_vPhase2CamPos;
		vCamDiff = m_vPhase3CamPos - m_vPhase2CamPos;
	}

	m_fRatio += fTimeDelta * 0.2f;
	if (m_fRatio > 1.f)
	{
		m_fRatio = 1.f;
	}

	_vec3 vCamPos = vStartPoint + vCamDiff * m_fRatio;

	CCamera::m_pMainCam->Set_CamEye(vCamPos);
	CCamera::m_pMainCam->Set_CamAt(m_vCenterPos);

	return TRUE;
}


_bool CKuro::HPEventCheck()
{
	_float fPer = (_float)m_tUnitInfo.iNowHp / (_float)m_tUnitInfo.iMaxHp;

	switch (m_iEventCount)
	{
	case 0:
		if (fPer < 0.9f)
		{
			++m_iEventCount;
			
			FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
			m_bInvn = TRUE;
			return TRUE;
		}
		
		break;
	case 1:
		if (fPer < 0.75f)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Boss_Attack_3);
			++m_iEventCount;
			m_bInvn = TRUE;
			return TRUE;
		}
		break;
	case 2:
		if (fPer < 0.6f)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_4, &CUnit::Boss_Attack_3);
			++m_iEventCount;
			m_bInvn = TRUE;
			return TRUE;
		}
		break;
	default:
		break;
	}
	
	return FALSE;
}

void CKuro::ColPlayer(CGameObject * pThis, CGameObject * pPlayer)
{
	CMonster* pMonster = static_cast<CMonster*>(pThis);
	static_cast<CUnit*>(pPlayer)->Set_Damage(pMonster->Get_UnitInfo(), pMonster->Get_MonsterInfo().eAtkType, static_cast<CKuro*>(pThis)->Get_DamageRatio());
}

void CKuro::ColRay(CGameObject * pThis, CGameObject * pPlayer)
{
	CMonster* pMonster = static_cast<CMonster*>(pThis);
	static_cast<CUnit*>(pPlayer)->Set_Damage(pMonster->Get_UnitInfo(), pMonster->Get_MonsterInfo().eAtkType, 0.8f);
}

