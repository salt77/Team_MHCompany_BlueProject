#include "Management.h"

USING(Engine)

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

CManagement::~CManagement()
{
	Free();
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
	Release_Managers();
}

HRESULT CManagement::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9 viewPort;
	pGraphicDev->GetViewport(&viewPort);

	// 렌더 타겟 생성
	_float fWidth = (float)WINCX / 5.f;
	_float fHeight = (float)WINCY / 5.f;

	// Deferred_Color
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Albedo",	viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Albedo",			fWidth * 0.f, fHeight * 0.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Emission",	viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Emission",		fWidth * 0.f, fHeight * 1.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Normal", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Normal",			fWidth * 0.f, fHeight * 2.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Depth", viewPort.Width, viewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.f, 1.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Depth",			fWidth * 0.f, fHeight * 3.f, fWidth, fHeight), E_FAIL);

	// Shadow
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Shadow", 8000, 6000, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.f, 1.f, 1.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Shadow", fWidth * 0.f, fHeight * 4.f, fWidth, fHeight), E_FAIL);

	// LightAcc
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Shade",	viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Shade", fWidth * 1.f, fHeight * 0.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Specular",	viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Specular", fWidth * 1.f, fHeight * 1.f, fWidth, fHeight), E_FAIL);

	// Effect
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Effect", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Effect", fWidth * 2.f, fHeight * 0.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Effect_Blur", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Effect_Blur", fWidth * 2.f, fHeight * 1.f, fWidth, fHeight), E_FAIL);

	// Blend_End
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Blend_Image", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Blend_Image", fWidth * 1.f, fHeight * 2.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Color_Image", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Color_Image", fWidth * 1.f, fHeight * 3.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Bright_Image", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Bright_Image", fWidth * 1.f, fHeight * 4.f, fWidth, fHeight), E_FAIL);

	// Blur
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Blur", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Blur", fWidth * 2.f, fHeight * 4.f, fWidth, fHeight), E_FAIL);
	
	// JustRay
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_Ray_Image", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_Ray_Image", fWidth * 2.f, fHeight * 2.f, fWidth, fHeight), E_FAIL);

	// GodRay
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_GodRay_Image", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_GodRay_Image", fWidth * 2.f, fHeight * 3.f, fWidth, fHeight), E_FAIL);

	// ShowWindow
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow", fWidth * 3.f, fHeight * 0.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Emission", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Emission", fWidth * 3.f, fHeight * 1.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Normal", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Normal", fWidth * 3.f, fHeight * 2.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Depth", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Depth", fWidth * 3.f, fHeight * 3.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Blur", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Blur", fWidth * 3.f, fHeight * 4.f, fWidth, fHeight), E_FAIL);

	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Shade", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Shade", fWidth * 4.f, fHeight * 0.f, fWidth, fHeight), E_FAIL);
	FAILED_CHECK_RETURN(Ready_RenderTarget(pGraphicDev, L"Target_ShowWindow_Spec", viewPort.Width, viewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_DebugBuffer(L"Target_ShowWindow_Spec", fWidth * 4.f, fHeight * 1.f, fWidth, fHeight), E_FAIL);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 멀티 렌더 그룹 생성

	// 디퍼드 렌더러 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred_Color", L"Target_Albedo"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred_Color", L"Target_Emission"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred_Color", L"Target_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Deferred_Color", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Shadow", L"Target_Shadow"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), E_FAIL);

	// 이펙트 랜더 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Effect", L"Target_Effect"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Effect", L"Target_Effect_Blur"), E_FAIL);

	// 블러 랜더 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blur", L"Target_Blur"), E_FAIL);

	// GodRay 랜더 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Ray", L"Target_Ray_Image"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_GodRay", L"Target_GodRay_Image"), E_FAIL);

	// 후처리 랜더 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blend_End", L"Target_Blend_Image"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blend_End", L"Target_Bright_Image"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_Blend_End", L"Target_Color_Image"), E_FAIL);

	// 쇼윈도우 랜더 타겟
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow", L"Target_ShowWindow"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow", L"Target_ShowWindow_Emission"), E_FAIL); 
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow", L"Target_ShowWindow_Normal"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow", L"Target_ShowWindow_Depth"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow_Shade", L"Target_ShowWindow_Shade"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow_Shade", L"Target_ShowWindow_Spec"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_MRT(L"MRT_ShowWindow_Blur", L"Target_ShowWindow_Blur"), E_FAIL);

	CShader*		pShader = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 디퍼드 쉐이더
	
	// Shader_Base
	pShader = CShader::Create(pGraphicDev, 20, L"../../Reference/Header/Shader_Base.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Base", pShader), E_FAIL);
	
	// Shader_Shade
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_Shade.fx");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Shade", pShader), E_FAIL);

	// Shader_Blend
	pShader = CShader::Create(pGraphicDev, 3, L"../../Reference/Header/Shader_Blend.fx");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Blend", pShader), E_FAIL);

	/////////////////////////////////////////////////////////////////////////////////////////////
	
	// Shader_Effect
	pShader = CShader::Create(pGraphicDev, 2, L"../../Reference/Header/Shader_Effect.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Effect", pShader), E_FAIL);

	// Shader_Effect_Blend
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_Effect_Blend.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Effect_Blend", pShader), E_FAIL);
	
	/////////////////////////////////////////////////////////////////////////////////////////////

	// Shader_Ui
	pShader = CShader::Create(pGraphicDev, 2, L"../../Reference/Header/Shader_Ui.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Ui", pShader), E_FAIL);
		
	// Shader_Trail
	pShader = CShader::Create(pGraphicDev, 2, L"../../Reference/Header/Shader_Trail.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Trail", pShader), E_FAIL);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 후처리 쉐이더

	// Blur
	pShader = CShader::Create(pGraphicDev, 2, L"../../Reference/Header/Shader_Blur.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Blur", pShader), E_FAIL);

	// Post_Process
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_PostProcess.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_PostProcess", pShader), E_FAIL);

	// Shader_LightMgr
	pShader = CShader::Create(pGraphicDev, 2, L"../../Reference/Header/Shader_LightMgr.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_LightMgr", pShader), E_FAIL);

	// Shader_GodRay
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_GodRay.fx");
	assert(pShader);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_GodRay", pShader), E_FAIL);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// 쇼윈도우 쉐이더

	// Shader_Show_Base
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_Show_Base.fx");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Show_Base", pShader), E_FAIL);

	// Shader_Show_Blend
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_Show_Blend.fx");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Show_Blend", pShader), E_FAIL);

	// Shader_Show_Shade
	pShader = CShader::Create(pGraphicDev, 1, L"../../Reference/Header/Shader_Show_Shade.fx");
	NULL_CHECK_RETURN(pShader, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Proto_Shader_Show_Shade", pShader), E_FAIL);


	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return RETURN_ERROR;

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) &&
		CKeyManager::GetInstance()->Key_Down(KEY_S))
	{
		CSound_Manager::GetInstance()->Set_Volume_BGM(0.f);
		CSound_Manager::GetInstance()->Set_Volume_FX(0.f);
	}
	else if (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) &&
			 CKeyManager::GetInstance()->Key_Down(KEY_D))
	{
		map<E_OBJTAG, vector<CGameObject*>>* mapObject = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::UI_Lobby); 
		map<E_OBJTAG, vector<CGameObject*>>::iterator	iter = mapObject->begin(); 

		for (; iter != mapObject->end(); ++iter)
		{
			for (_uint i = 0; i < iter->second.size(); ++i) 
			{
				static_cast<CUI_Object*>(iter->second[i])->Set_Active(FALSE); 
			}
		}
	}
	else if (CKeyManager::GetInstance()->Key_Pressing(KEY_Tab) &&
			 CKeyManager::GetInstance()->Key_Down(KEY_F))
	{
		map<E_OBJTAG, vector<CGameObject*>>* mapObject = CScene::Get_MainScene()->Get_LayerMapObject(E_LAYERTAG::UI_Lobby);
		map<E_OBJTAG, vector<CGameObject*>>::iterator	iter = mapObject->begin();

		for (; iter != mapObject->end(); ++iter)
		{
			for (_uint i = 0; i < iter->second.size(); ++i)
			{
				static_cast<CUI_Object*>(iter->second[i])->Set_Active(TRUE);
			}
		}
	}

	_float	fTimeScale = (_int)CGameManager::GetInstance()->Get_BattleSpeed() * 0.5f + 1.f;
	_bool	bSkillOnMouse = CGameManager::GetInstance()->Get_SkillOnMouse();

	_float fGameDeltaTime = fTimeDelta * fTimeScale;

	if (bSkillOnMouse)
	{
		fGameDeltaTime *= 0.3f;
	}

	CGameManager::GetInstance()->Update_GameManager(fGameDeltaTime);
	CTimeScale_Manager::GetInstance()->Update_TimeScale(fGameDeltaTime);
	m_pScene->Update_Scene(fGameDeltaTime);

	CTimeScale_Manager::GetInstance()->Update_TimeScale(fTimeDelta);
	// 아래의 2개의 매니저들은 나중에 fTimeDelta가 아니라 실제시간으로 바꿔줄 필요가 있음
	CSound_Manager::GetInstance()->Update_SoundManager(fTimeDelta);
	CUI_Manager::GetInstance()->Update_UIManager(fTimeDelta);
	CDialog_Manager::GetInstance()->Update_DialogManager(fTimeDelta);
	CActionCam_Mgr::GetInstance()->Update_ActionCamManager(fGameDeltaTime);
	CRenderer::GetInstance()->Render_Update(fTimeDelta);
	CLightMgr::GetInstance()->Update_Light_Mgr(fTimeDelta);

	return RETURN_OK;
}


_int CManagement::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return RETURN_ERROR;

	m_pScene->LateUpdate_Scene(fTimeDelta);

	return RETURN_OK;
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void CManagement::Debugging_Console()
{
#ifdef _DEBUG
	m_pScene->Debugging_Console();
#endif
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);	// 주의합시다!!!!!!!!!!!!

	CDialog_Manager::GetInstance()->End_Dialog(); 

	Clear_RenderGroup();
	Clear_Instance_RenderGroup();
	m_pScene = pScene;

	return S_OK;
}

CScene * CManagement::Get_Scene()
{
	return m_pScene;
}

CComponent * CManagement::Get_Component(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx, const wstring & pComponentTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(eLayerTag, eObjTag, iIdx, pComponentTag);
}

CGameObject * CManagement::Get_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, _uint iIdx)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_GameObject(eLayerTag, eObjTag, iIdx);
}

vector<CGameObject*>* CManagement::Get_vecGameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_vecGameObject(eLayerTag, eObjTag);
}

map<E_OBJTAG, vector<CGameObject*>>* CManagement::Get_LayerMapObject(E_LAYERTAG eLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_LayerMapObject(eLayerTag);
}

void CManagement::Set_Dead_AllObject(E_LAYERTAG eLayerTag)
{
	m_pScene->Set_Dead_AllObject(eLayerTag);
}

void CManagement::Set_Dead_CafeTile(E_LAYERTAG eLayerTag)
{
	m_pScene->Set_Dead_CafeTile(eLayerTag);
}

void CManagement::Set_Dead_CafeWall_L(E_LAYERTAG eLayerTag)
{
	m_pScene->Set_Dead_CafeWall_L(eLayerTag);
}

void CManagement::Set_Dead_CafeWall_R(E_LAYERTAG eLayerTag)
{
	m_pScene->Set_Dead_CafeWall_R(eLayerTag);
}

void CManagement::Add_GameObject(E_LAYERTAG eLayerTag, const E_OBJTAG eObjTag, CGameObject * pGameObject)
{
	pGameObject->Set_LayerTag(eLayerTag);
	m_pScene->Add_GameObject(eLayerTag, eObjTag, pGameObject);
}

void CManagement::Ready_Managers(HWND* pHwnd)
{
	CWarning_Mgr::GetInstance();
	CActionCam_Mgr::GetInstance()->Ready_ActionCamManager(CGraphicDev::GetInstance()->GetDevice());
	CD3DLine::GetInstance()->Ready_Line(CGraphicDev::GetInstance()->GetDevice());
	CFontMgr::GetInstance()->Ready_FontMgr(CGraphicDev::GetInstance()->GetDevice());
	CPrefabManager::GetInstance();
	CGameManager::GetInstance();
	CWall512::GetInstance();
	CDataManager::GetInstance();
	CKeyManager::GetInstance();
	CDialog_Manager::GetInstance()->Ready_DialogManager(); 
	CLightMgr::GetInstance();
	CRenderTargetMgr::GetInstance();
	CProtoMgr::GetInstance();
	//CXL_ParsingMgr::GetInstance()->Ready_Manager(L"../../Reference/Resource/Xlsx/쥬신 블루아카이브 모작.xlsx");
	CTimerMgr::GetInstance();
	CCollisionManager::GetInstance();
	CFogManager::GetInstance();
	CFrameMgr::GetInstance();
	CTimeScale_Manager::GetInstance();
	CRenderer::GetInstance();
	CSound_Manager::GetInstance()->Initialize();
	CCharacterMgr::GetInstance()->Ready_Manager();
	CUI_Manager::GetInstance()->Initialize(pHwnd);
	CSkill_Manager::GetInstance();
	CLoadingManager::GetInstance();
}

void CManagement::Release_Managers()
{
	CLoadingManager::DestroyInstance();
	CWarning_Mgr::DestroyInstance();
	CActionCam_Mgr::DestroyInstance();
	CD3DLine::DestroyInstance();
	CFontMgr::DestroyInstance();
	CPrefabManager::DestroyInstance();
	CGameManager::DestroyInstance();
	CWall512::DestroyInstance();
	CDataManager::DestroyInstance();
	CKeyManager::DestroyInstance();
	CDialog_Manager::DestroyInstance();
	CLightMgr::DestroyInstance();
	CRenderTargetMgr::DestroyInstance();
	CXL_ParsingMgr::DestroyInstance();
	CTimerMgr::DestroyInstance();
	CCollisionManager::DestroyInstance();
	CFogManager::DestroyInstance();
	CFrameMgr::DestroyInstance();
	CTimeScale_Manager::DestroyInstance();
	CRenderer::DestroyInstance();
	CSound_Manager::DestroyInstance();
	CCharacterMgr::DestroyInstance();
	CUI_Manager::DestroyInstance();
	CSkill_Manager::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CGraphicDev::DestroyInstance();
}

HRESULT CManagement::Create_Management(LPDIRECT3DDEVICE9 & pGraphicDev, CManagement ** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", CTransform::Create(pGraphicDev));
	FAILED_CHECK_RETURN(pManagement->Ready_Shader(pGraphicDev), E_FAIL);
	//CRenderer::GetInstance()->Set_RendererShader();

	*ppManagement = pManagement;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////						GraphicMgr
LPDIRECT3DDEVICE9 CManagement::GetDevice()
{
	return CGraphicDev::GetInstance()->GetDevice();
}

HRESULT CManagement::Ready_GraphicDev(HWND hWnd, E_WINMODE eMode, const _uint & iSizeX, const _uint & iSizeY, CGraphicDev ** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

void CManagement::Render_Begin(D3DXCOLOR Color)
{
	return CGraphicDev::GetInstance()->Render_Begin(Color);
}

void CManagement::Render_End()
{
	return CGraphicDev::GetInstance()->Render_End();
}

void CManagement::Render_End(HWND hWnd)
{
	return CGraphicDev::GetInstance()->Render_End(hWnd);
}

_float CManagement::Get_TimeDelta(const wstring& pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}

void CManagement::Set_TimeDelta(const wstring& pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}

HRESULT CManagement::Ready_Timer(const wstring& pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

_bool CManagement::IsPermit_Call(const wstring& pFrameTag, const _float & fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

HRESULT CManagement::Ready_Frame(const wstring& pFrameTag, const _float & fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

_float CManagement::Get_AccTimeDelta(const wstring & pFrameTag)
{
	return CFrameMgr::GetInstance()->Get_AccTimeDelta(pFrameTag);
}

//////////////////////////////////////////////////////////////////////////						TimeScaleMgr
float CManagement::Update_TimeScale(const _float & fTimeDelta)
{
	return CTimeScale_Manager::GetInstance()->Update_TimeScale(fTimeDelta);
}

void CManagement::Reset_TimeScale()
{
	CTimeScale_Manager::GetInstance()->Reset_TimeScale();
}

void CManagement::Set_TimeScale(float fTimeScale)
{
	CTimeScale_Manager::GetInstance()->Set_TimeScale(fTimeScale);
}

void CManagement::Set_TimeScale_Duration(float fTimeScale, float fTimeDuration)
{
	CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(fTimeScale, fTimeDuration);
}

void CManagement::Set_TimeScale_Late(float fTimer, float fTimeScale, float fTimeDuration, bool bSwitch)
{
	CTimeScale_Manager::GetInstance()->Set_TimeScale_Late(fTimer, fTimeScale, fTimeDuration, bSwitch);
}

float CManagement::Get_TimeScale()
{
	return CTimeScale_Manager::GetInstance()->Get_TimeScale();
}

float CManagement::Get_TimeReal()
{
	return  CTimeScale_Manager::GetInstance()->Get_TimeReal();
}

float CManagement::Get_TimeDelta()
{
	return CTimeScale_Manager::GetInstance()->Get_TimeDelta();
}

//////////////////////////////////////////////////////////////////////////						FontMgr
HRESULT CManagement::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & pFontTag, const T_FONT & pStFont)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pStFont);
}

void CManagement::Render_Font(const wstring & pFontTag, const wstring & pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}

//////////////////////////////////////////////////////////////////////////						KeyMgr
void CManagement::UpdateKeyManager(HWND hWnd)
{
	return CKeyManager::GetInstance()->UpdateKeyManager(hWnd);
}

bool CManagement::Key_Up(long long llKey)
{
	return CKeyManager::GetInstance()->Key_Up(llKey);
}

bool CManagement::Key_Down(long long llKey)
{
	return CKeyManager::GetInstance()->Key_Down(llKey);
}

bool CManagement::Key_Pressing(long long llKey)
{
	return CKeyManager::GetInstance()->Key_Pressing(llKey);
}

_vec2 CManagement::Get_MouseMovementFigure()
{
	return CKeyManager::GetInstance()->Get_MouseMovementFigure();
}

//////////////////////////////////////////////////////////////////////////						ProtoMgr
HRESULT CManagement::Ready_Prototype(const wstring& pProtoTag, CComponent * pInstance)
{
	return CProtoMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}

CComponent * CManagement::Clone_Prototype(const wstring& pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Prototype(pProtoTag);
}

HRESULT CManagement::Get_Texture(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrFullPath, LPDIRECT3DTEXTURE9 * ppOut)
{
	return CProtoMgr::GetInstance()->Get_Texture(pGraphicDev, wstrFullPath, ppOut);
}
HRESULT CManagement::Delete_Prototype(const wstring & pProtoTag)
{
	return CProtoMgr::GetInstance()->Delete_Prototype(pProtoTag);
}
//////////////////////////////////////////////////////////////////////////						PrefabManager
_bool CManagement::ReadPrefab(wstring wstrFileName, void * pOut, size_t sizeSize, DWORD * pDwordByte)
{
	return CPrefabManager::GetInstance()->ReadPrefab(wstrFileName, pOut, sizeSize, pDwordByte);
}
void CManagement::FileReset()
{
	CPrefabManager::GetInstance()->FileReset();
}
void CManagement::PrefabReload()
{
	CPrefabManager::GetInstance()->LoadFile();
}


//////////////////////////////////////////////////////////////////////////						Renderer
HRESULT CManagement::Ready_Renderer(LPDIRECT3DDEVICE9 & pGraphicDev)
{	
	return CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
}

CRenderer * CManagement::Get_Renderer()
{
	return CRenderer::GetInstance()->GetInstance();
}

void CManagement::Add_RenderGroup(E_RENDER_ID eRenderID, CGameObject * pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eRenderID, pGameObject);
}

void CManagement::Add_InstanceGroup(E_INSTANCE_RENDER_ID eInstanceRenderID, CGameObject * pGameObject)
{
	CRenderer::GetInstance()->Add_InstanceGroup(eInstanceRenderID, pGameObject);
}

void CManagement::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}

void CManagement::Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

void CManagement::Clear_Instance_RenderGroup()
{
	CRenderer::GetInstance()->Clear_Instance_RenderGroup();
}

void CManagement::Set_Render_Defferd()
{
	CRenderer::GetInstance()->Set_Render_Defferd();
}

void CManagement::Set_Render_Defferd(_bool bState)
{
	CRenderer::GetInstance()->Set_Render_Defferd(bState);
}

_bool CManagement::Get_Debug()
{
	return CRenderer::GetInstance()->Get_Debug();
}

void CManagement::Set_Debug()
{
	CRenderer::GetInstance()->Set_Debug();
}

void CManagement::Set_RendererOption(T_RenderOption tRenderOption)
{
	CRenderer::GetInstance()->Set_RenderOption(tRenderOption);
}

void CManagement::Reset_RendererOption()
{
	CRenderer::GetInstance()->Reset_RenderOption();	
}

T_RenderOption CManagement::Get_RendererOption()
{
	return CRenderer::GetInstance()->Get_RenderOption();
}

///////////////////////////////////////////////////////////////////////							LightMgr
HRESULT CManagement::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void CManagement::Render_Light(LPD3DXEFFECT & pEffect)
{
	return CLightMgr::GetInstance()->Render_Light(pEffect);
}

D3DLIGHT9 * CManagement::Get_Light(const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

_bool CManagement::IsLightExist()
{
	return CLightMgr::GetInstance()->IsLightExist();
}

void CManagement::Set_LightTarget(_int iIndex)
{
	CLightMgr::GetInstance()->Set_LightTarget(iIndex);
}

_vec3 CManagement::Get_LightPos()
{
	return CLightMgr::GetInstance()->Get_LightPos();
}

void CManagement::Set_LightPos(_vec3 vPos)
{
	CLightMgr::GetInstance()->Set_LightPos(vPos);
}

_vec3 CManagement::Get_LightDir()
{
	return CLightMgr::GetInstance()->Get_LightDir();
}

void CManagement::Set_LightDir(_vec3 vDir)
{
	CLightMgr::GetInstance()->Set_LightDir(vDir);
}

D3DLIGHTTYPE CManagement::Get_LightType()
{
	return CLightMgr::GetInstance()->Get_LightType();
}

void CManagement::Set_LightType(D3DLIGHTTYPE eLightType)
{
	CLightMgr::GetInstance()->Set_LightType(eLightType);
}

D3DXCOLOR CManagement::Get_LightDiffuse()
{
	return CLightMgr::GetInstance()->Get_LightDiffuse();
}

void CManagement::Set_LightDiffuse(D3DXCOLOR Diffuse)
{
	CLightMgr::GetInstance()->Set_LightDiffuse(Diffuse);
}

D3DXCOLOR CManagement::Get_LightSpecular()
{
	return CLightMgr::GetInstance()->Get_LightSpecular();
}

void CManagement::Set_LightSpecular(D3DXCOLOR Specular)
{
	CLightMgr::GetInstance()->Set_LightSpecular(Specular);
}

D3DXCOLOR CManagement::Get_LightAmbient()
{
	return CLightMgr::GetInstance()->Get_LightAmbient();
}

void CManagement::Set_LightAmbient(D3DXCOLOR Ambient)
{
	CLightMgr::GetInstance()->Set_LightAmbient(Ambient);
}

D3DXCOLOR CManagement::Get_LightBackBufferColor()
{
	return CLightMgr::GetInstance()->Get_LightBackBufferColor();
}

void CManagement::Set_LightBackBufferColor(D3DXCOLOR BackBufferColor)
{
	CLightMgr::GetInstance()->Set_LightBackBufferColor(BackBufferColor);
}

_float CManagement::Get_FogStart()
{	
	return CFogManager::GetInstance()->Get_FogStart();
}

void CManagement::Set_FogStart(_float fFogStart)
{
	CFogManager::GetInstance()->Set_FogStart(fFogStart);
}

_float CManagement::Get_FogEnd()
{
	return CFogManager::GetInstance()->Get_FogEnd();
}

void CManagement::Set_FogEnd(_float fFogEnd)
{
	CFogManager::GetInstance()->Set_FogEnd(fFogEnd);
}

_vec4 CManagement::Get_FogColor()
{
	return CFogManager::GetInstance()->Get_FogColor();
}

void CManagement::Set_FogColor(_vec4 FogColor)
{
	CFogManager::GetInstance()->Set_FogColor(FogColor);
}

void CManagement::Reset_Fog()
{
	CFogManager::GetInstance()->Reset_Fog();
}

//////////////////////////////////////////////////////////////////////////						RenderTarget
HRESULT CManagement::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& pTargetTag, const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, pTargetTag, iWidth, iHeight, Format, Color);
}

HRESULT CManagement::Ready_MRT(const wstring& pMRTTag, const wstring& pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}

HRESULT CManagement::Begin_MRT(const wstring& pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}

HRESULT CManagement::End_MRT(const wstring& pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}

HRESULT CManagement::Ready_DebugBuffer(const wstring& pTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}

void CManagement::Render_DebugBuffer(const wstring& pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}

void CManagement::Set_RenderTargetTexture(LPD3DXEFFECT & pEffect, const wstring& pTargetTag, const char * pConstantTable)
{
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, pTargetTag, pConstantTable);
}

_bool CManagement::Collision_CircleToCircle(CSphereCollider * pDestSphereCol, E_LAYERTAG eLayerTag)
{
	return CCollisionManager::GetInstance()->Collision_CircleToCircle(pDestSphereCol, eLayerTag);
}

_bool CManagement::Collision_FanToCircle(CGameObject * pDestObject, _vec3 vStartPos, _vec3 vDir, _float fFanDegree, _float fFanDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	return CCollisionManager::GetInstance()->Collision_FanToCircle(pDestObject, vStartPos, vDir, fFanDegree, fFanDistance, eLayerTag,Col);
}

_bool CManagement::Collision_StraightToCircle(CGameObject * pDestObject, _vec3 vStartPos, _vec3 vDir, _float fWidth, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	return CCollisionManager::GetInstance()->Collision_StraightToCircle(pDestObject, vStartPos, vDir, fWidth, fDistance, eLayerTag,Col);
}

_bool CManagement::Collision_SpotToCircle(CGameObject * pDestObject, _vec3 vStartPos, _float fRadius, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	return CCollisionManager::GetInstance()->Collision_SpotToCircle(pDestObject, vStartPos, fRadius, eLayerTag,Col);
}

void CManagement::Start_DialogEvent(const wstring & wstrEventName)
{
	CDialog_Manager::GetInstance()->Start_DialogEvent(wstrEventName); 
}

void CManagement::Render_DialogFont()
{
	CDialog_Manager::GetInstance()->Render_DialogFont();
}

CSkillObject* CManagement::SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale)
{
	return CSkill_Manager::GetInstance()->SetSkill(pSkillTag, eLayerTag, vPos, vRot, vScale);
}

void CManagement::Start_ActionCamEvent(const wstring & wstrFileName, const wstring & wstrAction, _vec3 vPos, _float fYAngle, CActionCam_Mgr::E_ActionCam_Mode eMode)
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(wstrFileName, wstrAction, vPos, fYAngle, eMode);
}

_uint CManagement::GetValue()
{
	return CWall512::GetInstance()->GetValue();
}

_uint CManagement::GetValue(unsigned int nMinValue, unsigned int nMaxValue)
{
	return CWall512::GetInstance()->GetValue(nMinValue, nMaxValue);
}

_uint CManagement::GetValue(unsigned int nMaxValue)
{
	return CWall512::GetInstance()->GetValue(nMaxValue);
}

_bool CManagement::GetRandBoolean()
{
	return CWall512::GetInstance()->GetRandBoolean();
}

_float CManagement::GetFloatValue()
{
	return CWall512::GetInstance()->GetFloatValue();
}

_float CManagement::GetFloatValue(float fMinValue, float fMaxValue)
{
	return CWall512::GetInstance()->GetFloatValue(fMinValue, fMaxValue);
}

_float CManagement::GetFloatValue(float fMaxValue)
{
	return CWall512::GetInstance()->GetFloatValue(fMaxValue);
}