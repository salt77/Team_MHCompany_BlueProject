#pragma once

#include "GameObject.h"

// 레이캐스트 총알이 제대로 된 충돌이 되는지 확인하는 용도의 라인 (임시)
class CRay_Line final : public CGameObject
{
private:
	explicit CRay_Line(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	explicit CRay_Line(const CRay_Line& rhs);
	virtual ~CRay_Line();

public:
	static	CRay_Line*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void		Free() override; 

public:
	virtual HRESULT		Ready_Object();
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;

public:
	void				Create_Line(_vec3*	vPoint);

private:
	void				Add_Component();
	virtual void		Set_Constantable() override;

private:
	_vec3				m_vPoint[2] = { VEC_ZERO, VEC_ZERO };

	LPD3DXLINE			m_pLine = nullptr;
	
	CShader*			m_pShaderCom = nullptr;
};

