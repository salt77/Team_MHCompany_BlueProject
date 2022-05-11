#pragma once
#ifndef __Light_H__
#define __Light_H__

#include "Base.h"
#include "Engine_Define.h"

//////////////////////////////
// 수정자 : 최은호
//////////////////////////////

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();

public:
	static CLight*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	virtual void			Free();

public:
	HRESULT					Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void					Render_Light(LPD3DXEFFECT& pEffect);

public:
	D3DLIGHT9*				Get_Light() { return &m_tLightInfo; }


private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_tLightInfo;

	_uint					m_iIndex = 0;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;


};

END
#endif // __Light_H__
