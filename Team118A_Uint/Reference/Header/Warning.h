#pragma once
#ifndef __WarningObject_H__
#define __WarningObject_H__

#include "GameObject.h"

USING(Engine)

#include "EffectObject.h"


class ENGINE_DLL CWarning : public CGameObject
{
protected:
	explicit CWarning(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWarning(const CWarning& rhs);
	virtual ~CWarning();

public:
	static CWarning*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free();

public:
	virtual HRESULT			Ready_Object() override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass) override;

public:
	void					Copy(CWarning* pWarn);
	void					Set_Warning(E_WarnType eOutType, E_WarnType eInnerType, _vec3 vPos, _float fTotalTime, _float fDelay, _float fRadius, D3DXCOLOR colorWarning = D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	void					Set_Delay(const _float& fDelay);

	void					Set_DamageToPlayer();
	void					Set_Damage(_int iDamage);
	void					Set_AfterSkillName(wstring wstrSkillName, _vec3 vAddPos, _vec3 vScale);
	void					Set_SoundName(TCHAR* szFirst, _float fSecond);
	void					Add_CombatStatus(T_CombatStatus tStatus)			{ m_vecCombatStatus.push_back(tStatus);	}


	CEffectObject*			Get_MaxEffect()										{ return m_pMaxRangeEffect;		}
	CEffectObject*			Get_NowEffect()										{ return m_pNowRangeEffect;		}

private:
	CEffectObject*			m_pMaxRangeEffect		= nullptr;
	CEffectObject*			m_pNowRangeEffect		= nullptr;

	T_UnitInfo				m_tUnitInfo;
	_float					m_fDelay				= 0.f;
	_float					m_fTotalTimer			= 1.f;
	_float					m_fNowTimer				= 0.f;

	_float					m_fRadius				= 0.5f;

	_bool					m_bDamage_OnOff			= FALSE;
	_int					m_iDamage				= 1;
	E_CHAR_ATKTYPE			m_eAtkType				= E_CHAR_ATKTYPE::ATKTYPE_NORMAL;


	wstring					m_wstrAfterSkillName	= L"None";
	_vec3					m_vAfterSkill_AddPos	= VEC_ZERO;
	_vec3					m_vAfterSkill_Scale		= VEC_ONE;

	pair<TCHAR*, _float>	m_pairSound;

	vector<T_CombatStatus>  m_vecCombatStatus;
};

#endif // !__WarningObject_H__