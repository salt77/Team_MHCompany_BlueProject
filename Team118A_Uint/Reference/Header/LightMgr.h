#pragma once
#ifndef __LightMgr_H__
#define __LightMgr_H__

#include "Light.h"
#include "RectBuffer.h"
#include "Shader.h"
#include "Camera.h"

//////////////////////////////
// ������ : ����ȣ
//////////////////////////////

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	CLightMgr();
	virtual ~CLightMgr();
	virtual	void Free() override;

public:
	HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void				Render_Light(LPD3DXEFFECT& pEffect);

	void				Update_Light_Mgr(const _float & fTimeDelta);
	void				Render_Light_Mgr();
	
public:
	D3DLIGHT9*			Get_Light(const _uint& iIndex = 0);

public:
	//////////////////////////////////////////////////////////////////////
	// �� ����

	// ���� �ϳ��� �����ϴ°�?
	_bool				IsLightExist();

#pragma region �� �ɼ� ����
	void				Set_LightTarget(_int iIndex);

	_vec3				Get_LightPos();
	void				Set_LightPos(_vec3 vPos);

	_vec3				Get_LightDir();
	void				Set_LightDir(_vec3 vDir);

	D3DLIGHTTYPE		Get_LightType();
	void				Set_LightType(D3DLIGHTTYPE eLightType);

	D3DXCOLOR			Get_LightDiffuse();
	void				Set_LightDiffuse(D3DXCOLOR Diffuse);

	D3DXCOLOR			Get_LightSpecular();
	void				Set_LightSpecular(D3DXCOLOR Specular);

	D3DXCOLOR			Get_LightAmbient();
	void				Set_LightAmbient(D3DXCOLOR Ambient);

	D3DXCOLOR			Get_LightBackBufferColor();
	void				Set_LightBackBufferColor(D3DXCOLOR BackBufferColor);
#pragma endregion

	//////////////////////////////////////////////////////////////////////

#pragma region �� ���� �ɼ�

	_vec3				Get_LightTargetPos(_float fDis);
	void				Set_LightTargetPos(_vec3 vPos, _float fDis);

	void				Set_LightMove(_bool bLightMove) { m_bLightMove = bLightMove; }
	_bool				Get_LightMove() { return m_bLightMove; }

	void				Set_LightDis(_float fDis);
	void				Add_LightDis(_float fDis);
	_float				Get_LightDis() { return m_fLightCameraDis; }

private:
	void				Apply_LightDis(_float fDis);
#pragma endregion

#pragma region �� ����Ʈ �ɼ�

public:
	// ������� �� ����Ʈ�� �ε����� �����´�.
	_int				Create_GodLight_New(_vec3 vPos, _vec4 vColor, _float fSize);

	void				Set_GodLight_Index_Main(_int iIndex);
	_int				Get_GodLight_Index_Main() { return m_iGodLightTarget; }

	// ���� ���� ȣ���ϸ� Ÿ���� ������ ������
	void				Set_GodLight_Info(_int iTargetIndex, T_GodLight tInfo);
	T_GodLight*			Get_GodLight_Info(_int iTargetIndex = -1);

	void				Reset_GodLight_List();
	void				Check_GodLight_Main();

#pragma endregion
	
	
public:
	//////////////////////////////////////////////////////////////////////
	// �׸��� ���� �ɼ�
	// �� ���� �� ��İ� ���� ���

	_matrix				Get_LightView(const _uint& iIndex = 0);
	_matrix				Get_LightProg();

	//////////////////////////////////////////////////////////////////////

private:
	list<CLight*>		m_LightList;

	_int				m_iLightTarget = 0;
	D3DXCOLOR			m_vBackBufferColor = D3DXCOLOR(0.53f, 0.88f, 0.98f, 1.f);

	_bool				m_bLightMove = TRUE;
	//_bool				m_bLightMove = FALSE;

	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	CRectBuffer*		m_pBuffer = nullptr;

	LPDIRECT3DTEXTURE9	m_pDebugTexture = nullptr;
	LPDIRECT3DTEXTURE9	m_pLightTexture = nullptr;
	CShader*			m_pShader = nullptr;

	_vec3				m_pLightCameraPos;
	_float				m_fLightCameraDis = 0.f;
	_vec3				m_pLightCameraPos_Ori;
	_vec3				m_pLightCameraPos_At = _vec3(0.f, 0.f, 0.f);



	_matrix				m_matTransWolrd;

	// �� ����Ʈ�� �ϳ��� Ÿ������ ������ �� �ִ�.
	_int				m_iGodLightTarget = 0;
	list<T_GodLight>	m_listGodLight;	
};


END
#endif // __LightMgr_H__
