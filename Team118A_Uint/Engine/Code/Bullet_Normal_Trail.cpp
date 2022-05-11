#include "Bullet_Normal_Trail.h"


#include "EffectObject.h"
#include "ParticleObject.h"
#include "Trail_Fix.h"
#include "Unit.h"

CBullet_Normal_Trail::CBullet_Normal_Trail(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CBullet_Normal_Trail::CBullet_Normal_Trail(const CBullet_Normal_Trail & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Normal_Trail::~CBullet_Normal_Trail()
{
}

CBullet_Normal_Trail * CBullet_Normal_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, const _vec3 & vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
{
	CBullet_Normal_Trail*		pInstance = new CBullet_Normal_Trail(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Normal_Trail::Free()
{
	if (m_pEffectObject)
	{
		Safe_Release(m_pEffectObject);
	}
	CAttackObj::Free(); 
}

void CBullet_Normal_Trail::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Normal_Trail::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Normal_Trail::Ready_Object(const _vec3 & vPos, const _vec3 & vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType)
{
	CAttackObj::Ready_Object();

	Add_Component();

	Set_Pos(vPos);
	m_vDir = vDir;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;
	
	m_pSphereColliderCom->SetRadius(0.1f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture);

	m_pTrail = CTrail_Fix::Create(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/Fire_Noise2.png", 0.2f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Trail, m_pTrail);
	m_pTrail->Set_FadeOutSize(TRUE);
	m_pTrail->Set_FadeOutAlpha(TRUE);

	m_fSpeed = 15.f;
	return S_OK;
}

_int CBullet_Normal_Trail::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CBullet_Normal_Trail::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());
	
	Billboard();

	m_pTrail->Add_Vertex(Get_Position() + _vec3(0.f, 0.05f, 0.f), Get_Position() + _vec3(0.f, -0.05f, 0.f));
	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CBullet_Normal_Trail::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}
	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_BILLBOARD, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CBullet_Normal_Trail::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	//if (0 == iPass)
	//{
	//	m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

	//	m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	//}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBullet_Normal_Trail::On_Collision(CGameObject * pDest)
{
	if (E_LAYERTAG::Monster == pDest->Get_LayerTag())
	{
		CUtility::Print_Wstring_DMode(L"Monster Hitted");
	}
	else if (E_LAYERTAG::Character == pDest->Get_LayerTag())
	{
		CUtility::Print_Wstring_DMode(L"Character Hitted");
	}

	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

	m_bDead = TRUE;

	return; 
}

void CBullet_Normal_Trail::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Test_Bullet.effect");

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CBullet_Normal_Trail::Set_Constantable()
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

void CBullet_Normal_Trail::Movement(const _float & fTimeDelta)
{
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta);

	if (5.f <= m_fCountingTime)
	{
		CUtility::Print_Wstring_DMode(L"Bullet_Normal Dead");

		m_bDead = true;
	}
}
