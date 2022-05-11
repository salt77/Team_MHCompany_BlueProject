#pragma once
#ifndef __FogManager_H__
#define __FogManager_H__

#include "Light.h"

//////////////////////////////
// 수정자 : 최은호
//////////////////////////////

BEGIN(Engine)

class ENGINE_DLL CFogManager : public CBase
{
	DECLARE_SINGLETON(CFogManager)

private:
	CFogManager();
	virtual ~CFogManager();
	virtual	void Free() override;

public:
	_float				Get_FogStart();
	void				Set_FogStart(_float fFogStart);

	_float				Get_FogEnd();
	void				Set_FogEnd(_float fFogEnd);

	_vec4				Get_FogColor();
	void				Set_FogColor(_vec4 FogColor);

	void				Reset_Fog();
	
	void				SetCustomFog(T_CustomFog tCustomFog) { m_tCustomFog = tCustomFog;}
	T_CustomFog			GetCustomFog() { return m_tCustomFog; }

private:
	float				m_fFogStart;
	float				m_fFogEnd; 
	_vec4				m_vFogColor;

	T_CustomFog			m_tCustomFog = T_CustomFog();
};


END
#endif // __FogManager_H__
