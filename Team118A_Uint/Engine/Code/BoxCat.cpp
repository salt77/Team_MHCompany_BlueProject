#include "BoxCat.h"
#include "UI_Damage.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CBoxCat::CBoxCat(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"BoxCat")
{
}

CBoxCat::CBoxCat(const CBoxCat & rhs)
	:CMonster(rhs)
{
}

CBoxCat::~CBoxCat()
{
}

CBoxCat * CBoxCat::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CBoxCat* pInstance = new CBoxCat(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoxCat::Free()
{
	CMonster::Free();
}

HRESULT CBoxCat::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName =		"bone_body0";
	m_szWeaponBoneName =	"bone_boxwell";

	Set_AmmoMax(0);

	m_tUnitInfo.iAttack = 0;
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
	m_tUnitInfo.iRange = 0;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CBoxCat::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CBoxCat::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CBoxCat::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CBoxCat::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CBoxCat::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CBoxCat::OnEnable()
{
	CMonster::OnEnable();
}

void CBoxCat::OnDisable()
{
	CMonster::OnDisable();
}

void CBoxCat::OnDestroy()
{
	CMonster::OnDestroy();
}