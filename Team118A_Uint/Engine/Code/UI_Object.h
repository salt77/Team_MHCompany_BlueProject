#pragma once
#ifndef __UI_OBJECT_H__
#define __UI_OBJECT_H__

#include "GameObject.h"


BEGIN(Engine)

class CRectBuffer;
class CTexture;

class ENGINE_DLL CUI_Object : public CGameObject
{
public:
	void		bTest(_bool basd);
	void		iTest(_int iTest);
	void		fTest(_float fasd);

protected:
	explicit CUI_Object(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	explicit CUI_Object(const CGameObject& rhs);
	virtual ~CUI_Object();

public:
	static CUI_Object*	Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag);
	virtual void		Free() override;
	virtual void		Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void		Load(HANDLE hFile, DWORD& dwByte) override;
	virtual CUI_Object*	Clone();

public:
	virtual	HRESULT		Ready_Object() override;
	virtual _int		Start_Object(const _float& fTimeDelta) override;
	virtual	_int		Update_Object(const _float& fTimeDelta) override;
	virtual	_int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual	void		Render_Object(_int iPass) override;
	virtual	void		Child_Render_Object();

	void				Render_Font();

public:
	void				Update_UiTransform();
	void				Update_MovingQueue(const _float& fTimeDelta);
	void				Set_Child(CUI_Object* pUi);
	void				Set_Ui_TextColor(_vec4 vColor)						{ memcpy(&m_tTextOption.tColor, &vColor, sizeof(_vec4));	}
	void				Set_Ui_Text(const wstring wText);
	void				Add_Ui_Text(const wstring wText);
	void				Set_Ui_FontType(E_FontType eType)					{ m_tTextOption.eFontType = eType;					}
	void				Set_Ui_FontSize(const _int iFontSize)				{ m_tTextOption.iFontSize = iFontSize;						}
	void				Set_Ui_FontBold(_bool bState)						{ m_tTextOption.bBold = bState; }

	void				Set_Ui_TransformPivot(const E_Pivot ePivot)			{ m_tUiTransform.eTransformPivot = ePivot;					}
	void				Set_Ui_Anchor(const E_Anchor eAnchor);
	void				Set_Ui_TextVertexOfAngle(const E_VertexOfAngle eVoa){ m_tTextOption.eTextVertexOfAngle = eVoa;					}

	void				Set_Ui_Pos(const _vec2& vPos)						{ m_tUiTransform.iPosX = (_int)vPos.x; m_tUiTransform.iPosY = (_int)vPos.y; }
	void				Set_Ui_PosX(const _int iPosX)						{ m_tUiTransform.iPosX		= iPosX;						}
	void				Set_Ui_PosY(const _int iPosY)						{ m_tUiTransform.iPosY		= iPosY;						}
	void				Set_Ui_TextPosX(const _int iTextPosX)				{ m_tTextOption.iTextPosX	= iTextPosX;					}
	void				Set_Ui_TextPosY(const _int iTextPosY)				{ m_tTextOption.iTextPosY	= iTextPosY;					}
	void				Set_Ui_SizeX(const _int iSizeX)						{ m_tUiTransform.iSizeX		= iSizeX;						}
	void				Set_Ui_SizeY(const _int iSizeY)						{ m_tUiTransform.iSizeY		= iSizeY;						}
	void				Add_Ui_PosX(const _int iAddPosX)					{ m_tUiTransform.iPosX		+= iAddPosX;					}
	void				Add_Ui_PosY(const _int iAddPosY)					{ m_tUiTransform.iPosY		+= iAddPosY;					}
	void				Add_Ui_TextPosX(const _int iAddTextPosX)			{ m_tTextOption.iTextPosX	+= iAddTextPosX;				}
	void				Add_Ui_TextPosY(const _int iAddTextPosY)			{ m_tTextOption.iTextPosY	+= iAddTextPosY;				}
	void				Add_Ui_SizeX(const _int iAddSizeX)					{ m_tUiTransform.iSizeX		+= iAddSizeX;					}
	void				Add_Ui_SizeY(const _int iAddSizeY)					{ m_tUiTransform.iSizeY		+= iAddSizeY;					}
	void				Set_Sorting_Layer(const _int iLayerIndex)			{ m_tUiTransform.iSorting_Layer = iLayerIndex;				}
	void				Set_Order_In_Layer(const _int iLayerIndex)			{ m_tUiTransform.iOrder_In_Layer = iLayerIndex;				}
	void				Add_EventQueue(const _int iEventQueue)				{ m_vecClickEvent.push_back(iEventQueue);					}
	void				Set_ButtonEventType(E_UiButtonEventType eType)		{ m_tUiTransform.iButtonEventType = (_int)eType;			}
	void				Clear_EventQueue()									{ m_vecClickEvent.clear();									}
	void				Clear_Child()										{ m_listChild.clear();										}
	void				Set_ImageAlpha(const _float fRatio)					{ m_ColorShaderOption.a = fRatio;							}
	void				Set_AnimStart(const _float fDir);
	void				Set_Ease(E_EASE eEase)								{ m_tUiTransform.eEase = eEase;								}
	void				Set_UV(E_MinMax eMinMax, E_POSITION ePos, _float fValue);
	void				Set_ProgMax(const _float fMax)						{ m_tUiAnimation.fProgMax = fMax;							}
	void				Set_float_Parameter_1(const _float fParam)			{ m_tUiAnimation.float_Parameter_1	= fParam;				}
	void				Set_float_Parameter_2(const _float fParam)			{ m_tUiAnimation.float_Parameter_2	= fParam;				}
	void				Set_int_Parameter_1(const _int iParam)				{ m_tUiAnimation.int_Parameter_1	= iParam;				}
	void				Set_int_Parameter_2(const _int iParam)				{ m_tUiAnimation.int_Parameter_2	= iParam;				}
	void				Set_AnimType(E_UiAnimationType eAnimType)			{ m_tUiAnimation.eAnimType			= eAnimType;			}
	void				Set_ChildTo(_bool bState)							{ m_tUiAnimation.bMyChildFollowMyAnim = bState;				}
	void				Set_ImageColor(D3DXCOLOR color)						{ m_ColorShaderOption = color;								}
	void				Set_ChildClickCheck(_bool bState)					{ m_bChildClickCheck = bState;								}
	void				Set_NameTag(wstring wstrName)						{ m_wstrNameTag = wstrName;									}
	void				Set_ClickAnimationOnOff(_bool bState)				{ m_bOnClickAnimation = bState;								}
	void				Set_OnEnableAnimationOnOff(_bool bState)			{ m_bOnEnableAnimation = bState;							}
	void				Set_Mask(_vec2 vMin, _vec2 vMax);
	void				Set_UITag(wstring wstrUITag)						{ m_wstrUITag = wstrUITag;									}
	void				Set_MaskOnOff(_bool bState)							{ m_bMask = bState;											}
	void				Set_Italic(_bool bState)							{ m_bItalic = bState;										}
	void				Set_Tex1_Tag(wstring wstrTag);
	void				Set_Tex2_Tag(wstring wstrTag);
	void				Set_TextAlign(E_TextAlign eAlign)					{ m_iTextAlign = (_int)eAlign;								}
	void				Set_Angle(_float fAngle)							{ m_fAngle = fAngle;										}
	void				Add_UiMoving(T_UiMoving tMoving);
	void				Set_AfterPass(_bool bState)							{ m_bAfterPass = bState;									}
	void				Set_ResetMask(_bool bState)							{ m_bResetMask = bState;									}
	void				Set_SortingOrder(_bool bUp);
	void				Set_FadeInOut(E_FADETYPE eFadeType, _float fFadeTime); 
	void				Set_TextAlpha(const _float& fTextAlpha)				{ m_tTextOption.tColor.a = fTextAlpha;						}

	void						Get_Tex1_FromTextureMgr();
	void						Get_Tex2_FromTextureMgr();
	const wstring				Get_Tex1_ProtoTag() { return m_wstrTexProtoTag_1; }
	CUI_Object*					Get_EventObject(vector<_int>& vecEvent, _int& iMaxDepth, _int iDeep);
	virtual const wstring		Get_Name() { return m_wstrNameTag; }
	const wstring&				Get_Ui_Text() { return m_wText; }
public:
	void						Get_ParentPos(_vec2& vPos);
	_vec2						Get_Ui_Pos()								{ return _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY); }
	CUI_Object*					Get_Parent()								{ return m_pParent;											}
	const T_TextOption			Get_TextOption()	const					{ return m_tTextOption;										}
	const T_UiTransform			Get_UiTransform()	const					{ return m_tUiTransform;									}
	const T_UiAnimation			Get_UiAnimation()	const					{ return m_tUiAnimation;									}
	const list<CUI_Object*>*	Get_ChildList()								{ return &m_listChild;										}
	list<CUI_Object*>*			Get_ChildList_NotConst()					{ return &m_listChild;										}
	CUI_Object*					Get_ChildByIndex(_int iIndex);
	const wstring&				Get_Text()									{ return m_wText;											}
	const _vec2					Get_PivotAddPos(E_Pivot ePivot);
	const _int					Get_MyVoa(E_DIR_ANCHOR eDir);
	const _int					Get_ParentVoa(E_DIR_ANCHOR eDir);
	const _vec2					Get_MyPivotAddPos();
	const HWND*					Get_Hwnd()									{ return m_pHwnd;											}
	const vector<_int>*			Get_vecClickEvent()							{ return  &m_vecClickEvent;									}
	_float						Get_ImageAlpha()							{ return m_tUiTransform.fImageAlpha;						}
	_int						Get_ButtonEventType()						{ return m_tUiTransform.iButtonEventType;					}
	_int						Get_Ease()									{ return (_int)m_tUiTransform.eEase;						}
	_bool						Get_OnOff()									{ return m_bOnOff;											}
	D3DXCOLOR					Get_ImageColor()							{ return m_ColorShaderOption;								}
	_bool						Get_ChildClickCheck()						{ return m_bChildClickCheck;								}
	virtual _int				Get_SortingLayer() override					{ return m_tUiTransform.iSorting_Layer;						}
	virtual _int				Get_OrderInLayer() override					{ return m_tUiTransform.iOrder_In_Layer;					}
	_bool						Get_ClickAnimationOnOff()					{ return m_bOnClickAnimation;								}
	_bool						Get_OnEnableAnimationOnOff()				{ return m_bOnEnableAnimation;								}
	list<vector<_int>>*			Get_EventVecList()							{ return &m_listClickEvent;									}
	list<E_UiButtonEventType>*	Get_EventTypeList()							{ return &m_listButtonEventType;							}
	wstring						Get_UITag()									{ return m_wstrUITag;										}
	void						Get_Mask(_vec2* vOutMin, _vec2* vOutMax)	{ *vOutMin = m_vMaskRatio_Min; *vOutMax = m_vMaskRatio_Max;	}
	_bool						Get_MaskOnOff()								{ return m_bMask;											}
	LPDIRECT3DTEXTURE9			Get_Texture_1()								{ return m_pTexture_1;										}
	_bool						Get_Italic()								{ return m_bItalic;											}
	_int						Get_TextAlign()								{ return m_iTextAlign;										}
	_bool						Get_IsScrollViewChild();
	const queue<T_UiMoving>&	Get_UiMovingQueue()							{ return m_qMoving;											}

public:
	void						Push_ClickEvent();
	void						Follow_ParentMaskUV();
	void						Add_Function_void(E_UiFunctionType eType, std::function<void(void)> Func);
	void						Add_Function_int(E_UiFunctionType eType, std::function<void(_int)> Func, _int iArg);
	void						Add_Function_float(E_UiFunctionType eType, std::function<void(_float)> Func, _float fArg);
	void						Add_Function_wstring(E_UiFunctionType eType, std::function<void(wstring)> Func, wstring wstrArg);
	void						Add_Function_voidPtr(E_UiFunctionType eType, std::function<void(void*)> Func, void* pArg);
	void						Clear_Function(E_UiFunctionType eType);
	void						Active_Function(E_UiFunctionType eType);

public:
	virtual void		OnEnable();
	virtual void		OnDisable();
	virtual _bool		Is_OnMouse(POINT& ptMouse);
	virtual _bool		Is_OnMouse(POINT& ptMouse, CUI_Object** ppOutDeepObject, _int* refMostDeep = nullptr, _int iDepth = 0);
	virtual void		OnMouse_ThisFrame();
	virtual void		OnClick();
	virtual void		OnTrigger();
	virtual void		Animation(const _float& fTimeDelta);

protected:
	void				Add_Component();
	void				Set_ConstantTable();
	virtual void		Debug_This_In_Console() override;
	void				FadeInOut(const _float& fTimeDelta);

	_vec2				Get_VertexOfAngle(E_VertexOfAngle eVoa);

protected:
	CUI_Object*			m_pParent			= nullptr;
	list<CUI_Object*>	m_listChild;

	CShader*			m_pShaderCom		= nullptr;
	CRectBuffer*		m_pRectBufferCom	= nullptr;

	wstring				m_wstrTexProtoTag_1 = L"";
	LPDIRECT3DTEXTURE9	m_pTexture_1		= nullptr;
	wstring				m_wstrTexProtoTag_2 = L"";
	LPDIRECT3DTEXTURE9	m_pTexture_2		= nullptr;
	wstring				m_wstrNameTag		= L"이름 없음";
	wstring				m_wstrUITag			= L"";

protected:
	T_UiTransform			m_tUiTransform;
	T_UiAnimation			m_tUiAnimation;
	T_TextOption			m_tTextOption;
	D3DXCOLOR				m_ColorShaderOption = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	vector<_int>				m_vecClickEvent;
	list<vector<_int>>			m_listClickEvent;
	list<E_UiButtonEventType>	m_listButtonEventType;

	wstring					m_wText;
	_vec2					m_vAnimBeginPos				= VEC_ZERO;
	_vec2					m_vAnimEndPos				= VEC_ZERO;
	_int					m_iTextAlign				= (_int)E_TextAlign::Left;
	_float					m_fAnimationDir				= 1.f;
	_bool					m_bClicked					= FALSE;
	_bool					m_bOnOff					= FALSE;				// OnOff 버튼을 위한 옵션
	_bool					m_bMouseOn					= FALSE;				// 마우스가 위에있는지 판단
	_bool					m_bChildClickCheck			= FALSE;				// 자식의 충돌검사를 할지 말지
	_bool					m_bOnClickAnimation			= TRUE;
	_bool					m_bOnEnableAnimation		= FALSE;
	_bool					m_bMask						= TRUE;
	_bool					m_bEnableThisFrame			= FALSE;
	_bool					m_bItalic					= FALSE;
	_bool					m_bIsScrollView				= FALSE;
	_bool					m_bAfterPass				= FALSE;
	_bool					m_bResetMask				= TRUE;
	_bool					m_bFadeInOut				= FALSE;

	_vec2					m_vMaskRatio_Min = _vec2(0.0f, 0.0f);
	_vec2					m_vMaskRatio_Max = _vec2(1.0f, 1.0f);
	_vec2					m_vUIMoveTarget				= VEC2_ZERO; 
	_vec2					m_vUIOriginPosition			= VEC2_ZERO; 
	_float					m_fAngle					= 0.f;
	_float					m_fFadeCountTime		    = 0.f;
	_float					m_fFadeEndTime				= 0.f; 

	E_FADETYPE				m_eFadeType					= E_FADETYPE::FADETYPE_END;

	queue<T_UiMoving>		m_qMoving;

protected:	// 함수 포인터 리스트
	list<std::function<void(void)>>						m_Funclist_void[(_int)E_UiFunctionType::UiFunctionTypeEnd];
	list<pair<_int, std::function<void(_int)>>>			m_Funclist_int[(_int)E_UiFunctionType::UiFunctionTypeEnd];
	list<pair<_float, std::function<void(_float)>>>		m_Funclist_float[(_int)E_UiFunctionType::UiFunctionTypeEnd];
	list<pair<wstring, std::function<void(wstring)>>>	m_Funclist_wstring[(_int)E_UiFunctionType::UiFunctionTypeEnd];
	list<pair<void*, std::function<void(void*)>>>		m_Funclist_voidPtr[(_int)E_UiFunctionType::UiFunctionTypeEnd];

protected:
	HWND*				m_pHwnd = nullptr;
};

END
#endif // !__UI_OBJECT_H__