#include "Bullet_Dead.h"

#include "EffectObject.h" 

#include "Renderer.h" 
#include "ProtoMgr.h" 

CBullet_Dead::CBullet_Dead(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag) 
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_Effect)
{
}

CBullet_Dead::CBullet_Dead(const CBullet_Dead & rhs) 
	: CGameObject(rhs)
{
}

CBullet_Dead::~CBullet_Dead()
{
}

CBullet_Dead * CBullet_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, const E_RUN_BULLETTYPE& eBulletType)
{
	CBullet_Dead*	pInstance = new CBullet_Dead(pGraphicDev, E_OBJTAG::OBJ_Effect); 

	if (FAILED(pInstance->Ready_Object(eBulletType)))
	{
		Safe_Release(pInstance); 
	}

	return pInstance;
}

void CBullet_Dead::Free()
{
	if (m_pEffectObject)
	{
		Safe_Release(m_pEffectObject); 
	}

	CGameObject::Free(); 
}

void CBullet_Dead::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte); 
}

void CBullet_Dead::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte); 
}

HRESULT CBullet_Dead::Ready_Object(const E_RUN_BULLETTYPE& eBulletType)
{
	CGameObject::Ready_Object(); 

	m_eBulletType = eBulletType; 

	Add_Component();

	//m_fScale = 100.f; 
	//Set_Scale(VEC_ONE * m_fScale);
	m_pEffectObject->Set_Effect_Scale(VEC_ONE * 100.f);

	return S_OK; 
}

_int CBullet_Dead::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Start_Object(fTimeDelta); 

	return iExit;
}

_int CBullet_Dead::Update_Object(const _float & fTimeDelta)
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

_int CBullet_Dead::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS)); 

	//0205_alpha_pass_change_byEH_랜더 문제시 문의
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this); 

	return iExit;
}

void CBullet_Dead::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBullet_Dead::Set_Constantable()
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

void CBullet_Dead::Set_Constantable_Fog(CShader * pShader)
{
}

void CBullet_Dead::Add_Component()
{
	CComponent*		pComponent = nullptr;

	switch (m_eBulletType)
	{
	case Engine::E_RUN_BULLETTYPE::BULLET_RED:
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_Dead.effect");
		break;

	case Engine::E_RUN_BULLETTYPE::BULLET_PINK:
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_Dead_Pink.effect");
		break;

	case Engine::E_RUN_BULLETTYPE::BULLET_BLUE:
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_Dead_Blue.effect");
		break;

	case Engine::E_RUN_BULLETTYPE::BULLET_YELLOW:
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_Dead_Yellow.effect");
		break;

	default:
		MSG_BOX("Bullet_Dead Texture 없음"); 
		assert(0); 
		break;
	}

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_EffectShader", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}
