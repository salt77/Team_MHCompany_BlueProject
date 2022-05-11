#ifndef __Obstacle_H__
#define __Obstacle_H__

#include "Base.h"
#include "Engine_Define.h"
#include "GameObject.h"

#include "SphereCollider.h"
#include "Renderer.h"
#include "SMesh.h"
#include "ProtoMgr.h"

USING(Engine)

class ENGINE_DLL CObstacle : public CGameObject
{
public:
	// 거리에 따라서 Sort하기 위한 Compare함수
	static _bool CompareByDistance(CObstacle* pSrc, CObstacle* pDest)
	{
		if (pSrc->m_fDistanceForSort < pDest->m_fDistanceForSort)
			return TRUE;
		return FALSE;
	}

protected:
	explicit CObstacle(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CObstacle(const CObstacle& rhs);
	virtual ~CObstacle();

public:
	static CObstacle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	static CObstacle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag);
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
	virtual const wstring	Get_Name()				{ return m_wstrMeshName;															}
	_vec3					Get_ShperePos()			{ return _vec3(m_pSphereCollider->GetPos().x, 0.f, m_pSphereCollider->GetPos().z);	}
	_float					Get_Radius()			{ return m_pSphereCollider->GetRadius();											}

	void					Set_DistanceForSort(_float fDistance)	{ m_fDistanceForSort = fDistance;									}

private:
	void					Add_Component();
	virtual void			Set_Constantable() override;

protected:
	CSphereCollider*    m_pSphereCollider = nullptr;
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

protected:
	wstring				m_wstrMeshFullPath;
	wstring				m_wstrMeshName;
	_float				m_fDistanceForSort = 0.f;
};

#endif // __Obstacle_H__
