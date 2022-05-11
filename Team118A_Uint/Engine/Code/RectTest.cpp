#include "RectTest.h"



CRectTest::CRectTest(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	m_wstrComTag = ComTag_RectTest;
}

CRectTest::CRectTest(const CRectTest& rhs)
	:CVIBuffer(rhs)
{

}

CRectTest::~CRectTest()
{
}

CComponent * CRectTest::Clone()
{
	return new CRectTest(*this);
}

void CRectTest::Free()
{
	CVIBuffer::Free();
}

CRectTest * CRectTest::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRectTest*	pInstance = new CRectTest(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CRectTest::Ready_Buffer()
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEST);
	m_dwFVF = FVF_Test;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEST*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vNormal = _vec3(0.f, 0.f, -1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vNormal = _vec3(0.f, 0.f, -1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vNormal = _vec3(0.f, 0.f, -1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vNormal = _vec3(0.f, 0.f, -1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	memcpy(m_pVertices, pVertex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	memcpy(m_pIndices, pIndex, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();

	return S_OK;
}

void CRectTest::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}