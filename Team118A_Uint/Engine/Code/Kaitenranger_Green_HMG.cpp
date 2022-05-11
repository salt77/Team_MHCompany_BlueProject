#include "Kaitenranger_Green_HMG.h"
#include "UI_Damage.h"

#include "KeyManager.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CKaitenranger_Green_HMG::CKaitenranger_Green_HMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Kaitenranger_Green_HMG")
{
}

CKaitenranger_Green_HMG::CKaitenranger_Green_HMG(const CKaitenranger_Green_HMG & rhs)
	: CMonster(rhs)
{
}

CKaitenranger_Green_HMG::~CKaitenranger_Green_HMG()
{
}

CKaitenranger_Green_HMG * CKaitenranger_Green_HMG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CKaitenranger_Green_HMG* pInstance = new CKaitenranger_Green_HMG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKaitenranger_Green_HMG::Free()
{
	CMonster::Free();
}

HRESULT CKaitenranger_Green_HMG::Ready_Object()
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

	m_fMonsterExSkillOriginTimer = 11.f;
	m_fMonsterExSkillTimer = 3.f;

	return S_OK;
}

_int CKaitenranger_Green_HMG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CKaitenranger_Green_HMG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	if (0.f >= m_tUnitInfo.iNowHp)
	{
		Release_Invoke();
	}

	//if (CKeyManager::GetInstance()->Key_Pressing(KEY_V) && CKeyManager::GetInstance()->Key_Down(KEY_B))
	//{
	//	FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
	//}

	return RETURN_OK;
}

_int CKaitenranger_Green_HMG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CKaitenranger_Green_HMG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CKaitenranger_Green_HMG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CKaitenranger_Green_HMG::OnEnable()
{
	CMonster::OnEnable();
}

void CKaitenranger_Green_HMG::OnDisable()
{
	CMonster::OnDisable();
}

void CKaitenranger_Green_HMG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CKaitenranger_Green_HMG::Trigger_Attack_1()
{
	m_tMonsterInfo.eWeaponType = m_tMonsterInfo.eWeaponType;

	m_tUnitInfo.iAttack = 100;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.75f), Get_Angle(), 1.f);

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_END;

	Hit_Target();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_01.wav", 1.f);
}

void CKaitenranger_Green_HMG::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_Reload_01.wav", 1.f);
}

void CKaitenranger_Green_HMG::Trigger_1()
{
	// 데미지를 준다
	m_tUnitInfo.iAttack = 10;

	m_eUnitHitType = E_UNIT_HIT_TYPE::HIT_EXS;

	Hit_Target();
}

void CKaitenranger_Green_HMG::Trigger_2()
{
	// 스킬 시작
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

}

void CKaitenranger_Green_HMG::Trigger_3()
{
	// 스킬 시작 _ 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Green_0.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.2f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Ex_04_Blading_Public_exs.wav", 1.f);

	// 공격 이펙트 발생 (1회)
	Release_Invoke();
	Invoke_Add(3.f, bind(&CKaitenranger_Green_HMG::Hit_Shot, this));
}

void CKaitenranger_Green_HMG::Hit_Shot()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Kai_Green_Shot_0.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.05f, 0.5f), Get_Angle(), 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_KaitenFX_MK0_Ex_01_Muzzle_G_exs.wav", 1.f);

}

void CKaitenranger_Green_HMG::Create_HitEffect()
{
	// 랜덤 값 생성
	_float fRad = m_pTarget->Get_SphereCollider()->GetRadius();
	_float fRand_X = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
	_float fRand_Y = CWall512::GetInstance()->GetFloatValue(-fRad * 0.1f, fRad * 0.3f);
	_float fRand_Z = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
	_vec3 vRandom = m_pTarget->Get_SphereCollider()->GetPos() + _vec3(fRand_X, fRand_Y, fRand_Z);

	if (E_UNIT_HIT_TYPE::HIT_EXS == m_eUnitHitType)
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Kai_Green_Hit_0.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
	}
	else
	{
		CUnit::Create_HitEffect();
	}
}