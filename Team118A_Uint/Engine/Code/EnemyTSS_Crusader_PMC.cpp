#include "EnemyTSS_Crusader_PMC.h"

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CEnemyTSS_Crusader_PMC::CEnemyTSS_Crusader_PMC(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"EnemyTSS_Crusader_PMC")
{
}

CEnemyTSS_Crusader_PMC::CEnemyTSS_Crusader_PMC(const CEnemyTSS_Crusader_PMC & rhs)
	: CMonster(rhs)
{
}

CEnemyTSS_Crusader_PMC::~CEnemyTSS_Crusader_PMC()
{
}

CEnemyTSS_Crusader_PMC * CEnemyTSS_Crusader_PMC::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CEnemyTSS_Crusader_PMC* pInstance = new CEnemyTSS_Crusader_PMC(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemyTSS_Crusader_PMC::Free()
{
	CMonster::Free();
}

HRESULT CEnemyTSS_Crusader_PMC::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Bone_Body";
	m_szWeaponBoneName = "Weapon_01";

	Set_AmmoMax(1);

	m_tUnitInfo.iAttack = 100;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 100000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CEnemyTSS_Crusader_PMC::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CEnemyTSS_Crusader_PMC::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CEnemyTSS_Crusader_PMC::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CEnemyTSS_Crusader_PMC::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CEnemyTSS_Crusader_PMC::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CEnemyTSS_Crusader_PMC::OnEnable()
{
	CMonster::OnEnable();
}

void CEnemyTSS_Crusader_PMC::OnDisable()
{
	CMonster::OnDisable();
}

void CEnemyTSS_Crusader_PMC::OnDestroy()
{
	CMonster::OnDestroy();
}

void CEnemyTSS_Crusader_PMC::Trigger_Attack_1()
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.65f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	Create_FireEffect();

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		vWeaponPos,
		*D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_TankBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CEnemyTSS_Crusader_PMC::Trigger_1()
{
	Create_ChargeEffect(0.1f);
}

void CEnemyTSS_Crusader_PMC::Trigger_2()
{
	Create_ChargeEffect(0.1f);
}

void CEnemyTSS_Crusader_PMC::Trigger_3()
{
	Create_ChargeEffect(0.1f);
}

void CEnemyTSS_Crusader_PMC::Trigger_ExSkill()
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.65f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	Create_FireEffect();

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		vWeaponPos,
		*D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_TankBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CEnemyTSS_Crusader_PMC::Create_FireEffect()
{
	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Smoke_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 1.5f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Normal.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 1.5f);
}

void CEnemyTSS_Crusader_PMC::Create_ChargeEffect(const _float& fRatio)
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.65f * fRatio;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Charge_Circle.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);
	pGameObject->Set_Pos(vWeaponPos);
}
