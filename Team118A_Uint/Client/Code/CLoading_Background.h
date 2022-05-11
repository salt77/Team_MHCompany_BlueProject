#pragma once
#ifndef __Loading_Background_H__
#define __Loading_Background_H__

#include "GameObject.h"
#include "Engine_Define.h"
#include "UI_Object.h"

typedef struct tagLoadingImages
{
	tagLoadingImages(_float _fMaxTime, wstring _wstrFullPath, E_Anchor _eStartAnchor, _float _fRatio)
		: fMaxTime(_fMaxTime), wstrFullPath(_wstrFullPath), eStartAnchor(_eStartAnchor), fRatio(_fRatio)
	{
		fTiming = fMaxTime;
	}
	void Reset()
	{
		fTiming = fMaxTime;
	}

	_float fTiming;
	_float fMaxTime;
	_float fRatio = 0.1f;
	wstring wstrFullPath;
	E_Anchor eStartAnchor;

}T_LoadingImages;

class CLoading_Background final : public CGameObject
{
private:
	explicit CLoading_Background(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CLoading_Background(const CLoading_Background& rhs);
	virtual ~CLoading_Background();

public:
	static	CLoading_Background*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void					Free() override;

public:
	virtual HRESULT					Ready_Object()								override;
	virtual _int					Start_Object(const _float& fTimeDelta)		override;
	virtual _int					Update_Object(const _float& fTimeDelta)		override;
	virtual _int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					Render_Object(_int iPass)					override;

public:
	void							Add_Component();
	void							Check_Image(const _float& fTimeDelta);

private:
	CUI_Object*						m_pBackground			= nullptr;
	CUI_Object*						m_pLoadingText_OutLine	= nullptr;
	CUI_Object*						m_p15Age				= nullptr;
	CUI_Object*						m_pLogo					= nullptr;

	queue<T_LoadingImages*>			m_qImageStruct;
	E_Anchor						m_eAnchor	= E_Anchor::Anchor_Center;
	_float							m_fTimer	= 0.f;

	_float							m_fSizeX	= 0.f;
	_float							m_fSizeY	= 0.f;


	_float							m_fExpansion = 0.1f;
};

#endif // !__Loading_Background_H__