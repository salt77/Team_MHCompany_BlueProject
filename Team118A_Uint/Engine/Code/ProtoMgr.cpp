#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

CProtoMgr::CProtoMgr()
{
}


CProtoMgr::~CProtoMgr()
{
	Free();
}

void CProtoMgr::Free()
{
	for_each(m_mapMeshTexture.begin(), m_mapMeshTexture.end(), CDeleteMap());
	m_mapMeshTexture.clear();

	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

HRESULT Engine::CProtoMgr::Ready_Prototype(const wstring pProtoTag, CComponent* pInstance)
{
	
	CComponent*        pComponent = Find_Prototype(pProtoTag);

	if (nullptr != pComponent)
		return E_FAIL;

	m_mapPrototype.emplace(pProtoTag, pInstance);

	// #ifdef _DEBUG
	// 	char szTag[MAX_PATH]{};
	// 	CUtility::TCHAR_TO_CHAR(pProtoTag, szTag);
	// 	cout << "프로토타입 Ready 성공 : " << szTag << endl;
	// #endif 

	return S_OK;
}

Engine::CComponent* Engine::CProtoMgr::Clone_Prototype(const wstring pProtoTag)
{
	CComponent*        pComponent = Find_Prototype(pProtoTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}

Engine::CComponent* Engine::CProtoMgr::Find_Prototype(const wstring pProtoTag)
{
	auto    iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

HRESULT CProtoMgr::Delete_Prototype(const wstring pProtoTag)
{
	CComponent*		pComponent = Find_Prototype(pProtoTag);

	if (nullptr == pComponent)
	{
		return E_FAIL;
	}
	else
	{
		Safe_Release(pComponent);
		m_mapPrototype.erase(pProtoTag);
	}

	return S_OK;
}

HRESULT CProtoMgr::Get_Texture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFullPath, LPDIRECT3DTEXTURE9 * ppOut)
{
	_int iCheck = (_int)wstrFullPath.rfind(L"/") + 1;
	wstring FileName = wstrFullPath.substr(iCheck);

	auto iter = find_if(m_mapMeshTexture.begin(), m_mapMeshTexture.end(), CTag_Finder(FileName));


	if (wstrFullPath == L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Event_Token_3.png")
	{
		int temp;
		temp = 3;
	}
	if (iter == m_mapMeshTexture.end())
	{
		LPDIRECT3DTEXTURE9 pTex;
		if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, wstrFullPath.c_str(), &pTex)))
		{
			return E_FAIL;  
		}

		D3DXIMAGE_INFO	tImageInfo;
		ZeroMemory(&tImageInfo, sizeof(D3DXIMAGE_INFO));
		if (FAILED(D3DXGetImageInfoFromFile(wstrFullPath.c_str(), &tImageInfo)))
		{
			return E_FAIL;
		}

		// #ifdef _DEBUG
		// 		char szFileName[MAX_PATH]{};
		// 		CUtility::TCHAR_TO_CHAR(FileName, szFileName);
		// 		cout << "텍스쳐 공유 성공 : " << szFileName << endl;
		// #endif 

		m_mapMeshTexture.emplace(FileName, pTex);
		m_mapTextureInfo[wstrFullPath] = tImageInfo;
		
		*ppOut = pTex;
	}
	else
	{
		*ppOut = (iter->second);
	}

	return S_OK;
}

D3DXIMAGE_INFO CProtoMgr::Get_TextureInfo(wstring wstrFullPath)
{
	return m_mapTextureInfo[wstrFullPath];
}
