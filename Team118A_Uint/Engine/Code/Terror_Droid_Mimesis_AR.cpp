#include "Terror_Droid_Mimesis_AR.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CTerror_Droid_Mimesis_AR::CTerror_Droid_Mimesis_AR(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Terror_Droid_Mimesis_AR")
{
}

CTerror_Droid_Mimesis_AR::CTerror_Droid_Mimesis_AR(const CTerror_Droid_Mimesis_AR & rhs)
	: CMonster(rhs)
{
}

CTerror_Droid_Mimesis_AR::~CTerror_Droid_Mimesis_AR()
{
}

CTerror_Droid_Mimesis_AR * CTerror_Droid_Mimesis_AR::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CTerror_Droid_Mimesis_AR* pInstance = new CTerror_Droid_Mimesis_AR(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerror_Droid_Mimesis_AR::Free()
{
	CMonster::Free();
}

HRESULT CTerror_Droid_Mimesis_AR::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName;
	m_szWeaponBoneName = "bone_magazine";

	Set_AmmoMax(30);

	m_tUnitInfo.iAttack = 35;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 3000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 700;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CTerror_Droid_Mimesis_AR::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	m_fMonsterExSkillTimer = 7.5f; 

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_AR::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_AR::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CTerror_Droid_Mimesis_AR::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CTerror_Droid_Mimesis_AR::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CTerror_Droid_Mimesis_AR::OnEnable()
{
	CMonster::OnEnable();
}

void CTerror_Droid_Mimesis_AR::OnDisable()
{
	CMonster::OnDisable();
}

void CTerror_Droid_Mimesis_AR::OnDestroy()
{
	CMonster::OnDestroy();
}

void CTerror_Droid_Mimesis_AR::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	m_fMonsterExSkillTimer -= fDeltaTime;

	if (0.f >= m_fMonsterExSkillTimer)
	{
		m_fMonsterExSkillTimer = m_fMonsterExSkillOriginTimer;

		FSM_FunctionSetting(&CUnit::Special_State_1, &CUnit::Special_State_2);
	}

	CUnit::Update_FSM(fDeltaTime, eTargetLayer, eObjTag);
}

void CTerror_Droid_Mimesis_AR::Special_State_1(_float fDeltaTime)
{
	E_ACTION eMain = E_ACTION::Temp1;
	m_eCurrentAction = eMain;

	Animation_Change(fDeltaTime);

	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		m_bStateLock = FALSE;
	}
}

void CTerror_Droid_Mimesis_AR::Special_State_2(_float fDeltaTime)
{
}

void CTerror_Droid_Mimesis_AR::Trigger_Attack_1()
{
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

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_Automatic_01.wav", m_fSoundVolume); 

	Hit_Target();
}

void CTerror_Droid_Mimesis_AR::Trigger_1()
{
	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Automata_Buff_Ground.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Automata_Buff_Light.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

	// 버프 
	static_cast<CUnit*>(this)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_HIT, 7.5f)); 

	if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster))
	{
		vector<CGameObject*>* vecGameObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);

		for (_uint i = 0; i < vecGameObject->size(); ++i)
		{
			if (static_cast<CUnit*>((*vecGameObject)[i])->Get_UnitDead())
			{
				continue;
			}

			static_cast<CUnit*>((*vecGameObject)[i])->Add_CombatStatus(T_CombatStatus(E_Combat_Status::B_HIT, 7.5f)); 

			pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, (*vecGameObject)[i]->Get_Position(), (*vecGameObject)[i]->Get_Angle(), L"Automata_Buff_Ground.effect");
			assert(pGameObject);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
			static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

			pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, (*vecGameObject)[i]->Get_Position(), (*vecGameObject)[i]->Get_Angle(), L"Automata_Buff_Light.effect");
			assert(pGameObject);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
			static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);
		}
	}

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Public_버프.wav", m_fSoundVolume); 
}
