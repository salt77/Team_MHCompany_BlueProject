#include "stdafx.h"
#include "Bullet.h"

#include "Management.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs) 
{
}

CBullet::~CBullet()
{
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vDir, E_OBJTAG eObjTag)
{
	CBullet*	pInstance = new CBullet(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(vPos, vDir)))
		Safe_Release(pInstance); 

	return pInstance;
}

void CBullet::Free()
{
	CGameObject::Free(); 
}


void CBullet::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte); 
}

void CBullet::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte); 
}

HRESULT CBullet::Ready_Object(const _vec3& vPos, const _vec3& vDir)
{
	CGameObject::Ready_Object(); 

	Add_Component(); 

	Set_Pos(vPos);
	m_vDir = vDir; 

	m_pSphereColliderCom->SetRadius(1.f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	Management Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture);

	return S_OK;
}

_int CBullet::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta); 

	return iExit;
}

_int CBullet::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta); 

	m_fCountingTime += fTimeDelta; 

	Movement(fTimeDelta); 
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	return iExit;
}

_int CBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta); 

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	Management Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CBullet::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// buffer
	m_pBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, Management Clone_Prototype(L"Proto_Buffer_RectTex")));

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", Management Clone_Prototype(L"Proto_Shader_DMesh")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CBullet::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
}

void CBullet::Movement(const _float & fTimeDelta)
{
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta); 

	if (2.f <= m_fCountingTime)
	{
		m_bDead = true; 
	}
}

