#pragma once
#ifndef __Char_Cherino_H__
#define __Char_Cherino_H__

#include "Character.h"
#include "Cherino_ExUnit.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Cherino final : public CCharacter
{
private:
	explicit CChar_Cherino(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Cherino(const CChar_Cherino& rhs);
	virtual ~CChar_Cherino();

public:
	static	CChar_Cherino*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

	CCherino_ExUnit*	m_pCherino_ExUnit = nullptr;
};

#endif //__Char_Aru_H__