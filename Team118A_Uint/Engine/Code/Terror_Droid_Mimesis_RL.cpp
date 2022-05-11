#include "Terror_Droid_Mimesis_RL.h"

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CTerror_Droid_Mimesis_RL::CTerror_Droid_Mimesis_RL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Terror_Droid_Mimesis_RL")
{
}

CTerror_Droid_Mimesis_RL::CTerror_Droid_Mimesis_RL(const CTerror_Droid_Mimesis_RL & rhs)
	: CMonster(rhs)
{
}

CTerror_Droid_Mimesis_RL::~CTerror_Droid_Mimesis_RL()
{
}

CTerror_Droid_Mimesis_RL * CTerror_Droid_Mimesis_RL::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CTerror_Droid_Mimesis_RL* pInstance = new CTerror_Droid_Mimesis_RL(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerror_Droid_Mimesis_RL::Free()
{
	CMonster::Free();
}

HRESULT CTerror_Droid_Mimesis_RL::Ready_Object()
{
	CMonster::Ready_Object();

	m_szWeaponBoneName = "Bip001_L_Finger1";
	Set_AmmoMax(1);

	m_tUnitInfo.iAttack = 60;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 3000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 800;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CTerror_Droid_Mimesis_RL::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_RL::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_RL::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CTerror_Droid_Mimesis_RL::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CTerror_Droid_Mimesis_RL::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CTerror_Droid_Mimesis_RL::OnEnable()
{
	CMonster::OnEnable();
}

void CTerror_Droid_Mimesis_RL::OnDisable()
{
	CMonster::OnDisable();
}

void CTerror_Droid_Mimesis_RL::OnDestroy()
{
	CMonster::OnDestroy();
}

void CTerror_Droid_Mimesis_RL::Trigger_Attack_1()
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

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
																 vWeaponPos,
																 *D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
																 m_tUnitInfo,
																 m_tMonsterInfo.eAtkType,
																 CBullet_Normal_Monster::Move_RocketLauncher);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_NormalAttack_Hit_Start.wav", m_fSoundVolume); 
}

void CTerror_Droid_Mimesis_RL::Trigger_1()
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

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		vWeaponPos,
		*D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_RocketLauncher);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}
