#pragma once

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Unit.h"
#include "ActionCam_Mgr.h"
#include "SkillGuideObject.h"
BEGIN(Engine)

class CSphereCollider;
class CTexture;

class ENGINE_DLL CCharacter abstract : public CUnit
{
protected:
	explicit CCharacter(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CCharacter(const CCharacter& rhs);
	virtual ~CCharacter();

public:
	virtual void					Free()								override;
	virtual void					Save(HANDLE hFile, DWORD& dwByte)	override;
	virtual void					Load(HANDLE hFile, DWORD& dwByte)	override;

public:		/* 기본					*/
	virtual	HRESULT					Ready_Object()									override;
	virtual _int					Start_Object(const _float& fTimeDelta)			override;
	virtual	_int					Update_Object(const _float& fTimeDelta)			override;
	virtual	_int					LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void					Render_Object(_int iPass)						override;
	virtual void					Set_Constantable()								override;

	virtual void					Set_GuideSkill();
	virtual void					Use_ExSkill()override;

	virtual void					Cancel_Aiming();

public:
	void							Update_CombatUI(const _float& fTimeDelta);

public:
	virtual void					OnEnable()		override;
	virtual void					OnDisable()		override;
	virtual void					OnDestroy()		override;
	virtual void					On_Collision(CGameObject* pDest)	override;


public:		/* Get		*/
	const T_CharInfo&				Get_CharInfo()						{ return m_tCharInfo;				}
	virtual const E_CHAR_ATKTYPE	Get_AttackType()	override		{ return m_tCharInfo.eAtkType;		}
	virtual const E_CHAR_DFFTYPE	Get_DefenseType()	override		{ return m_tCharInfo.eDffType;		}
	void							Set_AddRange(_int iRange) { m_iAddRange = iRange; }
	virtual _float					Get_UnitRange() { return _float(m_tUnitInfo.iRange + m_iAddRange) * Range_Cor; }
	virtual _bool					Get_UnitDead() { return ((0 >= m_tUnitInfo.iNowHp) || (-3.f > Get_Position().y)); }
	_bool							Get_Reader()						{ return m_bReader;					}

	void							Set_Aiming(_bool bAiming) { m_bAiming = bAiming; }
	void							Set_UseGravity(_bool bUse) { m_bUseGravity = bUse; }
public:		/* Set		*/
	virtual _int					Set_Damage(const T_UnitInfo& tEnemyInfo, E_CHAR_ATKTYPE eEnemyAtkType);
	void							Set_GeneralStat(E_CHAR_NAME eName);
	void							Set_ShowWindow(_bool bShowWindow)	{ m_bShowWindow = bShowWindow;		}
	void							Set_Action(E_ACTION eAction)		{ m_eCurrentAction = eAction;		}
	virtual void					Reset() override;
	void							LevelUpByLevel(_int iLevel);
	void							Set_Gravity(_vec3 vKnockBackDir, _float fKnockBackSpeed);
	void							Stop_Gravity();
	void							Set_Reader(_bool bReader)			{ m_bReader = bReader; }
	virtual void					End_Battle_Sound(const _bool& bWin); 
	
public:
	virtual void					ExSkill(_float fDeltaTime);
	virtual void					ExSkill_CurIn(_float fDeltaTime);

protected:
	virtual void					Begin_Battle(_float fDeltaTime);
	virtual void					Begin_Battle_Sound(); 
	virtual void					Update_Collider(const _float& fTimeDelta) override;

	virtual void					Update_Gravity(_float fTimeDelta);
	virtual _bool					Raycast_Gravity(_vec3* pOutPos);

	void							GuideSkillUpdate(_float fTimeDelta);
	_bool							GetRaycastMap(_vec3* pOutPos);
	virtual void					GuideSkillSetting(_vec3 vPickPos);
protected:
	virtual void					Trigger_Attack_1() override;

protected:
	_bool							m_bReader = FALSE;
	_bool							m_bStartSound = FALSE; 
	_bool							m_bAiming = FALSE;
	_vec3							m_vExDir = -VEC_LOOK;
	vector<CSkillGuideObject*>		m_vecGuideSkill;

	_int					m_iAddRange			= 0;

	CTexture*				m_pTexMouse			= nullptr;		// 입을 그리기 위한 텍스쳐
	
	T_CharInfo				m_tCharInfo;
	
	E_CHAR_ACTION			m_eCur_Char_Action	= E_CHAR_ACTION::C_ACTION_IDLE;
	E_CHAR_NAME				m_eMyName			= E_CHAR_NAME::Name_End;

	_float					m_fAniTime			= 0.f; 
	_float					m_fAniEndTime		= 0.f; 

	E_CharacterMouseTag		m_eMouse = E_CharacterMouseTag::MOUSE_END;

	_bool					m_bShowWindow = FALSE;

	// 중력 변수
	_bool					m_bUseGravity = FALSE;
	_bool					m_bGravityOn = FALSE;
	_float					m_fGravityAcc = 4.9f;
	_float					m_fGravitySpeed = 0.f;
	_vec3					m_vKnockBackDir = _vec3(0.f ,0.f, 0.f);
	_float					m_fKnockBackSpeed = 0.f;
};

END

#endif //__CHARACTER_H__