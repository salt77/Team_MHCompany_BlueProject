#include "Hieronymus.h"
#include "UI_Damage.h"

#include "UI_Manager.h"
#include "Boss_HpBar.h"

#include "Scene.h"
#include "Bullet_Bezier.h"
#include "EffectObject.h"
#include "Warning.h"
#include "Warning_Mgr.h"
#include "GameManager.h"
#include "LightMgr.h"
#include "SkyDome.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CHieronymus::CHieronymus(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CBossMonster(pGraphicDev, eObjTag, L"Hieronymus")
{
}

CHieronymus::CHieronymus(const CHieronymus & rhs)
	:CBossMonster(rhs)
{
}

CHieronymus::~CHieronymus()
{
}

CHieronymus * CHieronymus::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHieronymus* pInstance = new CHieronymus(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHieronymus::Free()
{
	Safe_Release(m_pGreen_Lantern);
	Safe_Release(m_pRed_Lantern);
	CLightMgr::GetInstance()->Reset_GodLight_List();
	CBossMonster::Free();
}

HRESULT CHieronymus::Ready_Object()
{
	CBossMonster::Ready_Object();
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_보스전_히에로니무스.mp3", 0.3f);

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bone_F_Hand";

	m_tUnitInfo.iAttack = 150;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence = 0;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 500000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;

	m_tUnitInfo.iRange = 999999;
	m_tUnitInfo.iStability;

	CScene::Get_MainScene()->Set_AutoCam_Ratio(1.2f);

	m_tGlowAnim_Arm.eEase					= E_EASE::InExpo;
	m_tGlowAnim_Arm.vStart_Min_UV			= _vec2(0.9f, 0.0f);
	m_tGlowAnim_Arm.vStart_Max_UV			= _vec2(1.0f, 0.004f);
	m_tGlowAnim_Arm.vEnd_Min_UV				= _vec2(0.9f, 0.0f);
	m_tGlowAnim_Arm.vEnd_Max_UV				= _vec2(1.0f, 0.01126f);
	m_tGlowAnim_Arm.fTotalTime				= 2.f;
	m_tGlowAnim_Arm.vBeginEnd_Power			= _vec2(0.f, 1.4f);

	m_tGlowAnim_FullBody.vStart_Min_UV		= _vec2(1.0f, 0.0f);
	m_tGlowAnim_FullBody.vStart_Max_UV		= _vec2(1.0f, 1.0f);
	m_tGlowAnim_FullBody.vEnd_Min_UV		= _vec2(0.0f, 0.0f);
	m_tGlowAnim_FullBody.vEnd_Max_UV		= _vec2(1.0f, 1.0f);
	m_tGlowAnim_Arm.vBeginEnd_Power			= _vec2(0.f, 1.4f);

	m_tGlowAnim_ToNormal.fTotalTime			= 1.f;
	m_tGlowAnim_ToNormal.vBeginEnd_Power	= _vec2(1.f, 0.f);

	Set_SkySphere(L"../../Reference/Resource/Texture/Test/Sky_Hieronymus.jpg");

	return S_OK;
}

_int CHieronymus::Start_Object(const _float & fTimeDelta)
{
	CBossMonster::Start_Object(fTimeDelta);

	m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Hieronymus", m_tUnitInfo.iMaxHp, 20);
	assert(m_pBossHpBar);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, m_pBossHpBar->Get_ObjTag(), m_pBossHpBar);
	static_cast<CUI_Object*>(m_pBossHpBar)->Set_ImageAlpha(0.f);

	Create_Collider(&m_pBoneWP_L, L"Col_WP_L", 0.25f);
	Create_Collider(&m_pBone_Pendant, L"Col_WP_C", 0.25f);
	Create_Collider(&m_pBoneWP_R, L"Col_WP_R", 0.25f);

	Create_Collider(&m_pBone_L_Finger, L"Bip001 L Finger2", 0.25f);
	Create_Collider(&m_pBone_R_Finger, L"Bip001 R Finger2", 0.25f);
	Create_Collider(&m_pBone_Cape_Center_Top, L"Bone_cape_F_01", 0.25f);
	Create_Collider(&m_pBone_Root, L"bone_root", 0.25f);

	// 히에로니무스 전용 랜턴
	m_pGreen_Lantern = CHiero_G_Lantern::Create(m_pGraphicDev, E_OBJTAG::OBJ_CombatItem);
	m_pGreen_Lantern->Set_Pos(Get_Position() + _vec3(2.f, 0.f, 8.f));
	m_pGreen_Lantern->Set_Scale(1.0f, 1.0f, 1.0f);

	// 히에로니무스 전용 랜턴
	m_pRed_Lantern = CHiero_R_Lantern::Create(m_pGraphicDev, E_OBJTAG::OBJ_CombatItem);
	m_pRed_Lantern->Set_Pos(Get_Position() + _vec3(-2.f, 0.f, 8.f));
	m_pRed_Lantern->Set_Scale(1.0f, 1.0f, 1.0f);

	Set_GroggyPercent(25.f);

	for (auto& pChar : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
	{
		pChar->Set_Active(FALSE);
	}
	FSM_FunctionSetting(&CUnit::Begin_Battle, nullptr);

	m_tGodRay.fSize		= 7.1f;
	m_tGodRay.vPosition	= _vec3(-0.21f, 3.5f, -37.f);
	m_tGodRay.vColor	= _vec4(1.f, 1.f, 0.f, 1.f);

	m_iGodRayIndex = CLightMgr::GetInstance()->Create_GodLight_New(_vec3(-0.21f, 3.5f, -37.f), _vec4(1.f, 1.f, 0.f, 1.f), 7.1f);
	CLightMgr::GetInstance()->Set_GodLight_Index_Main(m_iGodRayIndex);

	CScene::Get_MainScene()->OnOff_AutoCam(TRUE);

	CParticleObject* pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Hieronymus_Par_World.particle");
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, pParticle);

	return RETURN_OK;
}

_int CHieronymus::Update_Object(const _float & fTimeDelta)
{
	CBossMonster::Update_Object(fTimeDelta);

	m_pGreen_Lantern->Update_Object(fTimeDelta);
	m_pRed_Lantern->Update_Object(fTimeDelta);

	Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	Animation_Change(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_5))
	{
		Rand_Warn(FALSE);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_6))
	{
		_int iWarn = (_int)m_eRandWarn;
		iWarn++;

		if (iWarn == (_int)E_RandWarn::RandWarn_END)
		{
			iWarn = 0;
		}
		m_eRandWarn = (E_RandWarn)iWarn;
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_N))
	{
		m_bCutViewOnOff = TRUE;
		m_vCutView_Start_Eye	= CCamera::m_pMainCam->Get_CamEye();
		m_vCutView_Start_At		= CCamera::m_pMainCam->Get_CamAt();
		//CScene::Get_MainScene()->OnOff_AutoCam(FALSE);
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_N) && CKeyManager::GetInstance()->Key_Down(KEY_1))
	{
		T_UnitInfo tUnitInfo;
		tUnitInfo.iAttack = 50000;
		Set_Damage(tUnitInfo, E_CHAR_ATKTYPE::ATKTYPE_NORMAL);
	}

	CLightMgr::GetInstance()->Set_GodLight_Info(m_iGodRayIndex, m_tGodRay);

	ViewCut_Update(fTimeDelta);
	return RETURN_OK;
}

_int CHieronymus::LateUpdate_Object(const _float & fTimeDelta)
{
	CBossMonster::LateUpdate_Object(fTimeDelta);

	m_pGreen_Lantern->LateUpdate_Object(fTimeDelta);
	m_pRed_Lantern->LateUpdate_Object(fTimeDelta);

	if (m_pDMeshCom && m_pGlowAnim)
	{
		m_pGlowAnim->fLeftTime += fTimeDelta;
 		_float fPower = m_pGlowAnim->fGet_Power();
		_vec2 vMin = m_pGlowAnim->vGet_Min();
		_vec2 vMax = m_pGlowAnim->vGet_Max();

		m_pDMeshCom->Set_Glow_Option(0, 1.f, _vec3(0.f, 0.f, 0.f), fPower, vMin.x, vMax.x, vMin.y, vMax.y);


		if (m_pGlowAnim->fTotalTime < m_pGlowAnim->fLeftTime)
		{
			m_pGlowAnim->fLeftTime = 0.f;

			m_tGlowAnim_ToNormal.vStart_Min_UV = vMin;
			m_tGlowAnim_ToNormal.vStart_Max_UV = vMax;
			m_tGlowAnim_ToNormal.vEnd_Min_UV = vMin;
			m_tGlowAnim_ToNormal.vEnd_Max_UV = vMax;
			m_pGlowAnim = nullptr;
		}

	}

	if (m_bExBloom)
	{
		vector<CGameObject*>* pVecMap = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
		_float fRatio = m_fExBloom_Timer / 3.f;
		if (fRatio >= 1.f)
		{
			m_bExBloom = FALSE;
			CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = m_fEnd_Bloom;
		}
		m_fExBloom_Timer += fTimeDelta;

		CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = CUtility::Lerp_float(m_fBegin_Bloom, m_fEnd_Bloom, fRatio);

		for (auto& iter : *pVecMap)
		{
			static_cast<CSMesh*>(static_cast<CStageMap*>(iter)->Get_Component(ComTag_SMesh))->Set_Glow_Option(0, fRatio, _vec3(1.f, 1.f, 1.f), 1.f, 0.f, 1.f, 0.f, 1.f);
		}
	}
	else if(m_fExBloom_Timer > 0.f)
	{
		vector<CGameObject*>* pVecMap = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map);
		_float fRatio = m_fExBloom_Timer / 1.f;
		m_fExBloom_Timer -= fTimeDelta;
		CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = CUtility::Lerp_float(m_fBegin_Bloom, m_fEnd_Bloom, fRatio);

		for (auto& iter : *pVecMap)
		{
			static_cast<CSMesh*>(static_cast<CStageMap*>(iter)->Get_Component(ComTag_SMesh))->Set_Glow_Option(0, fRatio, _vec3(1.f, 1.f, 1.f), 1.f, 0.f, 1.f, 0.f, 1.f);
		}
	}

	if (m_bPhaseMiddle)
	{
		_float fRatio = m_fPhaseMiddle_Timer / 1.f;
		if (fRatio >= 1.f)
		{
			m_bPhaseMiddle = FALSE;
			m_fPhaseMiddle_Timer = 0.f;
			CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = m_fEnd_Bloom;
		}
		m_fPhaseMiddle_Timer += fTimeDelta;
		CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = CUtility::Lerp_float(m_fBegin_Middle, m_fEnd_Middle, fRatio);
	}


	return RETURN_OK;
}

void CHieronymus::Render_Object(_int iPass)
{
	CBossMonster::Render_Object(iPass);

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		if (m_pBoneWP_L)
		{
			m_pBoneWP_L->Render_Collider(m_pShaderCom->Get_EffectHandle());
		}
		if (m_pBone_Pendant)
		{
			m_pBone_Pendant->Render_Collider(m_pShaderCom->Get_EffectHandle());
		}
		if (m_pBoneWP_R)
		{
			m_pBoneWP_R->Render_Collider(m_pShaderCom->Get_EffectHandle());
		}
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CHieronymus::Set_Constantable()
{
	CBossMonster::Set_Constantable();
}

void CHieronymus::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;

		AnimationStart_Event();
	}
}

void CHieronymus::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (!m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
	}

	if (nullptr == m_pTarget || m_pTarget->Get_UnitDead())
	{
		Find_Target_InRange(eTargetLayer, eObjTag);
	}

	if (FALSE == m_bStateLock)
	{
		FSM_FunctionSetting(&CUnit::Idle, &CUnit::Idle);
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
}

void CHieronymus::Begin_Battle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::OP;

	m_fGodRayTimer += fDeltaTime;

	if (3.f >= m_fGodRayTimer)
	{
		_float fRatio = CUtility::Clamp_float((m_fGodRayTimer) * 0.33f, 0.f, 1.f);

		m_tGodRay.vPosition.y = CUtility::Lerp_float(3.5f, 4.5f, fRatio);
	}
	if (7.f <= m_fGodRayTimer)
	{
		_float fRatio = CUtility::Clamp_float((m_fGodRayTimer - 7.f) * 0.5f, 0.f, 1.f);

		m_tGodRay.fSize = CUtility::Lerp_float(7.1f, 3.5f, fRatio);
	}
	if (10.f <= m_fGodRayTimer)
	{
		m_tGodRay.fSize = 0.f;
	}

	if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
	{
		for (auto& pChar : CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers())
		{
			pChar->Set_Active(TRUE);
			pChar->Set_UseGravity(TRUE);
		}

		FSM_FunctionSetting(&CUnit::Idle, nullptr);
	}
	else
	{
		m_eCurrentAction = m_eMainAction;
	}
}

// Idle이 기본상태고 SkillGage (노란색 3개 차는거) 꽉차면 ExSkill하라는 거고
// 그외에는 Gage가 5넘었을때 랜덤한 기본공격
void CHieronymus::Idle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Idle;

	m_fGage += fDeltaTime;

	m_fGodRayTimer = 0.f;
	m_tGodRay.fSize = 0.f;
	m_tGodRay.vPosition = _vec3(0.49f, 3.f, -33.9f);

	if (m_fExSkill2_MaxGage <= m_fExSkill2_Gage /*&& 3 <= m_iPhase*/)
	{
		FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
	}
	else if (m_fGage >= 5.f)
	{
		_int iRand = CWall512::GetInstance()->GetValue(0, 100);

		if (iRand < 33)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
		}
		else if (iRand < 66)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
		}
		else
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_3, nullptr);
		}
	}

	m_eCurrentAction = m_eMainAction;
}

void CHieronymus::Dead(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Vital_Death;
	m_pTarget = nullptr;
}

// Exs04
void CHieronymus::ExSkill(_float fDeltaTime)
{
	m_fGodRayTimer += fDeltaTime;
	m_tGodRay.fSize = CUtility::Lerp_float(0.f, 2.1f, m_fGodRayTimer * 0.5f);
	if (m_eCurrentAction != (E_ACTION)E_Hieronymus_Anim::PC && m_eCurrentAction != (E_ACTION)E_Hieronymus_Anim::Exs04)
	{
		m_eMainAction = (E_ACTION)E_Hieronymus_Anim::PC;
	}
	else if (m_eCurrentAction == (E_ACTION)E_Hieronymus_Anim::Exs04)
	{
		if (Is_AnimFinished() && m_eMainAction == m_eCurrentAction)
		{
			m_bExBloom = FALSE;
			CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 1.f;
			FSM_FunctionSetting(&CUnit::Idle, nullptr);
		}
		else
		{
			m_eCurrentAction = m_eMainAction;
		}
	}
	else if (m_eCurrentAction == (E_ACTION)E_Hieronymus_Anim::PC && Is_AnimFinished())
	{
		m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Exs04;
	}

	m_bIsExSkill = TRUE;
	m_fExSkill2_Gage = 0.f;

	m_eCurrentAction = m_eMainAction;

}

// Exs01
void CHieronymus::Boss_Attack_1(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Exs01;

	Boss_FSM_Check(&CUnit::Idle);
}

// Exs02
void CHieronymus::Boss_Attack_2(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Exs02;

	Boss_FSM_Check(&CUnit::Idle);
}

// Exs03
void CHieronymus::Boss_Attack_3(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Exs04;
	m_bIsExSkill = FALSE;
	Boss_FSM_Check(&CUnit::Idle);
}

void CHieronymus::Boss_Groggy(_float fDeltaTime)
{
	CBossMonster::Boss_Groggy(fDeltaTime);

	m_eMainAction = (E_ACTION)E_Hieronymus_Anim::Groggy;
	if (m_iPhase == 3)
	{
		m_bPhaseMiddle = TRUE;
	}
	Boss_FSM_Check(&CUnit::Idle);
}

void CHieronymus::Update_Collider(const _float & fTimeDelta)
{
	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
	Collider_SetMatrix(m_pWeaponCollider, m_szWeaponBoneName);
	Collider_SetMatrix(m_pBoneWP_L, m_szWP_L);
	Collider_SetMatrix(m_pBone_Pendant, m_szWP_Pendant);
	Collider_SetMatrix(m_pBoneWP_R, m_szWP_R);

	Collider_SetMatrix(m_pBone_L_Finger, m_szBone_L_Finger);
	Collider_SetMatrix(m_pBone_R_Finger, m_szBone_R_Finger);
	Collider_SetMatrix(m_pBone_Cape_Center_Top, m_szBone_Cape_Center_Top);
	Collider_SetMatrix(m_pBone_Root, m_szBone_Root);
}

// Exs01
void CHieronymus::Trigger_Attack_1()
{
	Ex1_Pattern(m_iPatternRand, FALSE);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_Weapon_Hit.wav", 1.f);
}

// Exs02
void CHieronymus::Trigger_Attack_2()
{
	Ex2_Pattern(m_iPatternRand, FALSE);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_Weapon_Hit.wav", 1.f);
}

// Exs04
void CHieronymus::Trigger_ExSkill()
{
	if (m_bIsExSkill)
	{
		// 충격파 색에 맞는 랜턴 On
		if (m_iWitchLantern == 0)
		{
			m_pGreen_Lantern->Set_ActiveLantern(TRUE);
		}
		else
		{
			m_pRed_Lantern->Set_ActiveLantern(TRUE);
		}
		Ex4_Pattern(m_iPatternRand, FALSE);
	}
	else
	{
		Ex3_Pattern(m_iPatternRand, FALSE);
	}
}

void CHieronymus::AnimationStart_Event()
{
	m_iPatternRand = CWall512::GetInstance()->GetValue(0, 100);
	if ((E_ACTION)E_Hieronymus_Anim::OP == m_eCurrentAction)
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_Op.wav", 1.f);
		_float fAngle = Get_Angle().y;
		CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Hieronymus.camdat", L"Hieronymus", Get_Position(), fAngle);
	}
	else if ((E_ACTION)E_Hieronymus_Anim::Idle == m_eCurrentAction)
	{
		m_fGage = 0.f;
		m_pGlowAnim = &m_tGlowAnim_ToNormal;
	}
	else if ((E_ACTION)E_Hieronymus_Anim::Exs01 == m_eCurrentAction)
	{
		m_pGlowAnim = &m_tGlowAnim_Arm;
		Ex1_Pattern(m_iPatternRand, TRUE);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX1_Magiccircle.wav", 1.f);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX1_Weapon1.wav", 1.f);
	}
	else if ((E_ACTION)E_Hieronymus_Anim::Exs02 == m_eCurrentAction)
	{
		m_pGlowAnim = &m_tGlowAnim_Arm;
		Ex2_Pattern(m_iPatternRand, TRUE);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX2_Magiccircle.wav", 1.f);
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX2_Weapon1.wav", 1.f);
	}
	else if ((E_ACTION)E_Hieronymus_Anim::PC == m_eCurrentAction)
	{
		// 충격파 생성
		m_iWitchLantern = CWall512::GetInstance()->GetValue(0, 2);
		if (m_iWitchLantern == 0)
		{
			CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_GL_00.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
		}
		else
		{
			CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_RL_00.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
		}

		CUI_Manager::GetInstance()->Set_Announcement(L"히에로니무스가 강력한 공격을 준비합니다.", 3.f);
	}
	else if ((E_ACTION)E_Hieronymus_Anim::Exs04 == m_eCurrentAction)
	{
		if (m_bIsExSkill)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX4_Cast.wav", 1.f);
			Ex4_Pattern(m_iPatternRand, TRUE);
			m_pGlowAnim = &m_tGlowAnim_FullBody;
		}
		else
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX3_Magiccircle.wav", 1.f);
			Ex3_Pattern(m_iPatternRand, TRUE);
			m_pGlowAnim = &m_tGlowAnim_Arm;
		}
	}
	else if ((E_ACTION)E_Hieronymus_Anim::Groggy == m_eCurrentAction)
	{
		if (m_iPhase == 2)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_Phase.wav", 1.f);
		}
		else
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_Groggy.wav", 1.f);
		}
		CUI_Manager::GetInstance()->Set_Announcement(L"히에로니무스가 그로기 상태에 빠집니다.", 3.f);
		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
	}
}

void CHieronymus::Ex1_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 70)
	{
		if (bStart)
		{
			Hieronymus_Impact_0();
		}
		else
		{
			Hieronymus_Impact_1();
		}
	}
	else if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
			Rand_Warn(TRUE);
		}
	}
}

void CHieronymus::Ex2_Pattern(_int iRand, _bool bStart)
{
		if (iRand < 70)
		{
			if (bStart)
			{
				_vec3 vLeft = m_pBoneWP_L->GetPos();
				vLeft.y *= 0.5f;
				CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Aura_Red.skill", E_LAYERTAG::Effect, vLeft, VEC_ZERO, VEC_ONE);
			}
			else
			{
				_vec3 vLeft = m_pBoneWP_L->GetPos();
				vLeft.y = 0.1f;
				CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Staff_0.skill", E_LAYERTAG::Effect, vLeft, VEC_ZERO, VEC_ONE * 2.f);
				Invoke_Add(0.3f, bind(&CHieronymus::Hieronymus_Explode_0, this));

			}
		}
		else if (iRand < 100)
		{
			if (bStart)
			{
			}
			else
			{
				Rand_Warn(TRUE);
			}
		}
}

void CHieronymus::Ex3_Pattern(_int iRand, _bool bStart)
{
		if (iRand < 70)
		{
			if (bStart)
			{

				Invoke_Add(3.0f, bind(&CHieronymus::Hieronymus_Staff_1, this));
			}
			else
			{
				CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Impact_2.skill", E_LAYERTAG::Effect, m_pTarget->Get_Position(), VEC_ZERO, VEC_ONE * 4.f);
				Invoke_Add(2.5f, bind(&CHieronymus::Hieronymus_Impact_2_Sound, this));
			}
		}
		else if (iRand < 100)
		{
			if (bStart)
			{

			}
			else
			{
				Rand_Warn(TRUE);

			}
		}
}

void CHieronymus::Ex4_Pattern(_int iRand, _bool bStart)
{
	if (m_iPhase < 3)
	{
		if (bStart)
		{
			CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Aura_Zone.skill", E_LAYERTAG::Effect, m_pBone_Root->GetPos(), VEC_ZERO, VEC_ONE * 4.f);
		}
		else
		{
			for (_int i = 0; i < 8; ++i)
			{
				Invoke_Add(i * 0.1f, bind(&CHieronymus::Create_BezierEx, this, i), i);
			}
		}
	}
	else
	{
		if (bStart)
		{
		}
		else
		{
			m_bExBloom = TRUE;
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX4_Explosion.wav", 1.f);
			Invoke_Add(3.0f, bind(&CHieronymus::Hieronymus_Ex_Bomb, this));
		}
	}
}

// 랜덤한 캐릭터를 향하는 베지어 함수
void CHieronymus::Test_Bullet_6()
{
	// 첫 줄
	vector<_vec3> vStrikersPos = CGameManager::GetInstance()->Get_AliveStrikersPosition();

	
	for (_int i = 0; i < (_int)vStrikersPos.size(); ++i)
	{
		_vec3 vStartPos = VEC_ZERO;
		switch (CWall512::GetInstance()->GetValue(0, 3))
		{
			case 0:		vStartPos = m_pBone_Pendant->GetPos(); break;
			case 1:		vStartPos = m_pBone_L_Finger->GetPos(); break;
			case 2:		vStartPos = m_pBone_R_Finger->GetPos(); break;
			default:	vStartPos = m_pBone_Cape_Center_Top->GetPos(); break;
		}
		list<CBullet_Bezier*>* pBezierList = CBullet_Bezier::Create_Bezier(10, 1, 0.6f, 1.f, 0.f, 3.2f,
			vStartPos, vStrikersPos[i],
				_vec3(5.f, 3.5f, 5.f), _vec3(5.f, 3.f, 5.f), m_pGraphicDev, m_tUnitInfo, m_tMonsterInfo.eAtkType, m_eTargetLayerTag);

		for (auto& iter : (*pBezierList))
		{
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, iter);
		}

		Safe_Delete(pBezierList);
	}
}

// 스킬 소환 Invoke용도
void CHieronymus::Hieronymus_Explode_0()
{
	
	auto vStrikersPos = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
	for (_int i = 0; i < (_int)vStrikersPos.size(); ++i)
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Explode_0.skill", E_LAYERTAG::Effect, vStrikersPos[i]->Get_Position(), VEC_ZERO, VEC_ONE * 2.f);
		vStrikersPos[i]->Set_Damage(m_tUnitInfo, m_tMonsterInfo.eAtkType);
	}
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX2_Hit_Explosion.wav", 1.f);
}

// 스킬 소환 Invoke용도
void CHieronymus::Hieronymus_Impact_0()
{
	_vec3 vCenter = m_pSphereCollider->GetPos();
	vCenter.y = 0.1f;
	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Impact_0.skill", E_LAYERTAG::Effect, vCenter, VEC_ZERO, VEC_ONE * 4.f);
}

// 스킬 소환 Invoke용도
void CHieronymus::Hieronymus_Impact_1()
{
	if (m_pTarget) // nullptr로 터저서 은호가 주석처리함
	{
		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Impact_1.skill", E_LAYERTAG::Effect, m_pTarget->Get_Position(), VEC_ZERO, VEC_ONE * 2.f);
		m_pTarget->Set_Damage(m_tUnitInfo, m_tMonsterInfo.eAtkType);
	}
}

void CHieronymus::Hieronymus_Impact_2_Sound()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX4_Explosion.wav", 1.f);
}

void CHieronymus::Hieronymus_Staff_1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Hieronymus_EX3_Aura.wav", 1.f);
	_vec3 vRight = m_pBone_R_Finger->GetPos();
	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Staff_1.skill", E_LAYERTAG::Effect, vRight, VEC_ZERO, VEC_ONE * 3.f);

	_vec3 vLeft = m_pBone_L_Finger->GetPos();
	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Staff_1.skill", E_LAYERTAG::Effect, vLeft, VEC_ZERO, VEC_ONE * 3.f);
}

void CHieronymus::Hieronymus_Ex_Bomb()
{
	vector<CCharacter*> vecPlayer = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();

	_int iOldDamage = m_tUnitInfo.iAttack;
	m_tUnitInfo.iAttack = 100000;
	for (auto& iter : vecPlayer)
	{
		iter->Set_Damage(m_tUnitInfo, m_tMonsterInfo.eAtkType);
	}
	m_tUnitInfo.iAttack = iOldDamage;

	CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Ex_Bomb.skill", E_LAYERTAG::Effect, m_pBone_Root->GetPos(), VEC_ZERO, VEC_ONE);
}

// 무작위 탄막패턴 소환하기
void CHieronymus::Rand_Warn(_bool bRand)
{
	Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

	if (nullptr == m_pTarget)
	{
		return;
	}

	if (bRand)
	{
		_int iRand = CWall512::GetInstance()->GetValue(0, (_int)E_RandWarn::RandWarn_END);
		m_eRandWarn = E_RandWarn(iRand);
	}

	CUI_Manager::GetInstance()->Set_Announcement(L"Rand_Warn 발동 : " + to_wstring((_int)m_eRandWarn), 2.f);
	CUtility::Print_Wstring_DMode(L"Rand_Warn 발동 : " + to_wstring((_int)m_eRandWarn));
	_int iWarnTypeRand = CWall512::GetInstance()->GetValue(0, (_int)E_WarnType::WarnEnd);

	_vec3 vCenter		= (m_pTarget->Get_Position() + Get_Position()) * 0.5f;
	_vec3 vCenterCape	= m_pBone_Cape_Center_Top->GetPos();
	_vec3 vRight		= m_pBoneWP_R->GetPos();
	_vec3 vLeft			= m_pBoneWP_L->GetPos();
	_float fAddY		= 0.001f;

	vCenter.y = 0.201f;
	vCenterCape.y = 0.201f;
	vRight.y = 0.201f;
	vLeft.y = 0.201f;

	CWarning* pWarn = nullptr;
	switch (m_eRandWarn)
	{
	case E_RandWarn::Rand_Bomb:				// 그냥 무작위 보라색
	{
		for (_int i = 0; i < 25; ++i)
		{
			fAddY += 0.001f;
			pWarn = CWarning::Create(m_pGraphicDev);
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Blind, 3.f));
			_vec3 vAddPos = _vec3(CWall512::GetInstance()->GetFloatValue(-5.f, 5.f), 0.f, CWall512::GetInstance()->GetFloatValue(-10.f, 10.f));
			vAddPos.y += fAddY;
			pWarn->Set_Warning(E_WarnType::Circle_OutDot, E_WarnType::Circle_InsideFill, vCenter + vAddPos, 4.f, CWall512::GetInstance()->GetFloatValue(0.f, 3.f), 4.f, D3DXCOLOR(C_Purple));
			pWarn->Set_Damage(150);
			pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Posion_00.skill", VEC_ZERO, VEC_ONE * 4.f);
			pWarn->Set_SoundName(L"SFX_Electric_JSD_Shock_02.wav", 1.f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn);
		}
	}
		break;
	case E_RandWarn::Straight_Circles:		// 나선 수리검
	{
		for (_int i = 0; i < 3; ++i)
		{
			pWarn = CWarning::Create(m_pGraphicDev);
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Stunned, 3.f));
			fAddY += 0.001f;
			vCenter.y = 0.201f;
			
			pWarn->Set_Warning(E_WarnType::Circle_Wave, E_WarnType::Circle_Wave, vCenter, 1.5f, 0.f, 3.5f);

			pWarn->Set_Damage(150);
			pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Flower_00.skill", VEC_ZERO, VEC_ONE * 0.7f);
			pWarn->Set_SoundName(L"SFX_Suzumi_Skill_EX_Explosion.wav", 1.f);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarn, 0.f, 15.f, 6,   i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Yellow, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarn, 90.f, 15.f, 6,  i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Yellow, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarn, 180.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Yellow, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarn, 270.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Yellow, C_White);
			Safe_Release(pWarn);
		}
	}
	break;
	case E_RandWarn::_3:					// 5개 앞으로 진행하면서 오는거?
	{
		pWarn = CWarning::Create(m_pGraphicDev);
		pWarn->Set_Damage(150);
		pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Lightning.skill", VEC_ZERO, VEC_ONE * 1.f);
		pWarn->Set_SoundName(L"SFX_Electric_JSD_Shock_02.wav", 1.f);
		pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Stunned, 3.f));
		pWarn->Set_Warning(E_WarnType::Circle_OutDot, E_WarnType::Circle_Wave, Get_Position(), 2.f, 0.f, 2.f);
		CWarning_Mgr::GetInstance()->Create_Line_OneWay(pWarn, 270.f, 1.5f, 8, 1.0f, 0.25f, Get_Position() + _vec3(+4.f, 0.f, 0.f), C_Yellow, C_Green);
		CWarning_Mgr::GetInstance()->Create_Line_OneWay(pWarn, 270.f, 1.5f, 8, 0.5f, 0.25f, Get_Position() + _vec3(+2.f, 0.f, 0.f), C_Yellow, C_Green);
		CWarning_Mgr::GetInstance()->Create_Line_OneWay(pWarn, 270.f, 1.5f, 8, 0.0f, 0.25f, Get_Position() + _vec3(+0.f, 0.f, 0.f), C_Yellow, C_Green);
		CWarning_Mgr::GetInstance()->Create_Line_OneWay(pWarn, 270.f, 1.5f, 8, 0.5f, 0.25f, Get_Position() + _vec3(-2.f, 0.f, 0.f), C_Yellow, C_Green);
		CWarning_Mgr::GetInstance()->Create_Line_OneWay(pWarn, 270.f, 1.5f, 8, 1.0f, 0.25f, Get_Position() + _vec3(-4.f, 0.f, 0.f), C_Yellow, C_Green);
		Safe_Release(pWarn);
	}
		break;
	case E_RandWarn::Bezier_Targetting:
		Test_Bullet_6();
		break;
	case E_RandWarn::Line3:
		
		for (_int i = 1; i < 7; i++)
		{
			if (i > 4)
			{
				Invoke_Add(i * 1.f, bind(&CHieronymus::Lava_Invoke, this));
				Invoke_Add(i * 1.f, bind(&CHieronymus::Lightning_Invoke, this));
			}
			else if (i % 2 == 0)
			{
				Invoke_Add(i * 1.f, bind(&CHieronymus::Lava_Invoke, this));
			}
			else
			{
				Invoke_Add(i * 1.f, bind(&CHieronymus::Lightning_Invoke, this));
			}
		}
		break;
	default:
		break;
	}

	Safe_Release(pWarn);
}

void CHieronymus::Lava_Invoke()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Explosion_Bomb_Hit.wav", 1.f);
	_int iSize = (_int)CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers().size(); 
	_vec3 vPos = VEC_ZERO;
	if (iSize != 0)
	{
		CUnit* pRandPlayer = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers()[CWall512::GetInstance()->GetValue(0, iSize)];
		vPos = pRandPlayer->Get_Position();
	}
	else
	{
		vPos = Get_Position();
	}

	CWarning* pWarn = CWarning::Create(m_pGraphicDev);
	pWarn->Set_Damage(150);
	pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Burn, 3.f));
	pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Burn, 5.f));
	pWarn->Set_Warning(E_WarnType::Circle_OutDot, E_WarnType::Circle_Lava, Get_Position(), 2.f, 0.f, 4.f);
	pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Lava.skill", VEC_ZERO, VEC_ONE * 2.2f);
	pWarn->Set_SoundName(L"SFX_Explosion_Bomb_Hit.wav", 1.f);

	_float fRandAngle = CWall512::GetInstance()->GetFloatValue(0.f, 180.f);
	vPos.y = 0.25f;
	vPos.x += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
	vPos.z += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
	CWarning_Mgr::GetInstance()->Create_Line_TwoWay(pWarn, fRandAngle, 1.5f, 8, 0.0f, 0.1f, vPos, C_White, C_White);
	Safe_Release(pWarn);
}

void CHieronymus::Lightning_Invoke()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Electric_JSD_Shock_02.wav", 1.f);
	_int iSize = (_int)CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers().size();
	_vec3 vPos = VEC_ZERO;
	if (iSize != 0)
	{
		CUnit* pRandPlayer = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers()[CWall512::GetInstance()->GetValue(0, iSize)];
		vPos = pRandPlayer->Get_Position();
	}
	else
	{
		vPos = Get_Position();
	}

	CWarning* pWarn = CWarning::Create(m_pGraphicDev);
	pWarn->Set_Damage(150);
	pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Burn, 3.f));
	pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Burn, 5.f));
	pWarn->Set_Warning(E_WarnType::Circle_OutDot, E_WarnType::Circle_Lightning, Get_Position(), 2.f, 0.f, 4.f);
	pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Lightning.skill", VEC_ZERO, VEC_ONE * 1.f);
	pWarn->Set_SoundName(L"SFX_Electric_JSD_Shock_02.wav", 1.f);

	_float fRandAngle = CWall512::GetInstance()->GetFloatValue(0.f, 180.f);
	vPos.y = 0.25f;
	vPos.x += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
	vPos.z += CWall512::GetInstance()->GetFloatValue(-1.f, 1.f);
	CWarning_Mgr::GetInstance()->Create_Line_TwoWay(pWarn, fRandAngle, 1.5f, 8, 0.0f, 0.1f, vPos, C_White, C_White);
	Safe_Release(pWarn);
}

void CHieronymus::Create_BezierEx(_int i)
{
	for (_int j = 0; j < 8; j++)
	{
		_vec3 vStartPos = m_pBone_L_Finger->GetPos();
		_vec3 vPos = Get_Position() + _vec3(CWall512::GetInstance()->GetFloatValue(-5.f, 5.f), 0.f, 2.f + i * 1.f);

		if (j < 4)
		{
			vStartPos = m_pBone_R_Finger->GetPos();
		}

		list<CBullet_Bezier*>* pBezierList = CBullet_Bezier::Create_Bezier(1, 1, 1.3f, 1.7f, i * 0.3f, i * 0.3f + 0.15f,
			vStartPos, vPos,
			_vec3(5.f, 3.5f, 5.f), _vec3(5.f, 3.f, 5.f), m_pGraphicDev, m_tUnitInfo, m_tMonsterInfo.eAtkType, m_eTargetLayerTag);

		for (auto& iter : (*pBezierList))
		{
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, iter);

			CWarning* pWarn = CWarning::Create(m_pGraphicDev);
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::CC_Paralysis, 3.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_ATK, 10.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_ATK, 15.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_ATK, 20.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Bleed, 10.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Bleed, 15.f));
			pWarn->Add_CombatStatus(T_CombatStatus(E_Combat_Status::DB_Bleed, 20.f));
			pWarn->Set_Damage(200);
			pWarn->Set_AfterSkillName(L"Hieronymus_Warn_Lava.skill", VEC_ZERO, VEC_ONE * 1.f);
			pWarn->Set_Warning(E_WarnType::Circle_OutDot, E_WarnType::Circle_Magic, vPos, iter->Get_TotalTime(), iter->Get_DelayTime(), 3.5f, D3DXCOLOR(C_Purple));

			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn);
		}
		Safe_Delete(pBezierList);
	}
}

void CHieronymus::ViewCut_Update(const _float& fTimeDelta)
{
	if (FALSE == m_bCutViewOnOff)
		return;

	m_fCutViewCoolTime += fTimeDelta;

	_float fCutViewOnce = m_fCutViewTotalTime / m_iCutViewTime;
	_float fRatio = 1.f - (m_fCutViewCoolTime / fCutViewOnce);
	fRatio = CUtility::Clamp_float(fRatio, 0.f, 1.f);
	fRatio = CUtility::LerpEase_float(E_EASE::OutCubic, fRatio);


	_matrix	matView, matProj, matMainView;
	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	_vec3 vMainCamEye	= m_vCutView_Start_Eye;
	_vec3 vMainCamAt	= m_vCutView_Start_At;
	_vec3 vEye			= vMainCamEye;
	_vec3 vAt			= vMainCamAt;
	_vec3 vUp			= VEC_UP;

	_vec3 vCamLook		= vMainCamAt - vMainCamEye;
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	_vec3 vCamRight		= *D3DXVec3Cross(&vCamRight, &VEC_UP, &vCamLook);
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	_vec3 vCamUp		= *D3DXVec3Cross(&vCamUp, &vCamLook, &vCamRight);
	D3DXVec3Normalize(&vCamUp, &vCamUp);

	if (fCutViewOnce <= m_fCutViewCoolTime)
	{
		T_ViewCut tViewCut = T_ViewCut();

		tViewCut.fTimeMax	= fCutViewOnce;
		tViewCut.fTime		= tViewCut.fTimeMax * 0.5f;

		if (m_iCutViewStack % 3 == 0)
		{
			tViewCut.fDis = CWall512::GetInstance()->GetFloatValue(-3.f, -2.f);
		}
		else if (m_iCutViewStack % 3 == 1)
		{
			tViewCut.fDis = CWall512::GetInstance()->GetFloatValue(-0.2f, 0.2f);
		}
		else if (m_iCutViewStack % 3 == 2)
		{
			tViewCut.fDis = CWall512::GetInstance()->GetFloatValue(2.f, 3.f);
		}
		m_fCutViewTilt = tViewCut.fDis;

		m_iCutViewStack++;
		m_fCutViewCoolTime = 0.f;

		if (m_iCutViewStack > m_iCutViewTime)
		{
			m_iCutViewStack = 0;
			m_bCutViewOnOff = FALSE;
			CScene::Get_MainScene()->OnOff_AutoCam(TRUE);
			m_fCutViewCoolTime = 10.f;
			return;
		}
		else
		{
			_vec2 vUiPos = _vec2(WINCX * 0.5f, WINCY * 0.5f);
			_float fAngle = 1.f / m_fCutViewTilt * 360.f;
			CUISkillObject* pUiSkillObject = CSkill_Manager::GetInstance()->SetUISkill(L"Hieronymus_CutView.skill", E_LAYERTAG::Effect, vUiPos, 0.f, 0.05f);
			static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().fLifeTime = fCutViewOnce * (1.f + m_iCutViewTime - m_iCutViewStack);
			static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_Start.z = fAngle;
			static_cast<CEffectObject*>(pUiSkillObject->Get_vecSkillObjects()->front())->Get_Effect_Info().vRot_End.z = fAngle;
			CRenderer::GetInstance()->Set_ViewCut_Fix(fCutViewOnce, tViewCut);
		}
	}

	_vec3 vDir = vCamRight * -m_fCutViewTilt + vCamUp * 1.f;
	D3DXVec3Normalize(&vDir, &vDir);
	vDir *= 3.f;

	vEye += vDir * fRatio;
	vAt += vDir * fRatio;
	vMainCamEye -= vDir * fRatio;
	vMainCamAt -= vDir * fRatio;

	// 컷뷰 ViewMatrix세팅
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	CRenderer::GetInstance()->Set_ViewCut_Fix_Matrix(&matView, &matProj);

	// 메인 ViewMatrix세팅
	D3DXMatrixLookAtLH(&matMainView, &vMainCamEye, &vMainCamAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matMainView);
}

void CHieronymus::Set_SkySphere(wstring wstrSkyTexFullPath)
{
	if (!m_pSkySphere)
	{
		m_pSkySphere = CSkyDome::Create(m_pGraphicDev, wstrSkyTexFullPath);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_BackGround, m_pSkySphere);
	}
	else
	{
		static_cast<CSkyDome*>(m_pSkySphere)->Set_SkyTex(wstrSkyTexFullPath);
	}
}