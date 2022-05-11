#pragma once
#ifndef __RectTest_H__
#define __RectTest_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectTest final: public CVIBuffer
{
private:
	explicit CRectTest(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectTest(const CRectTest& rhs);
	virtual ~CRectTest();


public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	virtual CComponent*		Clone();
	virtual void			Free() override;
	static CRectTest*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END

#endif __RectTest_H__

