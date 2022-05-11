#include "stdafx.h"
#include "../Include/stdafx.h"
#include "StartMark.h"

USING(Engine)

CStartMark::CStartMark(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CStartMark::CStartMark(const CStartMark& rhs)
	: CGameObject(rhs)
{
}

CStartMark::~CStartMark()
{
}


HRESULT CStartMark::Ready_Object()
{
	CGameObject::Ready_Object(); 
	Add_Component();
	
	m_pSphereColliderCom->SetRadius(0.33f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	WCHAR wszPath[128];
	swprintf_s(wszPath, L"../../Reference/Resource/Texture/Test/%d.png", m_iNumber);

	Management Get_Texture(m_pGraphicDev,wszPath,&m_pTexture);
	return S_OK;
}

_int CStartMark::Update_Object(const _float& fTimeDelta)
{
	m_pTransformCom->Update_Component(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(m_pTransformCom->Get_WorldMatrix());
	return RETURN_OK;
}

_int CStartMark::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	//Management Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA, this);
	//Management Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}


void CStartMark::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(m_iPassNumber);

	m_pBufferCom->Render_Buffer();

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();

}

_int CStartMark::Get_Number()
{
	return m_iNumber;
}

void CStartMark::Set_Number(_int iNumber)
{
	m_iNumber = iNumber;
}

CStartMark* CStartMark::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _int iNumber)
{
	if (iNumber < 1 || iNumber>6)
	{
		return nullptr;
	}

	CStartMark*	pInstance = new CStartMark(pGraphicDev, eObjTag);

	pInstance->Set_Number(iNumber);
	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStartMark::Free()
{
	CGameObject::Free();
}

void CStartMark::Add_Component()
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	m_pBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, Management Clone_Prototype(L"Proto_Buffer_RectTex")));

	
	
	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev,this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CStartMark::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_BaseTexture", m_pTexture);
	
}

