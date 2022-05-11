#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{

}

Engine::CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_iMaxPass(rhs.m_iMaxPass)
	, m_iNowPass(rhs.m_iNowPass)
	, m_iPassCount(rhs.m_iPassCount)
	, m_bDMesh(rhs.m_bDMesh)
{
	m_pEffect->AddRef();
}

Engine::CShader::~CShader()
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath, _uint iMaxPass)
{
	m_iMaxPass = iMaxPass;
	if (0 > m_iMaxPass) 
	{
		assert(FALSE);
		return E_FAIL;
	}

	// 1. ���� m_pEffect ���� �ְ� m_pErrMsg ���� ���� ��� : �ڵ尡 ���������� �ۼ��Ǿ� �����Ͽ� ������ ���� ���
	// 2. ���� m_pEffect ���� ���� m_pErrMsg ���� �ִ� ��� : ������� �ڵ�� ���� �������� ������ ���
	// 3. ���� m_pEffect ���� �ְ� m_pErrMsg ���� �ִ� ��� : ������ �󿡴� ������ ������ �ۼ��� �ڵ� �� ��� �޼����� �߻��� ���

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, 
										 D3DXSHADER_DEBUG, NULL, 
										 &m_pEffect, &m_pErrMsg)))
	
	// ���̴� ����
	{
		// Shader_Error
		assert(FALSE);

		return E_FAIL;
	}
	// ���̴� ���
	else if (nullptr != m_pErrMsg)
	{
		//MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
		//MessageBoxW(NULL, (_tchar*)m_pErrMsg->GetBufferPointer(), L"Shader_Warning", MB_OK);

		assert(FALSE);
	}
	// ���̴� ���� ����
	else
	{
		return S_OK;
	}

	m_iPassCount = 0;

	// ����
	return S_OK;
}

void CShader::BeginShader()
{
	m_pEffect->Begin(&m_iMaxPass, NULL);
}

void CShader::EndShader()
{
	if (1 <= m_iPassCount) 
	{
		// �н��� ���� �ִ�;
		assert(FALSE);
	}
	else  if (0 == m_iPassCount)
	{
		m_pEffect->End();
	}
	else
	{
		// ������ ���� ����� ��
		assert(FALSE);
	}
}

void CShader::BeginPass(_uint iPass)
{					
	if (iPass >= m_iMaxPass) 
	{
		// iPass �� ���̴��� �ִ� �н� ����(m_iMaxPass)���� ũ��.
		assert(FALSE);
	}
	
	m_pEffect->BeginPass(iPass);

	m_iNowPass = iPass;

	m_iPassCount++;
}

void CShader::ChangePass(_uint iPass)
{
	if (m_iNowPass == iPass) 
	{
		// ���� �н��� �ٲ� �н��� �����ϴ�.
		assert(FALSE);
	}
	else
	{
		EndPass();

		BeginPass(iPass);
	}
}

void CShader::EndPass()
{
	if (0 < m_iPassCount) 
	{
		m_pEffect->EndPass();

		m_iPassCount--;
	}
	else
	{
		//m_iPassCount�� 0 �����ε� �н��� ���� �� ����..
		assert(FALSE);
	}
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint	iMaxPass, const _tchar* pFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath, iMaxPass)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone()
{
	return new CShader(*this);
}

void Engine::CShader::Free()
{
	Safe_Release(m_pEffect);

	CComponent::Free();
}

