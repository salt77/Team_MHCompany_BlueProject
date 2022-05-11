#pragma once
#ifndef __Char_Wakamo_H__
#define __Char_Wakamo_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Wakamo final : public CCharacter
{
private:
	explicit CChar_Wakamo(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Wakamo(const CChar_Wakamo& rhs);
	virtual ~CChar_Wakamo();

public:
	static	CChar_Wakamo*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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
	void			Trigger_Attack_1()	override;
	virtual void	Trigger_ExSkill()	override;
	virtual void	Trigger_1()			override;
	virtual void	Trigger_2()			override;
	virtual void	Trigger_3()			override;
	virtual void	Trigger_4()			override;
	virtual void	Trigger_5()			override;
	virtual void	Trigger_6()			override;
	virtual void	Trigger_7()			override;
	virtual void	Trigger_8()			override;
	virtual void	Trigger_9()			override;


	void		ExSound();
private:
	void			Create_ExBullet(_bool bIsLast = FALSE);


};

#endif //__Char_Wakamo_H__