#pragma once
#ifndef __CROSS_RECT_BUFFER_H__
#define __CROSS_RECT_BUFFER_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCrossRectBuffer final: public CVIBuffer
{
private:
	explicit CCrossRectBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCrossRectBuffer(const CCrossRectBuffer& rhs);
	virtual ~CCrossRectBuffer();


public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;
	_bool				Picking_Buffer(HWND hwnd, _vec3* pOutPos,_float* pOutDist,_matrix matWorld);
public:
	virtual CComponent*		Clone();
	virtual void			Free() override;
	static CCrossRectBuffer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END

#endif

