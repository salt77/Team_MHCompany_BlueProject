#include "Hod.h"
#include "Binah.h"
#include "Binah_Halo.h"
#include "Boss_HpBar.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "CollisionManager.h"
#include "GameManager.h"
#include "Warning.h"
#include "Warning_Mgr.h"
#include "WorldCreator.h"
#include "TimeScale_Manager.h"

CHod::CHod(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CBossMonster(pGraphicDev, eObjTag, L"Hod")
{
}

CHod::CHod(const CHod & rhs)
	:CBossMonster(rhs)
{
}

CHod::~CHod()
{
}

CHod * CHod::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHod* pInstance = new CHod(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod::Free()
{
	CBossMonster::Free();

	CScene::Get_MainScene()->Set_Binah_Cam(false);

	for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{
			pChar->Set_AddRange(-1000);
		}
	}

	if (m_pEXS2 != nullptr)
	{
		if (m_pEXS2->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pEXS2);
			m_pEXS2 = nullptr;
			m_vecColObject.clear();
		}
	}

	if (m_pAreaLaser != nullptr)
	{
		if (m_pAreaLaser->Get_Dead())
		{
			Set_Dead(true);
			Safe_Release(m_pAreaLaser);
			m_pAreaLaser = nullptr;
			m_vecColObject.clear();
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
}

HRESULT CHod::Ready_Object()
{
	CBossMonster::Ready_Object();
	
	CSound_Manager::GetInstance()->StopAll();
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_보스전_호드.mp3", 0.3f);

	m_tUnitInfo.iAttack = 220;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 500000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 999999;
	m_tUnitInfo.iStability;

	Ready_Glow();
	Ready_HDR();

	CBinah* pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));
	pBinah->Get_Boss_HP()->Set_Active(false);
	m_pBossHpBar = CBoss_HpBar::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), L"Hod", m_tUnitInfo.iMaxHp, 50);
	assert(m_pBossHpBar);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, m_pBossHpBar->Get_ObjTag(), m_pBossHpBar);
	static_cast<CUI_Object*>(m_pBossHpBar)->Set_ImageAlpha(0.f);

	static_cast<CUI_Object*>(m_pBossHpBar)->Set_ImageAlpha(0.f);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_PosX(28);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_PosY(20);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_SizeX(80);
	m_pBossHpBar->Get_BossHP_Parent()->Get_ChildByIndex(4)->Set_Ui_SizeY(20);

	return S_OK;
}

_int CHod::Start_Object(const _float & fTimeDelta)
{
	CBossMonster::Start_Object(fTimeDelta);

	CScene::Get_MainScene()->Set_Binah_Cam(false);
	
	//Char Range
	for (auto& pChar : (*CGameManager::GetInstance()->Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (pChar)
		{ 
			pChar->Set_AddRange(1000);
		}
	}

	CScene::Get_MainScene()->Set_AutoCam_Ratio(0.3f);
	CScene::Get_MainScene()->Set_Fit_Camera_Option(-1.4f, 29.25f, -5.2f, -1.f, 1.15f);
	CScene::Get_MainScene()->Set_Fit_Camera_Interval(-60.f, 60.f, -40.f, 40.f, -40.f, 40.f);

	m_szBodyBoneName = "bone_Head_01";
	m_szWeaponBoneName = "bone_L_Lasergun";
	Create_Collider(&m_pTentacle_R, L"Col_Tentacle_R", 0.25f);
	Create_Collider(&m_pTentacle_RB, L"Col_Tentacle_RB", 0.25f);
	Create_Collider(&m_pTentacle_L, L"Col_Tentacle_L", 0.25f);
	Create_Collider(&m_pTentacle_LB, L"Col_Tentacle_LB", 0.25f);
	Create_Collider(&m_pBone_LaserGun_L, L"LaserGun_L", 0.25f);
	Create_Collider(&m_pBone_LaserGun_R, L"LaserGun_R", 0.25f);
	Create_Collider(&m_pHead, L"Col_Head", 0.25f);
	Create_Collider(&m_pBone_Body, L"Col_Body", 0.25f);
	Create_Collider(&m_pTentacle_Attack_L, L"Col_Tentacle_Attack_L", 0.25f);

	CActionCam_Mgr::GetInstance()->Load_ActionCam(L"Hod_Fade.camdat");
	CActionCam_Mgr::GetInstance()->Load_ActionCam(L"Hod.camdat");

	m_pDMeshCom->Set_AnimSpeed(0.8f);

	Set_Pos(_vec3(-7.5f, 0.f, 1.f));
	Set_GroggyPercent(100.f);

	return RETURN_OK;
}

_int CHod::Update_Object(const _float & fTimeDelta)
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
				pChar->Set_AddRange(-1000);
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
				pChar->Set_AddRange(-1000);
			}
		}
	}

	if (m_tUnitInfo.iMaxHp <= 0)
	{
		Reset_HDR();
		Set_Dead(true);

		CBinah* pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));
		pBinah->Set_Dead(true);
	}

	m_pSphereCollider->SetRadius(0.1f);

	m_fActiveCharTime += fTimeDelta;
	m_fIntroTime += fTimeDelta;

	if (m_fIntroTime >= 0.f)
	{
		if (m_bIntro_Ani)
		{
			FSM_FunctionSetting(&CUnit::Begin_Battle, &CUnit::Idle);
			m_eCurrentAction = (E_ACTION)E_Hod_Anim::BattleReady;
			Animation_Change(0.f);
			m_bIntro_Ani = false;
		}
	}

	if (m_fActiveCharTime >= 12.5f)
	{
		vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
		for (auto& pChar : vecChar)
		{
			pChar->Set_Active(true);
			pChar->Set_UseGravity(true);
		}

		m_pDMeshCom->Set_AnimSpeed(1.5f);

		if (m_bShowHP)
		{
			m_pBossHpBar->Set_Active(true);
			m_bShowHP = false;
		}

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
	Collision_Distance(fTimeDelta);
	EX4_Function(fTimeDelta);
	Fit_CameraInput();
	Pattern_Test();

	return RETURN_OK;
}

_int CHod::LateUpdate_Object(const _float & fTimeDelta)
{
	CBossMonster::LateUpdate_Object(fTimeDelta);

	Glow_Setting(fTimeDelta);

	return RETURN_OK;
}

void CHod::Render_Object(_int iPass)
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

void CHod::Set_Constantable()
{
	CBossMonster::Set_Constantable();
}

void CHod::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
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

	if (nullptr == m_pTarget && m_eCurrentAction != (E_ACTION)E_Hod_Anim::BattleReady)
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

void CHod::Begin_Battle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::BattleReady;
	Boss_FSM_Check(&CUnit::Idle);
}

void CHod::Idle(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::Normal_Attack_Delay;

	m_tGlowAnim_ToNormal;

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

		if (iRand < 22)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
		}
		else if (iRand < 44)
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
		}
		else if (iRand < 66)
		{
			//체력 20만 이하 또는 타워가 없는 경우에만 수행한다.
			if (m_pBossHpBar->Get_Hp() < 200000 /* && m_pTower == nullptr*/)
			{
				FSM_FunctionSetting(&CUnit::Boss_Attack_3, nullptr);
			}
			else
			{
				FSM_FunctionSetting(&CUnit::Boss_Attack_4, nullptr);
			}
		}
		else
		{
			FSM_FunctionSetting(&CUnit::Boss_Attack_4, nullptr);
		}
	}

	m_eCurrentAction = m_eMainAction;
}

void CHod::Dead(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::Vital_Death;
	m_pTarget = nullptr;
	m_pDMeshCom->Set_AnimSpeed(1.f);
}

//광역 레이저
void CHod::ExSkill(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::EXS5;
	Boss_FSM_Check(&CUnit::Idle);

	m_bExSkill = true;
	m_fExSkill2_Gage = 0.f;

	m_eCurrentAction = m_eMainAction;
}

//차징 후 전기 충격
void CHod::Boss_Attack_1(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::EXS1;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//한손 후려치기
void CHod::Boss_Attack_2(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::EXS2;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//기믹 생성
void CHod::Boss_Attack_3(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::EXS3;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//파장 공격
void CHod::Boss_Attack_4(_float fDeltaTime)
{
	m_eMainAction = (E_ACTION)E_Hod_Anim::EXS4;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);
}

//그로기
void CHod::Boss_Groggy(_float fDeltaTime)
{
	CBossMonster::Boss_Groggy(fDeltaTime);

	Release_Invoke();

	m_pGlowAnim = &m_tGlowAnim_ToNormal;

	if (m_pEXS2 != nullptr)
	{
		m_pEXS2->Set_Dead(true);
		Safe_Release(m_pEXS2);
		m_pEXS2 = nullptr;
	}

	if (m_pAreaLaser != nullptr)
	{
		m_pAreaLaser->Set_Dead(true);
		Safe_Release(m_pAreaLaser);
		m_pAreaLaser = nullptr;
	}

	if (m_pQuake != nullptr)
	{
		m_fQuakeTime = 0.f;
		m_pQuake->Set_Dead(true);
		Safe_Release(m_pQuake);
		m_pQuake = nullptr;
	}

	m_eMainAction = (E_ACTION)E_Hod_Anim::Vital_Groggy;
	m_bExSkill = false;
	Boss_FSM_Check(&CUnit::Idle);


	if (m_bGroggySound)
	{
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Groggy.wav", 1.f);
		m_bGroggySound = false;
	}
}

void CHod::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;

		Animation_Event();
	}
}

void CHod::OnEnable()
{
	CBossMonster::OnEnable();
}

void CHod::OnDisable()
{
	CBossMonster::OnDisable();
}

void CHod::OnDestroy()
{
	CBossMonster::OnDestroy();
}

void CHod::Fit_CameraInput()
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

void CHod::Pattern_Test()
{
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_1))
	{
		Release_Invoke();
		FSM_FunctionSetting(&CUnit::Boss_Attack_1, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_2))
	{
		Release_Invoke();
		FSM_FunctionSetting(&CUnit::Boss_Attack_2, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_3))
	{
		Release_Invoke();
		FSM_FunctionSetting(&CUnit::Boss_Attack_3, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_4))
	{
		Release_Invoke();
		FSM_FunctionSetting(&CUnit::Boss_Attack_4, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_5))
	{
		Release_Invoke();
		FSM_FunctionSetting(&CUnit::ExSkill, nullptr);
	}
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_K) && CKeyManager::GetInstance()->Key_Down(KEY_6))
	{
		Random_Guide(true);
	}
}

void CHod::Pattern_Update(const _float& fTimeDelta)
{
	if (m_pAreaLaser != nullptr)
	{
		if (m_pAreaLaser->Get_Dead())
		{
			Safe_Release(m_pAreaLaser);
			m_pAreaLaser = nullptr;
			m_fQuakeTime = 0.f;
			m_bCreateQuake = true;
			m_fColDistance = 0.f;
			m_vecColObject.clear();
			return;
		}

		AreaLaser_Update(fTimeDelta);
	}

	if (m_pQuake != nullptr)
	{
		if (m_pQuake->Get_Dead())
		{
			Safe_Release(m_pQuake);
			m_pQuake = nullptr;
		}
	}
}

void CHod::Collision_Distance(const _float & fTimeDelta)
{
	if (m_bEXS2Distance)
	{
		if (m_pEXS2 != nullptr)
		{
			m_fEXS2ColTime += fTimeDelta;

			if (m_pEXS2->Get_Dead())
			{
				Safe_Release(m_pEXS2);
				m_pEXS2 = nullptr;

				m_fEXS2Distance = 0.f;
				Release_Invoke();
				m_fEXS2ColTime = 0.f;

				m_bEXS2Distance = false;
				return;
			}

			if (m_fEXS2ColTime >= 3.5f)
			{
				m_fEXS2Distance += fTimeDelta * 10.f;
				_matrix matRot;
				_vec3 vDir;
				_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
				D3DXMatrixRotationY(&matRot, 45.f * Deg2Rad);
				D3DXVec3TransformNormal(&vDir, &vLook, &matRot);
				D3DXVec3Normalize(&vDir, &vDir);
				CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position() + _vec3(-5.f, 0.01f, 7.5f), vDir, 5.f, m_fEXS2Distance, E_LAYERTAG::Character, Collision_Player);
			}
		}
	}
}

void CHod::AreaLaser_Update(const _float& fTimeDelta)
{
	CDMesh* pMeshCom = static_cast<CDMesh*>(Get_Component(ComTag_DMesh));
	const D3DXFRAME_DERIVED* pFrame = pMeshCom->Get_FrameByName("bone_Head_01");
	const _matrix* pBoneMatrix = &pFrame->CombinedTransformMatrix;
	const _matrix* pWorldMatrix = &Get_Transform()->Get_WorldMatrix();
	_matrix matParent = (*pBoneMatrix * *pWorldMatrix);

	_matrix matRot;
	_vec3 vLook = _vec3(matParent._31, matParent._32, matParent._33);
	_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, vLook);

	m_pAreaLaser->Set_Pos(_vec3(matParent._41, matParent._42, matParent._43));
	m_pAreaLaser->Set_Rotation(vRot);
	m_pAreaLaser->Set_Scale(3.f, 3.f, 3.f);

	m_fQuakeTime += fTimeDelta;
	if (m_fQuakeTime >= 5.f)
	{
		if (m_bLaserSound)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Laser_Attack.wav", 1.f);
			m_bLaserSound = false;
		}

		map<E_OBJTAG, vector<CGameObject*>>* pMap = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::Map);

		_vec3	vStartPos = m_pAreaLaser->Get_Position();
		_vec3	vRayDir = m_pAreaLaser->Get_Info(E_INFO::INFO_LOOK);

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

		m_fColDistance += fTimeDelta * 10.f;

		//단타
		//CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position() + _vec3(0.f, 0.01f, 0.f), Get_Info(E_INFO::INFO_LOOK), 5.f, m_fExDistance, E_LAYERTAG::Character, Collision_Player);

		//연타
		CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position() + vLook * m_fColDistance, Get_Info(E_INFO::INFO_LOOK), 3.f, 1.f, E_LAYERTAG::Monster, Collision_Player);
	}
}

void CHod::Collision_Attack(CGameObject * pPlayer)
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

void CHod::Trigger_1()
{
}

void CHod::Trigger_2()
{
}

void CHod::Trigger_3()
{
	CSkillObject* pFade = CSkill_Manager::GetInstance()->SetSkill(L"Hod_Fade.skill", E_LAYERTAG::Effect, CCamera::m_pMainCam->Get_CamAt(), VEC_ZERO, 3.f);
	_vec3	vPos = CCamera::m_pMainCam->Get_CamAt() + *D3DXVec3Normalize(&m_vOutTemp, &(CCamera::m_pMainCam->Get_CamAt() - CCamera::m_pMainCam->Get_CamEye()));
	pFade->Set_Look(vPos);
	pFade->Set_Rotation(_vec3(120.f, 0.f, 90.f));

	CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Hod_Fade.camdat", L"Hod_0", Get_Position(), Get_Angle().y);
}

void CHod::Trigger_4()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_Intro.wav", 3.f);
	CActionCam_Mgr::GetInstance()->Start_ActionCam_NotLoad(L"Hod.camdat", L"Hod_1", Get_Position(), Get_Angle().y);
}

void CHod::Trigger_5()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Hod.camdat", L"Hod_2", Get_Position(), Get_Angle().y);
}

void CHod::Trigger_6()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Hod.camdat", L"Hod_3", Get_Position(), Get_Angle().y);
}

void CHod::Trigger_7()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Hod.camdat", L"Hod_4", Get_Position(), Get_Angle().y);
}

void CHod::Animation_Event()
{
	m_iPatternRand = CWall512::GetInstance()->GetValue(0, 100);

	if ((E_ACTION)E_Hod_Anim::Normal_Attack_Delay == m_eCurrentAction)
	{
		m_fGage = 0.f;
		m_pGlowAnim = &m_tGlowAnim_ToNormal;
	}
	else if ((E_ACTION)E_Hod_Anim::EXS2 == m_eCurrentAction)	//내려찍기
	{
		CUI_Manager::GetInstance()->Set_Announcement(L"EXS2 가동", 1.f);
		m_pGlowAnim = &m_tGlowAnim_Arm;
		Normal_Attack_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Hod_Anim::EXS1 == m_eCurrentAction)	//차징공격
	{
		CUI_Manager::GetInstance()->Set_Announcement(L"EXS1 가동", 1.f);
		m_pGlowAnim = &m_tGlowAnim_Arm;
		Charging_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Hod_Anim::EXS3 == m_eCurrentAction)	//기믹생성
	{
		CUI_Manager::GetInstance()->Set_Announcement(L"EXS3 가동", 1.f);

		m_pGlowAnim = &m_tGlowAnim_Arm;
		Create_Tower_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Hod_Anim::EXS4 == m_eCurrentAction)	//파장공격
	{
		CUI_Manager::GetInstance()->Set_Announcement(L"EXS4 가동", 1.f);

		m_pGlowAnim = &m_tGlowAnim_Arm;
		Wave_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Hod_Anim::EXS5 == m_eCurrentAction)	//광선발사
	{
		CUI_Manager::GetInstance()->Set_Announcement(L"EXS5 가동", 2.f);
		m_pGlowAnim = &m_tGlowAnim_FullBody;
		AreaLaser_Pattern(m_iPatternRand, true);
	}
	else if ((E_ACTION)E_Hod_Anim::Vital_Groggy == m_eCurrentAction)
	{
		m_bGroggySound = true;

		CUI_Manager::GetInstance()->Set_Announcement(L"비나가 기력을 다했어요, 지금이에요!", 2.f);

		CSkill_Manager::GetInstance()->SetSkill(L"Hieronymus_Groggy_Wave.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, VEC_ONE);
	}
}

void CHod::Ready_Glow()
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
	m_tGlowAnim_Arm.fGlowBright = 0.3f;

	m_tGlowAnim_FullBody.eGlowAnimMode = E_GlowAnimMode::Loop;
	m_tGlowAnim_FullBody.eEase = E_EASE::OutSine;
	m_tGlowAnim_FullBody.vStart_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vEnd_Min_UV = _vec2(0.f, 0.f);
	m_tGlowAnim_FullBody.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_FullBody.vBeginEnd_Power = _vec2(0.f, 1.f);
	m_tGlowAnim_FullBody.fGlowBlue = 1.f;
	m_tGlowAnim_FullBody.fGlowBright = 0.5f;

	m_tGlowAnim_ToNormal.eGlowAnimMode = E_GlowAnimMode::One;
	m_tGlowAnim_ToNormal.eEase = E_EASE::None;
	m_tGlowAnim_ToNormal.vStart_Min_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vStart_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vEnd_Min_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vEnd_Max_UV = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.vBeginEnd_Power = _vec2(1.f, 1.f);
	m_tGlowAnim_ToNormal.fGlowBlue = 1.f;
	m_tGlowAnim_ToNormal.fGlowBright = 0.3f;
}

void CHod::Glow_Setting(const _float & fTimeDelta)
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

void CHod::EX_HDR(const _float & fTimeDelta)
{
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 0.f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 0.65f;
}

void CHod::Ready_HDR()
{
	CRenderer::GetInstance()->Get_RenderOption().fLuminance = 0.068f;
	CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = 0.18f;
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 0.9f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 0.95f;
}

void CHod::Reset_HDR()
{
	CRenderer::GetInstance()->Get_RenderOption().fLuminance = 0.068f;
	CRenderer::GetInstance()->Get_RenderOption().fMiddleGray = 0.18f;
	CRenderer::GetInstance()->Get_RenderOption().fWhiteCutoff = 0.9f;
	CRenderer::GetInstance()->Get_RenderOption().fBrightFilter = 1.f;
}

void CHod::Normal_Attack_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
			//Ref Release
			if (m_pEXS2)
			{
				m_pEXS2->Set_Dead(TRUE);
				Safe_Release(m_pEXS2);
				m_pEXS2 = nullptr;
			}

			//Sound
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX2_Ready.wav", 3.f);
			Invoke_Add(3.5f, bind(&CHod::Invoke_EXS2, this));

			//Create Skill
			CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position() + _vec3(-5.f, 0.01f, 7.5f), Get_Angle() + _vec3(0.f, 45.f, 0.f), 15.f, 4.f, 4.f, TRUE);
			m_pEXS2 = CSkill_Manager::GetInstance()->SetSkill(L"Hod_EXS2.skill", E_LAYERTAG::Effect, Get_Position() + _vec3(-5.f, 0.01f, 7.5f), Get_Angle() + _vec3(0.f, 45.f, 0.f), VEC_ONE * 3.f);
			m_pEXS2->AddRef();
			assert(m_pEXS2);

			//Collision
			m_bEXS2Distance = true;
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

void CHod::Charging_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
			//Sound, Shake
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX1_Ready.wav", 3.f);
			Invoke_Add(1.3f, bind(&CHod::Invoke_EXS1, this));

			vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
			_int iRandChar = CWall512::GetInstance()->GetValue(0, 10000) % (_int)(&vecChar)->size();
			m_vecCharPos = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers()[iRandChar]->Get_Position();
			CWarning* pWarning = CWarning::Create(m_pGraphicDev);
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, m_vecCharPos, 4.6f, 0.f, 8.f, D3DXCOLOR(C_Red));
			pWarning->Set_AfterSkillName(L"Hod_EXS1_Elect.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_SoundName(L"SFX_HOD_EX1_Hit.wav", 4.f);
			pWarning->Set_Damage(500);
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
			//Random_Guide(true);
		}
	}
}

void CHod::Wave_Pattern(_int iRand, _bool bStart)
{
	if (iRand < 100)
	{
		if (bStart)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX4.wav", 3.f);

			CSkill_Manager::GetInstance()->SetSkill(L"Hod_EXS4.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 1.f);

			CWarning* pWarning = CWarning::Create(m_pGraphicDev);
			pWarning->Set_Warning(E_WarnType::Circle_Fill, E_WarnType::Circle_InsideFill, Get_Position(), 3.3f, 0.f, 30.f, D3DXCOLOR(C_Red));
			pWarning->Set_AfterSkillName(L"Hod_EXS4_2.skill", VEC_ZERO, VEC_ONE * 2.f);
			pWarning->Set_Damage(200);
			pWarning->Set_SoundName(L"SFX_HOD_EX4_Hit.wav", 3.f);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarning);

			m_bEX4Func = true;
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

void CHod::EX4_Function(const _float & fTimeDelta)
{
	if (m_bEX4Func)
	{
		m_bEX4FuncTime += fTimeDelta;

		if (m_bEX4FuncTime <= 3.3)
		{
			vector<CCharacter*> vecChar = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();

			for (auto& pChar : vecChar)
			{
				_vec3 vCharPos = pChar->Get_Position();
				_vec3 vHodPosDest = Get_Position();
				_vec3 vecCharDir = vHodPosDest - vCharPos;
				D3DXVec3Normalize(&vecCharDir, &vecCharDir);

				_vec3 vMoveAmount = vecCharDir * fTimeDelta * 2.f;
				pChar->Add_Pos(vMoveAmount);
			}
		}
		else
		{
			m_bEX4FuncTime = 0.f;
			m_bEX4Func = false;
		}
	}
}

void CHod::Create_Tower_Pattern(_int iRand, _bool bStart)
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

void CHod::AreaLaser_Pattern(_int iRand, _bool bStart)
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

void CHod::Random_Guide(_bool bRand)
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

void CHod::Invoke_EXS1()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Hod_EXS1_Charge.skill", E_LAYERTAG::Effect, m_pTentacle_Attack_L->GetPos(), Get_Angle(), VEC_ONE * 4.f);
	CScene::Get_MainScene()->Active_ShakeCam(3.3f, 0.05f);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX1_Attack.wav", 2.f);
}

void CHod::Invoke_EXS2()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HOD_EX2_Attack.wav", 3.f);
	CScene::Get_MainScene()->Active_ShakeCam(1.5f, 0.15f);
}

void CHod::Invoke_EXS4()
{
}

void CHod::Update_Collider(const _float & fTimeDelta)
{
	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
	Collider_SetMatrix(m_pWeaponCollider, m_szWeaponBoneName);

	Collider_SetMatrix(m_pTentacle_R, m_szTentacle_R);
	Collider_SetMatrix(m_pTentacle_RB, m_szTentacle_RB);
	Collider_SetMatrix(m_pTentacle_L, m_szTentacle_L);
	Collider_SetMatrix(m_pTentacle_LB, m_szTentacle_LB);

	Collider_SetMatrix(m_pBone_LaserGun_L, m_szLaserGun_L);
	Collider_SetMatrix(m_pBone_LaserGun_R, m_szLaserGun_R);
	Collider_SetMatrix(m_pHead, m_szHead);
	Collider_SetMatrix(m_pBone_Body, m_szBone_Body);

	Collider_SetMatrix(m_pTentacle_Attack_L, m_szTentacle_Attack_L);
}

void CHod::AreaLaser_Skill()
{
	if (m_pAreaLaser)
	{
		m_pAreaLaser->Set_Dead(TRUE);
		Safe_Release(m_pAreaLaser);
		m_pAreaLaser = nullptr;
	}

	m_pAreaLaser = CSkill_Manager::GetInstance()->SetSkill(L"Binah_Normal_Laser.skill", E_LAYERTAG::Effect, Get_Position(), VEC_ZERO, 3.f);
	m_pAreaLaser->AddRef();
	assert(m_pAreaLaser);

	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Binah_Laser_Charge.wav", 1.f);
}

void CHod::Collision_Player(CGameObject * pThis, CGameObject * pPlayer)
{
	static_cast<CHod*>(pThis)->Collision_Attack(pPlayer);
}