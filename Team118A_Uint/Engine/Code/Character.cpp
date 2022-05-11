#include "Character.h"

#include "SphereCollider.h"
#include "Bullet_Normal.h"
#include "Bullet_Normal_Trail.h"

#include "XL_ParsingMgr.h"
#include "ProtoMgr.h"
#include "Texture.h"
#include "UI_Object.h"
#include "Renderer.h"
#include "Scene.h"
#include "CharacterMgr.h"
#include "GameManager.h"
#include "UI_Manager.h"
USING(Engine)

CCharacter::CCharacter(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CUnit(pGraphicDev, eObjTag, wstrUnitName)
{
	m_eMyLayerTag = E_LAYERTAG::Character;
	m_eTargetLayerTag = E_LAYERTAG::Monster;
	m_eTargetObjTag = E_OBJTAG::OBJ_Monster;
}

CCharacter::CCharacter(const CCharacter & rhs)
	: CUnit(rhs)
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::Free()
{

	CUnit::Free();
}

void CCharacter::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CCharacter::Load(HANDLE hFile, DWORD & dwByte)
{
}

// 여기서 애니메이션 로드
HRESULT CCharacter::Ready_Object()
{
	CUnit::Ready_Object();

	m_fSoundVolume += 0.075f; 

	wstring wstrParsingName = m_wstrUnitName;
	auto iterEnd = ++wstrParsingName.begin();
	transform(wstrParsingName.begin(), iterEnd, wstrParsingName.begin(), towupper);

	m_eUnitMode = E_UnitMode::Auto;

	Set_GeneralStat((E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(wstrParsingName, E_Translation_Type::Translation_CharInfo));
	Reload_Ammo();

	m_tUnitInfo = m_tCharInfo.tUnitInfo;

	m_pTexMouse = static_cast<CTexture*>(Emplace_Component(L"Com_TextureMouse", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CharacterMouse")));
	m_pHealthBar->Set_ImageColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	m_ePrevAction = E_ACTION::ACTION_END;
	m_eCurrentAction = E_ACTION::ACTION_END;

	m_bStartSound = FALSE; 

	m_eMouse = E_CharacterMouseTag::일자입;

	return S_OK;
}

_int CCharacter::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CUnit::Start_Object(fTimeDelta);

	return iExit;
}

_int CCharacter::Update_Object(const _float & fTimeDelta)
{
	if (E_UnitMode::TPS == m_eUnitMode)
	{

	}
	else
	{
		if (!m_bShowWindow)
		{
			if (m_bPlay)
			{
				m_pCombatObject->Set_Active(TRUE);
				Update_FSM(fTimeDelta, m_eTargetLayerTag, m_eTargetObjTag);
				Update_CombatUI(fTimeDelta);
			}
			else
			{
				Begin_Battle(fTimeDelta);
			}
		}
		else
		{
			m_pCombatObject->Set_Active(FALSE);
			if (m_eCurrentAction != E_ACTION::Formation_Pickup)
			{
				m_eCurrentAction = E_ACTION::Formation_Idle;
			}
		}

		if (m_ePrevAction != m_eCurrentAction)
		{
			//CUtility::Print_Wstring_DMode(CXL_ParsingMgr::GetInstance()->Translation_EnumToString((_int)m_tCharInfo.eName[0], E_CHAR_ENUMTYPE::CNUM_KORNAME) + L" : " + to_wstring((_int)m_eCurrentAction));
		}

		Animation_Change(fTimeDelta);
	}

	if (m_bUseGravity)
	{
		if (!m_bShowWindow)
		{
			if (m_bGravityOn)
			{
				Update_Gravity(fTimeDelta);
			}
			else
			{
				_vec3 vPickPos;
				if (!Raycast_Gravity(&vPickPos))
				{
					m_vKnockBackDir = VEC_ZERO;
					m_fKnockBackSpeed = 0.f;
					m_fGravitySpeed = 0.f;
					m_bGravityOn = TRUE;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	//_int iExit = CUnit::Update_Object(fTimeDelta);

	Update_CombatStatus(fTimeDelta);
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
			if (m_bShowWindow)
			{
				CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHOWWINDOW, this);
			}
			else
			{			
				CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);			
				CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
			}
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
	return iExit;

	////////////////////////////////////////////////////////////////////////////////////////////

	GuideSkillUpdate(fTimeDelta);


	//if (m_bShowWindow)
	//{
	//	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHOWWINDOW, this);
	//}

	return iExit;
}

_int CCharacter::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CUnit::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CCharacter::Render_Object(_int iPass)
{

	if (!m_bShowWindow)
	{
		CUnit::Render_Object(iPass);
	}
	else
	{		
		//CShader* pShader = CRenderer::GetInstance()->Get_ShowBaseShader();
		CShader* pShader = m_pShaderCom;

		if (pShader && m_pDMeshCom)
		{
			Set_Constantable();

			pShader->BeginShader();
			pShader->BeginPass(ShaderPass(E_ShaderPass::DMesh,0));

			m_pDMeshCom->Render_Meshes(pShader);

			pShader->EndPass();
			pShader->EndShader();
		}
	}

}

void CCharacter::Set_Constantable()
{
	CUnit::Set_Constantable();

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (m_pTexMouse)
	{
		m_pTexMouse->Set_Texture(pEffect, "g_MouseTexture");
	}
	
	_int iMouse = (_int)m_eMouse;
	float fU = float(iMouse % 4);
	float fV = float(iMouse / 4);

	pEffect->SetFloat("g_fMouseU", fU * 0.25f);
	pEffect->SetFloat("g_fMouseV", fV * 0.25f);

}

void CCharacter::Set_GuideSkill()
{
	/*CSkillGuideObject* pGuide = nullptr;
	pGuide = CSkill_Manager::GetInstance()->SetSkillGuideCircle(Get_Position(), 3.f, 9999.f, 30, FALSE);
	if (pGuide)
	{
		m_vecGuideSkill.emplace_back(pGuide);
	}*/
}

void CCharacter::Use_ExSkill()
{
	if (!m_vecGuideSkill.empty())
	{
		Set_Direction(m_vExDir);
		for (auto& pGuide : m_vecGuideSkill)
		{
			pGuide->Set_Dead(TRUE);
		}
		m_vecGuideSkill.clear();
	}
	m_bAiming = FALSE;
	CUnit::Use_ExSkill();
}

void CCharacter::Cancel_Aiming()
{
	m_bAiming = FALSE;
	m_vExDir = -VEC_LOOK;
	for (auto& pGuide : m_vecGuideSkill)
	{
		pGuide->Set_Dead(TRUE);
	}
	m_vecGuideSkill.clear();
}

void CCharacter::Update_CombatUI(const _float& fTimeDelta)
{
	if (E_ACTION::Normal_Reload == m_eCurrentAction || E_ACTION::Stand_Reload == m_eCurrentAction || E_ACTION::Sit_Reload == m_eCurrentAction)
	{
		m_pCombat_Reload->Set_Active(TRUE); 

		m_fCombat_Reload_Cool -= fTimeDelta;

		if (m_fCombat_Reload_Cool < 0.f)
		{
			m_fCombat_Reload_Cool = 0.25f;

			if (!m_pCombat_Reload->Get_ChildByIndex(0)->Get_Active())
			{
				m_pCombat_Reload->Get_ChildByIndex(0)->Set_Active(TRUE);
			}
			else if (!m_pCombat_Reload->Get_ChildByIndex(1)->Get_Active())
			{
				m_pCombat_Reload->Get_ChildByIndex(1)->Set_Active(TRUE);
			}
			else if (!m_pCombat_Reload->Get_ChildByIndex(2)->Get_Active())
			{
				m_pCombat_Reload->Get_ChildByIndex(2)->Set_Active(TRUE);
			}
			else  
			{
				for (_uint i = 0; i < 3; ++i) 
				{
					m_pCombat_Reload->Get_ChildByIndex(i)->Set_Active(FALSE);
				}
			}
		}
	}
	else
	{
		m_pCombat_Reload->Set_Active(FALSE); 

		for (_uint i = 0; i < 3; ++i) 
		{
			m_pCombat_Reload->Get_ChildByIndex(i)->Set_Active(FALSE); 
		}
	}
}

void CCharacter::OnEnable()
{
	CUnit::OnEnable();
}

void CCharacter::OnDisable()
{
	CUnit::OnDisable();
}

void CCharacter::OnDestroy()
{
	CUnit::OnDestroy();
}

void CCharacter::On_Collision(CGameObject * pDest)
{
	if (m_bStart)
		return;
}

void CCharacter::Reset()
{

	CUnit::Reset();
	CRenderer::GetInstance()->Set_CutIn(FALSE);
	m_bShowWindow = FALSE;
	Stop_Gravity();
	m_bUseGravity = FALSE;
}

void CCharacter::LevelUpByLevel(_int iLevel)
{
	m_tUnitInfo.iMaxHp += m_tCharInfo.iIncLevel_HP;
	m_tUnitInfo.iAttack += m_tCharInfo.iIncLevel_ATK;
	m_tUnitInfo.iDeffence += m_tCharInfo.iIncLevel_DEF;
	m_tUnitInfo.iHeal += m_tCharInfo.iIncLevel_HEAL;
}

void CCharacter::Set_Gravity(_vec3 vKnockBackDir, _float fKnockBackSpeed)
{
	m_fKnockBackSpeed = fKnockBackSpeed;
	m_vKnockBackDir = vKnockBackDir;
	m_bGravityOn = TRUE;
	m_fGravitySpeed = -0.1f;
}

void CCharacter::Stop_Gravity()
{
	m_vKnockBackDir = VEC_ZERO;
	m_fKnockBackSpeed = 0.f;
	m_fGravitySpeed = 0.f;
	m_bGravityOn = FALSE;
}

void CCharacter::ExSkill(_float fDeltaTime)
{

	Set_Direction(m_vExDir);

	CRenderer::GetInstance()->Set_CutIn(FALSE);
	CUnit::ExSkill(fDeltaTime);
}

void CCharacter::ExSkill_CurIn(_float fDeltaTime)
{
	CRenderer::GetInstance()->Set_CutIn(TRUE);
	CUnit::ExSkill_CurIn(fDeltaTime);
}

void CCharacter::Begin_Battle(_float fDeltaTime)
{
	if (m_eObjTag != E_OBJTAG::OBJ_Special)
	{
		if (Is_AnimFinished() && (m_eCurrentAction == E_ACTION::Normal_Idle || m_eCurrentAction == E_ACTION::Formation_Idle || m_eCurrentAction == E_ACTION::Normal_CallSign))
		{
			if (Get_ActionIsEmpty(E_ACTION::Formation_Idle))
			{
				Set_ActionByEnum(E_ACTION::Normal_Idle);
			}
			else
			{
				Set_ActionByEnum(E_ACTION::Formation_Idle);
			}
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

		if (m_bReader)
		{
			if (!(m_eCurrentAction == E_ACTION::Normal_Idle || m_eCurrentAction == E_ACTION::Formation_Idle))
			{
				Set_ActionByEnum(E_ACTION::Normal_CallSign);
				if (!m_bStartSound)
				{
					Begin_Battle_Sound();

					m_bStartSound = TRUE;
				}
			}
		}
		else
		{
			if (Get_ActionIsEmpty(E_ACTION::Formation_Idle))
			{
				Set_ActionByEnum(E_ACTION::Normal_Idle);
			}
			else
			{
				Set_ActionByEnum(E_ACTION::Formation_Idle);
			}
		}
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Normal_Ready_Start);
	}
}

void CCharacter::Begin_Battle_Sound()
{
}

void CCharacter::End_Battle_Sound(const _bool& bWin)
{
}

void CCharacter::Update_Collider(const _float & fTimeDelta)
{
	CUnit::Update_Collider(fTimeDelta);
}

void CCharacter::Update_Gravity(_float fTimeDelta)
{
	_float fPrevY = Get_Position().y;

	m_fGravitySpeed += m_fGravityAcc * fTimeDelta;
	Add_Pos(m_vKnockBackDir * m_fKnockBackSpeed);
	Add_Pos(_vec3(0.f, -m_fGravitySpeed * fTimeDelta, 0.f));

	if (fPrevY >= 0.f && Get_Position().y < 0.f)
	{
		_vec3 vPickPos;
		if (Raycast_Gravity(&vPickPos))
		{
			Set_Pos(vPickPos);
			m_bGravityOn = FALSE;
			m_fGravitySpeed = 0.f;
			m_vKnockBackDir = VEC_ZERO;
			m_fKnockBackSpeed = 0.f;
		}
	}
}

_bool CCharacter::Raycast_Gravity(_vec3 * pOutPos)
{
	for (auto& tPair : *CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Map))
	{
		for (auto& pMap : tPair.second)
		{
			CSMesh* pSMesh = static_cast<CSMesh*>(pMap->Get_Component(ComTag_SMesh));
			if (pSMesh)
			{
				_vec3  vShootPos = Get_Position();
				vShootPos.y += 3.f;
				_float fDist;
				if (pSMesh->PickingMesh(vShootPos, VEC_DOWN, pOutPos, &fDist, pMap->Get_Transform()->Get_WorldMatrix()))
				{
					if (fDist < 10.f)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

void CCharacter::GuideSkillUpdate(_float fTimeDelta)
{
	if (!m_bAiming)
	{
		return;
	}
	_vec3 vPickPos;
	if (GetRaycastMap(&vPickPos))
	{

		vPickPos.y = 0.f;
		m_vExDir = vPickPos - Get_Position();
		D3DXVec3Normalize(&m_vExDir, &m_vExDir);
		GuideSkillSetting(vPickPos);

	}
	else
	{
		return;
	}
}

_bool CCharacter::GetRaycastMap(_vec3 * pOutPos)
{
	_float fDist;
	_bool bCheck = FALSE;
	for (auto& tPair : *CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Map))
	{
		for (auto& pMap : tPair.second)
		{
			CSMesh* pSMesh = nullptr;
			pSMesh = static_cast<CSMesh*>(pMap->Get_Component(ComTag_SMesh));
			if (pSMesh)
			{
				if (pSMesh->PickingMesh(*CUI_Manager::GetInstance()->Get_Hwnd(), pOutPos, &fDist, pMap->Get_Transform()->Get_WorldMatrix()))
				{
					bCheck = TRUE;
					break;
				}
			}
		}
	}

	return bCheck;
}

void CCharacter::GuideSkillSetting(_vec3 vPickPos)
{
	for (auto& pGuide : m_vecGuideSkill)
	{
		pGuide->Set_Pos(vPickPos);
	}
}


void CCharacter::Trigger_Attack_1()
{
	CUnit::Trigger_Attack_1();

	_vec3 vTargetBodyPos = VEC_ZERO;
	if (m_pTarget)
	{
		vTargetBodyPos = m_pTarget->Get_SphereCollider()->GetPos() - Get_WeaponPos(0.f, 0.f, 0.f);
	}
	else
	{
		vTargetBodyPos = *D3DXVec3Normalize(&+Get_Info(E_INFO::INFO_LOOK), &Get_Info(E_INFO::INFO_LOOK)) - Get_WeaponPos(0.f, 0.f, 0.f);
	}
	D3DXVec3Normalize(&vTargetBodyPos, &vTargetBodyPos);

	CGameObject*	pGameObject = nullptr;
	switch (m_tCharInfo.eWeaponType)
	{
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:
		pGameObject = CBullet_Normal_Trail::Create(m_pGraphicDev, Get_WeaponPos(0.f, 0.f, 0.f), vTargetBodyPos, m_tUnitInfo, m_tCharInfo.eAtkType, E_LAYERTAG::Monster);
		break;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_NULL:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_HG:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MT:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RL:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RG:
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_END:
		pGameObject = CBullet_Normal::Create(m_pGraphicDev, Get_WeaponPos(0.f, 0.f, 0.f), vTargetBodyPos, m_tUnitInfo, m_tCharInfo.eAtkType, E_LAYERTAG::Monster);
		static_cast<CBullet_Normal*>(pGameObject)->Set_EffectObject(CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Test_Bullet.effect"));

		break;
	default:
		break;
	}

	assert(pGameObject);
	if (m_eTargetLayerTag == E_LAYERTAG::Monster)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Player, E_OBJTAG::OBJ_AtkObj, pGameObject);
	}
	else if (m_eTargetLayerTag == E_LAYERTAG::Character)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
	}
}

_int CCharacter::Set_Damage(const T_UnitInfo & tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType)
{
	_int iDamage = CUnit::Set_Damage(tEnemyInfo, eEnemyAtkType);

	if (CGameManager::GetInstance()->Get_PlayerInvn() && 0.5f > (m_tUnitInfo.iNowHp / m_tUnitInfo.iMaxHp))
	{
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	}

	return iDamage;
}

void CCharacter::Set_GeneralStat(E_CHAR_NAME eName)
{
	m_eMyName = eName;

	m_tCharInfo = CCharacterMgr::GetInstance()->Get_CharInfo(m_eMyName);
}