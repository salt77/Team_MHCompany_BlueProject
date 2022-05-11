#include "SkyDome.h"
#include "Renderer.h"
#include "KeyManager.h"
#include "FontMgr.h"
#include "Wall512.h"
#include "SphereTex2.h"
#include "ProtoMgr.h"
CSkyDome::CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_BackGround)
{

}

CSkyDome::CSkyDome(const CSkyDome& rhs)
	: CGameObject(rhs)
{

}

CSkyDome::~CSkyDome(void)
{

}

HRESULT CSkyDome::Ready_Object(wstring wstrSkyFullPath)
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(wstrSkyFullPath), E_FAIL);

	Get_Transform()->Set_Scale(10.f, 10.f, 10.f);

	return S_OK;
}

_int CSkyDome::Update_Object(const _float& fTimeDelta)
{
	_matrix matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);
	Get_Transform()->Set_Pos(_vec3(matInvView._41, matInvView._42, matInvView._43));

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_PRIORITY, this);

	return iExit;
}

void CSkyDome::Render_Object(_int iPass)
{
	Set_ConstantTable();
	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Debug, 1));

	m_pSphereDomeCom->Render_Buffer();
	m_pShaderCom->EndPass();

	m_pShaderCom->EndShader();
}

void CSkyDome::Set_SkyTex(wstring wstrTexFullPath)
{
	if (S_OK == CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrTexFullPath, &m_pTexture))
	{
		CUtility::Print_Wstring_DMode(L"성공");
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrTexFullPath, &m_pTexture);
	}

}

CSkyDome* CSkyDome::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrSkyFullPath)
{
	CSkyDome*	pInstance = new CSkyDome(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(wstrSkyFullPath)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkyDome::Free(void)
{
	CGameObject::Free();
}

HRESULT CSkyDome::Add_Component(wstring wstrSkyFullPath)
{
	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// buffer
	m_pSphereDomeCom = static_cast<CSphereTex2*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_SphereTex2")));

	// texture
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrSkyFullPath, &m_pTexture);
	
	if (S_OK == CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrSkyFullPath, &m_pTexture))
	{
		CUtility::Print_Wstring_DMode(L"성공");
	}
	else
	{
		return E_FAIL;
	}

	CComponent* pComponent = nullptr;


	return S_OK;
}

void CSkyDome::Set_ConstantTable()
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
