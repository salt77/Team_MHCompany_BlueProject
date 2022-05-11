#include "Tanmak_Effect.h"

#include "EffectObject.h" 

#include "Renderer.h"
#include "ProtoMgr.h"

CTanmak_Effect::CTanmak_Effect(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_Effect)
{
}

CTanmak_Effect::CTanmak_Effect(const CTanmak_Effect & rhs)
	: CGameObject(rhs)
{
}

CTanmak_Effect::~CTanmak_Effect()
{
}

CTanmak_Effect * CTanmak_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTanmak_Effect*		pInstance = new CTanmak_Effect(pGraphicDev, E_OBJTAG::OBJ_Effect); 

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance); 
	}

	return pInstance;
}

void CTanmak_Effect::Free()
{
	if (m_pEffectObject) 
	{
		Safe_Release(m_pEffectObject); 
	}

	CGameObject::Free(); 
}

void CTanmak_Effect::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte); 
}

void CTanmak_Effect::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte); 
}

HRESULT CTanmak_Effect::Ready_Object()
{
	CGameObject::Ready_Object(); 

	Add_Component();

	m_fScale = 25.f;
	Set_Scale(VEC_ONE * m_fScale);
	m_pEffectObject->Set_Effect_Scale(VEC_ONE * m_fScale);

	return S_OK;
}

_int CTanmak_Effect::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Start_Object(fTimeDelta); 

	return iExit;
}

_int CTanmak_Effect::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Update_Object(fTimeDelta); 

	if (m_pEffectObject->Get_Dead())
	{
		m_bDead = TRUE;

		return iExit;
	}

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CTanmak_Effect::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);

	return iExit;
}

void CTanmak_Effect::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CTanmak_Effect::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	//0204mat
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);
}

void CTanmak_Effect::Set_Constantable_Fog(CShader * pShader)
{
}

void CTanmak_Effect::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Bullet_Lighting.effect"); 
	assert(m_pEffectObject); 

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_EffectShader", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));
}
