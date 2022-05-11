#include "Kaitenranger_Pink_RL.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKaitenranger_Pink_RL::CKaitenranger_Pink_RL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Kaitenranger_Pink_RL")
{
}

CKaitenranger_Pink_RL::CKaitenranger_Pink_RL(const CKaitenranger_Pink_RL & rhs)
	: CMonster(rhs)
{
}

CKaitenranger_Pink_RL::~CKaitenranger_Pink_RL()
{
}

CKaitenranger_Pink_RL * CKaitenranger_Pink_RL::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKaitenranger_Pink_RL* pInstance = new CKaitenranger_Pink_RL(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKaitenranger_Pink_RL::Free()
{
	CMonster::Free();
}

HRESULT CKaitenranger_Pink_RL::Ready_Object()
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

	m_fMonsterExSkillOriginTimer = 10.f;
	m_fMonsterExSkillTimer = 3.f;

	return S_OK;
}

_int CKaitenranger_Pink_RL::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CKaitenranger_Pink_RL::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	if (0.f >= m_tUnitInfo.iNowHp)
	{
		Release_Invoke();
	}
	return RETURN_OK;
}

_int CKaitenranger_Pink_RL::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CKaitenranger_Pink_RL::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CKaitenranger_Pink_RL::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CKaitenranger_Pink_RL::OnEnable()
{
	CMonster::OnEnable();
}

void CKaitenranger_Pink_RL::OnDisable()
{
	CMonster::OnDisable();
}

void CKaitenranger_Pink_RL::OnDestroy()
{
	CMonster::OnDestroy();
}

void CKaitenranger_Pink_RL::Trigger_Attack_1()
{
	m_tMonsterInfo.eWeaponType = m_tMonsterInfo.eWeaponType;

	m_tUnitInfo.iAttack = 100;

	// 슈퍼 탄 발사 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Pink_Shot_0.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.6f), Get_Angle(), 1.f);

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_END;

	Release_Invoke();
	Invoke_Add(1.f, bind(&CKaitenranger_Pink_RL::Hit_Target, this));

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_GL_02.wav", 1.f);
}

void CKaitenranger_Pink_RL::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_GL_Reload_01.wav", 1.f);
}

void CKaitenranger_Pink_RL::Trigger_1()
{
	m_tMonsterInfo.eWeaponType = m_tMonsterInfo.eWeaponType;

	m_tUnitInfo.iAttack = 400;

	// 슈퍼 탄 발사 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Pink_Shot_0.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.6f), Get_Angle(), 1.f);

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_EXS;

	Release_Invoke();
	Invoke_Add(1.f, bind(&CKaitenranger_Pink_RL::Hit_Target, this));
}

void CKaitenranger_Pink_RL::Trigger_2()
{
	// 스킬 시작
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

}

void CKaitenranger_Pink_RL::Trigger_3()
{
	// 스킬 시작 _ 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Pink_0.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Ex_04_Blading_Public_exs.wav", 1.f);

}

void CKaitenranger_Pink_RL::Trigger_4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_GL_02.wav", 1.f);
}

void CKaitenranger_Pink_RL::Create_HitEffect()
{
	if (E_UNIT_HIT_TYPE::HIT_EXS == m_eUnitHitType)
	{
		// 소규모 폭발
		CSkill_Manager::GetInstance()->SetSkill(L"Mushroom_Cloud.skill", E_LAYERTAG::Effect, m_pTarget->Get_SphereCollider()->GetPos(), m_pTarget->Get_Angle(), 1.f);

		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_NormalAttack_Hit_Start.wav", 1.f);
	}
	else
	{
		// 랜덤 값 생성
		_float fRad = m_pTarget->Get_SphereCollider()->GetRadius();
		_float fRand_X = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
		_float fRand_Y = CWall512::GetInstance()->GetFloatValue(-fRad * 0.1f, fRad * 0.3f);
		_float fRand_Z = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);

		_vec3 vRandom = m_pTarget->Get_SphereCollider()->GetPos() + _vec3(fRand_X, fRand_Y, fRand_Z);

		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle(), 2.f);

		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX2_Hit_Explosion.wav", 1.f);
	}
}
