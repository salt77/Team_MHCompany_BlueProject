#ifndef __SMESH_TEST_H__
#define __SMESH_TEST_H__

#include "Engine_Define.h"
#include "Base.h"

#include "GameObject.h"

USING(Engine)

class CSMesh_Test : public CGameObject
{
private:
	explicit CSMesh_Test(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CSMesh_Test(const CSMesh_Test& rhs);
	virtual ~CSMesh_Test();

public:
	static CSMesh_Test*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free();

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;

	virtual void		Debug_This_In_Console() override;

private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
};

#endif // __SMESH_TEST_H__
