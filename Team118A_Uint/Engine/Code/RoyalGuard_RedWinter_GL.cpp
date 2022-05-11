#include "RoyalGuard_RedWinter_GL.h"
#include "UI_Damage.h"

//�����Ҷ� �ڽ��� �±׸� �Է��ؼ� �ڽ��� �޽��� �ҷ������� ��
CRoyalGuard_RedWinter_GL::CRoyalGuard_RedWinter_GL(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"RoyalGuard_RedWinter_GL")
{
}

CRoyalGuard_RedWinter_GL::CRoyalGuard_RedWinter_GL(const CRoyalGuard_RedWinter_GL & rhs)
	:CMonster(rhs)
{
}

CRoyalGuard_RedWinter_GL::~CRoyalGuard_RedWinter_GL()
{
}

CRoyalGuard_RedWinter_GL * CRoyalGuard_RedWinter_GL::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CRoyalGuard_RedWinter_GL* pInstance = new CRoyalGuard_RedWinter_GL(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoyalGuard_RedWinter_GL::Free()
{
	CMonster::Free();
}

HRESULT CRoyalGuard_RedWinter_GL::Ready_Object()
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

_int CRoyalGuard_RedWinter_GL::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CRoyalGuard_RedWinter_GL::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CRoyalGuard_RedWinter_GL::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CRoyalGuard_RedWinter_GL::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CRoyalGuard_RedWinter_GL::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CRoyalGuard_RedWinter_GL::OnEnable()
{
	CMonster::OnEnable();
}

void CRoyalGuard_RedWinter_GL::OnDisable()
{
	CMonster::OnDisable();
}

void CRoyalGuard_RedWinter_GL::OnDestroy()
{
	CMonster::OnDestroy();
}

void CRoyalGuard_RedWinter_GL::Trigger_Attack_1()
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