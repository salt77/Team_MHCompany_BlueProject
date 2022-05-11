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

// _float로 안개의 시작점을 반환 (안개가 시작하는 시점)
_float CFogManager::Get_FogStart()
{
	return m_fFogStart;
}

// _float로 안개의 시작점을 변경 (안개가 시작하는 시점)
void CFogManager::Set_FogStart(_float fFogStart)
{
	m_fFogStart = fFogStart;
}

// _float로 안개의 끝점을 반환 (안개의 농도가 완전해지는 시점)
_float CFogManager::Get_FogEnd()
{
	return m_fFogEnd;
}

// _float로 안개의 끝점을 변경 (안개의 농도가 완전해지는 시점)
void CFogManager::Set_FogEnd(_float fFogEnd)
{
	m_fFogEnd = fFogEnd;
}

// _vec4로 안개의 색을 반환
_vec4 CFogManager::Get_FogColor()
{
	return m_vFogColor;
}

// D3DXCOLOR로 안개의 색을 변경
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

