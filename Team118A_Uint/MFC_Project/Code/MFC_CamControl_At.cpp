#include "stdafx.h"
#include "MFC_CamControl_At.h"

USING(Engine)

CMFC_CamControl_At::CMFC_CamControl_At(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CMFC_CamControl_At::CMFC_CamControl_At(const CMFC_CamControl_At& rhs)
	: CGameObject(rhs)
{
}

CMFC_CamControl_At::~CMFC_CamControl_At()
{
}

CMFC_CamControl_At* CMFC_CamControl_At::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _float fScale)
{
	CMFC_CamControl_At*	pInstance = new CMFC_CamControl_At(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(vPos, fScale)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMFC_CamControl_At::Free()
{
	for (_uint i = 0; i < (_uint)E_WIRE_RENDER::WIRE_RENDER_END; ++i)
	{
		Safe_Release(m_pTexture[i]);
	}
	
	Safe_Release(m_pMesh);

	CGameObject::Free();
}

void CMFC_CamControl_At::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte);
}

void CMFC_CamControl_At::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}


HRESULT CMFC_CamControl_At::Ready_Object(_vec3 vPos, _float fScale)
{
	if (FAILED(CGameObject::Ready_Object()))
		return E_FAIL;

	Add_Component();
	Set_Pos(vPos);

	m_fScale = fScale;

	//m_iPassNumber = 3;
	m_iPassNumber = 0;

	if (FAILED(D3DXCreateBox(m_pGraphicDev, m_fScale, m_fScale, m_fScale, &m_pMesh, nullptr)))
		return E_FAIL;
	
	for (_uint i = 0; i < (_uint)E_WIRE_RENDER::WIRE_RENDER_END; ++i)
	{
		if (FAILED(D3DXCreateTexture(m_pGraphicDev,
									 1,
									 1,
									 1,
									 0,
									 D3DFMT_A8R8G8B8,
									 D3DPOOL_MANAGED,
									 &m_pTexture[i])))
									 return E_FAIL;

		D3DLOCKED_RECT LockRect;
		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		if ((_uint)E_WIRE_RENDER::WIRE_RENDER_NONE == i)
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		}
		else if ((_uint)E_WIRE_RENDER::WIRE_RENDER_INTERPOL == i)
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		}
		else
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);
		}

		m_pTexture[i]->UnlockRect(0);
	}
	
	return S_OK;
}

_int CMFC_CamControl_At::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);

	return iExit;
}

_int CMFC_CamControl_At::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
				
	return iExit;
}

_int CMFC_CamControl_At::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

void CMFC_CamControl_At::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Debug, 0));

	m_pMesh->DrawSubset(0);

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CMFC_CamControl_At::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
}

void CMFC_CamControl_At::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_BaseTexture", m_pTexture[(_uint)m_eRenderType]);
}
