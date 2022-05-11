#include "CamControl_Eye.h"
#include "Renderer.h"
#include "ProtoMgr.h"
USING(Engine)

CCamControl_Eye::CCamControl_Eye(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CCamControl_Eye::CCamControl_Eye(const CCamControl_Eye& rhs)
	: CGameObject(rhs)
{
}

CCamControl_Eye::~CCamControl_Eye()
{
}

CCamControl_Eye* CCamControl_Eye::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _float fRadius)
{
	CCamControl_Eye*	pInstance = new CCamControl_Eye(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(vPos, fRadius)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCamControl_Eye::Free()
{
	for (_uint i = 0; i < (_uint)E_WIRE_RENDER::WIRE_RENDER_END; ++i)
	{
		Safe_Release(m_pTexture[i]);
	}
	
	Safe_Release(m_pMesh);

	CGameObject::Free();
}

void CCamControl_Eye::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte);
}

void CCamControl_Eye::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);
}


HRESULT CCamControl_Eye::Ready_Object(_vec3 vPos, _float fRadius)
{
	if (FAILED(CGameObject::Ready_Object()))
		return E_FAIL;

	Add_Component();
	Set_Pos(vPos);

	m_fRadius = fRadius;

	m_iPassNumber = 3;

	if (FAILED(D3DXCreateSphere(m_pGraphicDev, m_fRadius, 10, 10, &m_pMesh, nullptr)))
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

_int CCamControl_Eye::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);

	return iExit;
}

_int CCamControl_Eye::Update_Object(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);
				
	return iExit;
}

_int CCamControl_Eye::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);

	return iExit;
}

void CCamControl_Eye::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CCamControl_Eye::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;

	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_BaseTexture", m_pTexture[(_uint)m_eRenderType]);
}
