#include "TimerMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr()
{

}

Engine::CTimerMgr::~CTimerMgr()
{
	Free();	// 소멸자에서 free 호출한다.
}

HRESULT Engine::CTimerMgr::Ready_Timer(const wstring& pTimerTag)	// 타이머 생성 용도
{
	CTimer*	pTimer = Find_Timer(pTimerTag);
	
	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimers.emplace(pTimerTag, pTimer);

	return S_OK;
}


Engine::CTimer* Engine::CTimerMgr::Find_Timer(const wstring& pTimerTag)
{
	// auto	iter = m_mapTimers.find(pTimerTag); // 오류 발생 가능성이 있음
	
	auto	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTag_Finder(pTimerTag));
	// 함수 포인터, 함수 객체, 람다
		
	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}


Engine::_float	Engine::CTimerMgr::Get_TimeDelta(const wstring& pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}
void		Engine::CTimerMgr::Set_TimeDelta(const wstring& pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return;

	pTimer->SetUp_TimeDelta();
}

void Engine::CTimerMgr::Free()
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CDeleteMap());
	m_mapTimers.clear();
}