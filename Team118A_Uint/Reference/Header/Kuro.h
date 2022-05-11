#pragma once
#ifndef __KURO_H__
#define __KURO_H__

#include "BossMonster.h"

class ENGINE_DLL CKuro final : public CBossMonster
{
protected:
	explicit CKuro(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CKuro(const CKuro& rhs);
	virtual ~CKuro();
public:
	static CKuro*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void	Free() override;

public:
	virtual	HRESULT	Ready_Object() override;
	virtual _int	Start_Object(const _float& fTimeDelta) override;
	virtual	_int	Update_Object(const _float& fTimeDelta) override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void	Render_Object(_int iPass) override;

	virtual void	Set_Constantable() override;
	virtual void	Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);

	virtual void	FSM_FunctionSetting(void(CUnit::*pCol)(_float), void(CUnit::*pDestFunc)(_float))override;


	virtual void	Boss_Groggy(_float fDeltaTime)override;
	virtual void	Idle(_float fDeltaTime)override;
	 
	//일반 공격 패턴
	virtual void	Shoot(_float fDeltaTime)override;

	//EX1 : 아무위치에서나 나가는 패턴
	virtual void    Boss_Attack_1(_float fDeltaTime)override;

	//EX2 : 회전목마, 피자 등등 중앙에서 나가는 패턴
	virtual void    Boss_Attack_2(_float fDeltaTime)override;

	//지형 파괴
	virtual void	Boss_Attack_3(_float fDeltaTime);

	//텔레포트
	virtual void	Boss_Attack_4(_float fDeltaTime)override;

	//맵 변경
	virtual void	ExSkill(_float fDeltaTime)override;

	//virtual void		Tracking(_float fDeltaTime);
	//virtual void		Idle(_float fDeltaTime);
	//virtual void		Ready_Shoot_Start(_float fDeltaTime);
	//virtual void		Shoot_Start(_float fDeltaTime);
	//virtual void		Shoot(_float fDeltaTime);
	//virtual void		Shoot_End(_float fDeltaTime);
	//virtual void		Ready_End(_float fDeltaTime);
	//virtual void		Move_End_Stance(_float fDeltaTime);
	//virtual void		Reload(_float fDeltaTime);
	//virtual void		Hiding(_float fDeltaTime);
	//virtual void		Dead(_float fDeltaTime);
	//virtual void		Victory(_float fDeltaTime);
	//virtual void		Begin_Battle(_float fDeltaTime);
	//virtual void		ExSkill(_float fDeltaTime);
	//virtual void		ExSkill_CurIn(_float fDeltaTime);
	//virtual void		Assemble(_float fDeltaTime);
	//virtual void		Dissipation(_float fDeltaTime);

	//// 보스용  FSM 재정의 하지 않으면 Idle로 강제 이동된다.
	//virtual void		Boss_Attack_1(_float fDeltaTime);
	//virtual void		Boss_Attack_2(_float fDeltaTime);
	//virtual void		Boss_Attack_3(_float fDeltaTime);
	//virtual void		Boss_Attack_4(_float fDeltaTime);
	//virtual void		Boss_Groggy(_float fDeltaTime);

public:
	virtual void	OnEnable() override;
	virtual void	OnDisable() override;
	virtual void	OnDestroy() override;

	_bool			Update_KuroCam(_float fTimeDelta);

	_float			Get_DamageRatio() { return m_fDamageRatio; }
protected:
	virtual void	Trigger_Attack_1()override;
	virtual void	Trigger_Attack_2()override;
	virtual void	Trigger_Reload()override;
	virtual void	Trigger_Buff_1()override;
	virtual void    Trigger_Buff_2()override;
	virtual void	Trigger_1()override;
	virtual void	Trigger_2()override;
	virtual void	Trigger_3()override; 
	virtual void	Trigger_4()override;

private:
	void			Phase1_EX();
	void			Phase2_EX();
	void			Phase3_EX();
	void			Phase1_Center_EX();
	void			Phase2_Center_EX();
	void			Phase3_Center_EX();



	void			Normal_AttackPattern();
	void			Normal_Pattern_Phase1();
	void			Normal_Pattern_Phase2();
	void			Normal_Pattern_Phase3();

	void			AttackGround(_int iIndex);
private:
	void			InvokeAttackGround(_int iIndex);
	void			Invoke_AttackFan();
	void			Invoke_AttackStraight();
	void			Invoke_AttackDonut();
	void			Invoke_AttackOutDonut();
	void			Invoke_ExplosionCircle(_int iIndex);
	void			Invoke_ExplosionCircleGuide(_int iIndex);
	void			Invoke_FireShoot();
	void			Invoke_FireShootCol();

	void			Invoke_FadeIn();
private: // EX 자유 위치 공격 패턴
	//1페
	void			TripleExplosion();

	//2페  
	void			BumpCar();
	void			DoubleDonut();

	//3페
	void			SpinCup();


private: // EX 중심 위치 공격 패턴
	//2페
	void			SpinLaser();
	void			Pizza();

	//3페
	void			FireShoot();
	void			SpinHorse();
	void			DoubleBumpCar();

private: // 일반 공격 패턴 
	void			Normal_AttackFan();
	void			Normal_AttackStraight();
	void			Normal_AttackDonut();
	void			Normal_AttackOutDonut();

private: //체력 이벤트
	_bool			HPEventCheck();

	void			ChangePhase2();
	void			GroundBreak();
	
private:
	_vec3			Get_RandomWarpPos();
	void			SpinLaserUpdate(_float fTimeDelta);

	void			Set_Shake(_float fMinShake, _float fMaxShake, _float fShakeDelay);
	void			Update_Shake(_float fTiemDelta);
	void			Update_Fade(_float fTimeDelta);
	void			Set_Fade();
private:
	static void		ColPlayer(CGameObject* pThis, CGameObject* pPlayer);
	static void		ColRay(CGameObject* pThis, CGameObject* pPlayer);
private:
	_float			m_fDamageRatio = 1.f;

	_float			m_fFadeAlpha = 0.f;
	_bool			m_bFadeOut = FALSE;

	_float			m_fShakeTimer = 0.f;
	_float			m_fMinShake = 0.f;
	_float			m_fMaxShake = 0.f;
	_float			m_fShakeDelay = 0.f;
	_bool			m_bShake = FALSE;

	CSkillObject*	m_pAuraObject = nullptr;

	_vec3			m_vCenterPos = _vec3(0.f, 0.f, 0.f);

	_int			m_iEventCount = 0;

	_bool			m_bFirst = FALSE;
	_float			m_fTimer = 0.f;

	_vec3			m_vWarpPos = _vec3(0.f, 0.f, 0.f);
	_bool			m_bRender = TRUE;

	vector<CGameObject*> m_vecLeftGround;
	vector<CGameObject*> m_vecRightGround;

	_float			m_fGage = 0.f;

	_float			m_fShootDelay = 0.f;

	CParticleObject* m_pStaffChargeParticle = nullptr;

	CParticleObject* m_pWarpParticle = nullptr;

	vector<CParticleObject*> m_vecLaserParticle;
	_float			m_fLaserSpawnTime = 0.f;

	vector<_vec3>	m_vecExplosionPos;

	T_GUIDE_OPTION  m_tFireShootGuide;

	CUI_Object*		m_pFadeUI = nullptr;

	_vec3			m_vPrevCamPos = _vec3(0.f, 0.f, 0.f);
	_vec3			m_vPhase2CamPos = _vec3(-20.f, 16.f, 0.f);
	_vec3			m_vPhase3CamPos = _vec3(-16.f, 12.f, 0.f);
	_float			m_fRatio = 0.f;

	_int			m_iUseSkillIndex = 0;
	_int            m_iUseCenterSkillIndex = 0;
};

#endif //__KURO_H__