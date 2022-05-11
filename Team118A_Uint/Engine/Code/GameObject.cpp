#include "GameObject.h"
#include "ProtoMgr.h"
#include "KeyManager.h"
#include "LightMgr.h"
#include "FogManager.h"

USING(Engine)

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: m_pGraphicDev(pGraphicDev)
	, m_eObjTag(eObjTag)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_eObjTag(rhs.m_eObjTag)
{
}

CGameObject::~CGameObject()
{

}

HRESULT CGameObject::Ready_Object()
{
	CComponent* pComponent = m_pTransformCom = static_cast<CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	m_mapComponent.emplace(ComTag_Transform, pComponent);

	assert(m_pTransformCom);

	return S_OK;
}

_int CGameObject::Start_Object(const _float & fTimeDelta)
{
	m_bStart = FALSE;
	m_pTransformCom->Update_Component(fTimeDelta);

	//Set_Scale(VEC_ONE * m_fScale); 

	return RETURN_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return RETURN_DEAD;

	m_pTransformCom->Update_Component(fTimeDelta);

	return RETURN_OK;
}

_int CGameObject::LateUpdate_Object(const _float & fTimeDelta)
{
	Update_Invoke(fTimeDelta);

	return RETURN_OK;
}

void CGameObject::Free()
{
	Release_Invoke();

	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();

	Safe_Release(m_pGraphicDev);
}

void CGameObject::Save(HANDLE hFile, DWORD& dwByte)
{
	CUtility::Data_WriteInt(hFile, (_int)m_eObjTag, dwByte);

	for (auto iter : m_mapComponent)
	{
		iter.second->Save(hFile, dwByte);
	}

	CUtility::Data_WriteBoolean(hFile, m_bActive, dwByte);


	CUtility::Data_WriteInt(hFile, m_iPassNumber, dwByte);
}

void CGameObject::Load(HANDLE hFile, DWORD& dwByte)
{
	for (auto iter : m_mapComponent)
	{
		iter.second->Load(hFile, dwByte);
	}

	CUtility::Data_ReadBoolean(hFile, m_bActive, dwByte);

	CUtility::Data_ReadInt(hFile, m_iPassNumber, dwByte);
	assert(0 <= m_iPassNumber);
}

void CGameObject::Set_Constantable_Fog(CShader* pShader)
{
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
	_vec4 vColor = (_vec4)CFogManager::GetInstance()->Get_FogColor();
	pEffect->SetVector("g_vFogColor", &vColor);
}

void CGameObject::Debug_This_In_Console()
{
#ifdef _DEBUG
	cout << endl;

	char g_szConsoleText[MAX_PATH]{};

	sprintf_s(g_szConsoleText, sizeof(g_szConsoleText),
		"GameObject [Tag : %d]",
		(_int)m_eObjTag);
	cout << g_szConsoleText << endl;

	sprintf_s(g_szConsoleText, sizeof(g_szConsoleText),
		"Scale (%d, %d, %d)",
		(_int)Get_Scale().x,
		(_int)Get_Scale().y,
		(_int)Get_Scale().z);
	cout << g_szConsoleText << endl;

	sprintf_s(g_szConsoleText, sizeof(g_szConsoleText),
		"Pos(%d, %d, %d)",
		(_int)Get_Info(E_INFO::INFO_POS).x,
		(_int)Get_Info(E_INFO::INFO_POS).y,
		(_int)Get_Info(E_INFO::INFO_POS).z);
	cout << g_szConsoleText << endl;
#endif
}

CTransform * CGameObject::Get_Transform()
{
	return m_pTransformCom;
}

CComponent * CGameObject::Get_Component(const wstring& wstrComponentTag)
{
	map<const wstring, CComponent*>::iterator iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTag_Finder(wstrComponentTag));

	if (iter == m_mapComponent.end())
	{
		return nullptr;
	}

	return iter->second;
}

CComponent* CGameObject::Emplace_Component(const wstring & wstrComponentTag, CComponent * pComponent)
{
	m_mapComponent.emplace(wstrComponentTag, pComponent);
	return pComponent;
}

void CGameObject::Delete_Component(const wstring& wstrComponentTag)
{
	map<const wstring, CComponent*>::iterator iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTag_Finder(wstrComponentTag));

	if (iter == m_mapComponent.end())
	{
		return;
	}

	Safe_Release(iter->second);
	m_mapComponent.erase(iter);
}

void CGameObject::Replace_Component(const wstring& wstrComponentTag, CComponent * pComponent)
{
	map<const wstring, CComponent*>::iterator iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTag_Finder(wstrComponentTag));

	if (iter == m_mapComponent.end())
	{
		return;
	}

	Safe_Release(iter->second);

	m_mapComponent[wstrComponentTag] = pComponent;
}

void CGameObject::OnEnable()
{
}

void CGameObject::OnDisable()
{
}

void CGameObject::OnDestroy()
{
}

void CGameObject::On_Collision(CGameObject * pDest)
{
	// 자식에서 재정의
}

void CGameObject::Set_Active(_bool bState)
{
	_bool bBeforeActive = m_bActive;
	m_bActive = bState; 
	if (m_bActive && FALSE == bBeforeActive)
	{
		OnEnable();
	}
	else if(FALSE == m_bActive && TRUE == bBeforeActive)
	{
		OnDisable();
	}
}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix		matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

_matrix CGameObject::Billboard()
{
	_matrix matScale, matRot, matTrans;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixScaling(&matScale, Get_Scale().x, Get_Scale().y, Get_Scale().z);
	D3DXMatrixRotationYawPitchRoll(&matRot, Get_Angle().y, Get_Angle().x, Get_Angle().z);
	D3DXMatrixTranslation(&matTrans, Get_Info(E_INFO::INFO_POS).x, Get_Info(E_INFO::INFO_POS).y, Get_Info(E_INFO::INFO_POS).z);

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	_matrix matBill;
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	_matrix matWorld;

	matWorld = matScale * matRot * matBill * matTrans;

	return matWorld;
}

void CGameObject::Invoke_Add(_float fDelayTime, std::function<void(void)> Func)
{
	pair<_float, std::function<void(void)>>* pInvoke = new pair<_float, std::function<void(void)>>;
	pInvoke->first = fDelayTime;
	pInvoke->second = Func;
	m_qInvoke_void.push(pInvoke);
}

void CGameObject::Invoke_Add(_float fDelayTime, std::function<void(_bool)> Func, _bool Val)
{
	pair<_float, pair<_bool, std::function<void(_bool)>>>* pInvoke = new pair<_float, pair<_bool, std::function<void(_bool)>>>;
	pInvoke->first = fDelayTime;
	pInvoke->second.first = Val;
	pInvoke->second.second = Func;
	m_qInvoke_Bool.push(pInvoke);
}

void CGameObject::Invoke_Add(_float fDelayTime, std::function<void(_int)> Func, _int Val)
{
	pair<_float, pair<_int, std::function<void(_int)>>>* pInvoke = new pair<_float, pair<_int, std::function<void(_int)>>>;
	pInvoke->first = fDelayTime;
	pInvoke->second.first = Val;
	pInvoke->second.second = Func;
	m_qInvoke_Int.push(pInvoke);
}

void CGameObject::Invoke_Add(_float fDelayTime, std::function<void(_float)> Func, _float Val)
{
	pair<_float, pair<_float, std::function<void(_float)>>>* pInvoke = new pair<_float, pair<_float, std::function<void(_float)>>>;
	pInvoke->first = fDelayTime;
	pInvoke->second.first = Val;
	pInvoke->second.second = Func;
	m_qInvoke_Float.push(pInvoke);
}

void CGameObject::Release_Invoke()
{
	while (FALSE == m_qInvoke_void.empty())
	{
		auto pFront = m_qInvoke_void.front();
		Safe_Delete(pFront);
		m_qInvoke_void.pop();
	}
	while (FALSE == m_qInvoke_Bool.empty())
	{
		auto pFront = m_qInvoke_Bool.front();
		Safe_Delete(pFront);
		m_qInvoke_Bool.pop();
	}
	while (FALSE == m_qInvoke_Int.empty())
	{
		auto pFront = m_qInvoke_Int.front();
		Safe_Delete(pFront);
		m_qInvoke_Int.pop();
	}
	while (FALSE == m_qInvoke_Float.empty())
	{
		auto pFront = m_qInvoke_Float.front();
		Safe_Delete(pFront);
		m_qInvoke_Float.pop();
	}
}

void CGameObject::Update_Invoke(const _float& fTimeDelta)
{
	_int iSize_void = (_int)m_qInvoke_void.size();
	for (_int i = 0; i < iSize_void; ++i)
	{
		auto pFront = m_qInvoke_void.front();
		m_qInvoke_void.pop();

		pFront->first -= fTimeDelta;

		if (pFront->first < 0.f)
		{
			pFront->second();
			Safe_Delete(pFront);
			continue;
		}

		m_qInvoke_void.push(pFront);
	}

	_int iSize_Bool = (_int)m_qInvoke_Bool.size();
	for (_int i = 0; i < iSize_Bool; ++i)
	{
		auto pFront = m_qInvoke_Bool.front();
		m_qInvoke_Bool.pop();

		pFront->first -= fTimeDelta;

		if (pFront->first < 0.f)
		{
			pFront->second.second(pFront->second.first);
			Safe_Delete(pFront);
			continue;
		}

		m_qInvoke_Bool.push(pFront);
	}

	_int iSize_Int = (_int)m_qInvoke_Int.size();
	for (_int i = 0; i < iSize_Int; ++i)
	{
		auto pFront = m_qInvoke_Int.front();
		m_qInvoke_Int.pop();

		pFront->first -= fTimeDelta;

		if (pFront->first < 0.f)
		{
			pFront->second.second(pFront->second.first);
			Safe_Delete(pFront);
			continue;
		}

		m_qInvoke_Int.push(pFront);
	}

	_int iSize_Float = (_int)m_qInvoke_Float.size();
	for (_int i = 0; i < iSize_Float; ++i)
	{
		auto pFront = m_qInvoke_Float.front();
		m_qInvoke_Float.pop();

		pFront->first -= fTimeDelta;

		if (pFront->first < 0.f)
		{
			pFront->second.second(pFront->second.first);
			Safe_Delete(pFront);
			continue;
		}

		m_qInvoke_Float.push(pFront);
	}
}