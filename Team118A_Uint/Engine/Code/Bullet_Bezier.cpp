#include "Bullet_Bezier.h"

#include "Unit.h"

CBullet_Bezier::CBullet_Bezier(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CAttackObj(pGraphicDev, eTargetLayerTag)
{
	ZeroMemory(m_vBezier, sizeof(_vec3) * 4);
	m_bColUseY = TRUE;
}

CBullet_Bezier::CBullet_Bezier(const CBullet_Bezier & rhs)
	: CAttackObj(rhs)
{
}

CBullet_Bezier::~CBullet_Bezier()
{
}

CBullet_Bezier * CBullet_Bezier::Create(LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType, E_LAYERTAG eTargetLayerTag)
{
	CBullet_Bezier*		pInstance = new CBullet_Bezier(pGraphicDev, eTargetLayerTag);

	if (FAILED(pInstance->Ready_Object(tUnitInfo, eAtkType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBullet_Bezier::Free()
{
	CAttackObj::Free(); 
}

void CBullet_Bezier::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CBullet_Bezier::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CBullet_Bezier::Ready_Object(const T_UnitInfo& tUnitInfo, const E_CHAR_ATKTYPE& eAtkType)
{
	CAttackObj::Ready_Object();

	Add_Component();

	m_eAtkType = eAtkType;
	m_tUnitInfo = tUnitInfo;

	m_pSphereColliderCom->SetRadius(0.1f);
	m_pSphereColliderCom->SetScale(Get_Scale());

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Bulletline_07.png", &m_pTexture);

	m_pTrail = CTrail_Fix::Create(m_pGraphicDev, L"../../Reference/Resource/Effect/Tex_Single/FX_TEX_Stretch_Line_13.png", 1.f);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Trail, m_pTrail);
	m_pTrail->Set_FadeOutSize(FALSE);
	m_pTrail->Set_FadeOutAlpha(FALSE);
	m_pTrail->Set_UV_X_Inverse(TRUE);
	return S_OK;
}

_int CBullet_Bezier::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CBullet_Bezier::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	if (Get_IsDelayed())
	{
		m_fDelayTime -= fTimeDelta;
		return iExit;
	}

	m_fCountingTime += fTimeDelta;

	Movement(fTimeDelta);
	m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());

	m_pTrail->Add_Vertex(Get_Position() + _vec3(0.f, 0.2f, 0.f), Get_Position() + _vec3(0.f, -0.2f, 0.f));
	if (m_fTotalTime <= m_fCountingTime)
	{
		Set_Dead(TRUE);
		return RETURN_DEAD;
	}


	return iExit;
}

_int CBullet_Bezier::LateUpdate_Object(const _float & fTimeDelta)
{
	if (Get_IsDelayed())
	{
		return RETURN_OK;
	}
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	Compute_ViewZ(&Get_Info(E_INFO::INFO_POS));

	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_BILLBOARD, this);
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CBullet_Bezier::Render_Object(_int iPass)
{
	if (Get_Dead())
		return;
}

void CBullet_Bezier::On_Collision(CGameObject * pDest)
{
	static_cast<CUnit*>(pDest)->Set_Damage(Get_UnitInfo(), Get_AtkType());

	m_bDead = TRUE; 

	return; 
}

void CBullet_Bezier::Set_Bezier(const _float & fTotalTime, const _vec3 & P1, const _vec3 & P2, const _vec3 & P3, const _vec3 & P4)
{
	m_fTotalTime = fTotalTime;
	m_vBezier[0] = P1;
	m_vBezier[1] = P2;
	m_vBezier[2] = P3;
	m_vBezier[3] = P4;
}

list<CBullet_Bezier*>* CBullet_Bezier::Create_Bezier(_int iCount, _int iCountEveryInterval, 
	const _float & fTotalTimeMin, const _float & fTotalTimeMax, const _float& fDelayMin, const _float& fDelayMax,
	const _vec3 & vStart, const _vec3 & vEnd, const _vec3 & vStartRand, const _vec3 & vEndRand, 
	LPDIRECT3DDEVICE9 pGraphicDev, const T_UnitInfo & tUnitInfo, const E_CHAR_ATKTYPE & eAtkType, E_LAYERTAG eTargetLayerTag)
{
	list<CBullet_Bezier*>* pList = new list<CBullet_Bezier*>;

	for (_int i = 0; i < iCount; ++i)
	{
		_float fDelay = CWall512::GetInstance()->GetFloatValue(fDelayMin, fDelayMax);
		_float fTotalTime = CWall512::GetInstance()->GetFloatValue(fTotalTimeMin, fTotalTimeMax);
		for (_int  j = 0; j < iCountEveryInterval; ++j)
		{
			CBullet_Bezier* pBezier = CBullet_Bezier::Create(pGraphicDev, tUnitInfo, eAtkType, eTargetLayerTag);

			_vec3 vP2 = vStart + _vec3(CWall512::GetInstance()->GetFloatValue(-vStartRand.x,	+vStartRand.x),
										CWall512::GetInstance()->GetFloatValue(-vStartRand.y * 0.5f,			+vStartRand.y), 
										CWall512::GetInstance()->GetFloatValue(-vStartRand.z,					+vStartRand.z));
			_vec3 vP3 = ((vStart + vEnd) * 0.5f) + _vec3(CWall512::GetInstance()->GetFloatValue(-vEndRand.x,	+vEndRand.x), 
										CWall512::GetInstance()->GetFloatValue(-vEndRand.y * 0.5f,				+vEndRand.y), 
										CWall512::GetInstance()->GetFloatValue(-vEndRand.z,						+vEndRand.z));
			pBezier->Set_Pos(vStart);
			pBezier->Set_LifeTime(fTotalTime);
			pBezier->Set_Bezier(fTotalTime, vStart, vP2, vP3, vEnd);
			pBezier->Set_DelayTime(fDelay);
			pList->push_back(pBezier);
		}
	}

	return pList;
}

void CBullet_Bezier::Add_Component()
{
	CComponent*		pComponent = nullptr;

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect")));
	m_pColliderShaderCom = static_cast<CShader*>(Emplace_Component(L"Com_Shader2", CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base")));

	// Collider
	pComponent = m_pSphereColliderCom = CSphereCollider::Create(m_pGraphicDev, this);
	assert(pComponent);
	m_mapComponent.emplace(ComTag_Collider, pComponent);
}

void CBullet_Bezier::Set_Constantable()
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

void CBullet_Bezier::Movement(const _float & fTimeDelta)
{
	// 0 -> 1
	_float fRatio = 1.f - Get_LifeRatio();

	if (fRatio < 1.f)
	{
		_float fNextRatio = 1.f - ((m_fNowLifeTime - fTimeDelta) / m_fTotalLifeTime);

		_vec3 vCurrentDir = CUtility::Bezier(fRatio, m_vBezier[0], m_vBezier[1], m_vBezier[2], m_vBezier[3]);
		_vec3 vNextDir = CUtility::Bezier(fNextRatio, m_vBezier[0], m_vBezier[1], m_vBezier[2], m_vBezier[3]);
		m_vDir = vCurrentDir - vNextDir;
		Set_Pos(vCurrentDir);
	}
	else
	{

	}

	// Move_Pos(m_vDir, 1.f, 1.f);
}
