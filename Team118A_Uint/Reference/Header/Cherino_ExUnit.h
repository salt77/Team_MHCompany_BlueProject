#pragma once
#ifndef __CHERINO_EXUNIT_H__
#define __CHERINO_EXUNIT_H__

#include "GameObject.h"
#include "DMesh.h"

BEGIN(Engine)

class ENGINE_DLL CCherino_ExUnit : public CGameObject
{
protected:
	explicit CCherino_ExUnit(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName);
	explicit CCherino_ExUnit(const CCherino_ExUnit& rhs);
	virtual ~CCherino_ExUnit();

public:
	virtual void	Free()											override;

	static	CCherino_ExUnit*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
public:		/* ±âº»		*/
	virtual	HRESULT	Ready_Object()									override;
	virtual _int	Start_Object(const _float& fTimeDelta)			override;
	virtual	_int	Update_Object(const _float& fTimeDelta)			override;
	virtual	_int	LateUpdate_Object(const _float& fTimeDelta)		override;
	virtual	void	Render_Object(_int iPass)						override;
	virtual void	Debug_This_In_Console()							override;
	virtual void	Set_Constantable()								override;


protected:
	CDMesh*				m_pDMeshCom		= nullptr;
	CShader*			m_pShaderCom	= nullptr;

	wstring				m_wstrUnitName = L"CherinoRoyalGuard";
};

END

#endif // !__CHERINO_EXUNIT_H__