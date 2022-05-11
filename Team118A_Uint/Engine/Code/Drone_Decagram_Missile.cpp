#include "Drone_Decagram_Missile.h"

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CDrone_Decagram_Missile::CDrone_Decagram_Missile(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Drone_Decagram_Missile")
{
}

CDrone_Decagram_Missile::CDrone_Decagram_Missile(const CDrone_Decagram_Missile & rhs)
	: CMonster(rhs)
{
}

CDrone_Decagram_Missile::~CDrone_Decagram_Missile()
{
}

CDrone_Decagram_Missile * CDrone_Decagram_Missile::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CDrone_Decagram_Missile* pInstance = new CDrone_Decagram_Missile(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDrone_Decagram_Missile::Free()
{
	CMonster::Free();
}

HRESULT CDrone_Decagram_Missile::Ready_Object()
{
	CMonster::Ready_Object();

	m_bCanExSkill = FALSE;

	m_szBodyBoneName = "Bone_Top";
	m_szWeaponBoneName = "Bone_Missile_L";

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
	m_tUnitInfo.iRange = CWall512::GetInstance()->GetValue(600, 750);
	m_tUnitInfo.iStability;

	Create_Collider(&m_pSecondWeaponCol, L"Com_Collider_SecondWeapon", 0.1f);

	return S_OK;
}

_int CDrone_Decagram_Missile::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CDrone_Decagram_Missile::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pSecondWeaponCol, m_szSecondWeaponBone);

	return RETURN_OK;
}

_int CDrone_Decagram_Missile::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CDrone_Decagram_Missile::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CDrone_Decagram_Missile::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CDrone_Decagram_Missile::OnEnable()
{
	CMonster::OnEnable();
}

void CDrone_Decagram_Missile::OnDisable()
{
	CMonster::OnDisable();
}

void CDrone_Decagram_Missile::OnDestroy()
{
	CMonster::OnDestroy();
}

void CDrone_Decagram_Missile::Trigger_1()
{
	if (m_pTarget)
	{
		m_vTargetPos = m_pTarget->Get_Position();
	}

	Create_Missile(TRUE);
}

void CDrone_Decagram_Missile::Trigger_2()
{
}

void CDrone_Decagram_Missile::Trigger_3()
{
	Create_Missile(FALSE);
}

void CDrone_Decagram_Missile::Trigger_5()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Missile::Trigger_6()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_01.wav", 0.075f) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_AttackStart_02.wav", 0.075f);
}

void CDrone_Decagram_Missile::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Spawn_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Spawn_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Missile::Trigger_Dead()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_Death_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Drone_Missile_Death_02.wav", m_fSoundVolume);
}

void CDrone_Decagram_Missile::Create_Missile(const _bool& bLeft)
{
	if (VEC_ZERO == m_vTargetPos)
	{
		return;
	}

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = bLeft ? m_pWeaponCollider->GetPos() : m_pSecondWeaponCol->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	_vec3	vDir = *D3DXVec3Normalize(&vDir, &(m_vTargetPos - vWeaponPos));

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		vWeaponPos,
		vDir,
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_HeliMissile);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_NormalAttack_Hit_Start.wav", m_fSoundVolume); 
}