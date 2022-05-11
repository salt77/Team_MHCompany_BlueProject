#include "FogManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFogManager)

Engine::CFogManager::CFogManager()
{
	Reset_Fog();
}

Engine::CFogManager::~CFogManager()
{
	Free();
}

void Engine::CFogManager::Free()
{
}

// _float�� �Ȱ��� �������� ��ȯ (�Ȱ��� �����ϴ� ����)
_float CFogManager::Get_FogStart()
{
	return m_fFogStart;
}

// _float�� �Ȱ��� �������� ���� (�Ȱ��� �����ϴ� ����)
void CFogManager::Set_FogStart(_float fFogStart)
{
	m_fFogStart = fFogStart;
}

// _float�� �Ȱ��� ������ ��ȯ (�Ȱ��� �󵵰� ���������� ����)
_float CFogManager::Get_FogEnd()
{
	return m_fFogEnd;
}

// _float�� �Ȱ��� ������ ���� (�Ȱ��� �󵵰� ���������� ����)
void CFogManager::Set_FogEnd(_float fFogEnd)
{
	m_fFogEnd = fFogEnd;
}

// _vec4�� �Ȱ��� ���� ��ȯ
_vec4 CFogManager::Get_FogColor()
{
	return m_vFogColor;
}

// D3DXCOLOR�� �Ȱ��� ���� ����
void CFogManager::Set_FogColor(_vec4 FogColor)
{
	m_vFogColor = FogColor;
}

void CFogManager::Reset_Fog()
{
	m_fFogStart = 5000.f;
	m_fFogEnd = 5000.f;
	m_vFogColor = _vec4(1.f, 0.f, 0.f, 1.f);

	SetCustomFog(T_CustomFog());
}

