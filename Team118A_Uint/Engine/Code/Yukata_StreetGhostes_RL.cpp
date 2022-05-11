#include "Yukata_StreetGhostes_RL.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CYukata_StreetGhostes_RL::CYukata_StreetGhostes_RL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Yukata_StreetGhostes_RL")
{
}

CYukata_StreetGhostes_RL::CYukata_StreetGhostes_RL(const CYukata_StreetGhostes_RL & rhs)
	:CMonster(rhs)
{
}

CYukata_StreetGhostes_RL::~CYukata_StreetGhostes_RL()
{
}

CYukata_StreetGhostes_RL * CYukata_StreetGhostes_RL::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CYukata_StreetGhostes_RL* pInstance = new CYukata_StreetGhostes_RL(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CYukata_StreetGhostes_RL::Free()
{
	CMonster::Free();
}

HRESULT CYukata_StreetGhostes_RL::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	Set_AmmoMax(30);

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

_int CYukata_StreetGhostes_RL::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CYukata_StreetGhostes_RL::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CYukata_StreetGhostes_RL::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CYukata_StreetGhostes_RL::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CYukata_StreetGhostes_RL::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CYukata_StreetGhostes_RL::OnEnable()
{
	CMonster::OnEnable();
}

void CYukata_StreetGhostes_RL::OnDisable()
{
	CMonster::OnDisable();
}

void CYukata_StreetGhostes_RL::OnDestroy()
{
	CMonster::OnDestroy();
}

void CYukata_StreetGhostes_RL::Trigger_Attack_1()
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