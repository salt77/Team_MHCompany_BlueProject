#pragma once
#ifndef __EFFECTMESH_H__
#define __EFFECTMESH_H__

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

// 제작 : 은호

class ENGINE_DLL CEMesh : public CComponent
{
private:
	explicit CEMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEMesh(const CEMesh& rhs);
	virtual ~CEMesh();

	virtual void	Free();

public:
	virtual		CComponent*			Clone();
	virtual	void					Save(HANDLE hFile);
	virtual void					Load(HANDLE hFile);
	static		CEMesh*				Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring&	pFilePath);

public:
	HRESULT							Ready_Meshes(const wstring& pFilePath);
	void							Render_Meshes();

public:
	_vec3*							Get_VtxPos() { return m_pVtxPos; }
	const _ulong&					Get_Stride() { return m_dwStride; }
	const _ulong&					Get_VtxCnt() { return m_dwVtxCnt; }
	const LPD3DXMESH&				Get_Mesh() { return m_pMesh; }

protected:
	LPD3DXMESH						m_pMesh;				// 노말정보를 삽입한 메쉬 정보를 담고 있는 객체
	LPD3DXMESH						m_pOriMesh;				// 최초 로드 시점의 메쉬 정보를 담고 있는 객체

	LPD3DXBUFFER					m_pAdjacency;			// 메쉬가 지닌 정점을 근거로 이뤄진 폴리곤 정보를 통해 법선을 생성하기 위한 값
	LPD3DXBUFFER					m_pSubset;				// 메쉬를 구성하는 파츠격에 해당하는 정보
															// 서브셋의 개수 == 재질의 개수 == 텍스처의 개수
	
	_vec3*							m_pVtxPos;				// 메쉬가 지닌 정점의 위치값들을 동적 할당(배열)한 메모리 공간에 보관하기 위한 포인터
	_ulong							m_dwStride;				// 정점의 크기를 저장하기 위한 변수
	_ulong							m_dwVtxCnt;				// 메쉬가 지닌 정점의 개수를 저장

	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;	// Normal Mapping을 위한 Declaration


	//vector<IDirect3DBaseTexture9*>	m_vecTexture;
};

END

#endif // !__EFFECTMESH_H__