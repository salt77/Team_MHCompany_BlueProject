#include "Terror_Droid_Mimesis_Shield.h"

#include "UI_Damage.h" 
#include "Bullet_Normal_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CTerror_Droid_Mimesis_Shield::CTerror_Droid_Mimesis_Shield(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Terror_Droid_Mimesis_Shield")
{
}

CTerror_Droid_Mimesis_Shield::CTerror_Droid_Mimesis_Shield(const CTerror_Droid_Mimesis_Shield & rhs)
	: CMonster(rhs)
{
}

CTerror_Droid_Mimesis_Shield::~CTerror_Droid_Mimesis_Shield()
{
}

CTerror_Droid_Mimesis_Shield * CTerror_Droid_Mimesis_Shield::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CTerror_Droid_Mimesis_Shield* pInstance = new CTerror_Droid_Mimesis_Shield(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerror_Droid_Mimesis_Shield::Free()
{
	if (m_pEffectShield)
	{
		Safe_Release(m_pEffectShield);
	}

	CMonster::Free();
}

HRESULT CTerror_Droid_Mimesis_Shield::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Prop2";

	Set_AmmoMax(2);

	m_tUnitInfo.iAttack = 50;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 10000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 500;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CTerror_Droid_Mimesis_Shield::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	Create_Collider(&m_pSecondWeaponCol, L"Com_Collider_SecondWeapon", 0.1f);
	Create_Collider(&m_pRightClavicleCol, L"Com_Collider_RightClavicle", 0.1f);
	Create_Collider(&m_pLeftClavicleCol, L"Com_Collider_LeftClavicle", 0.1f);

	m_iOriginDeff = m_tUnitInfo.iDeffence;

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_Shield::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pSecondWeaponCol, m_szSecondWeaponName);
	Collider_SetMatrix(m_pRightClavicleCol, m_szRightClavicle);
	Collider_SetMatrix(m_pLeftClavicleCol, m_szLeftClavicle);

	Update_ExSkillFire(fTimeDelta);

	if (m_pEffectShield &&
		m_pSecondWeaponCol)
	{
		m_pEffectShield->Update_Object(fTimeDelta);
		m_pEffectShield->Set_Effect_Pos(m_pSecondWeaponCol->GetPos());
		m_pEffectShield->Set_Direction(*D3DXVec3Normalize(&m_vOutTemp, &(m_pSecondWeaponCol->GetPos() - m_pRightClavicleCol->GetPos())));
	}

	return RETURN_OK;
}

_int CTerror_Droid_Mimesis_Shield::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	if (m_pEffectShield)
	{
		m_pEffectShield->LateUpdate_Object(fTimeDelta);
	}

	return RETURN_OK;
}

void CTerror_Droid_Mimesis_Shield::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CTerror_Droid_Mimesis_Shield::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CTerror_Droid_Mimesis_Shield::OnEnable()
{
	CMonster::OnEnable();
}

void CTerror_Droid_Mimesis_Shield::OnDisable()
{
	CMonster::OnDisable();
}

void CTerror_Droid_Mimesis_Shield::OnDestroy()
{
	CMonster::OnDestroy();
}

void CTerror_Droid_Mimesis_Shield::Trigger_Attack_1()
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

	CSkill_Manager::GetInstance()->SetSkill(L"Shot_Effect.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
}

void CTerror_Droid_Mimesis_Shield::Trigger_1()
{
	if (!m_pSecondWeaponCol)
	{
		return;
	}

	static_cast<CUnit*>(this)->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_DEF, 3.f)); 
	m_tUnitInfo.iDeffence *= (_int)3.f; 

	//m_pEffectShield = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pSecondWeaponCol->GetPos(), m_pSecondWeaponCol->GetLook(), L"TerrorAutomata_Shield.effect"); 
	//assert(m_pEffectShield); 
	//m_pEffectShield->Set_Effect_Scale(_vec3(2.f, 2.f, 2.f));
}

void CTerror_Droid_Mimesis_Shield::Trigger_2()
{
	if (!m_pWeaponCollider)
	{
		return;
	}

	m_bExFireTrigger = TRUE;
}

void CTerror_Droid_Mimesis_Shield::Trigger_3()
{
	m_bExFireTrigger = FALSE;

	m_tUnitInfo.iDeffence = m_iOriginDeff;

	if (m_pEffectShield)
	{
		Safe_Release(m_pEffectShield);
	}
}

void CTerror_Droid_Mimesis_Shield::Update_ExSkillFire(const _float & fTimeDelta)
{
	if (m_bExFireTrigger)
	{
		m_fExFireDelay -= fTimeDelta;

		if (0.f >= m_fExFireDelay)
		{
			m_fExFireDelay = m_fExFireOriginDelay;

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

			_vec3	vLeftClaviclePos = m_pLeftClavicleCol->GetPos();
			vLeftClaviclePos.y += 0.05f;

			Create_AutomataBullet(*D3DXVec3Normalize(&m_vOutTemp, &(m_pWeaponCollider->GetPos() - vLeftClaviclePos)));
		}
	}
}

void CTerror_Droid_Mimesis_Shield::Create_AutomataBullet(const _vec3& vDir)
{
	CGameObject*	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
		m_pWeaponCollider->GetPos(),
		vDir,
		m_tUnitInfo,
		m_tMonsterInfo.eAtkType,
		CBullet_Normal_Monster::Move_AutomataBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}
