#include "Char_Hina.h"
#include "CollisionManager.h"
#include "Monster.h"

USING(Engine)

CChar_Hina::CChar_Hina(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Hina::CChar_Hina(const CChar_Hina & rhs)
	: CCharacter(rhs)
{
}

CChar_Hina::~CChar_Hina()
{
}

CChar_Hina * CChar_Hina::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Hina*	pInstance = new CChar_Hina(pGraphicDev, eObjTag, L"Hina");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Hina::Free()
{
	CCharacter::Free();
}

void CChar_Hina::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Hina::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Hina::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	//for (auto iter : m_pDMeshCom->Get_MeshContainerList())
	//{
	//	iter->fTexGlowB = 1.f;
	//}

	//T_GlowOption tGLowOption = m_pDMeshCom->Get_Glow_Option();
	//
	//m_pDMeshCom->Set_Glow_Option(tGLowOption);

	return S_OK;
}

_int CChar_Hina::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Hina::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_F6))
	{
		FSM_FunctionSetting(&CUnit::ExSkill_CurIn, nullptr);
	}

	return iExit;
}

_int CChar_Hina::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Hina::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Hina::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Hina::ColMonster(CGameObject * pThis, CGameObject * pMonster)
{
	//CParticleObject* pHitParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"MomoiHit.particle");
	//
	//if (!pHitParticle)
	//{
	//	return;
	//}
	//pHitParticle->Set_Scale(_vec3(2.f, 2.f, 2.f));
	//pHitParticle->Set_Pos(pMonster->Get_Position());
	//
	//CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pHitParticle->Get_ObjTag(), pHitParticle);

	// 몬스터에게 데미지를 준다
	static_cast<CUnit*>(pMonster)->Set_Damage(static_cast<CUnit*>(pThis)->Get_UnitInfo(), static_cast<CCharacter*>(pThis)->Get_CharInfo().eAtkType);
}

void CChar_Hina::Add_Component()
{
	CComponent* pComponent = nullptr;
}

void CChar_Hina::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.85f), Get_Angle(), 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_MG_01.wav", 1.f);
}

void CChar_Hina::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_HMG_Reload_Hina_03.wav", 1.f);
}

void CChar_Hina::Trigger_ExSkill()
{
	// 스킬 시작시 
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

	StartCam();

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Hina_ExSkill_1.ogg", 2.f);
}

void CChar_Hina::Trigger_1()
{
	// 스킬 사용중 어둠 오라 이펙트
	Invoke_Add(0.3f, bind(&CChar_Hina::HinaAura, this));

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Hina_Ex.wav", 1.f);
}

void CChar_Hina::Trigger_2()
{
	// 총구 발사 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Hina_Exs_Cutin_3.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.6f), Get_Angle());
	CSkill_Manager::GetInstance()->SetSkill(L"Hina_Exs_Cutin_4.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.6f), Get_Angle(), 2.5f);

	for (size_t i = 0; i < 10; i++)
	{
		float fDelay = 0.4f * (_float)i;

		Invoke_Add(fDelay, bind(&CChar_Hina::ExsHina, this));
	}
}

void CChar_Hina::Trigger_3()
{
	// 카메라 컷인 풍압 이펙트
	CSkill_Manager::GetInstance()->SetSkill(L"Hina_Exs_Cutin.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle() + _vec3(0.f, 0.2f, 0.f),1.4f);
}

void CChar_Hina::HinaAura()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Hina_Exs_Cutin_2.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 2.f);
}

// 카메라 


void CChar_Hina::StartCam()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Hina_Ex.camdat", L"Hina_0", Get_Position(), Get_Angle().y);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Hina_Ex_Cut_in.wav", 1.f);

}

void CChar_Hina::ExsHina()
{
	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	CCollisionManager::GetInstance()->Collision_FanToCircle(this, Get_Position(), vLook, 25.f, 14.f, E_LAYERTAG::Monster, ColMonster);
}

void CChar_Hina::Trigger_9()
{
}

void CChar_Hina::Trigger_8()
{
}

void CChar_Hina::Trigger_7()
{
	CActionCam_Mgr::GetInstance()->Stop_ActionCam();
}
