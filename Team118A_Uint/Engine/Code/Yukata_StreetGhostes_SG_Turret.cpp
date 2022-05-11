#include "Yukata_StreetGhostes_SG_Turret.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CYukata_StreetGhostes_SG_Turret::CYukata_StreetGhostes_SG_Turret(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Yukata_StreetGhostes_SG_Turret")
{
}

CYukata_StreetGhostes_SG_Turret::CYukata_StreetGhostes_SG_Turret(const CYukata_StreetGhostes_SG_Turret & rhs)
	:CMonster(rhs)
{
}

CYukata_StreetGhostes_SG_Turret::~CYukata_StreetGhostes_SG_Turret()
{
}

CYukata_StreetGhostes_SG_Turret * CYukata_StreetGhostes_SG_Turret::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CYukata_StreetGhostes_SG_Turret* pInstance = new CYukata_StreetGhostes_SG_Turret(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYukata_StreetGhostes_SG_Turret::Free()
{
	CMonster::Free();
}

HRESULT CYukata_StreetGhostes_SG_Turret::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "bone_turret_body";
	m_szWeaponBoneName = "bone_turret_head";
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
	m_tUnitInfo.iMaxHp = 3000;
	m_tUnitInfo.iNowHp = 3000;
	m_tUnitInfo.iRange = 1200;
	m_tUnitInfo.iStability;
	return S_OK;
}

_int CYukata_StreetGhostes_SG_Turret::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CYukata_StreetGhostes_SG_Turret::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CYukata_StreetGhostes_SG_Turret::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CYukata_StreetGhostes_SG_Turret::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CYukata_StreetGhostes_SG_Turret::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CYukata_StreetGhostes_SG_Turret::OnEnable()
{
	CMonster::OnEnable();
}

void CYukata_StreetGhostes_SG_Turret::OnDisable()
{
	CMonster::OnDisable();
}

void CYukata_StreetGhostes_SG_Turret::OnDestroy()
{
	CMonster::OnDestroy();
}

void CYukata_StreetGhostes_SG_Turret::Trigger_Attack_1()
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

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
}