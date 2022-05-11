#pragma once
#ifndef __Char_Mutsuki_H__
#define __Char_Mutsuki_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Mutsuki final : public CCharacter
{
private:
	explicit CChar_Mutsuki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Mutsuki(const CChar_Mutsuki& rhs);
	virtual ~CChar_Mutsuki();

public:
	static	CChar_Mutsuki*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override; 
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object() override;
	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass)						override;

	void					Add_Component();
	virtual void			Set_Constantable() override;

protected:
	virtual void			Trigger_Attack_1() override;
};

#endif //__Char_MUTSUKI_H__