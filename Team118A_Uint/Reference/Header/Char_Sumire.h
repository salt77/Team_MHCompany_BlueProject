#pragma once
#ifndef __Char_Sumire_H__
#define __Char_Sumire_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Sumire final : public CCharacter
{
private:
	explicit CChar_Sumire(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Sumire(const CChar_Sumire& rhs);
	virtual ~CChar_Sumire();

public:
	static	CChar_Sumire*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta)				override;
	virtual	_int		Update_Object(const _float& fTimeDelta)				override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta)			override;
	virtual	void		Render_Object(_int iPass)							override;

	void				Add_Component();
	virtual void		Set_Constantable()	override;

	virtual void		Set_GuideSkill()	override;

private:
	static	void		ColMonster(CGameObject* pThis, CGameObject* pMonster);

	virtual void		ExSkill(_float fDeltaTime);

public:
	virtual void	End_Battle_Sound(const _bool& bWin) override;

protected:
	virtual void	Begin_Battle_Sound() override;
	virtual void	Trigger_Attack_1() override;
	virtual void	Trigger_Reload() override; 
	virtual void	Trigger_ExSkill()	override;
	virtual void	Trigger_1()			override;
	virtual void	Trigger_2()			override;
	virtual void	Trigger_3()			override;
	virtual void	Trigger_4()			override;
	virtual void	Trigger_5()			override;
	virtual void	Trigger_9()			override;

	virtual void	GuideSkillSetting(_vec3 vPickPos)	override;


private:
	CSkillObject*	m_pExsObject = nullptr;

private:
	_int			m_iExMoveType	= 0;
	_vec3			m_vExRight		= VEC_ZERO;
	_vec3			m_vExLeft		= VEC_ZERO;
	_vec3			m_vExDir		= VEC_ZERO;
};

#endif //__Char_Sumire_H__