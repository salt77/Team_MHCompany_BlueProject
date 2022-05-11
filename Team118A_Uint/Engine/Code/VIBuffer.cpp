#include "VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwFVF(0)
	, m_dwIdxSize(0)
	, m_dwTriCnt(0)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_IdxFmt(rhs.m_IdxFmt)
	, m_pVertices(rhs.m_pVertices)
	, m_pIndices(rhs.m_pIndices)
{
	if (m_pIB)
	{
		m_pIB->AddRef();
	}
	if (m_pVB)
	{
		m_pVB->AddRef();
	}
}

CVIBuffer::~CVIBuffer()
{
}

void CVIBuffer::Free()
{
	if (!m_bClone)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}

void CVIBuffer::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CVIBuffer::Load(HANDLE hFile, DWORD & dwByte)
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	m_pVertices = new _byte[m_dwVtxSize * m_dwVtxCnt];
	m_pIndices = new _byte[m_dwIdxSize * m_dwTriCnt];

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);

	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt); // 인덱스 버퍼도 활용하여 출력하는 함수
}

// 인자 bOpposite 는 Ray의 방향을 반대로 체크한다.
_bool CVIBuffer::RayCast(const POINT & ptMouse, 
						HWND hWnd, 
						_uint iWinCX, 
						_uint iWinCY, 
						const _matrix & _matWorld, 
						_vec3 * outVec3, 
						_bool bOpposite)
{
	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	// 윈도우 좌표를 투영 스페이스로 변환
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 스페이스를 뷰 스페이스로 변환
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	matProj = *D3DXMatrixInverse(&matProj, NULL, &matProj);
	vMousePos = *D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 상에서 광선의 시작점과 방향을 구한다.
	_vec3 vRayPos = VEC_ZERO;
	_vec3 vRayDir = vMousePos - vRayPos;
	if (bOpposite)
	{
		vRayDir = -vMousePos;
	}

	// vRayPos 와 vRayDir 을 월드 스페이스로 변환
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	matView = *D3DXMatrixInverse(&matView, NULL, &matView);
	vRayPos = *D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	vRayDir = *D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 스페이스 -> 로컬 스페이스
	_matrix matWorld = *D3DXMatrixInverse(&matWorld, NULL, &_matWorld);
	vRayPos = *D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	vRayDir = *D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	// 정점들과 인덱스들을 가져온다
	_byte* pVertices = (_byte*)m_pVertices;
	_byte* pIndices = (_byte*)m_pIndices;

	_vec3 vVtx1, vVtx2, vVtx3;
	ZeroMemory(&vVtx1, sizeof(_vec3));
	ZeroMemory(&vVtx2, sizeof(_vec3));
	ZeroMemory(&vVtx3, sizeof(_vec3));


	_float fMinDist = 999999.f;
	_uint iIndex = 0;
	_vec3 vResPos = {};

	_float fU = 0.f, fV = 0.f, fDist = 0.f;
	for (_uint iTriIndex = 0; iTriIndex < m_dwTriCnt; ++iTriIndex)
	{
		if (D3DFMT_INDEX16 == m_IdxFmt)
		{
			WORD sIndex1, sIndex2, sIndex3;
			memcpy(&sIndex1, pIndices + iTriIndex * m_dwIdxSize + sizeof(WORD) * 0,	sizeof(WORD));
			memcpy(&sIndex2, pIndices + iTriIndex * m_dwIdxSize + sizeof(WORD) * 1,	sizeof(WORD));
			memcpy(&sIndex3, pIndices + iTriIndex * m_dwIdxSize + sizeof(WORD) * 2, sizeof(WORD));

			memcpy(&vVtx1, pVertices + sIndex1 * m_dwVtxSize, sizeof(_vec3));
			memcpy(&vVtx2, pVertices + sIndex2 * m_dwVtxSize, sizeof(_vec3));
			memcpy(&vVtx3, pVertices + sIndex3 * m_dwVtxSize, sizeof(_vec3));
		}
		else
		{
			DWORD dwIndex1, dwIndex2, dwIndex3;
			memcpy(&dwIndex1, pIndices + iTriIndex * m_dwIdxSize + sizeof(DWORD) * 0,	sizeof(DWORD));
			memcpy(&dwIndex2, pIndices + iTriIndex * m_dwIdxSize + sizeof(DWORD) * 1,	sizeof(DWORD));
			memcpy(&dwIndex3, pIndices + iTriIndex * m_dwIdxSize + sizeof(DWORD) * 2,	sizeof(DWORD));

			memcpy(&vVtx1, pVertices + dwIndex1 * m_dwVtxSize, sizeof(_vec3));
			memcpy(&vVtx2, pVertices + dwIndex2 * m_dwVtxSize, sizeof(_vec3));
			memcpy(&vVtx3, pVertices + dwIndex3 * m_dwVtxSize, sizeof(_vec3));
		}

		if (D3DXIntersectTri(&vVtx1, &vVtx2, &vVtx3, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			if (fMinDist > fDist)
			{
				fMinDist = fDist;
				iIndex = iTriIndex;
				vResPos = vVtx1 + fU * (vVtx2 - vVtx1) + fV * (vVtx3 - vVtx1);
			}
		}
	}

	if (fMinDist > 900000.f)
		return false;
	else
	{
		*outVec3 = vResPos;
		D3DXVec3TransformCoord(outVec3, outVec3, &matWorld);
	}

	return true;
}

const _ulong & CVIBuffer::Get_TriCnt()
{
	return m_dwTriCnt;
}

const _ulong & CVIBuffer::Get_VtxCnt()
{
	return m_dwVtxCnt;
}

LPDIRECT3DVERTEXBUFFER9 CVIBuffer::Get_VB()
{
	return m_pVB;
}

LPDIRECT3DINDEXBUFFER9 CVIBuffer::Get_IB()
{
	return m_pIB;
}
