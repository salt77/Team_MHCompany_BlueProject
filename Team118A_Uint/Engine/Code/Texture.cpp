#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)	
{
}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CComponent(rhs)	
{
	_uint	iContainerSize = (_uint)rhs.m_vecTexture.size();

	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;

	memcpy(&m_tTexInfo, &rhs.m_tTexInfo, sizeof(T_TEXINFO));

	for (auto& iter : m_vecTexture)
		iter->AddRef();
}

Engine::CTexture::~CTexture()
{

}

CComponent* Engine::CTexture::Clone()
{
	return new CTexture(*this);
}

void Engine::CTexture::Free()
{
	for (auto& iter : m_vecTexture)
		Safe_Release(iter);

	m_vecTexture.clear();

	CComponent::Free();

}

void CTexture::Save(HANDLE hFile)
{
	DWORD dwByte;
	CUtility::Data_WriteInt(hFile, m_iTexOffSet, dwByte);
	CUtility::Data_WriteInt(hFile, m_iTexNum, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrProtoTag, dwByte);
}

void CTexture::Load(HANDLE hFile)
{
	DWORD dwByte;
	CUtility::Data_ReadInt(hFile, m_iTexOffSet, dwByte);
	CUtility::Data_ReadInt(hFile, m_iTexNum, dwByte);
	CUtility::Data_ReadWString(hFile, m_wstrProtoTag, dwByte);
}

HRESULT Engine::CTexture::Ready_Texture(const wstring& pPath,
										E_TEXTURE_TYPE eType,
										const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;
	
	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR	szFileName[256] = L"";

		wsprintf(szFileName, pPath.c_str(), i);
		switch (eType)
		{
		case E_TEXTURE_TYPE::TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case E_TEXTURE_TYPE::TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}
	Set_TexInfo(pPath);

	return S_OK;
}

void CTexture::Set_Texture_Device()
{
	m_pGraphicDev->SetTexture(0, m_vecTexture[0]);
}

void CTexture::Set_Texture(LPD3DXEFFECT pEffect, const char * pConstantTable, const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	// 텍스쳐를 그리기 하는 함수
	pEffect->SetTexture(pConstantTable, m_vecTexture[iIndex]);
}

void CTexture::Set_TexOffSet(_uint iIdx)
{
	m_iTexOffSet = iIdx;
}

void CTexture::Set_TexNum(_uint iIdx)
{
	m_iTexNum = iIdx;
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const wstring& pPath,
									E_TEXTURE_TYPE eType, 
									const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CTexture::Set_TexInfo(const wstring& pPath)
{
	if (FAILED(D3DXGetImageInfoFromFile(pPath.c_str(), &m_tTexInfo.tImageInfo)))
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(m_pGraphicDev,
		pPath.c_str(),
		m_tTexInfo.tImageInfo.Width, // 그림 가로
		m_tTexInfo.tImageInfo.Height, // 그림 세로 
		m_tTexInfo.tImageInfo.MipLevels, // 밉레벨 
		// 이미지 밉래벨은 있을수도 있고 없을 수도 있다. 
		0,
		m_tTexInfo.tImageInfo.Format,
		D3DPOOL_MANAGED, // 우리는 짬짜면을 쓰겠다. ㅇㅋ? 
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_tTexInfo.pTexture)))
	{
		return E_FAIL;
	}

	m_pGraphicDev->Release();
	return S_OK;
}

const T_TEXINFO & CTexture::Get_TexInfo()
{
	return m_tTexInfo;
}
