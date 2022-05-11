#include "Sukeban_SMG.h"
#include "UI_Damage.h"

//�����Ҷ� �ڽ��� �±׸� �Է��ؼ� �ڽ��� �޽��� �ҷ������� ��
CSukeban_SMG::CSukeban_SMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Sukeban_SMG")
{
}

CSukeban_SMG::CSukeban_SMG(const CSukeban_SMG & rhs)
	:CMonster(rhs)
{
}

CSukeban_SMG::~CSukeban_SMG()
{
}

CSukeban_SMG * CSukeban_SMG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSukeban_SMG* pInstance = new CSukeban_SMG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSukeban_SMG::Free()
{
	CMonster::Free();
}

HRESULT CSukeban_SMG::Ready_Object()
{
	CMonster::Ready_Object();
	
	m_szWeaponBoneName = "bone_magazine_R";


	m_tUnitInfo.iNowHp					= 1700;			// ���� ü��
	m_tUnitInfo.iMaxHp					= 1700;			// �ִ� ü��
	m_tUnitInfo.iAttack					= 20;			// ���ݷ�
	m_tUnitInfo.iDeffence				= 30;			// ����
	m_tUnitInfo.iHeal					= 1000;			// ȸ����
	m_tUnitInfo.iHitRatio				= 100;			// ���߷�
	m_tUnitInfo.iEvasion				= 1000;			// ȸ�Ƿ� 
	m_tUnitInfo.iCritical				= 200;			// ũ��Ƽ�� Ȯ�� ������ 10�ϸ��
	m_tUnitInfo.iCriticalDamage			= 200;			// ũ��Ƽ�� ���� ������ 100�ϸ��
	m_tUnitInfo.iStability				= 2000;			// ������
	m_tUnitInfo.iCrowdReinforce			= 100;			// �������ȭ
	m_tUnitInfo.iCrowdResist			= 100;			// ������������
	
	// ��Ÿ�
	m_tUnitInfo.iRange					= CDataManager::GetInstance()->Get_RangeByWeaponType(m_tMonsterInfo.eWeaponType);	
	// ź�� ��
	Set_AmmoMax(CDataManager::GetInstance()->Get_MaxAmmoByWeaponType(m_tMonsterInfo.eWeaponType));

	m_tUnitSound.pairShoot.first = L"SFX_Common_DSMG_01.wav";
	m_tUnitSound.pairReload.first = L"SFX_Common_DSMG_Reload_01.wav";

	return S_OK;
}

_int CSukeban_SMG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CSukeban_SMG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSukeban_SMG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CSukeban_SMG::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CSukeban_SMG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSukeban_SMG::OnEnable()
{
	CMonster::OnEnable();
}

void CSukeban_SMG::OnDisable()
{
	CMonster::OnDisable();
}

void CSukeban_SMG::OnDestroy()
{
	CMonster::OnDestroy();
}

void CSukeban_SMG::Trigger_Attack_1()
{
	Hit_Target();

	Create_Effect(L"Test_Minhyun_1231_Fire_00.skill", 0.f, 0.f, 0.4f);
}	