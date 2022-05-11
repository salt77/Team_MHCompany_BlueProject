#include "SphereTex2.h"

USING(Engine)

Engine::CSphereTex2::CSphereTex2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CSphereTex2::CSphereTex2(const CSphereTex2& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CSphereTex2::~CSphereTex2(void)
{

}

HRESULT Engine::CSphereTex2::Ready_Buffer(void)
{
	//m_dwVtxCnt = 8;
	//m_dwTriCnt = 12;
	m_dwVtxSize = sizeof(VTX_SPHERE);
	m_dwFVF = FVF_SPHERE;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	_float	radius = 10.f;
	_float	bottomRadius = 0.5f;
	_float	height = 3.0f;
	_uint	sliceCount = 128;
	_uint	stackCount = 128;

	//FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	vector<VTX_SPHERE> vecVertices;

	VTX_SPHERE topVertex;
	topVertex.vPosition = _vec3(0.0f, radius, 0.0f);
	topVertex.vTexUV = _vec2(0.0f, 0.0f);

	VTX_SPHERE bottomVertex;
	bottomVertex.vPosition = _vec3(0.0f, -radius, 0.0f);
	bottomVertex.vTexUV = _vec2(0.0f, 1.0f);

	vecVertices.push_back(topVertex);
	float phiStep = D3DX_PI / stackCount;
	float thetaStep = 2.0f * D3DX_PI / sliceCount;

	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			VTX_SPHERE v;

			// spherical to cartesian
			v.vPosition.x = radius * sinf(phi) * cosf(theta);
			v.vPosition.y = radius * cosf(phi);
			v.vPosition.z = radius * sinf(phi) * sinf(theta);

			v.vTexUV.x = theta / (2 * D3DX_PI);
			v.vTexUV.y = phi / D3DX_PI;

			vecVertices.push_back(v);
		}
	}
	vecVertices.push_back(bottomVertex);
	//----------

	vector<INDEX16> vecIndices;

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		INDEX16 idx;
		idx._0 = 0;
		idx._1 = i + 1;
		idx._2 = i;

		vecIndices.push_back(idx);
	}

	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			INDEX16 idx;
			idx._0 = baseIndex + i * ringVertexCount + j;
			idx._1 = baseIndex + i * ringVertexCount + j + 1;
			idx._2 = baseIndex + (i + 1) * ringVertexCount + j;
			vecIndices.push_back(idx);

			idx._0 = baseIndex + (i + 1) * ringVertexCount + j;
			idx._1 = baseIndex + i * ringVertexCount + j + 1;
			idx._2 = baseIndex + (i + 1) * ringVertexCount + j + 1;
			vecIndices.push_back(idx);
		}
	}

	UINT southPoleIndex = (UINT)vecVertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		INDEX16 idx;
		idx._0 = southPoleIndex;
		idx._1 = baseIndex + i;
		idx._2 = baseIndex + i + 1;
		vecIndices.push_back(idx);
	}

	m_dwVtxCnt = (_ulong)vecVertices.size();
	m_dwTriCnt = (_ulong)vecIndices.size();

	CVIBuffer::Ready_Buffer();

	VTX_SPHERE* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	for (size_t i = 0; i < vecVertices.size(); ++i)
		pVertices[i] = vecVertices[i];

	//memcpy(m_pVertices, pVertices, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();

	INDEX16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	for (size_t i = 0; i < vecIndices.size(); ++i)
		pIndices[i] = vecIndices[i];

	//memcpy(m_pIndices, pIndices, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();

	return S_OK;
}

void Engine::CSphereTex2::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

void Engine::CSphereTex2::Free(void)
{
	CVIBuffer::Free();
}

CSphereTex2* Engine::CSphereTex2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereTex2*	pInstance = new CSphereTex2(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CSphereTex2::Clone(void)
{
	return new CSphereTex2(*this);
}

