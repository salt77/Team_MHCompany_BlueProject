#include "Char_Pina.h"

USING(Engine)

CChar_Pina::CChar_Pina(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Pina::CChar_Pina(const CChar_Pina & rhs)
	: CCharacter(rhs)
{
}

CChar_Pina::~CChar_Pina()
{
}

CChar_Pina * CChar_Pina::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Pina*	pInstance = new CChar_Pina(pGraphicDev, eObjTag, L"Pina");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Pina::Free()
{
	CCharacter::Free();
}

void CChar_Pina::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Pina::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Pina::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Pina::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Pina::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Pina::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Pina::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Pina::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Pina::End_Battle_Sound(const _bool& bWin)
{
	bWin ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Pina_Battle_Victory_1.ogg", m_fSoundVolume) : 
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Pina_Tactic_Defeat_1.ogg", m_fSoundVolume); 
}

void CChar_Pina::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Pina::Begin_Battle_Sound()
{
	CSound_Manager::GetInstance()->PlaySound_(L"Pina_Battle_TacticalAction_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, m_fSoundVolume); 
}

void CChar_Pina::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vWeaponPos, Get_Angle(), L"Shot_Effect_1.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 1.5f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_01.wav", m_fSoundVolume);
}

void CChar_Pina::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_Reload_01.wav", m_fSoundVolume); 
}

void CChar_Pina::Trigger_ExSkill()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Pina_Ex.skill", E_LAYERTAG::Effect, m_pWeaponCollider->GetPos(), Get_Angle(), VEC_ONE);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Pina_Ex.wav", m_fSoundVolume);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Pina_ExSkill_1.ogg", m_fSoundVolume + 0.1f); 
}

void CChar_Pina::Trigger_1()
{
	Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_MoveSpeed, 30.f));
	Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_Dodge, 30.f));
	Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_HealPower, 30.f));
	Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_HIT, 30.f));
}

// ExCutin
void CChar_Pina::Trigger_9()
{
	CUtility::Print_Wstring_DMode(L"ÄÆ½Å");
}
