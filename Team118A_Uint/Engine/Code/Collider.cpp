#include "Collider.h"


USING(Engine)

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matColMatrix);
}

CCollider::CCollider(const CCollider & rhs)
	:CComponent(rhs)
	,m_eColliderTag(rhs.m_eColliderTag)
	,m_vScale(rhs.m_vScale)
	,m_pParentObject(rhs.m_pParentObject)
{
	for (_ulong i = 0; i < 2; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
}

CCollider::~CCollider()
{
}

//디버깅 렌더시에 사용하는 텍스쳐를 해제하는 함수
void CCollider::Free()
{
	for (_ulong i = 0; i < 2; ++i)
		Safe_Release(m_pTexture[i]);
	CComponent::Free();
}

//컴포넌트가 컴포넌트를 소유하지 않기위해 자체적으로 디버깅용 텍스쳐를 제작하는 함수
HRESULT CCollider::Ready_Collider()
{
	for (_ulong i = 0; i < 2; ++i)
	{
		D3DXCreateTexture(m_pGraphicDev,
			1,
			1,
			1,  // miplevel
			0,	// 텍스처의 용도, D3DUSAGE_RENDERTARGET
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&m_pTexture[i]);

		D3DLOCKED_RECT LockRect;
		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}
	return S_OK;
}

//콜라이더마다 렌더 방식이 다를 수 있으므로 오버라이딩해서 사용
void CCollider::Render_Collider(LPD3DXEFFECT pEffect)
{
}


//여러번 충돌을 방지하기 위해 만든 충돌 오브젝트 리스트를 비운다.
void CCollider::ClearColList()
{
	m_listColObj.clear(); 
}

//여러번 충돌을 방지하기 위해 만든 충돌 오브젝트 리스트에 충돌 오브젝트를 추가한다.
void CCollider::AddColList(const CGameObject * pColObj)
{
	m_listColObj.emplace_back(pColObj);
}

//콜라이더의 스케일 설정
void CCollider::SetScale(_vec3 vScale)
{
	m_vScale = vScale;
}

//콜라이더의 월드 매트릭스 설정
void CCollider::SetMatrix(_matrix matWorld)
{
	m_matColMatrix = matWorld;
}

//현재 콜라이더의 위치 반환
_vec3 CCollider::GetPos()
{
	return _vec3(m_matColMatrix._41, m_matColMatrix._42, m_matColMatrix._43);
}

_vec3 CCollider::GetRot()
{
	_vec3 vLook = _vec3(m_matColMatrix._31, m_matColMatrix._32, m_matColMatrix._33);
	return CMyMath::GetLookRotation(GetPos(), GetPos() + vLook);
}

_vec3 CCollider::GetLook()
{
	_vec3 vRot = GetRot();
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y * Deg2Rad, vRot.x * Deg2Rad, vRot.z * Deg2Rad);
	_vec3 vLook = VEC_LOOK;
	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
	return vLook;
}

_vec3 CCollider::GetRight()
{
	_vec3 vRot = GetRot();
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y * Deg2Rad, vRot.x * Deg2Rad, vRot.z * Deg2Rad);
	_vec3 vRight = VEC_RIGHT;
	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	return vRight;
}

_vec3 CCollider::GetUp()
{
	_vec3 vRot = GetRot();
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, vRot.y * Deg2Rad, vRot.x * Deg2Rad, vRot.z * Deg2Rad);
	_vec3 vUp = VEC_UP;
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	return vUp;
}

//오브젝트가 충돌 오브젝트 리스트에 있는지 확인
_bool CCollider::ColListCheck(const CGameObject * pColObj)
{
	for (auto& pObj : m_listColObj)
	{
		if (pObj == pColObj)
			return TRUE;
	}
	return FALSE;
}

//충돌체의 부모(게임오브젝트)를 반환
CGameObject * CCollider::GetParent()
{
	return m_pParentObject;
}

//충돌체의 태그 반환
E_COLTAG CCollider::GetColliderTag()
{
	return m_eColliderTag; 
}

