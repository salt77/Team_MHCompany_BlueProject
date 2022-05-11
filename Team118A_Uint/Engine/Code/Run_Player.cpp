#include "Run_Player.h"

#include "Run_Chesed.h" 
#include "Run_Drone.h" 
#include "Run_Map.h"
#include "Bullet_Run_Normal.h"
#include "Scene.h"
#include "DMesh.h" 
#include "UI_Object.h" 
#include "QuatCamera.h" 

#include "KeyManager.h"
#include "ProtoMgr.h"
#include "UI_Manager.h" 
#include "TimeScale_Manager.h"

CRun_Player::CRun_Player(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag)
	: CRun_Unit(pGraphicDev, wstrUnitName, eObjTag)
{
	//m_vecParticle_FireMuzzle.reserve(10);
}

CRun_Player::CRun_Player(const CRun_Player & rhs)
	: CRun_Unit(rhs)
{
}

CRun_Player::~CRun_Player()
{
}

CRun_Player * CRun_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrUnitName, E_OBJTAG eObjTag)
{
	CRun_Player*	pInstance = new CRun_Player(pGraphicDev, wstrUnitName, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CRun_Player::Free()
{
	if (m_pSkillNormalAttack)
	{
		if (!m_pSkillNormalAttack->Get_Dead())
		{
			m_pSkillNormalAttack->Set_Dead(TRUE);
		}

		m_pSkillNormalAttack = nullptr;
	}
	for (_uint i = 0; i < sizeof(m_pSkillEx_2) / sizeof(CSkillObject*); ++i)
	{
		if (m_pSkillEx_2[i])
		{
			if (!m_pSkillEx_2[i]->Get_Dead())
			{
				m_pSkillEx_2[i]->Set_Dead(TRUE);
			}

			m_pSkillEx_2[i] = nullptr;
		}
	}
	if (m_pParticleSkillEx1)
	{
		if (!m_pParticleSkillEx1->Get_Dead())
		{
			m_pParticleSkillEx1->Set_ParticleDead();
			m_pParticleSkillEx1->Set_Dead(TRUE);
		}

		m_pParticleSkillEx1 = nullptr;
	}
	if (m_pSkillChargeEffect)
	{
		m_pSkillChargeEffect->Set_Dead(TRUE);
		m_pSkillChargeEffect = nullptr;
	}
	if (m_pParticleRunSpark)
	{
		m_pParticleRunSpark->Set_Dead(TRUE);
		m_pParticleRunSpark = nullptr;
	}

	//vector<CParticleObject*>::iterator	iter = m_vecParticle_FireMuzzle.begin();

	//for (; iter != m_vecParticle_FireMuzzle.end(); ++iter)
	//{
	//	(*iter)->Set_ParticleDead();
	//	(*iter)->Set_Dead(TRUE);
	//}

	CUI_Manager::GetInstance()->Set_ClearAll_RunCombatUI();

	CRun_Unit::Free();
}

void CRun_Player::Save(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Save(hFile, dwByte);
}

void CRun_Player::Load(HANDLE hFile, DWORD & dwByte)
{
	CRun_Unit::Load(hFile, dwByte);
}


HRESULT CRun_Player::Ready_Object()
{
	CRun_Unit::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";

	Add_Component();

	//Set_Scale(VEC_ONE * 15.f); 
	m_fScale = 8.f;

	m_tUnitInfo.iMaxHp = 100;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;

	return S_OK;
}

_int CRun_Player::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Start_Object(fTimeDelta);

	m_fEndGameDelay = 2.f;

	_vec3	vUp = VEC_UP;
	_vec3	vDown = VEC_DOWN;

	Calculate_Dis(nullptr, Get_Position(), &vUp, &m_fDistanceFromMap);

	_vec3	vPipeUpPos = VEC_ZERO;
	_vec3	vPipeDownPos = VEC_ZERO;
	_vec3	vSkyPos = Get_Position() + (VEC_UP * m_fDistanceFromMap * 3.f);

	Calculate_Dis(nullptr, vSkyPos, &vDown, &m_fDistancePipe);

	vPipeUpPos = vSkyPos + (VEC_DOWN * m_fDistancePipe);

	vPipeDownPos = Get_Position() + VEC_UP * m_fDistanceFromMap;
	m_fDistancePipe = D3DXVec3Length(&(vPipeUpPos - vPipeDownPos)) * 0.5f;

	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));

	if (!m_pRunMap)
	{
		MSG_BOX("CRun_Player -> Start_Object() : m_pRunMap 없음");
	}

	Set_Scale(VEC_ONE * m_pRunMap->Get_Scale().x * m_fScale);
	m_pColliderCom->SetScale(Get_Scale() * 0.5f);

	//m_pParticleRunSpark = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Run_Particle.particle");
	//assert(m_pParticleRunSpark);
	//CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleRunSpark);
	//m_pParticleRunSpark->Set_Pos(Get_Position());
	//m_pParticleRunSpark->Set_Scale(Get_Scale());

	CUI_Manager::GetInstance()->Create_Run_Combat_UI();

	return iExit;
}

_int CRun_Player::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::Update_Object(fTimeDelta);

	m_fCountingTime += fTimeDelta;

	if (CKeyManager::GetInstance()->Key_Down(KEY_G))
	{
		m_bGodMode = TRUE; 
		m_fGodModeInterpol = 999.f; 
	}

	if (m_bStart)
	{
		Key_Input(fTimeDelta);
		Lining_Run(fTimeDelta);
	}

	Update_FSM(fTimeDelta);

	Update_WorldMatrix();
	Update_Collider(fTimeDelta);
	Update_Particle();
	Update_CheckSpeedChange(fTimeDelta);
	Update_GodModeTime(fTimeDelta);
	Update_ExSkill(fTimeDelta);
	Update_SkillGage(fTimeDelta);
	Update_EndGame(fTimeDelta); 

	Set_ActionByEnum(m_eAction);
	m_pDMeshCom->Set_AnimSpeed(1.f * CTimeScale_Manager::GetInstance()->Get_TimeScale());
	m_pDMeshCom->Play_Animation(fTimeDelta);

	return iExit;
}

_int CRun_Player::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CRun_Unit::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CRun_Player::Render_Object(_int iPass)
{
	if (!m_bStart || !m_bGameRender)
	{
		return; 
	}

	CRun_Unit::Render_Object(iPass);
}

void CRun_Player::Debug_This_In_Console()
{
}

void CRun_Player::Set_Constantable()
{
	CRun_Unit::Set_Constantable();
}

void CRun_Player::Set_Constantable_Fog(CShader * pShader)
{
	CRun_Unit::Set_Constantable_Fog(pShader);
}

void CRun_Player::Set_Damage(const _float & fDamage)
{
	CRun_Unit::Set_Damage(fDamage);

	m_bGodMode = TRUE;
	m_fGodModeTime = 2.f * m_fGodModeInterpol;

	CUI_Manager::GetInstance()->Active_RunHurtScreenUI(); 
}

void CRun_Player::Set_SideRotateSpeed(const _float & fRatio, const _float & fTime)
{
	m_fSpeed = m_fOriginSpeed * fRatio * Half;
	m_fRotatePipeSpeed = m_fRotatePipeOriginSpeed * fRatio;

	m_fSpeedDownTime = fTime;
}

void CRun_Player::Set_EndGame_Player(const _float& fEndDelay)
{
	m_bEndVictory = TRUE; 
	m_fEndGameDelay = fEndDelay; 
}

void CRun_Player::OnEnable()
{
	CRun_Unit::OnEnable();
}

void CRun_Player::OnDisable()
{
	CRun_Unit::OnDisable();
}

void CRun_Player::OnDestroy()
{
	CRun_Unit::OnDestroy();
}

void CRun_Player::On_Collision(CGameObject * pDest)
{
	CRun_Unit::On_Collision(pDest); 
}

void CRun_Player::Key_Input(const _float& fTimeDelta)
{
	if (E_ACTION::Vital_Death == m_eAction || 
		!m_bGameRender || 
		m_bEndVictory)
	{
		return; 
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_A))
	{
		m_vWeaponPos = Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Y] * 10.f + m_vAxis[(_uint)E_ROTATION::ROT_Z] * 25.f - m_vAxis[(_uint)E_ROTATION::ROT_X] * 5.f;
	}
	else if (CKeyManager::GetInstance()->Key_Pressing(KEY_D))
	{
		m_vWeaponPos = Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Y] * 10.f + m_vAxis[(_uint)E_ROTATION::ROT_Z] * 25.f + m_vAxis[(_uint)E_ROTATION::ROT_X] * 5.f;
	}
	else
	{
		m_vWeaponPos = Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Y] * 10.f + m_vAxis[(_uint)E_ROTATION::ROT_Z] * 25.f;
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_LBUTTON))
	{
		if (m_fFireDelay <= m_fCountingTime)
		{
			if (!m_pRunMap)
			{
				return;
			}

			m_fCountingTime = 0.f; 

			m_pSkillNormalAttack = CSkill_Manager::GetInstance()->SetSkill(L"Run_Midori_AttackNormal.skill",
																		   E_LAYERTAG::Effect,
																		   m_vWeaponPos,
																		   Get_Angle(),
																		   Get_Scale());
			assert(m_pSkillNormalAttack);

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster))
			{
				m_iAttackTarget = -1;

				vector<CGameObject*>*	vecDrones = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster);
				for (_uint i = 0; i < vecDrones->size(); ++i)
				{
					if (0.f >= static_cast<CRun_Drone*>((*vecDrones)[i])->Get_UnitInfo().iNowHp)
					{
						continue;
					}

					_vec3	vDir = (*vecDrones)[i]->Get_Position() - Get_Position();
					D3DXVec3Normalize(&vDir, &vDir);

					if (25.f >= D3DXToDegree(acos(D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &vDir))))
					{
						m_iAttackTarget = i;

						break;
					}
				}

				CGameObject*	pDrone = CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, m_iAttackTarget); 

				if (pDrone)
				{
					m_fSkillGage += 0.075f;
					static_cast<CRun_Unit*>(pDrone)->Set_Damage(-CWall512::GetInstance()->GetFloatValue(50.f, 75.f));
					CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, pDrone->Get_Position(), Get_Angle(), Get_Scale() * 0.75f);
					CUI_Manager::GetInstance()->Add_RunComboHit(1);
					CCamera::m_pMainCam->Set_ShakeCamera(0.5f, 0.25f);
				}
			}
			else if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				CRun_Chesed*	pBoss = static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster));
				pBoss->Set_Damage(-CWall512::GetInstance()->GetFloatValue(500.f, 750.f));
				CUI_Manager::GetInstance()->Add_RunComboHit(1);
				CCamera::m_pMainCam->Set_ShakeCamera(0.5f, 0.25f);
				m_fSkillGage += 0.05f;

				CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, pBoss->Get_Position(), Get_Angle(), Get_Scale());
			}

			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Laser_Short_01.wav", 1.75f);
		}
	}
	else if (0.33f < m_fSkillGage &&
			 CKeyManager::GetInstance()->Key_Pressing(KEY_RBUTTON))
	{
		for (_uint i = 0; i < sizeof(m_pSkillEx_2[i]) / sizeof(CSkillObject*); ++i)
		{
			if (m_pSkillEx_2[i])
			{
				return;
			}
		}
		if (m_pParticleSkillEx1)
		{
			return;
		}
		if (!m_pSkillChargeEffect)
		{
			m_pSkillChargeEffect = CSkill_Manager::GetInstance()->SetSkill(L"Run_Midori_ChargeEx.skill",
																		   E_LAYERTAG::Effect,
																		   m_vWeaponPos,
																		   Get_Angle(),
																		   Get_Scale() * 0.75f);
			assert(m_pSkillChargeEffect);

			CSound_Manager::GetInstance()->PlaySound_(L"Charge_Laser.wav", E_ChannelID::Channel2_20, E_SoundType::FX, 2.5f); 
		}
		else
		{
			m_pSkillChargeEffect->Set_Pos(m_vWeaponPos);
		}

		m_fSkillChargeDelay += fTimeDelta;
	}

	if (CKeyManager::GetInstance()->Key_Up(KEY_RBUTTON))
	{
		if (m_pSkillChargeEffect)
		{
			m_pSkillChargeEffect->Set_Dead(TRUE);
			m_pSkillChargeEffect = nullptr; 

			CSound_Manager::GetInstance()->StopSound(E_ChannelID::Channel2_20); 

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				if (2.f < m_fSkillChargeDelay)
				{
					m_fSkillGage -= 0.66f;

					m_pParticleSkillEx1 = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Run_Midori_Skill_1.particle");
					assert(m_pParticleSkillEx1);
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleSkillEx1);
					m_pParticleSkillEx1->Set_Scale(Get_Scale() * 1.5f);

					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Run_Ex_2.wav", 0.75f);
				}
				else if (0.33f <= m_fSkillGage)
				{
					m_fSkillGage -= 0.33f;

					m_bEx2Fire = TRUE;
				}
			}
			else
			{
				if (0.33f <= m_fSkillGage)
				{
					m_fSkillGage -= 0.33f;

					m_pParticleSkillEx1 = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Run_Midori_Skill_1.particle");
					assert(m_pParticleSkillEx1);
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticleSkillEx1);
					m_pParticleSkillEx1->Set_Scale(Get_Scale() * 1.5f);

					CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Run_Ex_2.wav", 0.75f);
				}
			}

			m_fSkillChargeDelay = 0.f;
		}
	}

	if (m_pSkillNormalAttack)
	{
		if (m_pSkillNormalAttack->Get_Dead())
		{
			m_pSkillNormalAttack = nullptr;
		}
		else
		{
			m_pSkillNormalAttack->Set_Pos(m_vWeaponPos);

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster) &&
				-1 != m_iAttackTarget)
			{
				CGameObject*	pGameObject = CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster, m_iAttackTarget);

				if (pGameObject)
				{
					m_pSkillNormalAttack->Set_Direction(*D3DXVec3Normalize(&m_vOutTemp, &(pGameObject->Get_Position() - m_vWeaponPos)));
				}
			}
			else if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				m_pSkillNormalAttack->Set_Direction(*D3DXVec3Normalize(&m_vOutTemp, &(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)->Get_Position() - m_vWeaponPos)));
			}
			else
			{
				//_vec3	vDir = VEC_UP; 
				//CUtility::Rotate_DirVector(m_fRunAngle + 40.f, VEC_RIGHT, &vDir); 
				//_vec3	vTargetPos = m_pRunMap->Get_Position() + vDir * m_fDistanceFromMap; 
				//vTargetPos += vDir * m_fDistancePipe; 

				//m_pSkillNormalAttack->Set_Look(vTargetPos); 
				m_pSkillNormalAttack->Set_Direction(m_vAxis[(_uint)E_ROTATION::ROT_Z] * 3.f + m_vAxis[(_uint)E_ROTATION::ROT_Y]);
			}
		}
	}
	for (_uint i = 0; i < sizeof(m_pSkillEx_2) / sizeof(CSkillObject*); ++i)
	{
		if (m_pSkillEx_2[i])
		{
			if (m_pSkillEx_2[i]->Get_Dead())
			{
				m_pSkillEx_2[i] = nullptr;

				continue;
			}

			m_pSkillEx_2[i]->Set_Pos(m_vWeaponPos);

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				m_pSkillEx_2[i]->Set_Direction(*D3DXVec3Normalize(&m_vOutTemp, &(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)->Get_Position() - m_vWeaponPos)));
			}
		}
	}
	if (m_pParticleSkillEx1)
	{
		if (m_pParticleSkillEx1->Get_Dead())
		{
			m_pParticleSkillEx1 = nullptr;

			return;
		}

		m_pParticleSkillEx1->Set_Pos(m_vWeaponPos);

		if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
		{
			m_pParticleSkillEx1->Set_Direction(*D3DXVec3Normalize(&m_vOutTemp, &(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)->Get_Position() - m_vWeaponPos)));
		}
		else
		{
			_vec3	vDir = VEC_UP; 
			CUtility::Rotate_DirVector(m_fRunAngle + 25.f, VEC_RIGHT, &vDir); 

			m_pParticleSkillEx1->Set_Look(m_pRunMap->Get_Position() + vDir * Get_DistanceOriginToPipeMid()); 
		}
	}
}

void CRun_Player::Lining_Run(const _float & fTimeDelta)
{
	if (E_ACTION::Vital_Death == m_eAction)
	{
		return;
	}

	CRun_Map*	pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));

	if (!pRunMap)
		return;

	_vec3	vRayStartPos = VEC_ZERO;
	_vec3	vOriginToPipeMidDir = VEC_UP;

	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vOriginToPipeMidDir);

	vOriginToPipeMidDir = pRunMap->Get_Position() + vOriginToPipeMidDir * (m_fDistanceFromMap + m_fDistancePipe);
	m_vPipeMidPos = pRunMap->Get_Position() + vOriginToPipeMidDir;

	m_vDirToPipeMidAhead = vOriginToPipeMidDir;
	CUtility::Rotate_DirVector(0.1f, VEC_RIGHT, &m_vDirToPipeMidAhead);

	_vec3	vOriginToPipeMid = *D3DXVec3Normalize(&vOriginToPipeMid, &vOriginToPipeMidDir);
	_vec3	vPipeMidFront = *D3DXVec3Cross(&vPipeMidFront, &VEC_RIGHT, &vOriginToPipeMid);
	_vec3	vPipeMidAhead = *D3DXVec3Cross(&vPipeMidAhead, &VEC_RIGHT, &m_vDirToPipeMidAhead);
	_float	fRotateAngleY = D3DXToDegree(acos(D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Z], D3DXVec3Normalize(&vPipeMidAhead, &vPipeMidAhead))));

	if (-360.f <= fRotateAngleY && 360.f > fRotateAngleY)
	{
		if (0.f < D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Y],
			D3DXVec3Cross(&vPipeMidAhead, &vPipeMidAhead, &m_vAxis[(_uint)E_ROTATION::ROT_Z])))
		{
			Add_Rotation_Quat(E_ROTATION::ROT_Y, -fRotateAngleY);
		}
		else
		{
			Add_Rotation_Quat(E_ROTATION::ROT_Y, fRotateAngleY);
		}
	}

	m_vTargetDir = *D3DXVec3Normalize(&m_vTargetDir, &(CUtility::Rotate_DirVector(m_fPipeAngle, vPipeMidFront, &vOriginToPipeMidDir)));
	vRayStartPos = m_vPipeMidPos + (*D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir) * m_fDistanceFromMap * Half);
	_vec3	vRayToPipeMid = *D3DXVec3Normalize(&vRayToPipeMid, &(m_vPipeMidPos - vRayStartPos));
	_float	fDistanceToMap = 0.f;

	_matrix		matWorld;
	_vec3		vRayPos = vRayStartPos;
	_vec3		vRayDir = vRayToPipeMid;
	D3DXMatrixInverse(&matWorld, NULL, &static_cast<CTransform*>(pRunMap->Get_Component(ComTag_Transform))->Get_WorldMatrix());
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	//////////////////////////////////////////////////////////////////

	_vec3 v1, v2, v3;
	ZeroMemory(&v1, sizeof(_vec3));
	ZeroMemory(&v2, sizeof(_vec3));
	ZeroMemory(&v3, sizeof(_vec3));

	LPD3DXMESH pMesh = pRunMap->Get_Mesh();

	float fU = 0.f, fV = 0.f;

	int iTriCount = pMesh->GetNumFaces();

	void* pIndices;
	void* pVertices;

	pMesh->LockVertexBuffer(0, &pVertices);
	pMesh->LockIndexBuffer(0, &pIndices);

	_vec3* VtxBuffer;
	VtxBuffer = static_cast<CSMesh*>(pRunMap->Get_Component(ComTag_SMesh))->Get_VtxPos();

	m_fDistanceRayToPlayer = (_float)INT_MAX;
	_float	fDist = 0.f;
	_float	fRotateAngle = 0.f;

	for (int i = 0; i < iTriCount; ++i)
	{
		WORD _1, _2, _3;
		memcpy(&_1, (char*)pIndices + i * sizeof(INDEX16), sizeof(WORD));
		memcpy(&_2, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD), sizeof(WORD));
		memcpy(&_3, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD) * 2, sizeof(WORD));

		memcpy(&v1, VtxBuffer[_1], sizeof(_vec3));
		memcpy(&v2, VtxBuffer[_2], sizeof(_vec3));
		memcpy(&v3, VtxBuffer[_3], sizeof(_vec3));

		if (D3DXIntersectTri(&v1, &v2, &v3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			if (fDist < m_fDistanceRayToPlayer)
			{
				m_fDistanceRayToPlayer = fDist;
			}
		}
	}

	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	if (m_fDistanceRayToPlayer > 99999.f)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////

	if (CKeyManager::GetInstance()->Key_Down(KEY_SPACE))
	{
		//m_bUpDown = !m_bUpDown;
	}

	//if (CKeyManager::GetInstance()->Key_Pressing(KEY_W))
	{
		m_fRunAngle += m_fSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();

		if (!m_bUpDown)
		{
			fRotateAngle = D3DXToDegree(acos(D3DXVec3Dot(D3DXVec3Normalize(&m_vOutTemp, &vRayToPipeMid), &m_vAxis[(_uint)E_ROTATION::ROT_Y])));

			if (-360.f <= fRotateAngle && 360.f > fRotateAngle)
			{
				_float	fAngle = D3DXToDegree(acos(D3DXVec3Dot(D3DXVec3Normalize(&m_vOutTemp, &vRayToPipeMid), &m_vAxis[(_uint)E_ROTATION::ROT_Z])));

				if (-90.f < fAngle &&
					90.f > fAngle)
				{
					Add_Rotation_Quat(E_ROTATION::ROT_X, fRotateAngle);
				}
				else
				{
					Add_Rotation_Quat(E_ROTATION::ROT_X, -fRotateAngle);
				}
			}
		}
		else
		{
			fRotateAngle = D3DXToDegree(acos(D3DXVec3Dot(&(-*D3DXVec3Normalize(&m_vOutTemp, &vRayToPipeMid)), &m_vAxis[(_uint)E_ROTATION::ROT_Y])));

			if (-360.f <= fRotateAngle && 360.f > fRotateAngle)
			{
				_float	fAngle = D3DXToDegree(acos(D3DXVec3Dot(&(-*D3DXVec3Normalize(&m_vOutTemp, &vRayToPipeMid)), &m_vAxis[(_uint)E_ROTATION::ROT_Z])));

				if (-90.f < fAngle &&
					90.f > fAngle)
				{
					Add_Rotation_Quat(E_ROTATION::ROT_X, fRotateAngle);
				}
				else
				{
					Add_Rotation_Quat(E_ROTATION::ROT_X, -fRotateAngle);
				}
			}
		}
	}

	if (!m_bUpDown)
	{
		fRotateAngle = D3DXToDegree(acos(D3DXVec3Dot(&(-*D3DXVec3Normalize(&m_vOutTemp, &m_vTargetDir)), &m_vAxis[(_uint)E_ROTATION::ROT_Y])));

		if (0 > D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Y], D3DXVec3Cross(&-m_vTargetDir, &(-*D3DXVec3Normalize(&m_vOutTemp, &m_vTargetDir)), &m_vAxis[(_uint)E_ROTATION::ROT_Z])))
		{
			fRotateAngle = -fRotateAngle;
		}

		if (-360.f <= fRotateAngle && 360.f > fRotateAngle)
		{
			Add_Rotation_Quat(E_ROTATION::ROT_Z, fRotateAngle);
		}
	}
	else
	{
		fRotateAngle = D3DXToDegree(acos(D3DXVec3Dot(&(-*D3DXVec3Normalize(&m_vOutTemp, &m_vTargetDir)), &m_vAxis[(_uint)E_ROTATION::ROT_Y])));

		if (0 > D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Y], D3DXVec3Cross(&m_vTargetDir, &(-*D3DXVec3Normalize(&m_vOutTemp, &m_vTargetDir)), &m_vAxis[(_uint)E_ROTATION::ROT_Z])))
		{
			fRotateAngle = -fRotateAngle;
		}

		if (-360.f <= fRotateAngle && 360.f > fRotateAngle)
		{
			Add_Rotation_Quat(E_ROTATION::ROT_Z, fRotateAngle);
		}
	}

	if (!m_bEndVictory) 
	{
		if (CKeyManager::GetInstance()->Key_Pressing(KEY_A))
		{
			if (!m_bUpDown)
			{
				m_fPipeAngle = m_fPipeAngle - m_fRotatePipeSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
			}
			else
			{
				m_fPipeAngle = m_fPipeAngle + m_fRotatePipeSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
			}
		}
		else if (CKeyManager::GetInstance()->Key_Pressing(KEY_D))
		{
			if (!m_bUpDown)
			{
				m_fPipeAngle = m_fPipeAngle + m_fRotatePipeSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
			}
			else
			{
				m_fPipeAngle = m_fPipeAngle - m_fRotatePipeSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
			}
		}
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift))
	{
		//m_fSpeed = m_fOriginSpeed * m_fSpeedBoost;
	}
	else
	{
		m_fSpeed = m_fOriginSpeed;
	}

	vRayStartPos += vRayToPipeMid * m_fDistanceRayToPlayer;

	Set_Pos(vRayStartPos);
}

void CRun_Player::Calculate_Dis(CGameObject* pTargetObject, _vec3 vRayPos, _vec3* vDir, _float* fDistance)
{
	CRun_Map*	pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));

	if (!pRunMap)
	{
		return;
	}

	BOOL		bHit = 0;
	_matrix		matWorld = static_cast<CTransform*>(pRunMap->Get_Component(ComTag_Transform))->Get_WorldMatrix();

	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(vDir, vDir, &matWorld);

	D3DXIntersect(pRunMap->Get_Mesh(), &vRayPos, vDir, &bHit, nullptr, nullptr, nullptr, fDistance, nullptr, nullptr);
}

void CRun_Player::Update_FSM(const _float& fTimeDelta)
{
	0 < m_tUnitInfo.iNowHp ? m_eAction = E_ACTION::Move_Ing : m_eAction = E_ACTION::Vital_Death;

	if (0 >= m_tUnitInfo.iNowHp)
	{
		m_fEndGameDelay -= fTimeDelta;

		if (0.f < m_fEndGameDelay)
		{
			CTimeScale_Manager::GetInstance()->Set_TimeScale(0.1f);
		}
		else
		{
			CTimeScale_Manager::GetInstance()->Reset_TimeScale();
			CUI_Manager::GetInstance()->Set_ClearAll_RunCombatUI();

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))->Set_ClearBossHpBar();
			}

			CUI_Manager::GetInstance()->Active_RunEndGameUI();
		}
	}
}

void CRun_Player::Update_SkillGage(const _float & fTimeDelta)
{
	m_fSkillGage += fTimeDelta * Hundredth;

	m_fSkillGage = CUtility::Clamp_float(m_fSkillGage, 0.f, 1.f);
	CUI_Manager::GetInstance()->Set_RunSkillBar(m_fSkillGage);
}

void CRun_Player::Update_ExSkill(const _float & fTimeDelta)
{
	if (m_pParticleSkillEx1)
	{
		if (m_pParticleSkillEx1->Get_Dead())
		{
			m_pParticleSkillEx1 = nullptr;

			m_fEx1Delay = m_fEx1OriginDelay;
		}

		m_fEx1AttackDelay -= fTimeDelta;

		if (0.f >= m_fEx1AttackDelay &&
			0.f < m_fEx1Delay)
		{
			m_fEx1AttackDelay = CWall512::GetInstance()->GetFloatValue(0.2f, 0.5f);

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster))
			{
				vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster);

				for (_uint i = 0; i < vecObject->size(); ++i) 
				{
					if (20.f >= D3DXVec3Dot(&m_vAxis[(_uint)E_ROTATION::ROT_Z], D3DXVec3Normalize(&m_vOutTemp, &((*vecObject)[i]->Get_Position() - m_vWeaponPos))))
					{
						static_cast<CRun_Unit*>((*vecObject)[i])->Set_Damage(-1500.f); 
					}
				}
			}
			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))->Set_Damage(-8000.f);
			}

			CCamera::m_pMainCam->Set_ShakeCamera(1.f, 0.35f);
		}
	}
	if (m_bEx2Fire)
	{
		m_fEx2FireDelay -= fTimeDelta;

		if (sizeof(m_pSkillEx_2) / sizeof(CSkillObject*) > m_iEx2FireCount &&
			0.f >= m_fEx2FireDelay)
		{
			m_fEx2FireDelay = m_fEx2FireOriginDelay;

			m_pSkillEx_2[m_iEx2FireCount] = CSkill_Manager::GetInstance()->SetSkill(L"Run_Midori_Skill_2.skill",
																					E_LAYERTAG::Effect,
																					m_vWeaponPos,
																					Get_Angle(),
																					Get_Scale());
			assert(m_pSkillEx_2[m_iEx2FireCount]);
			++m_iEx2FireCount; 

			if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))
			{
				CRun_Chesed*	pBoss = static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster));
				pBoss->Set_Damage(-7500.f);
				CSkill_Manager::GetInstance()->SetSkill(L"Explosion_Normal.skill", E_LAYERTAG::Effect, pBoss->Get_Position(), Get_Angle(), Get_Scale());
			}
			
			CUI_Manager::GetInstance()->Add_RunComboHit(1);
			CCamera::m_pMainCam->Set_ShakeCamera(0.5f, 0.25f);
			m_fSkillGage += 0.005f;

			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Laser_Short_02.wav", Half);
		}
		else if (sizeof(m_pSkillEx_2) / sizeof(CSkillObject*) <= m_iEx2FireCount)
		{
			m_bEx2Fire = FALSE;

			m_fEx2FireDelay = 0.f;
			m_iEx2FireCount = 0;
		}
	}
}

void CRun_Player::Update_Collider(const _float & fTimeDelta)
{
	_vec3		vPlayerBodyPos = Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Y] * m_fScale * 1.5f;
	_matrix		matPlayerWorld = Get_Transform()->Get_WorldMatrix();
	memcpy(&matPlayerWorld._41, &vPlayerBodyPos, sizeof(_vec3));

	m_bGodMode ? matPlayerWorld._41 = (_float)INT_MAX : m_bGodMode;
	m_pColliderCom->SetMatrix(matPlayerWorld);
}

void CRun_Player::Update_Particle()
{
	//vector<CParticleObject*>::iterator		iter = m_vecParticle_FireMuzzle.begin();

	//for (; iter != m_vecParticle_FireMuzzle.end(); )
	//{
	//	if ((*iter)->Get_Dead())
	//	{
	//		(*iter)->Set_Dead(TRUE);
	//		iter = m_vecParticle_FireMuzzle.erase(iter);
	//	}
	//	else
	//	{
	//		(*iter)->Set_Pos(Get_Position() + m_vAxis[(_uint)E_ROTATION::ROT_Y] * 7.5f + m_vAxis[(_uint)E_ROTATION::ROT_Z] * 10.f);
	//		++iter;
	//	}
	//}

	if (m_pParticleRunSpark)
	{
		m_pParticleRunSpark->Set_Pos(Get_Position());
		m_pParticleRunSpark->Set_Direction(m_vAxis[(_uint)E_ROTATION::ROT_Z]);
	}
}

void CRun_Player::Update_CheckSpeedChange(const _float & fTimeDelta)
{
	m_fSpeedDownTime -= fTimeDelta;

	if (0.f > m_fSpeedDownTime)
	{
		m_fSpeed = m_fOriginSpeed;
		m_fRotatePipeSpeed = m_fRotatePipeOriginSpeed;
	}
}

void CRun_Player::Update_GodModeTime(const _float & fTimeDelta)
{
	if (m_bGodMode)
	{
		m_fGodModeTime -= fTimeDelta;

		if (0.f >= m_fGodModeTime)
		{
			m_bGodMode = FALSE;
		}
	}
}

void CRun_Player::Update_RecordAxis(const _float & fTimeDelta)
{
}

void CRun_Player::Update_EndGame(const _float & fTimeDelta)
{
	if (!m_bEndVictory) 
	{
		return; 
	}

	m_fEndGameDelay -= fTimeDelta; 

	if (0.f >= m_fEndGameDelay + 1.5f && 
		!m_bEndUI)
	{
		m_bEndUI = TRUE; 

		CSound_Manager::GetInstance()->PlayBGM(L"BGM_전투_승리.ogg", 0.3f); 
		CUI_Manager::GetInstance()->Active_RunEndGameUI(TRUE); 
		static_cast<CQuatCamera*>(CCamera::m_pMainCam)->Set_EndGame_Camera(); 
	}
	else if (0.f >= m_fEndGameDelay) 
	{
		CTimeScale_Manager::GetInstance()->Reset_TimeScale(); 
	}
}

void CRun_Player::Add_Component()
{
	// Shader 
	m_pShaderCom = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base"));
	m_mapComponent.emplace(ComTag_Shader, m_pShaderCom);

	// Collider 
	m_pColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	m_mapComponent.emplace(ComTag_Collider, m_pColliderCom);
	m_pColliderCom->SetRadius(0.3f);
	m_pColliderCom->SetScale(Get_Scale());

	// DMesh 
	m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Player"));

	if (!m_pDMeshCom)
	{
		if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_DMesh_Run_Player",
			CDMesh::Create(m_pGraphicDev, L"../../Reference/Resource/Character/Momoi/", L"Momoi.X"))))
		{
			assert(0);
		}

		m_pDMeshCom = static_cast<CDMesh*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_DMesh_Run_Player"));
		m_mapComponent.emplace(ComTag_DMesh, m_pDMeshCom);
	}

	Initialize_Trigger();
	Load_Animation();
}
