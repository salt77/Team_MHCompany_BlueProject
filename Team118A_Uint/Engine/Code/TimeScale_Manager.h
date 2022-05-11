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
	/////		타임스케일 조절		/////////

	//										타임스케일을 초기화한다 (1.f)
	void		Reset_TimeScale();
	//										타임스케일을 fTimeScale로 변경합니다.
	void		Set_TimeScale(float fTimeScale);
	//										타임스케일을 fTimeScale로 fTimeDuration동안 변경합니다.
	void		Set_TimeScale_Duration(float fTimeScale, float fTimeDuration);
	//										fTimer의 시간 후에 타임스케일을 fTimeScale로 fTimeDuration동안 변경합니다.(bSwitch는 이미 있는 Late를 대체함)
	void		Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch = FALSE);


public:
	/////		타임 정보 가져오기		/////////
	
	//										현재 타임스케일을 반환
	float		Get_TimeScale()				{ return m_fTimeScale; }
	//										현재 리얼타임 (스케일이 적용된 실제 적용되는 델타)을 반환
	float		Get_TimeReal()				{ return m_fTimeReal; }
	//										현재 델타타임 (원래 델타타임)을 반환
	float		Get_TimeDelta()				{ return m_fTimeDelta; }
	
	
private:

	// 속도 조절

	float		m_fTimeDelta = 0.f;
	float		m_fTimeReal = 0.f;
	float		m_fTimeScale = 1.f;
	float		m_fTimeDuration = 0.f;


	// 지연 속도 조절

	float		m_bLate_Time = FALSE;
	float		m_fLate_Timer = 0.f;
	float		m_fLate_TimeScale = 1.f;
	float		m_fLate_TimeDuration = 0.f;


};

END
#endif // __TimeScale_Manager_H__