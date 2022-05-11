#include "Kuro_Cup.h"

#include "SphereCollider.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "Unit.h"
#include "Character.h"
USING(Engine)

CKuro_Cup::CKuro_Cup(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CKuro_Cup::CKuro_Cup(const CKuro_Cup & rhs)
	: CAttackObj(rhs)
{
}

CKuro_Cup::~CKuro_Cup()
{
}

CKuro_Cup * CKuro_Cup::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _float fDelayTime)
{
	CKuro_Cup*	pInstance = new CKuro_Cup(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vDir, vStartPos, tUnitInfo, eAtkType, fDelayTime)))
		Safe_Release(pInstance);

	return pInstance;
}

void CKuro_Cup::Free()
{
	CAttackObj::Free();
}

void CKuro_Cup::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CKuro_Cup::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CKuro_Cup::Ready_Object(const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _float fDelayTime)
{
	CAttackObj::Ready_Object();

	m_fDelayTime = fDelayTime;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	Add_Component();

	Set_Scale(_vec3(1.5f, 1.5f, 1.5f));

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);
	m_vDir = vDir;

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pSphereColliderCom->SetRadius(0.7f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Cup_Spark.particle");
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, m_pParticle->Get_ObjTag(), m_pParticle);
	m_pParticle->AddRef();
	m_pParticle->Set_Scale(_vec3(0.2f, 0.2f, 0.2f));

	return S_OK;
}

_int CKuro_Cup::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CKuro_Cup::Update_Object(const _float & fTimeDelta)
{
	m_fDelayTime -= fTimeDelta;

	m_pTransformCom->Add_Rotation(E_ROTATION::ROT_Y, D3DXToRadian(36000.f * fTimeDelta));
	if (!Get_IsDelayed())
	{
		if (!m_bMove)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Kuro_Prop_Move_01.wav", 1.f);
			
			m_bMove = TRUE;
		}
		Movement(fTimeDelta);
		m_eTargetLayerTag = E_LAYERTAG::Character;
	}

	m_pParticle->Set_Pos(Get_Position());
	
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);
	return iExit;
}

_int CKuro_Cup::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CKuro_Cup::Render_Object(_int iPass)
{
	if (nullptr == m_pSMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Base, iPass));

	m_pSMeshCom->Render_Meshes(m_pShaderCom);


	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	
	}

	

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CKuro_Cup::Debug_This_In_Console()
{
}

void CKuro_Cup::Set_Constantable()
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
	//
	//// ±×¸²ÀÚ¿ë
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// »ç¸Á½Ã ÇÊÅÍ
	pEffect->SetFloat("g_fFilter", 0.f);
}

void CKuro_Cup::Add_Component()
{
	CComponent*		pComponent = nullptr;

	wstring wstrMeshName = L"Kuro_Cup.X";
	wstring wstrMeshPath = L"../../Reference/Resource/Others/Kuro/Kuro_Cup/";

	// buffer
	pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrMeshName);
	if (pComponent)
	{
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_DMesh, pComponent));
	}
	else
	{
		CProtoMgr::GetInstance()->Ready_Prototype(wstrMeshName, CSMesh::Create(m_pGraphicDev, wstrMeshPath, wstrMeshName));
		pComponent = CProtoMgr::GetInstance()->Clone_Prototype(wstrMeshName);
		m_pSMeshCom = static_cast<CSMesh*>(Emplace_Component(ComTag_DMesh, pComponent));
	}

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CKuro_Cup::Movement(const _float & fTimeDelta)
{
	
	m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
	
	
}

void CKuro_Cup::OnEnable()
{
	CAttackObj::OnEnable();
}

void CKuro_Cup::OnDisable()
{
	CAttackObj::OnDisable();
}

void CKuro_Cup::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CKuro_Cup::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest);

	if (E_LAYERTAG::Character == pDest->Get_LayerTag())
	{
	/*	_vec3	vTargetObjPos = pDest->Get_Position();
		_vec3	vMyPos = Get_Position();
		vMyPos.y = vTargetObjPos.y; 
		_vec3	vDir = vTargetObjPos - vMyPos;
		D3DXVec3Normalize(&vDir, &vDir);

		vTargetObjPos = vTargetObjPos + (vDir * CWall512::GetInstance()->GetFloatValue(1.5f, 3.f));

		static_cast<CUnit*>(pDest)->Set_KnockBack(vTargetObjPos);
*/
		static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());
		_vec3 vKnockBackDir;
		_matrix matRotX;
		D3DXMatrixRotationX(&matRotX, -45.f * Deg2Rad);
		D3DXVec3TransformNormal(&vKnockBackDir, &m_vDir, &matRotX);
	
		static_cast<CCharacter*>(pDest)->Set_Gravity(vKnockBackDir, 0.1f);
		CUtility::Print_Wstring_DMode(L"Character°¡ Äí·Î ÄÅ¿¡ ¸ÂÀ½");
	}

	/*if (E_LAYERTAG::AttackObj_Monster == pDest->Get_LayerTag())
	{
		if (E_ATKOBJ_TYPE::ATKOBJ_BULLET_NORMAL == static_cast<CAttackObj*>(pDest)->Get_AtkObjType())
		{
			pDest->Set_Dead(TRUE);
		}
		else if (E_ATKOBJ_TYPE::ATKOBJ_SHIRO_BALL == static_cast<CAttackObj*>(pDest)->Get_AtkObjType())
		{
			_vec3	vAtkObjPos = pDest->Get_Position();
			vAtkObjPos.y = Get_Position().y;
			_vec3	vDir = Get_Position() - vAtkObjPos;
			D3DXVec3Normalize(&vDir, &vDir);

			_vec3	vTargetPos = Get_Position() + (vDir * CWall512::GetInstance()->GetFloatValue(1.5f, 3.f));

			Set_KnockBack(vTargetPos);
		}

		m_iTotalDamage += Get_Damage(static_cast<CAttackObj*>(pDest)->Get_UnitInfo(), static_cast<CAttackObj*>(pDest)->Get_AtkType());

		Create_HitEffect();

		CUtility::Print_Wstring_DMode(L"Player Hitted");
	}*/
}
