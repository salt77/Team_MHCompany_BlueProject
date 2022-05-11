#include "Char_Shun.h"

#include "CollisionManager.h"
USING(Engine)

CChar_Shun::CChar_Shun(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Shun::CChar_Shun(const CChar_Shun & rhs)
	: CCharacter(rhs)
{
}

CChar_Shun::~CChar_Shun()
{
}

CChar_Shun * CChar_Shun::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Shun*	pInstance = new CChar_Shun(pGraphicDev, eObjTag, L"Shun");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Shun::Free()
{
	CCharacter::Free();
}

void CChar_Shun::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Shun::Load(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Shun::Set_GuideSkill()
{
}


HRESULT CChar_Shun::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	m_tUnitSound.pairReload.first = L"SFX_Common_SR_Reload_03.wav";
	return S_OK;
}

_int CChar_Shun::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Shun::Update_Object(const _float & fTimeDelta)
{

	_int iExit = CCharacter::Update_Object(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_F7))
	{
		FSM_FunctionSetting(&CUnit::ExSkill_CurIn, nullptr);
	}

	return iExit;
}

_int CChar_Shun::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Shun::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Shun::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Shun::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Shun::GuideSkillSetting(_vec3 vPickPos)
{

}

void CChar_Shun::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();
	if (!m_pTarget)
	{
		return;
	}
	//// 직선범위
	//CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position(), m_pTransformCom->Get_Info(E_INFO::INFO_LOOK), 3.f, Get_UnitRange(), m_eTargetLayerTag, Hit_Target_FanToCircle);
	//
	//// 원 각도 범위
	//CCollisionManager::GetInstance()->Collision_FanToCircle(this, Get_Position(), m_pTransformCom->Get_Info(E_INFO::INFO_LOOK), 30.f, Get_UnitRange(), m_eTargetLayerTag, Hit_Target_FanToCircle);

	//// 타겟 주위 원형 범위

	//CCollisionManager::GetInstance()->Collision_SpotToCircle(this, m_pTarget->Get_Position(), 3.f, m_eTargetLayerTag, Hit_Target_FanToCircle);

	CSkill_Manager::GetInstance()->SetSkill(L"EH_Shun_Attack_1_0109.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.7f), Get_Angle(), 3.5f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_01.wav", 1.f);
}

void CChar_Shun::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_Reload_03.wav", 1.f);
}

void CChar_Shun::Trigger_ExSkill()
{
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Shun_Exs_1_0109.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Shun_Ex.wav", 1.f);
}

void CChar_Shun::Trigger_9()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Shun_Ex.camdat", L"Shun", Get_Position(), Get_Angle().y);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Shun_ExSkill_1.ogg", 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Shun_EX_Cut_in.wav", 2.f);
}