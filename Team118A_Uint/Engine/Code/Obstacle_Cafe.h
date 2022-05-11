#ifndef __Obstacle_Cafe_H__
#define __Obstacle_Cafe_H__

#include "Base.h"
#include "Engine_Define.h"
#include "Obstacle.h"

#include "SphereCollider.h"

USING(Engine)

class ENGINE_DLL CObstacle_Cafe : public CObstacle
{
protected:
	explicit CObstacle_Cafe(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CObstacle_Cafe(const CObstacle_Cafe& rhs);
	virtual ~CObstacle_Cafe();

public:
	static CObstacle_Cafe*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	static CObstacle_Cafe*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag);
	virtual void			Free();

public:
	virtual const wstring	Get_Name() { return m_wstrMeshName; }
	_bool					Get_SelRender() { return m_bSelRender; }
	_vec3					Get_ShperePos();
	_float					Get_Radius() { return m_pSphereCollider->GetRadius(); }

public:
	void					Set_SelRender(_bool pSelRender) { m_bSelRender = pSelRender; }
	void					Set_DistanceForSort(_float fDistance) { m_fDistanceForSort = fDistance; }
	virtual void			Set_Constantable() override;
	void					Set_CreateNew(_bool NewFur) { m_bCreateNew = NewFur; }

public:
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT			Ready_Object() override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass) override;

public:
	static _bool			CompareByDistance(CObstacle_Cafe* pSrc, CObstacle_Cafe* pDest);

private:
	void					Add_Component();

protected:
	CSMesh*					m_pSMeshCom = nullptr;
	CShader*				m_pShaderCom = nullptr;
	CSphereCollider*		m_pSphereCollider = nullptr;

protected:
	wstring					m_wstrMeshFullPath;
	wstring					m_wstrMeshName;

protected:
	_bool					m_bSelRender = false;
	_float					m_fRenderTime = 0.f;
	_float					m_fDistanceForSort = 0.f;

private:
	_float					m_fCreate_Up_Time = 0.f;
	_float					m_fvecY = -1.f;
	_bool					m_bCreateNew = false;
	
private:
	_float					m_fCreateFurShade = 0.f;
	_float					m_fCreateShaderFilter = 0.f;
	_bool					m_bCreateShaderSetting = true;
};

#endif // __Obstacle_Cafe_H__
