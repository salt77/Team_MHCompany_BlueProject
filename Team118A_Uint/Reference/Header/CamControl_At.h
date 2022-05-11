#ifndef __CamControl_At_H__
#define __CamControl_At_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"

USING(Engine)

class CCamControl_At final : public CGameObject
{
private:
	explicit CCamControl_At(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CCamControl_At(const CCamControl_At& rhs);
	virtual ~CCamControl_At();

public:
	static CCamControl_At*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _float fScale = 2.5f);
	virtual void				Free();
	virtual void				Save(HANDLE hFile, DWORD& dwByte);
	virtual void				Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT		Ready_Object(_vec3 vPos, _float fScale);
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;

public:
	void				Set_RenderType(E_WIRE_RENDER eRenderType) { m_eRenderType = eRenderType; }
	
private:
	void				Add_Component();
	virtual void		Set_Constantable() override; 

private:
	_float				m_fScale = 0.f;

	E_WIRE_RENDER		m_eRenderType = E_WIRE_RENDER::WIRE_RENDER_NORMAL;

	LPD3DXMESH			m_pMesh = nullptr;
	LPDIRECT3DTEXTURE9	m_pTexture[(_uint)E_WIRE_RENDER::WIRE_RENDER_END] = {};

	CShader*			m_pShaderCom = nullptr;
};

#endif // __CamControl_At_H__
