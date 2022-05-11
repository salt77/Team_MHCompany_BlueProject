#include "EnemyTSS_Aparche_Blackmarket.h"

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CEnemyTSS_Aparche_Blackmarket::CEnemyTSS_Aparche_Blackmarket(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"EnemyTSS_Aparche_Blackmarket")
{
}

CEnemyTSS_Aparche_Blackmarket::CEnemyTSS_Aparche_Blackmarket(const CEnemyTSS_Aparche_Blackmarket & rhs)
	: CMonster(rhs)
{
}

CEnemyTSS_Aparche_Blackmarket::~CEnemyTSS_Aparche_Blackmarket()
{
}

CEnemyTSS_Aparche_Blackmarket * CEnemyTSS_Aparche_Blackmarket::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CEnemyTSS_Aparche_Blackmarket* pInstance = new CEnemyTSS_Aparche_Blackmarket(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemyTSS_Aparche_Blackmarket::Free()
{
	CMonster::Free();
}

HRESULT CEnemyTSS_Aparche_Blackmarket::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "bone_eye";
	m_szWeaponBoneName = "bone_gun02";
	Set_AmmoMax(3);

	m_tUnitInfo.iAttack = 100;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 999999;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CEnemyTSS_Aparche_Blackmarket::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	Create_Collider(&m_pLeftSecondCol, L"Collider_LeftSecondWeapon", 0.1f);
	Create_Collider(&m_pRightSecondCol, L"Colldier_RightSecondWeapon", 0.1f);

	return RETURN_OK;
}

_int CEnemyTSS_Aparche_Blackmarket::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pLeftSecondCol, m_szLeftSecondWeapon);
	Collider_SetMatrix(m_pRightSecondCol, m_szRightSecondWeapon);

	return RETURN_OK;
}

_int CEnemyTSS_Aparche_Blackmarket::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CEnemyTSS_Aparche_Blackmarket::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CEnemyTSS_Aparche_Blackmarket::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CEnemyTSS_Aparche_Blackmarket::OnEnable()
{
	CMonster::OnEnable();
}

void CEnemyTSS_Aparche_Blackmarket::OnDisable()
{
	CMonster::OnDisable();
}

void CEnemyTSS_Aparche_Blackmarket::OnDestroy()
{
	CMonster::OnDestroy();
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_Attack_1()
{
	Create_NormalBullet();
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_Attack_2()
{
	Create_NormalBullet();
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_Attack_3()
{
	Create_NormalBullet();
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_1()
{
	Create_MissileBullet(TRUE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_2()
{
	Create_MissileBullet(FALSE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_3()
{
	Create_MissileBullet(TRUE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_4()
{
	Create_MissileBullet(FALSE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_5()
{
	Create_MissileBullet(TRUE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_6()
{
	Create_MissileBullet(FALSE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_7()
{
	Create_MissileBullet(TRUE);
}

void CEnemyTSS_Aparche_Blackmarket::Trigger_8()
{
	Create_MissileBullet(FALSE);
}

void CEnemyTSS_Aparche_Blackmarket::Create_NormalBullet()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.0f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
}

void CEnemyTSS_Aparche_Blackmarket::Create_MissileBullet(_bool bLeft)
{
	if (!m_pTarget)
	{
		return;
	}

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pLeftSecondCol->GetPos();
	bLeft ? vWeaponPos : vWeaponPos = m_pRightSecondCol->GetPos();

	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		vWeaponPos,
		*D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_HeliMissile);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}
