#pragma once
#ifndef __TRAIL_FIX_H__
#define __TRAIL_FIX_H__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTrail_Fix : public CGameObject
{
protected:
	explicit CTrail_Fix(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrail_Fix(const CTrail_Fix& rhs);
	virtual ~CTrail_Fix();

public:
	static CTrail_Fix*			Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexTag, const _float& fLifeTime);
	virtual void			Free();

public:
	virtual HRESULT			Ready_Object(wstring wstrTexTag, const _float& fLifeTime);
	virtual _int			Update_Object(const _float& fTimeDelta)		override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(_int iPass)					override;

public:
	void					Add_Vertex(const _vec3& vTop, const _vec3& vBottom);
	void					Trail_TimeCheck(const _float& fTimeDelta);

	void					Set_Constantable();

public:
	const list<pair<_float, _vec3>>&	Get_ListTop()					{ return m_listTop;						}
	const list<pair<_float, _vec3>>&	Get_ListBottom()				{ return m_listBottom;					}

public:
	void					Set_ParentDead(const _bool& bState)			{ m_bParentDead = bState;				}
	void					Set_UV_X_Inverse(const _bool& bState)		{ m_bUV_X_Inverse = bState;				}
	void					Set_FadeOutSize(const _bool& bState)		{ m_bFadeOut_Size = bState;				}
	void					Set_FadeOutAlpha(const _bool& bState)		{ m_bFadeOut_Alpha = bState;			}
	void					Set_BlackMask(const _bool& bState)			{ m_bBlackMask = bState;				}
	void					Set_Interpol_ByCurPos(const _vec3& vCurrentTop, const _vec3& vCurrentBottom);

public:
	LPDIRECT3DVERTEXBUFFER9					m_pVB				= nullptr;
	LPDIRECT3DINDEXBUFFER9					m_pIB				= nullptr;
	LPDIRECT3DTEXTURE9						m_pTrailTex			= nullptr;
	CShader*								m_pShaderCom		= nullptr;

	list<pair<_float, _vec3>>				m_listTop;
	list<pair<_float, _vec3>>				m_listBottom;

	_float									m_fTrailTime		= 0.f;
	_float									m_fTrailLifeTime	= 1.f;
	_bool									m_bUV_X_Inverse		= FALSE;
	_bool									m_bFadeOut_Size		= FALSE;
	_bool									m_bFadeOut_Alpha	= FALSE;
	_bool									m_bBlackMask		= TRUE;
	_bool									m_bParentDead		= FALSE;
	_vec2									m_vUVRatio			= _vec2(1.f, 1.f);
};

END

#endif // !__TRAIL_FIX_H__