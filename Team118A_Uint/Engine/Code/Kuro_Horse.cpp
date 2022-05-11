#include "Kuro_Horse.h"

#include "SphereCollider.h"
#include "SMesh.h"
#include "LightMgr.h"
#include "Unit.h"
#include "Character.h"
USING(Engine)

CKuro_Horse::CKuro_Horse(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

CKuro_Horse::CKuro_Horse(const CKuro_Horse & rhs)
	: CAttackObj(rhs)
{
}

CKuro_Horse::~CKuro_Horse()
{
}

CKuro_Horse * CKuro_Horse::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDir, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag, _float fDelayTime)
{
	CKuro_Horse*	pInstance = new CKuro_Horse(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(vDir, vStartPos, tUnitInfo, eAtkType, fDelayTime)))
		Safe_Release(pInstance);

	return pInstance;
}

void CKuro_Horse::Free()
{
	CAttackObj::Free();
}

void CKuro_Horse::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CKuro_Horse::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CKuro_Horse::Ready_Object(const _vec3& vCenter, const _vec3& vStartPos, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, _float fDelayTime)
{
	CAttackObj::Ready_Object();

	m_fDelayTime = fDelayTime;
	m_fStartDelayTime = fDelayTime;

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	Add_Component();

	Set_Scale(_vec3(2.f, 2.f, 2.f));
	m_fNowLifeTime = 8.f;
	m_fSpeed = 40.f;

	m_vStartPos = vStartPos;
	Set_Pos(m_vStartPos);
	m_vCenter = vCenter;

	_vec3 vDir = m_vStartPos - m_vCenter;
	m_fDistance = D3DXVec3Length(&vDir);

	m_fStartAngle = CMyMath::GetAngleToTarget_vec3(VEC_LOOK, vDir);
	if (vDir.x < 0.f)
	{
		m_fStartAngle = 360.f - m_fStartAngle;
	}


	D3DXVec3Cross(&m_vDir, &_vec3(0.f, 1.f, 0.f), &_vec3(m_vCenter - Get_Position()));
	Set_Direction(m_vDir);
	Add_Rotation(_vec3(0.f, -90.f, 0.f));

	m_pTransformCom->Update_Component(0.f);

	m_pSphereColliderCom->SetRadius(0.6f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	m_pParticle = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Test_MH_01.particle");
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, m_pParticle->Get_ObjTag(), m_pParticle);
	m_pParticle->AddRef();
	m_pParticle->Set_Scale(_vec3(2.f, 2.f, 2.f));
	


	return S_OK;
}

_int CKuro_Horse::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CKuro_Horse::Update_Object(const _float & fTimeDelta)
{
	m_fDelayTime -= fTimeDelta;

	if (!Get_IsDelayed())
	{
		m_eTargetLayerTag = E_LAYERTAG::Character;
		Movement(fTimeDelta);
	}
	else
	{
		_float fRatio = (m_fStartDelayTime - m_fDelayTime) / m_fStartDelayTime;

		Set_Pos(_vec3(Get_Position().x, m_fStartY + fRatio * -m_fStartY, Get_Position().z));

	}
	_vec3 vPos = Get_Position();
	vPos.y = 0.f;
	D3DXVec3Cross(&m_vDir, &_vec3(0.f, 1.f, 0.f), &_vec3(m_vCenter - vPos));
	Set_Direction(m_vDir);
	Add_Rotation(_vec3(0.f, -90.f, 0.f));

	if (m_pParticle)
	{
		m_pParticle->Set_Pos(Get_Position());
	}
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);
	return iExit;
}

_int CKuro_Horse::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CKuro_Horse::Render_Object(_int iPass)
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

void CKuro_Horse::Debug_This_In_Console()
{
}

void CKuro_Horse::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix	matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &Get_Transform()->Get_WorldMatrix());
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// 그림자용
	pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망시 필터
	pEffect->SetFloat("g_fFilter", 0.f);
}

void CKuro_Horse::Add_Component()
{
	CComponent*		pComponent = nullptr;

	wstring wstrMeshName = L"Kuro_Horse.X";
	wstring wstrMeshPath = L"../../Reference/Resource/Others/Kuro/Kuro_Horse/";

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

void CKuro_Horse::Movement(const _float & fTimeDelta)
{
	_matrix matRotY;
	
	m_fStartAngle -= fTimeDelta *m_fSpeed;
	D3DXMatrixRotationY(&matRotY, m_fStartAngle*Deg2Rad);

	_vec3 vDir;
	D3DXVec3TransformNormal(&vDir, &VEC_LOOK, &matRotY);
	D3DXVec3Normalize(&vDir, &vDir);

	
	Set_Pos(m_vCenter + vDir * m_fDistance);

	m_fSinValue += fTimeDelta*10.f;

	Set_Pos(_vec3(Get_Position().x, sinf(m_fSinValue) * 0.2f, Get_Position().z));
}

void CKuro_Horse::OnEnable()
{
	CAttackObj::OnEnable();
}

void CKuro_Horse::OnDisable()
{
	CAttackObj::OnDisable();
}

void CKuro_Horse::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CKuro_Horse::On_Collision(CGameObject * pDest)
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

		static_cast<CCharacter*>(pDest)->Set_Gravity(vKnockBackDir, 0.1f);
		CUtility::Print_Wstring_DMode(L"Character가 쿠로 말에 맞음");
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
