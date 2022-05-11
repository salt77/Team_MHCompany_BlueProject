#include "SchoolGirl03_Sukeban_HMG.h"
#include "UI_Damage.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CSchoolGirl03_Sukeban_HMG::CSchoolGirl03_Sukeban_HMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"SchoolGirl03_Sukeban_HMG")
{
}

CSchoolGirl03_Sukeban_HMG::CSchoolGirl03_Sukeban_HMG(const CSchoolGirl03_Sukeban_HMG & rhs)
	:CMonster(rhs)
{
}

CSchoolGirl03_Sukeban_HMG::~CSchoolGirl03_Sukeban_HMG()
{
}

CSchoolGirl03_Sukeban_HMG * CSchoolGirl03_Sukeban_HMG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSchoolGirl03_Sukeban_HMG* pInstance = new CSchoolGirl03_Sukeban_HMG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSchoolGirl03_Sukeban_HMG::Free()
{
	CMonster::Free();
}

HRESULT CSchoolGirl03_Sukeban_HMG::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "bone_Rgun1";

	Set_AmmoMax(30);

	m_tUnitInfo.iAttack = 300;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 5000;
	m_tUnitInfo.iNowHp = 5000;
	m_tUnitInfo.iRange = 300;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CSchoolGirl03_Sukeban_HMG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSchoolGirl03_Sukeban_HMG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSchoolGirl03_Sukeban_HMG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CSchoolGirl03_Sukeban_HMG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CSchoolGirl03_Sukeban_HMG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSchoolGirl03_Sukeban_HMG::OnEnable()
{
	CMonster::OnEnable();
}

void CSchoolGirl03_Sukeban_HMG::OnDisable()
{
	CMonster::OnDisable();
}

void CSchoolGirl03_Sukeban_HMG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CSchoolGirl03_Sukeban_HMG::Trigger_Attack_1()
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