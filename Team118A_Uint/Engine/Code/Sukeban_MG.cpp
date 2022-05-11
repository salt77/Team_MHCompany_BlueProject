#include "Sukeban_MG.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CSukeban_MG::CSukeban_MG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Sukeban_MG")
{
}

CSukeban_MG::CSukeban_MG(const CSukeban_MG & rhs)
	:CMonster(rhs)
{
}

CSukeban_MG::~CSukeban_MG()
{
}

CSukeban_MG * CSukeban_MG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSukeban_MG* pInstance = new CSukeban_MG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSukeban_MG::Free()
{
	CMonster::Free();
}

HRESULT CSukeban_MG::Ready_Object()
{
	CMonster::Ready_Object();

	m_szWeaponBoneName = "Bip001_Weapon";


	m_tUnitInfo.iNowHp					= 2000;			// 현재 체력
	m_tUnitInfo.iMaxHp					= 2000;			// 최대 체력
	m_tUnitInfo.iAttack					= 20;			// 공격력
	m_tUnitInfo.iDeffence				= 30;			// 방어력
	m_tUnitInfo.iHeal					= 1000;			// 회복력
	m_tUnitInfo.iHitRatio				= 100;			// 명중률
	m_tUnitInfo.iEvasion				= 1000;			// 회피력 
	m_tUnitInfo.iCritical				= 200;			// 크리티컬 확률 나누기 10하면됨
	m_tUnitInfo.iCriticalDamage			= 200;			// 크리티컬 배율 나누기 100하면됨
	m_tUnitInfo.iStability				= 2000;			// 안전성
	m_tUnitInfo.iCrowdReinforce			= 100;			// 군중제어강화
	m_tUnitInfo.iCrowdResist			= 100;			// 군중제어저항
	
	// 사거리
	m_tUnitInfo.iRange					= CDataManager::GetInstance()->Get_RangeByWeaponType(m_tMonsterInfo.eWeaponType);	
	// 탄약 수
	Set_AmmoMax(CDataManager::GetInstance()->Get_MaxAmmoByWeaponType(m_tMonsterInfo.eWeaponType));

	m_tUnitSound.pairShoot.first = L"SFX_Common_MG_01.wav";
	m_tUnitSound.pairReload.first = L"SFX_Common_MG_Reload_01.wav";

	return S_OK;
}

_int CSukeban_MG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CSukeban_MG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSukeban_MG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CSukeban_MG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CSukeban_MG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSukeban_MG::OnEnable()
{
	CMonster::OnEnable();
}

void CSukeban_MG::OnDisable()
{
	CMonster::OnDisable();
}

void CSukeban_MG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CSukeban_MG::Trigger_Attack_1()
{
	Hit_Target();

	Create_Effect(L"Test_Minhyun_1231_Fire_00.skill", 0.f, 0.f, 0.8f);
}