#ifndef __KURO_GROUND_H__
#define __KURO_GROUND_H__

#include "Base.h"
#include "Engine_Define.h"
#include "GameObject.h"

#include "StaticObject.h"
USING(Engine)

class ENGINE_DLL CKuroGround : public CStaticObject
{
public:
	// 거리에 따라서 Sort하기 위한 Compare함수
	static _bool CompareByDistance(CKuroGround* pSrc, CKuroGround* pDest)
	{
		if (pSrc->m_fDistanceForSort < pDest->m_fDistanceForSort)
			return TRUE;
		return FALSE;
	}

protected:
	explicit CKuroGround(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath);
	explicit CKuroGround(const CKuroGround& rhs);
	virtual ~CKuroGround();

public:
	static CKuroGround*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath);

	virtual void			Free();
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	
	virtual _int			Update_Object(const _float& fTimeDelta) override;

	void					Set_GroundDead();
public:
	virtual void			On_Collision(CGameObject* pDest) override;
private:
	void					Update_Gravity(_float fTimeDelta);
protected:
	_float				m_fDistanceForSort;
	CSMesh*				m_pSMeshCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

protected:
	wstring				m_wstrMeshFullPath = L"";
	wstring				m_wstrMeshName = L"";
	wstring				m_wstrMeshPath = L"";

	_float				m_fGravityAcc = 0.2f;
	_float				m_fGravitySpeed = 0.f;
	_bool				m_bGravityOn = FALSE;
};

#endif // __Obstacle_H__
