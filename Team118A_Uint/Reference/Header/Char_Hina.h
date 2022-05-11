#pragma once
#ifndef __Char_Hina_H__
#define __Char_Hina_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class ENGINE_DLL CChar_Hina final : public CCharacter
{
private:
	explicit CChar_Hina(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CChar_Hina(const CChar_Hina& rhs);
	virtual ~CChar_Hina();

public:
	static	CChar_Hina*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
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

private:
	static	void		ColMonster(CGameObject* pThis, CGameObject* pMonster);

	void				HinaAura();

	void				StartCam();
	
	void				ExsHina();

protected:
	void			Trigger_Attack_1() override;

	virtual void	Trigger_Reload() override;

	void			Trigger_ExSkill() override;

	// 오라 시작
	virtual void	Trigger_1()override;

	// 발사 시작
	virtual void	Trigger_2()override;

	// 어둠 풍압
	virtual void	Trigger_3()override;

	//Cam Start
	virtual void	Trigger_9()override;
	//Cam Change
	virtual void	Trigger_8()override;
	//Cam End
	virtual void	Trigger_7()override;

};

#endif //__Char_Aru_H__