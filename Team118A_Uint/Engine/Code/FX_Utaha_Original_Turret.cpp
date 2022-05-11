#include "FX_Utaha_Original_Turret.h"
#include "UI_Damage.h"
//�����Ҷ� �ڽ��� �±׸� �Է��ؼ� �ڽ��� �޽��� �ҷ������� ��
CFX_Utaha_Original_Turret::CFX_Utaha_Original_Turret(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"FX_Utaha_Original_Turret")
{
}

CFX_Utaha_Original_Turret::CFX_Utaha_Original_Turret(const CFX_Utaha_Original_Turret & rhs)
	:CMonster(rhs)
{
}

CFX_Utaha_Original_Turret::~CFX_Utaha_Original_Turret()
{
}

CFX_Utaha_Original_Turret * CFX_Utaha_Original_Turret::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CFX_Utaha_Original_Turret* pInstance = new CFX_Utaha_Original_Turret(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFX_Utaha_Original_Turret::Free()
{
	CMonster::Free();
}

HRESULT CFX_Utaha_Original_Turret::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "bone_robot_head";
	m_szWeaponBoneName = "bone_robot_run_L";
	// bone_robot_run_L, bone_robot_run_R
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
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;
	return S_OK;
}

_int CFX_Utaha_Original_Turret::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CFX_Utaha_Original_Turret::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CFX_Utaha_Original_Turret::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CFX_Utaha_Original_Turret::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CFX_Utaha_Original_Turret::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CFX_Utaha_Original_Turret::OnEnable()
{
	CMonster::OnEnable();
}

void CFX_Utaha_Original_Turret::OnDisable()
{
	CMonster::OnDisable();
}

void CFX_Utaha_Original_Turret::OnDestroy()
{
	CMonster::OnDestroy();
}

void CFX_Utaha_Original_Turret::Trigger_Attack_1()
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