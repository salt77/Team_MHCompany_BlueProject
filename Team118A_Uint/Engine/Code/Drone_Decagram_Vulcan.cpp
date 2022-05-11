#include "Drone_Decagram_Vulcan.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CDrone_Decagram_Vulcan::CDrone_Decagram_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Drone_Decagram_Vulcan")
{
}

CDrone_Decagram_Vulcan::CDrone_Decagram_Vulcan(const CDrone_Decagram_Vulcan & rhs)
	: CMonster(rhs)
{
}

CDrone_Decagram_Vulcan::~CDrone_Decagram_Vulcan()
{
}

CDrone_Decagram_Vulcan * CDrone_Decagram_Vulcan::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CDrone_Decagram_Vulcan* pInstance = new CDrone_Decagram_Vulcan(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrone_Decagram_Vulcan::Free()
{
	CMonster::Free();
}

HRESULT CDrone_Decagram_Vulcan::Ready_Object()
{
	CMonster::Ready_Object();

	m_bCanExSkill = FALSE;

	m_szBodyBoneName = "Bone_Top";
	m_szWeaponBoneName = "Bone_Vulcan_L";

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
	m_tUnitInfo.iMaxHp = 2000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = CWall512::GetInstance()->GetValue(600, 850);
	m_tUnitInfo.iStability;

	Create_Collider(&m_pSecondWeaponCol, L"Com_Collider_SecondWeapon", 0.1f);

	return S_OK;
}

_int CDrone_Decagram_Vulcan::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CDrone_Decagram_Vulcan::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pSecondWeaponCol, m_szSecondWeaponBone);

	_vec3	vPos = Get_Position();
	vPos.y = 1.f;
	Set_Pos(vPos);

	return RETURN_OK;
}

_int CDrone_Decagram_Vulcan::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CDrone_Decagram_Vulcan::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CDrone_Decagram_Vulcan::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CDrone_Decagram_Vulcan::OnEnable()
{
	CMonster::OnEnable();
}

void CDrone_Decagram_Vulcan::OnDisable()
{
	CMonster::OnDisable();
}

void CDrone_Decagram_Vulcan::OnDestroy()
{
	CMonster::OnDestroy();
}

void CDrone_Decagram_Vulcan::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vLeftWeaponPos = m_pWeaponCollider->GetPos();
	vLeftWeaponPos += vDirLook * fLookRatio;
	vLeftWeaponPos += vDirRight * fRightRatio;
	vLeftWeaponPos += vDirUp * fUpRatio;

	_vec3 vRightWeaponPos = m_pSecondWeaponCol->GetPos();
	vRightWeaponPos += vDirLook * fLookRatio;
	vRightWeaponPos += vDirRight * fRightRatio;
	vRightWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect.skill", E_LAYERTAG::Effect, vLeftWeaponPos, Get_Angle());
	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect.skill", E_LAYERTAG::Effect, vRightWeaponPos, Get_Angle());

	_int	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackIng_02.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackIng_03.wav", m_fSoundVolume);
}

void CDrone_Decagram_Vulcan::Trigger_5()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Vulcan::Trigger_6()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Vulcan_AttackEnd_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Vulcan_AttackEnd_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Vulcan::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Spawn_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Spawn_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Vulcan::Trigger_Dead()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_Death_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_Death_02.wav", m_fSoundVolume);
}
