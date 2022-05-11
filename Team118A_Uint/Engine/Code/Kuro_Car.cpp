#include "Kuro_Car.h"

#include "SphereCollider.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "Unit.h"
#include "Character.h"
USING(Engine)

CKuro_Car::CKuro_Car(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CKuro_Car::CKuro_Car(const CKuro_Car & rhs)
	: CAttackObj(rhs)
{
}

CKuro_Car::~CKuro_Car()
{
}

CKuro_Car * CKuro_Car::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _float fDelayTime)
{
	CKuro_Car*	pInstance = new CKuro_Car(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vDir, vStartPos, tUnitInfo, eAtkType, fDelayTime)))
		Safe_Release(pInstance);

	return pInstance;
}

void CKuro_Car::Free()
{
	CAttackObj::Free();
}

void CKuro_Car::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CKuro_Car::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CKuro_Car::Ready_Object(const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _float fDelayTime)
{
	CAttackObj::Ready_Object();

	m_fDelayTime = fDelayTime;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	Add_Component();

	Set_Scale(_vec3(3.f, 3.f, 3.f));
	m_fSpeed = 40.f;

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);
	m_vDir = vDir;

	D3DXVec3Normalize(&m_vDir, &m_vDir);



	m_pSphereColliderCom->SetRadius(0.7f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Kuro_Car.particle");
	
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, m_pParticle->Get_ObjTag(), m_pParticle);
	m_pParticle->AddRef();
	m_pParticle->Set_Scale(_vec3(1.f, 1.f, 1.f));

	Set_Direction(m_vDir);
	Add_Rotation(_vec3(0.f, 180.f, 0.f));

	m_pParticle->Set_Direction(m_vDir);
	m_pParticle->Add_Rotation(_vec3(0.f, 180.f, 0.f));
	return S_OK;
}

_int CKuro_Car::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CKuro_Car::Update_Object(const _float & fTimeDelta)
{
	m_fDelayTime -= fTimeDelta;

	if (!Get_IsDelayed())
	{
		if (!m_bMove)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Kuro_Projectile_00.wav", 1.f);

			m_bMove = TRUE;
		}
		Movement(fTimeDelta);
		m_eTargetLayerTag = E_LAYERTAG::Character;
	}
	
	if (m_pParticle)
	{
		m_pParticle->Set_Pos(Get_Position());
	}
	
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);
	return iExit;
}

_int CKuro_Car::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CKuro_Car::Render_Object(_int iPass)
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

void CKuro_Car::Debug_This_In_Console()
{
}

void CKuro_Car::Set_Constantable()
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
	//// 그림자용
	//pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	//pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망시 필터
	pEffect->SetFloat("g_fFilter", 0.f);
}

void CKuro_Car::Add_Component()
{
	CComponent*		pComponent = nullptr;

	wstring wstrMeshName = L"Kuro_Car.X";
	wstring wstrMeshPath = L"../../Reference/Resource/Others/Kuro/Kuro_Car/";

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

void CKuro_Car::Movement(const _float & fTimeDelta)
{
	
	m_pTransformCom->Move_Pos(m_vDir, m_fSpeed, fTimeDelta);
	
	
}

void CKuro_Car::OnEnable()
{
	CAttackObj::OnEnable();
}

void CKuro_Car::OnDisable()
{
	CAttackObj::OnDisable();
}

void CKuro_Car::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CKuro_Car::On_Collision(CGameObject * pDest)
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
		_vec3 vKnockBackDir = m_vDir;
		vKnockBackDir.y = 0.f;
		_matrix matRotX;
		D3DXMatrixRotationX(&matRotX, -45.f * Deg2Rad);
		D3DXVec3TransformNormal(&vKnockBackDir, &vKnockBackDir, &matRotX);

		static_cast<CCharacter*>(pDest)->Set_Gravity(vKnockBackDir, 0.3f);
		CUtility::Print_Wstring_DMode(L"Character가 쿠로 차에 맞음");
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
