#pragma once

#ifndef __RUN_CHESED_H__
#define __RUN_CHESED_H__

#include "Run_Unit.h"
#include "Boss_HpBar.h"
#include "Chesed_Web_Laser.h"

BEGIN(Engine)

class CRun_Player;
class CRun_Map;
class CSkillGuideObject;
class CParticleObject; 

class ENGINE_DLL CRun_Chesed final : public CRun_Unit
{
public:
	enum E_Chesed_Pattern
	{
		Pattern_Normal, Pattern_Spawn, Pattern_Retreat,
		Pattern_DroneSpawn, Pattern_Groggy,
		Pattern_Attack, Pattern_AttackSpree, Pattern_AttackFourthDir, Pattern_AttackRounded, Pattern_DirectLaser, Pattern_AttackCircle, Pattern_AttackWebLaser,
		Pattern_End
	};

private:
	explicit CRun_Chesed(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	explicit CRun_Chesed(const CRun_Chesed& rhs);
	virtual ~CRun_Chesed();

public:
	static	CRun_Chesed*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag = E_OBJTAG::OBJ_Run_BossMonster, _bool bCutSceneView = FALSE);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(_bool bCutSceneView);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta)	override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta)	override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void			Debug_This_In_Console() override;

	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

public:
	const _float&			Get_RunAngle() { return m_fRunAngle; }
	const _float&			Get_DistanceToPipeMid() { return m_fDistanceToPipeMid; }
	const _float&			Get_Speed() { return m_fSpeed; }
	const _vec3&			Get_VecAxis(E_ROTATION eRotationType) { return m_vAxis[(_uint)eRotationType]; }
	const _vec3&			Get_WeaponPos() { return m_vWeaponPos; }
	const _bool&			Get_CutSceneView() { return m_bCutSceneView; } 
	const _bool&			Get_Spawn() { return m_bSpawn; }
	const _vec3&			Get_LookDir() { return m_vLookDir; }

public:
	void					Set_PatternChange(const E_Chesed_Pattern& ePattern) { m_ePatternType = ePattern; }
	virtual void			Set_Damage(const _float& fDamage) override;
	void					Set_GroggyPercent(const _float& fPercentigy);
	void					Add_GroggyGage(const _float& fDamage);
	void					Set_ClearBossHpBar(); 

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

private:
	virtual	void			Animation_Change() override;
	void					Pattern_Change();
	virtual void			Update_Collider(const _float& fTimeDelta) override; 
	void					Add_Component();
	void					Ready_UnitInfo();
	void					Update_RotateInfo();
	void					Move_InPipe();
	void					LookAt_Player();
	void					Update_Particle(); 

	// FSM 
private:
	void					FSM_FunctionSetting(void(CRun_Chesed::*pCol)(const _float&), void(CRun_Chesed::*pDestFunc)(const _float&));
	void					FSM_FunctionRelease();
	virtual	void			Update_FSM(const _float& fTimeDelta);
	void					Begin_Battle(const _float& fTimeDelta); 
	void					Idle(const _float& fTimeDelta);
	void					Attack(const _float& fTimeDelta);
	void					Attack_WebLaser(const _float& fTimeDelta);
	void					Groggy_SpawnDrone(const _float& fTimeDelta);
	void					Dead(const _float& fTimeDelta);

private:
	void					Create_ChesedBullet(const E_CHESED_ATKTYPE& eAtkType,
												const E_TANMAKTYPE& eTanmakType = E_TANMAKTYPE::TANMAK_NORMAL,
												const _float& fPipeAngle = 0.f,
												const _float& fPipeAngleRand = 0.f);
	void					Create_ChesedCircleBullet(const _uint& iBulletCount);
	void					Create_ChesedDirectLaser(); 
	void					Create_ChesedWebLaser(const _uint& iLaserCount, const Chesed_Web_Laser::E_WebLaser_AtkType& eLaserType);
	void					Spawn_Drone();
	void					Decide_Pattern(); 

private:
	virtual void			Trigger_Attack_1() override;
	virtual void			Trigger_Attack_2() override;
	virtual void			Trigger_1() override; 
	virtual void			Trigger_2() override;
	virtual void			Trigger_3() override;
	virtual void			Trigger_4() override; 
	virtual void			Trigger_5() override; 

private:
	_bool					m_bStateLock = FALSE;
	_bool					m_bAttack = FALSE; 
	_bool					m_bLaserAttack = FALSE; 
	_bool					m_bCutSceneView = FALSE; 
	_bool					m_bSpawn = FALSE; 
	_bool					m_bEndGame = FALSE; 
	_bool					m_bSpawnDroneOnce = FALSE; 
	_bool					m_bJustOnceCheck = FALSE; 

	_uint					m_iPhase = 0;
	_uint					m_iLaserPhase = 0; 
	_uint					m_iCurrentMaxSkillUse = (E_Chesed_Pattern)Pattern_AttackSpree;

	_float					m_fAttackStartDelay = 1.75f; 
	_float					m_fAttackEndDelay = 17.f; 
	_float					m_fAttackCountingTime = 0.f; 
	_float					m_fSpeed = 0.f;
	_float					m_fRotatingSpeed = 0.1f;
	_float					m_fRunAngle = 0.f;
	_float					m_fAngleFront = 30.f;
	_float					m_fSpawnFrontAngle = 90.f;
	_float					m_fDistanceToPipeMid = 0.f;
	_float					m_fBulletDelay = 0.25f;
	_float					m_fDistancePlayerDeadLine = 0.f;
	_float					m_fDistanceFromPlayer = 999.f;
	_float					m_fDecRunAngle = 0.f;
	_float					m_fDeadMoveDelay = 1.f; 
	_float					m_fEndGameDelay = 2.f; 
	_float					m_fDeadBombDelay = 0.35f; 
	_float					m_fDroneSpawnDelay = 30.f; 

	// BossHpBar
	_float					m_fGroggy_Gage = 0.f;
	_float					m_fGroggy_MaxGage = 0.f;
	_float					m_fGroggy_Time = 0.f;
	_float					m_fGroggy_MaxTime = 5.f;

	_float					m_fIdleWaitingTime = 0.f;
	_float					m_fIdleWaitingMinTime = 1.5f;

	// General 
	_float					m_fSkillAttackOriginDelay = 1.5f;
	_float					m_fSkillAttackDelay = m_fSkillAttackOriginDelay;

	// Normal 
	_float					m_fNormalBulletDelay = 0.08f;

	// Rounded
	_float					m_fRoundedBulletDelay = 0.05f;
	_float					m_fRoundedFireAngle = 0.f;
	_float					m_fRoundedFireAngleChange = 20.f;

	// Circle 
	_float					m_fCircleBulletDelay = 1.5f;

	// Spree 
	_float					m_fSpreeBulletDelay = 0.08f;

	// Fourth Spree 
	_float					m_fFourthSpreeDelay = 0.16f; 

	// Direct Laser 
	_float					m_fDirectLaserDelay = 2.5f;
	_float					m_fDirectLaserTimeCount = m_fDirectLaserDelay;

	// Web Laser 
	_float					m_fWebLaserDelay = 2.5f;
	_float					m_fWebLaserTimeCount = m_fWebLaserDelay; 

	_vec3					m_vOriginAxisY = VEC_UP; 
	_vec3					m_vWeaponPos = VEC_ZERO; 
	_vec3					m_vLookDir = VEC_LOOK; 

	E_Chesed_Pattern		m_ePatternType = Pattern_End;
	E_Chesed_Pattern		m_ePrePatternType = Pattern_End;
	E_Chesed_Pattern		m_eMinPatternType = Pattern_End; 
	E_Chesed_Pattern		m_eMaxPatternType = Pattern_End; 

	CRun_Player*			m_pPlayer = nullptr;
	CRun_Map*				m_pRunMap = nullptr;
	CBoss_HpBar*			m_pBossHpBar = nullptr;

	CParticleObject*		m_pParticleChargeLaser = nullptr; 

	CSphereCollider*		m_pMainWeaponCol = nullptr; 

private:
	void(CRun_Chesed::*m_fpLockFunction)(const _float&) = nullptr;
	void(CRun_Chesed::*m_fpDestFunction)(const _float&) = nullptr;
};

END

#endif //__RUN_CHESED_H__