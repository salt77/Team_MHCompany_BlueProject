#pragma once
#ifndef __Char_Zunko_H__
#define __Char_Zunko_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Zunko final : public CCharacter
{
private:
	explicit CChar_Zunko(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Zunko(const CChar_Zunko& rhs);
	virtual ~CChar_Zunko();

public:
	static	CChar_Zunko*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	void				ColEx(CGameObject * pMonster);
	virtual void		Set_GuideSkill()override;
protected:
	void			Trigger_Attack_1() override;
	void			Trigger_ExSkill() override;
	virtual void	Trigger_1()override;

	//EX 카메라 시작
	virtual void	Trigger_9()override;
	//끝
	virtual void	Trigger_8()override;

	virtual void	GuideSkillSetting(_vec3 vPickPos)override;
private:
	static void ColMonster(CGameObject * pThis, CGameObject * pMonster);
private:
	CSkillObject* m_pShootObject = nullptr;
	CSkillObject* m_pGroundObject = nullptr;

	vector<CGameObject*> m_vecColObject;
	_float				m_fExDistance = 0.f;
};

#endif //__Char_Aru_H__