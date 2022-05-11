#include "HeavyTank_Decagram_Cannon.h"
#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 

#include "UI_Manager.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CHeavyTank_Decagram_Cannon::CHeavyTank_Decagram_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"HeavyTank_Decagram_Cannon")
{
}

CHeavyTank_Decagram_Cannon::CHeavyTank_Decagram_Cannon(const CHeavyTank_Decagram_Cannon & rhs)
	: CMonster(rhs)
{
}

CHeavyTank_Decagram_Cannon::~CHeavyTank_Decagram_Cannon()
{
}

CHeavyTank_Decagram_Cannon * CHeavyTank_Decagram_Cannon::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHeavyTank_Decagram_Cannon* pInstance = new CHeavyTank_Decagram_Cannon(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeavyTank_Decagram_Cannon::Free()
{
	CMonster::Free();
}

HRESULT CHeavyTank_Decagram_Cannon::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Bone_Lower";
	m_szWeaponBoneName = "Bone_Cannon_02";
	Set_AmmoMax(2);

	m_tUnitInfo.iAttack = 500;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_bNormalBoss ? m_tUnitInfo.iMaxHp = 50000 : m_tUnitInfo.iMaxHp = 35000; 
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 1100;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CHeavyTank_Decagram_Cannon::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	m_fMonsterExSkillTimer = CWall512::GetInstance()->GetFloatValue(5.f, 7.f); 

	if (m_bNormalBoss)
	{
		m_bNormalBoss ? m_tUnitInfo.iMaxHp = 50000 : m_tUnitInfo.iMaxHp = 35000;
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;

		m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"무거운 양산형 탱크", m_tUnitInfo.iMaxHp, 10, L"[Fixed]Boss_Hp_Bar_Normal.uidat");
		assert(m_pBossHpBar);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI_BossHpbar, m_pBossHpBar);
	}

	return RETURN_OK;
}

_int CHeavyTank_Decagram_Cannon::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Create_Bomb(fTimeDelta);

	return RETURN_OK;
}

_int CHeavyTank_Decagram_Cannon::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CHeavyTank_Decagram_Cannon::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CHeavyTank_Decagram_Cannon::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CHeavyTank_Decagram_Cannon::OnEnable()
{
	CMonster::OnEnable();
}

void CHeavyTank_Decagram_Cannon::OnDisable()
{
	CMonster::OnDisable();
}

void CHeavyTank_Decagram_Cannon::OnDestroy()
{
	CMonster::OnDestroy();
}

void CHeavyTank_Decagram_Cannon::Trigger_Attack_1()
{
	if (!m_pTarget)
	{
		return;
	}

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

	Create_FireEffect();

	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
																 vWeaponPos,
																 *D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
																 m_tUnitInfo,
																 m_tMonsterInfo.eAtkType,
																 CBullet_Normal_Monster::Move_TankBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject); 

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume); 
}

void CHeavyTank_Decagram_Cannon::Trigger_1()
{
	Create_FireEffect(); 

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);

	if (!m_pTarget)
	{
		return;
	}
}

void CHeavyTank_Decagram_Cannon::Trigger_2()
{
	if (!m_pTarget)
	{
		return;
	}

	m_bCreateBomb = TRUE;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pTarget->Get_Position(), Get_Angle(), L"Tank_AirBombTarget.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);

	CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_pTarget->Get_Position(), 2.5f, m_fSkillArriveTime, 30, TRUE);

	m_vTargetPos = m_pTarget->Get_Position();

	m_fMonsterExSkillOriginTimer = CWall512::GetInstance()->GetFloatValue(13.f, 20.f); 
}

void CHeavyTank_Decagram_Cannon::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Tank_Appear_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Tank_Appear_02.wav", m_fSoundVolume);
}

void CHeavyTank_Decagram_Cannon::Trigger_Dead()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Common_Death_01.wav", m_fSoundVolume); 
}

void CHeavyTank_Decagram_Cannon::Trigger_ExSkill()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_Ex_a_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_Ex_a_02.wav", m_fSoundVolume); 
}

void CHeavyTank_Decagram_Cannon::Create_FireEffect()
{
	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Smoke_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.5f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Normal.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.5f);
}

void CHeavyTank_Decagram_Cannon::Create_Bomb(const _float & fTimeDelta)
{
	if (m_bCreateBomb)
	{
		m_fSkillArriveTime -= fTimeDelta;

		if (0.f >= m_fSkillArriveTime)
		{
			_vec3	vPos = m_vTargetPos;
			vPos.y = m_vTargetPos.y + 7.5f;

			CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev, vPos, -VEC_UP, m_tUnitInfo, m_tMonsterInfo.eAtkType, CBullet_Normal_Monster::Move_FallDown);
			assert(pGameObject);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);

			m_bCreateBomb = FALSE;
			m_fSkillArriveTime = m_fSkillArriveOriginTime;
		}
	}
}
