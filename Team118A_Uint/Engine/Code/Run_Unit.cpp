#include "Run_Unit.h"

#include "DMesh.h"
#include "SphereCollider.h"
#include "UI_Damage.h" 

#include "LightMgr.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "UI_Manager.h" 

USING(Engine)


CRun_Unit::CRun_Unit(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrUnitName, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
	, m_wstrUnitName(wstrUnitName)
{
	ZeroMemory(&m_tUnitInfo, sizeof(T_UnitInfo));

	m_vecAnimActions.reserve(53);

	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXQuaternionIdentity(&m_quatRot); 
}

CRun_Unit::CRun_Unit(const CRun_Unit & rhs)
	: CGameObject(rhs)
{
}

CRun_Unit::~CRun_Unit()
{
}

void CRun_Unit::Free()
{
	if (m_pDMeshCom)
	{
		m_pDMeshCom->Clear_Bone();
	}

	CGameObject::Free();
}

void CRun_Unit::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte);
}

void CRun_Unit::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}

HRESULT CRun_Unit::Ready_Object()
{
	CGameObject::Ready_Object();

	ReSetting_Animation();
	ReSetting_ActionTrigger(); 
	Initialize_QuatRotation(); 

	return S_OK;
}

_int CRun_Unit::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Start_Object(fTimeDelta);

	return iExit;
}

_int CRun_Unit::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Update_Object(fTimeDelta); 

	return iExit;
}

_int CRun_Unit::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (1.f > m_fDeadShaderFilter)
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
	}

	return iExit;
}

void CRun_Unit::Render_Object(_int iPass)
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
		m_pColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CRun_Unit::Debug_This_In_Console()
{
}

void CRun_Unit::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix	matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// 그림자용
	pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망시 필터
	pEffect->SetFloat("g_fFilter", m_fDeadShaderFilter);
}

void CRun_Unit::Set_Constantable_Fog(CShader * pShader)
{
}

void CRun_Unit::Set_Damage(const _float& fDamage)
{
	Create_DamageFont(E_NumberFontType::Normal, -(_int)fDamage, TRUE); 

	m_tUnitInfo.iNowHp += (_int)fDamage; 
} 

void CRun_Unit::OnEnable()
{
}

void CRun_Unit::OnDisable()
{
}

void CRun_Unit::OnDestroy()
{
}

void CRun_Unit::On_Collision(CGameObject * pDest)
{
}

const _quat& CRun_Unit::Get_QuatRotation()
{
	return m_quatRot;
}

void CRun_Unit::Initialize_QuatRotation()
{
	m_vAxis[(_uint)E_ROTATION::ROT_X] = Get_Info(E_INFO::INFO_RIGHT);
	m_vAxis[(_uint)E_ROTATION::ROT_Y] = Get_Info(E_INFO::INFO_UP);
	m_vAxis[(_uint)E_ROTATION::ROT_Z] = Get_Info(E_INFO::INFO_LOOK);

	D3DXQuaternionRotationMatrix(&m_quatRot, &m_pTransformCom->Get_WorldMatrix());
}

void CRun_Unit::Initialize_Trigger()
{
	m_vecFunction_Trg.clear();
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_1, bind(&CRun_Unit::Trigger_Attack_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_2, bind(&CRun_Unit::Trigger_Attack_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Attack_3, bind(&CRun_Unit::Trigger_Attack_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Dead, bind(&CRun_Unit::Trigger_Dead, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Reload, bind(&CRun_Unit::Trigger_Reload, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::ExSkill, bind(&CRun_Unit::Trigger_ExSkill, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_1, bind(&CRun_Unit::Trigger_Buff_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Buff_2, bind(&CRun_Unit::Trigger_Buff_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_1, bind(&CRun_Unit::Trigger_1, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_2, bind(&CRun_Unit::Trigger_2, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_3, bind(&CRun_Unit::Trigger_3, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_4, bind(&CRun_Unit::Trigger_4, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_5, bind(&CRun_Unit::Trigger_5, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_6, bind(&CRun_Unit::Trigger_6, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_7, bind(&CRun_Unit::Trigger_7, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_8, bind(&CRun_Unit::Trigger_8, this)));
	m_vecFunction_Trg.push_back(make_pair(E_Trigger::Trg_9, bind(&CRun_Unit::Trigger_9, this)));

	ReSetting_Animation();
	ReSetting_ActionTrigger();
}

void CRun_Unit::Update_WorldMatrix()
{
	_matrix		matScale, matTrans;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixScaling(&matScale, Get_Scale().x, Get_Scale().y, Get_Scale().z);
	D3DXMatrixTranslation(&matTrans, Get_Position().x, Get_Position().y, Get_Position().z);

	m_matWorld = matScale * m_matRot * matTrans; 
}

void CRun_Unit::ReSetting_Animation()
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

void CRun_Unit::ReSetting_ActionTrigger()
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

void CRun_Unit::Active_Trigger(T_AnimTrg & trg)
{
	if (m_vecFunction_Trg[trg.iTrgIndex].first == trg.eTrigger)
	{
		m_vecFunction_Trg[trg.iTrgIndex].second();
	}
}

void CRun_Unit::Load_Animation()
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
		CUtility::Print_Wstring_DMode(to_wstring(iQueueSize) + L"개 로드 성공  [" + pAction->wstrActionKey + L"]");
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
		CUtility::Print_Wstring_DMode(L"Animation Index " + to_wstring(i) + L"에 트리거 " + to_wstring(iTrgCount) + L"개 로드 성공");
	}

	CloseHandle(hFile);
}

void CRun_Unit::Update_Trigger(const _float & fTimeDelta)
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

void CRun_Unit::Set_ActionByName(const wstring & wstrAction)
{
	for (auto& iter : m_vecAnimActions)
	{
		if (iter.wstrActionKey == wstrAction)
		{
			_int iSize = (_int)iter.qAnimation.size();
			queue<_int> qCopy = iter.qAnimation;

			for (_int i = 0; i < iSize; ++i)
			{
				_int iFrontAnimIndex = qCopy.front();
				qCopy.pop();

				m_pDMeshCom->Set_AnimationIndex_Loop(iFrontAnimIndex);

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

void CRun_Unit::Set_ActionByEnum(E_ACTION eAction)
{
	m_eAction = eAction;
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

void CRun_Unit::Set_AnimIndexForTrigger(_uint iIndex)
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

void CRun_Unit::Animation_Change()
{
	if (m_ePreAction != m_eAction)
	{
		Set_ActionByEnum(m_eAction);

		m_ePreAction = m_eAction;
	}
}

void CRun_Unit::Update_Collider(const _float & fTimeDelta)
{
}

void CRun_Unit::Collider_SetMatrix(CSphereCollider * pCol, const char * szFindBoneName)
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

void CRun_Unit::Create_Collider(CSphereCollider ** pCol, wstring wstrCompName, _float fRadius)
{
	*pCol = CSphereCollider::Create(m_pGraphicDev, this);
	Emplace_Component(wstrCompName, static_cast<CComponent*>(*pCol));
	(*pCol)->SetRadius(fRadius);
	(*pCol)->SetScale(Get_Scale());
}

void CRun_Unit::Create_DamageFont(E_NumberFontType eFontType, _int iDamage, _bool bCritical)
{
	CUI_Damage* pDamage = CUI_Damage::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), E_OBJTAG::OBJ_UI, 0.5f, eFontType, iDamage, bCritical);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Damage_Font, E_OBJTAG::OBJ_UI, pDamage);

	_int iSizeX = 30;
	_int iSizeY = 30;
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
	_vec3 vWorldPos = m_pColliderCom->GetPos();
	vWorldPos.y += m_pColliderCom->GetRadius() * 1.f;

	_vec2 vPos = CUtility::World2Ui(vWorldPos);
	vPos.y += 30.f;
	pDamage->Set_Ui_PosX(_int(vPos.x));
	pDamage->Set_Ui_PosY(_int(vPos.y));
}

void CRun_Unit::Add_Rotation_Quat(E_ROTATION eRotationType, const _float & fAngle)
{
	_quat		qRot;
	_matrix		matRot;

	D3DXQuaternionIdentity(&qRot);

	D3DXMatrixIdentity(&matRot);

	switch (eRotationType)
	{
	case Engine::E_ROTATION::ROT_X:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_X], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;
		m_vQuatAngle.x += fAngle; 

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &m_vAxis[(_uint)E_ROTATION::ROT_Z], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Y], &m_vAxis[(_uint)E_ROTATION::ROT_Y], &matRot);
		break;

	case Engine::E_ROTATION::ROT_Y:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_Y], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;
		m_vQuatAngle.y += fAngle;

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_X], &m_vAxis[(_uint)E_ROTATION::ROT_X], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Z], &m_vAxis[(_uint)E_ROTATION::ROT_Z], &matRot);
		break;

	case Engine::E_ROTATION::ROT_Z:
		D3DXQuaternionRotationAxis(&qRot, &m_vAxis[(_uint)E_ROTATION::ROT_Z], D3DXToRadian(fAngle));
		m_quatRot = m_quatRot * qRot;
		m_vQuatAngle.z += fAngle;

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_X], &m_vAxis[(_uint)E_ROTATION::ROT_X], &matRot);
		D3DXVec3TransformNormal(&m_vAxis[(_uint)E_ROTATION::ROT_Y], &m_vAxis[(_uint)E_ROTATION::ROT_Y], &matRot);
		break;

	default:
		MSG_BOX("CRun_Player->Add_Rotation_Quat() : 회전축 입력 오류");
		break;
	}

	for (_uint i = 0; i < (_uint)E_ROTATION::ROT_END; ++i)
	{
		D3DXVec3Normalize(&m_vAxis[i], &m_vAxis[i]); 
	}

	D3DXMatrixRotationQuaternion(&m_matRot, &m_quatRot);
}
