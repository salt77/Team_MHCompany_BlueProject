#ifndef __STATIC_OBJECT_H__
#define __STATIC_OBJECT_H__

#include "Base.h"
#include "Engine_Define.h"
#include "GameObject.h"
#include "WorldCreator.h"
USING(Engine)

class ENGINE_DLL CStaticObject : public CGameObject
{
public:
	// 거리에 따라서 Sort하기 위한 Compare함수
	static _bool CompareByDistance(CStaticObject* pSrc, CStaticObject* pDest)
	{
		if (pSrc->m_fDistanceForSort < pDest->m_fDistanceForSort)
			return TRUE;
		return FALSE;
	}

protected:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath);
	explicit CStaticObject(const CStaticObject& rhs);
	virtual ~CStaticObject();

public:
	static CStaticObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath);

	virtual void			Free();
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT			Ready_Object() override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass) override;

public:
	virtual void			On_Collision(CGameObject* pDest) override;

public:
	virtual void					Set_DistanceForSort(_float fDistance)				{ m_fDistanceForSort = fDistance;}
public:

	
protected:
	virtual void			Add_Component();
	virtual void			Set_Constantable() override;
	virtual void			Set_SMesh();
protected:
	_float				m_fDistanceForSort;
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

protected:
	wstring				m_wstrMeshFullPath = L"";
	wstring				m_wstrMeshName = L"";
	wstring				m_wstrMeshPath = L"";
};

#endif // __Obstacle_H__
