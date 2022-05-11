#pragma once
#ifndef __Hiero_R_Lantern_H__
#define __Hiero_R_Lantern_H__

#include "GameObject.h"

USING(Engine)

class ENGINE_DLL CHiero_R_Lantern : public CGameObject
{
protected:
	explicit CHiero_R_Lantern(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual ~CHiero_R_Lantern();

public:
	static CHiero_R_Lantern*		Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag);
	virtual void					Free();

public:
	virtual HRESULT					Ready_Object() override;
	virtual _int					Update_Object(const _float& fTimeDelta) override;
	virtual _int					LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void					Render_Object(_int iPass) override;

public:
	void							Set_ActiveLantern(const _bool& bState);

public:
	bool							Get_ActiveLantern() { return m_bLantern_Active; };

private:
	void							Add_Component();
	virtual void					Set_Constantable() override;


private:
	CSMesh*							m_pSMeshCom				= nullptr;
	CShader*						m_pShaderCom			= nullptr;

private:
	wstring							m_wstrMeshFullPath		= L"../../Reference/Resource/Monster/Hieronymus/HolyRelic_Red/";
	wstring							m_wstrMeshName			= L"HolyRelic_Red.X";

private:
	_bool							m_bLantern_Active		= FALSE;
	_float							m_fActiveTime			= 0.f;
	_float							m_fRadius				= 4.f;
};

#endif // !__Hiero_R_Lantern_H__