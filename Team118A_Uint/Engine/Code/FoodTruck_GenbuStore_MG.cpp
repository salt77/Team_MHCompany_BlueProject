#include "FoodTruck_GenbuStore_MG.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CFoodTruck_GenbuStore_MG::CFoodTruck_GenbuStore_MG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"FoodTruck_GenbuStore_MG")
{
}

CFoodTruck_GenbuStore_MG::CFoodTruck_GenbuStore_MG(const CFoodTruck_GenbuStore_MG & rhs)
	:CMonster(rhs)
{
}

CFoodTruck_GenbuStore_MG::~CFoodTruck_GenbuStore_MG()
{
}

CFoodTruck_GenbuStore_MG * CFoodTruck_GenbuStore_MG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CFoodTruck_GenbuStore_MG* pInstance = new CFoodTruck_GenbuStore_MG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFoodTruck_GenbuStore_MG::Free()
{
	CMonster::Free();
}

HRESULT CFoodTruck_GenbuStore_MG::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Head_sp";
	m_szWeaponBoneName = "Bip001_Weapon01";
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

_int CFoodTruck_GenbuStore_MG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CFoodTruck_GenbuStore_MG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CFoodTruck_GenbuStore_MG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CFoodTruck_GenbuStore_MG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CFoodTruck_GenbuStore_MG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CFoodTruck_GenbuStore_MG::OnEnable()
{
	CMonster::OnEnable();
}

void CFoodTruck_GenbuStore_MG::OnDisable()
{
	CMonster::OnDisable();
}

void CFoodTruck_GenbuStore_MG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CFoodTruck_GenbuStore_MG::Trigger_Attack_1()
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