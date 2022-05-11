#include "Binah.h"
#include "Binah_Halo.h"
#include "Binah_Environment.h"
#include "Hod.h"
#include "Boss_HpBar.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "CollisionManager.h"
#include "GameManager.h"
#include "Warning.h"
#include "Warning_Mgr.h"

CBinah::CBinah(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CBossMonster(pGraphicDev, eObjTag, L"Binah")
{
}

CBinah::CBinah(const CBinah & rhs)
	:CBossMonster(rhs)
{
}

CBinah::~CBinah()
{
}

CBinah * CBinah::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CBinah* pInstance = new CBinah(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinah::Free()
{
	CBossMonster::Free();

	CScene::Get_MainScene()->Set_Binah_Cam(false);

	for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			pChar->Set_AddRange(-500);
		}
	}

	if (m_pLaser != nullptr)
	{
		if (m_pLaser->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pLaser);
			m_pLaser = nullptr;
			m_vecColObject.clear();
		}
	}

	if (m_pEXLaser != nullptr)
	{
		if (m_pEXLaser->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pEXLaser);
			m_pEXLaser = nullptr;
		}
	}

	if (m_pQuake != nullptr)
	{
		if (m_pQuake->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pQuake);
			m_pQuake = nullptr;
		}
	}

	if (m_pHalo != nullptr)
	{
		if (m_pHalo->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pHalo);
			m_pHalo = nullptr;
		}
	}

	if (m_pBossHpBar != nullptr)
	{
		if (m_pBossHpBar->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pBossHpBar);
			m_pBossHpBar = nullptr;
		}
	}
}

HRESULT CBinah::Ready_Object()
{
	CBossMonster::Ready_Object();
	
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_보스전_비나.mp3", 0.4f);
	
	m_tUnitInfo.iAttack = 150;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 300000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 999999;
	m_tUnitInfo.iStability;

	Ready_Glow();
	Ready_HDR();

	CBinah_Tail* pBinah_Left = CBinah_Tail::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Trail);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Environment, pBinah_Left->Get_ObjTag(), pBinah_Left);
	pBinah_Left->Set_Pos(_vec3(6.f, 0.f, -0.5f));
	pBinah_Left->Set_Rotation(_vec3(0.f, 0.5f, -3.f));
	pBinah_Left->Set_Scale(_vec3(2.f, 2.f, 2.f));
	CBinah_Tail* pBinah_Right = CBinah_Tail::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Trail);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Environment, pBinah_Right->Get_ObjTag(), pBinah_Right);
	pBinah_Right->Set_Pos(_vec3(0.f, 0.f, -3.f));
	pBinah_Right->Set_Rotation(_vec3(30.f, 210.f, -30.f));
	pBinah_Right->Set_Scale(_vec3(2.f, 2.f, 2.f));

	m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Binah", m_tUnitInfo.iMaxHp, 50);
	assert(m_pBossHpBar);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, m_pBossHpBar->Get_ObjTag(), m_pBossHpBar);
	static_cast<CUI_Object*>(m_pBossHpBar)->Set_ImageAlpha(0.f);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_PosX(28);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_PosY(20);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_SizeX(80);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_SizeY(20);

	return S_OK;
}

_int CBinah::Start_Object(const _float & fTimeDelta)
{
	CBossMonster::Start_Object(fTimeDelta);

	for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			pChar->Set_AddRange(500);
		}
	}

	m_szBodyBoneName = "Head_cap_01";
	m_szWeaponBoneName = "bone_Cannon_mo_01";
	Create_Collider(&m_pBoneEye_L1, L"Col_Eye_L1", 0.25f);
	Create_Collider(&m_pBoneEye_L2, L"Col_Eye_L2", 0.25f);
	Create_Collider(&m_pBoneEye_R1, L"Col_Eye_L3", 0.25f);
	Create_Collider(&m_pBoneEye_R2, L"Col_Eye_L4", 0.25f);
	Create_Collider(&m_pBone_Canon_01, L"Col_Canon_01", 0.25f);
	Create_Collider(&m_pBone_Canon_02, L"Col_Canon_02", 0.25f);
	Create_Collider(&m_pBone_Canon_03, L"Col_Canon_03", 0.25f);
	Create_Collider(&m_pBone_Canon_04, L"Col_Canon_04", 0.25f);
	Create_Collider(&m_pBone_Canon_05, L"Col_Canon_05", 0.25f);
	Create_Collider(&m_pBone_Canon_06, L"Col_Canon_06", 0.25f);
	Create_Collider(&m_pHead, L"Head", 0.25f);
	Create_Collider(&m_pBone_Root, L"bone_root", 0.25f);

	CScene::Get_MainScene()->Set_Binah_Cam(true);
	CScene::Get_MainScene()->Set_AutoCam_Ratio(0.1f);
	CScene::Get_MainScene()->Set_Fit_Camera_Option(6.05f, 12.75f, -0.35f, -1.7f, 0.3f);
	CScene::Get_MainScene()->Set_Fit_Camera_Interval(-6.f, -6.f, -4.f, 4.f, -4.f, 4.f);
	CActionCam_Mgr::GetInstance()->Load_ActionCam(L"Binah.camdat");
	m_pDMeshCom->Set_AnimSpeed(0.7f);

	Set_Scale(_vec3(1.5f, 1.5f, 1.5f));
	Set_Pos(_vec3(4.f, 0.5f, -2.5f));
	Set_Rotation(_vec3(0.f, 65.f, 0.f));
	Set_GroggyPercent(40.f);
	
	return S_OK;
}

_int CBinah::Update_Object(const _float & fTimeDelta)
{
	CBossMonster::Update_Object(fTimeDelta);

	if (CKeyManager::GetInstance()->Key_Down(KEY_F2))
	{
		Reset_HDR();

		CScene::Get_MainScene()->Set_Binah_Cam(false);

		for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
		{
			if (pChar)
			{
				pChar->Set_AddRange(-500);
			}
		}
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_F3))
	{
		Reset_HDR();

		CScene::Get_MainScene()->Set_Binah_Cam(false);

		for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
		{
			if (pChar)
			{
				pChar->Set_AddRange(-500);
			}
		}
	}

	if (m_pBossHpBar->Get_Hp() < 150000)
	{
		if (m_bCreateHod)
		{
			Set_Active(false);
			m_pBossHpBar->Set_Active(false);
			Safe_Release(m_pGameObj_Halo);
			m_pGameObj_Halo = nullptr;
			m_bCreateHod = false;
		}
	}

	if (m_tUnitInfo.iMaxHp <= 0)
	{
		Reset_HDR();
		Set_Dead(true);
	}

	m_pSphereCollider->SetRadius(0.1f);

	Skill_Shake_Time(fTimeDelta);

	m_fActiveCharTime += fTimeDelta;
	m_fIntroNameTime += fTimeDelta;
	m_fIntroTime += fTimeDelta;

	if (m_fIntroTime >= 5.f)
	{
		if (m_bIntro_Ani)
		{
			FSM_FunctionSetting(&CUnit::Begin_Battle, &CUnit::Idle);
			m_eCurrentAction = (E_ACTION)E_Binah_Anim::Intro_V2;
			Animation_Change(0.f);
			m_bIntro_Ani = false;
		}
	}

	if (m_fActiveCharTime >= 15.f)
	{
		if (m_bCreateEnvironment)
		{
			CBinah_Environment* pEnv = CBinah_Environment::Create(m_pGraphicDev, E_OBJTAG::OBJ_BackGround, Get_Position() - _vec3(2.f, 0.f, 2.f), Get_Info(E_INFO::INFO_LOOK));
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Environment, pEnv->Get_ObjTag(), pEnv);

			if (m_bCreateHalo)
			{
				m_pGameObj_Halo = CBinah_Halo::Create(m_pGraphicDev, E_OBJTAG::OBJ_Halo, Get_Position(), Get_Info(E_INFO::INFO_UP));
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Environment, m_pGameObj_Halo->Get_ObjTag(), m_pGameObj_Halo);
				m_bCreateHalo = false;
			}

			m_bCreateEnvironment = false;
		}
	}

	if (m_fIntroNameTime >= 15.5f)
	{
		if (m_bIntroNameSound)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro_Name.wav", 2.f);
			m_bIntroNameSound = false;
		}
	}

	if (m_fActiveCharTime >= 18.f)
	{
		vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
		for (auto& pChar : vecChar)
		{
			pChar->Set_Active(true);
		}

		m_pDMeshCom->Set_AnimSpeed(1.5f);

		m_pBossHpBar->Set_Active(true);

		////전투 스킬 UI(F6키로 비나 전환시 쓰지 말것) 
		//if (m_bShowCombatUI)
		//{
		//	if (!CUI_Manager::GetInstance()->Get_CombatIngPanel()->Get_Active())
		//	{
		//		CUI_Manager::GetInstance()->Get_CombatIngPanel()->Set_Active(true);
		//	}
		//	m_bShowCombatUI = false;
		//}
	}
	else
	{
		vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
		for (auto& pChar : vecChar)
		{
			pChar->Set_Active(false);
		}

		m_pBossHpBar->Set_Active(false);

		////전투 스킬 UI(F6키로 비나 전환시 쓰지 말것)
		//if (CUI_Manager::GetInstance()->Get_CombatIngPanel()->Get_Active())
		//{
		//	CUI_Manager::GetInstance()->Get_CombatIngPanel()->Set_Active(false);
		//}
	}

	Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	Animation_Change(fTimeDelta);
	Pattern_Update(fTimeDelta);
	Fit_CameraInput();
	Pattern_Test();

	return RETURN_OK;
}

_int CBinah::LateUpdate_Object(const _float & fTimeDelta)
{
	CBossMonster::LateUpdate_Object(fTimeDelta);

	Glow_Setting(fTimeDelta);

	return RETURN_OK;
}

void CBinah::Render_Object(_int iPass)
{
	CBossMonster::Render_Object(iPass);

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBinah::Set_Constantable()
{
	CBossMonster::Set_Constantable();
}

void CBinah::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (!m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		m_pDMeshCom->Set_AnimSpeed(1.f);
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
		
		if (m_bDeadSound)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Dead.wav", 1.f);
			m_bDeadSound = false;
		}
	}

	if (nullptr == m_pTarget || m_pTarget->Get_UnitDead())
	{
		Find_Target_InRange(eTargetLayer, eObjTag);
	}

	if (nullptr == m_pTarget && m_eCurrentAction != (E_ACTION)E_Binah_Anim::Intro_V2)
	{
		FSM_FunctionRelease();
	}

	if (FALSE == m_bStateLock)
	{
		if (nullptr != m_pTarget)
		{
			FSM_FunctionSetting(&CUnit::Idle, &CUnit::Idle);
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
}

void CBinah::Begin_Battle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Intro_V2;
	Boss_FSM_Check(&CUnit::Idle);
}

void CBinah::Idle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Idel_1;

	if (m_pTarget != nullptr)
	{
		if (m_pTarget->Get_UnitDead())
		{
			FSM_FunctionRelease();
			return;
		}
	}

	m_fGage += fDeltaTime;

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

void CBinah::Dead(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Vital_Death;
	m_pTarget = nullptr;
	m_pDMeshCom->Set_AnimSpeed(1.f);
}

void CBinah::ExSkill(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Exs_01;
	Boss_FSM_Check(&CUnit::Idle);

	m_bExSkill = true;
	m_fExSkill2_Gage = 0.f;

	m_eCurrentAction = m_eMainAction;
}

//일반 공격
void CBinah::Boss_Attack_1(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Normal_Attack;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//차징 레이저
void CBinah::Boss_Attack_2(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Exs_01;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//미사일
void CBinah::Boss_Attack_3(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Binah_Anim::Exs_02_1;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//그로기
void CBinah::Boss_Groggy(_float fDeltaTime)
{
	CBossMonster::Boss_Groggy(fDeltaTime);

	Release_Invoke();

	m_pGlowAnim = &m_tGlowAnim_ToNormal;

	if (m_pLaser != nullptr)
	{
		m_pLaser->Set_Dead(true);
		Safe_Release(m_pLaser);
		m_pLaser = nullptr;
	}

	if (m_pEXLaser != nullptr)
	{
		m_pEXLaser->Set_Dead(true);
		Safe_Release(m_pEXLaser);
		m_pEXLaser = nullptr;
	}

	if (m_pQuake != nullptr)
	{
		m_fQuakeTime = 0.f;
		m_pQuake->Set_Dead(true);
		Safe_Release(m_pQuake);
		m_pQuake = nullptr;
	}

	m_eMainAction = (E_ACTION)E_Binah_Anim::Vital_Groggy;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);

	if (m_bGroggySound)
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Groggy.wav", 1.f);
		m_bGroggySound = false;
	}
}

void CBinah::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;

		Animation_Event();
	}
}

void CBinah::OnEnable()
{
	CBossMonster::OnEnable();
}

void CBinah::OnDisable()
{
	CBossMonster::OnDisable();
}

void CBinah::OnDestroy()
{
	CBossMonster::OnDestroy();
}

void CBinah::Fit_CameraInput()
{
	_float	fBaseY_At = CScene::Get_MainScene()->Get_BaseY_At(); 
	_float	fBaseY_Eye = CScene::Get_MainScene()->Get_BaseY_Eye();
	_float	fFarX = CScene::Get_MainScene()->Get_FarX();
	_float	fFarY = CScene::Get_MainScene()->Get_FarY();
	_float	fFarZ = CScene::Get_MainScene()->Get_FarZ();

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_J) && CKeyManager::GetInstance()->Key_Pressing(KEY_1))
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
		{
			fBaseY_At += 0.05f;
		}
		else
		{
			fBaseY_At -= 0.05f;
		}

		CScene::Get_MainScene()->Set_Fit_Camera_Option(fBaseY_At, fBaseY_Eye, fFarX, fFarY, fFarZ);
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_J) && CKeyManager::GetInstance()->Key_Pressing(KEY_2))
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
		{
			fBaseY_Eye += 0.05f;
		}
		else
		{
			fBaseY_Eye -= 0.05f;
		}

		CScene::Get_MainScene()->Set_Fit_Camera_Option(fBaseY_At, fBaseY_Eye, fFarX, fFarY, fFarZ);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_J) && CKeyManager::GetInstance()->Key_Pressing(KEY_3))
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
		{
			fFarX += 0.05f;
		}
		else
		{
			fFarX -= 0.05f;
		}

		CScene::Get_MainScene()->Set_Fit_Camera_Option(fBaseY_At, fBaseY_Eye, fFarX, fFarY, fFarZ);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_J) && CKeyManager::GetInstance()->Key_Pressing(KEY_4))
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
		{
			fFarY += 0.05f;
		}
		else
		{
			fFarY -= 0.05f;
		}

		CScene::Get_MainScene()->Set_Fit_Camera_Option(fBaseY_At, fBaseY_Eye, fFarX, fFarY, fFarZ);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_J) && CKeyManager::GetInstance()->Key_Pressing(KEY_5))
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
		{
			fFarZ += 0.05f;
		}
		else
		{
			fFarZ -= 0.05f;
		}

		CScene::Get_MainScene()->Set_Fit_Camera_Option(fBaseY_At, fBaseY_Eye, fFarX, fFarY, fFarZ);
	}
}

void CBinah::Pattern_Test()
{
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_1))
	{
		Release_Invoke();
		Normal_Attack_Pattern(0, true);
		FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_2))
	{
		Release_Invoke();
		Laser_Pattern(0, true);
		FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_3))
	{
		Release_Invoke();
		Misile_Pattern(0, true);
		FSM_FunctionSetting(&CUnit::Boss_Attack_3, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_4))
	{
		Release_Invoke();
		AreaLaser_Pattern(0, true);
		FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_5))
	{
		Random_Guide(true);
	}
}

void CBinah::Pattern_Update(const _float& fTimeDelta)
{
	if (m_pHalo != nullptr)
	{
		if (m_pHalo->Get_Dead())
		{
			Safe_Release(m_pHalo);
			m_pHalo = nullptr;
			return;
		}
		
		Halo_Update(fTimeDelta);
	}

	if (m_pLaser != nullptr)
	{
		if (m_pLaser->Get_Dead())
		{
			Safe_Release(m_pLaser);
			m_pLaser = nullptr;
			m_fQuakeTime = 0.f;
			m_bCreateQuake = true;
			m_fExDistance = 0.f;
			m_vecColObject.clear();
			return;
		}

		Laser_Update(fTimeDelta);
	}

	if (m_pEXLaser != nullptr)
	{
		if (m_pEXLaser->Get_Dead())
		{
			Safe_Release(m_pEXLaser);
			m_pEXLaser = nullptr;
			m_fEXLaserTime = 0.f;
			return;
		}

		EXLaser_Update(fTimeDelta);
	}

	if (m_pQuake != nullptr)
	{
		if (m_pQuake->Get_Dead())
		{
			Safe_Release(m_pQuake);
			m_pQuake = nullptr;
		}
	}
	
	if (m_pEye_L1 != nullptr)
	{
		if (!m_pEye_L1->Get_Dead())
		{
			m_pEye_L1->Set_Pos(m_pBoneEye_L1->GetPos());
		}
		else
		{
			m_pEye_L1 = nullptr;
		}
	}

	if (m_pEye_L2 != nullptr)
	{
		if (!m_pEye_L2->Get_Dead())
		{
			m_pEye_L2->Set_Pos(m_pBoneEye_L2->GetPos());
		}
		else
		{
			m_pEye_L2 = nullptr;
		}
	}
}

void CBinah::Laser_Update(const _float& fTimeDelta)
{
	CDMesh* pMeshCom = static_cast<CDMesh*>(Get_Component(ComTag_DMesh));
	const D3DXFRAME_DERIVED* pFrame = pMeshCom->Get_FrameByName("bone_Cannon_mo_01");
	const _matrix* pBoneMatrix = &pFrame->CombinedTransformMatrix;
	const _matrix* pWorldMatrix = &Get_Transform()->Get_WorldMatrix();
	_matrix matParent = (*pBoneMatrix * *pWorldMatrix);

	_matrix matRot;
	_vec3 vLook = _vec3(matParent._11, matParent._12, matParent._13);
	_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);

	m_pLaser->Set_Pos(_vec3(matParent._41, matParent._42, matParent._43));
	m_pLaser->Set_Rotation(vRot);
	m_pLaser->Set_Scale(3.f, 3.f, 3.f);

	m_fQuakeTime += fTimeDelta;
	if (m_fQuakeTime >= 5.f)
	{
		if (m_bLaserSound)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Laser_Attack.wav", 1.f);
			m_bLaserSound = false;
		}

		map<E_OBJTAG, vector<CGameObject*>>* pMap = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Map);

		_vec3	vStartPos = m_pLaser->Get_Position();
		_vec3	vRayDir = m_pLaser->Get_Info(E_INFO::INFO_LOOK);

		for (auto& tPair : *pMap)
		{
			for (auto& pObj : tPair.second)
			{
				CSMesh* pSMesh = static_cast<CSMesh*>(pObj->Get_Component(ComTag_SMesh));
				_vec3 vPos;
				_float fDist;

				if (pSMesh)
				{
					if (pSMesh->PickingMesh(vStartPos, vRayDir, &vPos, &fDist, pObj->Get_Transform()->Get_WorldMatrix()))
					{
						if (m_bCreateQuake)
						{
							CScene::Get_MainScene()->Active_ShakeCam(2.f, 0.05f);

							if (m_pQuake)
							{
								m_pQuake->Set_Dead(TRUE);
								Safe_Release(m_pQuake);
								m_pQuake = nullptr;
							}

							m_pQuake = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Normal_Laser_Quake.skill", E_LAYERTAG::Effect, vPos, Get_Angle(), VEC_ONE * 4.5f);
							
							m_pQuake->AddRef();
							assert(m_pQuake);
							m_bCreateQuake = false;
						}
					}
				}
			}
		}

		m_fExDistance += fTimeDelta * 10.f;
		CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position() + _vec3(0.f, 0.01f, 0.f), Get_Info(E_INFO::INFO_LOOK), 5.f, m_fExDistance, E_LAYERTAG::Character, Collision_Player);
		//CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position() + Look * m_fExDistance, Get_Info(E_INFO::INFO_LOOK), 3.f, 1.f, E_LAYERTAG::Monster, ColMonster);
	}
}

void CBinah::EXLaser_Update(const _float& fTimeDelta)
{
	m_fEXLaserTime += fTimeDelta;

	if (m_fEXLaserTime <= 3.f)
	{
		CDMesh* pMeshCom = static_cast<CDMesh*>(Get_Component(ComTag_DMesh));
		const D3DXFRAME_DERIVED* pFrame = pMeshCom->Get_FrameByName("bone_Cannon_mo_01");
		const _matrix* pBoneMatrix = &pFrame->CombinedTransformMatrix;
		const _matrix* pWorldMatrix = &Get_Transform()->Get_WorldMatrix();
		_matrix matParent = (*pBoneMatrix * *pWorldMatrix);

		_matrix matRot;
		_vec3 vLook = _vec3(matParent._11, matParent._12, matParent._13);
		_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);

		m_pEXLaser->Set_Pos(_vec3(matParent._41 + 1.f, matParent._42 - 1.f, matParent._43));
		m_pEXLaser->Set_Rotation(vRot);
		m_pEXLaser->Set_Scale(2.f, 2.f, 2.f);
	}
	else
	{
		_vec3 m_vSkillMoveDest = m_vecCharPos;
		_vec3 m_vSkillPos = m_pEXLaser->Get_Position();
		_vec3 m_vecSkillDir = m_vSkillMoveDest - m_vSkillPos;
		D3DXVec3Normalize(&m_vecSkillDir, &m_vecSkillDir);

		_vec3 vMoveAmount = m_vecSkillDir * fTimeDelta * 6.5f;
		m_pEXLaser->Set_Look(m_vSkillPos + m_vecSkillDir);
		m_pEXLaser->Add_Pos(vMoveAmount);
	}
}

void CBinah::Halo_Update(const _float& fTimeDelta)
{
	m_pHalo->Set_Pos(m_pHead->GetPos());
	m_pHalo->Set_Rotation(Get_Angle());
}

void CBinah::Collision_Laser(CGameObject * pPlayer)
{
	for (auto& pObj : m_vecColObject)
	{
		if (pObj == pPlayer)
		{
			return;
		}
	}

	m_vecColObject.emplace_back(pPlayer);
	static_cast<CUnit*>(pPlayer)->Set_Damage(Get_UnitInfo(), Get_MonsterInfo().eAtkType);
}

void CBinah::Misile_Pose1()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Misile_Pose.wav", 1.f);
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_01->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
	CScene::Get_MainScene()->Active_ShakeCam(1.f, 0.2f);
}

void CBinah::Misile_Pose2()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_02->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
}

void CBinah::Misile_Pose3()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_03->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
}

void CBinah::Misile_Pose4()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_04->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
	CScene::Get_MainScene()->Active_ShakeCam(1.f, 0.2f);
}

void CBinah::Misile_Pose5()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_05->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
}

void CBinah::Misile_Pose6()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Misile_Pose.skill", E_LAYERTAG::Effect, m_pBone_Canon_06->GetPos(), VEC_ZERO, VEC_ONE * 3.f);
}

void CBinah::Trigger_1()
{
	Normal_Attack_Pattern(m_iPatternRand, false);
}

void CBinah::Trigger_2()
{
	if (!m_bExSkill)
	{
		Laser_Pattern(m_iPatternRand, false);
	}
	else
	{
		AreaLaser_Pattern(m_iPatternRand, false);
	}
}

void CBinah::Trigger_3()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro1.wav", 3.f);
	CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Binah.camdat", L"Binah_Intro_1", Get_Position(), Get_Angle().y);
	CSkill_Manager::GetInstance()->SetSkill(L"Binah_Intro.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
	m_pHalo = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Intro_Halo.skill", E_LAYERTAG::Effect, m_pHead->GetPos(), VEC_ZERO, VEC_ONE * 2.f);
	m_pHalo->AddRef();
	
	assert(m_pHalo);
}

void CBinah::Trigger_4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro2.wav", 2.f);
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Binah.camdat", L"Binah_Intro_2", Get_Position(), Get_Angle().y);
}

void CBinah::Trigger_5()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro2.wav", 2.f);
	CScene::Get_MainScene()->Active_AutoCam();
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Binah.camdat", L"Binah_Intro_3", Get_Position(), Get_Angle().y);
}

void CBinah::Trigger_6()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro3.wav", 2.f);
	Set_Pos(_vec3(6.f, 0.5f, -0.5f));
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Binah.camdat", L"Binah_Intro_4", Get_Position(), Get_Angle().y);
}

void CBinah::Trigger_7()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Intro_Eye.wav", 2.f);
}

void CBinah::Animation_Event()
{
	m_iPatternRand = CWall512::GetInstance()->GetValue(0, 100);

	if ((E_ACTION)E_Binah_Anim::Idle == m_eCurrentAction)
	{
		m_fGage = 0.f;
		m_pGlowAnim = &m_tGlowAnim_ToNormal;
	}
	else if ((E_ACTION)E_Binah_Anim::Normal_Attack == m_eCurrentAction)
	{
		m_pGlowAnim = &m_tGlowAnim_Arm;

		Normal_Attack_Pattern(m_iPatternRand, true);
		m_bNormalBombSound = true;
	}
	else if ((E_ACTION)E_Binah_Anim::Exs_01 == m_eCurrentAction)
	{
		if (!m_bExSkill)
		{
			CUI_Manager::GetInstance()->Set_Announcement(L"  대피하세요, 선생님!", 1.f);

			m_pGlowAnim = &m_tGlowAnim_Arm;

			Laser_Pattern(m_iPatternRand, true);

			CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position() + _vec3(3.f, -0.5f, 3.f), Get_Angle(), 20.f, 5.f, 4.f, TRUE);
			Invoke_Add(0.f, bind(&CBinah::Laser_Skill, this));
		}
		else
		{
			m_bExBombSound = true;
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_EXLaser_Charge.wav",1.f);

			CUI_Manager::GetInstance()->Set_Announcement(L"  위험한게 오고있어요!", 2.f);

			m_pGlowAnim = &m_tGlowAnim_FullBody;

			AreaLaser_Pattern(m_iPatternRand, true);

			if (m_pEXLaser)
			{
				m_pEXLaser->Set_Dead(TRUE);
				Safe_Release(m_pEXLaser);
				m_pEXLaser = nullptr;
			}

			m_pEXLaser = CSkill_Manager::GetInstance()->SetSkill(L"Binah_EX_Charge.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
			m_pEXLaser->AddRef();
			assert(m_pEXLaser);

			_matrix matInvView;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
			D3DXMatrixInverse(&matInvView, 0, &matInvView);
			_vec3 vUp = _vec3(matInvView._21, matInvView._22, matInvView._23);
			D3DXVec3Normalize(&vUp, &vUp);
			D3DXVec3Normalize(&vUp, &vUp);

			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Warning.wav", 0.5f);
			CSkillObject* pWarningEff = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Warning.skill", E_LAYERTAG::Effect, CCamera::m_pMainCam->Get_CamAt() + vUp * 0.2f, VEC_ZERO, 0.1f);
			_vec3	vPos = CCamera::m_pMainCam->Get_CamAt() + *D3DXVec3Normalize(&m_vOutTemp, &(CCamera::m_pMainCam->Get_CamAt() - CCamera::m_pMainCam->Get_CamEye()));
			pWarningEff->Set_Look(vPos);

			vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
			_int iRandChar = CWall512::GetInstance()->GetValue(0, 10000) % (_int)(&vecChar)->size();
			m_vecCharPos = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers()[iRandChar]->Get_Position();
			CWarning* pWarning = CWarning::Create(m_pGraphicDev);
			pWarning->Set_Warning(E_WarnType::Circle_Magic, E_WarnType::Circle_InsideFill, m_vecCharPos, 4.f, 0.f, 11.f, D3DXCOLOR(C_Blue));
			pWarning->Set_AfterSkillName(L"Binah_EX_Bomb.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_SoundName(L"SFX_Binah_EX_Hit_Explosion.wav", 3.f);
			pWarning->Set_Damage(300);

			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
		}
	}
	else if ((E_ACTION)E_Binah_Anim::Exs_02_1 == m_eCurrentAction)
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Misile_Ready.wav",1.f);

		CUI_Manager::GetInstance()->Set_Announcement(L"전방에 미확인 비행체에 대비하세요!", 1.f);

		m_pGlowAnim = &m_tGlowAnim_Arm;

		Misile_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Binah_Anim::Vital_Groggy == m_eCurrentAction)
	{
		m_bGroggySound = true;

		CUI_Manager::GetInstance()->Set_Announcement(L"비나가 기력을 다했어요, 지금이에요!", 2.f);

		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
	}
}

void CBinah::Ready_Glow()
{
	m_tGlowAnim_Arm.eGlowAnimMode = E_GlowAnimMode::Loop;
	m_tGlowAnim_Arm.fTotalTime = 2.f;
	m_tGlowAnim_Arm.eEase = E_EASE::OutSine;
	m_tGlowAnim_Arm.vStart_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_Arm.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_Arm.vEnd_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_Arm.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_Arm.vBeginEnd_Power = _vec2(0.f, 1.f);
	m_tGlowAnim_Arm.fGlowBlue = 1.f;
	m_tGlowAnim_Arm.fGlowBright = 0.15f;

	m_tGlowAnim_FullBody.eGlowAnimMode = E_GlowAnimMode::Loop;
	m_tGlowAnim_FullBody.eEase = E_EASE::OutSine;
	m_tGlowAnim_FullBody.vStart_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vEnd_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vBeginEnd_Power = _vec2(0.f, 1.f);
	m_tGlowAnim_FullBody.fGlowBlue = 1.f;
	m_tGlowAnim_FullBody.fGlowBright = 0.3f;

	m_tGlowAnim_ToNormal.eGlowAnimMode = E_GlowAnimMode::One;
	m_tGlowAnim_ToNormal.eEase = E_EASE::None;
	m_tGlowAnim_ToNormal.vStart_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_ToNormal.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vEnd_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_ToNormal.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vBeginEnd_Power = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.fGlowBlue = 0.f;
	m_tGlowAnim_ToNormal.fGlowBright = 0.01f;
}

void CBinah::Glow_Setting(const _float & fTimeDelta)
{
	if (m_pDMeshCom && m_pGlowAnim)
	{
		m_pGlowAnim->fLeftTime += fTimeDelta;
		_float fPower = m_pGlowAnim->fGet_Power();
		_float fBright = m_pGlowAnim->fGlowBright;
		_float fBlue = m_pGlowAnim->fGlowBlue;
		_vec2 vMin = m_pGlowAnim->vGet_Min();
		_vec2 vMax = m_pGlowAnim->vGet_Max();

		m_pDMeshCom->Set_Glow_Option(0, fBright, _vec3(0.f, 0.f, fBlue), fPower, vMin.x, vMax.x, vMin.y, vMax.y, _vec3(1.f, 1.f, 1.f));

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
}

void CBinah::EX_HDR(const _float & fTimeDelta)
{
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 0.f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 0.65f;
}

void CBinah::Ready_HDR()
{
	CRenderer::GetInstance()->Get_RenderOption().fLuminance = 0.148f;
	CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = 0.18f;
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 1.f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 0.83f;
}

void CBinah::Reset_HDR()
{
	CRenderer::GetInstance()->Get_RenderOption().fLuminance = 0.068f;
	CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = 0.18f;
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 0.9f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 1.f;
}

void CBinah::Normal_Attack_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_NomalAttack.wav", 1.f);

			m_pEye_L1 = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Normal_Attack_1.skill", E_LAYERTAG::Effect, m_pBoneEye_L1->GetPos(), VEC_ZERO, VEC_ONE);
			m_pEye_L1->Set_Scale(_vec3(1.f, 1.f, 1.f));
			m_pEye_L2 = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Normal_Attack_2.skill", E_LAYERTAG::Effect, m_pBoneEye_L2->GetPos(), VEC_ZERO, VEC_ONE);
			m_pEye_L2->Set_Scale(_vec3(5.f, 5.f, 5.f));

			vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
			_int iRandChar = CWall512::GetInstance()->GetValue(0, 10000) % (_int)(&vecChar)->size();
			_vec3 vPos = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers()[iRandChar]->Get_Position();

			CWarning* pWarning = CWarning::Create(m_pGraphicDev);
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vPos, 1.5f, 0.f, 3.f, D3DXCOLOR(C_Red));
			pWarning->Set_AfterSkillName(L"Binah_Normal_Attack_Explosion.skill", VEC_ZERO, VEC_ONE * 3.f);
			pWarning->Set_SoundName(L"SFX_Binah_NomalAttack_Hit_Explosion.wav", 1.f);
			pWarning->Set_Damage(100);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
		}
		else
		{
		}
	}
	else if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
			Random_Guide(true);
		}
	}
}

void CBinah::Laser_Pattern(_int iRand, _bool bStart)
{
	//트리거 이후에 수행
	if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
		}
	}
	else if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
			//Random_Guide(true);
		}
	}
}

void CBinah::Misile_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Misile_Ready.wav", 1.f);

			Invoke_Add(2.5f, bind(&CBinah::Misile_Pose1, this));
			Invoke_Add(2.7f, bind(&CBinah::Misile_Pose2, this));
			Invoke_Add(2.9f, bind(&CBinah::Misile_Pose3, this));
			Invoke_Add(3.1f, bind(&CBinah::Misile_Pose4, this));
			Invoke_Add(3.3f, bind(&CBinah::Misile_Pose5, this));
			Invoke_Add(3.5f, bind(&CBinah::Misile_Pose6, this));

			CWarning* pWarning = nullptr;
			_vec3 vCenter = (m_pTarget->Get_Position() + Get_Position()) * 0.6f;
			_float fAddY = 0.001f;

			for (_int i = 0; i < 7; ++i)
			{
				fAddY += 0.001f;
				pWarning = CWarning::Create(m_pGraphicDev);
				_vec3 vAddPos = _vec3(CWall512::GetInstance()->GetFloatValue(-2.f, 2.f), 0.f, CWall512::GetInstance()->GetFloatValue(-5.f, 5.f));
				vAddPos.y += fAddY;
				pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter + vAddPos, 3.f, CWall512::GetInstance()->GetFloatValue(0.f, 3.f), 4.f, D3DXCOLOR(C_Red));
				pWarning->Set_Damage(100);
				pWarning->Set_AfterSkillName(L"Binah_Misile_Shot.skill", VEC_ZERO, VEC_ONE * 2.5f);
				pWarning->Set_SoundName(L"SFX_Binah_Misile_Shot.wav", 1.f);
				CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
			}
		}
		else
		{
		}
	}
	else if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
			//Random_Guide(true);
		}
	}
}


void CBinah::AreaLaser_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
		}
	}
	else if (iRand < 100)
	{
		if (bStart)
		{
		}
		else
		{
			//Random_Guide(true);
		}
	}
}

void CBinah::Random_Guide(_bool bRand)
{
	Find_Target_InRange(m_eTargetLayerTag, m_eTargetObjTag);

	if (nullptr == m_pTarget)
	{
		return;
	}

	if (bRand)
	{
		_int iRand = CWall512::GetInstance()->GetValue(0, (_int)E_Random_Guide::Random_Guide_END);
		m_eRandomGuide = E_Random_Guide(iRand);
	}

	CUI_Manager::GetInstance()->Set_Announcement(L"랜덤 가이드 시전 : " + to_wstring((_int)m_eRandomGuide), 2.f);

	_int iWarnTypeRand = CWall512::GetInstance()->GetValue(0, (_int)E_WarnType::WarnEnd);
	CWarning* pWarning = nullptr;
	_vec3 vCenter = (m_pTarget->Get_Position() + Get_Position()) * 0.5f;
	_float fAddY = 0.001f;
	vCenter.y = 0.201f;


	switch (m_eRandomGuide)
	{
	case E_Random_Guide::Random_Guide_Bomb:
	{
		for (_int i = 0; i < 7; ++i)
		{
			fAddY += 0.001f;
			pWarning = CWarning::Create(m_pGraphicDev);
			_vec3 vAddPos = _vec3(CWall512::GetInstance()->GetFloatValue(-5.f, 5.f), 0.f, CWall512::GetInstance()->GetFloatValue(-10.f, 10.f));
			vAddPos.y += fAddY;
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter + vAddPos, 4.f, CWall512::GetInstance()->GetFloatValue(0.f, 3.f), 3.f, D3DXCOLOR(C_Yellow));
			pWarning->Set_Damage(50);
			pWarning->Set_AfterSkillName(L"Mushroom_Cloud.skill", VEC_ZERO, VEC_ONE * 1.f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);
		}
	}
	break;
	case E_Random_Guide::Random_Guide_Circle:
	{
		for (_int i = 0; i < 3; ++i)
		{
			pWarning = CWarning::Create(m_pGraphicDev);
			fAddY += 0.001f;
			vCenter.y = 0.201f;

			// #1 : 외부 원 Rect 이미지, 
			// #2 : 내부 원 (커지는거) Rect 이미지, 
			// #3 : 포지션
			// #4 : 총 시간
			// #5 : 딜레이(아예안그려지는 업데이트를 안하는 딜레이)
			// #6 : 반지름
			// #7 : 시작 Color
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, vCenter, 1.5f, 0.f, 3.5f);

			// 데미지 설정해줘야 데미지 가하고
			pWarning->Set_Damage(50);

			// 워닝이 끝났을때 나올 스킬 이름, AddPos, Scale
			pWarning->Set_AfterSkillName(L"EH_Aru_Exs_Bomb_1_0110.skill", VEC_ZERO, VEC_ONE * 0.7f);

			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 0.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Red, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 90.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Red, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 180.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Red, C_White);
			CWarning_Mgr::GetInstance()->Create_Circle(pWarning, 270.f, 15.f, 6, i * 3.0f, 0.1f, vCenter + _vec3(0.f, fAddY, i * 2.f), 0.3f, 5.f, C_Red, C_White);

			Safe_Release(pWarning);
		}
	}
	break;

	default:
		break;
	}

	Safe_Release(pWarning);
}

void CBinah::Skill_Shake_Time(const _float & fTimeDelta)
{
	if (m_bExBombSound)
	{
		m_fExBombTime += fTimeDelta;
	}
	if (m_bNormalBombSound)
	{
		m_fNormalBombTime += fTimeDelta;
	}

	if (m_fExBombTime >= 4.f)
	{
		CScene::Get_MainScene()->Active_ShakeCam(2.f, 0.2f);

		m_fExBombTime = 0.f;
		m_bExBombSound = false;
	}
	if (m_fNormalBombTime >= 1.5f)
	{
		CScene::Get_MainScene()->Active_ShakeCam(0.5f, 0.1f);

		m_fNormalBombTime = 0.f;
		m_bNormalBombSound = false;
	}
}

void CBinah::Update_Collider(const _float & fTimeDelta)
{
	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
	Collider_SetMatrix(m_pWeaponCollider, m_szWeaponBoneName);

	Collider_SetMatrix(m_pBoneEye_L1, m_szEye_L1);
	Collider_SetMatrix(m_pBoneEye_L2, m_szEye_L2);
	Collider_SetMatrix(m_pBoneEye_R1, m_szEye_R1);
	Collider_SetMatrix(m_pBoneEye_R2, m_szEye_R2);

	Collider_SetMatrix(m_pBone_Canon_01, m_szCanon_01);
	Collider_SetMatrix(m_pBone_Canon_02, m_szCanon_02);
	Collider_SetMatrix(m_pBone_Canon_03, m_szCanon_03);
	Collider_SetMatrix(m_pBone_Canon_04, m_szCanon_04);
	Collider_SetMatrix(m_pBone_Canon_05, m_szCanon_05);
	Collider_SetMatrix(m_pBone_Canon_06, m_szCanon_06);

	Collider_SetMatrix(m_pHead, m_szHead);
	Collider_SetMatrix(m_pBone_Root, m_szBone_Root);
}

void CBinah::Laser_Skill()
{
	if (m_pLaser)
	{
		m_pLaser->Set_Dead(TRUE);
		Safe_Release(m_pLaser);
		m_pLaser = nullptr;
	}

	m_pLaser = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Normal_Laser.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, 3.f);
	m_pLaser->AddRef();
	assert(m_pLaser);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Laser_Charge.wav", 1.f);
	m_bLaserSound = true;
}

void CBinah::Collision_Player(CGameObject * pThis, CGameObject * pPlayer)
{
	static_cast<CBinah*>(pThis)->Collision_Laser(pPlayer);
}