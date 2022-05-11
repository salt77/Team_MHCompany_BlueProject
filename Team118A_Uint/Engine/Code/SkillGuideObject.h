#pragma once

#ifndef __SKILL_GUIDE_OBJECT_H__
#define __SKILL_GUIDE_OBJECT_H__

#include "GameObject.h"
#include "Texture.h"
BEGIN(Engine)

class CSkillGuideObject abstract:
	public CGameObject
{
protected:
	CSkillGuideObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	CSkillGuideObject(const CSkillGuideObject& rhs);
	virtual ~CSkillGuideObject();

public:
	virtual void			Free()override;

	virtual HRESULT			Ready_Object();


	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;

	const T_GUIDE_OPTION&	Get_GuideOption() { m_tGuideOption.vPosition = Get_Position(); m_tGuideOption.vRotation = Get_Angle(); return m_tGuideOption; 	}

	virtual void			Component_Update() { m_pTransformCom->Update_Component(0.f);  }
protected:
	virtual void			Update_DelayMesh()PURE;
	void					Delay_Check(_float fTimeDelta);
	void					Add_Component();
	virtual void			Set_Constantable()override;

	void					Set_GuideDead() { m_bGuideDead = TRUE; }
protected:
	_bool					m_bGuideDead = FALSE;
	T_GUIDE_OPTION			m_tGuideOption;

	CShader*				m_pShaderCom = nullptr;

	wstring					m_wstrTexturePath = L"";
	LPDIRECT3DTEXTURE9      m_pTexture = nullptr;
	//CTexture*				m_pTextureCom = nullptr;


	_float					m_fDelayTimer = 0.f;
	_float					m_fDelay = 0.f;

	_float					m_fAddY = 0.2f;
	_float					m_fDelayAddY = 0.21f;

	_float					m_fDeadDelay = 1.f;

	LPDIRECT3DVERTEXBUFFER9 m_pGuideVertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pGuideIndexBuffer = nullptr;
	_int					m_iGuideVertexCount;
	_int					m_iGuideTriCount;


	LPDIRECT3DVERTEXBUFFER9 m_pDelayVertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pDelayIndexBuffer = nullptr;
	_int					m_iDelayVertexCount;
	_int					m_iDelayTriCount;
	
};

END

#endif