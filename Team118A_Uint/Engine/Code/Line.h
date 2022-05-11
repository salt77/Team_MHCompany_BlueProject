#ifndef __LINE_H__
#define __LINE_H__

/*
라인 렌더링
담당자 : 최은호

*/

#include "Engine_Define.h"
#include "Base.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CD3DLine : public CBase
{
	DECLARE_SINGLETON(CD3DLine)

private:
	CD3DLine();
	virtual ~CD3DLine();

public:
	HRESULT		Ready_Line(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Render_Line(_vec3* vSrc, const _int& iSize, const DWORD color, const _matrix& matWorld);
	void		Render_Line2D(_vec2* vSrc, const _int& iSize, const DWORD color, const _matrix& matWorld);

private:
	virtual void Free();

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXSPRITE			m_pSprite;
	LPD3DXLINE				m_pLine;

};

END
#endif // __LINE_H__
