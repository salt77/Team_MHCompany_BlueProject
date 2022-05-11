#ifndef __Renderer_H__
#define __Renderer_H__

#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CUI_Object;
class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	CRenderer();
	virtual ~CRenderer();

public:
	virtual void Free() override;
	virtual CComponent*		Clone() override { return nullptr; }


public:
	HRESULT					Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_Update(const _float& fTimeDelta);
	void					Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
			
	void					Add_RenderGroup(E_RENDER_ID eRenderID, CGameObject* pGameObject);
	void					Add_InstanceGroup(E_INSTANCE_RENDER_ID eInstanceRenderID, CGameObject* pGameObject);


	void					Clear_RenderGroup();
	void					Clear_Instance_RenderGroup();

	void					Set_DefualtMaskTexture(LPD3DXEFFECT pEffect, D3DXHANDLE hParameter);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	void					RenderOption(const _float & fTimeDelta);

	void					Set_RendererShader();// 쉐이더 준비
	void					Check_IfActiveTrue_Render(E_RENDER_ID eRenderID, _int iPass = 0);
	void					Check_IfActiveTrue_Instance_Render(E_INSTANCE_RENDER_ID eInstanceRenderID);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void					Set_Debug()										{ m_bDebug = !m_bDebug;						} // 콜라이더 디버그 옵션
	void					Set_Render_Defferd()							{ m_bRenderDeferred = !m_bRenderDeferred;	} // 디버그 랜더타겟
	void					Set_Render_Defferd(_bool bState)				{ m_bRenderDeferred = bState;				} // 디버그 랜더타겟

public:
	CShader*				Get_BlendShader()								{ return m_pBlendShader; }
	CShader*				Get_LightAccShader()							{ return m_pLightAccShader; }
	CShader*				Get_ShowBaseShader()							{ return m_pShowBaseShader; }


	// 랜더 옵션 - set
public:
	void					Set_SSAO(_bool bState)							{ m_bSSAO_OnOff = bState;					}
	void					Set_Antialiasing(_bool bState)					{ m_bAntialiasing = bState;					}
	void					Set_CutIn(_bool bState)							{ m_bIsCutIn = bState;						}
	void					Set_BaseGlow(_bool bGlow)						{ m_bBaseGlow = bGlow;						} // 기본 글로우
	void					Set_TexGlow(_bool bGlow)						{ m_bTexGlow = bGlow;						} // 텍스처 기반 글로우

	
	void					Set_RenderOption(T_RenderOption tRenderOption)	{ m_tRenderOption = tRenderOption;			} // HDR
	void					Reset_RenderOption()							{ m_tRenderOption = T_RenderOption();		} // HDR

	// 랜더 옵션 - get
public:
	_bool					Get_Debug()										{ return m_bDebug;							} // 콜라이더 디버그 옵션
	_bool					Get_SSAO()										{ return m_bSSAO_OnOff;						}
	_bool					Get_Antialiasing()								{ return m_bAntialiasing;					}
	_bool					Get_CutIn()										{ return m_bIsCutIn;						}
	_bool					Get_BaseGlow()									{ return m_bBaseGlow;						} // 기본 글로우
	_bool					Get_TexGlow()									{ return m_bTexGlow;						} // 텍스처 기반 글로우
	T_RenderOption&			Get_RenderOption()								{ return m_tRenderOption;					} // HDR


	void					Reverse_Debug();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	 

	// 뷰 컷
public:
	void					Set_ViewCut(T_ViewCut tViewCut) { m_tViewCut = tViewCut; }
	void					Set_ViewCut(_float fTime, _float fDif, _float fDelayTime = 0.f);
	void					Set_ViewCut_Matrix(_matrix matView, _matrix matProj);

	void					Set_ViewCut_End() { m_tViewCut = T_ViewCut(); }
	void					Reset_ViewCut() { m_tViewCut = T_ViewCut(); }

	T_ViewCut&				Get_ViewCut() { return m_tViewCut; }

	void					Update_ViewCut(const _float& fTimeDelta);

	void					Set_ViewCut_Fix(_float fTime, T_ViewCut tViewcutFix)	{ m_fTime_ViewCutFix = fTime; m_tViewCutFix = tViewcutFix;	}
	T_ViewCut&				Get_ViewCut_Fix()										{ return m_tViewCutFix;										}

	void					Set_ViewCut_Fix_Easy(_float fTime, _float fRatio, _float fDis);
	void					Set_ViewCut_Fix_Matrix(_matrix* pMatView, _matrix* pMatProg);
	
private:
	// 포워드 랜더----------------------------------------------------------------------------------
	void					Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);

	// 메인 디퍼드 랜더------------------------------------------------------------------------------
	void					Render_Deferred_Color(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_Shadow(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_LightAcc(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_Blend(LPDIRECT3DDEVICE9& pGraphicDev);

	void					Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev, _int iPass);
	void					Render_Instance_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	
	// 이펙트 디퍼드 랜더----------------------------------------------------------------------------
	void					Render_Effect(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_EffectObj(LPDIRECT3DDEVICE9& pGraphicDev);

	// 후처리 랜더-----------------------------------------------------------------------------------
	
	void					Render_Blur(LPDIRECT3DDEVICE9& pGraphicDev);

	void					Render_Light(LPDIRECT3DDEVICE9 & pGraphicDev);

	void					Render_PostProcess(LPDIRECT3DDEVICE9& pGraphicDev);

	// 포워드 랜더-----------------------------------------------------------------------------------
	void					Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_Instance_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);

	void					Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

	// 민우 - 편성창
	void					Render_Billboard(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_Instance_Billboard(LPDIRECT3DDEVICE9& pGraphicDev);
	void					Render_ShowWindow(LPDIRECT3DDEVICE9& pGraphicDev);

	// 랜더 타겟들을 디버그 합니다.
	void					Render_Debug();

	// 기본이 되는 상수테이블들을 미리 설정합니다.
	void					Set_Base_Constantable();
	void					Set_Fog();
	void					Reset_Fog();

	// ---------------------------------------------------------------------------------------------

private:
	list<CGameObject*>		m_RenderGroup[(_int)E_RENDER_ID::RENDER_END];
	map<E_OBJTAG, vector<CGameObject*>> m_mapInstanceGroup[(_int)E_INSTANCE_RENDER_ID::RENDER_INSTANCE_RENDER_END];
	
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	LPDIRECT3DVERTEXBUFFER9 m_pInstanceBuffer				= nullptr;

	CShader*				m_pBaseShader					= nullptr;
	CShader*				m_pBlendShader					= nullptr;
	CShader*				m_pLightAccShader				= nullptr;
	CShader*				m_pEffectShader					= nullptr;
	CShader*				m_pBlurShader					= nullptr;
	CShader*				m_pGodRayShader					= nullptr;
	CShader*				m_pPostProcessShader			= nullptr;
	CShader*				m_pViewCutShader				= nullptr;
	CShader*				m_pShowShadeShader				= nullptr;
	CShader*				m_pShowBlendShader				= nullptr;
	CShader*				m_pShowBaseShader				= nullptr;
	
	LPDIRECT3DTEXTURE9		m_pTexMaskDefault				= nullptr; // 디폴트 마스크 텍스처 (마스크 텍스처가 없는 객체들을 위함)	

	_int					m_iDefferdPassCount				= 0;
	
	// UI 클릭 판단용
	CUI_Object*				pDeepTouchUi_Down				= nullptr;
	CUI_Object*				pDeepTouchUi_Up					= nullptr;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 랜더 옵션 용도

	_bool					m_bRenderDeferred				= FALSE;
	_bool					m_bRenderDeferredAlpha			= FALSE;
	_bool					m_bDebug						= FALSE;

	_bool					m_bSSAO_OnOff					= FALSE;
	_bool					m_bAntialiasing					= FALSE;
	_bool					m_bIsCutIn						= FALSE;

	_bool					m_bTest_For_PostProcess			= FALSE;
	_float					m_fTest_For_PrstPrecess_Ratio_X = 0.f;

	_bool					m_bBaseGlow						= TRUE;
	_bool					m_bTexGlow						= TRUE;
	_bool					m_bGodRay						= TRUE;
	
	T_RenderOption			m_tRenderOption					= T_RenderOption();	   

	// 1.f 에 가까울수록 뽀얍니다
	_float					m_fHDR_FixColor					= 0.f;

	_bool					m_bRenderDebug					= FALSE;
	_float					m_fRenderDebug					= 0.f;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 뷰 컷

	_bool					m_bViewCut = FALSE;
	_bool					m_bViewCut_Fix = FALSE;
	T_ViewCut				m_tViewCut = T_ViewCut();
	T_ViewCut				m_tViewCutFix = T_ViewCut();
	_float					m_fTime_ViewCutFix = 0.f;
	

};

END
#endif // __Renderer_H__