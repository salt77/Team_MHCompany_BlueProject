#pragma once
#ifndef __Char_Shun_H__
#define __Char_Shun_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Shun final : public CCharacter
{
private:
	explicit CChar_Shun(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Shun(const CChar_Shun& rhs);
	virtual ~CChar_Shun();

public:
	static	CChar_Shun*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Set_GuideSkill()override;
public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass)						override;

	void				Add_Component();
	virtual void		Set_Constantable() override;

protected:
	virtual void		GuideSkillSetting(_vec3 vPickPos)override;

	virtual void		Trigger_Attack_1() override;

	virtual void		Trigger_Reload() override;

	virtual void		Trigger_ExSkill() override;
	virtual void		Trigger_9() override;
};

#endif //__Char_Shun_H__