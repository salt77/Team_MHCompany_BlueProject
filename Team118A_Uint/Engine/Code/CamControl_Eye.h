#ifndef __CamControl_Eye_H__
#define __CamControl_Eye_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"

USING(Engine)

class CCamControl_Eye final : public CGameObject
{
private:
	explicit CCamControl_Eye(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CCamControl_Eye(const CCamControl_Eye& rhs);
	virtual ~CCamControl_Eye();

public:
	static CCamControl_Eye*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _float fRadius = 1.f);
	virtual void				Free();
	virtual void				Save(HANDLE hFile, DWORD& dwByte);
	virtual void				Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT		Ready_Object(_vec3 vPos, _float fRadius);
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;

public:
	void				Set_RenderType(E_WIRE_RENDER eRenderType) { m_eRenderType = eRenderType; }
	
private:
	void				Add_Component();
	virtual void		Set_Constantable() override; 

private:
	_float				m_fRadius = 1.f;

	E_WIRE_RENDER		m_eRenderType = E_WIRE_RENDER::WIRE_RENDER_NORMAL;

	LPD3DXMESH			m_pMesh = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture[(_uint)E_WIRE_RENDER::WIRE_RENDER_END] = {};

	CShader*			m_pShaderCom = nullptr;
};

#endif // __CamControl_Eye_H__
