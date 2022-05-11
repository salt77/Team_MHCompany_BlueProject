#ifndef __MFC_COMBAT_OBSTACLE_H__
#define __MFC_COMBAT_OBSTACLE_H__

#include "Base.h"
#include "Engine_Define.h"

#include "Obstacle.h"

USING(Engine)

class ENGINE_DLL CObstacle_Combat : public CObstacle
{
private:
	explicit CObstacle_Combat(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CObstacle_Combat(const CObstacle_Combat& rhs);
	virtual ~CObstacle_Combat();

public:
	static CObstacle_Combat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	static CObstacle_Combat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring pMeshFullPath, wstring pMeshTag);
	virtual void				Free();

public:
	virtual void		Save(HANDLE hFile, DWORD& dwByte);
	virtual void		Load(HANDLE hFile, DWORD& dwByte);
	
public:
	virtual HRESULT		Ready_Object()								override;
	virtual _int		Update_Object(const _float& fTimeDelta)		override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass)					override;
public:
	void				Bid_Obstacle(CGameObject* pObject);
	void				Check_Obstacle();
	
	_vec3				Get_HideSpotPos(CGameObject* pHideObject);
	void				CutHideObject();
	void				DeleteHideObject();

private:
	void                ChoiceBidObject();

	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	vector<CGameObject*>	m_vecBidObject;
	CGameObject*			m_pHideObject = nullptr;
	
};

#endif // __Obstacle_H__
