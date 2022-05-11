#ifndef __START_MARK_H__
#define __START_MARK_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"

USING(Engine)

class CStartMark : public CGameObject
{
private:
	explicit CStartMark(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CStartMark(const CStartMark& rhs);
	virtual ~CStartMark();

public:
	static CStartMark*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag,_int iNumber);
	virtual void		Free();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;
	
	_int				Get_Number();
	void				Set_Number(_int iNumber);
private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	_int m_iNumber;

	CRectBuffer*		m_pBufferCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	LPDIRECT3DTEXTURE9  m_pTexture = nullptr;
	CSphereCollider*	m_pSphereColliderCom = nullptr;

};

#endif // __BackGround_H__
