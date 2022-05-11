#pragma once
#ifndef __Char_Suzumi_H__
#define __Char_Suzumi_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Suzumi final : public CCharacter
{
private:
	explicit CChar_Suzumi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Suzumi(const CChar_Suzumi& rhs);
	virtual ~CChar_Suzumi();

public:
	static	CChar_Suzumi*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	virtual void		Set_Constantable() override;

public:
	virtual void	End_Battle_Sound(const _bool& bWin) override;

protected:
	virtual void	Begin_Battle_Sound() override;
	virtual void	Trigger_Attack_1()	override;
	virtual void	Trigger_Attack_3() override; 
	virtual void	Trigger_Reload() override; 
	virtual void	Trigger_ExSkill()	override;
	virtual void	Trigger_9()			override;

	void			Granade();
};

#endif //__Char_Suzumi_H__