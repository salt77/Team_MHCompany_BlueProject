#include "Char_Aru.h"
#include "Warning.h"
USING(Engine)

CChar_Aru::CChar_Aru(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Aru::CChar_Aru(const CChar_Aru & rhs)
	: CCharacter(rhs)
{
}

CChar_Aru::~CChar_Aru()
{
}

CChar_Aru * CChar_Aru::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Aru*	pInstance = new CChar_Aru(pGraphicDev, eObjTag, L"Aru");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Aru::Free()
{
	CCharacter::Free();
}

void CChar_Aru::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Aru::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Aru::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	m_tUnitSound.pairReload.first = L"SFX_Common_SR_Reload_03.wav";

	return S_OK;
}

_int CChar_Aru::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Aru::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_F8))
	{
		FSM_FunctionSetting(&CUnit::ExSkill_CurIn, nullptr);
	}

	return iExit;
}

_int CChar_Aru::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);
	   
	return iExit;
}

void CChar_Aru::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Aru::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Aru::Set_GuideSkill()
{
}


void CChar_Aru::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Aru::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();
	
	Hit_Target();

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.6f), Get_Angle(), 1.f);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_01.wav", 0.5f);
}

void CChar_Aru::Trigger_Reload()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_Reload_03.wav", 1.f);
}

void CChar_Aru::Trigger_ExSkill()
{
	CSkill_Manager::GetInstance()->SetSkill(L"EH_Public_Exs_Effect_0110.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle());

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Aru_Skill_EX_1.wav", 1.f);
}

void CChar_Aru::Trigger_1()
{
	m_tUnitInfo.iAttack = 200;

	Hit_Target_Enum(E_UNIT_HIT_TYPE::HIT_EXS);

	CSkill_Manager::GetInstance()->SetSkill(L"EH_Aru_Exs_Shot_1_0110.skill", E_LAYERTAG::Effect, Get_WeaponPos(-0.1f, 0.f, 0.3f), Get_Angle(), 2.f);

	//Create_Effect(L"EH_Aru_Exs_Shot_1_0110.skill", -0.1f, 0.f, 0.3f);	

	if (m_pExTarget) 
	{
		m_vPos_Exs = static_cast<CUnit*>(m_pExTarget)->Get_SphereCollider()->GetPos();
		m_vRot_Exs = m_pExTarget->Get_Angle();
	}
	else
	{
		m_vPos_Exs = Get_Position();
		m_vRot_Exs = _vec3(0.f, 0.f, 0.f);

	}

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Aru_Skill_EX_2.wav", 0.5f);
}


void CChar_Aru::Create_HitEffect()
{
	if (E_UNIT_HIT_TYPE::HIT_EXS == m_eUnitHitType) 
	{
		if (m_pExTarget) 
		{
			m_vPos_Exs = static_cast<CUnit*>(m_pExTarget)->Get_SphereCollider()->GetPos();
			m_vRot_Exs = m_pExTarget->Get_Angle();

		}
		else 
		{
			m_vPos_Exs = Get_Position();
			m_vRot_Exs = _vec3(0.f, 0.f, 0.f);
		}

		CSkill_Manager::GetInstance()->SetSkill(L"EH_Aru_Exs_Bomb_1_0110.skill", E_LAYERTAG::Effect, m_vPos_Exs, m_vRot_Exs);

		// 시한 폭탄
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Aru_Skill_EX_3.wav", 2.f);

		Invoke_Add(3.4f, bind(&CChar_Aru::AruBomb, this));
	}
	else
	{
		CUnit::Create_HitEffect();
	}
}

void CChar_Aru::AruBomb()
{
	// 몬스터가 사라지거나, 이동했을 수 있으므로, 변수에 저장된 위치값을 사용했다.
	CSkill_Manager::GetInstance()->SetSkill(L"Mushroom_Cloud.skill", E_LAYERTAG::Effect, m_vPos_Exs, m_vRot_Exs, VEC_ONE * 3.5f);

	// 폭팔 광음
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_NormalAttack_Hit_Start.wav", 3.f);

	m_vPos_Exs = Get_Position();
	m_vRot_Exs = _vec3();
}

void CChar_Aru::GuideSkillSetting(_vec3 vPickPos)
{
	_float fMinDistance = 999999.f;
	CGameObject* pExTarget = nullptr;
	for (auto& tPair : *CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Monster))
	{
		for (auto& pMonster : tPair.second)
		{
			if(!static_cast<CUnit*>(pMonster)->Get_UnitDead())
			{
				_float fDist = D3DXVec3Length(&(vPickPos - pMonster->Get_Position()));
				if (fDist < fMinDistance)
				{
					fMinDistance = fDist;
					pExTarget = pMonster;
				}
			}
		}
	}

	if (pExTarget)
	{
		m_pExTarget = pExTarget;
		m_vExDir = m_pExTarget->Get_Position() - Get_Position();
	}
	else
	{
		m_pExTarget = nullptr;
		m_vExDir = -VEC_LOOK;
	}

}

void CChar_Aru::Trigger_9()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Aru_Ex.camdat", L"Aru", Get_Position(), Get_Angle().y);
	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Aru_Exs_00.particle");
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);
	pParticle->Set_Pos(m_pSphereCollider->GetPos());

	// 컷인 대사 & 소리
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"Aru_ExSkill_3.ogg", 2.5f);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Skill_Aru_Ex_Cut_in.wav", 1.f);
}