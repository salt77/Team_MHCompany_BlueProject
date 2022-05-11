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

// ��Ÿ Ÿ�� ���, �ð� ���� ���
float CTimeScale_Manager::Update_TimeScale(const _float& fTimeDelta)
{
	// ���ӽð����� Ÿ�ӽ����� ����
	if (0.f < m_fTimeDuration) 
	{
		m_fTimeDuration -= fTimeDelta;

		if (0.f >= m_fTimeDuration)
		{
			Reset_TimeScale();
		}
	}
	
	// ����
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

// Ÿ�ӽ����� �ʱ�ȭ
void CTimeScale_Manager::Reset_TimeScale()
{
	m_fTimeScale = 1.f;
	m_fTimeDuration = 0.f;
}

// Ÿ�� �������� �ٷ� ������ (����, ���ӽð� ���� �ɼ� ����)
void CTimeScale_Manager::Set_TimeScale(float fTimeScale)
{
	m_fTimeScale = fTimeScale;
}

// Ÿ�� �������� ���ӽð� ���� �ٲ�
void CTimeScale_Manager::Set_TimeScale_Duration(float fTimeScale, float fTimeDuration)
{
	m_fTimeScale = fTimeScale;
	m_fTimeDuration = fTimeDuration;
}

// Ÿ�� �������� �����ð� �Ŀ� ���ӽð� ���� �ٲ�
// bSwitch�� �ɼ����� ��ȿ����, ������� �Ǵ�
void CTimeScale_Manager::Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch)
{
	if (bSwitch && m_bLate_Time)
	{
		// �̹� Set_TimeScale_Late�� ȣ��Ǿ� Ÿ�ӽ������� ������ �ִ�.
		assert(FALSE);
		// bSwitch�� True�� ȣ���ϸ� ���� ������ ����� 
	}

	m_bLate_Time = TRUE;
	m_fLate_Timer = fTimer;
	m_fLate_TimeScale = fTimeScale;
	m_fLate_TimeDuration = fTimeDuration;
}