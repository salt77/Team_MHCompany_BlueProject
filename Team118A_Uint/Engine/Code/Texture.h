#ifndef __Texture_H__
#define __Texture_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CComponent
{
protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	virtual CComponent*		Clone();
	virtual void			Free();
	virtual	void			Save(HANDLE hFile);
	virtual void			Load(HANDLE hFile);
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const wstring& pPath,
									E_TEXTURE_TYPE eType = E_TEXTURE_TYPE::TEX_NORMAL,
									const _uint& iCnt = 1);

	HRESULT		Ready_Texture(const wstring& pPath, E_TEXTURE_TYPE eType, const _uint& iCnt);

	void		Set_Texture_Device();
	void		Set_Texture(LPD3DXEFFECT pEffect, const char* pConstantTable, const _uint& iIndex = 0);
	void		Set_TexOffSet(_uint iIdx);
	void		Set_TexNum(_uint iIdx);

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;

public:
	HRESULT				Set_TexInfo(const wstring& pPath);
	const T_TEXINFO&	Get_TexInfo();

private:
	_int		m_iTexOffSet = 0;
	_int		m_iTexNum = 0;
	T_TEXINFO	m_tTexInfo;
};

END
#endif // __Texture_H__
