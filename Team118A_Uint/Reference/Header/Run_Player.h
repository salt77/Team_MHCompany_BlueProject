#pragma once

#ifndef __RUN_PLAYER_H__
#define __RUN_PLAYER_H__

#include "Run_Unit.h"

BEGIN(Engine)

class CRun_Map; 
class CEffectObject; 
class CParticleObject; 
class CSkillObject; 
class CUI_Object; 

class ENGINE_DLL CRun_Player final : public CRun_Unit
{
protected:
	explicit CRun_Player(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	explicit CRun_Player(const CRun_Player& rhs);
	virtual ~CRun_Player();

public:
	static	CRun_Player*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object();
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta)	override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta)	override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void			Debug_This_In_Console() override;

	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

public:
	_float					Get_DistanceOriginToPipeMid() { return m_fDistanceFromMap + m_fDistancePipe; }
	const _float&			Get_DistanceFromMap() { return m_fDistanceFromMap; }
	const _float&			Get_DistancePipeRadius() { return m_fDistancePipe; }
	const _float&			Get_Speed() { return m_fSpeed; }
	const _float&			Get_OriginSpeed() { return m_fOriginSpeed; }
	const _float&			Get_RunAngle() { return m_fRunAngle; }
	const _float&			Get_PipeAngle() { return m_fPipeAngle; }
	CSphereCollider*		Get_ColliderCom() { return m_pColliderCom; }

public:
	virtual void			Set_Damage(const _float& fDamage) override;
	void					Set_SideRotateSpeed(const _float& fRatio, const _float& fTime);
	void					Add_SkillGage(const _float& fGage) { m_fSkillGage += fGage; }
	void					Set_StartGame_Player() { m_bStart = TRUE; }
	void					Set_EndGame_Player(const _float& fEndDelay); 
	void					Set_GameRender() { m_bGameRender = TRUE; }

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

private:
	void					Add_Component();
	virtual void			Key_Input(const _float& fTimeDelta);
	void					Lining_Run(const _float& fTimeDelta);
	void					Calculate_Dis(CGameObject* pTargetObject, _vec3 vRayPos, _vec3* vDir, _float* fDistance);

private:
	void					Update_FSM(const _float& fTimeDelta);
	void					Update_SkillGage(const _float& fTimeDelta);
	void					Update_ExSkill(const _float& fTimeDelta); 
	virtual void			Update_Collider(const _float& fTimeDelta) override;
	void					Update_Particle();
	void					Update_CheckSpeedChange(const _float& fTimeDelta);
	void					Update_GodModeTime(const _float& fTimeDelta);
	void					Update_RecordAxis(const _float& fTimeDelta); 
	void					Update_EndGame(const _float& fTimeDelta); 

private:
	_bool					m_bStart = FALSE; 
	_bool					m_bGameRender = FALSE; 
	_bool					m_bEndVictory = FALSE; 
	_bool					m_bUpDown = FALSE;
	_bool					m_bStateLock = FALSE;
	_bool					m_bGodMode = FALSE;
	_bool					m_bEx2Fire = FALSE; 
	_bool					m_bEndUI = FALSE; 

	_uint					m_iEx2FireCount = 0; 
	_int					m_iAttackTarget = -1; 

	_float					m_fGodModeInterpol = 1.f;
	_float					m_fFireDelay = 0.165f;
	_float					m_fDistanceRayToPlayer = 0.f;
	_float					m_fDistanceFromMap = 0.f;
	_float					m_fDistancePipe = 0.f;
	_float					m_fOriginSpeed = 0.85f;
	_float					m_fSpeed = m_fOriginSpeed;
	//_float					m_fSpeedBoost = 2.5f;
	_float					m_fRotatePipeOriginSpeed = 1.f;
	_float					m_fRotatePipeSpeed = m_fRotatePipeOriginSpeed;
	_float					m_fSpeedDownTime = 0.f;
	_float					m_fRunAngle = 0.f; 
	_float					m_fPipeAngle = 0.f;
	_float					m_fGodModeTime = 0.f;
	_float					m_fSkillGage = 0.f;
	_float					m_fSkillChargeDelay = 0.f;
	_float					m_fEx1AttackDelay = 0.35f; 
	_float					m_fEx1OriginDelay = 1.f;
	_float					m_fEx1Delay = m_fEx1OriginDelay;
	_float					m_fEx2FireOriginDelay = 0.2f; 
	_float					m_fEx2FireDelay = 0.f; 
	_float					m_fEndGameDelay = 3.f; 

	_vec3					m_vPipeMidPos = VEC_ZERO;
	_vec3					m_vDirToPipeMidAhead = VEC_ZERO;
	_vec3					m_vTargetDir = VEC_UP;
	_vec3					m_vWeaponPos = VEC_ZERO; 
	_vec3					m_vRecordPreAxisX = VEC_RIGHT; 

	//vector<CParticleObject*>m_vecParticle_FireMuzzle;
	CSkillObject*			m_pSkillNormalAttack = nullptr; 
	CSkillObject*			m_pSkillChargeEffect = nullptr;
	CSkillObject*			m_pSkillEx_2[5] = { nullptr, nullptr, nullptr, nullptr, nullptr};
	CParticleObject*		m_pParticleSkillEx1 = nullptr; 
	CParticleObject*		m_pParticleRunSpark = nullptr; 

	CRun_Map*				m_pRunMap = nullptr; 
};

END

#endif //__RUN_PLAYER_H__