#include "Trail_Fix.h"
#include "Renderer.h"
#include "ProtoMgr.h"

USING(Engine)

CTrail_Fix::CTrail_Fix(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_Trail)
{
}

CTrail_Fix::CTrail_Fix(const CTrail_Fix & rhs)
	: CGameObject(rhs)
{
}

CTrail_Fix::~CTrail_Fix()
{
}

CTrail_Fix * CTrail_Fix::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexTag, const _float& fLifeTime)
{
	CTrail_Fix* pInstance = new CTrail_Fix(pGraphicDev);

	if (E_FAIL == pInstance->Ready_Object(wstrTexTag, fLifeTime))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrail_Fix::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	CGameObject::Free();
}

HRESULT CTrail_Fix::Ready_Object(wstring wstrTexTag, const _float& fLifeTime)
{
	CGameObject::Ready_Object();

	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Trail")));

	m_fTrailLifeTime = fLifeTime;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrTexTag, &m_pTrailTex);
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrTexTag, &m_pTrailTex);


	return S_OK;
}

_int CTrail_Fix::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Trail_TimeCheck(fTimeDelta);
	Billboard();

	return iExit;
}

_int CTrail_Fix::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);


	if (m_listTop.empty() && m_bParentDead)
	{
		Set_Dead(TRUE);
	}
	else
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_EFFECT, this);
	}
	return iExit;
}

void CTrail_Fix::Render_Object(_int iPass)
{
	if (nullptr == m_pShaderCom)
	{
		return;
	}

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	pEffect->AddRef();

	Set_Constantable();
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	if (2 > m_listTop.size())
	{
		pEffect->EndPass();
		pEffect->End();
		Safe_Release(pEffect);
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
		return;
	}

	DWORD dwVtxCnt = (DWORD)m_listTop.size();
	dwVtxCnt += (DWORD)m_listBottom.size();

	DWORD dwTriCnt = dwVtxCnt - 2;

	_ulong						dwVtxSize = sizeof(VTXTRAIL);
	_ulong						dwFVF = FVF_TRAIL;
	_ulong						dwIdxSize = sizeof(INDEX16);
	D3DFORMAT					IdxFmt = D3DFMT_INDEX16;

	VTXTRAIL*	pVertices = nullptr;
	INDEX16*	pIndices = nullptr;

	m_pGraphicDev->CreateVertexBuffer(dwVtxSize * dwVtxCnt,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		dwFVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	m_pGraphicDev->CreateIndexBuffer(dwIdxSize * dwTriCnt,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	list<pair<_float, _vec3>>::iterator		iter_Top = m_listTop.begin();
	list<pair<_float, _vec3>>::iterator		iter_Bottom = m_listBottom.begin();

	// Vertex 
	_float fLifeRT = 0.f;
	for (_int i = 0; i < (_int)dwVtxCnt; i += 2)
	{
		// Min
		fLifeRT = 1.f - (*iter_Top).first / m_fTrailLifeTime;
		pVertices[i].vPosition = (*iter_Top).second;
		pVertices[i].vTexUV = _vec2(fLifeRT, 0.f);
		pVertices[i].vRatio = _vec2(fLifeRT, fLifeRT);
		++iter_Top;

		// Max
		pVertices[i + 1].vPosition = (*iter_Bottom).second;
		pVertices[i + 1].vTexUV = _vec2(fLifeRT, 1.f);
		pVertices[i + 1].vRatio = _vec2(fLifeRT, fLifeRT);
		++iter_Bottom;


		if (m_bUV_X_Inverse)
		{
			pVertices[i].vTexUV.x = 1.f - pVertices[i].vTexUV.x;
			pVertices[i + 1].vTexUV.x = 1.f - pVertices[i + 1].vTexUV.x;
		}


		pVertices[i].vTexUV.x *= m_vUVRatio.x;
		pVertices[i].vTexUV.y *= m_vUVRatio.y;
		pVertices[i + 1].vTexUV.x *= m_vUVRatio.x;
		pVertices[i + 1].vTexUV.y *= m_vUVRatio.y;
	}

	// Index
	for (_int i = 0; i < (_int)dwTriCnt; i++)
	{
		if (i % 2 == 0)
		{
			pIndices[i]._0 = i;
			pIndices[i]._1 = i + 1;
			pIndices[i]._2 = i + 2;
		}
		else
		{
			pIndices[i]._0 = i;
			pIndices[i]._1 = i + 2;
			pIndices[i]._2 = i + 1;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, dwVtxSize);
	m_pGraphicDev->SetFVF(dwFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, dwVtxCnt, 0, dwTriCnt); // 인덱스 버퍼도 활용하여 출력하는 함수

	pEffect->EndPass();
	pEffect->End();
	Safe_Release(pEffect);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

void CTrail_Fix::Add_Vertex(const _vec3 & vTop, const _vec3& vBottom)
{
	m_listTop.push_back(make_pair(m_fTrailLifeTime, vTop));
	m_listBottom.push_back(make_pair(m_fTrailLifeTime, vBottom));
}

void CTrail_Fix::Trail_TimeCheck(const _float& fTimeDelta)
{
	list<pair<_float, _vec3>>::iterator iter_T = m_listTop.begin();
	list<pair<_float, _vec3>>::iterator iter_B = m_listBottom.begin();
	for (; iter_T != m_listTop.end();)
	{
		iter_T->first -= fTimeDelta;
		iter_B->first -= fTimeDelta;
		if (0 >= iter_T->first)
		{
			iter_T = m_listTop.erase(iter_T);
			iter_B = m_listBottom.erase(iter_B);
		}
		else
		{
			if (m_bFadeOut_Size)
			{
				iter_T->second = CUtility::Lerp_vec3(iter_T->second, iter_B->second, (1.f - iter_T->first / m_fTrailLifeTime) * 0.5f);
				iter_B->second = CUtility::Lerp_vec3(iter_B->second, iter_T->second, (1.f - iter_T->first / m_fTrailLifeTime) * 0.5f);
			}
			++iter_T;
			++iter_B;
		}
	}
}

// 오브젝트 셋 콘스탄트 테이블
void CTrail_Fix::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;
	matWorld = Get_Transform()->Get_WorldMatrix();
	//0204mat
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	//0204mat
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetTexture("g_TrailTexture", m_pTrailTex);

	pEffect->SetBool("g_bBlackMask", m_bBlackMask);
	pEffect->SetBool("g_bFadeOut_Alpha", m_bFadeOut_Alpha);
	pEffect->SetBool("g_bUV_X_Inverse", m_bUV_X_Inverse);


	pEffect->SetFloat("g_fLifeRT", m_fTrailTime / m_fTrailLifeTime);

}

void CTrail_Fix::Set_Interpol_ByCurPos(const _vec3& vCurrentTop, const _vec3& vCurrentBottom) 
{
	list<pair<_float, _vec3>>::iterator		iter_Top = m_listTop.begin(); 
	list<pair<_float, _vec3>>::iterator		iter_Bottom = m_listBottom.begin();

	_float	fDistance = D3DXVec3Length(&(vCurrentTop - iter_Top->second)); 

	for (; iter_Top != m_listTop.end(); ++iter_Top)
	{
		_vec3	vDirTop = *D3DXVec3Normalize(&vDirTop, &(vCurrentTop - iter_Top->second));
		_vec3	vDirBottom = *D3DXVec3Normalize(&vDirBottom, &(vCurrentBottom - iter_Bottom->second));

		iter_Top->second += vDirTop * fDistance;
		iter_Bottom->second += vDirBottom * fDistance;

		if (iter_Bottom != m_listBottom.end()) 
		{
			++iter_Bottom; 
		}
		else
		{
			break; 
		}
	} 
}
