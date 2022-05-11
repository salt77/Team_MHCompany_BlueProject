#include "Sukeban_SR.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CSukeban_SR::CSukeban_SR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Sukeban_SR")
{
}

CSukeban_SR::CSukeban_SR(const CSukeban_SR & rhs)
	:CMonster(rhs)
{
}

CSukeban_SR::~CSukeban_SR()
{
}

CSukeban_SR * CSukeban_SR::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSukeban_SR* pInstance = new CSukeban_SR(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSukeban_SR::Free()
{
	CMonster::Free();
}

HRESULT CSukeban_SR::Ready_Object()
{
	CMonster::Ready_Object();
	m_szWeaponBoneName = "Bip001_Weapon";


	m_tUnitInfo.iNowHp					= 3000;			// 현재 체력
	m_tUnitInfo.iMaxHp					= 3000;			// 최대 체력
	m_tUnitInfo.iAttack					= 60;			// 공격력
	m_tUnitInfo.iDeffence				= 50;			// 방어력
	m_tUnitInfo.iHeal					= 1000;			// 회복력
	m_tUnitInfo.iHitRatio				= 100;			// 명중률
	m_tUnitInfo.iEvasion				= 1000;			// 회피력 
	m_tUnitInfo.iCritical				= 400;			// 크리티컬 확률 나누기 10하면됨
	m_tUnitInfo.iCriticalDamage			= 300;			// 크리티컬 배율 나누기 100하면됨
	m_tUnitInfo.iStability				= 2000;			// 안전성
	m_tUnitInfo.iCrowdReinforce			= 100;			// 군중제어강화
	m_tUnitInfo.iCrowdResist			= 100;			// 군중제어저항
	
	// 사거리
	m_tUnitInfo.iRange					= CDataManager::GetInstance()->Get_RangeByWeaponType(m_tMonsterInfo.eWeaponType);	
	// 탄약 수
	Set_AmmoMax(CDataManager::GetInstance()->Get_MaxAmmoByWeaponType(m_tMonsterInfo.eWeaponType));

	m_tUnitSound.pairReload.first = L"SFX_Common_SR_Reload_03.wav";
	
	return S_OK;
}

_int CSukeban_SR::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CSukeban_SR::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSukeban_SR::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CSukeban_SR::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CSukeban_SR::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSukeban_SR::OnEnable()
{
	CMonster::OnEnable();
}

void CSukeban_SR::OnDisable()
{
	CMonster::OnDisable();
}

void CSukeban_SR::OnDestroy()
{
	CMonster::OnDestroy();
}

void CSukeban_SR::Trigger_Attack_1()
{
	Hit_Target();

	Create_Effect(L"Test_Minhyun_1231_Fire_00.skill", 0.f, 0.f, 0.4f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_01.wav", 0.5f);
}