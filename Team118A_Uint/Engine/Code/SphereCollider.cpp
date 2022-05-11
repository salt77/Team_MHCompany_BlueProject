#include "SphereCollider.h"
#include "Renderer.h"


USING(Engine)

//생성시 어떤 콜라이더인지 태그 설정
CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev)
{
	m_eColliderTag = E_COLTAG::COL_SPHERE;
}

CSphereCollider::CSphereCollider(const CSphereCollider & rhs)
	: CCollider(rhs)
	, m_fRadius(rhs.m_fRadius)
{
}

CSphereCollider::~CSphereCollider()
{
}

//디버깅용 메쉬를 해제한다.
void CSphereCollider::Free()
{
	Safe_Release(m_pMesh);
	CCollider::Free();
}

CComponent * CSphereCollider::Clone()
{
	return new CSphereCollider(*this);
}


HRESULT CSphereCollider::Ready_Sphere_Collider(CGameObject * pParent)
{
	m_pParentObject = pParent;
	return Ready_Collider();
}

//디버깅용 메쉬를 렌더링하는 함수로 오브젝트의 쉐이더를 사용한다.
void CSphereCollider::Render_Collider(LPD3DXEFFECT pEffect)
{
	if (!CRenderer::GetInstance()->Get_Debug()) 
	{
		return;
	}

	pEffect->SetMatrix("g_matWorld", &m_matColMatrix);
	pEffect->SetTexture("g_BaseTexture", m_pTexture[m_bCol]);
	pEffect->CommitChanges();

	m_pMesh->DrawSubset(0);
}

void CSphereCollider::SetScale(_vec3 vScale)
{
	CCollider::SetScale(vScale);

	Safe_Release(m_pMesh);
	D3DXCreateSphere(m_pGraphicDev, m_fRadius * m_vScale.x, 12, 6, &m_pMesh, nullptr);
}

//함수의 월드매트릭스를 설정한다. 구 충돌체의 경우 반지름값을 유지하기 위해 포지션만 받아온다.
void CSphereCollider::SetMatrix(_matrix matWorld)
{
	_matrix matRot;
	_vec3 vLook = _vec3(matWorld._31, matWorld._32, matWorld._33);
	_vec3 vRot = CMyMath::GetLookRotation(VEC_ZERO, VEC_ZERO + vLook);

	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y * Deg2Rad, vRot.x * Deg2Rad, vRot.z * Deg2Rad);
	D3DXMatrixTranslation(&m_matColMatrix, matWorld._41, matWorld._42, matWorld._43);
	m_matColMatrix = matRot * m_matColMatrix;
}

//구 충돌체의 반지름 값 설정
void CSphereCollider::SetRadius(_float fRad)
{
	m_fRadius = fRad;

	Safe_Release(m_pMesh);
	D3DXCreateSphere(m_pGraphicDev, m_fRadius * m_vScale.x, 12, 6, &m_pMesh, nullptr);
}

//구 충돌체의 반지름값*Scale값 반환, 충돌체 매니저에서 이 값을 이용해 충돌을 수행하기 때문에 실제 월드상의 반지름 값을 반환
_float CSphereCollider::GetRadius()
{
	return m_fRadius*m_vScale.x;
}

//구 충돌체의 레이캐스트를 수행하는 함수로 선택된 오브젝트를 ppObjOut에 채워준다.
_bool CSphereCollider::PickingCollider(HWND hWnd, CGameObject** ppObjOut)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 윈도우 좌표를 투영 좌표로 변환
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 좌표를 뷰스페이스 좌표로 변환
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰스페이스 좌표를 월드 좌표로 변환
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	// 광선을 발사하는 위치 vRayPos, 광선이 향하는 방향을 vRayDir
	_vec3		vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 좌표에서 로컬 좌표로 변환
	_matrix	 matWorld = m_matColMatrix;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	BOOL bCOL;
	DWORD dwFaceIndex;
	_float fV, fU, fDist;
	LPD3DXBUFFER pBuffer;
	DWORD dwHit;
	D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, &bCOL, &dwFaceIndex, &fU, &fV, &fDist, &pBuffer, &dwHit);

	if (bCOL)
	{
		*ppObjOut = m_pParentObject;

		return TRUE;
	}


	*ppObjOut = nullptr;
	return FALSE;
}

_bool CSphereCollider::PickingCollider(_vec3 vWorldRayStartPos, _vec3 vWorldRayDir, _float fRayDistance, CGameObject ** ppObjOut)
{
	
	// 광선을 발사하는 위치 vRayPos, 광선이 향하는 방향을 vRayDir
	_vec3		vRayPos = vWorldRayStartPos;
	_vec3		vRayDir = vWorldRayDir; 


	// 월드 좌표에서 로컬 좌표로 변환
	_matrix	 matWorld = m_matColMatrix;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	m_vScale;

	BOOL bCOL;
	DWORD dwFaceIndex;
	_float fV, fU, fDist;
	LPD3DXBUFFER pBuffer;
	DWORD dwHit;
	D3DXIntersect(m_pMesh, &vRayPos, &vRayDir, &bCOL, &dwFaceIndex, &fU, &fV, &fDist, &pBuffer, &dwHit);

	if (bCOL)
	{
		
		if (fDist > fRayDistance)
		{
			*ppObjOut = nullptr;
			return FALSE;
		}
		else
		{
			*ppObjOut = m_pParentObject;

			return TRUE;
		}
	}


	*ppObjOut = nullptr;
	return FALSE;
}

CSphereCollider * CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);


	if (FAILED(pInstance->Ready_Sphere_Collider(pParent)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}
