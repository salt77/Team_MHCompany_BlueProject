#pragma once
#ifndef __HIERARCHYLOADER_H__
#define __HIERARCHYLOADER_H__

#include "Engine_Define.h"

BEGIN(Engine)

// ÆÄÀÏ °¡Á®¿Â »ç¶÷ : ±è¹ÎÇö

class CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
	virtual ~CHierarchyLoader();

public:
	static CHierarchyLoader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void	Allocate_Name(char** ppName, const char* pFrameName);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	const _tchar*			m_pPath;

public:
	_ulong	Release();

};

END


#endif // !__HIERARACHYLOADER_H__