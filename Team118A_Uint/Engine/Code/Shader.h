#pragma once
#ifndef __Shader_H__
#define __Shader_H__

#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint	iMaxPass, const _tchar* pFilePath);
	virtual CComponent*	Clone();
	virtual void		Free();

	_bool				IsDMesh() { return m_bDMesh; }
	void				SetDMesh() { m_bDMesh = TRUE; }

public:
	HRESULT				Ready_Shader(const _tchar* pFilePath, _uint iMaxpass);

public:
	LPD3DXEFFECT		Get_EffectHandle() { return m_pEffect; }

	void				BeginShader();
	void				EndShader();

	void				BeginPass(_uint iPass = 0);
	void				ChangePass(_uint iPass);
	void				EndPass();

private:
	LPD3DXEFFECT		m_pEffect;		// 쉐이더를 제어하가 위한 컴객체
	LPD3DXBUFFER		m_pErrMsg;		// 작성한 hlsl 코드가 올바른지 검사 후, 발생한 에러메세지를 저장하기 위한 버퍼

	_uint				m_iMaxPass = 0;
	_uint				m_iNowPass = 0;
	_uint				m_iPassCount = 0;
	_bool				m_bDMesh = FALSE;
};

END
#endif // __Shader_H__
