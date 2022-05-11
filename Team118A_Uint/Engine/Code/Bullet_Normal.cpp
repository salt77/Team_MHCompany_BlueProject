#include "Bullet_Normal.h"


#include "EffectObject.h"
#include "ParticleObject.h"
#include "Unit.h"
#include "SkillObject.h"

CBullet_Normal::CBullet_Normal(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CBullet_Normal::CBullet_Normal(const CBullet_Normal & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Normal::~CBullet_Normal()
{
}

CBullet_Normal * CBullet_Normal::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 & vPos, const _vec3 & vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
{
	CBullet_Normal*		pInstance = new CBullet_Normal(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vPos, vDir, tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Normal::Free()
{
	if (m_pEffectObject)
	{
		Safe_Release(m_pEffectObject);
	}
	if (m_pSkillObject)
	{
		Safe_Release(m_pSkillObject);
	}
	if (m_pParticle)
	{
		Safe_Release(m_pParticle);
	}
	CAttackObj::Free(); 
}

void CBullet_Normal::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Normal::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Normal::Ready_Object(const _vec3 & vPos, const _vec3 & vDir, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType)
{
	CAttackObj::Ready_Object();

	Add_Component();

	Set_Pos(vPos);
	m_vDir = vDir;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;
	
	m_pSphereColliderCom->SetRadius(0.3f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture);

	return S_OK;
}

_int CBullet_Normal::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CBullet_Normal::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());
	Billboard();

	if (m_pEffectObject)
	{
		m_pEffectObject->Set_Effect_Pos(Get_Position());
		_float fAngle = acosf(m_vDir.x / m_vDir.z);
		m_pEffectObject->Set_Effect_Rotation(_vec3(0.f, D3DXToDegree(fAngle), 0.f));
		m_pEffectObject->Update_Object(fTimeDelta);
	}

	if (m_pSkillObject)
	{
		m_pSkillObject->Set_Pos(Get_Position());
		m_pSkillObject->Update_Object(fTimeDelta);
		m_pSkillObject->Set_Rotation(Get_Angle());
	}

	if (m_pParticle)
	{
		m_pParticle->Set_Pos(Get_Position());
		m_pParticle->Update_Object(fTimeDelta);
	}
	return iExit;
}

_int CBullet_Normal::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta);
	}
	if (m_pSkillObject)
	{
		m_pSkillObject->LateUpdate_Object(fTimeDelta);
	}
	if (m_pParticle)
	{
		m_pParticle->LateUpdate_Object(fTimeDelta);
	}
	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_BILLBOARD, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CBullet_Normal::Render_Object(_int iPass)
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

void CBullet_Normal::On_Collision(CGameObject * pDest)
{
	if (static_cast<CUnit*>(pDest)->Get_UnitDead() || m_bDead)
	{
		return;
	}
	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

	if (m_wstrAfterSkillName != L"Empty")
	{
		CSkill_Manager::GetInstance()->SetSkill(m_wstrAfterSkillName, E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE * 0.5f);
	}


	m_bDead = TRUE;

	return; 
}

void CBullet_Normal::Set_EffectObject(CEffectObject * pEffectObject)
{
	m_pEffectObject = pEffectObject;
}

void CBullet_Normal::Set_ParticleObject(CParticleObject * pParticleObject)
{
	m_pParticle = pParticleObject;
}

void CBullet_Normal::Set_SkillObject(CSkillObject* pSkillObject)
{
	m_pSkillObject = pSkillObject;
}

void CBullet_Normal::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CBullet_Normal::Set_Constantable()
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

void CBullet_Normal::Movement(const _float & fTimeDelta)
{
	Move_Pos(m_vDir, m_fSpeed, fTimeDelta);

	if (5.f <= m_fCountingTime)
	{
		CUtility::Print_Wstring_DMode(L"Bullet_Normal Dead");

		m_bDead = true;
	}
}
