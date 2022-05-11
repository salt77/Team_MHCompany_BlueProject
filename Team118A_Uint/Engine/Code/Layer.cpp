#include "Layer.h"
#include "Component.h"
#include "GameObject.h"

USING(Engine)

CLayer::CLayer()
	: m_bActive(TRUE)
{
}


CLayer::~CLayer()
{
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free()
{
	Clear_Layer();

}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int	iResult = RETURN_OK;

	auto iter = m_mapObject.begin();
	for (; iter != m_mapObject.end(); ++iter)
	{
		vector<CGameObject*>::iterator vecIter = iter->second.begin();

		for (; vecIter != iter->second.end();)
		{ 
			iResult = RETURN_OK;

			if ((*vecIter)->Get_Dead())
			{
				Safe_Release(*vecIter);
				vecIter = iter->second.erase(vecIter);
				continue;
			}

			if (FALSE == (*vecIter)->Get_Active())
			{
				++vecIter;
				continue;
			}
			else if ((*vecIter)->Get_Start())
			{
				(*vecIter)->Start_Object(fTimeDelta);
				++vecIter;
				continue;
			}
			else
			{
				iResult = (*vecIter)->Update_Object(fTimeDelta);
			}

			if (RETURN_DEAD == iResult)
			{
				Safe_Release(*vecIter);
				vecIter = iter->second.erase(vecIter);
			}
			else if (RETURN_ERROR == iResult)
			{
				assert(0);
			}
			else
			{
				++vecIter;
			}
		}
	}

	return iResult;
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	_int	iResult = RETURN_OK;

	auto iter = m_mapObject.begin();
	for (; iter != m_mapObject.end(); ++iter)
	{
		vector<CGameObject*>::iterator vecIter = iter->second.begin();

		for (; vecIter != iter->second.end();)
		{
			iResult = RETURN_OK;
			if ((*vecIter)->Get_Dead())
			{
				(*vecIter)->OnDestroy();
			}
			else if (FALSE == (*vecIter)->Get_Active())
			{
				++vecIter;
				continue;
			}
			else if (FALSE == (*vecIter)->Get_Start())
			{
				iResult = (*vecIter)->LateUpdate_Object(fTimeDelta);
			}
			
			if (RETURN_ERROR == iResult)
			{
				assert(0);
			}
			else
			{
				++vecIter;
			}
		}
	}

	return iResult;
}

#ifdef _DEBUG
void CLayer::Debugging_Console()
{
	_int iKeyCount = (_int)m_mapObject.size();
	cout << endl;
	cout << "--------   Layer   ---------" << endl;
	cout << "Layer 태그: " << (_int)m_eLayerTag << endl;
	cout << "Layer 에 있는 Object Key Count : " << iKeyCount << endl;

	for (auto& iter : m_mapObject)
	{
		cout << endl;
		_int iSize = (_int)iter.second.size();
		cout << "Vector Size : " << iSize << endl;
		vector<CGameObject*>::iterator vecIter = iter.second.begin();

		for (_int i = 0; i < iSize; i++)
		{
			(*vecIter)->Debug_This_In_Console();
		}
	}
}
#endif

HRESULT CLayer::Add_GameObject(const E_OBJTAG eObjTag, CGameObject * pInstance)
{
	assert(pInstance);

	auto	iter = m_mapObject.find(eObjTag);

	if (iter == m_mapObject.end())
	{
		vector<CGameObject*> vecTemp;
		m_mapObject.emplace(eObjTag, vecTemp);
		iter = m_mapObject.find(eObjTag);
	}

	iter->second.push_back(pInstance);

	return S_OK;
}

void CLayer::Clear_Layer()
{
	for (auto& iter : m_mapObject)
	{
		for (auto Obj : iter.second)
		{
			Safe_Release(Obj);
		}
		iter.second.clear();
	}
	m_mapObject.clear();
}

void CLayer::Set_Dead_AllObject()
{
	for (auto& iter : m_mapObject)
	{
		for (auto Obj : iter.second)
		{
			Obj->Set_Dead(TRUE);
		}
	}
}

void CLayer::Create_MapVector(const E_OBJTAG eObjTag)
{
	auto  iter = m_mapObject.find(eObjTag);

	if (iter == m_mapObject.end())
	{
		vector<CGameObject*> temp;
		m_mapObject.emplace(eObjTag, temp);
		iter = m_mapObject.find(eObjTag);
	}
}

CComponent * CLayer::Get_Component(const E_OBJTAG eObjTag, _uint iIdx, const wstring& pComponentTag)
{
	auto	iter = m_mapObject.find(eObjTag);

	assert(FALSE == (0 < iIdx && ((_int)iter->second.empty() || (_int)iter->second.size() <= iIdx)));

	if (iter == m_mapObject.end())
	{
		return nullptr;
	}

	return iter->second[iIdx]->Get_Component(pComponentTag);
}

CGameObject * CLayer::Get_GameObject(const E_OBJTAG eObjTag, _uint iIdx)
{
	auto iter = m_mapObject.find(eObjTag);

	if (iter == m_mapObject.end())
	{
		return nullptr;
	}

	if (0 < iIdx && ((_int)iter->second.empty() || (_int)iter->second.size() <= iIdx))
	{
		return nullptr;
	}

	return iter->second[iIdx];
}

vector<CGameObject*>* CLayer::Get_vecGameObject(const E_OBJTAG eObjTag)
{
	auto iter = m_mapObject.find(eObjTag);

	if (iter == m_mapObject.end())
	{
		return nullptr;
	}

	return &iter->second;
}

map<E_OBJTAG, vector<CGameObject*>>* CLayer::Get_LayerMapObject()
{
	return &m_mapObject;
}

_bool CLayer::IsExist_Obj(const E_OBJTAG eObjTag)
{
	auto iter = m_mapObject.find(eObjTag);

	if (iter == m_mapObject.end() || 0 == m_mapObject[eObjTag].size())
	{
		return false;
	}

	return true;
}
