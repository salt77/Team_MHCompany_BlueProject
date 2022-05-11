#ifndef Timer_h__
#define Timer_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float		Get_TimeDelta() { return m_fTimeDelta; }

public:
	HRESULT		Ready_Timer();
	void		SetUp_TimeDelta();	// update 함수

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;

	LARGE_INTEGER		m_CpuTick;

private:
	_float				m_fTimeDelta;

public:
	static	CTimer*		Create();
	virtual void		Free();
};

END
#endif // Timer_h__


// GetTickCount				: 운영체제, 대략 1000 정도가 됐을 때 1초라고 판단
// QueryPerformenceCounter	: 하드웨어, 대략 1,000,000 정도가 됐을 대 1초라고 판단