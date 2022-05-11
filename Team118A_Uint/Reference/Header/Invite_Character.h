#pragma once
#ifndef __INVITE_CHARACTER_H__
#define __INVITE_CHARACTER_H__

#include "GameObject.h"
#include "Obstacle_Cafe.h"
#include "SphereCollider.h"
#include "DMesh.h"

BEGIN(Engine)

class CTexture;
class ENGINE_DLL CInvite_Character : public CGameObject
{
protected:
	explicit										CInvite_Character(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit										CInvite_Character(const CInvite_Character& rhs);
	virtual											~CInvite_Character();

public:
	virtual void									Free()								override;
	virtual void									Save(HANDLE hFile, DWORD& dwByte)	override;
	virtual void									Load(HANDLE hFile, DWORD& dwByte)	override;
	void											Load_Animation();
	static	CInvite_Character*						Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrWorldName);

public:
	virtual	HRESULT									Ready_Object()									override;
	virtual _int									Start_Object(const _float& fTimeDelta)			override;
	virtual	_int									Update_Object(const _float& fTimeDelta)			override;
	virtual	_int									LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void									Render_Object(_int iPass)						override;
	virtual void									Debug_This_In_Console()							override;
	virtual void									Set_Constantable()								override;
	virtual void									On_Collision(CGameObject* pDest)				override;

public:
	void											Cafe_Move(const _float& fTimeDelta);
	_bool											Find_Target(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag);
	_bool											Find_Target_InRange(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag);
	virtual void									Animation_Change(const _float& fTimeDelta);
	void											CutHideObstacle();
	virtual void									Reset();
	void											FSM_FunctionRelease();
	void											FSM_FunctionSetting(void(CInvite_Character::*pCol)(_float), void(CInvite_Character::*pDestFunc)(_float));
	void											Clear_Astar();

public:
	virtual const wstring							Get_Name() override { return m_wstrUnitName; }
	T_UnitInfo										Get_UnitInfo() { return m_tUnitInfo; }
	_bool											Get_HasHideObstacle() { return (m_pHideObstacle) ? TRUE : FALSE; }
	_bool											Get_UnitDead() { return (0 >= m_tUnitInfo.iNowHp); }
	_bool											Get_ActionIsEmpty(E_ACTION eAction) { return m_vecAnimActions[(_int)eAction].qAnimation.empty(); }
	CSphereCollider*								Get_SphereCollider() { return m_pSphereCollider; }
	_float											Get_UnitRange() { return _float(m_tUnitInfo.iRange) * Range_Cor; }
	_bool											Get_CanJump() { return m_bJumpObstacle; }

public:
	void											Set_Action(E_ACTION eAction);
	void											Set_ActionByEnum(E_ACTION eAction);
	void											Set_HideObstacle(CGameObject* pObj);
	void											Set_TargetPos(_vec3 vPos) { m_vTargetPos = vPos; }
	void											Set_ChagnePreset(_bool Preset) { m_bChangePreset = Preset; }
	void											Set_AllStorage(_bool Storage) { m_bAllStorage = Storage; }
	void											Set_CreateUpdate(_bool Update) { m_bCreateUpdate = Update; }
	void											Set_TimeSet(_bool time) { m_bTimeSet = time; }

//FSM
public:
	virtual void									Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void									Tracking(_float fDeltaTime);
	virtual void									Idle(_float fDeltaTime);
	virtual void									Ready_End(_float fDeltaTime);
	virtual void									Move_End_Stance(_float fDeltaTime);
	virtual void									Hiding(_float fDeltaTime);
	virtual void									Assemble(_float fDeltaTime);
	virtual void									Dissipation(_float fDeltaTime);

//�׼� Ʈ����
protected:
	void											Initialize_Trigger();
	void											ReSetting_AnimActionVec();
	void											ReSetting_ActionTrigger();

	void											Active_Trigger(T_AnimTrg& trg);
	virtual void									Trigger_Attack_1() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack1"); }
	virtual void									Trigger_Attack_2() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack2"); }
	virtual void									Trigger_Attack_3() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack3"); }
	virtual void									Trigger_Dead() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Dead"); }
	virtual void									Trigger_Reload() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Reload"); }
	virtual void									Trigger_ExSkill() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� ExSkill"); }
	virtual void									Trigger_Buff_1() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Buff1"); }
	virtual void									Trigger_Buff_2() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Buff2"); }
	virtual void									Trigger_1() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 1"); }
	virtual void									Trigger_2() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 2"); }
	virtual void									Trigger_3() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 3"); }
	virtual void									Trigger_4() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 4"); }
	virtual void									Trigger_5() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 5"); }
	virtual void									Trigger_6() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 6"); }
	virtual void									Trigger_7() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 7"); }
	virtual void									Trigger_8() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 8"); }
	virtual void									Trigger_9() { CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 9"); }

protected:
	virtual void									Ready_DMesh();
	_bool											Find_DMesh();
	virtual void									DeleteObstacle();
	virtual void									FindObstacle();
	_bool											Is_AnimFinished();
	void											Update_Trigger(const _float& fTimeDelta);
	virtual void									Update_Collider(const _float& fTimeDelta);
	void											Look_Target_Slowly(const _float& fTimeDelta);
	void											Create_Collider(CSphereCollider** pCol, wstring wstrCompName, _float fRadius);
	void											Collider_SetMatrix(CSphereCollider* pCol, const char* szBoneName);

private:
	void											Set_AnimIndexForTrigger(_uint iIndex);
	void											Set_ActionByName(wstring wstrName);

protected:
	CDMesh*											m_pDMeshCom = nullptr;
	CShader*										m_pShaderCom = nullptr;
	CSphereCollider*								m_pSphereCollider = nullptr;				// Sphere ������Ʈ
	CInvite_Character*								m_pTarget = nullptr;						// Ÿ��
	CGameObject*									m_pHideObstacle = nullptr;					// �����ִ� ��ֹ�

	wstring											m_wstrUnitName = L"Empty";
	wstring											m_wstrDMeshProtoTag = L"Empty";
	wstring											m_wstrDMeshFullPath = L"Empty";
	char*											m_szBodyBoneName = "Bip001_Pelvis";			// ���� �� ��ġ ������ Bip001 Pelvis

	T_UnitInfo										m_tUnitInfo;

	E_UnitMode										m_eUnitMode = E_UnitMode::None;
	E_HIDE											m_eHide = E_HIDE::HIDE_NONE;
	E_ACTION										m_eCurrentAction = E_ACTION::ACTION_END;
	E_ACTION										m_ePrevAction = E_ACTION::ACTION_END;
	E_LAYERTAG										m_eTargetLayerTag = E_LAYERTAG::TAG_END;
	E_OBJTAG										m_eTargetObjTag = E_OBJTAG::OBJ_END;

	_float											m_fDestDistForArrive = 0.05f;				// �̵� �ּ� ����
	_float											m_fMoveSpeed = 1.8f;						// �̵��ӵ�
	_bool											m_bCanHideSmall = TRUE;						// ����	��ֹ� �������� ����			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool											m_bCanHideBig = TRUE;						// ū	��ֹ� �������� ����			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool											m_bJumpObstacle = TRUE;						// ����	��ֹ� ���� �� �ִ���			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool											m_bMoveThisFrame = FALSE;					// �̹�	�����ӿ� ���������� �Ǵ�
	_bool											m_bStateLock = FALSE;						// ����	�������� �ƴ���
	_float											m_fFindTargetCool = 0.5f;					// �������� Ÿ���� ���� ��� Cool���� Ÿ�� Ž��
	_float											m_fFindTargetTime = 0.5f;
	_float											m_fFindObstacleCool = 0.5f;					// ��ֹ��� ã�� ��Ÿ��
	_float											m_fFindObstacleTime = 0.5f;
	_float											m_fDeadShaderFilter = 0.f;

//�׼� Ʈ����
protected:
	vector<pair<E_Trigger, std::function<void()>>>	m_vecFunction_Trg;
	vector<list<T_AnimTrg>>							m_vecAnimTrg;
	vector<T_AnimActionSys>							m_vecAnimActions;
	queue<_int>										m_qNextAnimIdx;
	queue<T_AnimTrg>								m_qTrg;

//FSM
protected:
	void(CInvite_Character::*m_fpLockFunction)(_float) = nullptr;
	void(CInvite_Character::*m_fpDestFunction)(_float) = nullptr;

//Astar
public:
	void											Find_Astar(_vec3 vStartPos, _vec3 vDest, _bool bCanJump);
	void											MoveTo_Astar(_float fTimeDelta, _float fSpeed, _float fSRatio);

protected:
	list<T_AstarNode>								m_listAstarNode;
	_vec3											m_vTargetPos = VEC_ZERO;

private:
	CTexture*										m_pTexMouse = nullptr;		// ���� �׸��� ���� �ؽ���
	CGameObject*									m_pFurObj = nullptr;
	CObstacle_Cafe*									m_pFurniture = nullptr;
	_vec3											m_vCafeMoveDest;
	_bool											m_bRandomPick = true;
	_bool											m_bTimeSet = false;
	_float											m_fIdleTime = 0.f;
	_bool											m_bChangePreset = false;
	_bool											m_bAllStorage = false;
	_vec3											m_vecCharPos;
	_vec3											m_vecCharDir;
	_bool											m_bCreateUpdate = false;
};

END

#endif // !__INVITE_CHARACTER_H__