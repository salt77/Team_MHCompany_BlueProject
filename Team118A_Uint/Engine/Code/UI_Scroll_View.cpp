#include "UI_Scroll_View.h"

#include "ProtoMgr.h"
#include "Renderer.h"
#include "KeyManager.h"
#include "FontMgr.h"

#include "RectBuffer.h"
#include "Texture.h"
#include "Shader.h"

CUI_Scroll_View::CUI_Scroll_View(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag)
	: CUI_Object(pGraphicDev, pHwnd, eObjTag)
{
	m_bIsScrollView = TRUE;
}

CUI_Scroll_View::CUI_Scroll_View(const CGameObject & rhs)
	: CUI_Object(rhs)
{
}

CUI_Scroll_View::~CUI_Scroll_View()
{
}


// �����ϱ����� Create�Լ� HWND*�� �޾ƿ� ������ ���ο��� Get_MousePos������ �����ϰ��Ѵ�.
CUI_Scroll_View * CUI_Scroll_View::Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag)
{
	CUI_Scroll_View* pInstance = new CUI_Scroll_View(pGraphicDev, pHwnd, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Scroll_View::Free()
{
	CUI_Object::Free();
}

// CUI_Object �� �߰��� �������� Save�Ѵ�
void CUI_Scroll_View::Save(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Save(hFile, dwByte);


	CUtility::Data_WriteInt(hFile, (_int)m_eGeometry, dwByte);
	CUtility::Data_WriteInt(hFile, m_iPadding[(_int)E_DIR::E_LEFT], dwByte);
	CUtility::Data_WriteInt(hFile, m_iPadding[(_int)E_DIR::E_RIGHT], dwByte);
	CUtility::Data_WriteInt(hFile, m_iPadding[(_int)E_DIR::E_TOP], dwByte);
	CUtility::Data_WriteInt(hFile, m_iPadding[(_int)E_DIR::E_BOTTOM], dwByte);
	CUtility::Data_WriteInt(hFile, m_iChildSize[(_int)E_POSITION::POS_X], dwByte);
	CUtility::Data_WriteInt(hFile, m_iChildSize[(_int)E_POSITION::POS_Y], dwByte);
	CUtility::Data_WriteInt(hFile, m_iSpacing[(_int)E_POSITION::POS_X], dwByte);
	CUtility::Data_WriteInt(hFile, m_iSpacing[(_int)E_POSITION::POS_Y], dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bChildSizeUse, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bVertScroll, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bHoriScroll, dwByte);
}

// CUI_Object �� �߰��� �������� Load�Ѵ�
void CUI_Scroll_View::Load(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Load(hFile, dwByte);

	_int iGeo = 0;
	CUtility::Data_ReadInt(hFile, iGeo, dwByte);
	m_eGeometry = (E_Geometry)iGeo;

	CUtility::Data_ReadInt(hFile, m_iPadding[(_int)E_DIR::E_LEFT], dwByte);
	CUtility::Data_ReadInt(hFile, m_iPadding[(_int)E_DIR::E_RIGHT], dwByte);
	CUtility::Data_ReadInt(hFile, m_iPadding[(_int)E_DIR::E_TOP], dwByte);
	CUtility::Data_ReadInt(hFile, m_iPadding[(_int)E_DIR::E_BOTTOM], dwByte);
	CUtility::Data_ReadInt(hFile, m_iChildSize[(_int)E_POSITION::POS_X], dwByte);
	CUtility::Data_ReadInt(hFile, m_iChildSize[(_int)E_POSITION::POS_Y], dwByte);
	CUtility::Data_ReadInt(hFile, m_iSpacing[(_int)E_POSITION::POS_X], dwByte);
	CUtility::Data_ReadInt(hFile, m_iSpacing[(_int)E_POSITION::POS_Y], dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bChildSizeUse, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bVertScroll, dwByte);
	CUtility::Data_ReadBoolean(hFile, m_bHoriScroll, dwByte);
}

// ������� �Լ�
CUI_Scroll_View * CUI_Scroll_View::Clone()
{
	return new CUI_Scroll_View(*this);
}

// Ready �Լ� Ư���� ����� ���� ��ü �Լ��� Add_Component�� ȣ���Ѵ�.
HRESULT CUI_Scroll_View::Ready_Object()
{
	CGameObject::Ready_Object();
	m_wstrUITag = L"CUI_Scroll_View";

	Add_Component();

	return S_OK;
}

// ����� ����.
_int CUI_Scroll_View::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::Start_Object(fTimeDelta);

	for (auto& iter : m_listChild)
	{
		iter->Start_Object(fTimeDelta);
	}

	return iExit;
}

// CUI_Object::Update_Object
_int CUI_Scroll_View::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::Update_Object(fTimeDelta);

	return iExit;
}

// �ڽ��� ������ ���� �ڽ��� ������� ���� �� �ٿ� ��� �ڽ��� �� �� �ִ��� üũ �� ��
// �ڽĵ��� �����ϰ� ��ġ���ش�.
_int CUI_Scroll_View::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::LateUpdate_Object(fTimeDelta);

	Update_UiTransform();
	Update_MovingQueue(fTimeDelta);

	if (nullptr == m_pParent)
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_UI, this);
	}

	_int iChildStack = 0;
	_int iSize_X = m_iChildSize[(_int)E_POSITION::POS_X];
	_int iSize_Y = m_iChildSize[(_int)E_POSITION::POS_Y];
	_int iSpacing_X = m_iSpacing[(_int)E_POSITION::POS_X];
	_int iSpacing_Y = m_iSpacing[(_int)E_POSITION::POS_Y];

	_int iMaxStack_X = (m_tUiTransform.iSizeX / (iSize_X + iSpacing_X)) ;

	m_bChildClickCheck = Is_OnMouse(CUtility::Get_MousePoint(*m_pHwnd));

	if (iMaxStack_X >= 2)
	{
		iMaxStack_X -= 1;
	}
	for (auto& iter : m_listChild)
	{
		if (FALSE == iter->Get_Active() || TRUE == iter->Get_Start())
			continue;

		iter->LateUpdate_Object(fTimeDelta);

		_int iPosX = (+iSize_X + iSpacing_X) * iChildStack;
		_int iPosY = (-iSize_Y - iSpacing_Y) * iChildStack;
		
		if (E_Geometry::E_VERTICAL == m_eGeometry)
		{
			iPosX = 0;
			iPosY = (-iSize_Y - iSpacing_Y) * iChildStack;

		}
		else if (E_Geometry::E_HORIZONTAL == m_eGeometry)
		{
			iPosX = (+iSize_X + iSpacing_X) * iChildStack;
			iPosY = 0;
		}
		else
		{
			if (iChildStack != 0)
			{
				iPosX = (+iSize_X + iSpacing_X) * (iChildStack % (iMaxStack_X + 1));
				iPosY = (-iSize_Y - iSpacing_Y) * (iChildStack / (iMaxStack_X + 1));
			}
		}

		iter->Set_Ui_PosX(iPosX);
		iter->Set_Ui_PosY(iPosY);

		iter->Add_Ui_PosX(+m_iPadding[(_int)E_DIR::E_LEFT]);
		iter->Add_Ui_PosX(-m_iPadding[(_int)E_DIR::E_RIGHT]);
		iter->Add_Ui_PosY(+m_iPadding[(_int)E_DIR::E_TOP]);
		iter->Add_Ui_PosY(-m_iPadding[(_int)E_DIR::E_BOTTOM]);

		if (m_bChildSizeUse)
		{
			iter->Set_Ui_SizeX(iSize_X);
			iter->Set_Ui_SizeY(iSize_Y);
		}

		++iChildStack;
	}


	return iExit;
}

void CUI_Scroll_View::Render_Object(_int iPass)
{
	m_iPassNumber = (_int)E_UI_ShaderPassType::ScrollView_Child;
	CUI_Object::Render_Object(iPass);
}

void CUI_Scroll_View::Child_Render_Object()
{
	CUI_Object::Child_Render_Object();
}

void CUI_Scroll_View::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"��ũ�� ���Դϴ�.");
}

// RectBuffer, Shader�� ������Ʈ�� �����Ѵ�.
// Texture�� Get_Texture�� ���� ProgoMgr���� �����´�
void CUI_Scroll_View::Add_Component()
{
	// RectBuffer ����
	// �⺻ ������ -0.5f, 0.5f
	m_pRectBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));

	// �ӽ� �ؽ��� ����
	Set_Tex1_Tag(L"../../Reference/Resource/Texture/Test/TexForTest.png");

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Ui")));
}

// ���콺�� Ŭ���� ������ ��� ���� ��ũ�� ���� ��ũ���� ���콺 �̵��� �� ��ŭ �����ؼ�
// Padding Left, Top�� �� ���ش�.
void CUI_Scroll_View::OnMouse_ThisFrame()
{
	if (FALSE == CKeyManager::GetInstance()->Key_Pressing(KEY_LBUTTON))
		return;

	_int iMaxStack_X = (m_tUiTransform.iSizeX / (m_iChildSize[(_int)E_POSITION::POS_X] + m_iSpacing[(_int)E_POSITION::POS_X]));
	iMaxStack_X = CUtility::Clamp_int(iMaxStack_X, 1, iMaxStack_X);
	_int iX_LineSize = (_int)m_listChild.size() % iMaxStack_X;

	_int iMaxStack_Y = (m_tUiTransform.iSizeY / (m_iChildSize[(_int)E_POSITION::POS_Y] + m_iSpacing[(_int)E_POSITION::POS_Y]));
	iMaxStack_Y = CUtility::Clamp_int(iMaxStack_Y, 1, iMaxStack_Y);
	_int iY_LineSize = (_int)m_listChild.size() / iMaxStack_X;

	_int iVertMin = (_int)(m_iChildSize[(_int)E_POSITION::POS_Y] + m_iSpacing[(_int)E_POSITION::POS_Y] * 0.2f);
	_int iVertMax = (_int)((m_iChildSize[(_int)E_POSITION::POS_Y] + m_iSpacing[(_int)E_POSITION::POS_Y]) * (iY_LineSize + 1));

	if (m_bVertScroll)
	{
		_int iBottomLimit = (iVertMax - m_tUiTransform.iSizeY);
		m_iPadding[(_int)E_DIR::E_TOP] -= (_int)CKeyManager::GetInstance()->Get_MouseMovementFigure().y;

		// �Ʒ� ����
		if(m_iPadding[(_int)E_DIR::E_TOP] > iBottomLimit)
		{
			m_iPadding[(_int)E_DIR::E_TOP] += (_int)CKeyManager::GetInstance()->Get_MouseMovementFigure().y;
		}
		// �� ����
		else if (m_iPadding[(_int)E_DIR::E_TOP] < -(m_iChildSize[(_int)E_POSITION::POS_Y] + m_iSpacing[(_int)E_POSITION::POS_Y]) * 0.2f )
		{
			m_iPadding[(_int)E_DIR::E_TOP] += (_int)CKeyManager::GetInstance()->Get_MouseMovementFigure().y;
		}
	}

	if (m_bHoriScroll)
	{
		m_iPadding[(_int)E_DIR::E_LEFT] += (_int)CKeyManager::GetInstance()->Get_MouseMovementFigure().x;


	}

}

// �ڽ��� PassNumber�� ScrollView_Child�� �����.
void CUI_Scroll_View::Set_Child(CUI_Object * pUi)
{
	CUI_Object::Set_Child(pUi);
	(pUi)->Set_PassNumber((_int)E_UI_ShaderPassType::ScrollView_Child);
}

// ��ũ�� �ɼ� OnOff�� �Լ�
void CUI_Scroll_View::Set_ScrollOption(E_Geometry eGeo, _bool bState)
{
	if (E_Geometry::E_VERTICAL == eGeo)
		m_bVertScroll = bState;
	else if (E_Geometry::E_HORIZONTAL == eGeo)
		m_bHoriScroll = bState;
}

// ��ũ�� �ɼ� ��ȯ�� �Լ�
_bool CUI_Scroll_View::Get_ScrollOption(E_Geometry eGeo)
{
	if (E_Geometry::E_VERTICAL == eGeo)
		return m_bVertScroll;
	else if (E_Geometry::E_HORIZONTAL == eGeo)
		return m_bHoriScroll;


	return FALSE;
}
