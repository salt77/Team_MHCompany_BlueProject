#pragma once

#ifndef __SKILL_OBJECT_H__
#define __SKILL_OBJECT_H__

#include "GameObject.h"

BEGIN(Engine)

class CSkillObject : public CGameObject
{
public:
	CSkillObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CSkillObject(const CSkillObject& rhs);
	virtual ~CSkillObject();

public:
	static CSkillObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale);
	virtual void			Free()override;

	virtual HRESULT			Ready_Object(vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale);


	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

	vector<CGameObject*>*	Get_vecSkillObjects() { return &m_vecSkillObject; };

protected:
	vector<CGameObject*> m_vecSkillObject;
};

END

#endif