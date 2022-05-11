#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame()
	: m_fAccTimeDelta(0.f)
	, m_fCallLimit(0.f)
{

}

Engine::CFrame::~CFrame()
{

}

Engine::_bool Engine::CFrame::IsPermit_Call(const _float& fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if(m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fLastAcc = m_fAccTimeDelta;
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}
Engine::_float Engine::CFrame::Get_AccTimeDelta()
{
	return m_fLastAcc;
}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame* Engine::CFrame::Create(const _float& fCallLimit)
{
	CFrame*	pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(fCallLimit)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrame::Free()
{

}

