#pragma once

#ifndef __EffectObject_H__
#define __EffectObject_H__

#include "GameObject.h"

#include "EMesh.h"
#include "Shader.h"
#include "RectBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CEffectObject:
	public CGameObject
{
private:
	CEffectObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot);
	CEffectObject(const CEffectObject& rhs);
	virtual ~CEffectObject();

public:
	static CEffectObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag , T_EffectInfo tEffectInfo);
	static CEffectObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot, wstring wstrEffectTag); 
	static CEffectObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, _vec3 vPos, _vec3 vRot , wstring wstrEffectTag, _bool bQuatRot);
	virtual void			Free()override;
	static void				Create_Effect_RandAngle(LPDIRECT3DDEVICE9 pGraphicDev, _int iCount, _vec3 vPos, _vec3 vAngle, wstring wstrName);

public:	
	virtual HRESULT			Ready_Object(T_EffectInfo tEffectInfo);
	virtual HRESULT			Ready_Object(wstring wstrEffectTag);
	virtual HRESULT			Ready_Object(wstring wstrEffectTag, _bool bQuatRot); 

	virtual _int			Start_Object(const _float& fTimeDelta) override;
	virtual	_int			Update_Object(const _float& fTimeDelta) override;
	virtual	_int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void			Render_Object(_int iPass = 0) override;
	
public:
	void					Set_Effect_Scale(_vec3 vScale)				{ m_vScale = vScale;				}
	void					Set_Effect_Pos(_vec3 vPos)					{ m_vPos = vPos;					}
	void					Set_Effect_Parent(_vec3 vPos, _vec3 vRot)	{ m_vPos = vPos; m_vRot = vRot;		}
	void					Add_Effect_Parent(_vec3 vPos, _vec3 vRot)	{ m_vPos += vPos; m_vRot += vRot;	}
	void					Set_Effect_Rotation(_vec3 vRot)				{ m_vRot = vRot;					}
	void					Set_ShowWindow(_bool bShowWindow)			{ m_bShowWindow = bShowWindow;      }

	void					Add_Effect_Rotation(_vec3 vRot)				{ m_vRot += vRot;					}
	void					Add_Effect_Rotation(const E_ROTATION& eRotationType, const _float& fAngle);

	T_EffectInfo&			Get_Effect_Info()							{ return m_tEffectInfo;				}
	_vec3					Get_Effect_Scale()							{ return m_vScale;					}
	_vec3					Get_Effect_Pos()							{ return m_vPos;					}
	_vec3					Get_Effect_Rotation()						{ return m_vRot;					}

private:
	void					Add_Component();
	virtual void			Set_Constantable()override;
	virtual void			Debug_This_In_Console();

	_float					Saturate_Float(_float& fVar);
	void					PrefabLoad(wstring wstrPrefabName);
	virtual void			Save(HANDLE hFile, DWORD& dwByte);
	virtual void			Load(HANDLE hFile, DWORD& dwByte);

	void					Set_Ratio_EffectType(); 
	void					Set_Axis(const E_ROTATION& eRotationType, const _vec3& vDir); 
	
private:
	_bool					UpdateTime(const _float& fTimeDelta);

private:
	T_EffectInfo			m_tEffectInfo;

	CShader*				m_pShader;

	_bool					m_bMesh = false;

	CRectBuffer*			m_pBuffer			= nullptr;
	LPDIRECT3DTEXTURE9		m_pTexture			= nullptr;
	LPDIRECT3DTEXTURE9		m_pTexture_Filter_1	= nullptr;
	LPDIRECT3DTEXTURE9		m_pTexture_Filter_2	= nullptr;
	LPDIRECT3DTEXTURE9		m_pTexture_Filter_3	= nullptr;

	CEMesh*					m_pEMesh;
		
	_float					m_fTimeAcc;
	_float					m_fRatio = 0.f;
	_int					m_iSpriteIndex = 0;
	_float					m_fSpriteTime = 0.f;
	_bool					m_bBillboard = FALSE;

	_vec3					m_vPos		= _vec3(0.f, 0.f, 0.f);
	_vec3					m_vRot		= _vec3(0.f, 0.f, 0.f);
	_vec3					m_vScale	= _vec3(1.f, 1.f, 1.f);
	_matrix					m_matParent;

	_bool					m_bQuatRot = FALSE; 
	_vec3					m_vAxis[(_uint)E_ROTATION::ROT_END] = { VEC_ZERO, VEC_ZERO, VEC_ZERO };
	_quat					m_quatRot = _quat(0.f, 0.f, 0.f, 1.f); 

	_matrix					m_matRot; 
	_matrix					m_matWorld; 

	_bool					m_bShowWindow = FALSE;
};

END

#endif // !__EffectObject_H__