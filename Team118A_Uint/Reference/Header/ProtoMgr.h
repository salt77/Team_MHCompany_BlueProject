#pragma once
#ifndef __ProtoMgr_H__
#define __ProtoMgr_H__

#include "Engine_Define.h"
#include "Base.h"

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	CProtoMgr();
	virtual ~CProtoMgr();

public:
	virtual void    Free();

public:
	HRESULT			Ready_Prototype(const wstring pProtoTag, CComponent* pInstance);
	CComponent*		Clone_Prototype(const wstring pProtoTag);
	CComponent*		Find_Prototype(const wstring pProtoTag);

	HRESULT			Delete_Prototype(const wstring pProtoTag);

public:
	HRESULT			Get_Texture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFullPath, LPDIRECT3DTEXTURE9* ppOut);
	D3DXIMAGE_INFO	Get_TextureInfo(wstring wstrFullPath);

private:
	map<const wstring, LPDIRECT3DTEXTURE9>	m_mapMeshTexture;
	map<const wstring, D3DXIMAGE_INFO>		m_mapTextureInfo;
private:
	map<const wstring, CComponent*>        m_mapPrototype;

};

END

#endif // !__ProtoMgr_H__