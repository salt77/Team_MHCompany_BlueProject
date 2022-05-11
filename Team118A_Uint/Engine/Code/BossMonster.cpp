#include "BossMonster.h" 

USING(Engine)

CBossMonster::CBossMonster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CMonster(pGraphicDev, eObjTag, wstrUnitName)
{
}

CBossMonster::CBossMonster(const CBossMonster & rhs)
	: CMonster(rhs)
{
}

CBossMonster::~CBossMonster()
{
}

void CBossMonster::Free()
{
	CMonster::Free();
}

HRESULT CBossMonster::Ready_Object()
{
	CMonster::Ready_Object();

	return S_OK;
}

_int CBossMonster::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CMonster::Start_Object(fTimeDelta);

	m_pCombatObject->Set_Active(FALSE);
	return iExit;
}

_int CBossMonster::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CUnit::Update_Object(fTimeDelta);		// 보스 몬스터는 FSM 예외적이라서 

	if (RETURN_DEAD == iExit)
	{
		OnDestroy();
	}

	Update_ExSkill2_Gage(fTimeDelta);

	if (1.f <= Get_GroggyRatio())
	{
		FSM_FunctionSetting(&CUnit::Boss_Groggy, nullptr);
		m_fGroggy_Gage = 0.f;
		Add_Phase();
	}

	return iExit;
}

_int CBossMonster::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CMonster::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CBossMonster::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CBossMonster::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CBossMonster::OnEnable()
{
	CMonster::OnEnable();
}

void CBossMonster::OnDisable()
{
	CMonster::OnDisable();
}

void CBossMonster::OnDestroy()
{
	CMonster::OnDestroy();
}

void CBossMonster::Load_CombatUi()
{
	CMonster::Load_CombatUi();
}

void CBossMonster::Begin_Battle(_float fTimeDelta)
{
	FSM_FunctionRelease();
}

void CBossMonster::Boss_Groggy(_float fDeltaTime)
{
	Add_Phase();
	Release_Invoke();
	m_fGroggy_Gage = 0.f;
	m_fExSkill2_Gage = 0.f;
}

void CBossMonster::Boss_FSM_Check(void(CUnit::* pNext)(_float))
{
	if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
	{
		FSM_FunctionSetting(pNext, nullptr);
	}
	else
	{
		m_eCurrentAction = m_eMainAction;
	}
}

void CBossMonster::Set_MonsterInfo(T_MONSTER_INFO tMonsterInfo)
{
	CMonster::Set_MonsterInfo(tMonsterInfo);

	m_bNormalBoss = FALSE;
}

void CBossMonster::Update_ExSkill2_Gage(const _float & fTimeDelta)
{
	if (!m_pBossHpBar)
		return;

	m_fExSkill2_Gage += fTimeDelta * 0.1f;

	m_pBossHpBar->Set_Gage_OtherStat(m_fExSkill2_Gage);
}

_int CBossMonster::Set_Damage(const T_UnitInfo & tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType)
{
	_int	iReturn = CMonster::Set_Damage(tEnemyInfo, eEnemyAtkType);

	if (m_pBossHpBar)
	{
		m_pBossHpBar->Add_Damage_ForUI(-iReturn);
		Add_GroggyGage((_float)iReturn);

		if (0 >= m_tUnitInfo.iNowHp)
		{
			m_pBossHpBar->Set_Dead(TRUE);
			m_pBossHpBar = nullptr;
		}
	}

	return iReturn;
}

void CBossMonster::Add_GroggyGage(const _float & fDamage)
{
	// 받은 데미지 / 그로기 MaxHealth

	if (m_bInvn)
	{
		return;
	}
	m_fGroggy_Gage += fDamage;
	m_pBossHpBar->Set_Gage_Groggy(m_fGroggy_Gage / m_fGroggy_MaxGage);
}

// fPercentigy : 0 ~ 100 사이의 값을 받는다. 10 == 전체체력의 10%가 달면 그로기
void CBossMonster::Set_GroggyPercent(const _float & fPercentigy)
{
	m_fGroggy_MaxGage = m_tUnitInfo.iMaxHp * fPercentigy * 0.01f;
}
