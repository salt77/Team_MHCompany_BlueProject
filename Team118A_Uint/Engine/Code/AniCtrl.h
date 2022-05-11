#pragma once
#ifndef __ANICTRL_H__
#define __ANICTRL_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

// ���� ������ ��� : �����

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
	void			Set_AnimaRatioFor_Stop();							// MFC ������ �����Ӵ����� �ִϸ��̼��� �������� �Լ� ���� ���ֿ����� ���� �ʴ´�.
	void			Set_AnimationIndex_Loop(const _uint& iIndex);		// ���� ������ Animation�� Set���ֱ� ���� �Լ�.
	void			Set_AnimaRatio(_float fRatio);

public:
	_bool			Is_AnimationsetFinish();
	_uint			Get_AnimCountMax();
	_uint			Get_OldAniIndex();
	const _float	Get_AnimRatio();								// ���� �ִϸ��̼� ���� ����	(1���� 0.5�� �������̶�� 0.5f ��ȯ)
	const _float	Get_AnimMaxTime();								// ���� �ִϸ��̼� �����ð�	(���� �ð����� 1�� ¥�� �ִϸ��̼��̶�� 1�� ��ȯ)
	const _int		Get_AnimCurrentFrame();							// ���� �ִϸ��̼� Current	������
	const _int		Get_AnimMaxFrame();								// ���� �ִϸ��̼� Max		������

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
