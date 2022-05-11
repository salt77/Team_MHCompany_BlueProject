#pragma once
#ifndef __Char_Momoi_H__
#define __Char_Momoi_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Momoi final : public CCharacter
{
private:
	explicit CChar_Momoi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Momoi(const CChar_Momoi& rhs);
	virtual ~CChar_Momoi();

public:
	static	CChar_Momoi*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	virtual void		Set_GuideSkill()override;
private:
	static	void		ColMonster(CGameObject* pThis, CGameObject* pMonster);
protected:
	void			Trigger_Attack_1() override;
	void			Trigger_Attack_3() override;
	void			Trigger_ExSkill() override;
	virtual void	Trigger_1()override; 

	//Cam Start
	virtual void	Trigger_9()override;
	//Cam Change
	virtual void	Trigger_8()override;
	//Cam End
	virtual void	Trigger_7()override;
	virtual void	GuideSkillSetting(_vec3 vPickPos)override;
private:
	CSkillObject* m_pExsObject = nullptr;
};

#endif //__Char_Aru_H__