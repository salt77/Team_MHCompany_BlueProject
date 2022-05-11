#ifndef __BACK_BOARD_H__
#define __BACK_BOARD_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"
#include "RectBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBackBoard : public CGameObject
{
private:
	explicit CBackBoard(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrTextureFullPath);
	explicit CBackBoard(const CBackBoard& rhs);
	virtual ~CBackBoard();

public:
	static CBackBoard*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrTextureFullPath);
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
	CRectBuffer*		m_pBufferCom	= nullptr;
	CShader*			m_pShaderCom	= nullptr;
	LPDIRECT3DTEXTURE9  m_pTexture		= nullptr;
	wstring				m_wstrTextureFullPath;
};

END

#endif // __BackGround_H__
