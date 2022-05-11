#pragma once
#ifndef __UNIT_H__
#define __UNIT_H__

#include "GameObject.h"
#include "SphereCollider.h"
#include "Obstacle_Combat.h"
#include "DMesh.h"

#include "Skill_Manager.h"
#include "AttackObj.h"
#include "SkillObject.h"
#include "Sound_Manager.h"

BEGIN(Engine)

class CUI_Object;
class CTexture;
class ENGINE_DLL CUnit abstract : public CGameObject
{
protected:
	explicit CUnit(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CUnit(const CUnit& rhs);
	virtual ~CUnit();

public:
	virtual void	Free()											override;
	virtual void	Save(HANDLE hFile, DWORD& dwByte)				override;
	virtual void	Load(HANDLE hFile, DWORD& dwByte)				override;
	void			Load_Animation();

public:		/* �⺻		*/
	virtual	HRESULT	Ready_Object()									override;
	virtual _int	Start_Object(const _float& fTimeDelta)			override;
	virtual	_int	Update_Object(const _float& fTimeDelta)			override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void	Render_Object(_int iPass)						override;
	virtual void	Debug_This_In_Console()							override;
	virtual void	Set_Constantable()								override;

public:		/* �̺�Ʈ	*/
	virtual void			OnEnable()								override;
	virtual void			OnDisable()								override;
	virtual void			OnDestroy()								override;
	virtual void			On_Collision(CGameObject* pDest)		override;
	virtual void			Load_CombatUi();


public:		/* public �Լ� */
	void					Hit_Target();
	void					Hit_Target_Enum(E_UNIT_HIT_TYPE eUnitHitType);
	static void				Hit_Target_FanToCircle(CGameObject* pSrc, CGameObject* pDest);

	_bool					Find_Target(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag);
	_bool					Find_Target_InRange(E_LAYERTAG eFindLayerTag, E_OBJTAG eFindObjTag);
	virtual void			Animation_Change(const _float& fTimeDelta);
	void					CutHideObstacle();
	void					Reload_Ammo();
	virtual void			Reset();

	virtual void			Use_ExSkill();

	void					KnockBack(const _vec3& vTargetPos, const _float& fTimeDelta);
	void					FSM_FunctionRelease();
	virtual void			FSM_FunctionSetting(void(CUnit::*pCol)(_float), void(CUnit::*pDestFunc)(_float));
	void					Clear_Astar();
	void					CC_Taunt(_float fTime, CUnit* pTauntUnit);
	void					CC_Fear(_float fTime);

public:		/* Get		*/
	virtual const wstring			Get_Name() override							{ return m_wstrUnitName;										}
	virtual const E_CHAR_ATKTYPE	Get_AttackType()							{ return E_CHAR_ATKTYPE::ATKTYPE_END;							}
	virtual const E_CHAR_DFFTYPE	Get_DefenseType()							{ return E_CHAR_DFFTYPE::DFFTYPE_END;							}
	T_UnitInfo						Get_UnitInfo()								{ return m_tUnitInfo;											}
	_bool							Get_HasHideObstacle()						{ return (m_pHideObstacle) ? TRUE : FALSE;						}
	virtual _bool					Get_UnitDead()								{ return (0 >= m_tUnitInfo.iNowHp);								}
	_bool							Get_ActionIsEmpty(E_ACTION eAction)			{ return m_vecAnimActions[(_int)eAction].qAnimation.empty();	}
	_int							Get_TotalDamage()							{ return m_iTotalDamage;										}
	CSphereCollider*				Get_SphereCollider()						{ return m_pSphereCollider;										}
	virtual _float					Get_UnitRange()								{ return _float(m_tUnitInfo.iRange) * Range_Cor;				}
	_bool							Get_CanJump()								{ return m_bJumpObstacle;										}
	_vec3							Get_WeaponPos(_float fAddRight, _float fAddUp, _float fAddLook);
	_vec3							Get_WeaponRot();
	_bool							Get_Assemble()								{ return m_bAssemble;											}
	_bool							Get_Invn()									{ return m_bInvn;												}
	_bool							Get_IsCC();

public:		/* Set		*/
	virtual _int					Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType);
	virtual _int					Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType, _float fRatio);

	void							Set_ActionByEnum(E_ACTION eAction);
	void							Set_HideObstacle(CGameObject* pObj);
	void							Set_AmmoMax(_int iAmmoMax);
	void							Set_BodyBoneName(char* szName)				{ m_szBodyBoneName = szName;									}
	void							Set_KnockBack(const _vec3& vTargetPos)		{ m_bKnockBack = TRUE;	m_vKnockBackPos = vTargetPos;			}
	void							Set_TargetPos(_vec3 vPos)					{ m_vTargetPos = vPos;											}
	void							Set_Assemble(const _bool& bState)			{ m_bAssemble = bState;											}
	void							Set_Invn(const _bool& bState)				{ m_bInvn = bState;												}
	void							Set_TargetLayerTag(const E_LAYERTAG eLayer) { m_eTargetLayerTag = eLayer;									}
	void							Set_TargetObjTag(const E_OBJTAG eObjTag)	{ m_eTargetObjTag = eObjTag;									}
	void							Set_BeginBattle();
	void							Set_Play(_bool bPlay)						{ m_bPlay = bPlay;												}
	void							Set_NowHP(_int iHP)							{ m_tUnitInfo.iNowHp = iHP; }
	void							Reset_HP()									{ m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;						}
public:	/* FSM		*/
	virtual void		Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag);
	virtual void		Tracking(_float fDeltaTime);
	virtual void		Idle(_float fDeltaTime);
	virtual void		Ready_Shoot_Start(_float fDeltaTime);
	virtual void		Shoot_Start(_float fDeltaTime);
	virtual void		Shoot(_float fDeltaTime);
	virtual void		Shoot_End(_float fDeltaTime);
	virtual void		Ready_End(_float fDeltaTime);
	virtual void		Move_End_Stance(_float fDeltaTime);
	virtual void		Reload(_float fDeltaTime);
	virtual void		Hiding(_float fDeltaTime);
	virtual void		Vital_Panic(_float fDeltaTime);
	virtual void		CallSign(_float fDeltaTime);
	virtual void		Dead(_float fDeltaTime);
	virtual void		Victory(_float fDeltaTime);
	virtual void		Begin_Battle(_float fDeltaTime);
	virtual void		ExSkill(_float fDeltaTime);
	virtual void		ExSkill_CurIn(_float fDeltaTime);
	virtual void		Assemble(_float fDeltaTime);
	virtual void		Dissipation(_float fDeltaTime);

	virtual void		Special_State_1(_float fDeltaTime); 
	virtual void		Special_State_2(_float fDeltaTime); 

	// ������  FSM ������ ���� ������ Idle�� ���� �̵��ȴ�.
	virtual void		Boss_Attack_1(_float fDeltaTime);
	virtual void		Boss_Attack_2(_float fDeltaTime);
	virtual void		Boss_Attack_3(_float fDeltaTime);
	virtual void		Boss_Attack_4(_float fDeltaTime);
	virtual void		Boss_Groggy(_float fDeltaTime);

protected:	/* �׼� Ʈ����	*/
	void			Initialize_Trigger();
	void			ReSetting_AnimActionVec();
	void			ReSetting_ActionTrigger();

	void			Active_Trigger(T_AnimTrg& trg);
	virtual void	Trigger_Attack_1()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack1");		}
	virtual void	Trigger_Attack_2()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack2");		}
	virtual void	Trigger_Attack_3()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Attack3");		}
	virtual void	Trigger_Dead()			{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Dead");		}
	virtual void	Trigger_Reload()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Reload");		}
	virtual void	Trigger_ExSkill()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� ExSkill");		}
	virtual void	Trigger_Buff_1()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Buff1");		}
	virtual void	Trigger_Buff_2()		{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� Buff2");		}
	virtual void	Trigger_1()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 1" );			}
	virtual void	Trigger_2()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 2" );			}
	virtual void	Trigger_3()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 3" );			}
	virtual void	Trigger_4()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 4" );			}
	virtual void	Trigger_5()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 5" );			}
	virtual void	Trigger_6()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 6" );			}
	virtual void	Trigger_7()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 7" );			}
	virtual void	Trigger_8()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 8" );			}
	virtual void	Trigger_9()				{	CUtility::Print_Wstring_DMode(L"���� - Ʈ���� 9" );			}

protected:	/* protected �Լ� */
	virtual void				Ready_DMesh();
	_bool						Find_DMesh();
	virtual void				DeleteObstacle();
	virtual void				FindObstacle();
	_bool						Is_AnimFinished();
	void						Update_Trigger(const _float& fTimeDelta);
	virtual void				Update_Collider(const _float& fTimeDelta);
	void						Create_Effect(const wstring& wstrEffectName, const _float fRight, const _float fUp, const _float fLook);
	virtual void				Create_HitEffect();
	void						Look_Target_Slowly(const _float& fTimeDelta);
	void						Create_Collider(CSphereCollider** pCol, wstring wstrCompName, _float fRadius);
	void						Collider_SetMatrix(CSphereCollider* pCol, const char* szBoneName);
	
private:	/* private �Լ� */
	void			Create_DamageFont(E_NumberFontType eFontType, _int iDamage, _bool bCritical, _bool bEvade = FALSE);

private:
	void			Set_AnimIndexForTrigger(_uint iIndex);
	void			Set_ActionByName(wstring wstrName);




protected:  
	_bool				m_bPlay					= FALSE;

	_bool				m_bAnimPlay				= TRUE;

	CDMesh*				m_pDMeshCom				= nullptr;
	CShader*			m_pShaderCom			= nullptr;
	CSphereCollider*	m_pSphereCollider		= nullptr;					// Sphere ������Ʈ
	CSphereCollider*	m_pWeaponCollider		= nullptr;					// Weapon Sphere ������Ʈ ������ ��ġ
	CUnit*				m_pTarget				= nullptr;					// Ÿ��
	CGameObject*		m_pHideObstacle			= nullptr;					// �����ִ� ��ֹ�
	CObstacle*			m_pLastJumpObstacle		= nullptr;


	wstring				m_wstrUnitName			= L"Empty";
	wstring				m_wstrDMeshProtoTag		= L"Empty";
	wstring				m_wstrDMeshFullPath		= L"Empty";
	char*				m_szBodyBoneName		= "Bip001_Pelvis";			// ���� �� ��ġ ������ Bip001 Pelvis
	char*				m_szWeaponBoneName		= "Bip001_Weapon";			// ������ �� ��ġ	 bone_magazine, bone_magazine_L, bone_magazine_R

	T_UnitInfo			m_tUnitInfo;

	E_UnitMode			m_eUnitMode				= E_UnitMode::None;
	E_HIDE				m_eHide					= E_HIDE::HIDE_NONE;
	E_ACTION			m_eCurrentAction		= E_ACTION::ACTION_END;
	E_ACTION			m_ePrevAction			= E_ACTION::ACTION_END;
	E_LAYERTAG			m_eTargetLayerTag		= E_LAYERTAG::TAG_END;
	E_OBJTAG			m_eTargetObjTag			= E_OBJTAG::OBJ_END;
	E_UNIT_HIT_TYPE		m_eUnitHitType			= E_UNIT_HIT_TYPE::HIT_END;

	_float				m_fDestDistForArrive	= 0.05f;					// �̵� �ּ� ����
	_float				m_fMoveSpeed			= 1.8f;						// �̵��ӵ�
	_int				m_iAmmo					= 0;						// ���� �Ѿ�							( Reload_Ammo() ���� ����. )
	_int				m_iAmmo_Max				= 5;						// �Ѿ� Max��						( Set_AmmoMax() ���� ����. )
	_bool				m_bCanHideSmall			= TRUE;						// ����	��ֹ� �������� ����			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool				m_bCanHideBig			= TRUE;						// ū	��ֹ� �������� ����			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool				m_bJumpObstacle			= TRUE;						// ����	��ֹ� ���� �� �ִ���			( LoadAnimation�Լ����� �ڵ� ������. )
	_bool				m_bMoveThisFrame		= FALSE;					// �̹�	�����ӿ� ���������� �Ǵ�
	_bool				m_bStateLock			= FALSE;					// ����	�������� �ƴ���
	_int				m_iTotalDamage			= 0;						// ����	�� ������
	_float				m_fFindTargetCool		= 0.5f;						// �������� Ÿ���� ���� ��� Cool���� Ÿ�� Ž��
	_float				m_fFindTargetTime		= 0.5f;						// 
	_float				m_fFindObstacleCool		= 0.5f;						// ��ֹ��� ã�� ��Ÿ��
	_float				m_fFindObstacleTime		= 0.5f;						// 
	_float				m_fDeadShaderFilter		= 0.f;						//
	_bool				m_bKnockBack			= FALSE;
	_vec3				m_vKnockBackPos			= VEC_ZERO; 
	_bool				m_bAssemble				= FALSE;
	_bool				m_bInvn					= FALSE;
	_float				m_fCallSignTimer		= 15.f;
	_float				m_fJumpCoolTime			= 0.f;

	_float				m_fSoundVolume			= 0.4f; 

protected:	/* �׼� Ʈ����	*/
	vector<pair<E_Trigger, std::function<void()>>>	m_vecFunction_Trg;
	vector<list<T_AnimTrg>>							m_vecAnimTrg;
	vector<T_AnimActionSys>							m_vecAnimActions;
	queue<_int>										m_qNextAnimIdx;
	queue<T_AnimTrg>								m_qTrg;

protected:	/* FSM			*/
	void(CUnit::*m_fpLockFunction)(_float) = nullptr;
	void(CUnit::*m_fpDestFunction)(_float) = nullptr;

	
public:	/*	Astar ����	*/
	void					Find_Astar(_vec3 vStartPos, _vec3 vDest, _bool bCanJump);
	void					MoveTo_Astar(_float fTimeDelta, _float fSpeed, _float fSRatio);
protected:
	list<T_AstarNode>		m_listAstarNode;
	_vec3					m_vTargetPos = VEC_ZERO; 


public:		/*	Combat UI���� */
	CUI_Object*				m_pCombatObject			= nullptr;					// ������ UI 
	CUI_Object*				m_pHealthBar			= nullptr;					// ������ UI  ü�¹�
	CUI_Object*				m_pIcon_Hide			= nullptr;					// ������ UI  ����
	CUI_Object*				m_pIcon_Buff[4]			= { nullptr, nullptr, nullptr, nullptr};					// ������ UI  ����1
	CUI_Object*				m_pIcon_Debuff[4]		= { nullptr, nullptr, nullptr, nullptr};					// ������ UI  �����1
	CUI_Object*				m_pCombat_Reload		= nullptr;					// ������ UI  ���ε� Text
	_float					m_fCombat_Reload_Cool	= 0.25f;					// ���ε� Text (.) ���� �ٲ�� �ð�

	CParticleObject*		m_pParticle_CC			= nullptr;

public:
	void					Add_CombatStatus(T_CombatStatus tStatus) { m_vecCombatStatus.push_back(tStatus); }
	void					Update_CombatStatus(const _float& fTimeDelta);
public:
	_int					m_iArrNowCombatStatus[(_int)E_Combat_Status::Combat_StatusEnd];
	vector<T_CombatStatus>	m_vecCombatStatus;





public:
	T_UnitSound					m_tUnitSound;
};

END

#endif // !__UNIT_H__