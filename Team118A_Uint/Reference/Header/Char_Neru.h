#pragma once
#ifndef __Char_Neru_H__
#define __Char_Neru_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Neru final : public CCharacter
{
private:
	explicit CChar_Neru(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Neru(const CChar_Neru& rhs);
	virtual ~CChar_Neru();

public:
	static	CChar_Neru*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass)						override;

	void				Add_Component();
	virtual void		Set_Constantable() override;

protected:
	void				Trigger_Attack_1() override;

	virtual void		Trigger_Reload() override;

	virtual void		Trigger_ExSkill() override;

	virtual void		Trigger_1() override;
	virtual void		Trigger_2() override;
	virtual void		Trigger_3() override;
	virtual void		Trigger_8() override;
	virtual void		Trigger_9() override;

	void				MouseOri();
};

#endif //__Char_Neru_H__