#pragma once

#ifndef __STRAIGHT_GUIDE_H__
#define __STRAIGHT_GUIDE_H__

#include "SkillGuideObject.h"
class CStraightGuide :
	public CSkillGuideObject
{
private:
	CStraightGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CStraightGuide(const CStraightGuide& rhs);
	virtual ~CStraightGuide();

public:
	static CStraightGuide* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	HRESULT					Ready_Object(_float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath = L"");

	virtual void			Free()override;

	virtual HRESULT			Ready_Object();


	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void Update_DelayMesh() override;

private:
	_float	m_fDistance;
	_float  m_fWidth;
	_bool	m_bDelayMesh = FALSE;
};

#endif