#include "Unit.h"
#include "DMesh.h"
#include "Renderer.h"
#include "Layer.h"
#include "Obstacle_Combat.h"
#include "AttackObj.h"

#include "ProtoMgr.h"
#include "Renderer.h"
#include "ProtoMgr.h"
#include "LightMgr.h"
#include "FogManager.h"
#include "KeyManager.h"
#include "CharacterMgr.h"
#include "UI_Object.h"
#include "UI_Manager.h"
#include "Scene.h"
#include "Wall512.h"
#include "CollisionManager.h"
#include "UI_Damage.h"
#include "GameManager.h"
USING(Engine)

CUnit::CUnit(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CGameObject(pGraphicDev, eObjTag)
	, m_wstrUnitName(wstrUnitName)
{

}

CUnit::CUnit(const CUnit & rhs)
	: CGameObject(rhs)
{
	
}

CUnit::~CUnit()
{
}

// Mesh의 뼈를 제거하고 Release해준다.
void CUnit::Free()
{
	if (m_pParticle_CC)
	{
		Safe_Release(m_pParticle_CC);
	}

	if (m_pCombatObject)
	{
		Safe_Release(m_pCombatObject);
	}
	m_listAstarNode.clear();

	// Map에 Emplace해주지 않았기 때문에 
	// (로드되기전에 비어있는 경우가 있으므로 GameObject의 부모에서 Update를 돌리면 터진다)
	if (m_pDMeshCom)
	{
		m_pDMeshCom->Clear_Bone();
		Safe_Release(m_pDMeshCom);
	}
	m_wstrUnitName;
	
	CGameObject::Free();
}

void CUnit::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CUnit::Load(HANDLE hFile, DWORD & dwByte)
{
}

// Tool_Animation에서 저장한 애니메이션 트리거, 액션을 Load한다.
// Create에서 받아온 m_wstrUnitName를 이용해 파일을 찾는다.
void CUnit::Load_Animation()
{
	wstring TriggerPath = L"../../Reference/Data/Data_Unit/ActionTrigger/";
	TriggerPath.append(m_wstrUnitName + L".ChrAnimdat");
	HANDLE hFile = CreateFile(TriggerPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// 로드할 캐릭터 ActionTrigger 파일이 없음.
		assert(FALSE);
		return;
	}

	CUtility::Data_ReadWString(hFile, m_wstrDMeshProtoTag, dwByte);
	CUtility::Data_ReadWString(hFile, m_wstrDMeshFullPath, dwByte);
	CUtility::Print_Wstring_DMode(L"Load 성공 - ProtoTag [  " + m_wstrDMeshProtoTag + L"  ]");
	CUtility::Print_Wstring_DMode(L"Load 성공 - FullPath [  " + m_wstrDMeshFullPath + L"  ]");


	// 액션 로드
	_int iActionSize = 0;
	CUtility::Data_ReadInt(hFile, iActionSize, dwByte);
	for (_int i = 0; i < iActionSize; ++i)
	{
		T_AnimActionSys* pAction = &m_vecAnimActions[i];

		CUtility::Data_ReadWString(hFile, pAction->wstrActionKey, dwByte);

		_int iQueueSize = 0;
		CUtility::Data_ReadInt(hFile, iQueueSize, dwByte);

		_int iTrgIndex = 0;
		for (_int j = 0; j < iQueueSize; ++j)
		{
			CUtility::Data_ReadInt(hFile, iTrgIndex, dwByte);
			pAction->qAnimation.push(iTrgIndex);
		}
	}

	// 애니메이션 각 트리거 로드
	_int iAnimMaxSize = 0;
	CUtility::Data_ReadInt(hFile, iAnimMaxSize, dwByte);

	for (_int i = 0; i < iAnimMaxSize; ++i)
	{
		_int iTrgCount = 0;
		CUtility::Data_ReadInt(hFile, iTrgCount, dwByte);

		m_vecAnimTrg[i].clear();

		for (_int j = 0; j < iTrgCount; ++j)
		{
			T_AnimTrg tTrigger;
			_int iTriggerIndex = 0;
			CUtility::Data_ReadInt(hFile, iTriggerIndex, dwByte);
			tTrigger.eTrigger = (E_Trigger)iTriggerIndex;

			CUtility::Data_ReadFloat(hFile, tTrigger.fActiveDeltaTime, dwByte);
			CUtility::Data_ReadInt(hFile, tTrigger.iTriggerFrame, dwByte);
			CUtility::Data_ReadInt(hFile, tTrigger.iMaxFrame, dwByte);
			CUtility::Data_ReadInt(hFile, tTrigger.iTrgIndex, dwByte);

			m_vecAnimTrg[i].push_back(tTrigger);
		}
	}

	CloseHandle(hFile);

	// 엑션을 로드 후 Sit, Stand Shoot 엑션이 비었다면 작은, 큰 장애물에 엄폐 불가능
	if (m_vecAnimActions[(_int)E_ACTION::Sit_Shoot].qAnimation.empty())
		m_bCanHideSmall = FALSE;
	else
		m_bCanHideSmall = TRUE;

	if (m_vecAnimActions[(_int)E_ACTION::Stand_Shoot].qAnimation.empty())
		m_bCanHideBig = FALSE;
	else
		m_bCanHideBig = TRUE;

	// 점프 액션이 비었다면 장애물 점프해서 지나가기 불가능
	if (m_vecAnimActions[(_int)E_ACTION::Jump].qAnimation.empty())
		m_bJumpObstacle = FALSE;
	else
		m_bJumpObstacle = TRUE;

	FSM_FunctionSetting(&CUnit::Begin_Battle, &CUnit::Idle);
	Animation_Change(0.f);

	Load_CombatUi();

}

// Abstract 파일이기 때문에 Add_Component가 따로없다.
HRESULT CUnit::Ready_Object()
{
	CGameObject::Ready_Object();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	Ready_DMesh();

	Create_Collider(&m_pSphereCollider, ComTag_Collider, 0.3f);
	Create_Collider(&m_pWeaponCollider, L"Weapon_Collider", 0.1f);

	Load_CombatUi();

	m_pParticle_CC = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"CombatStatus_CC.particle");

	return S_OK;
}

// 부모와 동일
_int CUnit::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);
	return iExit;
}

// m_pDMeshCom가 있으면 Update_AnimTrigger를 실행하고 없으면 Find_DMesh를 실행한다.
// m_qTrg 트리거 큐를 순회하면서 제한시간에 도달한 트리거를 실행한다.
_int CUnit::Update_Object(const _float & fTimeDelta)
{
	Update_CombatStatus(fTimeDelta);

	m_fJumpCoolTime -= fTimeDelta;
	if (E_UnitMode::None == m_eUnitMode)
	{
		assert(FALSE);
	}

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pDMeshCom)
	{
		_bool bRender = TRUE;
		if (m_pCombatObject)
		{
			m_pCombatObject->Update_Object(fTimeDelta);
		}
		if (E_ACTION::Vital_Death != m_eCurrentAction)
		{
			if (m_bAnimPlay)
			{
				m_pDMeshCom->Play_Animation(fTimeDelta);
			}
		}
		else
		{
			if (FALSE == Is_AnimFinished())
			{
				m_pDMeshCom->Play_Animation(fTimeDelta);
			}
			else
			{
				m_fDeadShaderFilter += fTimeDelta * 1.f;
			}
		}

		if (1.f > m_fDeadShaderFilter)
		{
			CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
			CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
		}
	}
	else
	{
		Find_DMesh();
	}

	Update_Trigger(fTimeDelta);
	Update_Collider(fTimeDelta);

	if (Get_IsCC())
	{
		FSM_FunctionSetting(&CUnit::Vital_Panic, nullptr);
	}

	m_fCallSignTimer -= fTimeDelta * CWall512::GetInstance()->GetFloatValue(0.8f, 1.f);

	if (m_pParticle_CC)
	{
		if (m_fpLockFunction == &CUnit::Vital_Panic)
		{
			m_pParticle_CC->Set_Active(TRUE);
			m_pParticle_CC->Set_Pos(m_pSphereCollider->GetPos());
			m_pParticle_CC->Update_Object(fTimeDelta);
			m_pParticle_CC->LateUpdate_Object(fTimeDelta);
			CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_EFFECT, m_pParticle_CC);
		}
		else
		{
			m_pParticle_CC->Set_Active(FALSE);
		}
	}

	return iExit;
}

// 부모와 동일
_int CUnit::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (m_pSphereCollider)
	{
		m_pSphereCollider->ClearColList();
	}
	
	if (m_bPlay)
	{
		if (!(m_eCurrentAction == E_ACTION::Victory_Start || m_eCurrentAction == E_ACTION::Victory_End))
		{
			_matrix matView;
			_matrix matProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
			if (CMyMath::Get_InCamera(Get_Position(), matView, matProj))
			{

				if (m_pCombatObject && FALSE == Get_UnitDead())
				{
					m_pCombatObject->LateUpdate_Object(fTimeDelta);

					_vec3 vWorldPos = m_pSphereCollider->GetPos();
					vWorldPos.y += m_pSphereCollider->GetRadius() * 1.f;

					_vec2 vUiPos = CUtility::World2Ui(vWorldPos);
					vUiPos.y += 80.f;

					m_pCombatObject->Set_Ui_PosX(_int(vUiPos.x));
					m_pCombatObject->Set_Ui_PosY(_int(vUiPos.y));
					_float fHealthRatio = CUtility::Lerp_float(0.12f, 0.88f, (m_tUnitInfo.iNowHp / (_float)m_tUnitInfo.iMaxHp));

					m_pHealthBar->Set_Mask(_vec2(0.f, 0.f), _vec2(fHealthRatio, 1.f));
				}
			}
		}
	}

	return iExit;
}

// 렌더링 RENDER_NONALPHA
void CUnit::Render_Object(_int iPass)
{
	if (nullptr == m_pDMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	m_pDMeshCom->Render_Meshes(m_pShaderCom);

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
		m_pWeaponCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CUnit::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"유닛 입니다.");
}

void CUnit::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	//0204mat
	//_matrix	matView, matProj;
	//
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	//0204mat
	//// 그림자용
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망시 필터
	pEffect->SetFloat("g_fFilter", m_fDeadShaderFilter);	
}

// Abstract 기능없음
void CUnit::OnEnable()
{
	CGameObject::Update_Object(0.f);
}

// Abstract 기능없음
void CUnit::OnDisable()
{
}

// Abstract 기능없음
void CUnit::OnDestroy()
{
}

void CUnit::On_Collision(CGameObject * pDest)
{
	if (m_bStart)
		return;

	if (E_LAYERTAG::Obstacle == pDest->Get_LayerTag() && m_pLastJumpObstacle != pDest)
	{
		_bool bInside = CUtility::Distance_vec3(Get_Position(), pDest->Get_Position()) - static_cast<CObstacle*>(pDest)->Get_Radius() < 0 ? TRUE : FALSE;
		if (pDest->Get_Name().substr(1, 1) == L"S" && m_bJumpObstacle && nullptr == m_fpLockFunction && FALSE == Get_Dead() && bInside)
		{
			if (m_fJumpCoolTime < 0.f)
			{
				m_pLastJumpObstacle = static_cast<CObstacle*>(pDest);
				m_eCurrentAction = E_ACTION::Jump;
				m_fJumpCoolTime = 2.f;
				Animation_Change(0.f);
			}
		}
	}
	else if (E_LAYERTAG::AttackObj_Monster == pDest->Get_LayerTag() || 
			 E_LAYERTAG::AttackObj_Player == pDest->Get_LayerTag())
	{
		Create_HitEffect();
	}
}

void CUnit::Load_CombatUi()
{
	if (m_pCombatObject)
		return;
	
	m_pCombatObject = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Combat_Character.uidat");
	m_pHealthBar = m_pCombatObject->Get_ChildByIndex(0)->Get_ChildByIndex(0);
	m_pHealthBar->Set_ResetMask(FALSE);
	m_pIcon_Hide = m_pCombatObject->Get_ChildByIndex(1);
	m_pIcon_Buff[0] = m_pCombatObject->Get_ChildByIndex(2);
	m_pIcon_Buff[1] = m_pCombatObject->Get_ChildByIndex(3);
	m_pIcon_Buff[2] = m_pCombatObject->Get_ChildByIndex(4);
	m_pIcon_Buff[3] = m_pCombatObject->Get_ChildByIndex(5);
	m_pIcon_Debuff[0] = m_pCombatObject->Get_ChildByIndex(6);
	m_pIcon_Debuff[1] = m_pCombatObject->Get_ChildByIndex(7);
	m_pIcon_Debuff[2] = m_pCombatObject->Get_ChildByIndex(8);
	m_pIcon_Debuff[3] = m_pCombatObject->Get_ChildByIndex(9);
	m_pCombat_Reload = m_pCombatObject->Get_ChildByIndex(10);

	if ((m_bCanHideBig || m_bCanHideSmall) && m_pCombatObject)
		m_pIcon_Hide->Set_Active(TRUE);
	else
		m_pIcon_Hide->Set_Active(FALSE);
}

void CUnit::Hit_Target()
{
	if (m_pTarget)
	{
		Create_HitEffect();

		m_iTotalDamage += m_pTarget->Set_Damage(m_tUnitInfo, Get_AttackType());		
	}
}

void CUnit::Hit_Target_Enum(E_UNIT_HIT_TYPE eUnitHitType)
{
	E_UNIT_HIT_TYPE eTemp = m_eUnitHitType;

	m_eUnitHitType = eUnitHitType;

	Hit_Target();

	m_eUnitHitType = eTemp;
}

void CUnit::Hit_Target_FanToCircle(CGameObject * pSrc, CGameObject * pDest)
{
	if (FALSE == static_cast<CUnit*>(pDest)->Get_UnitDead())
	{
		static_cast<CUnit*>(pSrc)->Create_HitEffect();

		static_cast<CUnit*>(pSrc)->m_iTotalDamage += static_cast<CUnit*>(pDest)->Set_Damage(static_cast<CUnit*>(pSrc)->m_tUnitInfo, static_cast<CUnit*>(pSrc)->Get_AttackType());
	}
}

_bool CUnit::Find_Target(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag)
{
	CLayer* pTargetLayer = CScene::Get_MainScene()->Get_Layer(eFindLayerTag);

	vector<CGameObject*>* pVecMonster = pTargetLayer->Get_vecGameObject(eFindObjTag);

	if (nullptr == pVecMonster || pVecMonster->empty())
	{
		return FALSE;
	}
	CGameObject*	pTarget = nullptr;

	_float fMinDist = 99999.f;

	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (FALSE == (*pVecMonster)[i]->Get_Active())
			continue;

		if (static_cast<CUnit*>((*pVecMonster)[i])->Get_UnitDead())
			continue;

		_float fCalcDist = CUtility::Distance_vec3(Get_Position(), (*pVecMonster)[i]->Get_Position());

		if (fCalcDist < fMinDist)
		{
			fMinDist = fCalcDist;
			pTarget = (*pVecMonster)[i];
		}
	}

	if (pTarget)
	{
		m_pTarget = static_cast<CUnit*>(pTarget);
	}

	return TRUE;
}

_bool CUnit::Find_Target_InRange(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag)
{
	CLayer* pTargetLayer = CScene::Get_MainScene()->Get_Layer(eFindLayerTag);

	vector<CGameObject*>* pVecMonster = pTargetLayer->Get_vecGameObject(eFindObjTag);

	if (nullptr == pVecMonster || pVecMonster->empty())
	{
		return FALSE;
	}
	vector<CUnit*> vecCanTarget;
	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (FALSE == (*pVecMonster)[i]->Get_Active())
			continue;

		if (static_cast<CUnit*>((*pVecMonster)[i])->Get_UnitDead())
			continue;

		_float fCalcDist = CUtility::Distance_vec3(Get_Position(), (*pVecMonster)[i]->Get_Position());
	
		if (fCalcDist < Get_UnitRange())
		{
			vecCanTarget.push_back(static_cast<CUnit*>((*pVecMonster)[i]));
		}
	}

	if (vecCanTarget.empty())
	{
		return FALSE;
	}

	_int iRandEnemy = CWall512::GetInstance()->GetValue((_int)vecCanTarget.size());

	m_pTarget = vecCanTarget[iRandEnemy];

	return TRUE;
}

void CUnit::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;

		if (m_eCurrentAction == E_ACTION::Normal_Shoot || m_eCurrentAction == E_ACTION::Sit_Shoot || m_eCurrentAction == E_ACTION::Stand_Shoot)
		{
			if (SoundEmpty != m_tUnitSound.pairShoot.first)
			{
				CSound_Manager::GetInstance()->Add_Sound_NotOverlap(m_tUnitSound.pairShoot.first, m_tUnitSound.pairShoot.second);
			}
		}

		if (m_eCurrentAction == E_ACTION::Normal_Reload || m_eCurrentAction == E_ACTION::Sit_Reload || m_eCurrentAction == E_ACTION::Stand_Reload)
		{
			if (SoundEmpty != m_tUnitSound.pairReload.first)
			{
				CSound_Manager::GetInstance()->Add_Sound_NotOverlap(m_tUnitSound.pairReload.first, m_tUnitSound.pairReload.second);
			}
		}
	}
}

void CUnit::CutHideObstacle()
{
	m_pHideObstacle = nullptr;
}

void CUnit::Reload_Ammo()
{
	m_iAmmo = m_iAmmo_Max;
}


_vec3 CUnit::Get_WeaponPos(_float fAddRight, _float fAddUp, _float fAddLook)
{
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirRight * fAddRight;
	vWeaponPos += vDirUp	* fAddUp;
	vWeaponPos += vDirLook	* fAddLook;

	return vWeaponPos;
}

_vec3 CUnit::Get_WeaponRot()
{
	return m_pWeaponCollider->GetRot();
}

_bool CUnit::Get_IsCC()
{
	_bool bStillCC = FALSE;
	for (_int i = (_int)E_Combat_Status::CC_Blind; i < (_int)E_Combat_Status::CC_Stunned; ++i)
	{
		if (m_iArrNowCombatStatus[i] != 0)
		{
			bStillCC = TRUE;
		}
	}
	return bStillCC;
}

_int CUnit::Set_Damage(const T_UnitInfo & tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType)
{
	if (m_bInvn)
		return 0;

	// 회피 체크 명중률(iHitRatio) + 추가 명중률(쏘는사람 안전성 - 맞는사람 회피율) * 0.01f
	_float fEvaRand = CWall512::GetInstance()->GetFloatValue(0.f, 100.f);
	_bool bEvasion = FALSE;
	_int iAddAccuracy = tEnemyInfo.iStability - m_tUnitInfo.iEvasion;
	if (fEvaRand > tEnemyInfo.iHitRatio + (iAddAccuracy * 0.01f))
	{
		bEvasion = TRUE;
	}

	// 방어력만큼 데미지 감소 
	_int iDamage = tEnemyInfo.iAttack - m_tUnitInfo.iDeffence;
	if (iDamage < 1)
		iDamage = 1;

	// 상성에 따라서 데미지를 변경 (Resist 0.5배  Weak 2배)
	E_NumberFontType eFontType = CUtility::Calc_Damage(eEnemyAtkType, Get_DefenseType(), iDamage);

	// 크리티컬 체크 (iCritical * 0.1f의 확률로 tEnemyInfo.iCriticalDamage * 0.01f의 데미지 증가)
	_float fCriRand = CWall512::GetInstance()->GetFloatValue(0.f, 100.f);
	_bool bCritical = FALSE;
	if (tEnemyInfo.iCritical * 0.1f > fCriRand)
	{
		bCritical = TRUE;
		iDamage = (_int)(iDamage * (tEnemyInfo.iCriticalDamage * 0.01f));
	}

	m_tUnitInfo.iNowHp += iDamage * -1;

	if (FALSE == bEvasion)
	{
		// 데미지 Rand로
		if (0 == iDamage)
		{
			iDamage = 1; 
		}
		iDamage = CWall512::GetInstance()->GetValue((_int)(0.9f * iDamage), (_int)(1.1f * iDamage));
		Create_DamageFont(eFontType, iDamage, bCritical);
	}
	else		// 회피함.
	{
		Create_DamageFont(E_NumberFontType::Resist, 0, FALSE, bEvasion);
	}

	// 최종 데미지 계산
	m_iTotalDamage += iDamage; 

	return iDamage; 
}

_int CUnit::Set_Damage(const T_UnitInfo & tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType, _float fRatio)
{
	if (m_bInvn)
		return 0;

	_float fDamage = _float(tEnemyInfo.iAttack) * fRatio;

	// 회피 체크 명중률(iHitRatio) + 추가 명중률(쏘는사람 안전성 - 맞는사람 회피율) * 0.01f
	_float fEvaRand = CWall512::GetInstance()->GetFloatValue(0.f, 100.f);
	_bool bEvasion = FALSE;
	_int iAddAccuracy = tEnemyInfo.iStability - m_tUnitInfo.iEvasion;
	if (fEvaRand > tEnemyInfo.iHitRatio + (iAddAccuracy * 0.01f))
	{
		bEvasion = TRUE;
	}

	// 방어력만큼 데미지 감소 
	_int iDamage = _int(fDamage) - m_tUnitInfo.iDeffence;
	if (iDamage < 1)
		iDamage = 1;

	// 상성에 따라서 데미지를 변경 (Resist 0.5배  Weak 2배)
	E_NumberFontType eFontType = CUtility::Calc_Damage(eEnemyAtkType, Get_DefenseType(), iDamage);

	// 크리티컬 체크 (iCritical * 0.1f의 확률로 tEnemyInfo.iCriticalDamage * 0.01f의 데미지 증가)
	_float fCriRand = CWall512::GetInstance()->GetFloatValue(0.f, 100.f);
	_bool bCritical = FALSE;
	if (tEnemyInfo.iCritical * 0.1f > fCriRand)
	{
		bCritical = TRUE;
		iDamage = (_int)(iDamage * (tEnemyInfo.iCriticalDamage * 0.01f));
	}

	m_tUnitInfo.iNowHp += iDamage * -1;

	if (FALSE == bEvasion)
	{
		// 데미지 Rand로
		if (0 == iDamage)
		{
			iDamage = 1;
		}
		iDamage = CWall512::GetInstance()->GetValue((_int)(0.9f * iDamage), (_int)(1.1f * iDamage));
		Create_DamageFont(eFontType, iDamage, bCritical);
	}
	else		// 회피함.
	{
		Create_DamageFont(E_NumberFontType::Resist, 0, FALSE, bEvasion);
	}

	// 최종 데미지 계산
	m_iTotalDamage += iDamage;

	return iDamage;
}

void CUnit::Set_ActionByEnum(E_ACTION eAction)
{
	m_eCurrentAction = eAction;
	switch (eAction)
	{
	case Engine::E_ACTION::Normal_Idle:
		Set_ActionByName(L"Normal_Idle");
		break;
	case Engine::E_ACTION::Normal_Shoot_Start:
		Set_ActionByName(L"Normal_Shoot_Start");
		break;
	case Engine::E_ACTION::Normal_Shoot:
		Set_ActionByName(L"Normal_Shoot");
		break;
	case Engine::E_ACTION::Normal_Delay:
		Set_ActionByName(L"Normal_Delay");
		break;
	case Engine::E_ACTION::Normal_Shoot_End:
		Set_ActionByName(L"Normal_Shoot_End");
		break;
	case Engine::E_ACTION::Normal_Reload:
		Set_ActionByName(L"Normal_Reload");
		break;
	case Engine::E_ACTION::Normal_CallSign:
		Set_ActionByName(L"Normal_CallSign");
		break;
	case Engine::E_ACTION::Normal_Ready_Start:
		Set_ActionByName(L"Normal_Ready_Start");
		break;
	case Engine::E_ACTION::Normal_Ready_End:
		Set_ActionByName(L"Normal_Ready_End");
		break;
	case Engine::E_ACTION::Move_End_Normal:
		Set_ActionByName(L"Move_End_Normal");
		break;
	case Engine::E_ACTION::Stand_Idle:
		Set_ActionByName(L"Stand_Idle");
		break;
	case Engine::E_ACTION::Stand_Shoot_Start:
		Set_ActionByName(L"Stand_Shoot_Start");
		break;
	case Engine::E_ACTION::Stand_Shoot:
		Set_ActionByName(L"Stand_Shoot");
		break;
	case Engine::E_ACTION::Stand_Delay:
		Set_ActionByName(L"Stand_Delay");
		break;
	case Engine::E_ACTION::Stand_Shoot_End:
		Set_ActionByName(L"Stand_Shoot_End");
		break;
	case Engine::E_ACTION::Stand_Reload:
		Set_ActionByName(L"Stand_Reload");
		break;
	case Engine::E_ACTION::Stand_CallSign:
		Set_ActionByName(L"Stand_CallSign");
		break;
	case Engine::E_ACTION::Stand_Ready_Start:
		Set_ActionByName(L"Stand_Ready_Start");
		break;
	case Engine::E_ACTION::Stand_Ready_End:
		Set_ActionByName(L"Stand_Ready_End");
		break;
	case Engine::E_ACTION::Move_End_Stand:
		Set_ActionByName(L"Move_End_Stand");
		break;
	case Engine::E_ACTION::Sit_Idle:
		Set_ActionByName(L"Sit_Idle");
		break;
	case Engine::E_ACTION::Sit_Shoot_Start:
		Set_ActionByName(L"Sit_Shoot_Start");
		break;
	case Engine::E_ACTION::Sit_Shoot:
		Set_ActionByName(L"Sit_Shoot");
		break;
	case Engine::E_ACTION::Sit_Delay:
		Set_ActionByName(L"Sit_Delay");
		break;
	case Engine::E_ACTION::Sit_Shoot_End:
		Set_ActionByName(L"Sit_Shoot_End");
		break;
	case Engine::E_ACTION::Sit_Reload:
		Set_ActionByName(L"Sit_Reload");
		break;
	case Engine::E_ACTION::Sit_CallSign:
		Set_ActionByName(L"Sit_CallSign");
		break;
	case Engine::E_ACTION::Sit_Ready_Start:
		Set_ActionByName(L"Sit_Ready_Start");
		break;
	case Engine::E_ACTION::Sit_Ready_End:
		Set_ActionByName(L"Sit_Ready_End");
		break;
	case Engine::E_ACTION::Move_End_Sit:
		Set_ActionByName(L"Move_End_Sit");
		break;
	case Engine::E_ACTION::Move_Ing:
		Set_ActionByName(L"Move_Ing");
		break;
	case Engine::E_ACTION::Move_CallSign:
		Set_ActionByName(L"Move_CallSign");
		break;
	case Engine::E_ACTION::Jump:
		Set_ActionByName(L"Jump");
		break;
	case Engine::E_ACTION::Vital_Death:
		Set_ActionByName(L"Vital_Death");
		break;
	case Engine::E_ACTION::Vital_Dying_Ing:
		Set_ActionByName(L"Vital_Dying_Ing");
		break;
	case Engine::E_ACTION::Vital_Retreat:
		Set_ActionByName(L"Vital_Retreat");
		break;
	case Engine::E_ACTION::Vital_Panic:
		Set_ActionByName(L"Vital_Panic");
		break;
	case Engine::E_ACTION::Formation_Idle:
		Set_ActionByName(L"Formation_Idle");
		break;
	case Engine::E_ACTION::Formation_Pickup:
		Set_ActionByName(L"Formation_Pickup");
		break;
	case Engine::E_ACTION::ExSkill_CutIn:
		Set_ActionByName(L"ExSkill_CutIn");
		break;
	case Engine::E_ACTION::ExSkill:
		Set_ActionByName(L"ExSkill");
		break;
	case Engine::E_ACTION::Victory_Start:
		Set_ActionByName(L"Victory_Start");
		break;
	case Engine::E_ACTION::Victory_End:
		Set_ActionByName(L"Victory_End");
		break;
	case Engine::E_ACTION::Cafe_Idle:
		Set_ActionByName(L"Cafe_Idle");
		break;
	case Engine::E_ACTION::Cafe_Walk:
		Set_ActionByName(L"Cafe_Walk");
		break;
	case Engine::E_ACTION::Cafe_Reaction:
		Set_ActionByName(L"Cafe_Reaction");
		break;
	case Engine::E_ACTION::Public01:
		Set_ActionByName(L"Public01");
		break;

	case Engine::E_ACTION::Temp1:
		Set_ActionByName(L"Temp1");
		break;
	case Engine::E_ACTION::Temp2:
		Set_ActionByName(L"Temp2");
		break;
	case Engine::E_ACTION::Temp3:
		Set_ActionByName(L"Temp3");
		break;
	case Engine::E_ACTION::Temp4:
		Set_ActionByName(L"Temp4");
		break;
	case Engine::E_ACTION::Temp5:
		Set_ActionByName(L"Temp5");
		break;
	case Engine::E_ACTION::Appear:
		Set_ActionByName(L"Appear");
		break;
	case Engine::E_ACTION::ACTION_END:
		break;
	default:
		LMSG_BOX(L"E_ANIM 범위 초과");
		break;
	}
}

void CUnit::Set_HideObstacle(CGameObject * pObj)
{
	m_pHideObstacle = pObj;
}

void CUnit::Set_AmmoMax(_int iAmmoMax)
{
	m_iAmmo_Max = iAmmoMax;
	m_iAmmo = iAmmoMax;
}

void CUnit::Set_BeginBattle()
{
	FSM_FunctionSetting(&CUnit::Begin_Battle, &CUnit::Idle);
}

void CUnit::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (nullptr == m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
	}
	else
	{
		if (FALSE == CGameManager::GetInstance()->Get_BattleIng())
		{
			FSM_FunctionSetting(&CUnit::Victory, nullptr);
		}

		m_fFindObstacleTime += fDeltaTime;
		if (m_fFindObstacleCool <= m_fFindObstacleTime)
		{
			m_fFindObstacleTime = 0.f;

			if (m_pHideObstacle)
			{
				static_cast<CObstacle_Combat*>(m_pHideObstacle)->Check_Obstacle();
			}
			else
			{
				FindObstacle();
			}
		}
	}
	m_fFindTargetTime += fDeltaTime;



	if (FALSE == m_bStateLock)
	{
		if (nullptr == m_pTarget)
		{
			Find_Target(eTargetLayer, eObjTag);
			if (nullptr == m_pTarget)
				return;
		}

		if (m_pHideObstacle)
		{
			_vec3 vHidePos = static_cast<CObstacle_Combat*>(m_pHideObstacle)->Get_HideSpotPos(this);
			m_listAstarNode.clear();
			Find_Astar(Get_Position(), vHidePos, m_bJumpObstacle);
			FSM_FunctionSetting(&CUnit::Hiding, nullptr);
		}
		else
		{
			_float fDistToTarget = CUtility::Distance_vec3(Get_Position(), m_pTarget->Get_Position());
			if (fDistToTarget > Get_UnitRange())
			{
				if (m_fFindTargetCool <= m_fFindTargetTime)
				{
					Find_Target(eTargetLayer, eObjTag);
					m_fFindTargetTime = 0.f;
				}
				Tracking(fDeltaTime);
			}
			else
			{
				if (E_ACTION::Jump != m_eCurrentAction)
				{
					m_listAstarNode.clear();
					FSM_FunctionSetting(&CUnit::Move_End_Stance, nullptr);
				}
				else if(Is_AnimFinished())
				{
					m_eCurrentAction = E_ACTION::Move_Ing;
				}
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
		if (m_fpLockFunction != &CUnit::ExSkill)
		{
			Look_Target_Slowly(fDeltaTime);
		}
	}
	MoveTo_Astar(fDeltaTime, m_fMoveSpeed, 1.f);
}

void CUnit::Reset()
{
	FSM_FunctionRelease();
	
	m_listAstarNode.clear();
	m_vecCombatStatus.clear();
	m_pTarget			= nullptr;
	m_ePrevAction		= E_ACTION::ACTION_END;
	m_eCurrentAction	= E_ACTION::ACTION_END;
	m_eHide				= E_HIDE::HIDE_NONE;
	m_bDead				= FALSE;
	m_pHideObstacle		= nullptr;
	m_bAssemble			= FALSE;
	m_bInvn				= FALSE;
	m_bPlay				= FALSE;
	m_fDeadShaderFilter = 0.f;
	

	Set_Scale(_vec3(2.5f, 2.5f, 2.5f));

	Reset_HP();
	
}

void CUnit::Use_ExSkill()
{
	FSM_FunctionSetting(&CUnit::ExSkill_CurIn, nullptr);
}

void CUnit::KnockBack(const _vec3 & vTargetPos, const _float& fTimeDelta)
{
	if (m_bKnockBack)
	{
		_float	fDistance = CUtility::Distance_vec3(Get_Position(), vTargetPos);
		_vec3	vDir = vTargetPos - Get_Position();
		D3DXVec3Normalize(&vDir, &vDir);

		Move_Pos(vDir, fDistance * 1.5f, fTimeDelta);

		if (0.1f > fDistance)
		{
			m_bKnockBack = FALSE; 
			m_vKnockBackPos = VEC_ZERO; 
		}
	}
}

void CUnit::FSM_FunctionRelease()
{
	m_bStateLock = FALSE;
	(*this).m_fpLockFunction = nullptr;
}

void CUnit::FSM_FunctionSetting(void(CUnit::*pCol)(_float), void(CUnit::*pDestFunc)(_float))
{
	m_bStateLock = TRUE;
	(*this).m_fpLockFunction = pCol;
	(*this).m_fpDestFunction = pDestFunc;

}

void CUnit::Clear_Astar()
{
	m_listAstarNode.clear();
}

void CUnit::CC_Taunt(_float fTime, CUnit* pTauntUnit)
{
	FSM_FunctionRelease();
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
	m_pTarget = pTauntUnit;
	if (Get_UnitRange() < CUtility::Distance_vec3(Get_Position(), m_pTarget->Get_Position()))
	{
		Find_Astar(Get_Position(), m_pTarget->Get_Position(), m_bJumpObstacle);
	}

	CEffectObject* pTauntEffect = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), VEC_ZERO, L"CombatStatus_Taunt.effect");
	pTauntEffect->Get_Effect_Info().fLifeTime = fTime;
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pTauntEffect);
}

void CUnit::CC_Fear(_float fTime)
{
	Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Fear, fTime));

	CEffectObject* pTauntEffect = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), VEC_ZERO, L"CombatStatus_Panic.effect");
	pTauntEffect->Get_Effect_Info().fLifeTime = fTime;
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pTauntEffect);
}

void CUnit::Tracking(_float fDeltaTime)
{
	if (nullptr == m_pTarget)
	{
		return;
	}
	if (m_listAstarNode.empty())
	{
		if (m_bAssemble)
		{
			FSM_FunctionSetting(&CUnit::Idle, nullptr);
			return;
		}
		_float fMyRange = Get_UnitRange();

		_vec3 vDir = m_pTarget->Get_Position() - Get_Position();
		D3DXVec3Normalize(&vDir, &vDir);

		_vec3 vDist = m_pTarget->Get_Position() - fMyRange * vDir * 0.799f;
		vDist.x += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
		vDist.z += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
		Find_Astar(Get_Position(), vDist, m_bJumpObstacle);
	}
	if (E_ACTION::Jump == m_eCurrentAction && Is_AnimFinished())
	{
		m_eCurrentAction = E_ACTION::Move_Ing;
	}
	else if (E_ACTION::Jump != m_eCurrentAction)
	{
		m_eCurrentAction = E_ACTION::Move_Ing;
	}
}

void CUnit::Idle(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Idle;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Idle;		break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Idle;			break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Idle;			break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Formation_Idle;		break;
	default:							eMainAction = E_ACTION::Normal_Idle;		break;
	}

	if (m_bAssemble)
	{
		m_eCurrentAction = eMainAction;
	}

	_float fDistance = 10000.f;
	if (m_pTarget)
	{
		fDistance = CMyMath::Distance_vec3(m_pTarget->Get_Position(), Get_Position());
	}

	if (m_iAmmo <= 0)
	{
		FSM_FunctionSetting(&CUnit::Reload, nullptr);
		return;
	}
	else if (m_pHideObstacle && m_eHide == E_HIDE::HIDE_NONE)
	{
		Find_Astar(Get_Position(), static_cast<CObstacle_Combat*>(m_pHideObstacle)->Get_HideSpotPos(this), m_bJumpObstacle);
		FSM_FunctionSetting(&CUnit::Ready_End, &CUnit::Hiding);
		return;
	}
	else if (m_fpDestFunction == nullptr || m_fpDestFunction == &CUnit::Ready_End || m_pTarget == nullptr || m_pTarget->Get_UnitDead())
	{
		if (m_fCallSignTimer <= 0.f)
		{
			FSM_FunctionSetting(&CUnit::CallSign, nullptr);
		}
		else
		{
			FSM_FunctionSetting(&CUnit::Ready_End, nullptr);
		}
		return;
	}
	else if (m_pTarget && FALSE == m_pTarget->Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Shoot_Start, nullptr);
		return;
	}
	else if (m_pTarget && fDistance >= Get_UnitRange() && FALSE == m_bAssemble)
	{
		FSM_FunctionSetting(&CUnit::Ready_End, nullptr);
		return;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		if (m_pTarget == nullptr || m_pTarget->Get_UnitDead())
		{
			Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);
		}
	}
	else
	{
		m_eCurrentAction = eMainAction;
	}

}

// 자세에 맞는 Ready_Start 액션이 있을때만 이곳에 들어온다.
void CUnit::Ready_Shoot_Start(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Ready_Start;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Ready_Start;		break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Ready_Start;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Ready_Start;		break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Ready_Start;		break;
	default:							eMainAction = E_ACTION::Normal_Ready_Start;		break;
	}

	if (m_vecAnimActions[(_int)eMainAction].qAnimation.empty())
	{
		FSM_FunctionSetting(&CUnit::Idle, &CUnit::Shoot);
		return;
	}
	else if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		FSM_FunctionSetting(&CUnit::Idle, &CUnit::Shoot);
		return;
	}

	m_eCurrentAction = eMainAction;
}

void CUnit::Shoot_Start(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Shoot_Start;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Shoot_Start;		break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Shoot_Start;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Shoot_Start;		break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Shoot_Start;		break;
	default:							eMainAction = E_ACTION::Normal_Shoot_Start;		break;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		FSM_FunctionSetting(&CUnit::Shoot, nullptr);
		return;
	}

	m_eCurrentAction = eMainAction;
}

void CUnit::Shoot(_float fDeltaTime)
{
	if (0 >= m_iAmmo && Is_AnimFinished())
	{
		FSM_FunctionSetting(&CUnit::Shoot_End, &CUnit::Reload);
		return;
	}

	if (nullptr == m_pTarget)
	{
		return;
	}

	if (m_pTarget->Get_UnitDead())
	{
		m_pTarget = nullptr;
		FSM_FunctionSetting(&CUnit::Shoot_End, nullptr);
		return;
	}

	_float fDistance = CMyMath::Distance_vec3(m_pTarget->Get_Position(), Get_Position());

	if (fDistance >= Get_UnitRange())
	{
		Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

		if (nullptr == m_pTarget)
		{
			FSM_FunctionSetting(&CUnit::Shoot_End, &CUnit::Ready_End);
		}

		fDistance = CMyMath::Distance_vec3(m_pTarget->Get_Position(), Get_Position());
		if (fDistance >= Get_UnitRange())
		{
			FSM_FunctionSetting(&CUnit::Shoot_End, &CUnit::Ready_End);
		}
		return;
	}

	if (m_pTarget->Get_SphereCollider())
	{
		_vec3 vTargetColPos = m_pTarget->Get_SphereCollider()->GetPos();
		vTargetColPos.y = 0.f;
	}

	if (Is_AnimFinished() && !(m_eCurrentAction == E_ACTION::Normal_Shoot_Start || m_eCurrentAction == E_ACTION::Sit_Shoot_Start || m_eCurrentAction == E_ACTION::Stand_Shoot_Start))
	{
		E_ACTION eMainAction = E_ACTION::Normal_Delay;
		if ((m_eCurrentAction == E_ACTION::Normal_Delay || m_eCurrentAction == E_ACTION::Sit_Delay || m_eCurrentAction == E_ACTION::Stand_Delay))
		{
			eMainAction = E_ACTION::Normal_Shoot;
			switch (m_eHide)
			{
			case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Shoot;			break;
			case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Shoot;				break;
			case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Shoot;			break;
			case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Shoot;			break;
			default:							eMainAction = E_ACTION::Normal_Shoot;			break;
			}

			m_iAmmo -= 1;
		}
		else if (m_eCurrentAction == E_ACTION::Normal_Shoot || m_eCurrentAction == E_ACTION::Sit_Shoot || m_eCurrentAction == E_ACTION::Stand_Shoot)
		{
			switch (m_eHide)
			{
			case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Delay;			break;
			case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Delay;				break;
			case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Delay;			break;
			case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Delay;			break;
			default:							eMainAction = E_ACTION::Normal_Delay;			break;
			}
		}

		m_eCurrentAction = eMainAction;
	}
	else
	{
		if (m_eCurrentAction != E_ACTION::Normal_Delay && m_eCurrentAction != E_ACTION::Sit_Delay && m_eCurrentAction != E_ACTION::Stand_Delay &&
			m_eCurrentAction != E_ACTION::Normal_Shoot && m_eCurrentAction != E_ACTION::Sit_Shoot && m_eCurrentAction != E_ACTION::Stand_Shoot)
		{
			E_ACTION eMainAction = E_ACTION::Normal_Delay;
			switch (m_eHide)
			{
			case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Shoot;			break;
			case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Shoot;				break;
			case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Shoot;			break;
			case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Shoot;			break;
			default:							eMainAction = E_ACTION::Normal_Shoot;			break;
			}
			m_eCurrentAction = eMainAction;
		}
	}
}

void CUnit::Shoot_End(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Shoot_End;
	E_ACTION eEndAction = E_ACTION::Normal_Ready_End;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Shoot_End;	eEndAction = E_ACTION::Normal_Ready_End;	break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Shoot_End;		eEndAction = E_ACTION::Sit_Ready_End;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Shoot_End;	eEndAction = E_ACTION::Stand_Ready_End;		break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Shoot_End;	eEndAction = E_ACTION::Normal_Ready_End;	break;
	default:							eMainAction = E_ACTION::Normal_Shoot_End;	eEndAction = E_ACTION::Normal_Ready_End;	break;
	}
	
	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		if (m_fpDestFunction == &CUnit::Reload)
		{
			FSM_FunctionSetting(&CUnit::Idle, &CUnit::Reload);
		}
		else
		{
			FSM_FunctionSetting(&CUnit::Idle, &CUnit::Ready_End);
		}
		return;
	}

	m_eCurrentAction = eMainAction;
}


void CUnit::Ready_End(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Ready_End;

	if (m_bAssemble)
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
		return;
	}

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Ready_End;	break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Ready_End;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Ready_End;	break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Ready_End;	break;
	default:							eMainAction = E_ACTION::Normal_Ready_End;	break;
	}

	if (&CUnit::Hiding == m_fpDestFunction)
	{
		FSM_FunctionSetting(&CUnit::Hiding, nullptr);
		return;
	}
	if (FALSE == m_vecAnimActions[(_int)eMainAction].qAnimation.empty())
	{
		m_eHide = E_HIDE::HIDE_NONE;
		DeleteObstacle();
		m_pTarget = nullptr;
		FSM_FunctionRelease();
		return;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		m_eHide = E_HIDE::HIDE_NONE;
		DeleteObstacle();
		m_pTarget = nullptr;
		FSM_FunctionRelease();
		return;
	}

	m_eCurrentAction = eMainAction;
}

void CUnit::Move_End_Stance(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Move_End_Normal;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Move_End_Normal;	break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Move_End_Sit;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Move_End_Stand;		break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Move_End_Normal;	break;
	default:							eMainAction = E_ACTION::Move_End_Normal;	break;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		FSM_FunctionSetting(&CUnit::Ready_Shoot_Start, &CUnit::Shoot);
		return;
	}

	m_eCurrentAction = eMainAction;
}


// Idle상태에서 왔다고 가정하고 자세에 맞는 Reload 액션을 실행한 뒤 Idle상태로 돌아간다
void CUnit::Reload(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Reload;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Reload;		break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Reload;			break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Reload;		break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Reload;		break;
	default:							eMainAction = E_ACTION::Normal_Reload;		break;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		Reload_Ammo();
		Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

		if (m_pTarget)
		{
			_float fDistToTarget = CUtility::Distance_vec3(Get_Position(), m_pTarget->Get_Position());
			if (fDistToTarget <= Get_UnitRange())
			{
				m_fpDestFunction = &CUnit::Shoot;
			}
		}
		FSM_FunctionSetting(&CUnit::Idle, m_fpDestFunction);
	}

	m_eCurrentAction = eMainAction;
}

void CUnit::Hiding(_float fDeltaTime)
{
	if (nullptr == m_pHideObstacle)
	{
		m_listAstarNode.clear();
		FSM_FunctionRelease();
		return;
	}

	m_eCurrentAction = E_ACTION::Move_Ing;

	CObstacle_Combat* pObstacle = static_cast<CObstacle_Combat*>(m_pHideObstacle);

	_vec3 vSpotPos = pObstacle->Get_HideSpotPos(this);
	_vec3 vDir = vSpotPos - Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_listAstarNode.empty())
	{
		Find_Astar(Get_Position(), vSpotPos, m_bJumpObstacle);
	}

	_float fDistance = D3DXVec3Length(&(vSpotPos - Get_Position()));
	if (fDistance < m_fDestDistForArrive * 2.f)
	{
		m_listAstarNode.clear();
		FSM_FunctionSetting(&CUnit::Move_End_Stance, nullptr);
		Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

		if (m_pHideObstacle->Get_Name().substr(1, 1) == L"S")
		{
			m_eHide = E_HIDE::HIDE_SMALL;
		}
		else if (m_pHideObstacle->Get_Name().substr(1, 1) == L"B")
		{
			m_eHide = E_HIDE::HIDE_BIG;
		}
	}
}

void CUnit::Vital_Panic(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Vital_Panic;
	Release_Invoke();
	Clear_Astar();
	if (Get_IsCC())
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}

	m_eCurrentAction = eMainAction;
}

void CUnit::CallSign(_float fDeltaTime)
{
	m_fCallSignTimer = 30.f;
	E_ACTION eMainAction = E_ACTION::Normal_CallSign;
	Release_Invoke();
	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_CallSign;		break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_CallSign;			break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_CallSign;			break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_CallSign;		break;
	default:							eMainAction = E_ACTION::Normal_CallSign;		break;
	}

	if (Is_AnimFinished() && m_eCurrentAction == eMainAction)
	{
		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}

	m_eCurrentAction = eMainAction;

}

void CUnit::Dead(_float fDeltaTime)
{
	m_eCurrentAction = E_ACTION::Vital_Death;
	m_pTarget = nullptr;

	if (m_pHideObstacle)
	{
		DeleteObstacle();
	}
}

void CUnit::Victory(_float fDeltaTime)
{
	if (m_vecAnimActions[(_int)E_ACTION::Victory_End].qAnimation.empty() && m_vecAnimActions[(_int)E_ACTION::Victory_Start].qAnimation.empty())
	{
		m_eCurrentAction = E_ACTION::Normal_Idle;
	}
	else if (E_ACTION::Victory_Start == m_eCurrentAction && Is_AnimFinished())
	{
		m_eCurrentAction = E_ACTION::Victory_End;
	}
	else if (E_ACTION::Victory_Start != m_eCurrentAction && E_ACTION::Victory_End != m_eCurrentAction)
	{
		m_eCurrentAction = E_ACTION::Victory_Start;
	}
}

void CUnit::Begin_Battle(_float fDeltaTime)
{
}

void CUnit::ExSkill(_float fDeltaTime)
{
	m_listAstarNode.clear();
	if ((m_eCurrentAction == E_ACTION::ExSkill && Is_AnimFinished()))
	{
		FSM_FunctionSetting(&CUnit::Idle, &CUnit::Shoot);
		return;
	}

	m_eCurrentAction = E_ACTION::ExSkill;
}

void CUnit::ExSkill_CurIn(_float fDeltaTime)
{
	m_listAstarNode.clear();
	if ((m_eCurrentAction == E_ACTION::ExSkill_CutIn && Is_AnimFinished()) || Get_ActionIsEmpty(E_ACTION::ExSkill_CutIn))
	{
		FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
		return;
	}
	else
	{
		m_eCurrentAction = E_ACTION::ExSkill_CutIn;
	}
}

void CUnit::Assemble(_float fDeltaTime)
{
	E_ACTION eMain = E_ACTION::Move_Ing;

	if (m_listAstarNode.empty())
	{
		m_eHide = E_HIDE::HIDE_NONE;
		DeleteObstacle();
		m_pTarget = nullptr;
		FSM_FunctionRelease();
		return;
	}

	m_eCurrentAction = eMain;
}

void CUnit::Dissipation(_float fDeltaTime)
{
	E_ACTION eMain = E_ACTION::Move_Ing;

	
	if (m_listAstarNode.empty())
	{
		m_eHide = E_HIDE::HIDE_NONE;
		DeleteObstacle();
		m_pTarget = nullptr;
		FSM_FunctionRelease();
		return;
	}

	m_eCurrentAction = eMain;
}

void CUnit::Special_State_1(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Special_State_2(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Boss_Attack_1(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Boss_Attack_2(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Boss_Attack_3(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Boss_Attack_4(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

void CUnit::Boss_Groggy(_float fDeltaTime)
{
	FSM_FunctionSetting(&CUnit::Idle, nullptr);
}

// Trigger 함수포인터의 초기값을 세팅해준다.
void CUnit::Initialize_Trigger()
{
	//  수정 시 Utility 가서 '꼭' Get_TriggerName 같이 수정
	//  수정 시 Utility 가서 '꼭' Get_TriggerName 같이 수정
	m_vecFunction_Trg.clear();
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_1, bind(&CUnit::Trigger_Attack_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_2, bind(&CUnit::Trigger_Attack_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_3, bind(&CUnit::Trigger_Attack_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Dead, bind(&CUnit::Trigger_Dead, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Reload, bind(&CUnit::Trigger_Reload, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::ExSkill, bind(&CUnit::Trigger_ExSkill, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_1, bind(&CUnit::Trigger_Buff_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_2, bind(&CUnit::Trigger_Buff_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_1, bind(&CUnit::Trigger_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_2, bind(&CUnit::Trigger_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_3, bind(&CUnit::Trigger_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_4, bind(&CUnit::Trigger_4, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_5, bind(&CUnit::Trigger_5, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_6, bind(&CUnit::Trigger_6, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_7, bind(&CUnit::Trigger_7, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_8, bind(&CUnit::Trigger_8, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_9, bind(&CUnit::Trigger_9, this)));

	ReSetting_AnimActionVec();
	ReSetting_ActionTrigger();
}

// 액션 시스템의 초기값을 세팅한다.
void CUnit::ReSetting_AnimActionVec()
{
	for (_int i = 0; i < (_int)m_vecAnimActions.size(); ++i)
	{
		_int iQueueSize = (_int)m_vecAnimActions[i].qAnimation.size();
		for (_int j = 0; j < iQueueSize; j++)
		{
			m_vecAnimActions[i].qAnimation.pop();
		}
	}
	m_vecAnimActions.clear();


	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Idle"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Shoot_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Shoot"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Delay"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Shoot_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Reload"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_CallSign"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Ready_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Normal_Ready_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Move_End_Normal"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Idle"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Shoot_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Shoot"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Delay"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Shoot_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Reload"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_CallSign"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Ready_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Stand_Ready_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Move_End_Stand"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Idle"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Shoot_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Shoot"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Delay"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Shoot_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Reload"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_CallSign"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Ready_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Sit_Ready_End"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Move_End_Sit"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Move_Ing"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Move_CallSign"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Jump"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Vital_Death"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Vital_Dying_Ing"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Vital_Retreat"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Vital_Panic"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Formation_Idle"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Formation_Pickup"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"ExSkill_CutIn"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"ExSkill"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Victory_Start"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Victory_End"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Cafe_Idle"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Cafe_Walk"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Cafe_Reaction"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Appear"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Public01"));

	m_vecAnimActions.push_back(T_AnimActionSys(L"Temp1"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Temp2"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Temp3"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Temp4"));
	m_vecAnimActions.push_back(T_AnimActionSys(L"Temp5"));
}

// 액션 트리거의 초기값을 세팅한다.
void CUnit::ReSetting_ActionTrigger()
{
	if (m_pDMeshCom)
	{
		_int iMaxAnimSize = m_pDMeshCom->Get_AnimCountMax();
		m_vecAnimTrg.clear();
		m_vecAnimTrg.reserve(iMaxAnimSize);
		for (_int i = 0; i < (_int)iMaxAnimSize; ++i)
		{
			list<T_AnimTrg> listTemp;
			m_vecAnimTrg.push_back(listTemp);
		}
	}
	else
	{
		for (auto& iter : m_vecAnimTrg)
		{
			iter.clear();
		}
		m_vecAnimTrg.clear();
	}

}

// 함수포인터 트리거를 발동시킨다.
void CUnit::Active_Trigger(T_AnimTrg & trg)
{
	// E_Trigger 같으면 실행
	if (m_vecFunction_Trg[trg.iTrgIndex].first == trg.eTrigger)
	{
		m_vecFunction_Trg[trg.iTrgIndex].second();
	}
}

// Ready_Object에서 m_wstrUnitName을 통해 Mesh를 Load한다.
void CUnit::Ready_DMesh()
{
	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);

	if (nullptr == pMesh)
	{
		CUtility::Print_Wstring_DMode(L"프로토타입이 없어서 새로운 메쉬 프로토타입 생성");

		wstring wstrFullPath = L"../../Reference/Resource/Character/" + m_wstrUnitName + L"/";
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

	// 글로우 끄기
	for (_int i = 0; i < m_pDMeshCom->Get_MeshContainerList().size(); i++)
	{
		m_pDMeshCom->Set_Glow_Option(i, 0, VEC_ZERO, 0, 0.f, 1.f, 0.f, 1.f, VEC_ZERO);
	}
}

// 만약 Mesh가 없거나 아직 로드되지 않았다면  매 프레임 Mesh Load를 시도한다.
_bool CUnit::Find_DMesh()
{
	if (m_pDMeshCom)
		return TRUE;

	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);
	if (nullptr == pMesh)
	{
		return FALSE;
	}

	// 메쉬 찾음

	m_pDMeshCom = static_cast<CDMesh*>(pMesh);
	Initialize_Trigger();
	Load_Animation();

	return TRUE;
}

void CUnit::DeleteObstacle()
{
	if (m_pHideObstacle)
	{
		static_cast<CObstacle_Combat*>(m_pHideObstacle)->CutHideObject();
		m_pHideObstacle = nullptr;
	}
}

void CUnit::FindObstacle()
{
	if (!m_pHideObstacle)
	{
		vector<CGameObject*>* pVecObstacle = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);
		if (!pVecObstacle)
			return;

		for (auto& pObstacle : *pVecObstacle)
		{
			if (pObstacle->Get_Name().substr(1, 1) == L"S" && FALSE == m_bCanHideSmall)
			{
				continue;
			}
			else if (pObstacle->Get_Name().substr(1, 1) == L"B" && FALSE == m_bCanHideBig)
			{
				continue;
			}
			static_cast<CObstacle_Combat*>(pObstacle)->Bid_Obstacle(this);
		}
	}
}

_bool CUnit::Is_AnimFinished()
{
	return m_pDMeshCom->Is_AnimationsetFinish();
}

void CUnit::Update_Trigger(const _float& fTimeDelta)
{
	_int iTrgSize = (_int)m_qTrg.size();
	for (_int i = 0; i < iTrgSize; ++i)
	{
		T_AnimTrg Trg = m_qTrg.front();
		m_qTrg.pop();

		Trg.fActiveDeltaTime -= fTimeDelta;
		if (Trg.fActiveDeltaTime <= 0.f)
		{
			Active_Trigger(Trg);
		}
		else
		{
			m_qTrg.push(Trg);
		}
	}
}

void CUnit::Update_Collider(const _float & fTimeDelta)
{
	if (m_pSphereCollider)
	{
		CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereCollider, E_LAYERTAG::Obstacle);
	}

	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
	Collider_SetMatrix(m_pWeaponCollider, m_szWeaponBoneName);
}

void CUnit::Create_Effect(const wstring & wstrEffectName, const _float fRight, const _float fUp, const _float fLook)
{
	CSkill_Manager::GetInstance()->SetSkill(wstrEffectName, E_LAYERTAG::Effect, Get_WeaponPos(fRight, fUp, fLook), Get_Angle());
}

void CUnit::Create_DamageFont(E_NumberFontType eFontType, _int iDamage, _bool bCritical, _bool bEvade)
{
	CUI_Damage* pDamage = CUI_Damage::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), E_OBJTAG::OBJ_UI, 0.5f, eFontType, iDamage, bCritical);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Damage_Font, E_OBJTAG::OBJ_UI, pDamage);

	_int iSizeX = 16;
	_int iSizeY = 16;
	switch (eFontType)
	{
	case Engine::E_NumberFontType::Normal:
		break;
	case Engine::E_NumberFontType::Resist:
		iSizeX = 14;
		iSizeY = 14;
		break;
	case Engine::E_NumberFontType::Weak:
		iSizeX = 20;
		iSizeY = 20;
		break;
	default:
		assert(FALSE);
		break;
	}
	pDamage->Set_Ui_SizeX(iSizeX);
	pDamage->Set_Ui_SizeY(iSizeX);
	pDamage->Start_Font_Animation();

	//계산 과정
	_vec3 vWorldPos = m_pSphereCollider->GetPos();
	vWorldPos.y += m_pSphereCollider->GetRadius() * 1.f;

	_vec2 vPos = CUtility::World2Ui(vWorldPos);
	vPos.y += 30.f;
	pDamage->Set_Ui_PosX(_int(vPos.x));
	pDamage->Set_Ui_PosY(_int(vPos.y));

	if (bEvade)
	{
		pDamage->Set_Evade(TRUE);
	}
	if (0 >= iDamage)
	{
		pDamage->Set_Immune(TRUE);
	}

}

void CUnit::Create_HitEffect()
{
	// 랜덤 값 생성
	_float fRad = m_pTarget->Get_SphereCollider()->GetRadius();
	_float fRand_X = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);
	_float fRand_Y = CWall512::GetInstance()->GetFloatValue(-fRad * 0.1f, fRad * 0.3f);
	_float fRand_Z = CWall512::GetInstance()->GetFloatValue(-fRad * 0.3f, fRad * 0.5f);

	_vec3 vRandom = m_pTarget->Get_SphereCollider()->GetPos() + _vec3(fRand_X, fRand_Y, fRand_Z);

	switch (m_eUnitHitType)
	{
	case Engine::E_UNIT_HIT_TYPE::HIT_NOTYPE_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;

	case Engine::E_UNIT_HIT_TYPE::HIT_AR_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_SG_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_MG_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_HG_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_SMG_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_MT_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_RG_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	case Engine::E_UNIT_HIT_TYPE::HIT_GL_DEFAULT:
		CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;

	case Engine::E_UNIT_HIT_TYPE::HIT_END:
		
		//CSkill_Manager::GetInstance()->SetSkill(L"EH_Hit_Explosion_0106.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		CSkill_Manager::GetInstance()->SetSkill(L"Public_Hit_0.skill", E_LAYERTAG::Effect, vRandom, m_pTarget->Get_Angle());
		break;
	default:
		break;
	}
}

void CUnit::Look_Target_Slowly(const _float& fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;

	if (m_fpLockFunction == &CUnit::Hiding)
		return;

	if (FALSE == m_listAstarNode.empty())
		return;


	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK) - Get_SphereCollider()->GetPos();
	_vec3 vTargetPos = m_pTarget->Get_SphereCollider()->GetPos();
	vTargetPos.y = 0.f;

	_vec3 vDirToTarget = vTargetPos - Get_SphereCollider()->GetPos();

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vDirToTarget, &vDirToTarget);

	_float fAngleWithTarget = D3DXVec3Dot(&vLook, &vDirToTarget);

	if (abs(fAngleWithTarget) < 10.f)
	{
		Set_Look(vTargetPos);
		return;
	}

	_vec3 vRot = Get_Angle();
	if (fAngleWithTarget < 0.f)
	{
		vRot.y += fTimeDelta * 60.f;
	}
	else
	{
		vRot.y -= fTimeDelta * 60.f;
	}
	Set_Rotation(E_ROTATION::ROT_Y, vRot.y);
}

void CUnit::Create_Collider(CSphereCollider ** pCol, wstring wstrCompName, _float fRadius)
{
	*pCol = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(wstrCompName, static_cast<CComponent*>(*pCol));
	(*pCol)->SetRadius(fRadius);
	(*pCol)->SetScale(Get_Scale());
}

void CUnit::Collider_SetMatrix(CSphereCollider * pCol, const char * szFindBoneName)
{
	const D3DXFRAME_DERIVED* pBodyFrame = m_pDMeshCom->Get_FrameByName(m_szBodyBoneName);

	if (nullptr == pBodyFrame)
	{
		CUtility::Print_Wstring_DMode(L"뼈가 없어요 (m_szBodyBoneName)");
		return;
	}

	const D3DXFRAME_DERIVED* pFindFrame = m_pDMeshCom->Get_FrameByName(szFindBoneName);
	if (pFindFrame)
	{
		pCol->SetMatrix(pFindFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix());
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"뼈가 없어요" + CUtility::CHAR_TO_WSTRING(szFindBoneName));
		pCol->SetMatrix(pBodyFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix());
	}
}

// iIndex 번 째 애니메이션을 실행하면서 해당 애니메이션의 모든 트리거를 큐에 삽입해준다.
void CUnit::Set_AnimIndexForTrigger(_uint iIndex)
{
	m_pDMeshCom->Set_AnimationIndex_Loop(iIndex);

	if ((_int)m_vecAnimTrg.size() <= iIndex)
	{
		return;
	}

	for (auto& iter : m_vecAnimTrg[iIndex])
	{
		m_qTrg.push(iter);
	}
}

// wstrName이름을 가진 액션을 실행하면서 해당 액션의 첫 번째 애니메이션을 실행하고
void CUnit::Set_ActionByName(wstring wstrName)
// 2번째 이후 애니메이션을 m_qNextAnimIdx에 넣어준다.
{
	for (auto& iter : m_vecAnimActions)
	{
		if (iter.wstrActionKey == wstrName)
		{
			_int iSize = (_int)iter.qAnimation.size();
			queue<_int> qCopy = iter.qAnimation;
			for (_int i = 0; i < iSize; ++i)
			{
				_int iFrontAnimIndex = qCopy.front();
				qCopy.pop();

				if (0 != i)
				{
					m_qNextAnimIdx.push(iFrontAnimIndex);
				}
				else
				{
					Set_AnimIndexForTrigger(iFrontAnimIndex);
				}
			}
			break;
		}
	}
}

void CUnit::Find_Astar(_vec3 vStartPos, _vec3 vDest, _bool bCanJump)
{
	if (m_bAssemble)
	{
		return;
	}

	// 광선의 시작위치와 방향을 구한다.
	vDest.y = 0.f;
	vStartPos.y = 0.f;
	_vec3 vRayPos = vStartPos;
	_vec3 vRayDir = vDest - vRayPos;
	_float fDistanceToDest = CUtility::Distance_vec3(vStartPos, vDest);

	if (m_fDestDistForArrive > fDistanceToDest)
	{
		return;
	}

	D3DXVec3Normalize(&vRayDir, &vRayDir);

	// 충돌체
	CLayer* pObstacleLayer = CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::Obstacle);
	vector<CGameObject*>* pVecObstacle = pObstacleLayer->Get_vecGameObject(E_OBJTAG::OBJ_Obstacle);

	vector<CObstacle_Combat*> vecCloseObstacle;
	_bool bArriveDest = FALSE;
	_bool bColResult = FALSE;
	_vec3 vFinalPos = VEC_ZERO;

	
	// 장애물이 있을 경우에만 체크
	if (nullptr != pVecObstacle)
	{
		for (_int i = 0; i < (_int)pVecObstacle->size(); ++i)
		{
			if ((*pVecObstacle)[i] == m_pHideObstacle)
				continue;

			CObstacle_Combat* pObstacle = static_cast<CObstacle_Combat*>((*pVecObstacle)[i]);
			_float fDistWithObs = CUtility::Distance_vec3(vStartPos, pObstacle->Get_ShperePos());
			_vec3 vObsDir = pObstacle->Get_ShperePos() - vRayPos;

			// 예외처리 1 : 목적지보다 멀리 있다면
			if (fDistWithObs >= fDistanceToDest)
			{
				continue;
			}

			// 예외처리 2 : 작은 구조물을 뛰어넘는다면
			if (pObstacle->Get_Name().substr(1, 1) == L"S" && TRUE == bCanJump)
			{
				continue;
			}

			// 예외처리 3 : 내적을 통해 RayDir과 vObsDir의 Theta (Θ) 가 15도 이상이면
			// 혹은 내적이 0보다 작으면 (반대방향)
			if (15.f * Rad2Deg <= abs(D3DXVec3Dot(&vObsDir, &vRayDir)) || 0 > D3DXVec3Dot(&vObsDir, &vRayDir))
			{
				continue;
			}

			// 예외처리 4 : 
			pObstacle->Set_DistanceForSort(fDistWithObs);
			vecCloseObstacle.push_back(pObstacle);
		}

		// 충돌 가능한 장애물을 거리비례로 정렬
		sort(vecCloseObstacle.begin(), vecCloseObstacle.end(), CObstacle_Combat::CompareByDistance);
		_int iCloseObsSize = (_int)vecCloseObstacle.size();

		// 충돌 가능한 장애물을 거리가 가까운 순 부터 충돌할지 말지 결정
		for (_int i = 0; i < iCloseObsSize; ++i)
		{
			_vec3 vSpherePos = vecCloseObstacle[i]->Get_ShperePos();		// 충돌체 중점
			_float vSphereRadius = vecCloseObstacle[i]->Get_Radius();		// 충돌체 반지름
			_vec3 vRayToObs = vSpherePos - vRayPos;							// 시작지점 부터 충돌체방향 벡터
			_float fDot_Rto_Rdir = D3DXVec3Dot(&vRayToObs, &vRayDir);		// To목표, To충돌체의 내적
			_float fScalar_Rto = D3DXVec3Dot(&vRayToObs, &vRayToObs);		// To충돌체의 거리
			_float r2 = pow(vSphereRadius, 2);								// 충돌체 반지름 제곱

			// 광선이 구의 반대 방향을 향하거나 구를 지나친 경우
			if (0 > fDot_Rto_Rdir && fScalar_Rto > r2)
			{
				bColResult = FALSE;
				continue;
			}
			_float m2 = fScalar_Rto - pow(fDot_Rto_Rdir, 2);

			// 광선이 구를 비껴가는 경우
			if (m2 > r2)
			{
				bColResult = FALSE;
				continue;
			}

			bColResult = TRUE;
			_float q = sqrt(r2 - m2);
			_float distance = 0.f;

			// 장애물과 만난 경우
			if (fScalar_Rto > r2)
			{
				distance = fDot_Rto_Rdir - q;
				_vec3 vInsec = vRayPos + distance * vRayDir;
				_vec3 vTemp = vInsec - vSpherePos;
				_vec3 vInsecToDest = vInsec - vDest;
				_float fAngleToDest = atan2(vInsecToDest.z, vInsecToDest.x);
				_float fDir = 1.f;
				_vec3 vPlus = _vec3(cosf(atan2(vTemp.z, vTemp.x) + 135.f * 1.f * Deg2Rad), 0.f, sinf(atan2(vTemp.z, vTemp.x) + 135.f * 1.f * Deg2Rad));
				_vec3 vMinus = _vec3(cosf(atan2(vTemp.z, vTemp.x) - 135.f * 1.f * Deg2Rad), 0.f, sinf(atan2(vTemp.z, vTemp.x) - 135.f * 1.f * Deg2Rad));
				if (CUtility::Distance_vec3(vDest, vPlus + vSpherePos) >= CUtility::Distance_vec3(vDest, vMinus + vSpherePos))
				{
					fDir = -1.f;
				}

				m_listAstarNode.push_back(T_AstarNode(vInsec));
 
				D3DXVec3Normalize(&vTemp, &vTemp);
				_float fAngle = atan2(vTemp.z, vTemp.x) + fDir * 135.f * 1.f * Deg2Rad;
				_vec3 vNewDest = _vec3(cosf(fAngle), 0.f, sinf(fAngle));
				_vec3 vSphereEnd = vSpherePos + vNewDest * vSphereRadius * 1.5f;
				_vec3 vBeforeFrame = VEC_ZERO;
				for (_int j = 0; j < 3; ++j)
				{
					fAngle = atan2(vTemp.z, vTemp.x) + fDir * 135.f * j / 3 * Deg2Rad;
					vNewDest = _vec3(cosf(fAngle), 0.f, sinf(fAngle));
					_vec3 vCalc = vSpherePos + vNewDest * vSphereRadius * 1.01f;

					if (m_fDestDistForArrive * 6.f >= CUtility::Distance_vec3(vDest, vCalc))
					{
						m_listAstarNode.push_back(T_AstarNode(vDest));
						bArriveDest = TRUE;
					}
					else
					{
						m_listAstarNode.push_back(T_AstarNode(vCalc));
						vFinalPos = vCalc;
					}
					vBeforeFrame = vCalc;
				}
				break;
			}
			// 이번 Loop의 장애물과 만나지 않았을 경우 장애물의 반지름 만큼 전진.
			else
			{
				_vec3 vMoveDir = vRayToObs;
				D3DXVec3Normalize(&vMoveDir, &vMoveDir);
				vMoveDir *= (fScalar_Rto + vSphereRadius) * 1.01f;
				m_listAstarNode.push_back(T_AstarNode(vMoveDir));
				vFinalPos = vMoveDir;
			}

			break;
		}
	}

	// 장애물과 충돌하지 않았다면 목적지로 이동
	if (FALSE == bColResult)
	{
		m_listAstarNode.push_back(T_AstarNode(vDest));
	}
	// 목적지에 도착하지 못했다면 Astar에 넣은 마지막 위치에서 목표지점으로 다시 한번 Find_Astar를 재귀적으로
	else if (FALSE == bArriveDest)
	{
		Find_Astar(vFinalPos, vDest, bCanJump);
	}
}


void CUnit::MoveTo_Astar(_float fTimeDelta, _float fSpeed, _float fSRatio)
{
	if (Get_UnitDead())
	{
		DeleteObstacle();
		m_listAstarNode.clear();
		return;
	}
	if (m_listAstarNode.empty())
		return;

	_vec3 vDest = m_listAstarNode.front().vPosition;
	vDest.y = 0.f;

	_vec3 vPos = Get_Position();
	vPos.y = 0.f;

	_vec3 vDir = vDest - vPos;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	// 이번 프레임 이동 Vector
	_vec3 vMoveAmount = vDir * fTimeDelta * fSpeed * fSRatio;
	Set_Look(vPos + vDir);


	// #1 : 한 프레임 이동량이 목적지까지의 거리보다 크면 Set
	// #2 : 목적지까지의 거리가 0.1f 보다 크면 이동
	// #3 : 목적지까지의 거리가 0.1f 보다 작으면 노드 제거 후 다음 노드로

	if (m_fDestDistForArrive >= CUtility::Distance_vec3(vPos, vDest))
	{
		Add_Pos(vMoveAmount);
		m_listAstarNode.erase(m_listAstarNode.begin());
	}
	else if (m_fDestDistForArrive < CUtility::Distance_vec3(vPos, vDest))
	{
		Add_Pos(vMoveAmount);
	}

	m_bMoveThisFrame = TRUE;
}

void CUnit::Update_CombatStatus(const _float & fTimeDelta)
{
	// 매 프레임 스탯 상태 초기화
	ZeroMemory(m_iArrNowCombatStatus, sizeof(_int) * (_int)E_Combat_Status::Combat_StatusEnd);

	if (3 > CWall512::GetInstance()->GetValue(0, 100))
	{
		//Add_CombatStatus(T_CombatStatus((E_Combat_Status)CWall512::GetInstance()->GetValue(0, (_int)E_Combat_Status::Combat_StatusEnd), 3.f));
	}

	// 모든 스탯들의 지속시간을 감소시켜주고 지속시간이 0보다 작아지면 제거한다.
	vector<T_CombatStatus>::iterator iter = m_vecCombatStatus.begin();
	for (; iter != m_vecCombatStatus.end();)
	{
		iter->fLeftTime -= fTimeDelta;
		m_iArrNowCombatStatus[(_int)iter->eStatus] += 1;

		if (iter->fLeftTime <= 0.f)
		{
			iter = m_vecCombatStatus.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_int iBuffCount = 0;
	_int iDeBuffCount = 0;
	for (_int i = 0; i < (_int)E_Combat_Status::Combat_StatusEnd; ++i)
	{
		if (0 < m_iArrNowCombatStatus[i])
		{
			wstring wstrCountImagePath = L"../../Reference/Resource/Texture/UI/Combat/Combat_Nearlydead_Count_";

			m_iArrNowCombatStatus[i] > 9 ? wstrCountImagePath.append(to_wstring(9)) : wstrCountImagePath.append(to_wstring(m_iArrNowCombatStatus[i]));

			wstrCountImagePath.append(L".png");

			// 버프, 스폐셜
			if (i <= (_int)E_Combat_Status::Spe_Immortal && iBuffCount < 4)
			{
				wstring wstrImagePath = CUtility::Get_CombatStatusPath(E_Combat_Status(i));

				m_pIcon_Buff[iBuffCount]->Set_Tex1_Tag(wstrImagePath);
				m_pIcon_Buff[iBuffCount]->Set_Active(TRUE);
				m_pIcon_Buff[iBuffCount]->Update_Object(0.f);

				m_pIcon_Buff[iBuffCount]->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrCountImagePath);
				iBuffCount++;
			}

			// CC 디버프 스폐셜
			else if (i > (_int)E_Combat_Status::Spe_Immortal && iDeBuffCount < 4)
			{
				wstring wstrImagePath = CUtility::Get_CombatStatusPath(E_Combat_Status(i));
				m_pIcon_Debuff[iDeBuffCount]->Set_Tex1_Tag(wstrImagePath);
				m_pIcon_Debuff[iDeBuffCount]->Set_Active(TRUE);
				m_pIcon_Debuff[iDeBuffCount]->Update_Object(0.f);

				m_pIcon_Debuff[iDeBuffCount]->Get_ChildByIndex(0)->Set_Tex1_Tag(wstrCountImagePath);
				iDeBuffCount++;
			}
		}
	}

	for (_int i = iBuffCount; i < 4; ++i)
	{
		m_pIcon_Buff[i]->Set_Active(FALSE);
	}
	for (_int i = iDeBuffCount; i < 4; ++i)
	{
		m_pIcon_Debuff[i]->Set_Active(FALSE);
	}

}
