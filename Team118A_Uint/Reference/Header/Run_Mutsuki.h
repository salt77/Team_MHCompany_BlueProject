#pragma once

#ifndef __RUN_MUTSUKI_H__
#define __RUN_MUTSUKI_H__

#include "Run_Player.h"

BEGIN(Engine)

class ENGINE_DLL CRun_Mutsuki final : public CRun_Player
{
private:
	explicit CRun_Mutsuki(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag);
	explicit CRun_Mutsuki(const CRun_Mutsuki& rhs);
	virtual ~CRun_Mutsuki();

public:
	static	CRun_Mutsuki*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrUnitName, E_OBJTAG eObjTag = E_OBJTAG::OBJ_Run_Player);
	virtual void			Free() override;
	virtual void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT			Ready_Object();
	virtual _int			Start_Object(const _float& fTimeDelta)	override;
	virtual	_int			Update_Object(const _float& fTimeDelta)	override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta)	override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void			Debug_This_In_Console() override;

	virtual void			Set_Constantable() override;
	virtual void			Set_Constantable_Fog(CShader* pShader) override;

public:
	virtual void			OnEnable();
	virtual void			OnDisable();
	virtual void			OnDestroy();
	virtual void			On_Collision(CGameObject* pDest);

private:
	//virtual	void			Key_Input() override; 
};

END 

#endif //__RUN_MUTSUKI_H__