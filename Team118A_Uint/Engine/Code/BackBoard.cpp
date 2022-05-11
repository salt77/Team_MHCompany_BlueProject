
#include "BackBoard.h"
#include "Renderer.h"
#include "ProtoMgr.h"
USING(Engine)

CBackBoard::CBackBoard(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrTextureFullPath)
	: CGameObject(pGraphicDev, eObjTag),
	m_wstrTextureFullPath(wstrTextureFullPath)
{
}

CBackBoard::CBackBoard(const CBackBoard& rhs)
	: CGameObject(rhs)
{
}

CBackBoard::~CBackBoard()
{
}


HRESULT CBackBoard::Ready_Object()
{
	CGameObject::Ready_Object(); 
	Add_Component();
	Set_Scale(_float(WINCX*2), _float(WINCY*2), 0.f);
	return S_OK;
}

_int CBackBoard::Update_Object(const _float& fTimeDelta)
{

	_int iExit = CGameObject::Update_Object(fTimeDelta);
				
	return iExit;
}

_int CBackBoard::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_PRIORITY, this);

	return iExit;
}


void CBackBoard::Render_Object(_int iPass)
{

	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Debug, 1));

	m_pBufferCom->Render_Buffer();


	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();


}

CBackBoard* CBackBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrTextureFullPath)
{
	CBackBoard*	pInstance = new CBackBoard(pGraphicDev, eObjTag, wstrTextureFullPath);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBackBoard::Free()
{
	CGameObject::Free();
}

void CBackBoard::Add_Component()
{
	CComponent*		pComponent = nullptr;
	
	// buffer
	m_pBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));
	
	// texture
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_wstrTextureFullPath, &m_pTexture);
	
	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CBackBoard::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	pEffect->SetTexture("g_BaseTexture", m_pTexture);
}