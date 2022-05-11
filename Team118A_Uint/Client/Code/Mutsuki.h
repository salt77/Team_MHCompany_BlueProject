#pragma once

#ifndef __MUTSUKI_H__
#define __MUTSUKI_H__

#include "Character.h"

USING(Engine)

class CCamera_TPS;

class CMutsuki final : public CCharacter
{
private:
	explicit CMutsuki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CMutsuki(const CMutsuki& rhs);
	virtual ~CMutsuki();

public:
	static	CMutsuki*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	virtual void		Free() override; 
	virtual void		Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void		Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;

	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	void				Key_Input();
	void				Mouse_Move();
	void				Animation_Control(const _float& fTimeDelta);
	void				Animation_End(const _float& fTimeDelta);

	// 상태패턴 함수
	void				Movement(const _float& fTimeDelta);
	void				Shoot_Bullet();
	void				Fire();			// 레이 발사
	void				Fire2();		// 투사체 발사
	void				Stand_Fire();
	void				Aiming();
	void				Stand_Aiming();
	void				Reload();
	void				Stand(); 
	void				Skill();
	// 삭제 예정 함수 
	void				Rotate_DirVector(const _float& fDegree, const _vec3& vAxis, _vec3* vLook);
	void				Render_Line(const _vec3* vStartPos, const _vec3* vDir);

private:
	CCamera_TPS*		m_pTpsCam = nullptr;

	_float				m_fCountingTime = 0.f; 

	_vec3				m_vTargetPos = VEC_ZERO; 
	_vec3				m_vMoveDir = VEC_ZERO; 
	_vec3				m_vFrontDir = VEC_ZERO;
	_vec3				m_vRightDir = VEC_ZERO; 

};

#endif //__MUTSUKI_H__