#pragma once
#ifndef __ANICTRL_H__
#define __ANICTRL_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

// 파일 가져온 사람 : 김민현

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl();

public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free();

	HRESULT			Ready_AniCtrl();

public:
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl() { return m_pAniCtrl; }

	HRESULT			Bind_Frames(D3DXFRAME_DERIVED * pRootFrame);
public:
	void			Play_Animation(const _float& fTimeDelta);
public:
	void			Set_AnimaRatioFor_Stop();							// MFC 툴에서 프레임단위로 애니메이션을 보기위한 함수 실제 유닛에서는 쓰지 않는다.
	void			Set_AnimationIndex_Loop(const _uint& iIndex);		// 실제 유닛의 Animation을 Set해주기 위한 함수.
	void			Set_AnimaRatio(_float fRatio);

public:
	_bool			Is_AnimationsetFinish();
	_uint			Get_AnimCountMax();
	_uint			Get_OldAniIndex();
	const _float	Get_AnimRatio();								// 현재 애니메이션 진행 비율	(1초중 0.5초 진행중이라면 0.5f 반환)
	const _float	Get_AnimMaxTime();								// 현재 애니메이션 실제시간	(실제 시간으로 1초 짜리 애니메이션이라면 1초 반환)
	const _int		Get_AnimCurrentFrame();							// 현재 애니메이션 Current	프레임
	const _int		Get_AnimMaxFrame();								// 현재 애니메이션 Max		프레임

	_float			Get_CurAniTime();
	_float			Get_CurAniEndTime();
	

private:
	LPD3DXANIMATIONCONTROLLER				m_pAniCtrl;
	_uint									m_iCurrentTrack;
	_uint									m_iNewTrack;
	_double									m_dAccTime;
	_uint									m_iOldAniIndex;
	_double									m_dPeriod;
	_uint									m_iAnimCountMax;
	_double									m_dStartPeriod;
	_double									m_dOldAnimTime = 0;

	_float									m_fFrame60 = 1.f / 60.f;
	
	_float									m_fAniSpeed = 1.f; 

};

END
#endif // __ANICTRL_H__
