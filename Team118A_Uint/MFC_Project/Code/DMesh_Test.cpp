#include "stdafx.h"
#include "../Include/stdafx.h"
#include "DMesh_Test.h"

USING(Engine)


CDMesh_Test::CDMesh_Test(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CUnit(pGraphicDev, eObjTag, wstrUnitName)
{
}

CDMesh_Test::CDMesh_Test(const CDMesh_Test& rhs)
	: CUnit(rhs)
{

}

CDMesh_Test::~CDMesh_Test()
{

}

HRESULT CDMesh_Test::Ready_Object()
{
	CGameObject::Ready_Object();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	m_pTexMouse = static_cast<CTexture*>(Emplace_Component(L"Com_TextureMouse", Management Clone_Prototype(L"Proto_CharacterMouse")));

	Set_Scale(2.5f, 2.5f, 2.5f);
	if (m_pDMeshCom)
	{
		m_pDMeshCom->Set_AnimationIndex_Loop(0);
	}
	return S_OK;
}

_int CDMesh_Test::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pDMeshCom)
	{
		if (m_bAction_OnOff)
		{
			m_pDMeshCom->Play_Animation(fTimeDelta);
			if (m_pDMeshCom->Is_AnimationsetFinish())
			{
				if (FALSE == m_qNextAnimIdx.empty())
				{
					_int iNext = m_qNextAnimIdx.front();
					m_qNextAnimIdx.pop();
					Set_ActionByEnum((E_ACTION)iNext);
				}
				else
				{
					m_bAction_OnOff = FALSE;
					// Set_AnimIndex(0);
				}
			}
		}
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
		m_pDMeshCom->Update_Component(fTimeDelta);

		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
	}
	else
	{
		if (m_wstrDMeshProtoTag == L"Empty")
			return iExit;

		CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrDMeshProtoTag);
		if (nullptr == pMesh)
		{
			return iExit;
		}

		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		Initialize_Trigger();
	}

	return iExit;
}

_int CDMesh_Test::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CUnit::LateUpdate_Object(fTimeDelta);
	return iExit;
}

void CDMesh_Test::Render_Object(_int iPass)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (m_pTexMouse)
	{
		m_pTexMouse->Set_Texture(pEffect, "g_MouseTexture");
	}
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));


	if (m_pDMeshCom)
	{
		m_pDMeshCom->Render_Meshes(m_pShaderCom);
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

CDMesh_Test* CDMesh_Test::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
{
	CDMesh_Test*	pInstance = new CDMesh_Test(pGraphicDev, eObjTag, wstrUnitName);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDMesh_Test::Free()
{
	CUnit::Free();
	Safe_Release(m_pDMeshCom);
}


void CDMesh_Test::Save(HANDLE hFile, DWORD & dwByte)
{
	if (m_wstrDMeshProtoTag == L"Empty")
	{
		CUtility::Print_Wstring_DMode(L"Save 실패 - ProtoTag가 Empty입니다.");
		return;
	}

	if (m_wstrDMeshFullPath == L"Empty")
	{
		CUtility::Print_Wstring_DMode(L"Save 실패 - FullPath가 Empty입니다.");
		return;
	}

	CUtility::Data_WriteWString(hFile, m_wstrDMeshProtoTag, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrDMeshFullPath, dwByte);

	Save_AnimAction(hFile, dwByte);
	Save_AnimTrigger(hFile, dwByte);
}

void CDMesh_Test::Load(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_ReadWString(hFile, m_wstrDMeshProtoTag, dwByte);
	CUtility::Data_ReadWString(hFile, m_wstrDMeshFullPath, dwByte);
	CUtility::Print_Wstring_DMode(L"Load 성공 - ProtoTag [  " + m_wstrDMeshProtoTag + L"  ]");
	CUtility::Print_Wstring_DMode(L"Load 성공 - FullPath [  " + m_wstrDMeshFullPath + L"  ]");

	Load_AnimAction(hFile, dwByte);
	Load_AnimTrigger(hFile, dwByte);
}


void CDMesh_Test::Set_AnimForMFC(_int iIndex)
{
	m_pDMeshCom->Set_AnimationIndex_Loop(iIndex);
}

void CDMesh_Test::Set_DMesh(wstring wstrFullPath, wstring wstrName)
{
	if(m_pDMeshCom)
	{
		m_pDMeshCom->Clear_Bone();
		Safe_Release(m_pDMeshCom);
	}
	m_pDMeshCom = nullptr;

	m_wstrDMeshProtoTag = wstrName;
	m_wstrDMeshFullPath = wstrFullPath;

	CComponent* pMesh = Management Clone_Prototype(m_wstrDMeshProtoTag);

	if (nullptr == pMesh)
	{
		CUtility::Print_Wstring_DMode(L"프로토타입이 없어서 새로운 메쉬 프로토타입 생성");
		if (E_FAIL == Management Ready_Prototype(m_wstrDMeshProtoTag,
			CDMesh::Create(m_pGraphicDev, wstrFullPath.c_str(), m_wstrDMeshProtoTag.c_str())))
		{
			assert(FALSE);
		}
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"메쉬 찾기 성공");
		m_pDMeshCom = static_cast<CDMesh*>(pMesh);
		Initialize_Trigger();
	}
}

_bool CDMesh_Test::Find_TestMesh()
{
	if (m_pDMeshCom)
		return TRUE;

	if (m_wstrDMeshProtoTag == L"Empty")
		return FALSE;

	CComponent* pMesh = CProtoMgr::GetInstance()->Clone_Prototype(m_wstrDMeshProtoTag);
	if (nullptr == pMesh)
	{
		return FALSE;
	}

	// 메쉬 찾음

	m_pDMeshCom = static_cast<CDMesh*>(pMesh);
	Initialize_Trigger();

	return TRUE;
}

void CDMesh_Test::Choose_ActionForTest(wstring wstrActionName)
{
	for (auto& iter : m_vecAnimActions)
	{
		if (iter.wstrActionKey == wstrActionName)
		{
			m_bAction_OnOff = TRUE;
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
					CUtility::Print_Wstring_DMode(L"애니메이션 실행 - Index" + to_wstring(iFrontAnimIndex));
					Set_ActionByEnum((E_ACTION)iFrontAnimIndex);
				}
			}
			break;
		}
	}
}

void CDMesh_Test::Delete_T_AnimTrgByIndex(_int iAnimIndex, _int iIndex)
{
	if ((_int)m_vecAnimTrg.size() <= iAnimIndex)
	{
		CUtility::Print_Wstring_DMode(L"트리거 삭제 실패 - iAnimIndex넘어섬");
		return;
	}

	if ((_int)m_vecAnimTrg[iAnimIndex].size() <= iIndex)
	{
		CUtility::Print_Wstring_DMode(L"트리거 삭제 실패 - Index넘어섬");
		return;
	}

	auto& iter = m_vecAnimTrg[iAnimIndex].begin();
	for (_int i = 0; i < iIndex; i++)
	{
		++iter;
	}
	m_vecAnimTrg[iAnimIndex].erase(iter);

	CUtility::Print_Wstring_DMode(L"트리거 삭제 완료 - " + to_wstring(iAnimIndex));
}

void CDMesh_Test::Delete_T_AnimActionSysByName(wstring wstrName)
{
	_bool bDelete = FALSE;
	_int iActionSize = (_int)m_vecAnimActions.size();
	for (_int i = 0; i < iActionSize; ++i)
	{
		if (m_vecAnimActions[i].wstrActionKey == wstrName)
		{
			while (!m_vecAnimActions[i].qAnimation.empty())
			{
				m_vecAnimActions[i].qAnimation.pop();
			}
			bDelete = TRUE;
			break;
		}
	}

	if(bDelete)
		CUtility::Print_Wstring_DMode(L"액션 삭제 성공 - " + wstrName);
	else
		CUtility::Print_Wstring_DMode(L"액션 삭제 실패 - " + wstrName);
}

void CDMesh_Test::Clear_Action()
{
	Initialize_Trigger();
}

void CDMesh_Test::Save_AnimTrigger(HANDLE hFile, DWORD& dwByte)
{
	_int iAnimMaxSize = (_int)m_vecAnimTrg.size();
	CUtility::Data_WriteInt(hFile, iAnimMaxSize, dwByte);

	for (_int i = 0; i < iAnimMaxSize; ++i)
	{
		_int iTrgCount = (_int)m_vecAnimTrg[i].size();
		CUtility::Data_WriteInt(hFile, iTrgCount, dwByte);

		for (auto& iter : m_vecAnimTrg[i])
		{
			CUtility::Data_WriteInt(hFile, (_int)iter.eTrigger, dwByte);
			CUtility::Data_WriteFloat(hFile, iter.fActiveDeltaTime, dwByte);
			CUtility::Data_WriteInt(hFile, iter.iTriggerFrame, dwByte);
			CUtility::Data_WriteInt(hFile, iter.iMaxFrame, dwByte);
			CUtility::Data_WriteInt(hFile, iter.iTrgIndex, dwByte);
		}
		CUtility::Print_Wstring_DMode(L"Animation Index " + to_wstring(i) + L"에 트리거 " + to_wstring(iTrgCount) + L"개 저장 성공");
	}
}

void CDMesh_Test::Save_AnimAction(HANDLE hFile, DWORD& dwByte)
{
	_int iActionSize = (_int)m_vecAnimActions.size();
	CUtility::Data_WriteInt(hFile, iActionSize, dwByte);

	for (_int i = 0; i < iActionSize; ++i)
	{
		T_AnimActionSys* pAction = &m_vecAnimActions[i];

		CUtility::Data_WriteWString(hFile, pAction->wstrActionKey, dwByte);

		queue<_int> qCopy = pAction->qAnimation;

		_int iQueueSize = (_int)qCopy.size();
		CUtility::Data_WriteInt(hFile, iQueueSize, dwByte);
		for (_int j = 0; j < iQueueSize; ++j)
		{
			CUtility::Data_WriteInt(hFile, qCopy.front(), dwByte);
			qCopy.pop();
		}

		CUtility::Print_Wstring_DMode(pAction->wstrActionKey + L" Action " + to_wstring(iQueueSize) + L"개 저장 성공");
	}
}

void CDMesh_Test::Load_AnimTrigger(HANDLE hFile, DWORD& dwByte)
{
	_int iAnimMaxSize = 0;
	CUtility::Data_ReadInt(hFile, iAnimMaxSize, dwByte);
	if (iAnimMaxSize != (_int)m_vecAnimTrg.size())
	{
		CUtility::Print_Wstring_DMode(L"Load_AnimTrigger() - 잘못된 파일입니다.  (원인 iAnimMaxSize)");
		CUtility::Print_Wstring_DMode(L"iAnimMaxSize -" + to_wstring(iAnimMaxSize));
		CUtility::Print_Wstring_DMode(L"m_vecAnimTrg.size() -" + to_wstring((_int)m_vecAnimTrg.size()));
		return;
	}

	m_vecAnimTrg.clear();
	m_vecAnimTrg.reserve(iAnimMaxSize);
	m_vecAnimTrg.resize(iAnimMaxSize);

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

			if (E_Trigger::Trigger_End == tTrigger.eTrigger)
			{
				CUtility::Print_Wstring_DMode(L"Load_AnimTrigger 실패 - strTriggerName == Empty");
			}
			else
			{
				m_vecAnimTrg[i].push_back(tTrigger);
			}
		}
		CUtility::Print_Wstring_DMode(L"Animation Index " + to_wstring(i) + L"에 트리거 " + to_wstring(iTrgCount) + L"개 로드 성공");
	}

}

void CDMesh_Test::Load_AnimAction(HANDLE hFile, DWORD& dwByte)
{

	_int iActionSize = 0;
	CUtility::Data_ReadInt(hFile, iActionSize, dwByte);
	if (iActionSize != (_int)m_vecAnimActions.size())
	{
		CUtility::Print_Wstring_DMode(L"Load_AnimAction() - 잘못된 파일입니다.  (원인 iActionSize)");
		return;
	}

	for (_int i = 0; i < iActionSize; ++i)
	{
		T_AnimActionSys* pAction = &m_vecAnimActions[i];

		while (!m_vecAnimActions[i].qAnimation.empty())
		{
			m_vecAnimActions[i].qAnimation.pop();
		}
		m_vecAnimActions[i].wstrActionKey = L"Empty";

		CUtility::Data_ReadWString(hFile, pAction->wstrActionKey, dwByte);

		_int iQueueSize = 0;
		CUtility::Data_ReadInt(hFile, iQueueSize, dwByte);

		_int iTrgIndex = 0;
		for (_int j = 0; j < iQueueSize; ++j)
		{
			CUtility::Data_ReadInt(hFile, iTrgIndex, dwByte);
			pAction->qAnimation.push(iTrgIndex);
		}

		CUtility::Print_Wstring_DMode(pAction->wstrActionKey + L" Action " + to_wstring(iQueueSize) + L"개 로드 성공");
	}
}

void CDMesh_Test::Add_AnimAction(wstring wstrActionName, _int iAnimIdx)
{
	for (_int i = 0; i < (_int)m_vecAnimActions.size(); ++i)
	{
		if (m_vecAnimActions[i].wstrActionKey == wstrActionName)
		{
			CUtility::Print_Wstring_DMode(wstrActionName + L" 액션에 -> " + to_wstring(iAnimIdx) + L"추가");
			m_vecAnimActions[i].qAnimation.push(iAnimIdx);
		}
	}
}