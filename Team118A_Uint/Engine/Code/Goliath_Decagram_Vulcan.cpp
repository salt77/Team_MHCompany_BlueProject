#include "Goliath_Decagram_Vulcan.h"
#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

#include "UI_Manager.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CGoliath_Decagram_Vulcan::CGoliath_Decagram_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Goliath_Decagram_Vulcan")
{
}

CGoliath_Decagram_Vulcan::CGoliath_Decagram_Vulcan(const CGoliath_Decagram_Vulcan & rhs)
	: CMonster(rhs)
{
}

CGoliath_Decagram_Vulcan::~CGoliath_Decagram_Vulcan()
{
}

CGoliath_Decagram_Vulcan * CGoliath_Decagram_Vulcan::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CGoliath_Decagram_Vulcan* pInstance = new CGoliath_Decagram_Vulcan(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoliath_Decagram_Vulcan::Free()
{
	if (m_pWarningEffect)
	{
		m_pWarningEffect->Set_Dead(TRUE); 
		m_pWarningEffect = nullptr; 
	}
	if (m_pChargeEffect)
	{
		m_pChargeEffect->Set_Dead(TRUE); 
		m_pChargeEffect = nullptr; 
	}

	CMonster::Free();
}

HRESULT CGoliath_Decagram_Vulcan::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Vulcan_R_02";

	Set_AmmoMax(100);

	m_tUnitInfo.iAttack = 500;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 80000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 800;
	m_tUnitInfo.iStability;

	Create_Collider(&m_pSecondWeaponCol, L"Com_Collider_SecondWeapon", 0.1f);
	Create_Collider(&m_pExWeaponCol, L"Com_Collider_ExWeapon", 0.1f);

	m_fMonsterExSkillOriginTimer = CWall512::GetInstance()->GetFloatValue(10.f, 16.f); 

	return S_OK;
}

_int CGoliath_Decagram_Vulcan::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	if (m_bNormalBoss)
	{
		m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"골리앗", m_tUnitInfo.iMaxHp, 10, L"[Fixed]Boss_Hp_Bar_Normal.uidat");
		assert(m_pBossHpBar);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI_BossHpbar, m_pBossHpBar);
	}

	return RETURN_OK;
}

_int CGoliath_Decagram_Vulcan::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pSecondWeaponCol, m_szSecondWeaponBone);
	Collider_SetMatrix(m_pExWeaponCol, m_szExWeaponBone); 

	return RETURN_OK;
}

_int CGoliath_Decagram_Vulcan::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CGoliath_Decagram_Vulcan::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CGoliath_Decagram_Vulcan::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CGoliath_Decagram_Vulcan::OnEnable()
{
	CMonster::OnEnable();
}

void CGoliath_Decagram_Vulcan::OnDisable()
{
	CMonster::OnDisable();
}

void CGoliath_Decagram_Vulcan::OnDestroy()
{
	CMonster::OnDestroy();
}

void CGoliath_Decagram_Vulcan::Trigger_Attack_1()
{
	if (!m_pTarget)
	{
		return;
	}

	m_vTargetPos = m_pTarget->Get_Position(); 

	Create_FireEffect(); 

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Attacking_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Attacking_02.wav", m_fSoundVolume); 
}

void CGoliath_Decagram_Vulcan::Trigger_Attack_2()
{
	if (!m_pTarget)
	{
		return;
	}

	Create_FireEffect();
}

void CGoliath_Decagram_Vulcan::Trigger_Attack_3()
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vLeftWeaponPos = m_pWeaponCollider->GetPos();
	vLeftWeaponPos += vDirLook * fLookRatio;
	vLeftWeaponPos += vDirRight * fRightRatio;
	vLeftWeaponPos += vDirUp * fUpRatio;

	_vec3 vRightWeaponPos = m_pSecondWeaponCol->GetPos();
	vRightWeaponPos += vDirLook * fLookRatio;
	vRightWeaponPos += vDirRight * fRightRatio;
	vRightWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
																 vLeftWeaponPos,
																 *D3DXVec3Normalize(&m_vOutTemp, &(m_vTargetPos - vLeftWeaponPos)),
																 m_tUnitInfo,
																 m_tMonsterInfo.eAtkType,
																 CBullet_Normal_Monster::Move_RocketLauncher); 
	assert(pGameObject); 
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject); 

	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
												 vRightWeaponPos,
												 *D3DXVec3Normalize(&m_vOutTemp, &(m_vTargetPos - vRightWeaponPos)),
												 m_tUnitInfo,
												 m_tMonsterInfo.eAtkType,
												 CBullet_Normal_Monster::Move_RocketLauncher);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CGoliath_Decagram_Vulcan::Trigger_1()
{
	if (!m_pTarget)
	{
		if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player)) 
		{
			m_pTarget = static_cast<CUnit*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player)); 
			assert(m_pTarget); 
		}

		return; 
	}

	m_vTargetPos = m_pTarget->Get_Position(); 

	m_pWarningEffect = CSkill_Manager::GetInstance()->SetSkill(L"Goliath_ChargeEx_Warning.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), 1.5f); 
	assert(m_pWarningEffect); 
	m_pWarningEffect->Set_Scale(VEC_ONE * 3.5f); 

	CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos, 5.f, 3.5f, 30, TRUE);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Ex.wav", m_fSoundVolume + 0.25f);
}

void CGoliath_Decagram_Vulcan::Trigger_2()
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 1.5f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = -0.35f;

	_vec3 vWeaponPos = m_pExWeaponCol->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	m_pChargeEffect = CSkill_Manager::GetInstance()->SetSkill(L"Goliath_ChargeEx.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle(), 2.f); 
	assert(m_pChargeEffect); 
}

void CGoliath_Decagram_Vulcan::Trigger_3()
{
	if (m_pWarningEffect)
	{
		m_pWarningEffect->Set_Dead(TRUE);
		m_pWarningEffect = nullptr;
	}
	if (m_pChargeEffect)
	{
		m_pChargeEffect->Set_Dead(TRUE); 
		m_pChargeEffect = nullptr; 
	}

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 1.5f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = -0.15f;

	_vec3 vWeaponPos = m_pExWeaponCol->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Goliath_ChargeEx_Fire.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle(), 2.f); 

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
																 vWeaponPos,
																 *D3DXVec3Normalize(&m_vOutTemp, &(m_vTargetPos - vWeaponPos)),
																 m_tUnitInfo,
																 m_tMonsterInfo.eAtkType,
																 CBullet_Normal_Monster::Move_GoliathEx); 
	assert(pGameObject); 
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject); 

	m_fMonsterExSkillOriginTimer = CWall512::GetInstance()->GetFloatValue(20.f, 25.f); 
}

void CGoliath_Decagram_Vulcan::Trigger_5()
{
}

void CGoliath_Decagram_Vulcan::Trigger_6()
{
}

void CGoliath_Decagram_Vulcan::Trigger_7()
{
	//CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Drop_01.wav", m_fSoundVolume);
}

void CGoliath_Decagram_Vulcan::Trigger_Dead()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Death_01.wav", m_fSoundVolume + 0.1f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Death_02.wav", m_fSoundVolume + 0.1f);
}

void CGoliath_Decagram_Vulcan::Trigger_Reload()
{

}

void CGoliath_Decagram_Vulcan::Trigger_Buff_1()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_a_01.wav", m_fSoundVolume + 0.1f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_a_02.wav", m_fSoundVolume + 0.1f);
}

void CGoliath_Decagram_Vulcan::Trigger_Buff_2()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_b_01.wav", m_fSoundVolume + 0.1f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_b_02.wav", m_fSoundVolume + 0.1f);
}

void CGoliath_Decagram_Vulcan::Trigger_4()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_c_01.wav", m_fSoundVolume + 0.1f) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Goliath_Reload_c_02.wav", m_fSoundVolume + 0.1f);
}

void CGoliath_Decagram_Vulcan::Create_FireEffect()
{
	Hit_Target(); 

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vLeftWeaponPos = m_pWeaponCollider->GetPos();
	vLeftWeaponPos += vDirLook * fLookRatio;
	vLeftWeaponPos += vDirRight * fRightRatio;
	vLeftWeaponPos += vDirUp * fUpRatio;

	_vec3 vRightWeaponPos = m_pSecondWeaponCol->GetPos();
	vRightWeaponPos += vDirLook * fLookRatio;
	vRightWeaponPos += vDirRight * fRightRatio;
	vRightWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vLeftWeaponPos, Get_Angle(), L"Shot_Effect_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.75f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vRightWeaponPos, Get_Angle(), L"Shot_Effect_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.75f);
}
