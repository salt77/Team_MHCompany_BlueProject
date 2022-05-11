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

	// 1. 만약 m_pEffect 값이 있고 m_pErrMsg 값이 없는 경우 : 코드가 정상적으로 작성되어 컴파일에 문제가 없는 경우
	// 2. 만약 m_pEffect 값이 없고 m_pErrMsg 값이 있는 경우 : 비상적인 코드로 인해 컴파일이 실패한 경우
	// 3. 만약 m_pEffect 값이 있고 m_pErrMsg 값도 있는 경우 : 컴파일 상에는 문제가 없으나 작성한 코드 중 경고성 메세지가 발생한 경우

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, 
										 D3DXSHADER_DEBUG, NULL, 
										 &m_pEffect, &m_pErrMsg)))
	
	// 쉐이더 에러
	{
		// Shader_Error
		assert(FALSE);

		return E_FAIL;
	}
	// 쉐이더 경고
	else if (nullptr != m_pErrMsg)
	{
		//MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
		//MessageBoxW(NULL, (_tchar*)m_pErrMsg->GetBufferPointer(), L"Shader_Warning", MB_OK);

		assert(FALSE);
	}
	// 쉐이더 문제 없음
	else
	{
		return S_OK;
	}

	m_iPassCount = 0;

	// 리턴
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
		// 패스가 열려 있다;
		assert(FALSE);
	}
	else  if (0 == m_iPassCount)
	{
		m_pEffect->End();
	}
	else
	{
		// 있을수 없는 경우의 수
		assert(FALSE);
	}
}

void CShader::BeginPass(_uint iPass)
{					
	if (iPass >= m_iMaxPass) 
	{
		// iPass 가 쉐이더의 최대 패스 개수(m_iMaxPass)보다 크다.
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
		// 원래 패스와 바꿀 패스가 동일하다.
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
		//m_iPassCount가 0 이하인데 패스를 닫을 수 없다..
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

