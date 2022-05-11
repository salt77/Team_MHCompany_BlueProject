#include "Line.h"
USING(Engine)

IMPLEMENT_SINGLETON(CD3DLine)

Engine::CD3DLine::CD3DLine()
	: m_pSprite(nullptr)
	, m_pLine(nullptr)
{
}

Engine::CD3DLine::~CD3DLine()
{
	Free();
}

HRESULT CD3DLine::Ready_Line(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_pLine->SetWidth(10.f);
	m_pLine->SetAntialias(TRUE);
	D3DXCreateSprite(pGraphicDev, &m_pSprite);

	return S_OK;
}

void CD3DLine::Render_Line(_vec3* vSrc, const _int& iSize, const DWORD color, const _matrix& matWorld)
{
	_matrix matItentity;
	D3DXMatrixIdentity(&matItentity);

	_matrix matView = *D3DXMatrixIdentity(&matView);
	_matrix	matProj = *D3DXMatrixIdentity(&matProj);
	m_pSprite->SetTransform(&matItentity);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	for (_int i = 0; i < iSize; ++i)
	{
		D3DXVec3TransformCoord(&vSrc[i], &vSrc[i], &matView);

		if (vSrc[i].z < 1e-5f)
			vSrc[i].z = 1e-5f;
	
		D3DXVec3TransformCoord(&vSrc[i], &vSrc[i], &matProj);
	}

	m_pLine->Begin();
	m_pLine->DrawTransform(vSrc, iSize, &matItentity, 0xffffffff);
	m_pLine->End();
}

void CD3DLine::Render_Line2D(_vec2 * vSrc, const _int & iSize, const DWORD color, const _matrix & matWorld)
{
	_matrix matItentity;
	D3DXMatrixIdentity(&matItentity);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	m_pSprite->SetTransform(&matWorld);
	m_pLine->Begin();
	m_pLine->Draw(vSrc, iSize, color);
	m_pLine->End();
	m_pSprite->End();
};

void Engine::CD3DLine::Free()
{
	Engine::Safe_Release(m_pLine);
	Engine::Safe_Release(m_pSprite);
}
