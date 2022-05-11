#include "Droid_Yoheki_Shield.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CDroid_Yoheki_Shield::CDroid_Yoheki_Shield(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Droid_Yoheki_Shield")
{
}

CDroid_Yoheki_Shield::CDroid_Yoheki_Shield(const CDroid_Yoheki_Shield & rhs)
	:CMonster(rhs)
{
}

CDroid_Yoheki_Shield::~CDroid_Yoheki_Shield()
{
}

CDroid_Yoheki_Shield * CDroid_Yoheki_Shield::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CDroid_Yoheki_Shield* pInstance = new CDroid_Yoheki_Shield(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDroid_Yoheki_Shield::Free()
{
	if (m_pEffectShield)
	{
		Safe_Release(m_pEffectShield); 
	}

	CMonster::Free();
}

HRESULT CDroid_Yoheki_Shield::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Prop2";
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
	m_tUnitInfo.iMaxHp = 10000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;

	m_iOriginDef = m_tUnitInfo.iDeffence;

	return S_OK;
}

_int CDroid_Yoheki_Shield::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	Create_Collider(&m_pShieldCollider, L"Colldier_Shield", 0.1f); 

	return RETURN_OK;
}

_int CDroid_Yoheki_Shield::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pShieldCollider, m_szShieldBoneName); 

	if (m_pEffectShield)
	{
		m_pEffectShield->Update_Object(fTimeDelta); 
		m_pEffectShield->Set_Effect_Pos(m_pShieldCollider->GetPos() + m_pShieldCollider->GetLook() * 1.2f); 
	}

	return RETURN_OK;
}

_int CDroid_Yoheki_Shield::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	if (m_pEffectShield)
	{
		m_pEffectShield->LateUpdate_Object(fTimeDelta); 
	}

	return RETURN_OK;
}

void CDroid_Yoheki_Shield::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CDroid_Yoheki_Shield::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CDroid_Yoheki_Shield::OnEnable()
{
	CMonster::OnEnable();
}

void CDroid_Yoheki_Shield::OnDisable()
{
	CMonster::OnDisable();

	if (m_pEffectShield)
	{
		Safe_Release(m_pEffectShield); 
	}
}

void CDroid_Yoheki_Shield::OnDestroy()
{
	CMonster::OnDestroy();
}

void CDroid_Yoheki_Shield::ExSkill(_float fDeltaTime)
{
	m_listAstarNode.clear();

	if (m_eCurrentAction == E_ACTION::ExSkill && Is_AnimFinished())
	{
		FSM_FunctionSetting(&CUnit::Special_State_1, &CUnit::Special_State_2);

		return;
	}

	m_eCurrentAction = E_ACTION::ExSkill;
}

void CDroid_Yoheki_Shield::Special_State_1(_float fDeltaTime)
{
	E_ACTION eMain = E_ACTION::Temp1; 

	m_fExSkillTime -= fDeltaTime;

	if (m_eCurrentAction == E_ACTION::Temp1 && 
		0.f >= m_fExSkillTime)
	{
		m_bStateLock = FALSE;
		m_fExSkillTime = m_fExSkillOriginTime;

		FSM_FunctionSetting(&CUnit::Special_State_2, &CUnit::Idle);
	}

	m_eCurrentAction = eMain;
}

void CDroid_Yoheki_Shield::Special_State_2(_float fDeltaTime)
{
	E_ACTION eMain = E_ACTION::Temp2;

	m_fMonsterExSkillTimer = CWall512::GetInstance()->GetFloatValue(m_fMonsterExSkillOriginTimer, m_fMonsterExSkillOriginTimer * 2.f);
	m_tUnitInfo.iDeffence = m_iOriginDef;

	if (m_eCurrentAction == E_ACTION::Temp2 && Is_AnimFinished())
	{
		m_bStateLock = FALSE;

		FSM_FunctionSetting(&CUnit::Idle, &CUnit::Shoot);
	}

	m_eCurrentAction = eMain;
}

void CDroid_Yoheki_Shield::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook		= *D3DXVec3Normalize(&vDirLook,		&Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight		= *D3DXVec3Normalize(&vDirRight,	&Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp		= *D3DXVec3Normalize(&vDirUp,		&Get_Info(E_INFO::INFO_UP));
	_float fLookRatio	= 0.4f;
	_float fRightRatio	= 0.0f;
	_float fUpRatio		= 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook	* fLookRatio;
	vWeaponPos += vDirRight	* fRightRatio;
	vWeaponPos += vDirUp	* fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect_OneShot.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_01.wav", 0.075f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_02.wav", 0.075f); 
}

void CDroid_Yoheki_Shield::Trigger_Attack_2()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect_OneShot.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_03.wav", 0.075f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_04.wav", 0.075f);
}

void CDroid_Yoheki_Shield::Trigger_Attack_3()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.45f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect_OneShot.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_One_05.wav", m_fSoundVolume);
}

void CDroid_Yoheki_Shield::Trigger_1()
{
	m_pEffectShield = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pShieldCollider->GetPos() + *D3DXVec3Normalize(&m_vOutTemp, &(Get_Info(E_INFO::INFO_LOOK))) * 0.35f, Get_Angle(), L"Automata_Shield.effect");
	assert(m_pEffectShield); 

	m_pEffectShield->Set_Effect_Scale(VEC_ONE * 1.35f); 
	m_pEffectShield->Set_Effect_Rotation(m_pShieldCollider->GetLook());

	static_cast<CUnit*>(this)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_DEF, 3.f)); 
	m_tUnitInfo.iDeffence *= 3; 
}

void CDroid_Yoheki_Shield::Trigger_2()
{
	m_tUnitInfo.iDeffence = m_iOriginDef;

	if (m_pEffectShield)
	{
		Safe_Release(m_pEffectShield);
	}

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_Reload_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_Reload_02.wav", m_fSoundVolume);
}

void CDroid_Yoheki_Shield::Trigger_5()
{
}

void CDroid_Yoheki_Shield::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Appear_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Appear_02.wav", m_fSoundVolume);
}

void CDroid_Yoheki_Shield::Trigger_Dead()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Death_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Death_02.wav", m_fSoundVolume);
}
