#pragma once

#ifndef __UI_SKILL_OBJECT_H__
#define __UI_SKILL_OBJECT_H__

#include "GameObject.h"
#include "SkillObject.h"
BEGIN(Engine)

class CUISkillObject :
	public CSkillObject
{
public:
	CUISkillObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CUISkillObject(const CUISkillObject& rhs);
	virtual ~CUISkillObject();

public:
	static CUISkillObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale);
	virtual void			Free()override;

	virtual HRESULT			Ready_Object(vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale);

	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

public:
	void				Set_UIPos(_vec2 vUIPos);
	_vec2				Get_UIPos() { return m_vUIPos; }

	void				Set_UIDir(_vec2 vUIDir) { m_vDir = vUIDir; }
	void				Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void				Set_RotSpeed(_float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
private:
	virtual void			Set_Pos(_vec3 vPos) { m_pTransformCom->Set_Pos(vPos); }
	virtual void			Set_Pos(E_POSITION ePosition, _float fPos) { m_pTransformCom->Set_Pos(ePosition, fPos); }
	virtual void			Add_Pos(_vec3 vAddPos) { m_pTransformCom->Add_Pos(vAddPos); }
	virtual void			Add_Pos(E_POSITION ePosition, _float fAddPos) { m_pTransformCom->Add_Pos(ePosition, fAddPos); }
	virtual void			Move_Pos(const _vec3& pDir, const _float& fSpeed, const _float& fTimeDelta) { m_pTransformCom->Move_Pos(pDir, fSpeed, fTimeDelta); }

	void					Update_UISkill(_float fTimeDelta);
private:
	_vec2				m_vDir = _vec2(0.f, 0.f);
	_float				m_fSpeed = 0.f;
	_float				m_fRotSpeed = 0.f;
	_vec2				m_vUIPos = _vec2(0.f, 0.f);
	_float				m_fZRot = 0.f;
};

END

#endif