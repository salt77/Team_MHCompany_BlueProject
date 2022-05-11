#pragma once

#ifndef __ATTACKOBJ_MONSTER_H__
#define __ATTACKOBJ_MONSTER_H__

#include "AttackObj.h"

class CAttackObj_Monster : public CAttackObj
{
public:
	explicit CAttackObj_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_ATKOBJ_TYPE eAtkObjType);
	explicit CAttackObj_Monster(const CAttackObj_Monster& rhs);
	virtual ~CAttackObj_Monster();

public:
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta);
	virtual	_int		Update_Object(const _float& fTimeDelta);
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta);
	virtual	void		Render_Object(_int iPass = 0);

	virtual void		Debug_This_In_Console();

	virtual void		Set_Constantable();

public:
	virtual void		OnEnable() override;
	virtual void		OnDisable() override;
	virtual void		OnDestroy() override;
	virtual void		On_Collision(CGameObject* pDest) override;
};

#endif //__ATTACKOBJ_MONSTER_H__