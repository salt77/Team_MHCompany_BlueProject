#pragma once
#ifndef __STATICMESH_H__
#define __STATICMESH_H__

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

// 파일 가져온 사람 : 김민현

class ENGINE_DLL CSMesh : public CComponent
{
private:
	explicit CSMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSMesh(const CSMesh& rhs);
	virtual ~CSMesh();

	virtual void	Free();

public:
	virtual		CComponent*			Clone();
	virtual	void					Save(HANDLE hFile);
	virtual void					Load(HANDLE hFile);
	static		CSMesh*				Create(LPDIRECT3DDEVICE9 pGraphicDev,
											const wstring&	pFilePath,
											const wstring&	pFileName);

public:
	HRESULT							Ready_Meshes(const wstring& pFilePath, const wstring& pFileName);
	void							Render_Meshes(CShader* pShaderCom);
	void							Render_Meshes_Instance(CShader* pShaderCom);

	void							Set_Glow_Option(_int iContainerIndex, _float bBaseGlow, _vec3 vTexGlowRGB, _float bGlowPower, _float fGlow_U_Min, _float fGlow_U_Max, _float fGlow_V_Min, _float fGlow_V_Max, _vec3 vGlowColor = _vec3(1.f, 1.f, 1.f));
	void							Set_Glow_Option(_int iContainerIndex, T_GlowOption tGlowOption);

	// SMesh는 래퍼런스로 바로 변경 가능 / Dmesh 와 차이가 있음
	T_GlowOption&					Get_Glow_Option(_int iContainerIndex);

public:
	_vec3*							Get_VtxPos() { return m_pVtxPos; }
	const _ulong&					Get_Stride() { return m_dwStride; }
	const _ulong&					Get_VtxCnt() { return m_dwVtxCnt; }
	const LPD3DXMESH&				Get_Mesh() { return m_pMesh; }

	_bool							PickingMesh(HWND hWnd,_vec3* pOutPos,_float* fOutDis,_matrix _matWorld);
	_bool							PickingMesh(_vec3 vWorldRayStart, _vec3 vWorldRayDir, _vec3* pOutPos, _float* fOutDis, _matrix _matWorld);
protected:
	LPD3DXMESH						m_pMesh;				// 노말정보를 삽입한 메쉬 정보를 담고 있는 객체
	LPD3DXMESH						m_pOriMesh;				// 최초 로드 시점의 메쉬 정보를 담고 있는 객체

	LPD3DXBUFFER					m_pAdjacency;			// 메쉬가 지닌 정점을 근거로 이뤄진 폴리곤 정보를 통해 법선을 생성하기 위한 값
	LPD3DXBUFFER					m_pSubset;				// 메쉬를 구성하는 파츠격에 해당하는 정보
															// 서브셋의 개수 == 재질의 개수 == 텍스처의 개수
	D3DXMATERIAL*					m_pMtrl;				// 텍스처의 이름과 재질의 색상 정보를 보관하기 위한 구조체

	_ulong							m_dwSubsetCnt;			// 서브셋의 개수
	vector<T_GlowOption>			m_vector_GlowOption;

	LPDIRECT3DTEXTURE9*				m_ppTexture;			// 여러장의 메쉬 텍스처를 동적 배열의 형태로 보관하기 위한 포인터
	LPDIRECT3DTEXTURE9*				m_ppNormalTexture;
	LPDIRECT3DTEXTURE9*				m_ppSpecularTexture;
	LPDIRECT3DTEXTURE9*				m_ppMaskTexture;

	_vec3*							m_pVtxPos;				// 메쉬가 지닌 정점의 위치값들을 동적 할당(배열)한 메모리 공간에 보관하기 위한 포인터
	_ulong							m_dwStride;				// 정점의 크기를 저장하기 위한 변수
	_ulong							m_dwVtxCnt;				// 메쉬가 지닌 정점의 개수를 저장

	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;	// Normal Mapping을 위한 Declaration

	LPD3DXATTRIBUTERANGE m_pAttributeTable = nullptr; // 메시의 속성 테이블 구조체.
	DWORD m_dwNumAttributeGroups; // 속성 갯수.
};

END

#endif // !__STATICMESH_H__