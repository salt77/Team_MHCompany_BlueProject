#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_dAccTime(0.f)
	, m_iOldAniIndex(0)
	, m_dPeriod(0.0)
	, m_iAnimCountMax(0)
{
	m_pAniCtrl->AddRef();
	m_iAnimCountMax = m_pAniCtrl->GetMaxNumAnimationSets();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_dAccTime(rhs.m_dAccTime)
	, m_iOldAniIndex(rhs.m_iOldAniIndex)
	, m_dPeriod(0.0)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),// 복제 시 원본객체가 가진 최대 애니메이션의 개수
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),	// 복제 시 구동 가능한 애니메이션 셋의 최대 개수(대개 첫 번째 인자값과 동일)
		rhs.m_pAniCtrl->GetMaxNumTracks(),	// 제공되어지는 최대 트랙의 개수(진짜 많이 사용해야 두 개)
		rhs.m_pAniCtrl->GetMaxNumEvents(),	// 애니메이션이 지닌 내부적인 이벤트 정보(우리는 사용하지 않음)
		&m_pAniCtrl);

	m_iAnimCountMax = rhs.m_pAniCtrl->GetMaxNumAnimationSets();
}

Engine::CAniCtrl::~CAniCtrl()
{

}

Engine::CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*		pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*		pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free()
{
}

HRESULT Engine::CAniCtrl::Ready_AniCtrl()
{
	return S_OK;
}

HRESULT CAniCtrl::Bind_Frames(D3DXFRAME_DERIVED * pRootFrame)
{
	if (FAILED(D3DXFrameRegisterNamedMatrices(pRootFrame, m_pAniCtrl)))
	{
		return E_FAIL;
	}
	return S_OK;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	//AdvanceTime 함수 호출 시, 내부적으로 알 수 없는 시간이 증가
	// 애니메이션 교체 시, 내부적으로 증가하는 시간을 reset해줘야 문제가 생기지 않는다

	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 2인자 : 애니메이션 동작 시 사운드나 이펙트에 대한 처리를 담당하는 객체 주소
	m_dAccTime += (_double)fTimeDelta;
}

void CAniCtrl::Set_AnimaRatioFor_Stop()
{
	if (m_iOldAniIndex >= m_iAnimCountMax)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);
	m_dStartPeriod = TrackInfo.Position;
	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniCtrl->GetAnimationSet(m_iOldAniIndex, &pAS);
	// m_pAniCtrl->GetAnimationSetByName(, &pAS);

	// 트랙위에 애니메이션 셋(모션)을 설정하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 현재 애니메이션 셋의 재생 완료 시간을 반환하는 함수
	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + (m_dPeriod - m_dStartPeriod));

	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 0.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);

	// 트랙의 활성화 유무를 결정하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);

	// advancedtime 함수 호출시 증가하던 시간 값을 초기화
	m_pAniCtrl->ResetTime();
	m_dAccTime = 0.f;

	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iCurrentTrack = m_iNewTrack;

	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iNewTrack, &TrackInfo);
	m_dStartPeriod = TrackInfo.Position;
	m_dOldAnimTime = (m_dPeriod - m_dStartPeriod);
}

void CAniCtrl::Set_AnimationIndex_Loop(const _uint & iIndex)
{
	if (m_iOldAniIndex == iIndex) 
	{
		return;
	}

	// 뉴 트랙은 새로 들어온 트랙이다.
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET pAS = nullptr;
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	// 트랙위에 애니메이션 셋(모션)을 설정하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 현재 애니메이션 셋의 재생 완료 시간을 반환하는 함수
	m_dPeriod = pAS->GetPeriod();

	// 트랙 재생 시 발생할 수 있는 각종 이벤트 정보를 해제하는 함수(트랙끼리 모션 보간을 할 때 실패의 가능성이 있어서 이벤트 정보를 강제로 해제시킴)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);
	
	_double dDuration = 0.03;

	// 이전 트랙을 비활성화
	m_pAniCtrl->SetTrackEnable(m_iCurrentTrack, FALSE);

	// 새 트랙을 활성화
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_fAniSpeed, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);

	// advancedtime 함수 호출시 증가하던 시간 값을 초기화
	m_pAniCtrl->ResetTime();
	m_dAccTime = 0.f;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	D3DXTRACK_DESC TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);
	m_dStartPeriod = TrackInfo.Position;

	m_iOldAniIndex = iIndex;
	m_iCurrentTrack = m_iNewTrack;

}

void CAniCtrl::Set_AnimaRatio(_float fRatio)
{
	Set_AnimaRatioFor_Stop();
	_double dTotalTime = (m_dPeriod - m_dStartPeriod);

	Play_Animation((_float)((fRatio * dTotalTime) / dTotalTime));
}

// 이게 현재 실행중인 애니메이션셋
Engine::_bool Engine::CAniCtrl::Is_AnimationsetFinish()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;


	//if (TrackInfo.Position >= m_dPeriod - 0.2)
	// if (TrackInfo.Position >= m_dPeriod - m_dOldAnimTime * 0.2)
	// return true;
	// 

	return false;
}

_uint CAniCtrl::Get_AnimCountMax()
{
	return m_iAnimCountMax;
}

_uint CAniCtrl::Get_OldAniIndex()
{
	return m_iOldAniIndex;
}

const _float CAniCtrl::Get_AnimRatio()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iNewTrack, &TrackInfo);

	return (_float)((TrackInfo.Position - m_dStartPeriod) / (m_dPeriod - m_dStartPeriod));
}

const _float CAniCtrl::Get_AnimMaxTime()
{
	return (_float)(m_dPeriod - m_dStartPeriod);
}

const _int CAniCtrl::Get_AnimCurrentFrame()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);
	_float fCur = (_float)(TrackInfo.Position - m_dStartPeriod);

	return (_int)(fCur / m_fFrame60);
}

const _int CAniCtrl::Get_AnimMaxFrame()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);
	_float fMax = (_float)(m_dPeriod - m_dStartPeriod);

	return (_int)(fMax / m_fFrame60);
}

_float CAniCtrl::Get_CurAniTime()
{
	return (_float)m_dAccTime;
}

_float CAniCtrl::Get_CurAniEndTime()
{
	return (_float)(m_dPeriod - m_dStartPeriod) / m_fAniSpeed;
}
