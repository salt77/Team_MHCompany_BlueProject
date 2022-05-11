#pragma once
#ifndef __Map_Test_Eunho_H__
#define __Map_Test_Eunho_H__

#include "GameObject.h"

USING(Engine)

class CMap_Test_Eunho : public CGameObject
{
private:
	explicit CMap_Test_Eunho(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CMap_Test_Eunho(const CMap_Test_Eunho& rhs);
	virtual ~CMap_Test_Eunho();

public:
	static CMap_Test_Eunho*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object() override;
	virtual void		Render_Shadow() override;
	
private:
	void				Add_Component();
	virtual void		Set_Constantable() override;
	void				Set_Constantable_Fog();

private:
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
};

#endif // __Map_Test_Eunho_H__
