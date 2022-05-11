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

//����� �����ÿ� ����ϴ� �ؽ��ĸ� �����ϴ� �Լ�
void CCollider::Free()
{
	for (_ulong i = 0; i < 2; ++i)
		Safe_Release(m_pTexture[i]);
	CComponent::Free();
}

//������Ʈ�� ������Ʈ�� �������� �ʱ����� ��ü������ ������ �ؽ��ĸ� �����ϴ� �Լ�
HRESULT CCollider::Ready_Collider()
{
	for (_ulong i = 0; i < 2; ++i)
	{
		D3DXCreateTexture(m_pGraphicDev,
			1,
			1,
			1,  // miplevel
			0,	// �ؽ�ó�� �뵵, D3DUSAGE_RENDERTARGET
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

//�ݶ��̴����� ���� ����� �ٸ� �� �����Ƿ� �������̵��ؼ� ���
void CCollider::Render_Collider(LPD3DXEFFECT pEffect)
{
}


//������ �浹�� �����ϱ� ���� ���� �浹 ������Ʈ ����Ʈ�� ����.
void CCollider::ClearColList()
{
	m_listColObj.clear(); 
}

//������ �浹�� �����ϱ� ���� ���� �浹 ������Ʈ ����Ʈ�� �浹 ������Ʈ�� �߰��Ѵ�.
void CCollider::AddColList(const CGameObject * pColObj)
{
	m_listColObj.emplace_back(pColObj);
}

//�ݶ��̴��� ������ ����
void CCollider::SetScale(_vec3 vScale)
{
	m_vScale = vScale;
}

//�ݶ��̴��� ���� ��Ʈ���� ����
void CCollider::SetMatrix(_matrix matWorld)
{
	m_matColMatrix = matWorld;
}

//���� �ݶ��̴��� ��ġ ��ȯ
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

//������Ʈ�� �浹 ������Ʈ ����Ʈ�� �ִ��� Ȯ��
_bool CCollider::ColListCheck(const CGameObject * pColObj)
{
	for (auto& pObj : m_listColObj)
	{
		if (pObj == pColObj)
			return TRUE;
	}
	return FALSE;
}

//�浹ü�� �θ�(���ӿ�����Ʈ)�� ��ȯ
CGameObject * CCollider::GetParent()
{
	return m_pParentObject;
}

//�浹ü�� �±� ��ȯ
E_COLTAG CCollider::GetColliderTag()
{
	return m_eColliderTag; 
}

