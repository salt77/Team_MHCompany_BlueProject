#include "QuatCamera.h" 

#include "Run_Player.h" 
#include "Run_Map.h" 
#include "Run_Chesed.h"
#include "ParticleObject.h" 

#include "TimeScale_Manager.h" 
#include "UI_Manager.h" 

CQuatCamera::CQuatCamera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd, E_OBJTAG eObjTag)
	: CCamera(pGraphicDev, eObjTag)
	, m_Hwnd(hWnd)
{
	D3DXQuaternionIdentity(&m_quatRot);
}

CQuatCamera::CQuatCamera(const CQuatCamera& rhs)
	: CCamera(rhs)
{
}

CQuatCamera::~CQuatCamera()
{
}

CQuatCamera* CQuatCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	HWND hWnd,
	T_CAMINFO tCamInfo,
	E_OBJTAG eObjTag)
{
	CQuatCamera*		pInstance = new CQuatCamera(pGraphicDev, hWnd, eObjTag);

	if (FAILED(pInstance->Ready_Object(tCamInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CQuatCamera::Free()
{
	if (m_pFadeInOutUI)
	{
		if (!m_pFadeInOutUI->Get_Dead())
		{
			m_pFadeInOutUI->Set_Dead(TRUE);
		}

		m_pFadeInOutUI = nullptr;
	}
	if (m_pParticle_Wind)
	{
		m_pParticle_Wind->Set_ParticleDead();
		m_pParticle_Wind->Set_Dead(TRUE);
		m_pParticle_Wind = nullptr;
		//Safe_Release(m_pParticle_Wind); 
	}

	Safe_Release(m_pTexture);
	Safe_Release(m_pSphereMesh);

	CCamera::Free();
}


HRESULT CQuatCamera::Ready_Object(T_CAMINFO tCamInfo)
{
	m_tCamInfo.vEye = tCamInfo.vEye;
	m_tCamInfo.vAt = tCamInfo.vAt;

	m_tCamInfo.fFov = tCamInfo.fFov;
	m_tCamInfo.fAspect = tCamInfo.fAspect;
	m_tCamInfo.fNear = tCamInfo.fNear;
	m_tCamInfo.fFar = tCamInfo.fFar;

	_matrix		matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_tCamInfo.matView);
	//D3DXQuaternionRotationMatrix(&m_quatRot, &matWorld);

	memcpy(&m_tCamInfo.vRight, &matWorld.m[0][0], sizeof(_vec3));
	m_tCamInfo.vUp = *D3DXVec3Cross(&m_tCamInfo.vUp, &Get_CamLookNormalize(), &m_tCamInfo.vRight);

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);

	Add_Component();

	if (FAILED(D3DXCreateSphere(m_pGraphicDev, 0.5f, 10, 10, &m_pSphereMesh, nullptr)))
		return E_FAIL;

	if (FAILED(D3DXCreateTexture(m_pGraphicDev,
								 1,
								 1,
								 1,
								 0,
								 D3DFMT_A8R8G8B8,
								 D3DPOOL_MANAGED,
								 &m_pTexture)))
								 return E_FAIL;

	D3DLOCKED_RECT LockRect;
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pTexture->UnlockRect(0);

	return S_OK;
}

_int CQuatCamera::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CCamera::Start_Object(fTimeDelta);

	m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));
	m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));

	if (!m_pPlayer || !m_pRunMap)
	{
		MSG_BOX("CQuat_Camera -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음");
	}

	m_pFadeInOutUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/[Fixed]Fade.uidat");
	assert(m_pFadeInOutUI);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI_Combat, E_OBJTAG::OBJ_UI, m_pFadeInOutUI);
	m_pFadeInOutUI->Set_Active(TRUE);
	m_pFadeInOutUI->Set_ImageAlpha(0.f);

	return iExit;
}

Engine::_int CQuatCamera::Update_Object(const _float& fTimeDelta)
{
	m_pTransformCom->Set_Pos(m_tCamInfo.vEye);
	m_pTransformCom->Update_Component(fTimeDelta);
	Rotate_Cam(fTimeDelta);
	CutScene_View(fTimeDelta);
	EndGame_View(fTimeDelta); 

	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::Update_Object(fTimeDelta);

	Key_Input(fTimeDelta);
	Update_MousePos();

	if (m_pParticle_Wind)
	{
		m_pParticle_Wind->Set_Pos(m_tCamInfo.vAt + m_pPlayer->Get_VecAxis(E_ROTATION::ROT_Y) * 10.f);
		m_pParticle_Wind->Set_Direction(m_tCamInfo.vEye - m_tCamInfo.vAt);
	}

	return iExit;
}

_int CQuatCamera::LateUpdate_Object(const _float & fTimeDelta)
{
	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	if (m_eMyCamType != CCamera::Get_MainCamType())
		return RETURN_OK;

	_int	iExit = CCamera::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CQuatCamera::Render_Object(_int iPass)
{
	if (m_bRenderSphere)
	{
		Set_Constantable();

		m_pSphereShaderCom->BeginShader();
		m_pSphereShaderCom->BeginPass(ShaderPass(E_ShaderPass::Debug, 0));

		m_pSphereMesh->DrawSubset(0);

		m_pSphereShaderCom->EndPass();
		m_pSphereShaderCom->EndShader();
	}
}

void CQuatCamera::Set_StartGame_Camera()
{
	m_bCutSceneView = TRUE;

	if (!m_pPlayer)
	{
		m_pPlayer = static_cast<CRun_Player*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Run_Player));
		m_pRunMap = static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map));

		if (!m_pPlayer || !m_pRunMap)
		{
			MSG_BOX("CQuat_Camera -> Start_Object() : m_pPlayer 또는 m_pRunMap 없음");
		}
	}
	m_fRunAngle = m_pPlayer->Get_RunAngle() - 225.f;

	m_pParticle_Wind = CParticleObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Particle, L"Run_Effect.particle");
	assert(m_pParticle_Wind);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Particle, m_pParticle_Wind);
	m_pParticle_Wind->Set_Pos(m_tCamInfo.vAt);
	m_pParticle_Wind->Set_Scale(m_pPlayer->Get_Scale() * 100.f);
}

void CQuatCamera::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader
	m_pSphereShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));
}

void CQuatCamera::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pSphereShaderCom->Get_EffectHandle();

	//0204mat
	//_matrix		matView, matProj;
	//
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix());

	//0204mat
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_BaseTexture", m_pTexture);
}

// 기본 이동 및 마우스 Lock & Unlock 
void CQuatCamera::Key_Input(const _float& fTimeDelta)
{
	if (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) && CKeyManager::GetInstance()->Key_Down(KEY_R))
	{
		m_bRenderSphere = !m_bRenderSphere;
	}
}

// 마우스 포지션 업데이트 
void CQuatCamera::Update_MousePos()
{
	POINT pPoint = CUtility::Get_MousePoint(m_Hwnd);
	ClientToScreen(m_Hwnd, &pPoint);

	m_vMovePos.x = pPoint.x - m_vCenterPos.x;
	m_vMovePos.y = pPoint.y - m_vCenterPos.y;

	m_vPtMousePos.x = (_float)pPoint.x;
	m_vPtMousePos.y = (_float)pPoint.y;
}

// 회전 
void CQuatCamera::Rotate_Cam(const _float& fTimeDelta)
{
	if (!m_pPlayer || m_bCutSceneView || m_bEndGame)
	{
		return;
	}

	_matrix		matRot;
	D3DXMatrixIdentity(&matRot);

	_vec3		vCamDir = VEC_ZERO;
	_vec3		vCamTargetDir = VEC_BACK;

	D3DXQuaternionSlerp(&m_quatRot, &m_quatRot, &m_pPlayer->Get_QuatRotation(), 0.05f);
	D3DXMatrixRotationQuaternion(&matRot, &m_quatRot);

	D3DXVec3TransformNormal(&vCamTargetDir, &vCamTargetDir, &matRot);
	CUtility::Rotate_DirVector(m_fCamAngle, m_pPlayer->Get_VecAxis(E_ROTATION::ROT_X), &vCamTargetDir);

	m_vTargetEyePos = m_pPlayer->Get_Position() + vCamTargetDir * m_fCamDistance;
	m_fSpeed = D3DXVec3Length(&(m_tCamInfo.vEye - m_vTargetEyePos)) * m_fFollowSpeed * CTimeScale_Manager::GetInstance()->Get_TimeScale();
	vCamDir = *D3DXVec3Normalize(&m_vTemp, &(m_vTargetEyePos - m_tCamInfo.vEye));
	m_tCamInfo.vEye += vCamDir * m_fSpeed;

	_vec3	vUp = VEC_UP;
	D3DXVec3TransformNormal(&m_tCamInfo.vUp, &vUp, &matRot);

	m_tCamInfo.vAt = m_pPlayer->Get_Position() + m_pPlayer->Get_VecAxis(E_ROTATION::ROT_Y) * 30.f + m_vShakeInterpol - m_vPreShakeInterpol;
	m_vPreShakeInterpol = m_vShakeInterpol;
}

void CQuatCamera::CutScene_View(const _float & fTimeDelta)
{
	if (!m_bCutSceneView)
	{
		return;
	}

	assert(m_pPlayer);
	assert(m_pRunMap);

	m_fRunAngle += m_pPlayer->Get_OriginSpeed() * fabs(m_pPlayer->Get_RunAngle() - m_fRunAngle) * m_fCutSceneSpeed; 
	m_fCutSceneSpeed += 0.005f * fTimeDelta; 

	_vec3	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fRunAngle, VEC_RIGHT, &vDir);
	m_tCamInfo.vEye = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();
	m_tCamInfo.vUp = m_tCamInfo.vEye - vDir * m_pPlayer->Get_DistancePipeRadius();

	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_fRunAngle + 10.f, VEC_RIGHT, &vDir);
	m_tCamInfo.vAt = m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid();

	if (35.f >= fabs(m_fRunAngle - m_pPlayer->Get_RunAngle()))
	{
		if (m_pFadeInOutUI)
		{
			if (0.f >= m_pFadeInOutUI->Get_ImageColor().a)
			{
				m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, 0.75f);
				m_pFadeInOutUI->Set_ImageAlpha(0.01f);
			}
		}
	}
	if (1.f <= m_pFadeInOutUI->Get_ImageColor().a)
	{
		m_bCutSceneView = FALSE;

		m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, 0.75f);
		static_cast<CRun_Map*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Environment, E_OBJTAG::OBJ_Run_Map))->Set_GameStart();
		m_pPlayer->Set_GameRender(); 
	}
}

void CQuatCamera::EndGame_View(const _float & fTimeDelta)
{
	if (!m_bEndGame)
	{
		return; 
	}

	m_fEndGameInterpolAngle += 10.f * fTimeDelta; 

	_vec3	vDir = VEC_UP; 
	CUtility::Rotate_DirVector(m_pPlayer->Get_RunAngle() + m_fEndGameInterpolAngle, VEC_RIGHT, &vDir); 

	Set_CamEye(m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid());

	vDir = VEC_UP;
	CUtility::Rotate_DirVector(m_pPlayer->Get_RunAngle() + m_fEndGameInterpolAngle + 15.f, VEC_RIGHT, &vDir); 

	Set_CamAt(m_pRunMap->Get_Position() + vDir * m_pPlayer->Get_DistanceOriginToPipeMid()); 
}

// 커서 중앙 고정 
void CQuatCamera::Mouse_Fix()
{
	POINT		pPoint = { WINCX >> 1, WINCY >> 1 };

	ClientToScreen(m_Hwnd, &pPoint);
	SetCursorPos(pPoint.x, pPoint.y);

	m_vCenterPos.x = (_float)pPoint.x;
	m_vCenterPos.y = (_float)pPoint.y;
}
