#ifndef __DMESH_TEST_H__
#define __DMESH_TEST_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Unit.h"

USING(Engine)

class CDMesh_Test : public CUnit
{
private:
	explicit CDMesh_Test(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CDMesh_Test(const CDMesh_Test& rhs);
	virtual ~CDMesh_Test();

public:
	static CDMesh_Test*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	virtual void		Free()override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void		Load(HANDLE hFile, DWORD& dwByte) override;

public:
	virtual HRESULT		Ready_Object() override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;

public:
	void				Set_AnimForMFC(_int iIndex);
	void				Set_DMesh(wstring wstrFullPath, wstring wstrName);
	CDMesh*				Get_DMesh() { return m_pDMeshCom; }
	_bool				Find_TestMesh();
	void				Choose_ActionForTest(wstring wstrActionName);
	void				Delete_T_AnimTrgByIndex(_int iAnimIndex, _int iIndex);
	void				Delete_T_AnimActionSysByName(wstring wstrName);
	
private:

public:
	vector<pair<E_Trigger, std::function<void()>>>*	Get_vecTrg() { return &m_vecFunction_Trg; }
	vector<list<T_AnimTrg>>*						Get_AnimTrg() { return &m_vecAnimTrg; }
	vector<T_AnimActionSys>*						Get_AactionVec() { return &m_vecAnimActions; }
	void		Add_AnimAction(wstring wstrActionName, _int iAnimIdx);
	void		Clear_Action();

	void		Save_AnimTrigger(HANDLE hFile, DWORD& dwByte);
	void		Save_AnimAction(HANDLE hFile, DWORD& dwByte);
	void		Load_AnimTrigger(HANDLE hFile, DWORD& dwByte);
	void		Load_AnimAction(HANDLE hFile, DWORD& dwByte);

public:
	CTexture*				m_pTexMouse = nullptr;		// 입을 그리기 위한 텍스쳐

	_bool		m_bAction_OnOff = FALSE;
	E_CharacterMouseTag		m_eMouse = E_CharacterMouseTag::MOUSE_END;

};

#endif // __DMESH_TEST_H__
