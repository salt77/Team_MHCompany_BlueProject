#include "SkillGuideObject.h"
#include "Renderer.h"
#include "Scene.h"
#include "ProtoMgr.h"
USING(Engine)

CSkillGuideObject::CSkillGuideObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CGameObject(pGraphicDev, eObjTag)
{
	ZeroMemory(&m_tGuideOption, sizeof(T_GUIDE_OPTION));
}

CSkillGuideObject::CSkillGuideObject(const CSkillGuideObject & rhs)
	:CGameObject(rhs)
{
}

CSkillGuideObject::~CSkillGuideObject()
{
}

void CSkillGuideObject::Free()
{
	Safe_Release(m_pGuideVertexBuffer);
	Safe_Release(m_pGuideIndexBuffer);
	Safe_Release(m_pDelayVertexBuffer);
	Safe_Release(m_pDelayIndexBuffer);
	CGameObject::Free();
}


_int CSkillGuideObject::Start_Object(const _float & fTimeDelta)
{
	return CGameObject::Start_Object(fTimeDelta);
}

_int CSkillGuideObject::Update_Object(const _float & fTimeDelta)
{
	Delay_Check(fTimeDelta);
	CGameObject::Update_Object(fTimeDelta);
	m_tGuideOption.vPosition = Get_Position();
	m_tGuideOption.vRotation = Get_Angle();
	
	return RETURN_OK;
}

_int CSkillGuideObject::LateUpdate_Object(const _float & fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);
	return CGameObject::LateUpdate_Object(fTimeDelta);
}

void CSkillGuideObject::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Alpha, iPass));
	

	if (m_pGuideVertexBuffer)
	{
		m_pGraphicDev->SetStreamSource(0, m_pGuideVertexBuffer, 0, sizeof(VTXUI));
		m_pGraphicDev->SetFVF(FVF_UI);

		m_pGraphicDev->SetIndices(m_pGuideIndexBuffer);

		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iGuideVertexCount, 0, m_iGuideTriCount);
	}

	if (m_pDelayVertexBuffer)
	{
		m_pGraphicDev->SetStreamSource(0, m_pDelayVertexBuffer, 0, sizeof(VTXUI));
		m_pGraphicDev->SetFVF(FVF_UI);

		m_pGraphicDev->SetIndices(m_pDelayIndexBuffer);

		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iDelayVertexCount, 0, m_iDelayTriCount);
	}


	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CSkillGuideObject::Update_DelayMesh()
{
}

void CSkillGuideObject::Delay_Check(_float fTimeDelta)
{
	if (m_bGuideDead)
	{
		m_fDeadDelay -= fTimeDelta;
		if (m_fDeadDelay < 0.f)
		{
			m_bDead = TRUE;
		}
	}
	else
	{
		m_fDelayTimer += fTimeDelta;
		Update_DelayMesh();
		if (m_fDelayTimer >= m_fDelay)
		{
			m_bGuideDead = TRUE;
		}
	}
	
}

void CSkillGuideObject::Add_Component()
{
	if (!m_wstrTexturePath.empty())
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_wstrTexturePath, &m_pTexture);
	}
	else
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/Test/GuideTex.png", &m_pTexture);
	}
	//m_pTextureCom = static_cast<CTexture*>(Emplace_Component(L"Com_Texture", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Texture_Logo")));
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CSkillGuideObject::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	//0204mat
	//_matrix	matView, matProj;
	//
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	//m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);
	pEffect->SetTexture("g_BaseTexture", m_pTexture);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pEffect->SetFloat("g_fAddAlpha", -(1 - m_fDeadDelay));
	//CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//// 그림자용
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());
}

HRESULT CSkillGuideObject::Ready_Object()
{
	
	CGameObject::Ready_Object();
	Add_Component();

	return S_OK;
}
