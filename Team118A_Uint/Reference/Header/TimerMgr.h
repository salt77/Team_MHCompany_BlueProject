#ifndef TimerMgr_h__
#define TimerMgr_h__

#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float		Get_TimeDelta(const wstring& pTimerTag);
	void		Set_TimeDelta(const wstring& pTimerTag);

public:
	HRESULT		Ready_Timer(const wstring& pTimerTag);

private:
	CTimer*		Find_Timer(const wstring& pTimerTag);

private:
	map<const wstring, CTimer*>		m_mapTimers;

public:
	virtual void	Free();

};

END
#endif // TimerMgr_h__
