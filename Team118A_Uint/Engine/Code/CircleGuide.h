#pragma once

#ifndef __CIRCLE_GUIDE_H__
#define __CIRCLE_GUIDE_H__

#include "SkillGuideObject.h"
class CCircleGuide :
	public CSkillGuideObject
{
private:
	CCircleGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CCircleGuide(const CCircleGuide& rhs);
	virtual ~CCircleGuide();

public:
	static CCircleGuide* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	HRESULT					Ready_Object(_float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");

	virtual void			Free()override;

	virtual HRESULT			Ready_Object();


	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void Update_DelayMesh() override;

private:
	_float	m_fRadius;
	_int	m_iVertexCount;

	_bool	m_bDelayMesh = FALSE;
};

#endif