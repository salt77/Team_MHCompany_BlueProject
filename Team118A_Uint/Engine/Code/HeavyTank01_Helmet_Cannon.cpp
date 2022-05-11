#include "HeavyTank01_Helmet_Cannon.h" 

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

#include "UI_Manager.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CHeavyTank01_Helmet_Cannon::CHeavyTank01_Helmet_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"HeavyTank01_Helmet_Cannon")
{
}

CHeavyTank01_Helmet_Cannon::CHeavyTank01_Helmet_Cannon(const CHeavyTank01_Helmet_Cannon & rhs)
	: CMonster(rhs)
{
}

CHeavyTank01_Helmet_Cannon::~CHeavyTank01_Helmet_Cannon()
{
}

CHeavyTank01_Helmet_Cannon * CHeavyTank01_Helmet_Cannon::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _bool bIsBoss)
{
	CHeavyTank01_Helmet_Cannon* pInstance = new CHeavyTank01_Helmet_Cannon(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(bIsBoss)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeavyTank01_Helmet_Cannon::Free()
{
	CMonster::Free();
}

HRESULT CHeavyTank01_Helmet_Cannon::Ready_Object(_bool bIsBoss)
{
	CMonster::Ready_Object();

	m_bNormalBoss = bIsBoss;

	m_szBodyBoneName = "bone_root";
	m_szWeaponBoneName = "Weapon_01";
	Create_Collider(&m_pSecondWeaponCol, L"Collider_SecondWeapon", 0.1f);

	Set_AmmoMax(2);

	m_tUnitInfo.iAttack = 50;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_bNormalBoss ? m_tUnitInfo.iMaxHp = 50000 : m_tUnitInfo.iMaxHp = 35000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 1000;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CHeavyTank01_Helmet_Cannon::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	m_fMonsterExSkillTimer = CWall512::GetInstance()->GetFloatValue(7.f, 10.f);

	if (m_bNormalBoss)
	{
		m_bNormalBoss ? m_tUnitInfo.iMaxHp = 50000 : m_tUnitInfo.iMaxHp = 35000;
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;

		m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"무거운 헬멧단 탱크", m_tUnitInfo.iMaxHp, 10, L"[Fixed]Boss_Hp_Bar_Normal.uidat");
		assert(m_pBossHpBar);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI_BossHpbar, m_pBossHpBar);
	}

	return RETURN_OK;
}

_int CHeavyTank01_Helmet_Cannon::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pSecondWeaponCol, m_szWeaponSecond);

	return RETURN_OK;
}

_int CHeavyTank01_Helmet_Cannon::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CHeavyTank01_Helmet_Cannon::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CHeavyTank01_Helmet_Cannon::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CHeavyTank01_Helmet_Cannon::OnEnable()
{
	CMonster::OnEnable();
}

void CHeavyTank01_Helmet_Cannon::OnDisable()
{
	CMonster::OnDisable();
}

void CHeavyTank01_Helmet_Cannon::OnDestroy()
{
	CMonster::OnDestroy();
}

void CHeavyTank01_Helmet_Cannon::Trigger_Attack_1()
{
	Fire_NormalAttack(m_pWeaponCollider);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume); 
}

void CHeavyTank01_Helmet_Cannon::Trigger_Attack_2()
{
	Fire_NormalAttack(m_pSecondWeaponCol);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_1()
{
	Fire_ExSkill(0, m_pWeaponCollider);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_2()
{
	Fire_ExSkill(1, m_pSecondWeaponCol);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_3()
{
	Fire_ExSkill(2, m_pWeaponCollider);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_4()
{
	Fire_ExSkill(3, m_pSecondWeaponCol);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_5()
{
	Fire_ExSkill(4, m_pWeaponCollider, m_pSecondWeaponCol);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_03.wav", m_fSoundVolume + 0.25f); 
}

void CHeavyTank01_Helmet_Cannon::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Tank_Appear_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Tank_Appear_02.wav", m_fSoundVolume);
}

void CHeavyTank01_Helmet_Cannon::Trigger_9()
{
	if (!m_pTarget)
	{
		return;
	}

	m_vTargetPos = m_pTarget->Get_Position();
	m_vRotateDir = *D3DXVec3Normalize(&m_vRotateDir, &(Get_Position() - m_vTargetPos));

	CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos, m_fAttackRange, 4.8f, 30, TRUE);

	for (_uint i = 0; i < 4; ++i)
	{
		CUtility::Rotate_DirVector(90.f * i, VEC_UP, &m_vRotateDir);

		switch (i)
		{
		case 0:
			CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos + m_vRotateDir * 3.f, m_fAttackRange, 0.82f, 30, TRUE);
			break;

		case 1:
			CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos + m_vRotateDir * 3.f, m_fAttackRange, 1.36f, 30, TRUE);
			break;

		case 2:
			CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos + m_vRotateDir * 3.f, m_fAttackRange, 1.77f, 30, TRUE);
			break;

		case 3:
			CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos + m_vRotateDir * 3.f, m_fAttackRange, 2.62f, 30, TRUE);
			break;
		}
	}

	m_vRotateDir = *D3DXVec3Normalize(&m_vRotateDir, &(Get_Position() - m_vTargetPos)); 
}

void CHeavyTank01_Helmet_Cannon::Trigger_Dead()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Common_Death_01.wav", m_fSoundVolume); 
}

void CHeavyTank01_Helmet_Cannon::Fire_NormalAttack(CSphereCollider* pCollider)
{
	if (!m_pTarget)
	{
		return;
	}

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 1.f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = pCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	Create_FireEffect();

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev, vWeaponPos, *D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)), m_tUnitInfo, m_tMonsterInfo.eAtkType, CBullet_Normal_Monster::Move_TankBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CHeavyTank01_Helmet_Cannon::Create_FireEffect()
{
	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Smoke_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Normal.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);
}

void CHeavyTank01_Helmet_Cannon::Fire_ExSkill(_uint iExFireCount, CSphereCollider* pCollider_1, CSphereCollider* pCollider_2)
{
	if (VEC_ZERO == m_vTargetPos)
	{
		return;
	}

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 1.f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = pCollider_1->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_Ex_a_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_Ex_a_02.wav", m_fSoundVolume);

	Create_FireEffect();

	if (pCollider_2)
	{
		vWeaponPos = pCollider_2->GetPos();
		vWeaponPos += vDirLook * fLookRatio;
		vWeaponPos += vDirRight * fRightRatio;
		vWeaponPos += vDirUp * fUpRatio;

		CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
		CSkill_Manager::GetInstance()->SetSkill(L"Test_Explosion.skill", E_LAYERTAG::Effect, m_vTargetPos, VEC_ZERO, m_fAttackRange);
		CScene::Get_MainScene()->Active_ShakeCam(1.f, 0.13f);

		Create_FireEffect();
	}

	if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
	{
		vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);

		CUtility::Rotate_DirVector(90.f * iExFireCount + 1, VEC_UP, &m_vRotateDir);

		_vec3	vTargetPos = (4 == iExFireCount) ? m_vTargetPos : m_vTargetPos + m_vRotateDir * 3.f;

		if (4 != iExFireCount)
		{
			CSkill_Manager::GetInstance()->SetSkill(L"TankBullet_Ground.skill", E_LAYERTAG::Effect, vTargetPos, VEC_ZERO, m_fAttackRange); 
			CScene::Get_MainScene()->Active_ShakeCam(0.75f, 0.08f); 
		}

		for (_uint i = 0; i < vecObject->size(); ++i)
		{
			if (m_fAttackRange >= CUtility::Distance_vec3(vTargetPos, (*vecObject)[i]->Get_Position()))
			{
				static_cast<CUnit*>((*vecObject)[i])->Set_Damage(m_tUnitInfo, m_tMonsterInfo.eAtkType);
				
				if (4 == iExFireCount)
				{
					static_cast<CUnit*>((*vecObject)[i])->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Confusion, 3.f)); 
				}
			}
		}
	}
}
