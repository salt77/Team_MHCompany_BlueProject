#include "FrameMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

Engine::_bool Engine::CFrameMgr::IsPermit_Call(const wstring& pFrameTag,
											const _float& fTimeDelta)
{
	CFrame*		pInstance = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pInstance, false);

	return pInstance->IsPermit_Call(fTimeDelta);
}

_float CFrameMgr::Get_AccTimeDelta(const wstring& pFrameTag)
{
	CFrame*		pInstance = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pInstance, false);

	return pInstance->Get_AccTimeDelta();
}

HRESULT Engine::CFrameMgr::Ready_Frame(const wstring& pFrameTag, const _float& fCallLimit)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.emplace(pFrameTag, pFrame);

	return S_OK;
}

CFrame* Engine::CFrameMgr::Find_Frame(const wstring& pFrameTag)
{
	auto	iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_Finder(pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}

