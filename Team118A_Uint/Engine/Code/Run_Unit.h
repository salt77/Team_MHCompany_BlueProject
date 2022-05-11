#pragma once

#ifndef __RUN_UNIT_H__
#define __RUN_UNIT_H__

#include "GameObject.h"

BEGIN(Engine)

class CSphereCollider; 

class ENGINE_DLL CRun_Unit abstract : public CGameObject
{
protected:
	explicit CRun_Unit(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	explicit CRun_Unit(const CRun_Unit& rhs);
	virtual ~CRun_Unit();

public:
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
	const _vec3&			Get_vQuatAngle() { return m_vQuatAngle; }
	const _vec3&			Get_VecAxis(E_ROTATION eRotationType) { return m_vAxis[(_uint)eRotationType]; }
	const _quat&			Get_QuatRotation();
	const T_UnitInfo&		Get_UnitInfo() { return m_tUnitInfo; }

public:
	virtual void			Set_Damage(const _float& fDamage); 

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

protected:
	virtual void			Initialize_QuatRotation(); 
	void					Initialize_Trigger();
	void					Update_WorldMatrix();
	//void					Update_TimeScaleEvent(const _float& fTimeDelta); 
	void					ReSetting_Animation();
	void					ReSetting_ActionTrigger();
	void					Active_Trigger(T_AnimTrg& trg);
	void					Load_Animation();
	void					Update_Trigger(const _float& fTimeDelta); 
	void					Set_ActionByName(const wstring& wstrAction);
	void					Set_ActionByEnum(E_ACTION eAction);
	void					Set_AnimIndexForTrigger(_uint iIndex);
	virtual	void			Animation_Change();
	virtual void			Update_Collider(const _float& fTimeDelta); 
	void					Collider_SetMatrix(CSphereCollider* pCol, const char* szFindBoneName);
	void					Create_Collider(CSphereCollider** pCol, wstring wstrCompName, _float fRadius);
	void					Create_DamageFont(E_NumberFontType eFontType, _int iDamage, _bool bCritical);

	void					Add_Rotation_Quat(E_ROTATION eRotationType, const _float& fAngle);

protected:
	virtual void			Trigger_Attack_1() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack1"); }
	virtual void			Trigger_Attack_2() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack2"); }
	virtual void			Trigger_Attack_3() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Attack3"); }
	virtual void			Trigger_Dead() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Dead"); }
	virtual void			Trigger_Reload() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Reload"); }
	virtual void			Trigger_ExSkill() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 ExSkill"); }
	virtual void			Trigger_Buff_1() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Buff1"); }
	virtual void			Trigger_Buff_2() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 Buff2"); }
	virtual void			Trigger_1() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 1"); }
	virtual void			Trigger_2() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 2"); }
	virtual void			Trigger_3() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 3"); }
	virtual void			Trigger_4() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 4"); }
	virtual void			Trigger_5() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 5"); }
	virtual void			Trigger_6() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 6"); }
	virtual void			Trigger_7() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 7"); }
	virtual void			Trigger_8() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 8"); }
	virtual void			Trigger_9() { CUtility::Print_Wstring_DMode(L"실행 - 트리거 9"); }

protected:
	_float						m_fCountingTime = 0.f; 
	_float						m_fDeadShaderFilter = 0.f; 

	_vec3						m_vAxis[(_uint)E_ROTATION::ROT_END] = { VEC_ZERO, VEC_ZERO, VEC_ZERO };
	_vec3						m_vQuatAngle = VEC_ZERO;

	_matrix						m_matRot;
	_matrix						m_matWorld;

	_quat						m_quatRot = _vec4(0.f, 0.f, 0.f, 1.f);

	wstring						m_wstrUnitName = L"Empty";
	wstring						m_wstrDMeshProtoTag = L"Empty";
	wstring						m_wstrDMeshFullPath = L"Empty";
	char*						m_szBodyBoneName = "Bip001_Pelvis";

	E_ACTION					m_eAction = E_ACTION::ACTION_END;
	E_ACTION					m_ePreAction = E_ACTION::ACTION_END;

	T_UnitInfo					m_tUnitInfo; 

	CDMesh*						m_pDMeshCom = nullptr;
	CSphereCollider*			m_pColliderCom = nullptr;
	CShader*					m_pShaderCom = nullptr;

protected:
	vector<pair<E_Trigger, std::function<void()>>>	m_vecFunction_Trg;
	vector<list<T_AnimTrg>>							m_vecAnimTrg;
	vector<T_AnimActionSys>							m_vecAnimActions;
	queue<_int>										m_qNextAnimIdx;
	queue<T_AnimTrg>								m_qTrg;
};

END

#endif //__RUN_UNIT_H__