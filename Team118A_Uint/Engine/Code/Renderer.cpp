#include "Renderer.h"
#include "Camera.h"
#include "ProtoMgr.h"
#include "RenderTargetMgr.h"
#include "RectBuffer.h"
#include "SMesh.h"
#include "DMesh.h"
#include "UI_Manager.h"
#include "UI_Object.h"
#include "LightMgr.h"
#include "Dialog_Manager.h"
#include "FontMgr.h"

#include "FogManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CRenderer)

_bool Compare_Z(CGameObject * pDest, CGameObject * pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

_bool Compare_UiByLayer(CGameObject* pDest, CGameObject* pSrc)
{
	// Sorting이 높을수록 먼저 그린다.
	if (pDest->Get_SortingLayer() < pSrc->Get_SortingLayer())
	{
		return TRUE;
	}
	else if (pDest->Get_SortingLayer() > pSrc->Get_SortingLayer())
	{
		return FALSE;
	}

	// Sorting이 같을 경우 Order_In_Layer를 통해 정렬
	else if (pDest->Get_SortingLayer() == pSrc->Get_SortingLayer())
	{
		// Order_In_Layer가 높을수록 먼저 그린다.
		if (pDest->Get_OrderInLayer() < pSrc->Get_OrderInLayer())
		{
			return TRUE;
		}
		else if (pDest->Get_OrderInLayer() >= pSrc->Get_OrderInLayer())
		{
			return FALSE;
		}
	}

	return FALSE;
}

Engine::CRenderer::CRenderer()
{
}

Engine::CRenderer::~CRenderer()
{
	Free();
}

void Engine::CRenderer::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pTexMaskDefault);

	
	Safe_Release(m_pBaseShader);
	Safe_Release(m_pBlendShader);
	Safe_Release(m_pLightAccShader);
	Safe_Release(m_pEffectShader);
	Safe_Release(m_pGodRayShader);	
	Safe_Release(m_pBlurShader);
	Safe_Release(m_pPostProcessShader); 
	Safe_Release(m_pViewCutShader);
	Safe_Release(m_pShowShadeShader);
	Safe_Release(m_pShowBlendShader);
	Safe_Release(m_pShowBaseShader);
	
	Clear_RenderGroup();
	Clear_Instance_RenderGroup();
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	if (FAILED(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4((_float)ViewPort.Width, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4((_float)ViewPort.Width, (_float)ViewPort.Height, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(0.f, (_float)ViewPort.Height, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	Set_RendererShader();

	// 마스크 텍스처가 없는 객체들을 위한 기본 텍스처
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/Default_Mask.png", &m_pTexMaskDefault);
	
	return S_OK;
}

void Engine::CRenderer::Add_RenderGroup(E_RENDER_ID eRenderID, CGameObject* pGameObject)
{
	if (E_RENDER_ID::RENDER_END <= eRenderID)
		return;

	m_RenderGroup[(_int)eRenderID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Add_InstanceGroup(E_INSTANCE_RENDER_ID eInstanceRenderID, CGameObject * pGameObject)
{
	if (E_INSTANCE_RENDER_ID::RENDER_INSTANCE_RENDER_END <= eInstanceRenderID)
		return;

	E_OBJTAG eObjTag = pGameObject->Get_ObjTag();
	auto iter = find_if(m_mapInstanceGroup[_int(eInstanceRenderID)].begin(), m_mapInstanceGroup[_int(eInstanceRenderID)].end(), CEnumTag_Finder(_int(eObjTag)));

	if (iter == m_mapInstanceGroup[_int(eInstanceRenderID)].end())
	{
		vector<CGameObject*> vecObj;
		vecObj.emplace_back(pGameObject);
		pGameObject->AddRef();
		m_mapInstanceGroup[_int(eInstanceRenderID)].emplace(eObjTag, vecObj);
	}
	else
	{
		iter->second.emplace_back(pGameObject);
		pGameObject->AddRef();
	}
}



void Engine::CRenderer::Render_Update(const _float & fTimeDelta)
{
	// 키 입력을 통하여 랜더러의 옵션을 설정한다.
	RenderOption(fTimeDelta);

	Update_ViewCut(fTimeDelta);



	if (0.f < m_fRenderDebug) 
	{
		m_fRenderDebug -= fTimeDelta;
		m_bRenderDebug = TRUE;
	}

	if (m_bRenderDebug && 0.f >= m_fRenderDebug)
	{
		m_fRenderDebug = 0.f;
		m_bRenderDebug = FALSE;

		m_fTime_ViewCutFix = 0.f;
		m_tViewCutFix = T_ViewCut();
	}
}



void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{	
	if (!m_pGraphicDev)
	{
		m_pGraphicDev = CGraphicDev::GetInstance()->GetDevice();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 메인 디퍼드

	// ViewCut 셋팅
	LPD3DXEFFECT pEffect = m_pPostProcessShader->Get_EffectHandle();
	pEffect->SetFloat("g_fCutLine_Dir", 1.f);

	Set_Base_Constantable();
		
	// 일반 디퍼드 //////////////////////////

	// 디퍼드 칼라
	Render_Deferred_Color(pGraphicDev);

	// 쉐도우
	Render_Shadow(pGraphicDev);

	// 빛 누적
	Render_LightAcc(pGraphicDev);

	// 이미지 합성
	Render_Blend(pGraphicDev);

	// 라이트 필터 블러 -> 글로우
	Render_Blur(pGraphicDev);

	// 광채 표현 + 갓 레이 렌더
	Render_Light(pGraphicDev);
	
	// 최종 합성
	Render_PostProcess(pGraphicDev);

	// 이펙트 디퍼드 //////////////////////////

	// 이펙트 Obj + 알파 Obj
	Render_EffectObj(pGraphicDev);

	// 그 위에 이펙트를 그린다.
	Render_Effect(pGraphicDev);
				
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ViewCut 디퍼드

	if (m_bViewCut) 
	{
		Reverse_Debug();

		_matrix	matView_Origin, matProj_Origin;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView_Origin);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj_Origin);
		if (m_bViewCut_Fix)
		{
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_tViewCutFix.matView_ViewCut);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_tViewCutFix.matProg_ViewCut);
		}
		else
		{
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_tViewCut.matView_ViewCut);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_tViewCut.matProg_ViewCut);
		}

		// 모든 랜더타겟의 Z값을 초기화합니다
		CRenderTargetMgr::GetInstance()->Clear_ZWrite();

		// ViewCut 셋팅
		LPD3DXEFFECT pEffect = m_pPostProcessShader->Get_EffectHandle();
		pEffect->SetFloat("g_fCutLine_Dir", -1.f);

		Set_Base_Constantable();

		// 일반 디퍼드 //////////////////////////

		// 디퍼드 칼라
		Render_Deferred_Color(pGraphicDev);

		// 쉐도우
		Render_Shadow(pGraphicDev);

		// 빛 누적
		Render_LightAcc(pGraphicDev);

		// 이미지 합성
		Render_Blend(pGraphicDev);

		// 라이트 필터 블러 -> 글로우
		Render_Blur(pGraphicDev);

		// 광채 표현 + 갓 레이 렌더
		Render_Light(pGraphicDev);

		// 최종 합성
		Render_PostProcess(pGraphicDev);

		// 이펙트 디퍼드 //////////////////////////

		// 이펙트 Obj + 알파 Obj
		Render_EffectObj(pGraphicDev);

		// 그 위에 이펙트를 그린다.
		Render_Effect(pGraphicDev);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView_Origin);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj_Origin);

		Reverse_Debug();
	}
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Render_UI(pGraphicDev);

	Render_ShowWindow(pGraphicDev);
	
	CDialog_Manager::GetInstance()->Render_DialogFont();
			
	if (m_bRenderDeferred)
	{
		// 랜더 타겟들을 화면에 디버그한다
		Render_Debug();
	}

	Clear_RenderGroup();
	Clear_Instance_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup()
{
	for (_ulong i = 0; i < (_ulong)E_RENDER_ID::RENDER_END; ++i)
	{
		if (FALSE == m_RenderGroup[i].empty())
		{
			for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
			m_RenderGroup[i].clear();
		}
	}
}

void CRenderer::Clear_Instance_RenderGroup()
{
	for (_int i = 0; i < _int(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_RENDER_END); ++i)
	{
		for (auto& tPair : m_mapInstanceGroup[i])
		{
			for (auto& pObj : tPair.second)
			{
				Safe_Release(pObj);
			}
			tPair.second.clear();
		}
		m_mapInstanceGroup[i].clear();
	}
}

void CRenderer::Set_DefualtMaskTexture(LPD3DXEFFECT pEffect, D3DXHANDLE hParameter)
{
	pEffect->SetTexture(hParameter, m_pTexMaskDefault);
}

void CRenderer::Set_RendererShader()
{
	// 랜더러에서 사용할 쉐이더들을 셋팅합니다

	m_pBaseShader			= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Base"));
	m_pBlendShader			= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Blend"));
	m_pLightAccShader		= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Shade"));
	m_pEffectShader			= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Effect_Blend"));

	m_pBlurShader			= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Blur"));
	m_pGodRayShader			= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_GodRay"));
	m_pPostProcessShader	= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_PostProcess"));
	m_pViewCutShader		= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_ViewCut"));

	m_pShowShadeShader		= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Show_Shade"));
	m_pShowBlendShader		= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Show_Blend"));
	m_pShowBaseShader		= static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Show_Base"));
}

void CRenderer::Check_IfActiveTrue_Render(E_RENDER_ID eRenderID, _int iPass)
{
	for (auto& iter : m_RenderGroup[(_int)eRenderID])
	{
		if(iter->Get_Active())
			iter->Render_Object(iPass);
	}
}

void CRenderer::Check_IfActiveTrue_Instance_Render(E_INSTANCE_RENDER_ID eInstanceRenderID)
{

	LPDIRECT3DVERTEXDECLARATION9	pDecl = nullptr;

	for (auto& tPair : m_mapInstanceGroup[_int(eInstanceRenderID)])
	{
		if (tPair.second.empty())
			continue;

		_int iInstanceCount = 0;
		for (auto& pObj : tPair.second)
		{
			if (pObj->Get_Active())
			{
				++iInstanceCount;
			}
		}
		if (iInstanceCount == 0)
		{
			continue;
		}

		Safe_Release(m_pInstanceBuffer);

		CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(iInstanceCount * sizeof(T_INSTANCE)),
			0, 0, D3DPOOL_MANAGED, &m_pInstanceBuffer, 0);

		T_INSTANCE* pInstanceVertex = nullptr;
		m_pInstanceBuffer->Lock(0, 0, (void**)&pInstanceVertex, 0);

		_int iIndex = 0;
		for (auto& pObj : tPair.second)
		{
			if (pObj->Get_Active())
			{
				pInstanceVertex[iIndex].vColor = { 1.f,1.f,1.f,1.f };
				pInstanceVertex[iIndex].matWorld = static_cast<CTransform*>(pObj->Get_Component(ComTag_Transform))->Get_WorldMatrix();
				++iIndex;
			}
		}

		m_pInstanceBuffer->Unlock();

		CShader* pShaderCom = static_cast<CShader*>(tPair.second[0]->Get_Component(ComTag_Shader));
		pShaderCom->BeginShader();
		//_int i = 0;
		pShaderCom->BeginPass(ShaderPass(E_ShaderPass::Stage_Instance, 0));
		//pShaderCom->BeginPass(0);

		//======= 그리기 ======
		CRectBuffer* pRectBuffer = nullptr;
		CSMesh* pSMesh = nullptr;
		CDMesh* pDMesh = nullptr;

		pRectBuffer = static_cast<CRectBuffer*>(tPair.second[0]->Get_Component(ComTag_RectBuffer));
		if (!pRectBuffer)
		{
			pSMesh = static_cast<CSMesh*>(tPair.second[0]->Get_Component(ComTag_SMesh));
			if (!pSMesh)
			{
				pDMesh = static_cast<CDMesh*>(tPair.second[0]->Get_Component(ComTag_DMesh));
				if (pDMesh)
				{
					//아직 지원 안함 
					return;
				}
				else
				{
					return;
				}
			}
		}

		LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = nullptr;

		if (pRectBuffer)
		{
			CGraphicDev::GetInstance()->GetDevice()->CreateVertexDeclaration(declRect, &pDecl);
			CGraphicDev::GetInstance()->GetDevice()->SetVertexDeclaration(pDecl);
			CGraphicDev::GetInstance()->GetDevice()->SetStreamSource(0, pRectBuffer->Get_VB(), 0, sizeof(VTXUI));
		}
		else if (pSMesh)
		{
			CGraphicDev::GetInstance()->GetDevice()->CreateVertexDeclaration(declMesh, &pDecl);
			CGraphicDev::GetInstance()->GetDevice()->SetVertexDeclaration(pDecl);
			pSMesh->Get_Mesh()->GetVertexBuffer(&pVertexBuffer);
			if (pVertexBuffer)
			{
				CGraphicDev::GetInstance()->GetDevice()->SetStreamSource(0, pVertexBuffer, 0, sizeof(VTXTEST));
			}
		}
		
		CGraphicDev::GetInstance()->GetDevice()->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | iInstanceCount));

		CGraphicDev::GetInstance()->GetDevice()->SetStreamSource(1, m_pInstanceBuffer, 0, sizeof(T_INSTANCE));
		CGraphicDev::GetInstance()->GetDevice()->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

		LPD3DXEFFECT pEffect = pShaderCom->Get_EffectHandle();

		tPair.second[0]->Set_Constantable();
		pEffect->CommitChanges();

		LPDIRECT3DINDEXBUFFER9 pIndexBuffer = nullptr;
		//그리기 코드
		if (pRectBuffer)
		{
			CGraphicDev::GetInstance()->GetDevice()->SetIndices(pRectBuffer->Get_IB());
			CGraphicDev::GetInstance()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pRectBuffer->Get_VtxCnt(), 0, pRectBuffer->Get_TriCnt()); // 인덱스 버퍼도 활용하여 출력하는 함수
		}
		else if (pSMesh)
		{
			pSMesh->Get_Mesh()->GetIndexBuffer(&pIndexBuffer);
			CGraphicDev::GetInstance()->GetDevice()->SetIndices(pIndexBuffer);

			pSMesh->Render_Meshes_Instance(pShaderCom);
		}

		CGraphicDev::GetInstance()->GetDevice()->SetStreamSourceFreq(0, 1);
		CGraphicDev::GetInstance()->GetDevice()->SetStreamSourceFreq(1, 1);


		pShaderCom->EndPass();
		pShaderCom->EndShader();


		Safe_Release(pVertexBuffer);
		Safe_Release(pIndexBuffer);
		Safe_Release(m_pInstanceBuffer);
		Safe_Release(pDecl);
	}
}

void CRenderer::RenderOption(const _float & fTimeDelta)
{

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_Shift) && CKeyManager::GetInstance()->Key_Pressing(KEY_Alt))
	{
		// 콜라이더 디버그를 할지 안할지
		if (CKeyManager::GetInstance()->Key_Down(KEY_Q))
		{
			Set_Debug();
		}

		// SSAO를 할지 안할지
		if (CKeyManager::GetInstance()->Key_Down(KEY_P))
		{
			Set_SSAO(!m_bSSAO_OnOff);
		}

		// HDR
		if (CKeyManager::GetInstance()->Key_Down(KEY_E))
		{
			m_tRenderOption.bHDR = !m_tRenderOption.bHDR;
		}

		// Bloom
		if (CKeyManager::GetInstance()->Key_Down(KEY_R))
		{
			m_tRenderOption.bBloom = !m_tRenderOption.bBloom;
		}

		// SSAA안티엘리어싱
		if (CKeyManager::GetInstance()->Key_Down(KEY_O))
		{
			Set_Antialiasing(!Get_Antialiasing());
		}

		// BaseGlow
		if (CKeyManager::GetInstance()->Key_Down(KEY_U))
		{
			m_bBaseGlow = !m_bBaseGlow;
		}

		// TextGlow
		if (CKeyManager::GetInstance()->Key_Down(KEY_I))
		{
			m_bTexGlow = !m_bTexGlow;
		}

		// GodRay
		if (CKeyManager::GetInstance()->Key_Down(KEY_G))
		{
			m_bGodRay = !m_bGodRay;
		}

		if (CKeyManager::GetInstance()->Key_Down(KEY_T))
		{
			m_bTest_For_PostProcess = !m_bTest_For_PostProcess;
			m_fTest_For_PrstPrecess_Ratio_X = m_bTest_For_PostProcess ? 0.f : 0.5f;
		}

		// 랜더타겟 디버그의 알파값
		if (CKeyManager::GetInstance()->Key_Down(KEY_Z))
		{
			m_bRenderDeferredAlpha = !m_bRenderDeferredAlpha;
		}

		// 뷰 컷 셋팅
		if (CKeyManager::GetInstance()->Key_Down(KEY_X))
		{
			T_ViewCut tViewCut = T_ViewCut();

			tViewCut.fTimeMax = 2.f;
			tViewCut.fTime = tViewCut.fTimeMax;

			tViewCut.fDelay = 0.f;
			tViewCut.fDis = 0.4f;

			_matrix	matView, matProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

			tViewCut.matView_ViewCut = matView;
			tViewCut.matProg_ViewCut = matProj;
			
			m_tViewCut = tViewCut;
		}

		// 캐릭터 색 보정
		if (CKeyManager::GetInstance()->Key_Down(KEY_C))
		{
			if (0.f == m_fHDR_FixColor)
			{
				m_fHDR_FixColor = 0.05f;
			}
			else if (0.05f == m_fHDR_FixColor)
			{
				m_fHDR_FixColor = 0.1f;
			}
			else
			{
				m_fHDR_FixColor = 0.f;
			}
		}

		// 고정 뷰 컷 셋팅
		if (CKeyManager::GetInstance()->Key_Down(KEY_V))
		{
			T_ViewCut tViewCut = T_ViewCut();

			tViewCut.fTimeMax = 2.f;
			tViewCut.fTime = 1.f;

			tViewCut.fDelay = 0.f;

			tViewCut.fDis = CWall512::GetInstance()->GetFloatValue(-2.f, 2.f);
			//tViewCut.fDis = 0.8f;

			_matrix	matView, matProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
			matView._41 += 0;
			_float fDir = (0.f < CWall512::GetInstance()->GetFloatValue(-1.f, 1.f)) ? 1.f : -1.f;
			matView._42 += fDir;
			matView._43 += 0;

			tViewCut.matView_ViewCut = matView;
			tViewCut.matProg_ViewCut = matProj;

			m_fTime_ViewCutFix = 2.f;
			m_tViewCutFix = tViewCut;
		}

		// 랜더 디버그
		if (CKeyManager::GetInstance()->Key_Down(KEY_B))
		{
			T_ViewCut tViewCut = T_ViewCut();

			tViewCut.fTimeMax = 5.f;
			tViewCut.fTime = 2.5f;
			
			tViewCut.fDis = 0.2f;

			_matrix	matView, matProj;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
			tViewCut.matView_ViewCut = matView;
			tViewCut.matProg_ViewCut = matProj;

			m_fTime_ViewCutFix = tViewCut.fTimeMax;
			m_tViewCutFix = tViewCut;

			m_fRenderDebug = 5.f;
		}
		//m_fRenderDebug
	}

	m_fTest_For_PrstPrecess_Ratio_X += m_bTest_For_PostProcess ? +(fTimeDelta * 0.25f) : -(fTimeDelta * 0.25f);
	m_fTest_For_PrstPrecess_Ratio_X = CUtility::Clamp_float(m_fTest_For_PrstPrecess_Ratio_X, 0.f, 0.5f);


}

void Engine::CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_PRIORITY);
}

void CRenderer::Render_Deferred_Color(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	m_iDefferdPassCount = 0;

	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Deferred_Color");

	Render_NonAlpha(pGraphicDev, m_iDefferdPassCount);
	Render_Instance_NonAlpha(pGraphicDev);

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Deferred_Color");
}

void CRenderer::Render_Shadow(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	m_iDefferdPassCount++;

	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Shadow");

	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_SHADOW, m_iDefferdPassCount);

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Shadow");
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_LightAcc");

	LPD3DXEFFECT pEffect = m_pLightAccShader->Get_EffectHandle();
	pEffect->AddRef();
	pEffect->SetBool("g_bSSAO_OnOff", m_bSSAO_OnOff);

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Normal", "g_NormalTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Depth", "g_DepthTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Shadow", "g_ShadowTexture");

	pEffect->Begin(NULL, 0);

	CLightMgr::GetInstance()->Render_Light(pEffect);

	pEffect->End();

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_LightAcc");

	Safe_Release(pEffect);
}

void CRenderer::Render_Blend(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Blend_End");

	Render_Priority(pGraphicDev);

	LPD3DXEFFECT pEffect = m_pBlendShader->Get_EffectHandle();
	pEffect->AddRef();

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Albedo", "g_AlbedoTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Emission", "g_EmissionTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Shade", "g_ShadeTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Specular", "g_SpecularTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Depth", "g_DepthTexture");


	pEffect->SetBool("g_bHDR", m_tRenderOption.bHDR);
	pEffect->SetFloat("g_fLuminance", m_tRenderOption.fLuminance);
	pEffect->SetFloat("g_fMiddleGray", m_tRenderOption.fMiddleGray);
	pEffect->SetFloat("g_fWhiteCutoff", m_tRenderOption.fWhiteCutoff);

	pEffect->SetBool("g_bBloom", m_tRenderOption.bBloom);
	pEffect->SetFloat("g_fBrightFilter", m_tRenderOption.fBrightFilter);

	pEffect->SetBool("g_bAntialiasing", m_bAntialiasing ? 1 : 0);
	pEffect->SetBool("g_bTest", m_fTest_For_PrstPrecess_Ratio_X > 0.f);
	pEffect->SetFloat("g_fTestRatio", m_fTest_For_PrstPrecess_Ratio_X);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
		
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Blend_End");
	
}

void Engine::CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev, _int iPass)
{
	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_NONALPHA, iPass);
}

void CRenderer::Render_Instance_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Check_IfActiveTrue_Instance_Render(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA);
}

void CRenderer::Render_Effect(LPDIRECT3DDEVICE9 & pGraphicDev)
{

	LPD3DXEFFECT pEffect = m_pEffectShader->Get_EffectHandle();
	pEffect->AddRef();

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Effect", "g_EffectTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Effect_Blur", "g_EffectBlurTexture");
		

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);


	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

}

void CRenderer::Render_EffectObj(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Effect");
	

	// 이펙트 랜더
	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_EFFECT);
	
	// 이펙트 랜더링 옵션을 기본값으로 초기화 합니다.
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	// 알파 랜더
	Render_Instance_Alpha(pGraphicDev);
	Render_Alpha(pGraphicDev);



	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Effect");
}

void CRenderer::Render_Blur(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	LPD3DXEFFECT pEffect = m_pBlurShader->Get_EffectHandle();
	pEffect->AddRef();
	
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Blur");

	pEffect->Begin(NULL, 0);

	////////////////////////////////////////////////////////////

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Bright_Image", "g_SampleTexture");

	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
			
	////////////////////////////////////////////////////////////

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Blur", "g_SampleTexture");
	
	pEffect->BeginPass(1);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();

	////////////////////////////////////////////////////////////

	pEffect->End();

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Blur");

	Safe_Release(pEffect);
}

void CRenderer::Render_Light(LPDIRECT3DDEVICE9 & pGraphicDev)
{	
	// light manager ender
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_Ray");

	CLightMgr::GetInstance()->Render_Light_Mgr();

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_Ray");
	
	if (!m_bGodRay) 
	{
		return;
	}

	// god ray render
		
	T_GodLight* pGodLight = CLightMgr::GetInstance()->Get_GodLight_Info();

	if (pGodLight) 
	{
		CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_GodRay");

		LPD3DXEFFECT pEffect = m_pGodRayShader->Get_EffectHandle();
		pEffect->AddRef();

		CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Ray_Image", "g_RayTexture");
		CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Depth", "g_DepthTexture");

		_vec3 vLightPos = pGodLight->vPosition;
		pEffect->SetVector("g_vLightWorldPos", &_vec4(vLightPos.x, vLightPos.y, vLightPos.z, 1.f));

		_matrix	matVP, matView, matProj;

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		matVP = matView * matProj;
		pEffect->SetMatrix("g_matVP", &matVP);
		pEffect->SetBool("g_bSun", pGodLight->bSun);

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);

		pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
		pGraphicDev->SetFVF(FVF_SCREEN);
		pGraphicDev->SetIndices(m_pIB);
		pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);

		CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_GodRay");
	}	
}

void CRenderer::Render_PostProcess(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	LPD3DXEFFECT pEffect = m_pPostProcessShader->Get_EffectHandle();
	pEffect->AddRef();

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Blend_Image", "g_BlendTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Blur", "g_LightTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_GodRay_Image", "g_GodRayTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_Color_Image", "g_ColorTexture");

	// HDR 후 색보정용
	pEffect->SetFloat("g_fFixColor", m_fHDR_FixColor);
	
	pEffect->SetBool("g_bAntialiasing", m_bAntialiasing ? 1 : 0);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void Engine::CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_RenderGroup[(_int)E_RENDER_ID::RENDER_ALPHA].sort(Compare_Z);

	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_ALPHA);
}

void CRenderer::Render_Instance_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Check_IfActiveTrue_Instance_Render(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_ALPHA);
}

void Engine::CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_RenderGroup[(_int)E_RENDER_ID::RENDER_UI].sort(Compare_UiByLayer);

	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_UI);

	for (auto& iter : m_RenderGroup[(_int)E_RENDER_ID::RENDER_UI])
	{
		CUI_Object* pUi = static_cast<CUI_Object*>(iter);
		if (pUi->Get_Active())
			pUi->Is_OnMouse(CUtility::Get_MousePoint(*pUi->Get_Hwnd()));
	}

	static _bool bDown = FALSE;
	static _int iMostDeep = 0;
	if (bDown)
	{
		pDeepTouchUi_Up = nullptr;
	}

	if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON) && bDown == FALSE)
	{
		for (auto& iter : m_RenderGroup[(_int)E_RENDER_ID::RENDER_UI])
		{
			iMostDeep = 0;
			CUI_Object* pUi = static_cast<CUI_Object*>(iter);
			if (pUi->Get_Active())
				pUi->Is_OnMouse(CUtility::Get_MousePoint(*pUi->Get_Hwnd()), &pDeepTouchUi_Down, &iMostDeep);

			if (pDeepTouchUi_Down)
			{
				bDown = TRUE;
			}
		}
	}
	else if (bDown && CKeyManager::GetInstance()->Key_Up(KEY_LBUTTON))
	{
		for (auto& iter : m_RenderGroup[(_int)E_RENDER_ID::RENDER_UI])
		{
			iMostDeep = 0;
			CUI_Object* pUi = static_cast<CUI_Object*>(iter);
			if (pUi->Get_Active())
				pUi->Is_OnMouse(CUtility::Get_MousePoint(*pUi->Get_Hwnd()), &pDeepTouchUi_Up, &iMostDeep);

			if (pDeepTouchUi_Up)
			{
				if (pDeepTouchUi_Down == pDeepTouchUi_Up)
				{
					CUI_Manager::GetInstance()->Set_LastPickUi(pDeepTouchUi_Down);
					pDeepTouchUi_Down->OnClick();
					break;
				}
			}
		}
		pDeepTouchUi_Down = nullptr;
		pDeepTouchUi_Up = nullptr;
		bDown = FALSE;
		iMostDeep = 0;
	}

	if (m_fTest_For_PrstPrecess_Ratio_X > 0.f)
	{
		CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, L"PostProcess On", _vec2(-WINCX * 0.5f, -WINCY * 0.5f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 15, 15, FW_NORMAL, FALSE));
		CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, L"PostProcess Off", _vec2(0.f, -WINCY * 0.5f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), 15, 15, FW_NORMAL, FALSE));
	}
}

void CRenderer::Render_Billboard(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	m_RenderGroup[(_int)E_RENDER_ID::RENDER_BILLBOARD].sort(Compare_Z);

	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_BILLBOARD);
}

void CRenderer::Render_Instance_Billboard(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Check_IfActiveTrue_Instance_Render(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_BILLBOARD);
}

void CRenderer::Render_ShowWindow(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_ShowWindow");

	if (!m_RenderGroup[(_int)E_RENDER_ID::RENDER_SHOWWINDOW].size())
	{
		CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_ShowWindow");
		CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_ShowWindow_Blur");
		CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_ShowWindow_Blur");
		return;
	}
	
	Check_IfActiveTrue_Render(E_RENDER_ID::RENDER_SHOWWINDOW);


	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_ShowWindow");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 초기화

	LPD3DXEFFECT pEffect = nullptr;

	// 이펙트 랜더링 옵션을 기본값으로 초기화 합니다.
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 음영

	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_ShowWindow_Shade");

	pEffect = m_pShowShadeShader->Get_EffectHandle();

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Normal", "g_NormalTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Depth", "g_DepthTexture");

	pEffect->Begin(NULL, 0);

	CLightMgr::GetInstance()->Render_Light(pEffect);

	pEffect->End();

	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_ShowWindow_Shade");


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 블러
	CRenderTargetMgr::GetInstance()->Begin_MRT(L"MRT_ShowWindow_Blur");
	
	pEffect = m_pBlurShader->Get_EffectHandle();
	
	pEffect->Begin(NULL, 0);
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Emission", "g_SampleTexture");

	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();

	////////////////////////////////////////////////////////////

	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Blur", "g_SampleTexture");

	pEffect->BeginPass(1);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();
	
	CRenderTargetMgr::GetInstance()->End_MRT(L"MRT_ShowWindow_Blur");



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 블랜드
	   
	pEffect = m_pShowBlendShader->Get_EffectHandle();
	
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow", "g_AlbedoTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Emission", "g_EmissionTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Shade", "g_ShadeTexture");
	CRenderTargetMgr::GetInstance()->Set_RenderTargetTexture(pEffect, L"Target_ShowWindow_Spec", "g_SpecularTexture");
	
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	
	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	
	pEffect->EndPass();
	pEffect->End();

}

void CRenderer::Render_Debug()
{
	if (m_bRenderDeferredAlpha)
	{
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Deferred_Color");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_LightAcc");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Shadow");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Effect");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Blur");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Ray");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_GodRay");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_Blend_End");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_CutView_Main");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_CutView_CutIn");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_ShowWindow");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_ShowWindow_Shade");
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(L"MRT_ShowWindow_Blur");

	CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Set_Base_Constantable()
{
	// 기본 상수 테이블들을 베이스 텍스처로 셋팅하겠습니다. (객체들이 보편적으로 사용하므로)

	LPD3DXEFFECT pEffect = m_pBaseShader->Get_EffectHandle();
	
	_matrix	matView, matProj;	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	
	// 그림자용
	pEffect->SetMatrix("g_matLightView", &CLightMgr::GetInstance()->Get_LightView());
	pEffect->SetMatrix("g_matLightProj", &CLightMgr::GetInstance()->Get_LightProg());

	// 사망, 선택시 사용되는 필터의 디폴트값
	pEffect->SetFloat("g_fFilter", 0.f);

	Set_Fog();
}

void CRenderer::Set_Fog()
{
	// 커스텀 포그
	LPD3DXEFFECT pEffect = m_pBaseShader->Get_EffectHandle();

	// 포그
	pEffect->SetVector("g_vFogColor", &CFogManager::GetInstance()->Get_FogColor());
	pEffect->SetFloat("g_fFogStart", CFogManager::GetInstance()->Get_FogStart());
	pEffect->SetFloat("g_fFogEnd", CFogManager::GetInstance()->Get_FogEnd());
		
	T_CustomFog tCustomFog = CFogManager::GetInstance()->GetCustomFog();

	pEffect->SetVector("g_CF_vPos", &(_vec4)(tCustomFog.vPos));

	pEffect->SetFloat("g_CF_Under", tCustomFog.fView_Under);
	pEffect->SetFloat("g_CF_Over", tCustomFog.fView_Over);

	pEffect->SetFloat("g_CF_Start", tCustomFog.fDistance_Start);
	pEffect->SetVector("g_CF_vColor_Start", &(_vec4)(tCustomFog.vColor_Start));

	pEffect->SetFloat("g_CF_End", tCustomFog.fDistance_End);
	pEffect->SetVector("g_CF_vColor_End", &(_vec4)(tCustomFog.vColor_End));
}

void CRenderer::Reset_Fog()
{
	CFogManager::GetInstance()->Set_FogColor(_vec4(0.f, 0.f, 0.f, 1.f));
	CFogManager::GetInstance()->Set_FogStart(0.f);
	CFogManager::GetInstance()->Set_FogEnd(1.f);

	T_CustomFog tCustomFog = T_CustomFog();
	tCustomFog.vColor_Start.w = 0;
	tCustomFog.vColor_End.w = 0;
	CFogManager::GetInstance()->SetCustomFog(tCustomFog);
}

void CRenderer::Reverse_Debug()
{
	if (!m_bRenderDebug) 
	{
		return;
	}

	// 랜더타겟 디버그
	//m_bRenderDeferred = FALSE;
	//m_bRenderDeferredAlpha = FALSE;

	// 콜라이더
	m_bDebug = !m_bDebug;

	// SSAO
	m_bSSAO_OnOff = !m_bSSAO_OnOff;

	// 안티엘리어싱
	m_bAntialiasing = !m_bAntialiasing;

	// 몰름
	//m_bIsCutIn = FALSE;

	// 민현이형꺼
	//m_bTest_For_PostProcess = FALSE;
	//m_fTest_For_PrstPrecess_Ratio_X = 0.f;


	m_bBaseGlow = !m_bBaseGlow;
	m_bTexGlow	= !m_bTexGlow;
	m_bGodRay	= !m_bGodRay;

	_matrix	matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_tViewCutFix.matView_ViewCut = matView;
	m_tViewCutFix.matProg_ViewCut = matProj;
	
}

void CRenderer::Set_ViewCut(_float fTime, _float fDif, _float fDelayTime)
{
	T_ViewCut tViewCut = T_ViewCut();

	tViewCut.fTimeMax = fTime;
	tViewCut.fTime = tViewCut.fTimeMax;

	tViewCut.fDelay = fDelayTime;
	tViewCut.fDis = fDif;

	m_tViewCut = tViewCut;
}

void CRenderer::Set_ViewCut_Matrix(_matrix matView, _matrix matProj)
{
	m_tViewCut.matView_ViewCut = matView;
	m_tViewCut.matProg_ViewCut = matProj;
}

void CRenderer::Update_ViewCut(const _float & fTimeDelta)
{
	// 뷰 컷의 랜더 여부
	m_bViewCut = FALSE;

	// 뷰 컷의 활성상태
	_bool bViewCut = TRUE;

	if (0.f >= m_tViewCut.fTimeMax)
	{
		// default 또는 유효하지 않은 값
		bViewCut = FALSE;
	}
	else
	{
		// 시간이 남았다면 차감
		if (0.f < m_tViewCut.fTime)
		{
			m_tViewCut.fTime -= fTimeDelta;
		}

		// 지연시간이 남았다면 차감
		if (0.f >= m_tViewCut.fTime)
		{
			m_tViewCut.fTime = 0.f;

			// 지연
			if (0.f < m_tViewCut.fDelay)
			{
				m_tViewCut.fDelay -= fTimeDelta;
			}
		}

		// 완전 종료
		if (0.f >= m_tViewCut.fTime && 0.f >= m_tViewCut.fDelay)
		{
			// 초기화
			m_tViewCut = T_ViewCut();

			bViewCut = FALSE;
		}
	}

	T_ViewCut tViewCut = m_tViewCut;

	// 고정형 뷰컷
	if (0.f < m_fTime_ViewCutFix)
	{
		m_fTime_ViewCutFix -= fTimeDelta;

		tViewCut = m_tViewCutFix;
		//tViewCut.matView_ViewCut = m_tViewCut.matView_ViewCut;
		//tViewCut.matProg_ViewCut = m_tViewCut.matProg_ViewCut;

		bViewCut = TRUE;
		m_bViewCut_Fix = TRUE;

		// 고정 종료
		if (0.f >= m_fTime_ViewCutFix)
		{
			m_fTime_ViewCutFix = 0.f;

			m_tViewCutFix = T_ViewCut();
			m_bViewCut_Fix = FALSE;
		}
	}

	// 뷰컷 적용
	float fLineBegin = 0.f;
	float fLineEnd = 0.f;
	if (bViewCut)
	{
		m_bViewCut = TRUE;

		// 컷 라인의 진행도 (0.f ~ 1.f)	( 100% - 라인의 남은 수명 %	= 현재 진행률	)
		// -> ex. 20% 진행되었을때		( 100% - 80 %				= 20%			)
		float fRatio = 1.f - (tViewCut.fTime / tViewCut.fTimeMax) * tViewCut.fSpeed;

		if (0.f >= fRatio)
		{
			fRatio = 0.f;
		}
		if (1.f <= fRatio)
		{
			fRatio = 1.f;
		}

		//fSpeed

		float fDis = 1.f + abs(tViewCut.fDis);

		if (0.f <= tViewCut.fDis)
		{
			// 위점의 시작위치를 차이 만큼 뒤로 뺀다
			fLineBegin = fDis * fRatio - tViewCut.fDis;
			fLineEnd = fDis * fRatio;
		}
		else
		{
			// 아래의 시작위치를 차이 만큼 뒤로 뺀다
			fLineBegin = fDis * fRatio;
			fLineEnd = fDis * fRatio + tViewCut.fDis;
		}
	}


	LPD3DXEFFECT pEffect = m_pPostProcessShader->Get_EffectHandle();
	pEffect->SetFloat("g_fCutLine_Begin", fLineBegin);
	pEffect->SetFloat("g_fCutLine_End", fLineEnd);
	
}

void CRenderer::Set_ViewCut_Fix_Easy(_float fTime, _float fRatio, _float fDis)
{
	//{ m_fTime_ViewCutFix = fTime; m_tViewCutFix = tViewcutFix; }

	T_ViewCut tViewCut = T_ViewCut();

	tViewCut.fTimeMax = fTime;
	tViewCut.fTime = fTime * (1.f - fRatio) + 0.003f;

	tViewCut.fDelay = 0.f;
	tViewCut.fDis = fDis;

	_matrix	matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	tViewCut.matView_ViewCut = matView;
	tViewCut.matProg_ViewCut = matProj;

	m_fTime_ViewCutFix = fTime;
	m_tViewCutFix = tViewCut;
}

void CRenderer::Set_ViewCut_Fix_Matrix(_matrix * pMatView, _matrix * pMatProg)
{
	if (pMatView)
	{
		m_tViewCutFix.matView_ViewCut = *pMatView;
	}

	if (pMatProg)
	{
		m_tViewCutFix.matProg_ViewCut = *pMatProg;
	}
}