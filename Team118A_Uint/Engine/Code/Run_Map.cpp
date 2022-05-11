#include "Run_Map.h"

#include "Scene.h"
#include "Run_Player.h" 
#include "Run_Chesed.h" 
#include "Run_Drone.h"
#include "Run_Obstacle.h"
#include "Camera.h" 
#include "QuatCamera.h" 

#include "Renderer.h"
#include "FogManager.h"
#include "ProtoMgr.h"
#include "KeyManager.h" 
#include "Sound_Manager.h" 
#include "ActionCam_Mgr.h" 
#include "Skill_Manager.h" 
#include "UI_Manager.h" 

USING(Engine)

CRun_Map::CRun_Map(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CRun_Map::CRun_Map(const CRun_Map & rhs)
	: CGameObject(rhs)
{
}

CRun_Map::~CRun_Map()
{
}

CRun_Map * CRun_Map::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _bool bCutSceneMap) 
{
	CRun_Map*	pInstance = new CRun_Map(pGraphicDev, eObjTag); 

	if (FAILED(pInstance->Ready_Object(bCutSceneMap))) 
		Safe_Release(pInstance); 

	return pInstance; 
}

void CRun_Map::Free()
{
	CSound_Manager::GetInstance()->StopSound(E_ChannelID::BGM); 

	if (m_pSkillWarningEffect)
	{
		m_pSkillWarningEffect->Set_Dead(TRUE);
		m_pSkillWarningEffect = nullptr;
	}
	//if (m_pFadeInOutUI)
	//{ 
	//	m_pFadeInOutUI->Set_Dead(TRUE); 
	//	m_pFadeInOutUI = nullptr; 
	//}

	CGameObject::Free(); 
}

void CRun_Map::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte); 
}

void CRun_Map::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte); 
}


HRESULT CRun_Map::Ready_Object(_bool bCutSceneMap)
{
	CGameObject::Ready_Object(); 

	Add_Component(); 

	Set_Scale(VEC_ONE * 6.5f); 
	Set_Rotation(E_ROTATION::ROT_Z, 90.f); 

	return S_OK;
}

_int CRun_Map::Start_Object(const _float & fTimeDelta) 
{
	_int	iExit = CGameObject::Start_Object(fTimeDelta); 

	//m_pFadeInOutUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Fade.uidat");
	//assert(m_pFadeInOutUI);
	//CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pFadeInOutUI);
	//m_pFadeInOutUI->Set_Active(TRUE);
	//m_pFadeInOutUI->Set_ImageAlpha(0.f);

	/////////// 임시 
	//CGameObject*	pGameObject = CRun_Chesed::Create(m_pGraphicDev, L"Chesed", E_OBJTAG::OBJ_Run_BossMonster);
	//assert(pGameObject);
	//CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster, pGameObject);

	//m_pChesed = static_cast<CRun_Chesed*>(pGameObject);
	//assert(m_pChesed);

	//m_pChesed->Set_PatternChange(CRun_Chesed::Pattern_Spawn);

	//static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Set_StartGame_Player();
	//m_bGameStart = TRUE;

	//CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Run_Map_2.camdat", L"1", Get_Position(), 0.f, CActionCam_Mgr::ActionCam_Observe); 
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_보스전_헤세드_2.mp3", 0.5f); 

	//m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, 0.5f);

	CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_QUAT);
	static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Set_StartGame_Player();
	static_cast<CQuatCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Quat))->Set_StartGame_Camera();

	return iExit;
}

_int CRun_Map::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Update_Object(fTimeDelta); 

	//Update_CheckGameStart(fTimeDelta); 

	if (!m_bGameStart)
	{
		return iExit; 
	}

	Spawn_Chesed(fTimeDelta);
	Spawn_Drone(fTimeDelta);

	Appear_Obstacle(fTimeDelta);
	Update_BossWarningEffect(fTimeDelta);

	return iExit;
}

_int CRun_Map::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

void CRun_Map::Render_Object(_int iPass)
{
	CGameObject::Render_Object(iPass);

	Set_Constantable();

	m_pShaderCom->BeginShader(); 
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::RunMap, 0));

	if (m_pSMeshCom)
	{
		m_pSMeshCom->Render_Meshes(m_pShaderCom);
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CRun_Map::Debug_This_In_Console()
{
}

void CRun_Map::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix	matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// 포그
	pEffect->SetVector("g_vFogColor", &(_vec4)(CFogManager::GetInstance()->Get_FogColor()));
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	CFogManager::GetInstance()->Set_FogEnd(10000.f);
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
}

void CRun_Map::Set_Constantable_Fog(CShader * pShader)
{
}

void CRun_Map::OnEnable()
{
}

void CRun_Map::OnDisable()
{
}

void CRun_Map::OnDestroy()
{
}

void CRun_Map::On_Collision(CGameObject * pDest)
{
}

void CRun_Map::Add_Component()
{
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	CComponent*		pMesh = CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Run_Map");

	if (!pMesh)
	{
		if (E_FAIL == CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_SMesh_Run_Map", 
			CSMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Map/Runner/Donut/", L"Donut.X")))
		{
			assert(FALSE);
		}

		pMesh = CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SMesh_Run_Map");
	}

	m_pSMeshCom = static_cast<CSMesh*>(pMesh);
	Emplace_Component(ComTag_SMesh, m_pSMeshCom); 
}

void CRun_Map::Spawn_Chesed(const _float & fTimeDelta)
{
	if (3 == m_iPhase && 
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster) && 
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster) && 
		!m_bObstacleTime)
	{
		if (!m_pSkillWarningEffect) 
		{
			CSound_Manager::GetInstance()->PlaySound_(L"SE_보스등장.wav", E_ChannelID::Channel2_1, E_SoundType::FX, 0.75f);

			m_pSkillWarningEffect = CSkill_Manager::GetInstance()->SetSkill(L"Warning.skill", E_LAYERTAG::Effect, CCamera::m_pMainCam->Get_CamEye() + CCamera::m_pMainCam->Get_CamLookNormalize(), VEC_ZERO, 1.f);
			assert(m_pSkillWarningEffect);
			m_pSkillWarningEffect->Set_Scale(VEC_ONE * 20.f);
		}

		m_fChesedSpawnDelay -= fTimeDelta; 

		if (0.f >= m_fChesedSpawnDelay)
		{
			++m_iPhase;

			CGameObject*	pGameObject = CRun_Chesed::Create(m_pGraphicDev, L"Chesed", E_OBJTAG::OBJ_Run_BossMonster);
			assert(pGameObject);
			CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster, pGameObject);

			m_pChesed = static_cast<CRun_Chesed*>(pGameObject);
			assert(m_pChesed);

			m_pChesed->Set_PatternChange(CRun_Chesed::Pattern_Spawn);
		}
	}
}

void CRun_Map::Spawn_Drone(const _float & fTimeDelta)
{
	if (!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster) && 
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster) &&
		3 > m_iPhase)
	{
		m_fDroneSpawnDelay -= fTimeDelta; 

		if (0.f >= m_fDroneSpawnDelay)
		{
			m_fDroneSpawnDelay = m_fDroneOriginSpawnDelay; 

			if (0 == m_iPhase)
			{
				m_iDroneSpawnCount = 4;

				for (_uint i = 0; i < m_iDroneSpawnCount; ++i)
				{
					CGameObject*	pGameObject = CRun_Drone::Create(m_pGraphicDev, L"Drone_Decagram_Vulcan", i * 360.f / m_iDroneSpawnCount, E_DRONE_ATKTYPE::DRONE_ROUNDED, m_iDroneSpawnCount, E_OBJTAG::OBJ_Run_Monster);
					assert(pGameObject);
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, pGameObject);
				}
			}
			else if (1 == m_iPhase) 
			{
				m_iDroneSpawnCount = 6;

				for (_uint i = 0; i < m_iDroneSpawnCount; ++i)
				{
					CGameObject*	pGameObject = CRun_Drone::Create(m_pGraphicDev, L"Drone_Decagram_Vulcan", i * 360.f / m_iDroneSpawnCount, E_DRONE_ATKTYPE::DRONE_ROTATE, m_iDroneSpawnCount, E_OBJTAG::OBJ_Run_Monster);
					assert(pGameObject);
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, pGameObject);
				}
			}
			else if (2 == m_iPhase) 
			{
				m_iDroneSpawnCount = 5;

				for (_uint i = 0; i < m_iDroneSpawnCount; ++i)
				{
					CGameObject*	pGameObject = CRun_Drone::Create(m_pGraphicDev, L"Drone_Decagram_Vulcan", i * 360.f / m_iDroneSpawnCount, E_DRONE_ATKTYPE::DRONE_ROTATE, m_iDroneSpawnCount, E_OBJTAG::OBJ_Run_Monster);
					assert(pGameObject);
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, pGameObject);
				}
			}

			++m_iPhase; 
		}
	}
}

void CRun_Map::Ready_DroneSpawn(_uint iDroneCount)
{
	m_fDroneSpawnDelay = m_fDroneOriginSpawnDelay; 
	m_iDroneSpawnCount = iDroneCount; 
}

void CRun_Map::Appear_Obstacle(const _float & fTimeDelta)
{
	if (2 == m_iPhase && 
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster))
	{
		m_fObstacleAppearDealy -= fTimeDelta; 

		if (0.f >= m_fObstacleAppearDealy)
		{
			m_bObstacleTime = TRUE; 
		}
	}
	if (!m_bObstacleTime || 
		!CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))
	{
		return; 
	}

	m_fObstacleDelay -= fTimeDelta; 

	if (0.f >= m_fObstacleDelay && 
		m_iObstacleSpawnCount <= m_iObstacleMaxSpawnCount)
	{
		++m_iObstacleSpawnCount; 

		m_fObstacleDelay = m_fObstacleOriginDelay; 
		m_fObstacleOriginDelay = CWall512::GetInstance()->GetFloatValue(0.75f, 2.5f); 

		_float	fPlayerAngle = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Get_RunAngle(); 

		CGameObject*	pGameObject = CRun_Obstacle::Create(m_pGraphicDev, fPlayerAngle, E_LAYERTAG::Character); 
		assert(pGameObject); 
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject); 
	}
	else if (m_iObstacleSpawnCount > m_iObstacleMaxSpawnCount)
	{
		m_bObstacleTime = FALSE; 

		m_iObstacleSpawnCount = 0; 
		m_iObstacleMaxSpawnCount = CWall512::GetInstance()->GetValue(0, 8); 

		2 == m_iPhase ? ++m_iPhase : m_iPhase; 
	}
}

void CRun_Map::Update_BossWarningEffect(const _float & fTimeDelta)
{
	if (m_pSkillWarningEffect)
	{
		m_fWarningEffectTime -= fTimeDelta;

		if (0.f >= m_fWarningEffectTime)
		{
			m_pSkillWarningEffect->Set_Dead(TRUE);
			m_pSkillWarningEffect = nullptr;

			return;
		}

		m_pSkillWarningEffect->Set_Pos(CCamera::m_pMainCam->Get_CamAt());
	}
}

void CRun_Map::Update_CheckGameStart(const _float & fTimeDelta)
{
	//m_fCutSceneDelay -= fTimeDelta; 

	//if (0 == m_iCutSceneCount)
	//{
	//	_vec3	vDir = VEC_UP;
	//	CUtility::Rotate_DirVector(20.f, VEC_LOOK, &vDir);
	//	CUtility::Rotate_DirVector(m_fCutSceneAngle, VEC_UP, &vDir);

	//	m_fCutSceneAngle += 35.f * fTimeDelta;
	//	m_fCutSceneLength -= 115.f * fTimeDelta;
	//	CCamera::m_pMainCam->Set_CamEye(CCamera::m_pMainCam->Get_CamAt() + vDir * m_fCutSceneLength);

	//	if (0.f >= m_fCutSceneDelay)
	//	{
	//		++m_iCutSceneCount;
	//	}
	//}
	//else if (0.f >= m_fCutSceneDelay &&
	//		 1 == m_iCutSceneCount)
	//{
	//	m_fCutSceneDelay = 6.5f;
	//	++m_iCutSceneCount;

	//	if (m_pFadeInOutUI)
	//	{
	//		m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, 0.1f);
	//	}
	//}

	//if (m_pFadeInOutUI)
	//{
	//	if (1.f <= m_pFadeInOutUI->Get_ImageColor().a)
	//	{
	//		m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, 0.5f);

	//		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_QUAT);
	//		static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player))->Set_StartGame_Player();
	//		static_cast<CQuatCamera*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_Quat))->Set_StartGame_Camera();
	//	}
	//}
}
