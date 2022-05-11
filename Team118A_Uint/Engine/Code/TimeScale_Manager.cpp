#include "TimeScale_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimeScale_Manager)

CTimeScale_Manager::CTimeScale_Manager()
{
	m_fTimeScale = 1.f;
}


CTimeScale_Manager::~CTimeScale_Manager()
{
	Free();
}

void CTimeScale_Manager::Free()
{
}

// 델타 타임 계산, 시간 지연 계산
float CTimeScale_Manager::Update_TimeScale(const _float& fTimeDelta)
{
	// 지속시간동안 타임스케일 적용
	if (0.f < m_fTimeDuration) 
	{
		m_fTimeDuration -= fTimeDelta;

		if (0.f >= m_fTimeDuration)
		{
			Reset_TimeScale();
		}
	}
	
	// 지연
	if (m_bLate_Time)
	{
		m_fLate_Timer -= fTimeDelta;

		if (0.f >= m_fLate_Timer)
		{
			m_fLate_Timer = 0.f;
			m_bLate_Time = FALSE;

			m_fTimeScale = m_fLate_TimeScale;
			m_fTimeDuration = m_fLate_TimeDuration;
		}
	}
	
	m_fTimeDelta = fTimeDelta;
	m_fTimeReal = fTimeDelta * m_fTimeScale;

	return m_fTimeReal;
}

// 타임스케일 초기화
void CTimeScale_Manager::Reset_TimeScale()
{
	m_fTimeScale = 1.f;
	m_fTimeDuration = 0.f;
}

// 타임 스케일을 바로 조정함 (지연, 지속시간 등의 옵션 없음)
void CTimeScale_Manager::Set_TimeScale(float fTimeScale)
{
	m_fTimeScale = fTimeScale;
}

// 타임 스케일을 지속시간 동안 바꿈
void CTimeScale_Manager::Set_TimeScale_Duration(float fTimeScale, float fTimeDuration)
{
	m_fTimeScale = fTimeScale;
	m_fTimeDuration = fTimeDuration;
}

// 타임 스케일을 지연시간 후에 지속시간 동안 바꿈
// bSwitch의 옵션으로 유효한지, 덮어쓰울지 판단
void CTimeScale_Manager::Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch)
{
	if (bSwitch && m_bLate_Time)
	{
		// 이미 Set_TimeScale_Late가 호출되어 타임스케일의 예약이 있다.
		assert(FALSE);
		// bSwitch를 True로 호출하면 기존 예약을 덮어쓰움 
	}

	m_bLate_Time = TRUE;
	m_fLate_Timer = fTimer;
	m_fLate_TimeScale = fTimeScale;
	m_fLate_TimeDuration = fTimeDuration;
}