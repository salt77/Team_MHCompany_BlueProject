#include "Mascot.h"
#include "DMesh.h"
#include "Renderer.h"
#include "Layer.h"
#include "Obstacle_Combat.h"
#include "UI_Manager.h"
#include "UI_Object.h"
#include "CollisionManager.h"

USING(Engine)

CMascot::CMascot(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName)
	: CGameObject(pGraphicDev, eObjTag)
	, m_wstrWorldName(wstrWorldName)
{
}

CMascot::CMascot(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_SQUAD eSquad, wstring wstrUnitName)
	: CGameObject(pGraphicDev, eObjTag)
	, m_eSquad(eSquad)
	, m_wstrUnitName(wstrUnitName)
{
	
}

CMascot::CMascot(const CMascot & rhs)
	: CGameObject(rhs)
{
}

CMascot::~CMascot()
{
}

CMascot * CMascot::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName)
{
	CMascot* pInstance = new CMascot(pGraphicDev, eObjTag, wstrWorldName);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMascot * CMascot::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_SQUAD eSquad, wstring wstrUnitName)
{
	CMascot* pInstance = new CMascot(pGraphicDev, eObjTag, eSquad, wstrUnitName);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

// Mesh�� ���� �����ϰ� Release���ش�.
void CMascot::Free()
{
	// Map�� Emplace������ �ʾұ� ������ 
	// (�ε�Ǳ����� ����ִ� ��찡 �����Ƿ� GameObject�� �θ𿡼� Update�� ������ ������)
	if (m_pDMeshCom)
	{
		m_pDMeshCom->Clear_Bone();
		Safe_Release(m_pDMeshCom);
	}
	Safe_Release(m_pRankUI);
	Safe_Release(m_pPlayTypeUI);
	Safe_Release(m_pWorldCreater);
	CGameObject::Free();
}

void CMascot::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CMascot::Load(HANDLE hFile, DWORD & dwByte)
{
}

// Tool_Animation���� ������ �ִϸ��̼� Ʈ����, �׼��� Load�Ѵ�.
// Create���� �޾ƿ� m_wstrUnitName�� �̿��� ������ ã�´�.
void CMascot::Load_Animation()
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
		CUtility::Print_Wstring_DMode(to_wstring(iQueueSize) + L"�� �ε� ����  [" + pAction->wstrActionKey + L"]");
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
		CUtility::Print_Wstring_DMode(L"Animation Index " + to_wstring(i) + L"�� Ʈ���� " + to_wstring(iTrgCount) + L"�� �ε� ����");
	}

	CloseHandle(hFile);

}


HRESULT CMascot::Ready_Object()
{
	CGameObject::Ready_Object();

	if (m_eSquad == E_SQUAD::SQUAD_END)
	{
		m_pWorldCreater = CWorldCreator::Create(m_wstrWorldName);
		if (!m_pWorldCreater)
		{
			return E_FAIL;
		}


		E_MONSTER_TYPE eHighMonsterType = E_MONSTER_TYPE::TYPE_NORMAL;
		T_MONSTER_INFO tHighMonsterInfo;
		_bool bCheck = FALSE;

		m_vecMonsterPool = m_pWorldCreater->Get_MonsterInfo();
		for (auto& tMonsterInfo : m_vecMonsterPool)
		{
			if (tMonsterInfo.eMonsterType == E_MONSTER_TYPE::TYPE_END)
			{
				continue;
			}

			if (eHighMonsterType <= tMonsterInfo.eMonsterType)
			{
				eHighMonsterType = tMonsterInfo.eMonsterType;
				tHighMonsterInfo = tMonsterInfo;
				bCheck = TRUE;
			}
		}

		if (!bCheck)
		{
			return E_FAIL;
		}

		m_wstrUnitName = tHighMonsterInfo.wszName;
		if (tHighMonsterInfo.eMonsterType == E_MONSTER_TYPE::TYPE_BOSS)
		{
			m_bBoss = TRUE;
		}
	}
	else
	{
		m_wstrUnitName;
	}
	

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	Ready_DMesh();

	m_pSphereCollider = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(ComTag_Collider, static_cast<CComponent*>(m_pSphereCollider));
	m_pSphereCollider->SetRadius(0.3f);
	Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
	m_pSphereCollider->SetScale(Get_Scale());

	UIReady();
	return S_OK;
}

// �θ�� ����
_int CMascot::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);
	return iExit;
}

// m_pDMeshCom�� ������ Update_AnimTrigger�� �����ϰ� ������ Find_DMesh�� �����Ѵ�.
// m_qTrg Ʈ���� ť�� ��ȸ�ϸ鼭 ���ѽð��� ������ Ʈ���Ÿ� �����Ѵ�.
_int CMascot::Update_Object(const _float & fTimeDelta)
{
	E_RENDER_ID::RENDER_PRIORITY;
	if (m_pDMeshCom)
	{
		_bool bRender = TRUE;
		
		if (!m_bMascotDead)
		{
			Update_FSM(fTimeDelta);
			Animation_Change(fTimeDelta);
			m_pDMeshCom->Play_Animation(fTimeDelta);
		}
		else
		{
				m_fDeadShaderFilter += fTimeDelta * 1.f;
				if (m_fDeadShaderFilter > 3.f)
				{
					m_bDead = TRUE;
					Set_Active(FALSE);
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

	_int iExit = CGameObject::Update_Object(fTimeDelta);
	UIUpdate(fTimeDelta);


	return iExit;
}

// �θ�� ����
_int CMascot::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (m_pSphereCollider)
	{
		m_pSphereCollider->ClearColList();
	}
	UILateUpdate(fTimeDelta);
	return iExit;
}

// ������ RENDER_NONALPHA
void CMascot::Render_Object(_int iPass)
{
	if (nullptr == m_pDMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	m_pDMeshCom->Render_Meshes(m_pShaderCom);

	
	m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

	m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CMascot::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"���� �Դϴ�.");
}

void CMascot::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

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

// Abstract ��ɾ���
void CMascot::OnEnable()
{
}

// Abstract ��ɾ���
void CMascot::OnDisable()
{
}

// Abstract ��ɾ���
void CMascot::OnDestroy()
{
}

void CMascot::Animation_Change(const _float & fTimeDelta)
{
	if (m_ePrevAction != m_eCurrentAction)
	{
		Set_ActionByEnum(m_eCurrentAction);
		m_ePrevAction = m_eCurrentAction;
	}
}




void CMascot::Set_ActionByEnum(E_ACTION eAction)
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



void CMascot::Reset()
{
	m_ePrevAction = E_ACTION::ACTION_END;
	m_eCurrentAction = E_ACTION::Normal_Idle;
	
	m_bDead = FALSE;
	
	Set_Scale(_vec3(2.5f, 2.5f, 2.5f));
}








void CMascot::Set_MovePos(_vec3 vTargetPos)
{
	m_vMovePos = vTargetPos;
	m_bMove = TRUE;
}

void CMascot::SetUIActive(_bool bActive)
{
	if (m_pPlayTypeUI)
	{
		m_pPlayTypeUI->Set_Active(bActive);
	}
	if (m_pRankUI)
	{
		m_pRankUI->Set_Active(bActive);
	}

}

// Trigger �Լ��������� �ʱⰪ�� �������ش�.
void CMascot::Initialize_Trigger()
{
	//  ���� �� Utility ���� '��' Get_TriggerName ���� ����
	//  ���� �� Utility ���� '��' Get_TriggerName ���� ����
	m_vecFunction_Trg.clear();
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_1, bind(&CMascot::Trigger_Attack_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_2, bind(&CMascot::Trigger_Attack_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_3, bind(&CMascot::Trigger_Attack_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Dead, bind(&CMascot::Trigger_Dead, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Reload, bind(&CMascot::Trigger_Reload, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::ExSkill, bind(&CMascot::Trigger_ExSkill, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_1, bind(&CMascot::Trigger_Buff_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_2, bind(&CMascot::Trigger_Buff_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_1, bind(&CMascot::Trigger_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_2, bind(&CMascot::Trigger_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_3, bind(&CMascot::Trigger_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_4, bind(&CMascot::Trigger_4, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_5, bind(&CMascot::Trigger_5, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_6, bind(&CMascot::Trigger_6, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_7, bind(&CMascot::Trigger_7, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_8, bind(&CMascot::Trigger_8, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_9, bind(&CMascot::Trigger_9, this)));

	ReSetting_AnimActionVec();
	ReSetting_ActionTrigger();
}

// �׼� �ý����� �ʱⰪ�� �����Ѵ�.
void CMascot::ReSetting_AnimActionVec()
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
void CMascot::ReSetting_ActionTrigger()
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
void CMascot::Active_Trigger(T_AnimTrg & trg)
{
	// E_Trigger ������ ����
	if (m_vecFunction_Trg[trg.iTrgIndex].first == trg.eTrigger)
	{
		m_vecFunction_Trg[trg.iTrgIndex].second();
	}
}

// Ready_Object���� m_wstrUnitName�� ���� Mesh�� Load�Ѵ�.
void CMascot::Ready_DMesh()
{
	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrUnitName);

	if (nullptr == pMesh)
	{
		CUtility::Print_Wstring_DMode(L"������Ÿ���� ��� ���ο� �޽� ������Ÿ�� ����");

		wstring wstrFullPath;
		if (m_eObjTag == E_OBJTAG::OBJ_Monster)
		{
			wstrFullPath = L"../../Reference/Resource/Monster/" + m_wstrUnitName + L"/";
		}
		else
		{
			wstrFullPath = L"../../Reference/Resource/Character/" + m_wstrUnitName + L"/";
		}
		wstring wstrFileName = m_wstrUnitName + L".X";

		if (E_FAIL == CProtoMgr::GetInstance()->Ready_Prototype(m_wstrUnitName,
			CDMesh::Create(m_pGraphicDev, wstrFullPath.c_str(), wstrFileName.c_str())))
		{
				// �޽� ���� Ready_Prototype ����
				assert(FALSE);
		}
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
_bool CMascot::Find_DMesh()
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



_bool CMascot::Is_AnimFinished()
{
	return m_pDMeshCom->Is_AnimationsetFinish();
}

void CMascot::Update_Trigger(const _float& fTimeDelta)
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

void CMascot::Update_Collider(const _float & fTimeDelta)
{
	if (m_pSphereCollider)
	{
		CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereCollider, E_LAYERTAG::Obstacle);
	}

	const D3DXFRAME_DERIVED* pFrame = m_pDMeshCom->Get_FrameByName(m_szBodyBoneName);
	if (nullptr == pFrame)
	{
		m_pSphereCollider->SetMatrix(Get_Transform()->Get_WorldMatrix());
		return;
	}
	m_pSphereCollider->SetMatrix(pFrame->CombinedTransformMatrix * m_pTransformCom->Get_WorldMatrix());
	if (nullptr == pFrame)
	{
		CUtility::Print_Wstring_DMode(L"Bip001_Pelvis ���� �����ϴ�. (���� ��ũ������? ������ �����)");
		return;
	}


}

void CMascot::Update_FSM(_float fTimeDelta)
{
	if (m_eCurrentAction != E_ACTION::Formation_Pickup)
	{
		m_eCurrentAction = E_ACTION::Normal_Idle;
		Move(fTimeDelta);
	}
}

void CMascot::Move(_float fTimeDelta)
{
	if (m_bMove)
	{
		if (D3DXVec3Length(&(m_vMovePos - Get_Position())) < 0.05f)
		{
			Set_Pos(m_vMovePos);
			m_bMove = FALSE;
		}
		else
		{
			m_eCurrentAction = E_ACTION::Move_Ing;
			_vec3 vDir;
			D3DXVec3Normalize(&vDir, &(m_vMovePos - Get_Position()));
			Set_Direction(vDir);
			Move_Pos(vDir,m_fMoveSpeed,fTimeDelta);
		}
	}
}

// iIndex �� ° �ִϸ��̼��� �����ϸ鼭 �ش� �ִϸ��̼��� ��� Ʈ���Ÿ� ť�� �������ش�.
void CMascot::Set_AnimIndexForTrigger(_uint iIndex)
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
// 2��° ���� �ִϸ��̼��� m_qNextAnimIdx�� �־��ش�.
void CMascot::Set_ActionByName(wstring wstrName)
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

void CMascot::UIReady()
{
	Safe_Release(m_pRankUI);
	Safe_Release(m_pPlayTypeUI);

	if (m_eObjTag != E_OBJTAG::OBJ_Monster)
	{
		return;
	}

	if (m_ePlayType == E_ENEMY_PLAY_TYPE::TYPE_AGGRESSIVE || m_ePlayType == E_ENEMY_PLAY_TYPE::TYPE_TRACKING)
	{
		m_pPlayTypeUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Play_Type_Icon.uidat");
		if (!m_pPlayTypeUI)
		{
			return;
		}
		switch (m_ePlayType)
		{
		case Engine::E_ENEMY_PLAY_TYPE::TYPE_AGGRESSIVE:
			m_pPlayTypeUI->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Aggressive_Icon.png");
			break;
		case Engine::E_ENEMY_PLAY_TYPE::TYPE_TRACKING:
			m_pPlayTypeUI->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/Tracking_Icon.png");
			break;
		default:
			break;
		}
	}
	if (!m_bBoss)
	{
		if (!m_pWorldCreater)
		{
			return;
		}
		_int iSize = m_pWorldCreater->Get_GroupSize();
		m_pRankUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]����ũUI.uidat");

		switch (iSize)
		{
		case 1:
		case 2:
			iSize = 1;
			break;
		case 3:
			iSize = 2;
			break;
		default:
			iSize = 3;
		}
		for (_int i = 0; i < iSize; ++i)
		{
			m_pRankUI->Get_ChildByIndex(i)->Set_Active(TRUE);
		}
		
	}
	else
	{
		m_pRankUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Boss_Text_Icon.uidat");
	}


}

void CMascot::UIUpdate(_float fTimeDelta)
{
	_vec2 vUIPos = CUtility::World2Ui(Get_Position());

	if (m_pPlayTypeUI)
	{
		if (m_bBoss)
		{
			m_pPlayTypeUI->Set_Ui_PosX((_int)vUIPos.x - 60);
		}
		else
		{
			m_pPlayTypeUI->Set_Ui_PosX((_int)vUIPos.x - 50);
		}
		m_pPlayTypeUI->Set_Ui_PosY((_int)vUIPos.y-3);
		m_pPlayTypeUI->Update_Object(fTimeDelta);
	}
	if (m_pRankUI)
	{
		m_pRankUI->Set_Ui_PosX((_int)vUIPos.x);
		m_pRankUI->Set_Ui_PosY((_int)vUIPos.y-3);
		m_pRankUI->Update_Object(fTimeDelta);
	}
}

void CMascot::UILateUpdate(_float fTimeDelta)
{
	if (m_pPlayTypeUI)
	{
		m_pPlayTypeUI->LateUpdate_Object(fTimeDelta);
	}
	if (m_pRankUI)
	{
		m_pRankUI->LateUpdate_Object(fTimeDelta);
	}
}




