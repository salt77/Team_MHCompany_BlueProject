#pragma once
#ifndef __Char_Chise_H__
#define __Char_Chise_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Chise final : public CCharacter
{
private:
	explicit CChar_Chise(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Chise(const CChar_Chise& rhs);
	virtual ~CChar_Chise();

public:
	static	CChar_Chise*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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