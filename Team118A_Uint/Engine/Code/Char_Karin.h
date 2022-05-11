#pragma once
#ifndef __CChar_Karin_H__
#define __CChar_Karin_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Karin final : public CCharacter
{
private:
	explicit CChar_Karin(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Karin(const CChar_Karin& rhs);
	virtual ~CChar_Karin();

public:
	static	CChar_Karin*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

protected:
	void			Trigger_Attack_1() override;
};

#endif //__Char_Aru_H__