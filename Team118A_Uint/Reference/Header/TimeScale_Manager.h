#pragma once
#ifndef __TimeScale_Manager_H__
#define __TimeScale_Manager_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimeScale_Manager : public CBase
{
	DECLARE_SINGLETON(CTimeScale_Manager)

private:
	CTimeScale_Manager(); 
	virtual ~CTimeScale_Manager();
	virtual void Free() override;
		
public:
	float		Update_TimeScale(const _float& fTimeDelta);

public:
	/////		Ÿ�ӽ����� ����		/////////

	//										Ÿ�ӽ������� �ʱ�ȭ�Ѵ� (1.f)
	void		Reset_TimeScale();
	//										Ÿ�ӽ������� fTimeScale�� �����մϴ�.
	void		Set_TimeScale(float fTimeScale);
	//										Ÿ�ӽ������� fTimeScale�� fTimeDuration���� �����մϴ�.
	void		Set_TimeScale_Duration(float fTimeScale, float fTimeDuration);
	//										fTimer�� �ð� �Ŀ� Ÿ�ӽ������� fTimeScale�� fTimeDuration���� �����մϴ�.(bSwitch�� �̹� �ִ� Late�� ��ü��)
	void		Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch = FALSE);


public:
	/////		Ÿ�� ���� ��������		/////////
	
	//										���� Ÿ�ӽ������� ��ȯ
	float		Get_TimeScale()				{ return m_fTimeScale; }
	//										���� ����Ÿ�� (�������� ����� ���� ����Ǵ� ��Ÿ)�� ��ȯ
	float		Get_TimeReal()				{ return m_fTimeReal; }
	//										���� ��ŸŸ�� (���� ��ŸŸ��)�� ��ȯ
	float		Get_TimeDelta()				{ return m_fTimeDelta; }
	
	
private:

	// �ӵ� ����

	float		m_fTimeDelta = 0.f;
	float		m_fTimeReal = 0.f;
	float		m_fTimeScale = 1.f;
	float		m_fTimeDuration = 0.f;


	// ���� �ӵ� ����

	float		m_bLate_Time = FALSE;
	float		m_fLate_Timer = 0.f;
	float		m_fLate_TimeScale = 1.f;
	float		m_fLate_TimeDuration = 0.f;


};

END
#endif // __TimeScale_Manager_H__