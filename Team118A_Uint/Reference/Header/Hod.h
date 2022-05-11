#pragma once
#ifndef __HOD_H__
#define __HOD_H__

#include "BossMonster.h"
#include "ParticleObject.h"
#include "Warning.h"

typedef struct tagGlowAnimationForHod
{
	_float Get_Ratio()
	{
		_float fRatio = fLeftTime / fTotalTime;

		if (eEase != E_EASE::None)
		{
			fRatio = CUtility::LerpEase_float(eEase, fRatio);
		}
		return fRatio;
	}

	_vec2 vGet_Min()
	{
		return _vec2(CUtility::Lerp_float(vStart_Min_UV.x, vEnd_Min_UV.x, Get_Ratio()), CUtility::Lerp_float(vStart_Min_UV.y, vEnd_Min_UV.y, Get_Ratio()));
	}

	_vec2 vGet_Max()
	{
		return _vec2(CUtility::Lerp_float(vStart_Max_UV.x, vEnd_Max_UV.x, Get_Ratio()), CUtility::Lerp_float(vStart_Max_UV.y, vEnd_Max_UV.y, Get_Ratio()));
	}

	_float fGet_Power()
	{
		return CUtility::Lerp_float(vBeginEnd_Power.x, vBeginEnd_Power.y, Get_Ratio());
	}

	E_GlowAnimMode eGlowAnimMode = E_GlowAnimMode::One;

	E_EASE eEase = E_EASE::None;
	_int iLoopTime = 10;
	_float fTotalTime = 3.f;
	_float fLeftTime = 0.f;
	_float fGlowBright = 0.f;
	_float fGlowBlue = 1.f;

	_vec2 vBeginEnd_Power = _vec2(0.f, 1.f);
	_vec2 vStart_Min_UV = _vec2(0.f, 0.f);
	_vec2 vStart_Max_UV = _vec2(1.f, 1.f);
	_vec2 vEnd_Min_UV = _vec2(0.f, 0.f);
	_vec2 vEnd_Max_UV = _vec2(1.f, 1.f);

}T_GlowAnim_Hod;

class ENGINE_DLL CHod final : public CBossMonster
{
private:
	enum class E_Hod_Anim
	{
		EXS1, EXS2, EXS3, EXS4, EXS5, 
		Normal_Attack_Ing, Normal_Attack_End,
		Normal_Attack_Start, Normal_Attack_Delay,
		BattleReady, Vital_Groggy, Vital_Death, Anim_End
	};

	enum class E_Random_Guide
	{
		Random_Guide_Bomb, Random_Guide_Circle,
		Random_Guide_END
	};

protected:
	explicit				CHod(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit				CHod(const CHod& rhs);
	virtual					~CHod();

public:
	static CHod*			Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;

public:
	virtual	HRESULT			Ready_Object() override;
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass) override;

	virtual void			Set_Constantable() override;
	virtual void			Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void			Begin_Battle(_float fDeltaTime) override;
	virtual void			Idle(_float fDeltaTime);
	virtual void			Dead(_float fDeltaTime);
	virtual void			ExSkill(_float fDeltaTime);	
	void					Boss_Attack_1(_float fDeltaTime);
	void					Boss_Attack_2(_float fDeltaTime);
	void					Boss_Attack_3(_float fDeltaTime);
	void					Boss_Attack_4(_float fDeltaTime);
	virtual void			Boss_Groggy(_float fDeltaTime);
	virtual void			Animation_Change(const _float& fTimeDelta);

public:
	virtual void			OnEnable() override;
	virtual void			OnDisable() override;
	virtual void			OnDestroy() override;

public:
	void					Fit_CameraInput();
	void					Pattern_Test();

public:
	void					Pattern_Update(const _float& fTimeDelta);
	void					Collision_Distance(const _float& fTimeDelta);
	void					Collision_Attack(CGameObject * pPlayer);

public:					
	void					AreaLaser_Update(const _float& fTimeDelta);
	void					AreaLaser_Skill();

public:
	void					Animation_Event();
	void					Normal_Attack_Pattern(_int iRand, _bool bStart);
	void					Charging_Pattern(_int iRand, _bool bStart);
	void					Wave_Pattern(_int iRand, _bool bStart);
	void					EX4_Function(const _float& fTimeDelta);
	void					Create_Tower_Pattern(_int iRand, _bool bStart);
	void					AreaLaser_Pattern(_int iRand, _bool bStart);

public:
	void					Random_Guide(_bool bRand);

public:
	void					Invoke_EXS1();
	void					Invoke_EXS2();
	void					Invoke_EXS4();
	

protected:
	virtual void			Trigger_1() override;
	virtual void			Trigger_2()	override;
	virtual void			Trigger_3()	override;
	virtual void			Trigger_4()	override;
	virtual void			Trigger_5()	override;
	virtual void			Trigger_6()	override;
	virtual void			Trigger_7()	override;

protected:
	void					Ready_Glow();
	void					Ready_HDR();
	void					Reset_HDR();
	void					Glow_Setting(const _float& fTimeDelta);
	void					EX_HDR(const _float& fTimeDelta);

protected:
	virtual void			Update_Collider(const _float& fTimeDelta);
	static void				Collision_Player(CGameObject* pThis, CGameObject* pPlayer);

private:
	_float					m_fGage = 0.f;
	_int					m_iPatternRand = 0;
	_bool					m_bExSkill = false;
	E_Random_Guide			m_eRandomGuide = E_Random_Guide::Random_Guide_Bomb;

private:
	CSkillObject*			m_pEXS1 = nullptr;
	CSkillObject*			m_pEXS2 = nullptr;
	CSkillObject*			m_pAreaLaser = nullptr;
	CSkillObject*			m_pQuake = nullptr;

private:
	_float					m_fEXLaserTime = 0.f;
	_float					m_fQuakeTime = 0.f;
	_bool					m_bCreateQuake = true;
	_bool					m_bLaserSound = false;
	_bool					m_bGroggySound = false;
	_bool					m_bDeadSound = true;
	_vec3					m_vecCharPos;

private:
	CSphereCollider*		m_pTentacle_R		= nullptr;
	CSphereCollider*		m_pTentacle_RB		= nullptr;
	CSphereCollider*		m_pTentacle_L		= nullptr;
	CSphereCollider*		m_pTentacle_LB		= nullptr;
	const char*				m_szTentacle_R		= "bone_Body_Tentacle_R_11";
	const char*				m_szTentacle_RB		= "bone_Body_Tentacle_RB_11";
	const char*				m_szTentacle_L		= "bone_Body_Tentacle_L_11";
	const char*				m_szTentacle_LB		= "bone_Body_Tentacle_LB_11";

	CSphereCollider*		m_pBone_LaserGun_L	= nullptr;
	CSphereCollider*		m_pBone_LaserGun_R	= nullptr;
	const char*				m_szLaserGun_L		= "bone_L_Lasergun";
	const char*				m_szLaserGun_R		= "bone_R_Lasergun";

	CSphereCollider*		m_pHead				= nullptr;
	CSphereCollider*		m_pBone_Body		= nullptr;
	const char*				m_szHead			= "bone_Head_01";
	const char*				m_szBone_Body		= "bone_body_02";

	CSphereCollider*		m_pTentacle_Attack_L = nullptr;
	const char*				m_szTentacle_Attack_L = "bone_Tentacle_L_15_01";

private:
	_bool					m_bGlowAnim = false;
	T_GlowAnim_Hod*			m_pGlowAnim = nullptr;
	T_GlowAnim_Hod			m_tGlowAnim_Arm;
	T_GlowAnim_Hod			m_tGlowAnim_FullBody;
	T_GlowAnim_Hod			m_tGlowAnim_ToNormal;

private:
	_float					m_fActiveCharTime = 0.f;
	_bool					m_bShowHP = true;
	_bool					m_bShowCombatUI = true;

private:
	vector<CGameObject*>	m_vecColObject;
	_bool					m_bColDistance = false;
	_float					m_fColDistance = 0.f;

private:
	_bool					m_bEXS2Distance = false;
	_float					m_fEXS2Distance = 0.f;
	_float					m_fEXS2ColTime = 0.f;

	_bool					m_bEX4Func = false;
	_float					m_bEX4FuncTime = 0.f;
	
private:
	_bool					m_bIntro_Ani = true;
	_float					m_fIntroTime = 0.f;
};

#endif //__HOD_H__