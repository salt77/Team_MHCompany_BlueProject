#pragma once
#ifndef __BINAH_H__
#define __BINAH_H__

#include "BossMonster.h"
#include "ParticleObject.h"
#include "Warning.h"
#include "Binah_Tail.h"
#include "Hod.h"

BEGIN(Engine)

typedef struct tagGlowAnimationForBinah
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

}T_GlowAnim_Binah;



class ENGINE_DLL CBinah final : public CBossMonster
{
private:
	enum class E_Binah_Anim
	{
		Move_End, Normal_Attack_Start, Vital_Death, Exs_03_Start, Noraml_Attack_Ing, Exs_01, 
		Idle, Move_Start, Vital_Groggy, Normal_Attack, Exs_03_End, Exs_03_Start_1,
		Exs_02, Normal_Attack_Ing_1, Vital_Groggy_Death, Exs_02_1, Noraml_Attack_Start_1,
		Idel_1, Intro_V2, Exs_03_Start_Ing_End, Noraml_Attack_End, Exs_03_End_1, 
		Normal_Attack_End_1, Intro02, Exs_03_Vital_Death, Exs_01_1, Anim_End
	};

	enum class E_Random_Guide
	{
		Random_Guide_Bomb, Random_Guide_Circle,
		Random_Guide_END
	};

protected:
	explicit				CBinah(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit				CBinah(const CBinah& rhs);
	virtual					~CBinah();

public:
	static CBinah*			Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void			Boss_Attack_1(_float fDeltaTime);
	virtual void			Boss_Attack_2(_float fDeltaTime);
	virtual void			Boss_Attack_3(_float fDeltaTime);
	virtual void			Boss_Groggy(_float fDeltaTime);
	virtual void			Animation_Change(const _float& fTimeDelta);

public:
	virtual void			OnEnable() override;
	virtual void			OnDisable() override;
	virtual void			OnDestroy() override;

public:
	void					Fit_CameraInput();
	void					Pattern_Test();
	void					Pattern_Update(const _float& fTimeDelta);
	void					Laser_Update(const _float& fTimeDelta);
	void					EXLaser_Update(const _float& fTimeDelta);
	void					Halo_Update(const _float& fTimeDelta);
	void					Collision_Laser(CGameObject * pPlayer);

public:
	void					Misile_Pose1();
	void					Misile_Pose2();
	void					Misile_Pose3();
	void					Misile_Pose4();
	void					Misile_Pose5();
	void					Misile_Pose6();

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
	void					Animation_Event();
	void					Normal_Attack_Pattern(_int iRand, _bool bStart);
	void					Laser_Pattern(_int iRand, _bool bStart);
	void					Laser_Skill();
	void					Misile_Pattern(_int iRand, _bool bStart);
	void					AreaLaser_Pattern(_int iRand, _bool bStart);
	void					Random_Guide(_bool bRand);
	void					Skill_Shake_Time(const _float& fTimeDelta);

protected:
	virtual void			Update_Collider(const _float& fTimeDelta);

private:
	static void				Collision_Player(CGameObject* pThis, CGameObject* pPlayer);

private:
	_float					m_fGage = 0.f;
	_int					m_iPatternRand = 0;
	_bool					m_bExSkill = false;
	E_Random_Guide			m_eRandomGuide = E_Random_Guide::Random_Guide_Bomb;

private:
	CSkillObject*			m_pLaser = nullptr;
	CSkillObject*			m_pEXLaser = nullptr;
	CSkillObject*			m_pHalo = nullptr;
	CSkillObject*			m_pQuake = nullptr;

	_float					m_fEXLaserTime = 0.f;
	_float					m_fQuakeTime = 0.f;
	_bool					m_bCreateQuake = true;
	_bool					m_bLaserSound = false;
	_bool					m_bGroggySound = false;
	_bool					m_bDeadSound = true;
	_float 					m_fIntroNameTime = 0.f;
	_bool					m_bIntroNameSound = true;
	_vec3					m_vecCharPos;

private:
	_float					m_fExBombTime = 0.f;
	_bool					m_bExBombSound = false;
	_float					m_fNormalBombTime = 0.f;
	_bool					m_bNormalBombSound = false;

private:
	CSkillObject*			m_pEye_L1 = nullptr;
	CSkillObject*			m_pEye_L2 = nullptr;

private:
	CSphereCollider*		m_pBoneEye_L1		= nullptr;
	CSphereCollider*		m_pBoneEye_L2		= nullptr;
	CSphereCollider*		m_pBoneEye_R1		= nullptr;
	CSphereCollider*		m_pBoneEye_R2		= nullptr;
	const char*				m_szEye_L1			= "Bone_Eye_01";
	const char*				m_szEye_L2			= "Bone_Eye_02";
	const char*				m_szEye_R1			= "Bone_Eye_03";
	const char*				m_szEye_R2			= "Bone_Eye_04";

	CSphereCollider*		m_pBone_Canon_01	= nullptr;
	CSphereCollider*		m_pBone_Canon_02	= nullptr;
	CSphereCollider*		m_pBone_Canon_03	= nullptr;
	CSphereCollider*		m_pBone_Canon_04	= nullptr;
	CSphereCollider*		m_pBone_Canon_05	= nullptr;
	CSphereCollider*		m_pBone_Canon_06	= nullptr;
	const char*				m_szCanon_01		= "Bone_Body_canon_op_01";
	const char*				m_szCanon_02		= "Bone_Body_canon_op_02";
	const char*				m_szCanon_03		= "Bone_Body_canon_op_03";
	const char*				m_szCanon_04		= "Bone_Body_canon_op_04";
	const char*				m_szCanon_05		= "Bone_Body_canon_op_05";
	const char*				m_szCanon_06		= "Bone_Body_canon_op_06";

	CSphereCollider*		m_pHead = nullptr;
	const char*				m_szHead = "Head_cap_01";

	CSphereCollider*		m_pBone_Root		= nullptr;
	const char*				m_szBone_Root		= "bone_root";

private:
	_bool					m_bGlowAnim = false;
	T_GlowAnim_Binah*		m_pGlowAnim = nullptr;
	T_GlowAnim_Binah		m_tGlowAnim_Arm;
	T_GlowAnim_Binah		m_tGlowAnim_FullBody;
	T_GlowAnim_Binah		m_tGlowAnim_ToNormal;

private:
	_float					m_fActiveCharTime = 0.f;
	_bool					m_bCreateHalo = true;
	_bool					m_bCreateEnvironment = true;
	_bool					m_bShowCombatUI = true;

private:
	vector<CGameObject*>	m_vecColObject;
	_float					m_fExDistance = 0.f;

private:
	CGameObject*			m_pGameObj_Halo = nullptr;

private:
	_bool					m_bCreateHod = true;

private:
	_bool					m_bIntro_Ani = true;
	_float					m_fIntroTime = 0.f;
};
END

#endif //__Binah_H__