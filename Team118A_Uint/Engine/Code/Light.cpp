#include "Light.h"
#include "LightMgr.h"

USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight()
{

}

// 씬(아마도)에서 빛정보를 만들어서 넘겨주면 이를 기반으로 빛을 생성함 
HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	m_iIndex = iIndex;

	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0, // 정적 버퍼(사이즈를 정해준 만큼 할당해서 사용한다는 뜻) - D3DUSAGE_DYNAMIC(동적 버퍼 : 파티클)
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL)))
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	float fDisX = 0.5f;
	float fDisY = 0.5f;

	pVertex[0].vPosition = _vec4(0.f - fDisX, 0.f - fDisY, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(ViewPort.Width - fDisX, 0.f - fDisY, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(ViewPort.Width - fDisX, ViewPort.Height - fDisY, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(0.f - fDisX, ViewPort.Height - fDisY, 0.f, 1.f);
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

	return S_OK;
}

// 빛을 만들고 받아온 빛정보로 오브젝트 를 레디 함
Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*		pInstance = new CLight(pGraphicDev);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pGraphicDev);
}

// 라이팅의 랜더연산을 한다 (그림자의 연산도 함께한다)
void Engine::CLight::Render_Light(LPD3DXEFFECT& pEffect)
{
	// 일반 빛 정보
	_vec4 vLightDir;
	D3DXVec4Normalize(&vLightDir, &_vec4(m_tLightInfo.Direction, 0.f));
	pEffect->SetVector("g_vLightDir", &vLightDir);

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&m_tLightInfo.Specular);

	///////////////////////////////////////////////////////////////////////////
	// 그림자용 상수 테이블

	_matrix		matProj, matInvProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInvProj, NULL, &matProj);

	_matrix		matView, matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, NULL, &matView);

	pEffect->SetMatrix("g_matProj", &matProj);	
	pEffect->SetMatrix("g_matInvProj", &matInvProj);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matInvView", &matInvView);

	pEffect->SetVector("g_vCamPos", (_vec4*)&matView._41);

	///////////////////////////////////////////////////////////////////////////
	// 그림자용 상수 테이블 - 빛 뷰 행렬

	// 빛 Eye
	_vec3 vPos_Eye = m_tLightInfo.Position;

	// 빛 At
	_float fX_At = vPos_Eye.x + m_tLightInfo.Direction.x;
	_float fY_At = vPos_Eye.y + m_tLightInfo.Direction.y;
	_float fZ_At = vPos_Eye.z + m_tLightInfo.Direction.z;
	_vec3 vPos_At = _vec3(fX_At, fY_At, fZ_At);

	_matrix matLightView;
	D3DXMatrixLookAtLH(&matLightView, &vPos_Eye, &vPos_At, &VEC_UP);

	pEffect->SetMatrix("g_matLightView", &matLightView);

	///////////////////////////////////////////////////////////////////////////
	// 랜더

	pEffect->CommitChanges();

	pEffect->BeginPass(0);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(FVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

