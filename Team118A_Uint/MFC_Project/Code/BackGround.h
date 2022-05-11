#ifndef __BackGround_H__
#define __BackGround_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"

USING(Engine)

class CBackGround : public CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround();

public:
	static CBackGround*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;
	
private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	CRectBuffer*		m_pBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;	
	CSphereCollider*	m_pSphereColliderCom = nullptr;
};

#endif // __BackGround_H__
