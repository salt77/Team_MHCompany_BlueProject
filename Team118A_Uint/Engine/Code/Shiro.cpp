#include "Shiro.h"
#include "UI_Damage.h"

#include "UI_Manager.h"
#include "Boss_HpBar.h"
#include "Shiro_Bomb_S.h" 
#include "Shiro_Ball.h"

USING(Engine)

CShiro::CShiro(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CBossMonster(pGraphicDev, eObjTag, L"Shiro")
{
}

CShiro::CShiro(const CShiro & rhs)
	: CBossMonster(rhs)
{
}

CShiro::~CShiro()
{
}

CShiro * CShiro::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CShiro* pInstance = new CShiro(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CShiro::Free()
{
	CBossMonster::Free();
}

HRESULT CShiro::Ready_Object()
{
	CBossMonster::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	m_tUnitInfo.iAttack = 1;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 125356;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 99999;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CShiro::Start_Object(const _float & fTimeDelta)
{
	CBossMonster::Start_Object(fTimeDelta);

	OnEnable();

	return RETURN_OK;
}

_int CShiro::Update_Object(const _float & fTimeDelta)
{
	CBossMonster::Update_Object(fTimeDelta);

	Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	Animation_Change(fTimeDelta); 

	return RETURN_OK;
}

_int CShiro::LateUpdate_Object(const _float & fTimeDelta)
{
	CBossMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CShiro::Render_Object(_int iPass)
{
	CBossMonster::Render_Object(iPass);
}

void CShiro::Set_Constantable()
{
	CBossMonster::Set_Constantable();
}

void CShiro::OnEnable()
{
	CBossMonster::OnEnable();

	CGameObject*	pGameObject = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Shiro", m_tUnitInfo.iMaxHp, 25);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, pGameObject->Get_ObjTag(), pGameObject);
	static_cast<CUI_Object*>(pGameObject)->Set_ImageAlpha(0.f);

	m_pBossHpBar = static_cast<CBoss_HpBar*>(pGameObject);
}

void CShiro::OnDisable()
{
	CBossMonster::OnDisable();
}

void CShiro::OnDestroy()
{
	CBossMonster::OnDestroy();
}

void CShiro::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (!m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
	}

	m_fCountingTime += fDeltaTime;

	if (m_fExSkill2_Gage >= m_fExSkill2_MaxGage)
	{
		m_fCountingTime = 0.f; 
		m_fExSkill2_Gage = 0.f; 

		FSM_FunctionSetting(&CUnit::Boss_Attack_1, &CUnit::Idle);
	}
	else if (m_fExSkill_Cool <= m_fCountingTime)
	{
		m_fCountingTime = 0.f;

		FSM_FunctionSetting(&CUnit::ExSkill, &CUnit::Idle);
	}

	if (FALSE == m_bStateLock)
	{
		if (nullptr == m_pTarget)
		{
			Find_Target(eTargetLayer, eObjTag);

			if (nullptr == m_pTarget)
				return;
		}
		else
		{
			FSM_FunctionSetting(&CUnit::Shoot_Start, &CUnit::Shoot);
		}

		_float fDistToTarget = CUtility::Distance_vec3(Get_Position(), m_pTarget->Get_Position());

		if (fDistToTarget > Get_UnitRange())
		{
			if (m_fFindTargetCool <= m_fFindTargetTime)
			{
				Find_Target(eTargetLayer, eObjTag);

				m_fFindTargetTime = 0.f;
			}
		}
	}
	else
	{
		if (m_fpLockFunction)
		{
			((*this).*m_fpLockFunction)(fDeltaTime);
		}
		else
		{
			if (Is_AnimFinished())
			{
				m_bStateLock = FALSE;
			}
		}
	}

	m_fFindTargetTime += fDeltaTime;
}

void CShiro::Begin_Battle(_float fDeltaTime)
{
	if (Is_AnimFinished())
	{
		m_bStateLock = FALSE;
		m_fpLockFunction = nullptr;

		return;
	}

	if (!m_pTarget)
	{
		Find_Target(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player); 
	}

	Set_ActionByEnum(E_ACTION::Appear); 
}

void CShiro::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;
	}
}

void CShiro::Idle(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Normal_Idle; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_Start);
	}
}

void CShiro::Shoot_Start(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Normal_Shoot_Start; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot); 
	}
}

void CShiro::Shoot(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Normal_Shoot; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_End);
	}
}

void CShiro::Shoot_End(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Normal_Shoot_End; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Idle);
	}
}

void CShiro::Dead(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Vital_Death;
	m_pTarget = nullptr;
}

void CShiro::ExSkill(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_Start);
	}
}

void CShiro::Boss_Attack_1(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::ExSkill_CutIn; 

	m_fExSkill2_Gage = 0.f; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_Start);
	}
}

void CShiro::Move_Left(const _float & fTimeDelta)
{
	m_eCurrentAction = E_ACTION::Move_Ing; 

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_Start);
	}
}

void CShiro::Move_Right(const _float & fTimeDelta)
{
	m_eCurrentAction = E_ACTION::Move_CallSign;

	if (Is_AnimFinished())
	{
		FSM_FunctionSetting(m_fpDestFunction, &CUnit::Shoot_Start);
	}
}

void CShiro::Trigger_Attack_1()
{
	CUnit*	pChar = static_cast<CUnit*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player, CWall512::GetInstance()->GetValue(0, 4)));

	if (!pChar)
	{
		return; 
	}

	_vec3	vMyPos = Get_Position();
	vMyPos.y += 2.5f;

	CGameObject*	pGameObject = CShiro_Bomb_S::Create(m_pGraphicDev, vMyPos, pChar->Get_Position(), m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::Character, TRUE);
	assert(pGameObject); 
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CShiro::Trigger_ExSkill()
{

}

void CShiro::Trigger_1()
{
	_vec3	vMyPos = Get_Position();
	vMyPos.y += 2.5f;

	CGameObject*	pGameObject = CShiro_Ball::Create(m_pGraphicDev, Get_Info(E_INFO::INFO_LOOK), vMyPos, m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::Character);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CShiro::Trigger_2()
{
	CUnit*	pChar = static_cast<CUnit*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player));

	if (pChar)
	{
		_vec3	vMyPos = pChar->Get_Position();
		vMyPos.y += 6.f;

		_float	fDistance = CUtility::Distance_vec3(Get_Position(), pChar->Get_Position());

		CGameObject*	pGameObject = CShiro_Bomb_S::Create(m_pGraphicDev, vMyPos, pChar->Get_Position(), m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::Character, FALSE);
		assert(pGameObject);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);

		for (_uint i = 0; i < 3; ++i)
		{
			_vec3	vDir = _vec3(0.f, 0.f, 1.f);
			CUtility::Rotate_DirVector(CWall512::GetInstance()->GetFloatValue(0.f, 359.f), VEC_UP, &vDir);

			vMyPos += vDir * CWall512::GetInstance()->GetFloatValue(0.f, 3.f);

			pGameObject = CShiro_Bomb_S::Create(m_pGraphicDev, vMyPos, pChar->Get_Position(), m_tUnitInfo, m_tMonsterInfo.eAtkType, E_LAYERTAG::Character, FALSE);
			assert(pGameObject);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
		}
	}
}
