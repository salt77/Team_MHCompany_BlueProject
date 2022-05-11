#include "TechSoldier_Decagram_AR.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CTechSoldier_Decagram_AR::CTechSoldier_Decagram_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"TechSoldier_Decagram_AR")
{
}

CTechSoldier_Decagram_AR::CTechSoldier_Decagram_AR(const CTechSoldier_Decagram_AR & rhs)
	:CMonster(rhs)
{
}

CTechSoldier_Decagram_AR::~CTechSoldier_Decagram_AR()
{
}

CTechSoldier_Decagram_AR * CTechSoldier_Decagram_AR::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CTechSoldier_Decagram_AR* pInstance = new CTechSoldier_Decagram_AR(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTechSoldier_Decagram_AR::Free()
{
	CMonster::Free();
}

HRESULT CTechSoldier_Decagram_AR::Ready_Object()
{
	CMonster::Ready_Object();
	m_szWeaponBoneName = "bone_magazine";
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
	m_tUnitInfo.iMaxHp = 3000;
	m_tUnitInfo.iNowHp = 3000;
	m_tUnitInfo.iRange = 500;
	m_tUnitInfo.iStability;
	return S_OK;
}

_int CTechSoldier_Decagram_AR::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CTechSoldier_Decagram_AR::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CTechSoldier_Decagram_AR::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CTechSoldier_Decagram_AR::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CTechSoldier_Decagram_AR::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CTechSoldier_Decagram_AR::OnEnable()
{
	CMonster::OnEnable();
}

void CTechSoldier_Decagram_AR::OnDisable()
{
	CMonster::OnDisable();
}

void CTechSoldier_Decagram_AR::OnDestroy()
{
	CMonster::OnDestroy();
}

void CTechSoldier_Decagram_AR::Trigger_Attack_1()
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