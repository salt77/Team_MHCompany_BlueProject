#include "Chesed_Normal_Bullet.h"

#include "Run_Player.h" 
#include "Run_Map.h" 
#include "EffectObject.h" 
#include "Tanmak_Effect.h" 
#include "Bullet_Dead.h" 
#include "Bullet_Run_Normal.h" 
#include "Run_Chesed.h" 

#include "Texture.h"
#include "Renderer.h"
#include "ProtoMgr.h"
#include "KeyManager.h"
#include "CollisionManager.h"  
#include "UI_Manager.h" 
#include "TimeScale_Manager.h" 
#include "Sound_Manager.h" 

USING(Engine)

Chesed_Normal_Bullet::Chesed_Normal_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
}

Chesed_Normal_Bullet::Chesed_Normal_Bullet(const Chesed_Normal_Bullet & rhs)
	: CAttackObj(rhs)
{
}

Chesed_Normal_Bullet::~Chesed_Normal_Bullet()
{
}

Chesed_Normal_Bullet * Chesed_Normal_Bullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
													const _vec3& vDir, 
													const _float& fRunAngle, 
													const _float& fPipeAngle, 
													const E_CHESED_ATKTYPE& eAtkType, 
													const _float& fPipeAngleRandValue,
													const E_TANMAKTYPE& eType,
													E_LAYERTAG eTargetLayerTag)
{
	Chesed_Normal_Bullet*	pInstance = new Chesed_Normal_Bullet(pGraphicDev, eTargetLayerTag); 

	if (FAILED(pInstance->Ready_Object(vDir, fRunAngle, fPipeAngle, fPipeAngleRandValue, eAtkType, eType)))
		Safe_Release(pInstance); 

	return pInstance;
}

void Chesed_Normal_Bullet::Free()
{
	if (m_pTanmakEffect) 
	{
		m_pTanmakEffect->Set_Dead(TRUE);
		m_pTanmakEffect = nullptr; 
	}

	CAttackObj::Free(); 
}

void Chesed_Normal_Bullet::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte); 
}

void Chesed_Normal_Bullet::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte); 
}


HRESULT Chesed_Normal_Bullet::Ready_Object(const _vec3& vDir, 
										   const _float& fRunAngle, 
										   const _float& fPipeAngle, 
										   const _float& fPipeAngleRandValue,
										   const E_CHESED_ATKTYPE& eAtkType, 
										   const E_TANMAKTYPE& eType)
{
	CAttackObj::Ready_Object(); 

	m_eTanmakType = eType; 

	Add_Component(); 

	m_bColUseY = TRUE; 

	m_fScale = 25.f; 
	Set_Scale(VEC_ONE * m_fScale);
	m_pEffectObject->Set_Effect_Scale(VEC_ONE * m_fScale);
	m_pSphereColliderCom->SetRadius(0.3f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	m_vMoveDir = *D3DXVec3Normalize(&m_vMoveDir, &vDir);
	m_fRunAngle = fRunAngle; 
	switch (eAtkType)
	{
	case E_CHESED_ATKTYPE::CHESED_ATK_SPREE: 
		m_fPipeAngle = fPipeAngle + CWall512::GetInstance()->GetFloatValue(-fPipeAngleRandValue, fPipeAngleRandValue); 
		break; 

	default:
		m_fPipeAngle = fPipeAngle;
		break;
	}
	
	m_eAtkType = eAtkType; 
	m_eTargetLayerTag = E_LAYERTAG::Character; 

	m_fSkillCountTime = m_fArriveTime; 

	m_fNowLifeTime = 999.f; 

	return S_OK;
}

_int Chesed_Normal_Bullet::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta); 

	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player)); 
	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map)); 
	
	if (!m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CChesed_Normal_Bullet -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음"); 
	}

	m_fSpeed = m_pPlayer->Get_OriginSpeed(); 

	if (E_CHESED_ATKTYPE::CHESED_ATK_SPREE == m_eAtkType)
	{
		m_fRunSpeed = m_pPlayer->Get_OriginSpeed() * m_fRunSpeedInterpol * 0.85f; 
	}
	else
	{
		m_fRunSpeed = m_pPlayer->Get_OriginSpeed() * m_fRunSpeedInterpol; 
	}

	if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_Monster)) 
	{

	}
	else if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster)) 
	{
		Set_Pos(static_cast<CRun_Chesed*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Run_BossMonster))->Get_WeaponPos());
	}

	_vec3	vDir = VEC_UP; 
	CUtility::Rotate_DirVector(m_pPlayer->Get_RunAngle() + 30.f, VEC_RIGHT, &vDir); 

	_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid(); 
	_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir); 

	CUtility::Rotate_DirVector(m_pPlayer->Get_PipeAngle() + CWall512::GetInstance()->GetFloatValue(-10.f, 10.f), vAxisZ, &vDir); 
	m_vTargetPos = vPipeMidPos + vDir * m_pPlayer->Get_DistancePipeRadius(); 

	m_vMoveDir = *D3DXVec3Normalize(&m_vMoveDir, &(m_vTargetPos - Get_Position()));
	m_fDistanceToTarget = D3DXVec3Length(&(Get_Position() - m_vTargetPos));

	m_pTanmakEffect = CTanmak_Effect::Create(m_pGraphicDev); 
	assert(m_pTanmakEffect); 
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, m_pTanmakEffect); 
	m_pTanmakEffect->Set_Pos(Get_Position()); 

	return iExit;
}

_int Chesed_Normal_Bullet::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta); 

	if (Get_Dead())
	{
		return iExit;
	}

	m_fSkillCountTime -= fTimeDelta; 
	0.15f >= m_fSkillCountTime ? 0.15f : m_fSkillCountTime;

	Movement(fTimeDelta); 
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	if (m_pEffectObject)
	{
		m_pEffectObject->Update_Object(fTimeDelta); 
		m_pEffectObject->Set_Effect_Pos(Get_Position()); 
	}
	if (m_pTanmakEffect)
	{
		m_pTanmakEffect->Set_Pos(Get_Position()); 
	}

	Check_Dead(); 

	return iExit;
}

_int Chesed_Normal_Bullet::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta); 

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	if (m_pEffectObject)
	{
		m_pEffectObject->LateUpdate_Object(fTimeDelta); 
	}

	//CRenderer::GetInstance()->Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_ALPHA, this); 

	//0205_alpha_pass_change_byEH_랜더 문제시 문의
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this); 

	//CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereColliderCom, E_LAYERTAG::Character, TRUE); 

	return iExit;
}

void Chesed_Normal_Bullet::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;

	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Alpha, iPass));

	if (m_pBufferCom) 
	{
		m_pBufferCom->Render_Buffer(); 
	}
	
	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
		m_pSphereColliderCom->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void Chesed_Normal_Bullet::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest); 

	if (E_LAYERTAG::Character == pDest->Get_LayerTag())
	{
		static_cast<CRun_Player*>(pDest)->Set_Damage(-(m_fDamageStandard + CWall512::GetInstance()->GetFloatValue(-m_fDamageRandRange, m_fDamageRandRange)));
		CUI_Manager::GetInstance()->Get_ApplyRunPlayerHpText();
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SE_Laser_01.wav", 0.85f);

		m_bDead = TRUE;
		Create_Bullet_Dead(); 
		CCamera::m_pMainCam->Set_ShakeCamera(3.f, 3.f);
		CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, 0.75f); 
	}
	else if (E_LAYERTAG::AttackObj_Player == pDest->Get_LayerTag() &&
			 E_TANMAKTYPE::TANMAK_DESTROY == m_eTanmakType)
	{
		m_fLife -= static_cast<CBullet_Run_Normal*>(pDest)->Get_Damage();

		if (0.f >= m_fLife) 
		{
			Create_Bullet_Dead(); 

			m_bDead = TRUE;
			pDest->Set_Dead(TRUE); 
		}
	}
}

void Chesed_Normal_Bullet::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj, matBill, matScale, matTrans;
	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//0204mat
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	if (m_pTextureCom) 
	{
		m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);
	}

	matBill = matView; 
	matBill._41 = 0.f; 
	matBill._42 = 0.f; 
	matBill._43 = 0.f; 
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	pEffect->SetMatrix("g_matWorld", &(matBill * matWorld));
	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);
}

void Chesed_Normal_Bullet::Set_Constantable_Fog(CShader * pShader)
{
}

void Chesed_Normal_Bullet::Add_Component()
{
	CComponent*		pComponent = nullptr;

	//m_pBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));

	//switch (m_eTanmakType)
	//{
	//case Engine::E_TANMAKTYPE::TANMAK_NORMAL:
	//	m_pTextureCom = static_cast<CTexture*>(Emplace_Component(L"Com_Texture",
	//		CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Chesed/Chesed_Tanmak.png", E_TEXTURE_TYPE::TEX_NORMAL)));
	//	break;

	//case Engine::E_TANMAKTYPE::TANMAK_DESTROY:
	//	m_pTextureCom = static_cast<CTexture*>(Emplace_Component(L"Com_Texture",
	//		CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Chesed/Chesed_Tanmak_3.png", E_TEXTURE_TYPE::TEX_NORMAL)));
	//	break;

	//case Engine::E_TANMAKTYPE::TANMAK_SLOW:
	//	m_pTextureCom = static_cast<CTexture*>(Emplace_Component(L"Com_Texture",
	//		CTexture::Create(m_pGraphicDev, L"../../Reference/Resource/Others/Chesed/Chesed_Tanmak.png", E_TEXTURE_TYPE::TEX_NORMAL)));
	//	break;

	//default:
	//	assert(0); 
	//	break;
	//} 

	if (E_TANMAKTYPE::TANMAK_DESTROY != m_eTanmakType)
	{
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_New.effect");
	}
	else
	{
		m_pEffectObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"JY_Chesed_Bullet_New_Blue.effect");
	}

	assert(m_pEffectObject);

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
	//m_pEffectShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_EffectShader", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect"))); 
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void Chesed_Normal_Bullet::Movement(const _float & fTimeDelta)
{
	if (!m_pPlayer || !m_pRunMap)
	{
		return; 
	}

	if (E_CHESED_ATKTYPE::CHESED_ATK_DEPLOY == m_eAtkType) 
	{
		m_fDistanceToTarget = D3DXVec3Length(&(Get_Position() - m_vTargetPos));

		_vec3	vDir = *D3DXVec3Normalize(&vDir, &(Get_Position() - m_vTargetPos)); 
		_vec3	vTargetPos = m_vTargetPos + vDir * (m_fDistanceToTarget * (m_fSkillCountTime / m_fArriveTime));

		if (0.15f < m_fSkillCountTime)
		{
			Set_Pos(vTargetPos);
		}
	}
	else if (E_CHESED_ATKTYPE::CHESED_ATK_ROUNDED == m_eAtkType) 
	{
		if (m_pPlayer->Get_DistancePipeRadius() * m_fDistanceFromPipeMidInterpol > m_fDistanceRounded)
		{
			m_fRunAngle += m_fRunSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
			m_fPipeAngle += m_fRoundedSpeed * fTimeDelta;
			m_fDistanceRounded += m_fRoundedSpeed * fTimeDelta;
		}
		else
		{
			m_fRunAngle += m_fRunSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
		}
		
		_vec3	vDir = VEC_UP; 
		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir); 
		_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid(); 
		_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir); 
		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vDir); 

		Set_Pos(vPipeMidPos + vDir * m_fDistanceRounded); 
	}
	else if (E_CHESED_ATKTYPE::CHESED_ATK_NORMAL == m_eAtkType) 
	{

	}
	else if (E_CHESED_ATKTYPE::CHESED_ATK_SPREE == m_eAtkType) 
	{
		m_fRunAngle += m_fRunSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();

		if (m_pPlayer->Get_DistancePipeRadius() * m_fDistanceFromPipeMidInterpol > m_fDistanceRounded)
		{
			m_fDistanceRounded += m_fRoundedSpeed * fTimeDelta;
		}

		_vec3	vDir = VEC_UP; 
		CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir); 
		_vec3	vPipeMidPos = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid(); 
		_vec3	vAxisZ = *D3DXVec3Cross(&vAxisZ, &VEC_RIGHT, &vDir); 
		CUtility::Rotate_DirVector(m_fPipeAngle, vAxisZ, &vDir); 

		Set_Pos(vPipeMidPos + vDir * m_fDistanceRounded); 
	}
}

void Chesed_Normal_Bullet::Create_Bullet_Dead()
{
	CGameObject*	pGameObject = nullptr;

	switch (m_eTanmakType)
	{
	case Engine::E_TANMAKTYPE::TANMAK_NORMAL:
		pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_RED);
		break;

	case Engine::E_TANMAKTYPE::TANMAK_DESTROY:
		pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_BLUE);
		break;

	case Engine::E_TANMAKTYPE::TANMAK_SLOW:
		pGameObject = CBullet_Dead::Create(m_pGraphicDev, E_RUN_BULLETTYPE::BULLET_RED);
		break;

	default:
		MSG_BOX("Chesed_Normal_Bullet -> Create_Bullet_Dead() : m_eTanmakType is default");
		assert(0);
		break;
	}

	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	pGameObject->Set_Pos(Get_Position());
	pGameObject->Set_Scale(Get_Scale());
}

void Chesed_Normal_Bullet::Check_Dead()
{
	if (!m_pPlayer) 
	{
		return; 
	}

	if (m_pPlayer->Get_RunAngle() - m_fDeadAngle >= m_fRunAngle) 
	{
		m_bDead = TRUE; 

		//if (m_pTanmakEffect)
		//{
		//	m_pTanmakEffect->Set_Dead(TRUE); 
		//	m_pTanmakEffect = nullptr; 
		//}
	}
}
