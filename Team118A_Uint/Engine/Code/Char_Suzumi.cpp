#include "Char_Suzumi.h"

USING(Engine)

CChar_Suzumi::CChar_Suzumi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Suzumi::CChar_Suzumi(const CChar_Suzumi & rhs)
	: CCharacter(rhs)
{
}

CChar_Suzumi::~CChar_Suzumi()
{
}

CChar_Suzumi * CChar_Suzumi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Suzumi*	pInstance = new CChar_Suzumi(pGraphicDev, eObjTag, L"Suzumi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Suzumi::Free()
{
	CCharacter::Free();
}

void CChar_Suzumi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Suzumi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Suzumi::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Suzumi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Suzumi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	//if (CKeyManager::GetInstance()->Key_Down(KEY_F) || CKeyManager::GetInstance()->Key_Down(KEY_G))
	//{
	//	vector<CGameObject*>& pVec = *CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);
	//	for (_int i = 0; i < (_int)pVec.size(); ++i)
	//	{
	//		if (static_cast<CUnit*>(pVec[i])->Get_UnitDead())
	//			continue;
	//
	//		if (CKeyManager::GetInstance()->Key_Down(KEY_F))
	//		{
	//			static_cast<CUnit*>(pVec[i])->CC_Taunt(3.f, this);
	//		}
	//		if (CKeyManager::GetInstance()->Key_Down(KEY_G))
	//		{
	//			static_cast<CUnit*>(pVec[i])->CC_Fear(3.f);
	//		}
	//	}
	//}

	return iExit;
}

_int CChar_Suzumi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Suzumi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Suzumi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Suzumi::End_Battle_Sound(const _bool & bWin)
{
	bWin ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Suzumi_Tactic_Victory_1.ogg", m_fSoundVolume + 0.1f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Suzumi_Tactic_Defeat_1.ogg", m_fSoundVolume + 0.1f); 
}

void CChar_Suzumi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Suzumi::Begin_Battle_Sound()
{
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"")
}

void CChar_Suzumi::Trigger_Attack_1()
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
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE); 
}

void CChar_Suzumi::Trigger_Attack_3()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_01.wav", m_fSoundVolume - 0.1f) : 
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_05.wav", m_fSoundVolume - 0.1f);
}

void CChar_Suzumi::Trigger_Reload()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_Reload_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_AR_Reload_05.wav", m_fSoundVolume);
}

void CChar_Suzumi::Trigger_ExSkill()
{
	// 수류탄 까는소리
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Suzumi_Skill_EX_Start.wav", m_fSoundVolume + 0.5f);

	// 1초 뒤
	Invoke_Add(1.5f, bind(&CChar_Suzumi::Granade, this));
}

// ExCutin
void CChar_Suzumi::Trigger_9()
{
	CUtility::Print_Wstring_DMode(L"컷신");
}

void CChar_Suzumi::Granade()
{
	_vec3 vPos = VEC_ZERO;
	if (m_pTarget)
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Suzumi_Ex_Granade_After.skill", E_LAYERTAG::Effect, m_pTarget->Get_Position(), Get_Angle(), VEC_ONE);
		vPos = m_pTarget->Get_Position();
	}
	else
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Suzumi_Ex_Granade_After.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE);
		vPos = Get_Position();
	}

	vector<CGameObject*>& pVec = *CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);
	for (_int i = 0; i < (_int)pVec.size(); ++i)
	{
		if (static_cast<CUnit*>(pVec[i])->Get_UnitDead())
			continue;

		_float fDistance = CUtility::Distance_vec3(pVec[i]->Get_Position(), vPos);

		if (fDistance <= 3.f)
		{
			static_cast<CUnit*>(pVec[i])->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Emp, 3.f));
		}
	}


	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Suzumi_Skill_EX_Explosion.wav", m_fSoundVolume);
}
