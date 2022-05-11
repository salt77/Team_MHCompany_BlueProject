#pragma once

#ifndef __DONUT_GUIDE_H__
#define __DONUT_GUIDE_H__

#include "SkillGuideObject.h"
class CDonutGuide :
	public CSkillGuideObject
{
private:
	CDonutGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CDonutGuide(const CDonutGuide& rhs);
	virtual ~CDonutGuide();

public:
	static CDonutGuide* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fMinDistance, _float fMaxDistance, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	HRESULT					Ready_Object(_float fMinDistance, _float fMaxDistance, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");

	virtual void			Free()override;

	virtual HRESULT			Ready_Object();


	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

	virtual void Update_DelayMesh() override;

private:
	_float	m_fMinDistance;
	_float  m_fMaxDistance;
	_int	m_iVertexCount;

	_bool	m_bDelayMesh = FALSE;
};

#endif