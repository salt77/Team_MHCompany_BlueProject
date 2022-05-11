#include "EMesh.h"

CEMesh::CEMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pOriMesh(nullptr)
	, m_pMesh(nullptr)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_dwStride(0)
	, m_pVtxPos(nullptr)
	, m_dwVtxCnt(0)
{

}

CEMesh::CEMesh(const CEMesh & rhs)
	: CComponent(rhs)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwStride(rhs.m_dwStride)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
{
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();
}

CEMesh::~CEMesh()
{
}

HRESULT CEMesh::Ready_Meshes(const wstring& pFilePath)
{
	if (FAILED(D3DXLoadMeshFromX(pFilePath.c_str(), D3DXMESH_MANAGED, m_pGraphicDev,
		&m_pAdjacency,
		&m_pSubset,
		NULL,
		NULL,
		&m_pOriMesh)))
		return E_FAIL;

	if (FAILED(m_pOriMesh->CloneMesh(m_pOriMesh->GetOptions(), vertexDecl, m_pGraphicDev, &m_pMesh)))
	{
		return E_FAIL;
	}
	
	D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	D3DXComputeTangent(m_pMesh, 0, 0, 0, 0, (_ulong*)m_pAdjacency->GetBufferPointer());

	m_pGraphicDev->CreateVertexDeclaration(vertexDecl, &m_pVertexDeclaration);

	_ulong  dwFVF = m_pOriMesh->GetFVF();
	
	m_dwVtxCnt = m_pMesh->GetNumVertices();  // 메쉬가 지닌 정점의 개수를 반환
	m_pVtxPos = new _vec3[m_dwVtxCnt];

	void *		pVertex = nullptr;

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte			byOffSet = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = (_ubyte)Decl[i].Offset;
			break;
		}
	}
	// FVF 정보를 토대로 정점의 크기를 계산해주는 함수
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffSet)));
	}
	
	return S_OK;
}

void CEMesh::Render_Meshes()
{
	m_pGraphicDev->SetVertexDeclaration(m_pVertexDeclaration);

	m_pMesh->DrawSubset(0);
}

void CEMesh::Save(HANDLE hFile)
{
}

void CEMesh::Load(HANDLE hFile)
{
}

CEMesh * CEMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pFilePath)
{
	CEMesh*		pInstance = new CEMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CEMesh::Clone()
{
	return new CEMesh(*this);
}

void CEMesh::Free()
{
	if (FALSE == m_bClone)
	{
		Safe_Delete_Array(m_pVtxPos);
	}

	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pVertexDeclaration);

	CComponent::Free();
}
