#pragma once
#ifndef __MFC_LINE_H__
#define __MFC_LINE_H__
#include "GameObject.h"

class CMFC_Line : public CGameObject
{
private:
	explicit CMFC_Line(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CMFC_Line(const CMFC_Line& rhs);
	virtual ~CMFC_Line();

public:
	static	CMFC_Line*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free() override; 

public:
	virtual HRESULT		Ready_Object();
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(_int iPass) override;

private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

public:
	void				Emplace_Line(_vec3	vPoint);
	void				Clear_VecPoint() { m_vecPoint.clear(); }

private:
	LPD3DXLINE			m_pLine = nullptr;

	vector<_vec3>		m_vecPoint;
	
	CShader*			m_pShaderCom = nullptr;
};

#endif // !__MFC_LINE_H__