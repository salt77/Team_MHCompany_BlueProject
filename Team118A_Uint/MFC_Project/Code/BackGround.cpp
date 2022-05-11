#include "stdafx.h"
#include "../Include/stdafx.h"
#include "BackGround.h"

USING(Engine)

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}


HRESULT CBackGround::Ready_Object()
{
	CGameObject::Ready_Object(); 
	Add_Component();
		
	return S_OK;
}

_int CBackGround::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
				
	return iExit;
}

_int CBackGround::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}


void CBackGround::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Stage, iPass));

	m_pBufferCom->Render_Buffer();

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CBackGround*	pInstance = new CBackGround(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBackGround::Free()
{
	CGameObject::Free();
}

void CBackGround::Add_Component()
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	m_pBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, Management Clone_Prototype(L"Proto_Buffer_RectTex")));

	// texture
	m_pTextureCom = static_cast<CTexture*>(Emplace_Component(L"Com_Texture", Management Clone_Prototype(L"Proto_Texture_Logo")));
	
	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev,this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CBackGround::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);
	
	////// Ä¿½ºÅÒ Æ÷±×
	//T_CustomFog tCustomFog = CFogManager::GetInstance()->GetCustomFog();
	//
	//tCustomFog.vPos = _vec3(0.f, 0.f, 0.f);
	//
	//tCustomFog.fView_Under = 2.f;
	//tCustomFog.fView_Over = 0.f;
	//
	//tCustomFog.fDistance_Start = 1.5f;
	//tCustomFog.vColor_Start = _vec4(1.f, 0.f, 0.f, 0.5f);
	//
	//tCustomFog.fDistance_End = 3.f;
	//tCustomFog.vColor_End = _vec4(0.f, 0.f, 1.f, 1.f);
	//
	//CFogManager::GetInstance()->SetCustomFog(tCustomFog);
	//CFogManager::GetInstance()->SetCustomFog(T_CustomFog());

	// Glow		
	pEffect->SetFloat("g_fBaseGlow", 1.f);

	pEffect->SetFloat("g_fTexGlowR", 0.f);
	pEffect->SetFloat("g_fTexGlowG", 0.f);
	pEffect->SetFloat("g_fTexGlowB", 0.f);

	CRenderer::GetInstance()->Set_DefualtMaskTexture(pEffect, "g_MaskTexture");

	pEffect->SetFloat("g_fGlow_U_Min", 0.f);
	pEffect->SetFloat("g_fGlow_U_Max", 0.f);
	pEffect->SetFloat("g_fGlow_V_Min", 0.f);
	pEffect->SetFloat("g_fGlow_V_Max", 0.f);

	pEffect->SetFloat("g_fGlowPower", 0.f);

	pEffect->SetFloat("g_fGlowColorR", 1.f);
	pEffect->SetFloat("g_fGlowColorG", 1.f);
	pEffect->SetFloat("g_fGlowColorB", 1.f);
}

