#include "Invite_Character.h"
#include "DMesh.h"
#include "Renderer.h"
#include "Layer.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "ProtoMgr.h"
#include "LightMgr.h"
#include "FogManager.h"
#include "KeyManager.h"
#include "CharacterMgr.h"
#include "Texture.h"
#include "Obstacle.h"
#include "Scene.h"
#include "Wall512.h"
#include "Obstacle_Cafe.h"
#include "Obstacle_Combat.h"
#include "CollisionManager.h"

USING(Engine)

CInvite_Character::CInvite_Character(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CGameObject(pGraphicDev, eObjTag)
	, m_wstrUnitName(wstrUnitName)
{
}

CInvite_Character::CInvite_Character(const CInvite_Character & rhs)
	: CGameObject(rhs)
{
}

CInvite_Character::~CInvite_Character()
{
}

// Mesh�� ���� �����ϰ� Release���ش�.
void CInvite_Character::Free()
{
	m_listAstarNode.clear();

	// Map�� Emplace������ �ʾұ� ������ 
	// (�ε�Ǳ����� ����ִ� ��찡 �����Ƿ� GameObject�� �θ𿡼� Update�� ������ ������)
	if (m_pDMeshCom)
	{
		m_pDMeshCom->Clear_Bone();
		Safe_Release(m_pDMeshCom);
	}

	CGameObject::Free();
}

void CInvite_Character::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CInvite_Character::Load(HANDLE hFile, DWORD & dwByte)
{
}

// Tool_Animation���� ������ �ִϸ��̼� Ʈ����, �׼��� Load�Ѵ�.
// Create���� �޾ƿ� m_wstrUnitName�� �̿��� ������ ã�´�.
void CInvite_Character::Load_Animation()
{
	wstring TriggerPath = L"../../Reference/Data/Data_Unit/ActionTrigger/";
	TriggerPath.append(m_wstrUnitName + L".ChrAnimdat");
	HANDLE hFile = CreateFile(TriggerPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// �ε��� ĳ���� ActionTrigger ������ ����.
		assert(FALSE);
		return;
	}

	CUtility::Data_ReadWString(hFile, m_wstrDMeshProtoTag, dwByte);
	CUtility::Data_ReadWString(hFile, m_wstrDMeshFullPath, dwByte);
	CUtility::Print_Wstring_DMode(L"Load ���� - ProtoTag [  " + m_wstrDMeshProtoTag + L"  ]");
	CUtility::Print_Wstring_DMode(L"Load ���� - FullPath [  " + m_wstrDMeshFullPath + L"  ]");

	// �׼� �ε�
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

	// �ִϸ��̼� �� Ʈ���� �ε�
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

	// ������ �ε� �� Sit, Stand Shoot ������ ����ٸ� ����, ū ��ֹ��� ���� �Ұ���
	if (m_vecAnimActions[(_int)E_ACTION::Sit_Shoot].qAnimation.empty())
		m_bCanHideSmall = FALSE;
	else
		m_bCanHideSmall = TRUE;

	if (m_vecAnimActions[(_int)E_ACTION::Stand_Shoot].qAnimation.empty())
		m_bCanHideBig = FALSE;
	else
		m_bCanHideBig = TRUE;

	// ���� �׼��� ����ٸ� ��ֹ� �����ؼ� �������� �Ұ���
	if (m_vecAnimActions[(_int)E_ACTION::Jump].qAnimation.empty())
		m_bJumpObstacle = FALSE;
	else
		m_bJumpObstacle = TRUE;
}

CInvite_Character * CInvite_Character::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName)
{
	CInvite_Character* pInstance = new CInvite_Character(pGraphicDev, eObjTag, wstrWorldName);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

// Abstract �����̱� ������ Add_Component�� ���ξ���.
HRESULT CInvite_Character::Ready_Object()
{
	CGameObject::Ready_Object();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	Ready_DMesh();

	Create_Collider(&m_pSphereCollider, ComTag_Collider, 0.15f);

	m_pTexMouse = static_cast<CTexture*>(Emplace_Component(L"Com_TextureMouse", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CharacterMouse")));

	return S_OK;
}

// �θ�� ����
_int CInvite_Character::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);

	m_eCurrentAction = E_ACTION::Cafe_Idle;

	return iExit;
}

// m_pDMeshCom�� ������ Update_AnimTrigger�� �����ϰ� ������ Find_DMesh�� �����Ѵ�.
// m_qTrg Ʈ���� ť�� ��ȸ�ϸ鼭 ���ѽð��� ������ Ʈ���Ÿ� �����Ѵ�.
_int CInvite_Character::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pDMeshCom)
	{
		_bool bRender = TRUE;

		if (m_bStateLock)
		{
			if (Is_AnimFinished())
			{
				m_bStateLock = FALSE;
			}
		}
		else
		{
			m_eCurrentAction = E_ACTION::Cafe_Idle;
		}

		Animation_Change(fTimeDelta);

		if (E_ACTION::Vital_Death != m_eCurrentAction)
		{
			m_pDMeshCom->Play_Animation(fTimeDelta);
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

	return iExit;
}

// �θ�� ����
_int CInvite_Character::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Update_Collider(fTimeDelta);

	Cafe_Move(fTimeDelta);

	m_pTransformCom->Update_Component(fTimeDelta);

	if (m_pSphereCollider)
	{
		m_pSphereCollider->ClearColList();
	}
	
	return iExit;
}

// ������ RENDER_NONALPHA
void CInvite_Character::Render_Object(_int iPass)
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
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CInvite_Character::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"Invite_Char : Cafe Character");
}

void CInvite_Character::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (m_pTexMouse)
	{
		m_pTexMouse->Set_Texture(pEffect, "g_MouseTexture");
	}

	_matrix	matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// �׸��ڿ�
	pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// ����� ����
	pEffect->SetFloat("g_fFilter", m_fDeadShaderFilter);	
}

void CInvite_Character::On_Collision(CGameObject * pDest)
{
	Get_Transform()->Update_Component(0.f);
}

void CInvite_Character::Cafe_Move(const _float& fTimeDelta)
{
	if (m_bChangePreset)
	{
		m_pFurObj = nullptr;
		m_pFurniture = nullptr;
		m_bRandomPick = true;
		m_bTimeSet = true;
		m_bChangePreset = false;
		return;
	}

	if (m_bAllStorage)
	{
		m_pFurObj = nullptr;
		m_pFurniture = nullptr;
		m_bTimeSet = true;
		Set_Action(E_ACTION::Cafe_Idle);
		m_bAllStorage = false;
		return;
	}

	if (m_eCurrentAction == E_ACTION::Formation_Pickup ||
		m_eCurrentAction == E_ACTION::Cafe_Reaction
		)
	{
		return;
	}
	
	Set_Action(E_ACTION::Cafe_Walk);

	if (m_bTimeSet)
	{
		m_fIdleTime += fTimeDelta;
		if (m_fIdleTime < 2.99f)
		{
			Set_Action(E_ACTION::Cafe_Idle);
		}
		if (m_fIdleTime >= 3.f) 
		{
			m_bRandomPick = true;
			m_bTimeSet = false;
			m_fIdleTime = 0.f;
		}
	}

	if (m_bCreateUpdate)
	{
		m_pFurObj = nullptr;
		m_pFurniture = nullptr;
		m_bRandomPick = true;
		m_bTimeSet = true;
		m_bCreateUpdate = false;
	}

	vector<CGameObject*>* vecFurniture = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle);

	_int i = (_int)vecFurniture->size();
	_int iRandomSize = 0;

	if (vecFurniture->size() >= 1)
	{
		if (m_bRandomPick)
		{
			while (true)
			{
				m_vCafeMoveDest = _vec3(0.f, 0.f, 0.f);
				m_vecCharPos = _vec3(0.f, 0.f, 0.f);
				m_vecCharDir = _vec3(0.f, 0.f, 0.f);
				iRandomSize = CWall512::GetInstance()->GetValue(0, 10000) % i;
				m_pFurObj = (*vecFurniture)[iRandomSize];

				if (m_pFurObj->Get_Dead())
				{
					continue;
				}
				else
				{
					break;
				}
			}

			m_pFurniture = static_cast<CObstacle_Cafe*>(m_pFurObj);

			m_bRandomPick = false;
		}

		if (m_pFurniture != nullptr)
		{
			if (m_pFurniture->Get_Dead())
			{
				m_pFurniture = nullptr;
				m_bTimeSet = true;
			}
			else
			{
				m_vCafeMoveDest = m_pFurniture->Get_Position();
				m_vCafeMoveDest.y = 0.f;
				m_vecCharPos = Get_Position();
				m_vecCharPos.y = 0.f;
				m_vecCharDir = m_vCafeMoveDest - m_vecCharPos;
				m_vecCharDir.y = 0.f;
				D3DXVec3Normalize(&m_vecCharDir, &m_vecCharDir);

				if (CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereCollider, E_LAYERTAG::Obstacle))
				{
					if (m_pFurniture->Get_Name().find(L"[W]") != string::npos)
					{
						if (D3DXVec3Length(&(m_vecCharPos - m_pFurniture->Get_Position())) <= 3.f)
						{
							m_bTimeSet = true;
						}
					}
					else
					{
						if (D3DXVec3Length(&(m_vecCharPos - m_pFurniture->Get_Position())) <= 0.5f)
						{
							m_bTimeSet = true;
						}
					}
				}
			}
		}

		if (!m_bTimeSet)
		{
			_vec3 vMoveAmount = m_vecCharDir * fTimeDelta * 0.4f;
			Set_Look(m_vecCharPos + m_vecCharDir);
			Add_Pos(vMoveAmount);
		}
	}
	else
	{
		Set_Action(E_ACTION::Cafe_Idle);
	}
}

_bool CInvite_Character::Find_Target(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag)
{
	CLayer* pTargetLayer = CScene::Get_MainScene()->Get_Layer(eFindLayerTag);

	vector<CGameObject*>* pVecMonster = pTargetLayer->Get_vecGameObject(eFindObjTag);

	if (nullptr == pVecMonster || pVecMonster->empty())
	{
		CUtility::Print_Wstring_DMode(L"Ÿ�� ã�� ���� - ���� : ���� ����.");
		return FALSE;
	}
	CGameObject*	pTarget = nullptr;

	_float fMinDist = 99999.f;

	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (FALSE == (*pVecMonster)[i]->Get_Active())
			continue;

		if (static_cast<CInvite_Character*>((*pVecMonster)[i])->Get_UnitDead())
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
		m_pTarget = static_cast<CInvite_Character*>(pTarget);
	}

	return TRUE;
}

_bool CInvite_Character::Find_Target_InRange(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag)
{
	CLayer* pTargetLayer = CScene::Get_MainScene()->Get_Layer(eFindLayerTag);

	vector<CGameObject*>* pVecMonster = pTargetLayer->Get_vecGameObject(eFindObjTag);

	if (nullptr == pVecMonster || pVecMonster->empty())
	{
		CUtility::Print_Wstring_DMode(L"Ÿ�� ã�� ���� - ���� : ���� ����.");
		return FALSE;
	}
	vector<CInvite_Character*> vecCanTarget;
	for (_int i = 0; i < (_int)pVecMonster->size(); ++i)
	{
		if (FALSE == (*pVecMonster)[i]->Get_Active())
			continue;

		if (static_cast<CInvite_Character*>((*pVecMonster)[i])->Get_UnitDead())
			continue;

		_float fCalcDist = CUtility::Distance_vec3(Get_Position(), (*pVecMonster)[i]->Get_Position());

		if (fCalcDist < Get_UnitRange())
		{
			vecCanTarget.push_back(static_cast<CInvite_Character*>((*pVecMonster)[i]));
		}
	}

	if (vecCanTarget.empty())
	{
		CUtility::Print_Wstring_DMode(L"�������� �� ã�� ����");
		return FALSE;
	}

	_int iRandEnemy = CWall512::GetInstance()->GetValue((_int)vecCanTarget.size());

	CUtility::Print_Wstring_DMode(L"�������� ������ �� ã�� �� " + to_wstring((_int)vecCanTarget.size()) + L"������ �� ��" + to_wstring(iRandEnemy) + L"�� ° �� ����");
	m_pTarget = vecCanTarget[iRandEnemy];

	FindObstacle();
	return TRUE;
}

void CInvite_Character::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;
	}
}

void CInvite_Character::CutHideObstacle()
{
	m_pHideObstacle = nullptr;
}

void CInvite_Character::Set_Action(E_ACTION eAction)
{
	m_eCurrentAction = eAction;
	m_bStateLock = TRUE;
}

void CInvite_Character::Set_ActionByEnum(E_ACTION eAction)
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
		LMSG_BOX(L"E_ANIM ���� �ʰ�");
		break;
	}
}

void CInvite_Character::Set_HideObstacle(CGameObject * pObj)
{
	m_pHideObstacle = pObj;
}

void CInvite_Character::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (nullptr == m_pDMeshCom)
		return;

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
			FSM_FunctionSetting(&CInvite_Character::Hiding, nullptr);
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
					FSM_FunctionSetting(&CInvite_Character::Move_End_Stance, nullptr);
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
		Look_Target_Slowly(fDeltaTime);
	}
	MoveTo_Astar(fDeltaTime, m_fMoveSpeed, 1.f);
}

void CInvite_Character::Reset()
{
	FSM_FunctionRelease();
	
	m_listAstarNode.clear();
	m_pTarget = nullptr;
	m_ePrevAction = E_ACTION::ACTION_END;
	m_eCurrentAction = E_ACTION::ACTION_END;
	m_eHide = E_HIDE::HIDE_NONE;
	m_bDead = FALSE;
	m_pHideObstacle = nullptr;
	Set_Scale(_vec3(2.5f, 2.5f, 2.5f));

	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
}

void CInvite_Character::FSM_FunctionRelease()
{
	m_bStateLock = FALSE;
	(*this).m_fpLockFunction = nullptr;
}

void CInvite_Character::FSM_FunctionSetting(void(CInvite_Character::*pCol)(_float), void(CInvite_Character::*pDestFunc)(_float))
{
	m_bStateLock = TRUE;
	(*this).m_fpLockFunction = pCol;
	(*this).m_fpDestFunction = pDestFunc;

}

void CInvite_Character::Clear_Astar()
{
	m_listAstarNode.clear();
}

void CInvite_Character::Tracking(_float fDeltaTime)
{
	if (nullptr == m_pTarget)
	{
		return;
	}
	if (m_listAstarNode.empty())
	{
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
	else if (E_ACTION::Jump == m_eCurrentAction)
	{

	}
	else
	{
		m_eCurrentAction = E_ACTION::Move_Ing;
	}
}

void CInvite_Character::Idle(_float fDeltaTime)
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

	_float fDistance = 10000.f;
	if (m_pTarget)
	{
		fDistance = CMyMath::Distance_vec3(m_pTarget->Get_Position(), Get_Position());
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

	if (m_pHideObstacle)
	{
		if (m_eHide == E_HIDE::HIDE_NONE)
		{
			Find_Astar(Get_Position(), static_cast<CObstacle_Combat*>(m_pHideObstacle)->Get_HideSpotPos(this), m_bJumpObstacle);
			FSM_FunctionSetting(&CInvite_Character::Hiding, nullptr);
		}
	}
}

void CInvite_Character::Ready_End(_float fDeltaTime)
{
	E_ACTION eMainAction = E_ACTION::Normal_Ready_End;

	switch (m_eHide)
	{
	case Engine::E_HIDE::HIDE_NONE:		eMainAction = E_ACTION::Normal_Ready_End;	break;
	case Engine::E_HIDE::HIDE_SMALL:	eMainAction = E_ACTION::Sit_Ready_End;		break;
	case Engine::E_HIDE::HIDE_BIG:		eMainAction = E_ACTION::Stand_Ready_End;	break;
	case Engine::E_HIDE::HIDE_END:		eMainAction = E_ACTION::Normal_Ready_End;	break;
	default:							eMainAction = E_ACTION::Normal_Ready_End;	break;
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

void CInvite_Character::Move_End_Stance(_float fDeltaTime)
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

	m_eCurrentAction = eMainAction;
}

void CInvite_Character::Hiding(_float fDeltaTime)
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
		FSM_FunctionSetting(&CInvite_Character::Move_End_Stance, nullptr);

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

void CInvite_Character::Assemble(_float fDeltaTime)
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

void CInvite_Character::Dissipation(_float fDeltaTime)
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

// Trigger �Լ��������� �ʱⰪ�� �������ش�.
void CInvite_Character::Initialize_Trigger()
{
	//  ���� �� Utility ���� '��' Get_TriggerName ���� ����
	//  ���� �� Utility ���� '��' Get_TriggerName ���� ����
	m_vecFunction_Trg.clear();
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_1, bind(&CInvite_Character::Trigger_Attack_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_2, bind(&CInvite_Character::Trigger_Attack_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_3, bind(&CInvite_Character::Trigger_Attack_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Dead, bind(&CInvite_Character::Trigger_Dead, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Reload, bind(&CInvite_Character::Trigger_Reload, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::ExSkill, bind(&CInvite_Character::Trigger_ExSkill, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_1, bind(&CInvite_Character::Trigger_Buff_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_2, bind(&CInvite_Character::Trigger_Buff_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_1, bind(&CInvite_Character::Trigger_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_2, bind(&CInvite_Character::Trigger_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_3, bind(&CInvite_Character::Trigger_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_4, bind(&CInvite_Character::Trigger_4, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_5, bind(&CInvite_Character::Trigger_5, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_6, bind(&CInvite_Character::Trigger_6, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_7, bind(&CInvite_Character::Trigger_7, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_8, bind(&CInvite_Character::Trigger_8, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_9, bind(&CInvite_Character::Trigger_9, this)));

	ReSetting_AnimActionVec();
	ReSetting_ActionTrigger();
}

// �׼� �ý����� �ʱⰪ�� �����Ѵ�.
void CInvite_Character::ReSetting_AnimActionVec()
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

// �׼� Ʈ������ �ʱⰪ�� �����Ѵ�.
void CInvite_Character::ReSetting_ActionTrigger()
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

// �Լ������� Ʈ���Ÿ� �ߵ���Ų��.
void CInvite_Character::Active_Trigger(T_AnimTrg & trg)
{
	// E_Trigger ������ ����
	if (m_vecFunction_Trg[trg.iTrgIndex].first == trg.eTrigger)
	{
		m_vecFunction_Trg[trg.iTrgIndex].second();
	}
}

// Ready_Object���� m_wstrUnitName�� ���� Mesh�� Load�Ѵ�.
void CInvite_Character::Ready_DMesh()
{
	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);

	if (nullptr == pMesh)
	{
		CUtility::Print_Wstring_DMode(L"������Ÿ���� ��� ���ο� �޽� ������Ÿ�� ����");

		wstring wstrFullPath = L"../../Reference/Resource/Character/" + m_wstrUnitName + L"/";
		wstring wstrFileName = m_wstrUnitName + L".X";

		if (E_FAIL == CProtoMgr::GetInstance()->Ready_Prototype(m_wstrUnitName,
			CDMesh::Create(m_pGraphicDev, wstrFullPath.c_str(), wstrFileName.c_str())))
		{
			// �޽� ���� Ready_Prototype ����
			
			assert(FALSE);
		}
		pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);
		CUtility::Print_Wstring_DMode(L"�޽� ã�� ����");
		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		Initialize_Trigger();
		Load_Animation();
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"�޽� ã�� ����");
		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		Initialize_Trigger();
		Load_Animation();
	}
}

// ���� Mesh�� ���ų� ���� �ε���� �ʾҴٸ�  �� ������ Mesh Load�� �õ��Ѵ�.
_bool CInvite_Character::Find_DMesh()
{
	if (m_pDMeshCom)
		return TRUE;

	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);
	if (nullptr == pMesh)
	{
		return FALSE;
	}

	// �޽� ã��

	m_pDMeshCom = static_cast<CDMesh*>(pMesh);
	Initialize_Trigger();
	Load_Animation();

	return TRUE;
}

void CInvite_Character::DeleteObstacle()
{
	if (m_pHideObstacle)
	{
		static_cast<CObstacle_Combat*>(m_pHideObstacle)->CutHideObject();
		m_pHideObstacle = nullptr;
	}
}

void CInvite_Character::FindObstacle()
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

_bool CInvite_Character::Is_AnimFinished()
{
	return m_pDMeshCom->Is_AnimationsetFinish();
}

void CInvite_Character::Update_Trigger(const _float& fTimeDelta)
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

void CInvite_Character::Update_Collider(const _float & fTimeDelta)
{
	_vec3 vPushPos;

	if (m_pSphereCollider)
	{
		CCollisionManager::GetInstance()->Push_Sphere(m_pSphereCollider, E_LAYERTAG::Obstacle);
	}

	Collider_SetMatrix(m_pSphereCollider, m_szBodyBoneName);
}

void CInvite_Character::Look_Target_Slowly(const _float& fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;

	if (m_fpLockFunction == &CInvite_Character::Hiding)
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

void CInvite_Character::Create_Collider(CSphereCollider ** pCol, wstring wstrCompName, _float fRadius)
{
	*pCol = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(wstrCompName, static_cast<CComponent*>(*pCol));
	(*pCol)->SetRadius(fRadius);
	(*pCol)->SetScale(Get_Scale());
}

void CInvite_Character::Collider_SetMatrix(CSphereCollider * pCol, const char * szFindBoneName)
{
	const D3DXFRAME_DERIVED* pBodyFrame = m_pDMeshCom->Get_FrameByName(m_szBodyBoneName);

	if (nullptr == pBodyFrame)
	{
		CUtility::Print_Wstring_DMode(L"���� ����� (m_szBodyBoneName)");
		return;
	}

	const D3DXFRAME_DERIVED* pFindFrame = m_pDMeshCom->Get_FrameByName(szFindBoneName);
	if (pFindFrame)
	{
		pCol->SetMatrix(pFindFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix());
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"���� �����" + CUtility::CHAR_TO_WSTRING(szFindBoneName));
		pCol->SetMatrix(pBodyFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix());
	}
}

// iIndex �� ° �ִϸ��̼��� �����ϸ鼭 �ش� �ִϸ��̼��� ��� Ʈ���Ÿ� ť�� �������ش�.
void CInvite_Character::Set_AnimIndexForTrigger(_uint iIndex)
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

// wstrName�̸��� ���� �׼��� �����ϸ鼭 �ش� �׼��� ù ��° �ִϸ��̼��� �����ϰ�
void CInvite_Character::Set_ActionByName(wstring wstrName)
// 2��° ���� �ִϸ��̼��� m_qNextAnimIdx�� �־��ش�.
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

void CInvite_Character::Find_Astar(_vec3 vStartPos, _vec3 vDest, _bool bCanJump)
{
	// ������ ������ġ�� ������ ���Ѵ�.
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

	// �浹ü
	CLayer* pObstacleLayer = CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::Obstacle);
	vector<CGameObject*>* pVecObstacle = pObstacleLayer->Get_vecGameObject(E_OBJTAG::OBJ_Obstacle);

	vector<CObstacle_Combat*> vecCloseObstacle;
	_bool bArriveDest = FALSE;
	_bool bColResult = FALSE;
	_vec3 vFinalPos = VEC_ZERO;

	
	// ��ֹ��� ���� ��쿡�� üũ
	if (nullptr != pVecObstacle)
	{
		for (_int i = 0; i < (_int)pVecObstacle->size(); ++i)
		{
			if ((*pVecObstacle)[i] == m_pHideObstacle)
				continue;

			CObstacle_Combat* pObstacle = static_cast<CObstacle_Combat*>((*pVecObstacle)[i]);
			_float fDistWithObs = CUtility::Distance_vec3(vStartPos, pObstacle->Get_ShperePos());
			_vec3 vObsDir = pObstacle->Get_ShperePos() - vRayPos;

			// ����ó�� 1 : ���������� �ָ� �ִٸ�
			if (fDistWithObs >= fDistanceToDest)
			{
				continue;
			}

			// ����ó�� 2 : ���� �������� �پ�Ѵ´ٸ�
			if (pObstacle->Get_Name().substr(1, 1) == L"S" && TRUE == bCanJump)
			{
				continue;
			}

			// ����ó�� 3 : ������ ���� RayDir�� vObsDir�� Theta (��) �� 15�� �̻��̸�
			// Ȥ�� ������ 0���� ������ (�ݴ����)
			if (15.f * Rad2Deg <= abs(D3DXVec3Dot(&vObsDir, &vRayDir)) || 0 > D3DXVec3Dot(&vObsDir, &vRayDir))
			{
				continue;
			}

			// ����ó�� 4 : 

			pObstacle->Set_DistanceForSort(fDistWithObs);
			vecCloseObstacle.push_back(pObstacle);
		}

		// �浹 ������ ��ֹ��� �Ÿ���ʷ� ����
		sort(vecCloseObstacle.begin(), vecCloseObstacle.end(), CObstacle_Combat::CompareByDistance);
		_int iCloseObsSize = (_int)vecCloseObstacle.size();

		// �浹 ������ ��ֹ��� �Ÿ��� ����� �� ���� �浹���� ���� ����
		for (_int i = 0; i < iCloseObsSize; ++i)
		{
			_vec3 vSpherePos = vecCloseObstacle[i]->Get_ShperePos();		// �浹ü ����
			_float vSphereRadius = vecCloseObstacle[i]->Get_Radius();		// �浹ü ������
			_vec3 vRayToObs = vSpherePos - vRayPos;							// �������� ���� �浹ü���� ����
			_float fDot_Rto_Rdir = D3DXVec3Dot(&vRayToObs, &vRayDir);		// To��ǥ, To�浹ü�� ����
			_float fScalar_Rto = D3DXVec3Dot(&vRayToObs, &vRayToObs);		// To�浹ü�� �Ÿ�
			_float r2 = pow(vSphereRadius, 2);					// �浹ü ������ ����

			// ������ ���� �ݴ� ������ ���ϰų� ���� ����ģ ���
			if (0 > fDot_Rto_Rdir && fScalar_Rto > r2)
			{
				bColResult = FALSE;
				continue;
			}
			_float m2 = fScalar_Rto - pow(fDot_Rto_Rdir, 2);

			// ������ ���� �񲸰��� ���
			if (m2 > r2)
			{
				bColResult = FALSE;
				continue;
			}

			bColResult = TRUE;
			_float q = sqrt(r2 - m2);
			_float distance = 0.f;

			// ��ֹ��� ���� ���
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
				for (_int j = 0; j < 30; ++j)
				{
					fAngle = atan2(vTemp.z, vTemp.x) + fDir * 135.f * j / 30 * Deg2Rad;
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
			// �̹� Loop�� ��ֹ��� ������ �ʾ��� ��� ��ֹ��� ������ ��ŭ ����.
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

	// ��ֹ��� �浹���� �ʾҴٸ� �������� �̵�
	if (FALSE == bColResult)
	{
		m_listAstarNode.push_back(T_AstarNode(vDest));
	}
	// �������� �������� ���ߴٸ� Astar�� ���� ������ ��ġ���� ��ǥ�������� �ٽ� �ѹ� Find_Astar�� ���������
	else if (FALSE == bArriveDest)
	{
		Find_Astar(vFinalPos, vDest, bCanJump);
	}
}


void CInvite_Character::MoveTo_Astar(_float fTimeDelta, _float fSpeed, _float fSRatio)
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

	// �̹� ������ �̵� Vector
	_vec3 vMoveAmount = vDir * fTimeDelta * fSpeed * fSRatio;
	Set_Look(vPos + vDir);


	// #1 : �� ������ �̵����� ������������ �Ÿ����� ũ�� Set
	// #2 : ������������ �Ÿ��� 0.1f ���� ũ�� �̵�
	// #3 : ������������ �Ÿ��� 0.1f ���� ������ ��� ���� �� ���� ����

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
