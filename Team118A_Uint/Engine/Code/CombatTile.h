#ifndef __COMBAT_TILE_H__
#define __COMBAT_TILE_H__

#include "Base.h"
#include "Engine_Define.h"
#include "GameObject.h"
#include "WorldCreator.h"
USING(Engine)

class ENGINE_DLL CCombatTile : public CGameObject
{
public:
	// 거리에 따라서 Sort하기 위한 Compare함수
	static _bool CompareByDistance(CCombatTile* pSrc, CCombatTile* pDest)
	{
		if (pSrc->m_fDistanceForSort < pDest->m_fDistanceForSort)
			return TRUE;
		return FALSE;
	}

protected:
	explicit CCombatTile(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_TILE_OPTION eTileOption);
	explicit CCombatTile(const CCombatTile& rhs);
	virtual ~CCombatTile();

public:
	static CCombatTile*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_TILE_OPTION eTileOption);

	virtual void			Free();
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

public:
	virtual HRESULT			Ready_Object() override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass) override;
	virtual void			Set_Constantable() override;

	void					Set_TileActive(_bool bActive);
	_bool					Get_TileActive() { return m_bTileActive; }
public:
	virtual void			On_Collision(CGameObject* pDest) override;

public:
	virtual const wstring	Get_Name() { return m_wstrMeshName; }

	void					Set_DistanceForSort(_float fDistance) { m_fDistanceForSort = fDistance; }
public:
	E_TILE_OPTION			Get_TileOption() { return m_eTileOption; }
	void					Set_TileOption(E_TILE_OPTION eTileOption) { m_eTileOption = eTileOption; }

	_int					Get_LinkTileIndex() { return m_iLinkTileIndex; }
	void					Set_LinkTileIndex(_int iLinkTileIndex) { m_iLinkTileIndex = iLinkTileIndex; }

	const vector<_int>&		Get_NearTileIndex() { return m_vecNearTileIndex; }
	void					Set_NearTileIndex(vector<_int> vecNearTileIndex) { m_vecNearTileIndex = vecNearTileIndex; }

	_bool					Get_Used() { return m_bUsed; }
	void					Set_Used(_bool bUsed) { m_bUsed = bUsed; }
private:
	void					Add_Component();
	void					Set_TileSMesh();

	void					Update_TileActive(_float fTimeDelta);
protected:
	_bool				m_bRenderAlpha = FALSE;
	_bool				m_bTileActive = TRUE;

	CSMesh*				m_pSMeshCom = nullptr;
	CSMesh*				m_pSMeshCap = nullptr;

	CShader*			m_pShaderCom = nullptr;

protected:
	CGameObject*		m_pWarpParticle = nullptr;

	_bool               m_bUsed = FALSE;
	vector<_int>		m_vecNearTileIndex;

	wstring				m_wstrMeshFullPath;
	wstring				m_wstrMeshName;
	_float				m_fDistanceForSort = 0.f;

	E_TILE_OPTION		m_eTileOption = E_TILE_OPTION::TILE_EMPTY;

	_int				m_iLinkTileIndex = -1;

	CWorldCreator*		m_pWorldCreater = nullptr;

	_float				m_fAlphaRatio = 0.f;
	


};

#endif // __Obstacle_H__
