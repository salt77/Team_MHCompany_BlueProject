#ifndef SphereTex2_h__
#define SphereTex2_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex2 : public CVIBuffer
{
private:
	explicit CSphereTex2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereTex2(const CSphereTex2& rhs);
	virtual ~CSphereTex2(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	static CSphereTex2*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void)override;
	virtual void Free(void) override;	
};

END
#endif // SphereTex2_h__
