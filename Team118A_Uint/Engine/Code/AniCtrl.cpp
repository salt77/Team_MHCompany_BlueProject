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
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),// ���� �� ������ü�� ���� �ִ� �ִϸ��̼��� ����
		rhs.m_pAniCtrl->GetMaxNumAnimationSets(),	// ���� �� ���� ������ �ִϸ��̼� ���� �ִ� ����(�밳 ù ��° ���ڰ��� ����)
		rhs.m_pAniCtrl->GetMaxNumTracks(),	// �����Ǿ����� �ִ� Ʈ���� ����(��¥ ���� ����ؾ� �� ��)
		rhs.m_pAniCtrl->GetMaxNumEvents(),	// �ִϸ��̼��� ���� �������� �̺�Ʈ ����(�츮�� ������� ����)
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
	//AdvanceTime �Լ� ȣ�� ��, ���������� �� �� ���� �ð��� ����
	// �ִϸ��̼� ��ü ��, ���������� �����ϴ� �ð��� reset����� ������ ������ �ʴ´�

	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 2���� : �ִϸ��̼� ���� �� ���峪 ����Ʈ�� ���� ó���� ����ϴ� ��ü �ּ�
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

	// Ʈ������ �ִϸ��̼� ��(���)�� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ���� �ִϸ��̼� ���� ��� �Ϸ� �ð��� ��ȯ�ϴ� �Լ�
	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_dAccTime + (m_dPeriod - m_dStartPeriod));

	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 0.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);

	// Ʈ���� Ȱ��ȭ ������ �����ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f, m_dAccTime, (m_dPeriod - m_dStartPeriod), D3DXTRANSITION_LINEAR);

	// advancedtime �Լ� ȣ��� �����ϴ� �ð� ���� �ʱ�ȭ
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

	// �� Ʈ���� ���� ���� Ʈ���̴�.
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET pAS = nullptr;
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	// Ʈ������ �ִϸ��̼� ��(���)�� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ���� �ִϸ��̼� ���� ��� �Ϸ� �ð��� ��ȯ�ϴ� �Լ�
	m_dPeriod = pAS->GetPeriod();

	// Ʈ�� ��� �� �߻��� �� �ִ� ���� �̺�Ʈ ������ �����ϴ� �Լ�(Ʈ������ ��� ������ �� �� ������ ���ɼ��� �־ �̺�Ʈ ������ ������ ������Ŵ)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);
	
	_double dDuration = 0.03;

	// ���� Ʈ���� ��Ȱ��ȭ
	m_pAniCtrl->SetTrackEnable(m_iCurrentTrack, FALSE);

	// �� Ʈ���� Ȱ��ȭ
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_fAniSpeed, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 1.f, m_dAccTime, dDuration, D3DXTRANSITION_LINEAR);

	// advancedtime �Լ� ȣ��� �����ϴ� �ð� ���� �ʱ�ȭ
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

// �̰� ���� �������� �ִϸ��̼Ǽ�
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
