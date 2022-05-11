#include "Monster.h"
#include "UI_Object.h"

#include "ProtoMgr.h"
#include "CollisionManager.h" 
#include "UI_Manager.h" 

USING(Engine)

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CUnit(pGraphicDev, eObjTag, wstrUnitName)
{
	ZeroMemory(&m_tMonsterInfo, sizeof(T_MONSTER_INFO));
	m_eMyLayerTag = E_LAYERTAG::Monster;
	m_eTargetLayerTag = E_LAYERTAG::Character;
	m_eTargetObjTag = E_OBJTAG::OBJ_Player;
}

CMonster::CMonster(const CMonster & rhs)
	: CUnit(rhs)
{
}

CMonster::~CMonster()
{
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
{
	CMonster* pInstance = new CMonster(pGraphicDev, eObjTag, wstrUnitName);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	if (m_pBossWarningEffect)
	{
		m_pBossWarningEffect->Set_Dead(TRUE);
		m_pBossWarningEffect = nullptr;
	}
	if (m_pBossHpBar)
	{
		m_pBossHpBar->Set_Dead(TRUE);
		m_pBossHpBar = nullptr;
	}

	CUnit::Free();
}

//몬스터오브젝트의 준비작업
HRESULT CMonster::Ready_Object()
{
	CUnit::Ready_Object();

	m_eUnitMode = E_UnitMode::Auto;

	Set_MonsterInfo(m_tMonsterInfo);

	return S_OK;
}

//유닛과 동일
_int CMonster::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CUnit::Start_Object(fTimeDelta);

	m_bPlay = TRUE;
	Reload_Ammo();

	if (m_bNormalBoss)
	{
		CSound_Manager::GetInstance()->PlaySound_(L"SE_보스등장.wav", E_ChannelID::Channel2_1, E_SoundType::FX);

		m_pBossWarningEffect = CSkill_Manager::GetInstance()->SetSkill(L"Warning.skill", E_LAYERTAG::Effect, CCamera::m_pMainCam->Get_CamEye() + CCamera::m_pMainCam->Get_CamLookNormalize(), VEC_ZERO, 1.f);
		assert(m_pBossWarningEffect);
		m_pBossWarningEffect->Set_Scale(VEC_ONE * 2.f);
	}

	return iExit;
}

//구 충돌체를 Pelvis뼈에 붙여준다.
_int CMonster::Update_Object(const _float & fTimeDelta)
{
	//if (m_bPlay)
	//{
	Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	/*}
	else
	{
		Begin_Battle(fTimeDelta);
	}*/
	Animation_Change(fTimeDelta);

	_int iExit = CUnit::Update_Object(fTimeDelta);

	Update_BossWarningEffect(fTimeDelta);
	Check_PlayerAllDead(); 

	return iExit;
}

//유닛과 동일
_int CMonster::LateUpdate_Object(const _float & fTimeDelta)
{
	return CUnit::LateUpdate_Object(fTimeDelta);
}

//몬스터와 충돌체를 렌더링한다.
void CMonster::Render_Object(_int iPass)
{
	CUnit::Render_Object(iPass);
}

//유닛과 동일
void CMonster::Set_Constantable()
{
	CUnit::Set_Constantable();
}

//유닛과 동일
void CMonster::OnEnable()
{
	CUnit::OnEnable();
}

//유닛과 동일
void CMonster::OnDisable()
{
	CUnit::OnDisable();
}

//유닛과 동일
void CMonster::OnDestroy()
{
	CUnit::OnDestroy();
}

void CMonster::On_Collision(CGameObject * pDest)
{
	if (m_bStart)
		return;

	//if (E_LAYERTAG::AttackObj_Player == pDest->Get_LayerTag())
	//{
	//	if (E_OBJTAG::OBJ_AtkObj == pDest->Get_ObjTag())
	//	{
	//		pDest->Set_Dead(TRUE);
	//	}

	//	m_iTotalDamage += Get_Damage(static_cast<CAttackObj*>(pDest)->Get_UnitInfo(), static_cast<CAttackObj*>(pDest)->Get_AtkType());

	//	Create_HitEffect();

	//}
}

void CMonster::Load_CombatUi()
{
	CUnit::Load_CombatUi();

	// 몬스터의 경우 Elite, Boss는 엄폐 아이콘 대신 Elite, Boss 아이콘을 사용
	if (E_MONSTER_TYPE::TYPE_ELITE == m_tMonsterInfo.eMonsterType)
	{
		m_pIcon_Hide->Set_Active(TRUE);
		m_pIcon_Hide->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/엘리트아이콘.png");
	}
	if (E_MONSTER_TYPE::TYPE_BOSS == m_tMonsterInfo.eMonsterType)
	{
		m_pIcon_Hide->Set_Active(TRUE);
		m_pIcon_Hide->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/보스아이콘.png");
	}
}

_int CMonster::Set_Damage(const T_UnitInfo & tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType)
{
	_int iDamage = CUnit::Set_Damage(tEnemyInfo, eEnemyAtkType);

	if (m_bNormalBoss &&
		m_pBossHpBar)
	{
		m_pBossHpBar->Add_Damage_ForUI(-iDamage);

		if (0 >= m_tUnitInfo.iNowHp) 
		{
			m_pBossHpBar->Set_Dead(TRUE);
			m_pBossHpBar = nullptr;
		}
	}

	return iDamage;
}

//몬스터에 필요한 몬스터인포 구조체를 설정하는 함수, 충돌체 정보도 여기서 설정한다.
void CMonster::Set_MonsterInfo(T_MONSTER_INFO tMonsterInfo)
{
	m_tMonsterInfo = tMonsterInfo;
	m_pSphereCollider->SetRadius(m_tMonsterInfo.fRadius);
	m_pTransformCom->Set_Scale(m_tMonsterInfo.vScale);

	if (m_tMonsterInfo.eMonsterType == E_MONSTER_TYPE::TYPE_BOSS)
	{
		m_bNormalBoss = TRUE;
	}
}

void CMonster::Begin_Battle(_float fDeltaTime)
{
	if (Is_AnimFinished() && (m_eCurrentAction == E_ACTION::Appear || m_eCurrentAction == E_ACTION::Normal_Idle))
	{
		m_bStateLock = false;
		(*this).m_fpLockFunction = nullptr;
		return;
	}

	if (nullptr == m_pTarget)
	{
		Find_Target(m_eTargetLayerTag, m_eTargetObjTag);
	}

	if (m_pTarget)
	{
		_vec3 vTargetColPos = m_pTarget->Get_SphereCollider()->GetPos();
		vTargetColPos.y = 0.f;
		Set_Look(vTargetColPos);
	}

	if (Get_ActionIsEmpty(E_ACTION::Appear))
	{
		Set_ActionByEnum(E_ACTION::Normal_Idle);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Appear);
	}
}

//유닛과 다르게 몬스터 파일의 경로를 찾아서 설정하는 함수.
void CMonster::Ready_DMesh()
{
	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);

	if (nullptr == pMesh)
	{
		CUtility::Print_Wstring_DMode(L"프로토타입이 없어서 새로운 메쉬 프로토타입 생성");

		wstring wstrFullPath = L"../../Reference/Resource/Monster/" + m_wstrUnitName + L"/";
		wstring wstrFileName = m_wstrUnitName + L".X";

		if (E_FAIL == CProtoMgr::GetInstance()->Ready_Prototype(m_wstrUnitName,
			CDMesh::Create(m_pGraphicDev, wstrFullPath.c_str(), wstrFileName.c_str())))
		{
			// 메쉬 문제 Ready_Prototype 실패
			assert(FALSE);
		}
		pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);
		CUtility::Print_Wstring_DMode(L"메쉬 찾기 성공");
		m_mapComponent.emplace(ComTag_DMesh, pMesh);
		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		m_pDMeshCom->AddRef();
		Initialize_Trigger();
		Load_Animation();
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"메쉬 찾기 성공");
		m_mapComponent.emplace(ComTag_DMesh, pMesh);
		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		m_pDMeshCom->AddRef();
		Initialize_Trigger();
		Load_Animation();
	}
}

void CMonster::Update_Collider(const _float & fTimeDelta)
{
	CUnit::Update_Collider(fTimeDelta);

	if (m_pSphereCollider)
	{
		CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereCollider, E_LAYERTAG::AttackObj_Player);
	}
}

void CMonster::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (m_pTarget &&
		m_bCanExSkill)
	{
		if (CUtility::Distance_vec3(Get_Position(), m_pTarget->Get_Position()) <= Get_UnitRange())
		{
			m_fMonsterExSkillTimer -= fDeltaTime;

			if (0.f >= m_fMonsterExSkillTimer &&
				m_pDMeshCom->Is_AnimationsetFinish())
			{
				CWall512::GetInstance()->GetFloatValue(m_fMonsterExSkillOriginTimer - 5.f, m_fMonsterExSkillOriginTimer + 5.f);
				m_fMonsterExSkillTimer = m_fMonsterExSkillOriginTimer;

				FSM_FunctionSetting(&CUnit::ExSkill, &CUnit::Idle);
			}
		}
	}

	CUnit::Update_FSM(fDeltaTime, eTargetLayer, eObjTag);
}

void CMonster::Update_BossWarningEffect(const _float& fTimeDelta) 
{
	if (m_bNormalBoss &&
		m_pBossWarningEffect) 
	{
		m_fWarningEffectTime -= fTimeDelta; 

		if (0.f >= m_fWarningEffectTime) 
		{
			m_pBossWarningEffect->Set_Dead(TRUE); 
			m_pBossWarningEffect = nullptr; 

			return;
		}

		m_pBossWarningEffect->Set_Pos(CCamera::m_pMainCam->Get_CamAt()); 
	}
}

void CMonster::Check_PlayerAllDead()
{
	if (m_pBossHpBar) 
	{
		if (m_bNormalBoss && 
			CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player)) 
		{
			vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player); 

			_bool	bAllDead = TRUE; 

			for (_uint i = 0; i < vecObject->size(); ++i) 
			{
				if (!static_cast<CUnit*>((*vecObject)[i])->Get_UnitDead())
				{
					bAllDead = FALSE; 

					break; 
				}
			}

			if (bAllDead) 
			{
				m_pBossHpBar->Set_Dead(TRUE); 
				m_pBossHpBar = nullptr; 
			}
		}
	}
}
