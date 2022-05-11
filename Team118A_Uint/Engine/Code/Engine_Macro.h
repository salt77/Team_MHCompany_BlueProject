#ifndef __Engine_Macro_H__
#define __Engine_Macro_H__

namespace Engine
{

	#define		PlayFab_TitleID				("66AAF")
	#define		StrNone						("None")
	#define		PlayFab_ID_Minimum			(4)
	#define		PlayFab_Password_Minimum	(6)
	#define		PlayFab_NickName_Minimum	(3)
	
	#define		PlayFab_ID_Maximum			(15)
	#define		PlayFab_Password_Maximum	(100)
	#define		PlayFab_NickName_Maximum	(25)


	#define		EnumToChar(x) #x
	#define		Management CManagement::GetInstance()->
	#define		WINCX (1280)
	#define		WINCY (720)

	#define VEC2_ZERO			D3DXVECTOR2(0.f, 0.f)

	#define VEC_ZERO			D3DXVECTOR3(0.f, 0.f, 0.f)
	#define VEC_RIGHT			D3DXVECTOR3(1.f, 0.f, 0.f)
	#define VEC_UP				D3DXVECTOR3(0.f, 1.f, 0.f)
	#define VEC_LOOK			D3DXVECTOR3(0.f, 0.f, 1.f)
	#define VEC_LEFT			D3DXVECTOR3(-1.f,0.f,0.f)
	#define VEC_DOWN			D3DXVECTOR3(0.f,-1.f,0.f)
	#define VEC_BACK			D3DXVECTOR3(0.f,0.f,-1.f)
	#define VEC_ONE				D3DXVECTOR3(1.f, 1.f, 1.f)

	#define C_White				(_vec4(1.f, 1.f, 1.f, 1.f))
	#define C_Red				(_vec4(1.f, 0.f, 0.f, 1.f))
	#define C_Green				(_vec4(0.f, 1.f, 0.f, 1.f))
	#define C_Blue				(_vec4(0.f, 0.f, 1.f, 1.f))
	#define C_Yellow			(_vec4(1.f, 1.f, 0.f, 1.f))
	#define C_Purple			(_vec4(0.545f, 0.f, 1.f, 1.f))
	#define C_LightBlue			(_vec4(0.342f, 0.752f, 1.f, 1.f))

	#define CAM_DEFAULT			D3DXVECTOR3(-2.4f, 2.6f, -2.4f)

	#define COLOR_EXPLOSION     (D3DXCOLOR(0.572549f,	0.f,		0.031372f,	1.f))
	#define COLOR_PIERCE		(D3DXCOLOR(0.741176f,	0.533333f,	0.003921f,	1.f))
	#define COLOR_MYSTERY		(D3DXCOLOR(0.133333f,	0.435294f,	0.607843f,	1.f))
	
	#define RETURN_ACTIVE_FALSE	(1)
	#define RETURN_DEAD			(-3)
	#define	RETURN_WARINING		(-2)
	#define RETURN_ERROR		(-1)
	#define RETURN_OK			(0)
	#define OBJTAG_NULL			(-1)
	#define OBJTAG_UI			(10)

	#define TRIGGER_DEFAULT		(-1)
	#define	ANIM_ERROR			(-1)
	#define	Range_Cor			(0.007f)

	#define	PI					(3.141592f)
	#define GR					(9.8f)
	#define Deg2Rad				(0.0174533f)
	#define Rad2Deg				(57.2958f)

	// 컴포넌트 태그 Define
	#define	ComTag_Transform	(L"Com_Transform")
	#define ComTag_RectBuffer	(L"Com_RectBuffer")
	#define ComTag_RectTest		(L"Com_RectTest")
	#define	ComTag_Shader		(L"Com_Shader")
	#define ComTag_Renderer		(L"Com_Renderer")
	#define ComTag_Collider		(L"Com_Collider")
	#define ComTag_SMesh		(L"Com_SMesh")
	#define ComTag_DMesh		(L"Com_DMesh")

	
	#define	SoundEmpty			(L"Empty")

	// 쉐이더 패스 Define
	#define ShaderPass(eShaderPass,i) { (_uint)eShaderPass * 2 + (_uint)i } // 랜더타겟_1
	//#define ShaderPass(eShaderPass,i) { (_uint)eShaderPass * 3 + (_uint)i } // 랜더타겟_2
	#define ShaderPass_RunMap	(ShaderPass(E_ShaderPass::RunMap,0))

	#define D3DCOLORTOCOLORREF( color ) (COLORREF)(((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16)     )
	#define GetRGB255_TO_RGB1f(RGB255) ((_float)RGB255 / 255.f)
	// 색상 조절용 float 자리수 변환
	#define	SetFloatColor(floatColor) ((float)((int)(floatColor * 100.f)) * 0.01f)

	// MFC BOOL 자료형 _bool 형변환
	#define BOOL2bool(bTrue) (bTrue ? TRUE : FALSE)

	// 아이템 가격 등 
	#define ItemPrice_Tier1	300
	#define ItemPrice_Tier2	500
	#define ItemPrice_Tier3	800	
	#define ItemPrice_Tier4	1300

	#define CharExpInc		125

	#define Dialog_CharName		_vec2(-500.f, 150.f)
	#define Dialog_CharCircle	_vec2(-370.f, 155.f)
	#define Dialog_Text			_vec2(-500.f, 225.f)
	#define Dialog_MiddleUp		_vec2(-200.f, -100.f) 
	#define Dialog_Middle		_vec2(-200.f, 0.f)
	#define Dialog_MiddleDown	_vec2(-200.f, 100.f) 
	#define Dialog_CharCenter	_vec2(0.f, 0.f)
	#define Dialog_CharLeftMid	_vec2(-125.f, 0.f)
	#define Dialog_CharRightMid _vec2(125.f, 0.f)
	#define Dialog_CharLeft		_vec2(-250.f, 0.f)
	#define Dialog_CharRight	_vec2(250.f, 0.f)
	#define Dialog_CharLeftOut	_vec2(-1000.f, 0.f)
	#define Dialog_CharRightOut _vec2(1000.f, 0.f)
	#define Dialog_Emoticon		_vec2(-110.f, 150.f) 

	#define Tenth		0.1f
	#define Half		0.5f
	#define Hundredth	0.01f

	#ifndef			MSG_BOX
	#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
	#define			LMSG_BOX(_message)			MessageBox(NULL, (_message), L"System Message", MB_OK)
	#endif

	#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
	#define			END						}

	#define			USING(NAMESPACE)	using namespace NAMESPACE;

	#ifdef	ENGINE_EXPORTS
	#define ENGINE_DLL		_declspec(dllexport)
	#else
	#define ENGINE_DLL		_declspec(dllimport)
	#endif

	#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){ return;}}

	#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){ return _return;}}

	#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK); assert(0);}}

	#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK); assert(0);return _return;}}

	#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); assert(0);	return E_FAIL;}

	#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK);  assert(0);	return _return;}

	#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); assert(0);	return E_FAIL;}

	#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); assert(0);	return _return;}

	#define NO_COPY(CLASSNAME)							\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);		

	#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

	#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

}

#endif // __Engine_Macro_H__
