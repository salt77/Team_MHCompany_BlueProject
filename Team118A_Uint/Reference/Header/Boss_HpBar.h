#pragma once

#ifndef __BOSS_HPBAR_H__
#define __BOSS_HPBAR_H__

#include "UI_Object.h"

class ENGINE_DLL CBoss_HpBar final : public CUI_Object
{
private:
	explicit CBoss_HpBar(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	explicit CBoss_HpBar(const CBoss_HpBar& rhs); 
	virtual ~CBoss_HpBar();

public:
	static	CBoss_HpBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							HWND* pHwnd, 
							const wstring& wstrBossName, 
							_int iBossMaxHp, 
							_uint iLineCount, 
							const wstring& wstrLoadFile = L"[Fixed]Boss_Hp_Bar.uidat", 
							E_OBJTAG eObjTag = E_OBJTAG::OBJ_UI_BossHpbar);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object(const wstring& wstrBossName, _int iBossMaxHp, _uint iLineCount, const wstring& wstrLoadFile);
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass) override;

public:
	const _int&				Get_Hp() { return m_iBossHp; }
	CUI_Object*				Get_BossHP_Parent() { return m_pBossHpBar_Parent; }

	void					Add_Damage_ForUI(_int iDamage);
	void					Add_Gage_OtherStat(_float fValue);
	void					Set_Gage_OtherStat(const _float& fValue);

	void					Set_Gage_Groggy(const _float fGroggyRatio);

private:
	void					Add_Component();
	void					Set_ConstantTable();

	void					Load_SaveFile(const wstring& wstrLoadFile); 
	void					Update_BossHpLerpBar(const _float& fTimeDelta); 

private:
	CUI_Object*			m_pBossHpBar_Parent							= nullptr;
	CUI_Object*			m_pBossHpBar[(_uint)E_BOSS_HP::BOSSHP_END]	= { nullptr, nullptr, nullptr };
	CUI_Object*			m_pBossHpBar_Lerp							= nullptr;
	CUI_Object*			m_pBossHpBar_GroggyGage						= nullptr;

	_uint				m_iMaxLineCount			= 10;
	_uint				m_iCurLineCount			= m_iMaxLineCount;

	_int				m_iBossHp				= 999999;
	_int				m_iBossMaxHp			= m_iBossHp;
	_int				m_iLineHp				= m_iBossMaxHp / m_iMaxLineCount;

	_float				m_fCountingTime			= 0.f;
	_float				m_fLerpBar				= 1.f;
	_float				m_fLerpDelayTime		= 0.25f;
	_float				m_fLerpSpeed			= 0.65f;
	_float				m_fGageStat				= 0.f;
	E_BOSS_HP			m_eCurBossHp			= E_BOSS_HP::BOSSHP_2;
};

#endif //__BOSS_HPBAR_H__