#pragma once

#ifndef __RUN_MAP_H__
#define __RUN_MAP_H__

#include "GameObject.h"
#include "SMesh.h"

BEGIN(Engine)

class CRun_Chesed; 
class CSkillObject; 
class CUI_Object; 

class ENGINE_DLL CRun_Map final : public CGameObject
{
private:
	explicit CRun_Map(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CRun_Map(const CRun_Map& rhs);
	virtual ~CRun_Map();

public:
	static	CRun_Map*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _bool bCutSceneMap = FALSE); 
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(_bool bCutSceneMap);
	virtual _int			Start_Object(const _float& fTimeDelta)	override;
	virtual	_int			Update_Object(const _float& fTimeDelta)	override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta)	override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void			Debug_This_In_Console() override;

	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

public:
	const LPD3DXMESH&		Get_Mesh() { return m_pSMeshCom->Get_Mesh(); } 

public:
	void					Set_GameStart() { m_bGameStart = TRUE; }
	void					Set_ObstacleTime() { m_bObstacleTime = TRUE; m_iObstacleSpawnCount = 0; }

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

private:
	void					Add_Component(); 
	void					Spawn_Chesed(const _float& fTimeDelta); 
	void					Spawn_Drone(const _float& fTimeDelta);
	void					Ready_DroneSpawn(_uint iDroneCount); 
	void					Appear_Obstacle(const _float& fTimeDelta); 
	void					Update_BossWarningEffect(const _float& fTimeDelta); 
	void					Update_CheckGameStart(const _float& fTimeDelta); 

private:
	_bool					m_bGameStart = FALSE; 
	_bool					m_bObstacleTime = FALSE; 
	_bool					m_bCutSceneView = FALSE; 

	_uint					m_iPhase = 0; 
	_uint					m_iDroneSpawnCount = 6; 
	_uint					m_iObstacleSpawnCount = 0; 
	_uint					m_iObstacleMaxSpawnCount = 9; 
	_uint					m_iCutSceneCount = 0; 

	_float					m_fObstacleAppearDealy = 6.f; 
	_float					m_fDroneOriginSpawnDelay = 3.f;
	_float					m_fDroneSpawnDelay = m_fDroneOriginSpawnDelay;
	_float					m_fLength = 10.f; 
	_float					m_fObstacleOriginDelay = 2.5f; 
	_float					m_fObstacleDelay = m_fObstacleOriginDelay; 
	_float					m_fChesedSpawnDelay = 7.5f;
	_float					m_fCutSceneDelay = 6.5f; 
	_float					m_fWarningEffectTime = 4.5f;
	_float					m_fCutSceneAngle = 0.f; 
	_float					m_fCutSceneLength = 800.f; 

	_vec3					m_vDir = VEC_ZERO; 

	CRun_Chesed*			m_pChesed = nullptr; 
	CSkillObject*			m_pSkillWarningEffect = nullptr; 
	//CUI_Object*				m_pFadeInOutUI = nullptr; 

	CSMesh*					m_pSMeshCom = nullptr; 
	CShader*				m_pShaderCom = nullptr;
};

END

#endif //__RUN_MAP_H__