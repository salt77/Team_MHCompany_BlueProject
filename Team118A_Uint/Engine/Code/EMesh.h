#pragma once
#ifndef __EFFECTMESH_H__
#define __EFFECTMESH_H__

#include "Component.h"
#include "Shader.h"

BEGIN(Engine)

// ���� : ��ȣ

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
	LPD3DXMESH						m_pMesh;				// �븻������ ������ �޽� ������ ��� �ִ� ��ü
	LPD3DXMESH						m_pOriMesh;				// ���� �ε� ������ �޽� ������ ��� �ִ� ��ü

	LPD3DXBUFFER					m_pAdjacency;			// �޽��� ���� ������ �ٰŷ� �̷��� ������ ������ ���� ������ �����ϱ� ���� ��
	LPD3DXBUFFER					m_pSubset;				// �޽��� �����ϴ� �����ݿ� �ش��ϴ� ����
															// ������� ���� == ������ ���� == �ؽ�ó�� ����
	
	_vec3*							m_pVtxPos;				// �޽��� ���� ������ ��ġ������ ���� �Ҵ�(�迭)�� �޸� ������ �����ϱ� ���� ������
	_ulong							m_dwStride;				// ������ ũ�⸦ �����ϱ� ���� ����
	_ulong							m_dwVtxCnt;				// �޽��� ���� ������ ������ ����

	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;	// Normal Mapping�� ���� Declaration


	//vector<IDirect3DBaseTexture9*>	m_vecTexture;
};

END

#endif // !__EFFECTMESH_H__