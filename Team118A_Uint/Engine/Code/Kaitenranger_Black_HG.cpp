#include "Kaitenranger_Black_HG.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKaitenranger_Black_HG::CKaitenranger_Black_HG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Kaitenranger_Black_HG")
{
}

CKaitenranger_Black_HG::CKaitenranger_Black_HG(const CKaitenranger_Black_HG & rhs)
	: CMonster(rhs)
{
}

CKaitenranger_Black_HG::~CKaitenranger_Black_HG()
{
}

CKaitenranger_Black_HG * CKaitenranger_Black_HG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKaitenranger_Black_HG* pInstance = new CKaitenranger_Black_HG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKaitenranger_Black_HG::Free()
{
	CMonster::Free();
}

HRESULT CKaitenranger_Black_HG::Ready_Object()
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

	m_fMonsterExSkillOriginTimer = 9.f;
	m_fMonsterExSkillTimer = 3.f;

	return S_OK;
}

_int CKaitenranger_Black_HG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CKaitenranger_Black_HG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	if (0.f >= m_tUnitInfo.iNowHp)
	{
		Release_Invoke();
	}
	return RETURN_OK;
}

_int CKaitenranger_Black_HG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CKaitenranger_Black_HG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CKaitenranger_Black_HG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CKaitenranger_Black_HG::OnEnable()
{
	CMonster::OnEnable();
}

void CKaitenranger_Black_HG::OnDisable()
{
	CMonster::OnDisable();
}

void CKaitenranger_Black_HG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CKaitenranger_Black_HG::Trigger_Attack_1()
{
	m_tMonsterInfo.eWeaponType = m_tMonsterInfo.eWeaponType;

	m_tUnitInfo.iAttack = 100;

	// 권총 총격
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Black_Shot_1.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.3f), Get_Angle(), 1.f);

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_END;

	Hit_Target();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_HG_03.wav", 1.f);
}

void CKaitenranger_Black_HG::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_HG_Reload_03.wav", m_fSoundVolume);
}

void CKaitenranger_Black_HG::Trigger_1()
{
	// 데미지를 준다
	m_tUnitInfo.iAttack = 40;

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_EXS;

	// 공격
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Black_Shot_1.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.3f), Get_Angle(), 2.2f);

	Hit_Target();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Normal_B_exs.wav", 1.f);
}

void CKaitenranger_Black_HG::Trigger_2()
{
	// 스킬 시작
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());
}

void CKaitenranger_Black_HG::Trigger_3()
{
	// 스킬 시작 _ 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Black_0.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Ex_04_Blading_Public_exs.wav", 1.f);

}

void CKaitenranger_Black_HG::Trigger_4()
{
	// 스킬 공격 전 바람
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Black_Shot_0.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.3f), Get_Angle(), 2.2f);

}

void CKaitenranger_Black_HG::Create_HitEffect()
{
	// 랜덤 값 생성
	_float fRad = m_pTarget->Get_SphereCollider()->GetRadius();
	_float fRand_X = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
	_float fRand_Y = CWall512::GetInstance()->GetFloatValue(-fRad * 0.1f, fRad * 0.3f);
	_float fRand_Z = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
	_vec3 vRandom = m_pTarget->Get_SphereCollider()->GetPos() + _vec3(fRand_X, fRand_Y, fRand_Z);

	if (E_UNIT_HIT_TYPE::HIT_EXS == m_eUnitHitType)
	{
		//CSkill_Manager::GetInstance()->SetSkill(L"Kai_Green_Hit_0.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());

		CUnit::Create_HitEffect();

		if (m_pTarget)
		{
			m_pTarget->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_DEF, 2.f));
			m_pTarget->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_DEF, 4.f));
			m_pTarget->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_DEF, 6.f));
		}
	}
	else
	{
		CUnit::Create_HitEffect();
	}
}
