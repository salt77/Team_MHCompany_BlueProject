#pragma once
#ifndef __RECT_BUFFER_H__
#define __RECT_BUFFER_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRectBuffer final: public CVIBuffer
{
private:
	explicit CRectBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRectBuffer(const CRectBuffer& rhs);
	virtual ~CRectBuffer();


public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;
	_bool				Picking_Buffer(HWND hwnd, _vec3* pOutPos,_float* pOutDist,_matrix matWorld);
public:
	virtual CComponent*		Clone();
	virtual void			Free() override;
	static CRectBuffer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END

#endif

