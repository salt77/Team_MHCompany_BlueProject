#pragma once
#ifndef __Hieronymus_H__
#define __Hieronymus_H__

#include "BossMonster.h"
#include "Hiero_G_Lantern.h"
#include "Hiero_R_Lantern.h"


typedef struct tagGlowAnimationForHieronymus
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
	_int iLoopTime = 1;
	_float fTotalTime = 3.f;
	_float fLeftTime = 0.f;

	_vec2 vBeginEnd_Power = _vec2(0.f, 1.f);
	_vec2 vStart_Min_UV = _vec2(0.f, 0.f);
	_vec2 vStart_Max_UV = _vec2(1.f, 1.f);
	_vec2 vEnd_Min_UV = _vec2(0.f, 0.f);
	_vec2 vEnd_Max_UV = _vec2(1.f, 1.f);
}T_GlowAnim_Hiero;

class ENGINE_DLL CHieronymus final : public CBossMonster
{
private:
	enum class E_Hieronymus_Anim
	{
		Exs03, Normal_Attack, Groggy, Exs02, Exs04,	Normal_Reload, Vital_Death, OP, Vital_Groggy, Idle, Exs01, PC
	};
	enum class E_RandWarn
	{
		Rand_Bomb, Straight_Circles, _3, Bezier_Targetting, Line3,
		RandWarn_END
	};

protected:
	explicit CHieronymus(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CHieronymus(const CHieronymus& rhs);
	virtual ~CHieronymus();
public:
	static CHieronymus*			Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void				Free() override;

public:
	virtual	HRESULT				Ready_Object() override;
	virtual _int				Start_Object(const _float& fTimeDelta) override;
	virtual	_int				Update_Object(const _float& fTimeDelta) override;
	virtual	_int				LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void				Render_Object(_int iPass) override;
	virtual void				Set_Constantable() override;
	virtual void				Animation_Change(const _float& fTimeDelta);

public:
	virtual void				Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void				Begin_Battle(_float fDeltaTime) override;
	virtual void				Idle(_float fDeltaTime);
	virtual void				Dead(_float fDeltaTime);
	virtual void				ExSkill(_float fDeltaTime);				// Exs04
	virtual void				Boss_Attack_1(_float fDeltaTime);		// Exs01
	virtual void				Boss_Attack_2(_float fDeltaTime);		// Exs02
	virtual void				Boss_Attack_3(_float fDeltaTime);		// Exs03
	virtual void				Boss_Groggy(_float fDeltaTime);

protected:
	virtual void				Update_Collider(const _float& fTimeDelta);

protected:
	void						Trigger_Attack_1()	override;			// Exs01 Right 
	void						Trigger_Attack_2()	override;			// Exs02 Left		
	void						Trigger_ExSkill()	override;			// Exs04

protected:
	void						AnimationStart_Event();
	void						Ex1_Pattern(_int iRand, _bool bStart);
	void						Ex2_Pattern(_int iRand, _bool bStart);
	void						Ex3_Pattern(_int iRand, _bool bStart);
	void						Ex4_Pattern(_int iRand, _bool bStart);

private:
	void						Test_Bullet_6();

private:
	void						Hieronymus_Explode_0();
	void						Hieronymus_Impact_0();
	void						Hieronymus_Impact_1();
	void						Hieronymus_Impact_2_Sound();
	void						Hieronymus_Staff_1();
	void						Hieronymus_Ex_Bomb();

private:
	void						Rand_Warn(_bool bRand);
	void						Lava_Invoke();
	void						Lightning_Invoke();
	void						Create_BezierEx(_int i);
	void						ViewCut_Update(const _float& fTimeDelta);
	void						Set_SkySphere(wstring wstrSkyTexFullPath);


protected:
	CHiero_G_Lantern*			m_pGreen_Lantern = nullptr;
	CHiero_R_Lantern*			m_pRed_Lantern = nullptr;

protected:
	// 히에로니무스 무기 뼈 3개 이름
	CSphereCollider*			m_pBoneWP_L					= nullptr;
	CSphereCollider*			m_pBone_Pendant				= nullptr;
	CSphereCollider*			m_pBoneWP_R					= nullptr;
	const char*					m_szWP_L					= "Bone_WP_L";
	const char*					m_szWP_Pendant				= "Bone_Pendant_04";
	const char*					m_szWP_R					= "Bone_WP_R";

	CSphereCollider*			m_pBone_L_Finger			= nullptr;
	CSphereCollider*			m_pBone_R_Finger			= nullptr;
	CSphereCollider*			m_pBone_Cape_Center_Top		= nullptr;
	CSphereCollider*			m_pBone_Root				= nullptr;
	const char*					m_szBone_L_Finger			= "Bip001_L_Finger2";
	const char*					m_szBone_R_Finger			= "Bip001_R_Finger2";
	const char*					m_szBone_Cape_Center_Top	= "Bone_cape_F_01";
	const char*					m_szBone_Root				= "bone_root";


	wstring						m_wstrAura_00				= L"Hieronymus_Aura_00.effect";
	wstring						m_wstrAura_01				= L"Hieronymus_Aura_01.effect";
	wstring						m_wstrAura_02				= L"Hieronymus_Aura_02.effect";

	_float						m_fGage						= 0.f;
	_int						m_iPatternRand				= 0;

	E_RandWarn					m_eRandWarn					= E_RandWarn::Rand_Bomb;
	_bool						m_bIsExSkill				= FALSE;

	_int						m_iWitchLantern				= 0;


	_bool						m_bGlowAnim					= FALSE;
	T_GlowAnim_Hiero*			m_pGlowAnim					= nullptr;
	T_GlowAnim_Hiero			m_tGlowAnim_Arm;
	T_GlowAnim_Hiero			m_tGlowAnim_FullBody;
	T_GlowAnim_Hiero			m_tGlowAnim_ToNormal;

	_bool						m_bPhaseMiddle				= FALSE;
	_bool						m_bExBloom					= FALSE;
	_float						m_fExBloom_Timer			= 0.f;
	_float						m_fPhaseMiddle_Timer		= 0.f;
	_float						m_fBegin_Middle				= 0.18f;
	_float						m_fEnd_Middle				= 0.06f;
	_float						m_fBegin_Bloom				= 1.f;
	_float						m_fEnd_Bloom				= 0.0f;

	// 갓레이
	T_GodLight					m_tGodRay;
	_int						m_iGodRayIndex				= 0;
	_float						m_fGodRaySize_Start			= 7.1f;
	_float						m_fGodRaySize_End			= 3.5f;
	_float						m_fGodRayTimer				= 0.f;

	// 컷뷰액션
	_float						m_fCutViewTotalTime			= 3.f;
	_float						m_fCutViewCoolTime			= 10.f;
	_int						m_iCutViewTime				= 13;
	_int						m_iCutViewStack				= 0;
	_float						m_fCutViewTilt				= 0.f;
	_bool						m_bCutViewOnOff				= FALSE;
	_vec3						m_vCutView_Start_Eye		= VEC_ZERO;
	_vec3						m_vCutView_Start_At			= VEC_ZERO;

	// 스카이스피어
	CGameObject*				m_pSkySphere				= nullptr;

};

#endif //__Hieronymus_H__