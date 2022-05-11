#include "Char_Sumire.h"
#include "UI_Manager.h"
#include "CollisionManager.h"
USING(Engine)

CChar_Sumire::CChar_Sumire(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Sumire::CChar_Sumire(const CChar_Sumire & rhs)
	: CCharacter(rhs)
{
}

CChar_Sumire::~CChar_Sumire()
{
}

CChar_Sumire * CChar_Sumire::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Sumire*	pInstance = new CChar_Sumire(pGraphicDev, eObjTag, L"Sumire");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Sumire::Free()
{
	CCharacter::Free();
}

void CChar_Sumire::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Sumire::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Sumire::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon_R";

	Add_Component();

	return S_OK;
}

_int CChar_Sumire::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Sumire::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Sumire::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Sumire::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Sumire::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Sumire::Set_GuideSkill()
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CSkill_Manager::GetInstance()->SetSkillGuideFan(Get_Position(), Get_Angle(), 5.f, 30.f, 9999.f, 10, FALSE, L"../../Reference/Resource/Texture/Test/GuideTexBlue.png");
	if (pGuide)
	{
		m_vecGuideSkill.emplace_back(pGuide);
	}
}

void CChar_Sumire::ColMonster(CGameObject * pThis, CGameObject * pMonster)
{
	CParticleObject* pHitParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"MomoiHit.particle");
	if (pHitParticle)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pHitParticle->Get_ObjTag(), pHitParticle);
		pHitParticle->Set_Scale(_vec3(2.f, 2.f, 2.f));
		pHitParticle->Set_Pos(pMonster->Get_Position());
	}

	static_cast<CUnit*>(pMonster)->Set_Damage(static_cast<CUnit*>(pThis)->Get_UnitInfo(), static_cast<CCharacter*>(pThis)->Get_CharInfo().eAtkType);
}

void CChar_Sumire::ExSkill(_float fDeltaTime)
{
	CCharacter::ExSkill(fDeltaTime);

	if (m_iExMoveType == 1)
	{
		Add_Pos(m_vExRight * 2.f * fDeltaTime);
	}
	else if (m_iExMoveType == 2)
	{
		Add_Pos(m_vExLeft * 2.f * fDeltaTime);
	}
	else if (m_iExMoveType == 3)
	{
		Add_Pos(-m_vExDir * 2.f * 1.f * fDeltaTime);
	}
}

void CChar_Sumire::End_Battle_Sound(const _bool & bWin)
{
	bWin ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_Battle_Victory_2.ogg", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_Tactic_Defeat_2.ogg", m_fSoundVolume); 
}

void CChar_Sumire::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Sumire::Begin_Battle_Sound()
{
	CSound_Manager::GetInstance()->PlaySound_(L"Sumire_Battle_TacticalAction_1.ogg", E_ChannelID::Channel2_1, E_SoundType::Voice, m_fSoundVolume);
}

void CChar_Sumire::Trigger_Attack_1()
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

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vWeaponPos, Get_Angle(), L"Shot_Effect_OneShot.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_06.wav", m_fSoundVolume);

	m_iExMoveType = 0;

	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	CCollisionManager::GetInstance()->Collision_FanToCircle(this, Get_Position(), vLook, 30.f, 5.f, E_LAYERTAG::Monster, ColMonster);
}

void CChar_Sumire::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_Reload_Cart_01.wav", m_fSoundVolume); 
}

void CChar_Sumire::Trigger_ExSkill()
{
	m_vExDir	= CUI_Manager::GetInstance()->m_vPickingPos - Get_Position();
	m_vExDir.y	= 0.f;
	D3DXVec3Normalize(&m_vExDir, &m_vExDir);

	m_vExRight	= _vec3(cosf(60.f) * m_vExDir.x - sinf(60.f) * m_vExDir.z, 0.f, sinf(60.f) * m_vExDir.x + cosf(60.f) * m_vExDir.z);
	m_vExLeft	= _vec3(cosf(-60.f) * m_vExDir.x - sinf(-60.f) * m_vExDir.z, 0.f, sinf(-60.f) * m_vExDir.x + cosf(-60.f) * m_vExDir.z);
	m_vExDir	= Get_Position() - CUI_Manager::GetInstance()->m_vPickingPos;
	m_vExDir.y = 0.f;
	D3DXVec3Normalize(&m_vExDir, &m_vExDir);

	m_iExMoveType = 1;

	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Sumire_ExSkill_Level_2.wav", m_fSoundVolume);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Sumire_Public.wav", m_fSoundVolume);

	CSkill_Manager::GetInstance()->SetSkill(L"Sumire_Ex.skill", E_LAYERTAG::Effect, m_pWeaponCollider->GetPos(), Get_Angle(), VEC_ONE);
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_ExSkill_Level_2.ogg", m_fSoundVolume + 0.1f); 
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_01.wav", m_fSoundVolume);

}

void CChar_Sumire::Trigger_1()
{
	m_iExMoveType = 4;
}

void CChar_Sumire::Trigger_2()
{
	m_iExMoveType = 2;
	CSkill_Manager::GetInstance()->SetSkill(L"Sumire_Ex.skill", E_LAYERTAG::Effect, m_pWeaponCollider->GetPos(), Get_Angle(), VEC_ONE);
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_ExSkill_Level_3.ogg", m_fSoundVolume + 0.1f);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_06.wav", m_fSoundVolume);
}

void CChar_Sumire::Trigger_3()
{
	m_iExMoveType = 4;
}

void CChar_Sumire::Trigger_4()
{
	m_iExMoveType = 3;
	CSkill_Manager::GetInstance()->SetSkill(L"Sumire_Ex.skill", E_LAYERTAG::Effect, m_pWeaponCollider->GetPos(), Get_Angle(), VEC_ONE);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_ExSkill_Level_1.ogg", m_fSoundVolume + 0.1f);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SG_01.wav", m_fSoundVolume);
}

void CChar_Sumire::Trigger_5()
{
	m_iExMoveType = 4;
}

// ExCutin
void CChar_Sumire::Trigger_9()
{
	CUtility::Print_Wstring_DMode(L"ÄÆ½Å");
	_float fAngle = Get_Angle().y;
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Sumire_Ex.camdat", L"Sumire", Get_Position(), fAngle);
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Sumire_ExSkill_Level_3.wav", m_fSoundVolume);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Sumire_Ex_Cut_in.wav", m_fSoundVolume);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Sumire_ExSkill_1.ogg", m_fSoundVolume + 0.1f);
}

void CChar_Sumire::GuideSkillSetting(_vec3 vPickPos)
{
	_vec3 vDir = vPickPos - Get_Position();

	for (auto& pGuide : m_vecGuideSkill)
	{
		pGuide->Set_Pos(Get_Position());
		pGuide->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));
	}
}
