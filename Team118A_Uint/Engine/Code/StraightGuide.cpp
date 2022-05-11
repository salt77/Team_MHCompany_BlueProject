#include "StraightGuide.h"




CStraightGuide::CStraightGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CSkillGuideObject(pGraphicDev, eObjTag)
{
}

CStraightGuide::CStraightGuide(const CStraightGuide & rhs)
	:CSkillGuideObject(rhs)
{
}

CStraightGuide::~CStraightGuide()
{
}

CStraightGuide * CStraightGuide::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag,_float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CStraightGuide* pInstance = new CStraightGuide(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(fDistance, fWidth, fDelay, bUseDelayMesh, wstrTexPath)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CStraightGuide::Ready_Object(_float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath)
{
	m_wstrTexturePath = wstrTexPath;
	CSkillGuideObject::Ready_Object();

	m_tGuideOption.fDelay = fDelay;
	m_tGuideOption.fWidth = fWidth;
	m_tGuideOption.fMaxDistance = fDistance;

	m_fDelay = fDelay;
	m_bDelayMesh = bUseDelayMesh;

	m_iGuideVertexCount = 4;
	m_iGuideTriCount = 2;

	m_fDistance = fDistance;
	m_fWidth = fWidth;
	

	//버텍스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(m_iGuideVertexCount * sizeof(VTXUI)), 0, 0, D3DPOOL_MANAGED, &m_pGuideVertexBuffer, 0);

	VTXUI* pVertexBuffer = nullptr;
	m_pGuideVertexBuffer->Lock(0, 0, (void**)&pVertexBuffer, 0);

	pVertexBuffer[0].vPosition = _vec3(-fWidth * 0.5f, m_fAddY, 0.f);
	pVertexBuffer[0].vTexUV = _vec2(0.f, 0.f);

	pVertexBuffer[1].vPosition = _vec3(fWidth * 0.5f, m_fAddY, 0.f);
	pVertexBuffer[1].vTexUV = _vec2(1.f, 0.f);

	pVertexBuffer[2].vPosition = _vec3(-fWidth * 0.5f, m_fAddY, fDistance);
	pVertexBuffer[2].vTexUV = _vec2(0.f, 1.f);

	pVertexBuffer[3].vPosition = _vec3(fWidth * 0.5f, m_fAddY, fDistance);
	pVertexBuffer[3].vTexUV = _vec2(1.f, 1.f);

	m_pGuideVertexBuffer->Unlock();

	
	// 인덱스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iGuideTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pGuideIndexBuffer, NULL);

	INDEX16* pIndexBuffer = nullptr;

	m_pGuideIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

	pIndexBuffer[0] = { 0,2,1 };
	pIndexBuffer[1] = { 2,3,1 };

	m_pGuideIndexBuffer->Unlock();


	if (bUseDelayMesh)
	{
		m_iDelayVertexCount = m_iGuideVertexCount;
		m_iDelayTriCount = m_iGuideTriCount;
		Update_DelayMesh();
	}
	return S_OK;
}

void CStraightGuide::Free()
{
	CSkillGuideObject::Free();
}

HRESULT CStraightGuide::Ready_Object()
{
	return E_NOTIMPL;
}

_int CStraightGuide::Start_Object(const _float & fTimeDelta)
{
	return CSkillGuideObject::Start_Object(fTimeDelta);
	
}

_int CStraightGuide::Update_Object(const _float & fTimeDelta)
{
	
	CSkillGuideObject::Update_Object(fTimeDelta);
	return RETURN_OK;
}

_int CStraightGuide::LateUpdate_Object(const _float & fTimeDelta)
{
	CSkillGuideObject::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CStraightGuide::Render_Object(_int iPass)
{
	CSkillGuideObject::Render_Object(iPass);
}

void CStraightGuide::Update_DelayMesh()
{
	if (!m_bDelayMesh)
	{
		return;
	}


	//버텍스 버퍼
	_float fRatio = m_fDelayTimer / m_fDelay;
	_float fDelayDistance = fRatio * m_fDistance;

	if (!m_pDelayVertexBuffer)
	{
		CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(m_iDelayVertexCount * sizeof(VTXUI)), 0, 0, D3DPOOL_MANAGED, &m_pDelayVertexBuffer, 0);
	}

	VTXUI* pVertexBuffer = nullptr;
	m_pDelayVertexBuffer->Lock(0, 0, (void**)&pVertexBuffer, 0);

	pVertexBuffer[0].vPosition = _vec3(-m_fWidth * 0.5f, m_fDelayAddY, 0.f);
	pVertexBuffer[0].vTexUV = _vec2(0.f, 0.f);

	pVertexBuffer[1].vPosition = _vec3(m_fWidth * 0.5f, m_fDelayAddY, 0.f);
	pVertexBuffer[1].vTexUV = _vec2(1.f, 0.f);

	pVertexBuffer[2].vPosition = _vec3(-m_fWidth * 0.5f, m_fDelayAddY, fDelayDistance);
	pVertexBuffer[2].vTexUV = _vec2(0.f, 1.f);

	pVertexBuffer[3].vPosition = _vec3(m_fWidth * 0.5f, m_fDelayAddY, fDelayDistance);
	pVertexBuffer[3].vTexUV = _vec2(1.f, 1.f);
	
	m_pDelayVertexBuffer->Unlock();

	// 인덱스 버퍼
	if (!m_pDelayIndexBuffer)
	{
		CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iDelayTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pDelayIndexBuffer, NULL);

		INDEX16* pIndexBuffer = nullptr;

		m_pDelayIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

		pIndexBuffer[0] = { 0,2,1 };
		pIndexBuffer[1] = { 2,3,1 };

		m_pDelayIndexBuffer->Unlock();
	}

}
