#pragma once

#ifndef __CUT_DONUT_GUIDE_H__
#define __CUT_DONUT_GUIDE_H__

#include "SkillGuideObject.h"
class CCutDonutGuide :
	public CSkillGuideObject
{
private:
	CCutDonutGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CCutDonutGuide(const CCutDonutGuide& rhs);
	virtual ~CCutDonutGuide();

public:
	static CCutDonutGuide* Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fMinDistance, _float fMaxDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	HRESULT					Ready_Object(_float fMinDistance, _float fMaxDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");

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
	_float  m_fAngle;
	_bool	m_bDelayMesh = FALSE;
};

#endif