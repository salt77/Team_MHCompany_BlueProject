#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent()
	: m_pGraphicDev(nullptr)
	, m_bClone(false)
{

}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_bClone(true)
	, m_wstrProtoTag(rhs.m_wstrProtoTag)
	, m_wstrComTag(rhs.m_wstrComTag)
{
	m_pGraphicDev->AddRef();
}


CComponent::~CComponent()
{
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}

void CComponent::Save(HANDLE hFile, DWORD& dwByte)
{
}

void CComponent::Load(HANDLE hFile, DWORD& dwByte)
{
}
