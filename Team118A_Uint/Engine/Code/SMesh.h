#pragma once
#ifndef __STATICMESH_H__
#define __STATICMESH_H__

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

// ���� ������ ��� : �����

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

	// SMesh�� ���۷����� �ٷ� ���� ���� / Dmesh �� ���̰� ����
	T_GlowOption&					Get_Glow_Option(_int iContainerIndex);

public:
	_vec3*							Get_VtxPos() { return m_pVtxPos; }
	const _ulong&					Get_Stride() { return m_dwStride; }
	const _ulong&					Get_VtxCnt() { return m_dwVtxCnt; }
	const LPD3DXMESH&				Get_Mesh() { return m_pMesh; }

	_bool							PickingMesh(HWND hWnd,_vec3* pOutPos,_float* fOutDis,_matrix _matWorld);
	_bool							PickingMesh(_vec3 vWorldRayStart, _vec3 vWorldRayDir, _vec3* pOutPos, _float* fOutDis, _matrix _matWorld);
protected:
	LPD3DXMESH						m_pMesh;				// �븻������ ������ �޽� ������ ��� �ִ� ��ü
	LPD3DXMESH						m_pOriMesh;				// ���� �ε� ������ �޽� ������ ��� �ִ� ��ü

	LPD3DXBUFFER					m_pAdjacency;			// �޽��� ���� ������ �ٰŷ� �̷��� ������ ������ ���� ������ �����ϱ� ���� ��
	LPD3DXBUFFER					m_pSubset;				// �޽��� �����ϴ� �����ݿ� �ش��ϴ� ����
															// ������� ���� == ������ ���� == �ؽ�ó�� ����
	D3DXMATERIAL*					m_pMtrl;				// �ؽ�ó�� �̸��� ������ ���� ������ �����ϱ� ���� ����ü

	_ulong							m_dwSubsetCnt;			// ������� ����
	vector<T_GlowOption>			m_vector_GlowOption;

	LPDIRECT3DTEXTURE9*				m_ppTexture;			// �������� �޽� �ؽ�ó�� ���� �迭�� ���·� �����ϱ� ���� ������
	LPDIRECT3DTEXTURE9*				m_ppNormalTexture;
	LPDIRECT3DTEXTURE9*				m_ppSpecularTexture;
	LPDIRECT3DTEXTURE9*				m_ppMaskTexture;

	_vec3*							m_pVtxPos;				// �޽��� ���� ������ ��ġ������ ���� �Ҵ�(�迭)�� �޸� ������ �����ϱ� ���� ������
	_ulong							m_dwStride;				// ������ ũ�⸦ �����ϱ� ���� ����
	_ulong							m_dwVtxCnt;				// �޽��� ���� ������ ������ ����

	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;	// Normal Mapping�� ���� Declaration

	LPD3DXATTRIBUTERANGE m_pAttributeTable = nullptr; // �޽��� �Ӽ� ���̺� ����ü.
	DWORD m_dwNumAttributeGroups; // �Ӽ� ����.
};

END

#endif // !__STATICMESH_H__