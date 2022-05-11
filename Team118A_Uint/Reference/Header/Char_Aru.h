#pragma once
#ifndef __Char_Aru_H__
#define __Char_Aru_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Aru final : public CCharacter
{
private:
	explicit CChar_Aru(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Aru(const CChar_Aru& rhs);
	virtual ~CChar_Aru();

public:
	static	CChar_Aru*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

protected:
	virtual void		Create_HitEffect() override;
	void				AruBomb();

	virtual void		GuideSkillSetting(_vec3 vPickPos)override;
protected:
	void				Trigger_Attack_1() override;

	virtual void		Trigger_Reload() override;

	virtual void		Trigger_ExSkill() override;

	virtual void		Trigger_1() override;
	virtual void		Trigger_9() override;
	
private:	
	CGameObject*	m_pExTarget = nullptr;

	_vec3 m_vPos_Exs = _vec3();
	_vec3 m_vRot_Exs = _vec3();
};

#endif //__Char_Aru_H__