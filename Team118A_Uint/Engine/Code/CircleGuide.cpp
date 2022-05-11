#include "CircleGuide.h"




CCircleGuide::CCircleGuide(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CSkillGuideObject(pGraphicDev, eObjTag)
{
}

CCircleGuide::CCircleGuide(const CCircleGuide & rhs)
	:CSkillGuideObject(rhs)
{
}

CCircleGuide::~CCircleGuide()
{
}

CCircleGuide * CCircleGuide::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CCircleGuide* pInstance = new CCircleGuide(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(fRadius, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CCircleGuide::Ready_Object(_float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	m_wstrTexturePath = wstrTexPath;
	CSkillGuideObject::Ready_Object();

	m_tGuideOption.fDelay = fDelay;
	m_tGuideOption.fMaxDistance = fRadius;

	m_fDelay = fDelay;
	m_bDelayMesh = bUseDelayMesh;

	m_fRadius = fRadius;
	
	if (iVertexCount < 4)
	{
		m_iVertexCount = 4;
	}
	else
	{
		m_iVertexCount = iVertexCount;
	}
	

	m_iGuideVertexCount = m_iVertexCount * 2 + 2;
	m_iGuideTriCount = m_iVertexCount * 2;


	

	//버텍스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(m_iGuideVertexCount * sizeof(VTXUI)), 0, 0, D3DPOOL_MANAGED, &m_pGuideVertexBuffer, 0);

	VTXUI* pVertexBuffer = nullptr;
	m_pGuideVertexBuffer->Lock(0, 0, (void**)&pVertexBuffer, 0);

	_vec3 vLook = VEC_LOOK;
	_float fAngle = 0.f;
	_float fAddAngle = 360.f / (_float)m_iVertexCount;

	_vec3 vVertexLook;
	_matrix matRotY;

	_float fUVX = 0.f;
	_float fAddUVX = 1.f / m_iVertexCount;
	
	for (_int i = 0; i < m_iVertexCount + 1; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);
		D3DXVec3TransformNormal(&vVertexLook, &vLook, &matRotY);
		D3DXVec3Normalize(&vVertexLook, &vVertexLook);

		pVertexBuffer[i].vPosition = vVertexLook * 0.f;
		pVertexBuffer[i].vPosition.y = m_fAddY;
		pVertexBuffer[i].vTexUV = _vec2(fUVX, 0.f);

		pVertexBuffer[i + m_iVertexCount + 1].vPosition = vVertexLook * m_fRadius;
		pVertexBuffer[i + m_iVertexCount + 1].vPosition.y = m_fAddY;
		pVertexBuffer[i + m_iVertexCount + 1].vTexUV = _vec2(fUVX, 1.f);

		fAngle += fAddAngle;
		fUVX += fAddUVX;
	}
	
	
	m_pGuideVertexBuffer->Unlock();

	
	// 인덱스 버퍼

	CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iGuideTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pGuideIndexBuffer, NULL);

	INDEX16* pIndexBuffer = nullptr;

	m_pGuideIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

	for (_int i = 0; i < m_iVertexCount; ++i)
	{
		_int iIndex = i * 2;
		pIndexBuffer[iIndex] = { unsigned short(i), unsigned short(i + m_iVertexCount+1), unsigned short(i + 1) };
		pIndexBuffer[iIndex + 1] = { unsigned short(i + m_iVertexCount+1), unsigned short(i + m_iVertexCount + 2), unsigned short(i + 1) };
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

void CCircleGuide::Free()
{
	CSkillGuideObject::Free();
}

HRESULT CCircleGuide::Ready_Object()
{
	return E_NOTIMPL;
}

_int CCircleGuide::Start_Object(const _float & fTimeDelta)
{
	return CSkillGuideObject::Start_Object(fTimeDelta);
	
}

_int CCircleGuide::Update_Object(const _float & fTimeDelta)
{
	
	CSkillGuideObject::Update_Object(fTimeDelta);
	return RETURN_OK;
}

_int CCircleGuide::LateUpdate_Object(const _float & fTimeDelta)
{
	CSkillGuideObject::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CCircleGuide::Render_Object(_int iPass)
{
	CSkillGuideObject::Render_Object(iPass);
}

void CCircleGuide::Update_DelayMesh()
{

	
	if (!m_bDelayMesh)
	{
		return;
	}
	//버텍스 버퍼
	_float fRatio = m_fDelayTimer / m_fDelay;
	_float fDelayDistance = fRatio * (m_fRadius);

	if (!m_pDelayVertexBuffer)
	{
		CGraphicDev::GetInstance()->GetDevice()->CreateVertexBuffer((_uint)(m_iDelayVertexCount * sizeof(VTXUI)), 0, 0, D3DPOOL_MANAGED, &m_pDelayVertexBuffer, 0);
	}

	VTXUI* pVertexBuffer = nullptr;
	m_pDelayVertexBuffer->Lock(0, 0, (void**)&pVertexBuffer, 0);

	_vec3 vLook = VEC_LOOK;
	_float fAngle = 0.f;
	_float fAddAngle = 360.f / (_float)m_iVertexCount;

	_vec3 vVertexLook;
	_matrix matRotY;

	_float fUVX = 0.f;
	_float fAddUVX = 1.f / m_iVertexCount;
	for (_int i = 0; i < m_iVertexCount + 1; ++i)
	{
		D3DXMatrixRotationY(&matRotY, fAngle * Deg2Rad);
		D3DXVec3TransformNormal(&vVertexLook, &vLook, &matRotY);
		D3DXVec3Normalize(&vVertexLook, &vVertexLook);

		pVertexBuffer[i].vPosition = vVertexLook * 0.f;
		pVertexBuffer[i].vPosition.y = m_fDelayAddY;
		pVertexBuffer[i].vTexUV = _vec2(fUVX, 0.f);

		pVertexBuffer[i + m_iVertexCount + 1].vPosition = vVertexLook * fDelayDistance;
		pVertexBuffer[i + m_iVertexCount + 1].vPosition.y = m_fDelayAddY;
		pVertexBuffer[i + m_iVertexCount + 1].vTexUV = _vec2(fUVX, 1.f);

		fAngle += fAddAngle;
		fUVX += fAddUVX;
	}


	m_pDelayVertexBuffer->Unlock();



	// 인덱스 버퍼
	if (!m_pDelayIndexBuffer)
	{
		CGraphicDev::GetInstance()->GetDevice()->CreateIndexBuffer(m_iDelayTriCount * sizeof(INDEX16), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pDelayIndexBuffer, NULL);

		INDEX16* pIndexBuffer = nullptr;

		m_pDelayIndexBuffer->Lock(0, 0, (void**)&pIndexBuffer, 0);

		for (_int i = 0; i < m_iVertexCount; ++i)
		{
			_int iIndex = i * 2;
			pIndexBuffer[iIndex] = { unsigned short(i), unsigned short(i + m_iVertexCount + 1), unsigned short(i + 1) };
			pIndexBuffer[iIndex + 1] = { unsigned short(i + m_iVertexCount + 1), unsigned short(i + m_iVertexCount + 2), unsigned short(i + 1) };
		}

		m_pDelayIndexBuffer->Unlock();
	}

}
