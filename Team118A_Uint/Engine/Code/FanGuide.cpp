#include "FanGuide.h"




CFanGuide::CFanGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CSkillGuideObject(pGraphicDev, eObjTag)
{
}

CFanGuide::CFanGuide(const CFanGuide & rhs)
	:CSkillGuideObject(rhs)
{
}

CFanGuide::~CFanGuide()
{
}

CFanGuide * CFanGuide::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CFanGuide* pInstance = new CFanGuide(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(fDistance, fAngle, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CFanGuide::Ready_Object(_float fDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	m_wstrTexturePath = wstrTexPath;
	CSkillGuideObject::Ready_Object();

	m_tGuideOption.fDelay = fDelay;
	m_tGuideOption.fAngle = fAngle;
	m_tGuideOption.fMaxDistance = fDistance;

	m_fDelay = fDelay;
	m_bDelayMesh = bUseDelayMesh;

	m_fDistance = fDistance;
	if (iVertexCount < 1)
	{
		m_iVertexCount = 1;
	}
	else
	{
		m_iVertexCount = iVertexCount;
	}
	m_fAngle = fAngle;

	m_iGuideVertexCount = 4 + m_iVertexCount;
	m_iGuideTriCount = m_iVertexCount + 1;


	

	//버텍스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(m_iGuideVertexCount * sizeof(VTXUI)), 0, 0, D3DPOOL_MANAGED, &m_pGuideVertexBuffer, 0);

	VTXUI* pVertexBuffer = nullptr;
	m_pGuideVertexBuffer->Lock(0, 0, (void**)&pVertexBuffer, 0);

	//원점 2점
	pVertexBuffer[0].vPosition = _vec3(0.f, m_fAddY, 0.f);
	pVertexBuffer[0].vTexUV = _vec2(0.f, 0.f);

	pVertexBuffer[1].vPosition = _vec3(0.f, m_fAddY, 0.f);
	pVertexBuffer[1].vTexUV = _vec2(1.f, 0.f);

	_float fDegree = -m_fAngle * 0.5f;
	_float fAddDegree = m_fAngle / _float(m_iVertexCount + 1);
	_float fUVX = 0.f;
	_float fAddUVX = 1.f / _float(m_iVertexCount + 1);
	_vec3 vLook = VEC_LOOK;
	_vec3 vVertexLook;
	_matrix matRotY;
	//부채꼴 곡면에 생성되는 정점
	for (_int i = 0; i < m_iVertexCount+2; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fDegree * Deg2Rad);
		D3DXVec3TransformNormal(&vVertexLook, &vLook, &matRotY);
		D3DXVec3Normalize(&vVertexLook, &vVertexLook);

		pVertexBuffer[2 + i].vPosition = vVertexLook * m_fDistance;
		pVertexBuffer[2 + i].vPosition.y = m_fAddY;
		pVertexBuffer[2 + i].vTexUV = _vec2(fUVX, 1.f);
	
		fDegree += fAddDegree;
		fUVX += fAddUVX;
	}
	
	m_pGuideVertexBuffer->Unlock();

	
	// 인덱스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iGuideTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pGuideIndexBuffer, NULL);

	INDEX16* pIndexBuffer = nullptr;

	m_pGuideIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

	for (unsigned short i = 0; i < unsigned short(m_iGuideTriCount); ++i)
	{
		if (i >= unsigned short(m_iGuideTriCount >> 1))
		{
			pIndexBuffer[i] = { unsigned short(1), unsigned short(2 + i), unsigned short(3 + i) };
		}
		else
		{
			pIndexBuffer[i] = { unsigned short(0), unsigned short(2 + i), unsigned short(3 + i) };
		}
	}

	m_pGuideIndexBuffer->Unlock();


	if (bUseDelayMesh)
	{
		m_iDelayVertexCount = m_iGuideVertexCount;
		m_iDelayTriCount = m_iGuideTriCount;
		Update_DelayMesh();
	}
	return S_OK;
}

void CFanGuide::Free()
{
	CSkillGuideObject::Free();
}

HRESULT CFanGuide::Ready_Object()
{
	return E_NOTIMPL;
}

_int CFanGuide::Start_Object(const _float & fTimeDelta)
{
	return CSkillGuideObject::Start_Object(fTimeDelta);
	
}

_int CFanGuide::Update_Object(const _float & fTimeDelta)
{
	
	CSkillGuideObject::Update_Object(fTimeDelta);
	return RETURN_OK;
}

_int CFanGuide::LateUpdate_Object(const _float & fTimeDelta)
{
	CSkillGuideObject::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CFanGuide::Render_Object(_int iPass)
{
	CSkillGuideObject::Render_Object(iPass);
}

void CFanGuide::Update_DelayMesh()
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

	pVertexBuffer[0].vPosition = _vec3(0.f, m_fAddY, 0.f);
	pVertexBuffer[0].vTexUV = _vec2(0.f, 0.f);

	pVertexBuffer[1].vPosition = _vec3(0.f, m_fAddY, 0.f);
	pVertexBuffer[1].vTexUV = _vec2(1.f, 0.f);

	_float fUVX = 0.f;
	_float fAddUVX = 1.f / _float(m_iVertexCount + 1);

	_float fDegree = -m_fAngle * 0.5f;
	_float fAddDegree = m_fAngle / _float(m_iVertexCount + 1);
	
	_vec3 vLook = VEC_LOOK;
	_vec3 vVertexLook;
	_matrix matRotY;


	//부채꼴 곡면에 생성되는 정점
	for (_int i = 0; i < m_iVertexCount + 2; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fDegree * Deg2Rad);
		D3DXVec3TransformNormal(&vVertexLook, &vLook, &matRotY);
		D3DXVec3Normalize(&vVertexLook, &vVertexLook);

		pVertexBuffer[2 + i].vPosition = vVertexLook * fDelayDistance;
		pVertexBuffer[2 + i].vPosition.y = m_fDelayAddY;
		pVertexBuffer[2 + i].vTexUV = _vec2(fUVX, 1.f);

		fDegree += fAddDegree;
		fUVX += fAddUVX;
	}

	m_pDelayVertexBuffer->Unlock();


	// 인덱스 버퍼
	if (!m_pDelayIndexBuffer)
	{
		CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iDelayTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pDelayIndexBuffer, NULL);

		INDEX16* pIndexBuffer = nullptr;

		m_pDelayIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

		for (unsigned short i = 0; i < unsigned short(m_iDelayTriCount); ++i)
		{
			if (i >= unsigned short(m_iDelayTriCount >> 1))
			{
				pIndexBuffer[i] = { unsigned short(1), unsigned short(2 + i), unsigned short(3 + i) };
			}
			else
			{
				pIndexBuffer[i] = { unsigned short(0), unsigned short(2 + i), unsigned short(3 + i) };
			}
		}

		m_pDelayIndexBuffer->Unlock();
	}

}
