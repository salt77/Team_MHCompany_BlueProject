#include "CrossRectBuffer.h"



CCrossRectBuffer::CCrossRectBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
	m_wstrComTag = ComTag_RectBuffer;
}

CCrossRectBuffer::CCrossRectBuffer(const CCrossRectBuffer& rhs)
	:CVIBuffer(rhs)
{

}

CCrossRectBuffer::~CCrossRectBuffer()
{
}

CComponent * CCrossRectBuffer::Clone()
{
	return new CCrossRectBuffer(*this);
}

void CCrossRectBuffer::Free()
{
	CVIBuffer::Free();
}

CCrossRectBuffer * CCrossRectBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossRectBuffer*	pInstance = new CCrossRectBuffer(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

HRESULT CCrossRectBuffer::Ready_Buffer()
{
	m_dwTriCnt = 4;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXUI);
	m_dwFVF = FVF_UI;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXUI*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	pVertex[4].vPosition = _vec3(-0.5f, 0.f, 0.5f);
	pVertex[4].vTexUV = _vec2(0.f, 0.f);

	pVertex[5].vPosition = _vec3(0.5f, 0.f, 0.5f);
	pVertex[5].vTexUV = _vec2(1.f, 0.f);

	pVertex[6].vPosition = _vec3(0.5f, 0.f, -0.5f);
	pVertex[6].vTexUV = _vec2(1.f, 1.f);

	pVertex[7].vPosition = _vec3(-0.5f, 0.f, -0.5f);
	pVertex[7].vTexUV = _vec2(0.f, 1.f);

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

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 5;
	pIndex[2]._2 = 6;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 6;
	pIndex[3]._2 = 7;

	memcpy(m_pIndices, pIndex, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();

	return S_OK;
}

void CCrossRectBuffer::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

_bool CCrossRectBuffer::Picking_Buffer(HWND hwnd, _vec3 * pOutPos, _float * pOutDist, _matrix _matWorld)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	_matrix matWorld = _matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_vec3 v1, v2, v3;
	ZeroMemory(&v1, sizeof(_vec3));
	ZeroMemory(&v2, sizeof(_vec3));
	ZeroMemory(&v3, sizeof(_vec3));

	
	float fU = 0.f, fV = 0.f;

	
	int iTriCount = (_int)m_dwTriCnt;

	void* pIndices;
	void* pVertices;

	m_pVB->Lock(0, 0, &pVertices, 0);
	m_pIB->Lock(0, 0, &pIndices, 0);


	_float fDist = 0.f;


	for (int i = 0; i < iTriCount; ++i)
	{
		WORD _1, _2, _3;
		memcpy(&_1, (char*)pIndices + i * sizeof(INDEX16), sizeof(WORD));
		memcpy(&_2, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD), sizeof(WORD));
		memcpy(&_3, (char*)pIndices + i * sizeof(INDEX16) + sizeof(WORD) * 2, sizeof(WORD));

		memcpy(&v1, (VTXUI*)pVertices + _1, sizeof(_vec3));
		memcpy(&v2, (VTXUI*)pVertices + _2, sizeof(_vec3));
		memcpy(&v3, (VTXUI*)pVertices + _3, sizeof(_vec3));

		if (D3DXIntersectTri(&v1, &v2, &v3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			_vec3 vOut;

			vOut = v1 + (fU * (v2 - v1)) + fV * (v3 - v1);

			*pOutDist= fDist;

			D3DXVec3TransformCoord(&vOut, &vOut, &_matWorld);

			*pOutPos = vOut;
			m_pVB->Unlock();
			m_pIB->Unlock();

			return TRUE;
		
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return FALSE;
}
