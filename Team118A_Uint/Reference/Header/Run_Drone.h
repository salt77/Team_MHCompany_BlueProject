#pragma once

#ifndef __RUN_DRONE_H__
#define __RUN_DRONE_H__

#include "Run_Unit.h"

BEGIN(Engine)

class CRun_Player; 
class CRun_Map; 

class ENGINE_DLL CRun_Drone final : public CRun_Unit
{
private:
	explicit CRun_Drone(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	explicit CRun_Drone(const CRun_Drone& rhs);
	virtual ~CRun_Drone();

public:
	static	CRun_Drone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								   const wstring& wstrUnitName, 
								   const _float& fPipeAngle, 
								   const E_DRONE_ATKTYPE& eAtkType, 
								   const _uint&	iSpawnCount,
								   E_OBJTAG eObjTag = E_OBJTAG::OBJ_Run_Monster); 
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const _float& fPipeAngle, const E_DRONE_ATKTYPE& eAtkType, const _uint& iSpawnCount);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta)	override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta)	override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void			Debug_This_In_Console() override;

	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

public:
	const _float&			Get_DistanceToPipeMid() { return m_fDistanceToPipeMid; }
	const _vec3&			Get_VecAxis(E_ROTATION eRotationType) { return m_vAxis[(_uint)eRotationType]; }

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

private:
	virtual void			Update_Collider(const _float& fTimeDelta) override; 
	void					Add_Component(); 
	void					Ready_UnitInfo(); 
	void					Move_InPipe(const _float& fTimeDelta);
	void					Movement(const _float& fTimeDelta); 
	void					Interpolate_Rotation(); 
	void					Create_DroneBullet(const _float& fTimeDelta); 
	virtual void			Animation_Change() override; 

	// FSM 
private:
	void					FSM_FunctionSetting(void(CRun_Drone::*pCol)(const _float&), void(CRun_Drone::*pDestFunc)(const _float&));
	void					FSM_FunctionRelease();
	virtual	void			Update_FSM(const _float& fTimeDelta);
	void					MoveEnd(const _float& fTimeDelta); 
	void					Idle(const _float& fTimeDelta);
	void					Reload(const _float& fTimeDelta); 
	void					Dead(const _float& fTimeDelta); 

	virtual void			Trigger_1();
	virtual void			Trigger_2();

private:
	_bool					m_bUpdateFSM = FALSE; 
	_bool					m_bStateLock = FALSE;
	_bool					m_bFireTrigger = FALSE; 

	_uint					m_iRotateType = 0; 
	_uint					m_iChangePosCount = 0; 

	_float					m_fFireOriginDelay = 0.2f; 
	_float					m_fFireDelay = m_fFireOriginDelay;
	_float					m_fSpeed = 0.f; 
	_float					m_fSpeedInterpol = 3.f;					// 캐릭터 스피드 0.25배 기준 
	_float					m_fDistanceToPipeMid = 0.f; 
	_float					m_fRunAngle = 0.f; 
	_float					m_fPlayerRunAngle = 0.f; 
	_float					m_fDecRunAngle = 0.f; 
	_float					m_fFrontSpawnAngle = 120.f;				// 맵 스케일 5배 기준 Start_에서 다시 정의 
	_float					m_fPipeAngle = 0.f; 
	_float					m_fRotatePipeAngle = 5.f;
	_float					m_fNextRotateTargetAngle = 0.f; 
	_float					m_fDistancePlayerDeadline = 250.f;		// 맵 스케일 5배 기준 Start_에서 다시 정의 
	_float					m_fDistanceFromPlayer = 999.f; 
	_float					m_fDistanceFromPipeMid = 0.f; 
	_float					m_fMovementOriginDelay = 5.5f;
	_float					m_fMovementDelay = m_fMovementOriginDelay;

	_vec3					m_vTargetPos = VEC_ZERO; 
	_vec3					m_vPipeMidPos = VEC_ZERO; 

	E_DRONE_ATKTYPE			m_eDroneType = E_DRONE_ATKTYPE::DRONE_END; 

	CRun_Player*			m_pPlayer = nullptr; 
	CRun_Map*				m_pRunMap = nullptr; 

private:
	void(CRun_Drone::*m_fpLockFunction)(const _float&) = nullptr;
	void(CRun_Drone::*m_fpDestFunction)(const _float&) = nullptr;
};

END

#endif //__RUN_DRONE_H__