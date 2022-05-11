#ifndef SkyDome_h__
#define SkyDome_h__

#include "GameObject.h"
#include "Engine_Define.h"
BEGIN(Engine)

class CSphereTex2;
class CCubeTex;
class CTexture;
class CRenderer;
class CTransform;
class CShader;

END

class ENGINE_DLL CSkyDome : public CGameObject
{
private:
	explicit CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome(void);

public:
	virtual HRESULT		Ready_Object(wstring wstrSkyFullPath);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;

	void				Set_SkyTex(wstring wstrTexFullPath);
private:
	HRESULT				Add_Component(wstring wstrSkyFullPath);
	void				Set_ConstantTable();

private:
	CShader*			m_pShaderCom		= nullptr;
	CSphereTex2*		m_pSphereDomeCom	= nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture			= nullptr;
	

public:
	static CSkyDome*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrSkyFullPath);
	virtual void			Free(void);
};

#endif // SkyDome_h__
