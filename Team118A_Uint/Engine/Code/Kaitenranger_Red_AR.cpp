#include "Kaitenranger_Red_AR.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKaitenranger_Red_AR::CKaitenranger_Red_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Kaitenranger_Red_AR")
{
}

CKaitenranger_Red_AR::CKaitenranger_Red_AR(const CKaitenranger_Red_AR & rhs)
	: CMonster(rhs)
{
}

CKaitenranger_Red_AR::~CKaitenranger_Red_AR()
{
}

CKaitenranger_Red_AR * CKaitenranger_Red_AR::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKaitenranger_Red_AR* pInstance = new CKaitenranger_Red_AR(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKaitenranger_Red_AR::Free()
{
	CMonster::Free();
}

HRESULT CKaitenranger_Red_AR::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	Set_AmmoMax(2);

	m_tUnitInfo.iAttack = 30;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 20000;
	m_tUnitInfo.iNowHp = 20000;
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;

	m_fMonsterExSkillOriginTimer = 12.f;
	m_fMonsterExSkillTimer = 3.f;

	return S_OK;
}

_int CKaitenranger_Red_AR::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CKaitenranger_Red_AR::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	if (0.f >= m_tUnitInfo.iNowHp)
	{
		Release_Invoke();
	}
	return RETURN_OK;
}

_int CKaitenranger_Red_AR::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CKaitenranger_Red_AR::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CKaitenranger_Red_AR::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CKaitenranger_Red_AR::OnEnable()
{
	CMonster::OnEnable();
}

void CKaitenranger_Red_AR::OnDisable()
{
	CMonster::OnDisable();
}

void CKaitenranger_Red_AR::OnDestroy()
{
	CMonster::OnDestroy();
}

void CKaitenranger_Red_AR::Trigger_Attack_1()
{
	m_tMonsterInfo.eWeaponType = m_tMonsterInfo.eWeaponType;

	m_tUnitInfo.iAttack = 10;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.77f), Get_Angle(), 1.7f);

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_END;

	Hit_Target();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_01.wav", 1.f);
}

void CKaitenranger_Red_AR::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_Reload_02.wav", 1.f);
}

void CKaitenranger_Red_AR::Trigger_1()
{
	// 도발 효과
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Red_2.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);


	if (m_pTarget)
	{
		m_pTarget->CC_Taunt(5.f, this);
		m_pTarget->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_OppressionResist, 3.f));
	}


	for (auto& iter : *CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::Monster)->Get_vecGameObject(E_OBJTAG::OBJ_Monster))
	{
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_DEF, 3.f));
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_Dodge, 3.f));
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_HIT, 5.f));
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_Mind, 5.f));
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_MoveSpeed, 5.f));
		static_cast<CUnit*>(iter)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_Range, 5.f));
	}

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"FX_Public_버프.wav", 1.f);
}

void CKaitenranger_Red_AR::Trigger_2()
{
	// 스킬 시작
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

}

void CKaitenranger_Red_AR::Trigger_3()
{
	// 스킬 시작 _ 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Red_0.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Ex_04_Blading_Public_exs.wav", 1.f);

}

void CKaitenranger_Red_AR::Trigger_4()
{
	// 도발 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Red_1.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_EX04_R_exs.wav", 1.f);
}

void CKaitenranger_Red_AR::Create_HitEffect()
{
	CUnit::Create_HitEffect();

}
