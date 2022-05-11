#include "UI_Slider_Vert.h"

#include "ProtoMgr.h"
#include "Renderer.h"
#include "KeyManager.h"
#include "FontMgr.h"

#include "RectBuffer.h"
#include "Texture.h"
#include "Shader.h"

CUI_Slider_Vert::CUI_Slider_Vert(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag)
	: CUI_Object(pGraphicDev, pHwnd, eObjTag)
{
}

CUI_Slider_Vert::CUI_Slider_Vert(const CGameObject & rhs)
	: CUI_Object(rhs)
{
}

CUI_Slider_Vert::~CUI_Slider_Vert()
{
}

// 생성하기위한 Create함수 HWND*를 받아와 저장해 내부에서 Get_MousePos연산이 가능하게한다.
CUI_Slider_Vert * CUI_Slider_Vert::Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag)
{
	CUI_Slider_Vert* pInstance = new CUI_Slider_Vert(pGraphicDev, pHwnd, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

// 부모와 동일
void CUI_Slider_Vert::Free()
{
	CUI_Object::Free();
}

// 부모와 동일
void CUI_Slider_Vert::Save(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Save(hFile, dwByte);
}

// Load가 다른 이유는 Bar를 생성후 바로 만들기 때문에
// 생성된 Bar를 지우고 Load한 Bar를 자식으로 하기 위해서
void CUI_Slider_Vert::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte);

	ReadFile(hFile, &m_tUiTransform, sizeof(T_UiTransform), &dwByte, nullptr);
	ReadFile(hFile, &m_tUiAnimation, sizeof(T_UiAnimation), &dwByte, nullptr);
	ReadFile(hFile, &m_ColorShaderOption, sizeof(D3DXCOLOR), &dwByte, nullptr);

	ReadFile(hFile, &m_tTextOption.tColor, sizeof(D3DXCOLOR), &dwByte, nullptr);
	_int iFontType = 0;
	CUtility::Data_ReadInt(hFile, iFontType, dwByte);
	m_tTextOption.eFontType = (E_FontType)iFontType;
	CUtility::Data_ReadInt(hFile, m_tTextOption.iTextPosX, dwByte);
	CUtility::Data_ReadInt(hFile, m_tTextOption.iTextPosY, dwByte);
	CUtility::Data_ReadInt(hFile, m_tTextOption.iFontSize, dwByte);

	_int iTextVoa = 0;
	CUtility::Data_ReadInt(hFile, iTextVoa, dwByte);
	m_tTextOption.eTextVertexOfAngle = (E_VertexOfAngle)iTextVoa;

	CUtility::Data_ReadBoolean(hFile, m_tTextOption.bBold, dwByte);


	CUtility::Data_ReadWString(hFile, m_wText, dwByte);
	m_wText = L"";
	CUtility::Data_ReadBoolean(hFile, m_bClicked, dwByte);

	CUtility::Data_ReadWString(hFile, m_wstrTexProtoTag_1, dwByte);
	if (m_wstrTexProtoTag_1 != L"")
		Set_Tex1_Tag(m_wstrTexProtoTag_1);
	CUtility::Data_ReadWString(hFile, m_wstrTexProtoTag_2, dwByte);
	if (m_wstrTexProtoTag_2 != L"")
		Set_Tex1_Tag(m_wstrTexProtoTag_2);
	CUtility::Data_ReadWString(hFile, m_wstrNameTag, dwByte);

	CUtility::Data_ReadBoolean(hFile, m_bChildClickCheck, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bOnClickAnimation, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bOnEnableAnimation, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bMask, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bItalic, dwByte);
	CUtility::Data_ReadInt(hFile, m_iTextAlign, dwByte);

	_int iSize = 0;
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	m_vecClickEvent.clear();
	for (_int i = 0; i < iSize; ++i)
	{
		_int iLoadEventQueue = 0;
		CUtility::Data_ReadInt(hFile, iLoadEventQueue, dwByte);
		Add_EventQueue(iLoadEventQueue);
	}

	m_listButtonEventType.clear();
	m_listClickEvent.clear();
	_int iEventTotalSize = 0;
	CUtility::Data_ReadInt(hFile, iEventTotalSize, dwByte);
	for (_int i = 0; i < iEventTotalSize; ++i)
	{
		vector<_int> vecTemp;
		_int iEventSize = 0;
		CUtility::Data_ReadInt(hFile, iEventSize, dwByte);
		vecTemp.reserve(iEventSize);
		vecTemp.resize(iEventSize);
		for (_int j = 0; j < iEventSize; ++j)
		{
			CUtility::Data_WriteInt(hFile, vecTemp[j], dwByte);
		}

		_int iEventType = 0;
		CUtility::Data_WriteInt(hFile, iEventType, dwByte);
		m_listButtonEventType.push_back((E_UiButtonEventType)iEventType);
	}

	_int iChildSize = (_int)m_listChild.size();
	CUtility::Data_ReadInt(hFile, iChildSize, dwByte);
	for (_int i = 0; i < iChildSize; ++i)
	{
		wstring wstrUITag;
		CUtility::Data_ReadWString(hFile, wstrUITag, dwByte);

		_int iObjTag = 0;
		E_OBJTAG eTag = E_OBJTAG::OBJ_END;
		CUtility::Data_ReadInt(hFile, iObjTag, dwByte);
		eTag = (E_OBJTAG)iObjTag;

		CUI_Object* pChild = nullptr;
		if (L"CUI_Object" == wstrUITag)
		{
			pChild = CUI_Object::Create(m_pGraphicDev, m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER_BAR);
		}

		if (pChild)
		{
			Safe_Release(Get_ChildList_NotConst()->front());
			m_listChild.clear();
			pChild->Load(hFile, dwByte);
			Set_Child(pChild);
			m_pBar = pChild;
		}
	}

	// 자식 로드 안해도됨.
}

// 복사생성 함수
CUI_Slider_Vert * CUI_Slider_Vert::Clone()
{
	return new CUI_Slider_Vert(*this);
}

// Ready 함수 특별한 기능은 없고 자체 함수인 Add_Component만 호출한다.
HRESULT CUI_Slider_Vert::Ready_Object()
{
	CGameObject::Ready_Object();
	m_wstrUITag = L"CUI_Slider_Vert";

	Add_Component();

	return S_OK;
}

// 부모와 동일
_int CUI_Slider_Vert::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::Start_Object(fTimeDelta);

	return iExit;
}

// Bar 액션을 매 프레임 실행한다.
_int CUI_Slider_Vert::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::Update_Object(fTimeDelta);

	Bar_Action();

	return iExit;
}

// 부모와 동일
_int CUI_Slider_Vert::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::LateUpdate_Object(fTimeDelta);

	return iExit;
}

// 부모와 동일
void CUI_Slider_Vert::Render_Object(_int iPass)
{
	CUI_Object::Render_Object(iPass);
}

// 부모와 동일
void CUI_Slider_Vert::Child_Render_Object()
{
	CUI_Object::Child_Render_Object();
}

void CUI_Slider_Vert::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"슬라이더입니다.");
}

// 기본적으로 슬라이더는 Bar를 가지고 있어야 하기 때문에 Bar를 생성해준다.
void CUI_Slider_Vert::Add_Component()
{
	// RectBuffer 세팅
	// 기본 사이즈 -0.5f, 0.5f
	m_pRectBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));

	// 임시 텍스쳐 설정
	Set_Tex1_Tag(L"../../Reference/Resource/Texture/Test/TexForTest.png");

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Ui")));


	m_pBar = CUI_Object::Create(m_pGraphicDev, m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER_BAR);
	Set_Child(m_pBar);
}

// Bar를 클릭하고 마우스를 움직이면 수치를 보정해 Bar를 움직여 준다.
void CUI_Slider_Vert::Bar_Action()
{
	if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON) && FALSE == m_bBarClicked)
	{
		POINT ptMouse = CUtility::Get_MousePoint(*m_pHwnd);
		if (m_pBar->Is_OnMouse(ptMouse))
		{
			m_bBarClicked = TRUE;
		}
	}
	else if (TRUE == m_bBarClicked)
	{
		if (CKeyManager::GetInstance()->Key_Up(KEY_LBUTTON))
		{
			m_bBarClicked = FALSE;
		}
		_int xMove = (_int)CKeyManager::GetInstance()->Get_MouseMovementFigure().x;
		if ((_int)(Get_UiTransform().iSizeX * 0.4) > m_pBar->Get_UiTransform().iPosX && 0 < xMove)
		{
			m_pBar->Add_Ui_PosX(xMove);

			if ((_int)(Get_UiTransform().iSizeX * 0.4) < m_pBar->Get_UiTransform().iPosX)
			{
				m_pBar->Set_Ui_PosX((_int)(Get_UiTransform().iSizeX * 0.4));
			}
		}
		else if ((_int)(Get_UiTransform().iSizeX * -0.4) < m_pBar->Get_UiTransform().iPosX && 0 > xMove)
		{
			m_pBar->Add_Ui_PosX(xMove);

			if ((_int)(Get_UiTransform().iSizeX * -0.4) > m_pBar->Get_UiTransform().iPosX)
			{
				m_pBar->Set_Ui_PosX((_int)(Get_UiTransform().iSizeX * -0.4));
			}
		}
	}
}

// 슬라이더의 Radio를 반환하는 함수
_float CUI_Slider_Vert::Get_Slider_Ratio()
{
	_float fDistance	= (_float)m_tUiTransform.iSizeX;
	_float fBarLength	= (_float)m_pBar->Get_UiTransform().iPosX;

	_float fResult = fBarLength / fDistance;
	return (fResult + 0.4f) * 1.25f;
}
