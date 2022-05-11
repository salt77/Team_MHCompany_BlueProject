#include "Char_Neru.h"
#include "ActionCam_Mgr.h"
USING(Engine)

CChar_Neru::CChar_Neru(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Neru::CChar_Neru(const CChar_Neru & rhs)
	: CCharacter(rhs)
{
}

CChar_Neru::~CChar_Neru()
{
}

CChar_Neru * CChar_Neru::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Neru*	pInstance = new CChar_Neru(pGraphicDev, eObjTag, L"Neru");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Neru::Free()
{
	CCharacter::Free();
}

void CChar_Neru::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Neru::Load(HANDLE hFile, DWORD & dwByte)
{
}

HRESULT CChar_Neru::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	m_tUnitInfo.iAttack = 50; 

	m_szWeaponBoneName = "Bip001_Weapon_L";

	m_tUnitSound.pairShoot.first = L"SFX_Common_DSMG_01.wav";
	m_tUnitSound.pairReload.first = L"SFX_Common_DSMG_Reload_01.wav";

	m_eMouse = E_CharacterMouseTag::비웃음;

	return S_OK;
}

_int CChar_Neru::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Neru::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_F5))
	{
		FSM_FunctionSetting(&CUnit::ExSkill_CurIn, nullptr);
	}

	return iExit;
}

_int CChar_Neru::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Neru::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Neru::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Neru::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Neru::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();

	Create_Effect(L"EH_Neru_Attack_1_0109.skill", 0.f, 0.f, 0.3f);
	Create_Effect(L"EH_Neru_Attack_1_0109.skill", 0.15f, 0.f, 0.4f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SMG_01.wav", 1.f);
}

void CChar_Neru::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SMG_Reload_02.wav", 1.f);
}

void CChar_Neru::Trigger_ExSkill()
{
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Neru_Exs_1_0109.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 2.f);
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 2.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Neru_Ex.wav", 1.f);
}

void CChar_Neru::Trigger_1()
{
	Hit_Target();
	Invoke_Add(0.01f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.03f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.05f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.13f, bind(&CChar_Neru::Hit_Target, this));

	Create_Effect(L"EH_Neru_Attack_1_0109.skill", -0.4f, 0.1f, 0.7f);

	Create_Effect(L"Neru_0.skill", -0.4f, 0.1f, 0.7f);

	//Neru_0
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_Reload_01.wav", m_fSoundVolume);
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"FX_Public_버프.wav", 1.f);
}

void CChar_Neru::Trigger_2()
{
	Hit_Target();
	Invoke_Add(0.01f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.05f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.11f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.13f, bind(&CChar_Neru::Hit_Target, this));

	Create_Effect(L"EH_Neru_Attack_1_0109.skill", 0.f, 0.1f, 0.4f);
}

void CChar_Neru::Trigger_3()
{
	Hit_Target();
	Invoke_Add(0.01f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.03f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.09f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.11f, bind(&CChar_Neru::Hit_Target, this));
	Invoke_Add(0.13f, bind(&CChar_Neru::Hit_Target, this));

	Create_Effect(L"EH_Neru_Attack_1_0109.skill", -0.1f, 0.f, 0.3f);
	Create_Effect(L"EH_Neru_Attack_1_0109.skill", 0.f, 0.0f, 0.4f);
}

void CChar_Neru::Trigger_8() 
{
	// 입2
	m_eMouse = E_CharacterMouseTag::화남;

	Invoke_Add(3.4f, bind(&CChar_Neru::MouseOri, this));
}

void CChar_Neru::Trigger_9()
{
	_float fAngle = Get_Angle().y;
	CUtility::Print_Wstring_DMode(to_wstring(fAngle));
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Neru_Ex.camdat", L"Neru", Get_Position(), fAngle);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Neru_Ex_Cut_in.wav", 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Neru_ExSkill_1.ogg", 1.f);

	// 입1
	m_eMouse = E_CharacterMouseTag::당황;
}

void CChar_Neru::MouseOri()
{
	// 원래입
	m_eMouse = E_CharacterMouseTag::비웃음;
}
