#include "Tank_Projectile.h"

#include "EffectObject.h"
#include "ParticleObject.h"
#include "Unit.h"

CTank_Projectile::CTank_Projectile(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CTank_Projectile::CTank_Projectile(const CTank_Projectile & rhs)
	: CAttackObj(rhs)
{
}

CTank_Projectile::~CTank_Projectile()
{
}

CTank_Projectile * CTank_Projectile::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								  _vec3 vPos,
								  _vec3 vDir,
								  const T_UnitInfo& tUnitInfo,
								  E_CHAR_ATKTYPE eAtkType,
								  E_LAYERTAG eTargetLayerTag)
{
	CTank_Projectile*		pInstance = new CTank_Projectile(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTank_Projectile::Free()
{
	if (m_pEffectObject)
	{
		Safe_Release(m_pEffectObject);
	}
	if (m_pEffectSmoke)
	{
		Safe_Release(m_pEffectSmoke); 
	}
	if (m_pEffectWave)
	{
		Safe_Release(m_pEffectWave);
	}
	if (m_pEffectBomb)
	{
		Safe_Release(m_pEffectBomb);
	}

	CAttackObj::Free(); 
}

void CTank_Projectile::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CTank_Projectile::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CTank_Projectile::Ready_Object(_vec3 vPos, _vec3 vDir, const T_UnitInfo& tUnitInfo, E_CHAR_ATKTYPE eAtkType)
{
	CAttackObj::Ready_Object();

	Add_Component();

	Set_Pos(vPos);
	m_vDir = vDir;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;
	
	m_pSphereColliderCom->SetRadius(0.5f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture);

	return S_OK;
}

_int CTank_Projectile::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CTank_Projectile::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix()); 

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	return iExit;
}

_int CTank_Projectile::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}
	
	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CTank_Projectile::Render_Object(_int iPass)
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

void CTank_Projectile::On_Collision(CGameObject * pDest)
{
	if (static_cast<CUnit*>(pDest)->Get_UnitDead())
	{
		return;
	}

	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

	m_pEffectBomb = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Tank_Fire_Attacked.effect");

	if (m_pSphereColliderCom)
	{
		_matrix		matWorld = m_pSphereColliderCom->Get_WorldMatrix(); 
		matWorld._41 = 999.f; 
		m_pSphereColliderCom->SetMatrix(matWorld); 
	}

	return; 
}

void CTank_Projectile::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pEffectSmoke = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Tank_Fire_Smoke.effect"); 
	m_pEffectWave = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Tank_Fire_Normal.effect");

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CTank_Projectile::Set_Constantable()
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

void CTank_Projectile::Movement(const _float & fTimeDelta)
{
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta);

	if (5.f <= m_fCountingTime)
	{
		CUtility::Print_Wstring_DMode(L"Tank_Projectile Dead"); 

		m_bDead = true;
	}
}
