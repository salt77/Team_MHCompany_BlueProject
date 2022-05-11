#include "UI_Object.h"

#include "ProtoMgr.h"
#include "Renderer.h"
#include "KeyManager.h"
#include "FontMgr.h"
#include "Wall512.h"

#include "RectBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "UI_Slider_Vert.h"
#include "UI_Scroll_View.h"
#include "UI_Damage.h"

void CUI_Object::bTest(_bool basd)
{
	CUtility::Print_Wstring_DMode(to_wstring(basd));
}

void CUI_Object::iTest(_int iTest)
{
	CUtility::Print_Wstring_DMode(to_wstring(iTest));
}

void CUI_Object::fTest(_float fasd)
{
	CUtility::Print_Wstring_DMode(to_wstring(fasd));
}

CUI_Object::CUI_Object(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
	, m_pHwnd(pHwnd)
{
	Set_PassNumber((_int)E_UI_ShaderPassType::ScrollView_Child);
	m_tTextOption.tColor = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
}

CUI_Object::CUI_Object(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

CUI_Object::~CUI_Object()
{
}

// �����ϱ����� Create�Լ� HWND*�� �޾ƿ� ������ ���ο��� Get_MousePos������ �����ϰ��Ѵ�.
CUI_Object* CUI_Object::Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND* pHwnd, E_OBJTAG eObjTag)
{
	CUI_Object* pInstance = new CUI_Object(pGraphicDev, pHwnd, eObjTag);
	CFontMgr::GetInstance()->Add_LoadingGage();
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

// ��� �ڽĵ��� for loop �� ���� ��������� �����Ѵ�.
void CUI_Object::Free()
{
	// �ڽ��� �ִٸ� ����
	Clear_Function(E_UiFunctionType::OnClick);
	Clear_Function(E_UiFunctionType::OnDisable);
	Clear_Function(E_UiFunctionType::OnEnable);

	list<CUI_Object*>::iterator iter = m_listChild.begin();
	for (; iter != m_listChild.end(); ++iter)
	{
		Safe_Release(*iter);
	}
	m_listChild.clear();

	CGameObject::Free();
}

// UI_Object���� Save�ϱ����� �Լ� ��� ������ Save�� �� �ڽ��� ������ �ڽĵ��� ��ȸ�ϸ� Save�Ѵ�.
void CUI_Object::Save(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_WriteWString(hFile, m_wstrUITag, dwByte);

	CGameObject::Save(hFile, dwByte);

	WriteFile(hFile, &m_tUiTransform, sizeof(T_UiTransform), &dwByte, nullptr);
	WriteFile(hFile, &m_tUiAnimation, sizeof(T_UiAnimation), &dwByte, nullptr);
	WriteFile(hFile, &m_ColorShaderOption, sizeof(D3DXCOLOR), &dwByte, nullptr);

	// ������ �߸�����. DWORD���� D3DXCOLOR�� �����ϸ� ��� ���� �������ϱ� ���߿� ���ļ� ����
	WriteFile(hFile, &m_tTextOption.tColor, sizeof(D3DXCOLOR), &dwByte, nullptr);
	CUtility::Data_WriteInt(hFile, (_int)m_tTextOption.eFontType, dwByte);
	CUtility::Data_WriteInt(hFile, m_tTextOption.iTextPosX, dwByte);
	CUtility::Data_WriteInt(hFile, m_tTextOption.iTextPosY, dwByte);
	if (m_tTextOption.iFontSize == 0)
		m_tTextOption.iFontSize = 15;
	CUtility::Data_WriteInt(hFile, m_tTextOption.iFontSize, dwByte);

	CUtility::Data_WriteInt(hFile, (_int)m_tTextOption.eTextVertexOfAngle, dwByte);

	CUtility::Data_WriteBoolean(hFile, (_int)m_tTextOption.bBold, dwByte);

	CUtility::Data_WriteWString(hFile, m_wText, dwByte);

	CUtility::Data_WriteBoolean(hFile, m_bClicked, dwByte);

	CUtility::Data_WriteWString(hFile, m_wstrTexProtoTag_1, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrTexProtoTag_2, dwByte);
	CUtility::Data_WriteWString(hFile, m_wstrNameTag, dwByte);

	CUtility::Data_WriteBoolean(hFile, m_bChildClickCheck, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bOnClickAnimation, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bOnEnableAnimation, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bMask, dwByte);
	CUtility::Data_WriteBoolean(hFile, m_bItalic, dwByte);
	CUtility::Data_WriteInt(hFile, m_iTextAlign, dwByte);

	_int iSize = (_int)m_vecClickEvent.size();
	CUtility::Data_WriteInt(hFile, iSize, dwByte);
	for (_int i = 0; i < iSize; ++i)
	{
		CUtility::Data_WriteInt(hFile, m_vecClickEvent[i], dwByte);
	}

	auto iterEventType = m_listButtonEventType.begin();
	_int iEventTotalSize = (_int)m_listClickEvent.size();
	CUtility::Data_WriteInt(hFile, iEventTotalSize, dwByte);
	for (auto& iterTotal : m_listClickEvent)
	{
		_int iEventSize = (_int)iterTotal.size();
		CUtility::Data_WriteInt(hFile, iEventSize, dwByte);
		for (_int i = 0; i < (_int)iterTotal.size(); ++i)
		{
			CUtility::Data_WriteInt(hFile, iterTotal[i], dwByte);
		}

		CUtility::Data_WriteInt(hFile, (_int)*iterEventType, dwByte);
		++iterEventType;
	}


	_int iChildSize = (_int)m_listChild.size();
	CUtility::Data_WriteInt(hFile, iChildSize, dwByte);
	for (auto& iter : m_listChild)
	{
		iter->Save(hFile, dwByte);
	}
}

// UI_Object���� Load�ϱ����� �Լ� ��� ������ Load�� �� �ڽ��� ������ �ڽĵ��� ��ȸ�ϸ� Create�Ѵ�.
void CUI_Object::Load(HANDLE hFile, DWORD & dwByte)
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

	m_listClickEvent.clear();
	m_listButtonEventType.clear();
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
			CUtility::Data_ReadInt(hFile, vecTemp[j], dwByte);
		}

		_int iEventType = 0;
		CUtility::Data_ReadInt(hFile, iEventType, dwByte);
		m_listClickEvent.push_back(vecTemp);
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
			pChild = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI);
		}
		else if (L"CUI_Slider_Vert" == wstrUITag)
		{
			pChild = CUI_Slider_Vert::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER);
		}
		else if (L"CUI_Scroll_View" == wstrUITag)
		{
			pChild = CUI_Scroll_View::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SCROLLVIEW);
		}
		else if (L"OBJ_UI_SLIDER_BAR" == wstrUITag)
		{
			pChild = CUI_Object::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI_SLIDER_BAR);
		}
		else if (L"CUI_Damage" == wstrUITag)
		{
			pChild = CUI_Damage::Create(CGraphicDev::GetInstance()->GetDevice(), m_pHwnd, E_OBJTAG::OBJ_UI, 9999999.f, E_NumberFontType::Normal, 0);
		}

		if (pChild)
		{
			pChild->Load(hFile, dwByte);
			Set_Child(pChild);
		}
	}

}

// ������� �Լ�
CUI_Object * CUI_Object::Clone()
{
	return new CUI_Object(*this);
}

// Ready �Լ� Ư���� ����� ���� ��ü �Լ��� Add_Component�� ȣ���Ѵ�.
HRESULT CUI_Object::Ready_Object()
{
	CGameObject::Ready_Object();
	m_wstrUITag = L"CUI_Object";

	Add_Component();

	return S_OK;
}

// ����� ����.
_int CUI_Object::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);
	//Set_Mask(_vec2(0.f, 0.f), _vec2(1.f, 1.f));

	for (auto& iter : m_listChild)
	{
		iter->Start_Object(fTimeDelta);
	}

	// Invoke_Add(2.f, bind(&CUI_Object::fTest, this, 2.13f), 1.2345f);
	// Invoke_Add(4.f, bind(&CUI_Object::bTest, this, false), true);
	// Invoke_Add(6.f, bind(&CUI_Object::iTest, this, 5), 10);

	if (m_bOnEnableAnimation)
	{
		Set_AnimStart(1.f);
		m_tUiAnimation.fProgNow = 0.f;
	}

	return iExit;
}

// �ڽĵ��� ��������� Update���ش�. Dead�� True�� �ڽ��� �ִٸ� erase�Ѵ�.
// m_tUiTransform.bAnimationState�� True�� �� �ִϸ��̼��� �� ������ �����Ų��.
_int CUI_Object::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	list<CUI_Object*>::iterator iter = m_listChild.begin();
	for (; iter != m_listChild.end();)
	{
		if (FALSE == (*iter)->Get_Active())
		{
		}
		else if ((*iter)->Get_Dead())
		{
			Safe_Release(*iter);
			iter = m_listChild.erase(iter);
			continue;
		}
		else if ((*iter)->Get_Start())
		{
			(*iter)->Start_Object(fTimeDelta);
		}
		else
		{
			(*iter)->Update_Object(fTimeDelta);
		}
		++iter;
	}

	Animation(fTimeDelta); 
	FadeInOut(fTimeDelta); 

	return iExit;
}

// Update_UiTransform�� ���� �θ��� ��ǥ�� ������� ��ǥ�� Position�� �����Ѵ�.
// �θ� ������ RENDER_UI ���� �׷쿡 �߰��Ѵ�, �θ� �ִٸ� �θ��� ���� Render_Object�Լ����� ��������� ȣ���Ѵ�.
_int CUI_Object::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Update_UiTransform();
	Update_MovingQueue(fTimeDelta);
	if (nullptr == m_pParent)
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_UI, this);
	}

	for (auto& iter : m_listChild)
	{
		if (FALSE == iter->Get_Active())
			continue;
		else if (FALSE == iter->Get_Start())
		{
			iter->LateUpdate_Object(fTimeDelta);
		}
	}

	return iExit;
}

// �ڽ��� Texture -> �ڽ��� Font -> �ڽ��� Texture -> �ڽ��� Font ������ Rendering �Ѵ�.
void CUI_Object::Render_Object(_int iPass)
{
	if (TRUE == m_bEnableThisFrame || CRenderer::GetInstance()->Get_CutIn())
	{
		m_bEnableThisFrame = FALSE;
		return;
	}

	Set_ConstantTable();
	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(m_iPassNumber);

	m_pRectBufferCom->Render_Buffer();
	m_pShaderCom->EndPass();

	m_pShaderCom->EndShader();

	Render_Font();

	for (auto& iter : m_listChild)
	{
		if (FALSE == m_bMouseOn && E_UiButtonEventType::E_POPUP == (E_UiButtonEventType)m_tUiTransform.iButtonEventType)
		{
			return;
		}
		if (FALSE == iter->Get_Active())
			continue;
		iter->Child_Render_Object();
	}
}

// �ڽĿ� Redner �Լ�, Follow_ParentMaskUV�Լ��� ���� Mask �ɼǵ� �������ش�.
// �ڽ��� Texture -> �ڽ��� Font -> �ڽ��� Texture -> �ڽ��� Font ������ Rendering �Ѵ�.
void CUI_Object::Child_Render_Object()
{
	Follow_ParentMaskUV();

	if (TRUE == m_bMask &&
		(m_vMaskRatio_Min.x >= 1.f ||
			m_vMaskRatio_Min.y >= 1.f ||
			m_vMaskRatio_Max.x <= 0.f ||
			m_vMaskRatio_Max.y <= 0.f))
	{
		return;
	}

	if (FALSE == m_bStart)
	{
		Set_ConstantTable();

		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);

		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();

		Render_Font();

		for (auto& iter : m_listChild)
		{
			if (FALSE == m_bMouseOn && E_UiButtonEventType::E_POPUP == (E_UiButtonEventType)m_tUiTransform.iButtonEventType)
			{
				break;
			}
			if (FALSE == iter->Get_Active())
				continue;
			iter->Child_Render_Object();
		}
	}
	m_bStart = FALSE;
}

// UI�� ��Ʈ�� ���� ȭ�鿡 �ش� Font�� ������ش�.
void CUI_Object::Render_Font()
{
	if (FALSE == m_tUiTransform.bUseText)
		return;

	_vec2 vRenderPosition = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
	if (m_pParent)
	{
		_vec2 vPos = VEC_ZERO;
		Get_ParentPos(vPos);
		vRenderPosition.x += vPos.x + Get_PivotAddPos(m_tUiTransform.eTransformPivot).x;
		vRenderPosition.y += vPos.y + Get_PivotAddPos(m_tUiTransform.eTransformPivot).y;
		vRenderPosition.x += m_pParent->Get_PivotAddPos(m_pParent->m_tUiTransform.eTransformPivot).x;
		vRenderPosition.y += m_pParent->Get_PivotAddPos(m_pParent->m_tUiTransform.eTransformPivot).y;
	}

	_vec2 vTextPos = Get_VertexOfAngle(m_tTextOption.eTextVertexOfAngle) + _vec2((_float)m_tTextOption.iTextPosX, (_float)m_tTextOption.iTextPosY);

	wstring wstrFontName = FontTag_���õ������_Medium;
	UINT iHeight = 0;

	switch (m_tTextOption.eFontType)
	{
	case Engine::E_FontType::���õ������_Medium:
		wstrFontName = FontTag_���õ������_Medium;
		break;
	case Engine::E_FontType::Noto_Sans_Medium:
		wstrFontName = FontTag_NotoSansTC_Medium;
		break;
	case Engine::E_FontType::Noto_Sans_TC_Medium:
		wstrFontName = FontTag_NotoSans_Medium;
		break;
	case Engine::E_FontType::RSU_Medium:
		wstrFontName = FontTag_RSU_Medium;
		break;
	case Engine::E_FontType::���õ������_Bold:
		wstrFontName = FontTag_���õ������_Bold;
		break;
	case Engine::E_FontType::Noto_Sans_Bold:
		wstrFontName = FontTag_NotoSans_Bold;
		break;
	case Engine::E_FontType::Noto_Sans_TC_Bold:
		wstrFontName = FontTag_NotoSansTC_Bold;
		break;
	case Engine::E_FontType::RSU_Bold:
		wstrFontName = FontTag_RSU_Bold;
		break;
	default:
		break;
	}


	_float fTotalSize = 0;
	if (m_iTextAlign != (_int)E_TextAlign::Left)
	{
		_int iIsHalfLength = 0;
		_int iIsLowerLength = 0;
		_int iIsHighLength = 0;
		for (_int i = 0; i < (_int)m_wText.size(); ++i)
		{
			if (m_wText[i] >= 44032 && m_wText[i] < 55199) // �ѱ��� ���
			{
			}
			else if (TRUE == iswupper(m_wText[i]))	// �빮���� ���
			{
				iIsHighLength++;
			}
			else if (FALSE != iswlower(m_wText[i]))	// �ҹ����� ���
			{
				iIsLowerLength++;
			}
			else
			{
				iIsHalfLength++;
			}
		}
		fTotalSize = (_int)m_wText.size() - (0.5f * iIsHalfLength) - (0.27f * iIsHighLength) - (0.37f * iIsLowerLength);
	}

	switch (m_iTextAlign)
	{
	case (_int)E_TextAlign::Left:
		break;
	case (_int)E_TextAlign::Center:
		if ((_int)E_FontType::Noto_Sans_TC_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.49f * fTotalSize;
		}
		else if ((_int)E_FontType::Noto_Sans_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.49f * fTotalSize;
		}
		else if ((_int)E_FontType::RSU_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.9f * fTotalSize;
		}
		else
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.9f * fTotalSize;
		}
		break;
	case (_int)E_TextAlign::Right:
		if ((_int)E_FontType::Noto_Sans_TC_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.98f * fTotalSize;
		}
		else if ((_int)E_FontType::Noto_Sans_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 0.98f * fTotalSize;
		}
		else if ((_int)E_FontType::RSU_Medium == (_int)m_tTextOption.eFontType % (_int)E_FontType::���õ������_Bold)
		{
			vTextPos.x -= m_tTextOption.iFontSize * 1.8f * fTotalSize;
		}
		else
		{
			vTextPos.x -= m_tTextOption.iFontSize * 1.8f * fTotalSize;
		}
		break;
	default:
		break;
	}

	CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(wstrFontName, m_wText, vTextPos, m_tTextOption.tColor, m_tTextOption.iFontSize, iHeight, FW_NORMAL, m_bItalic));
}

// Scale�� �������ְ� Get_ParentPos �Լ����� �θ��� ��ǥ�� ��� ������ �ڽ��� ��ǥ�� ���� �� 
// �����ǿ� �θ��� Animation��ǥ�� �� ���� �� �ڽ��� ���������� �����Ѵ�.
void CUI_Object::Update_UiTransform()
{
	Set_Scale(m_tUiTransform.Get_SizeFloat(E_POSITION::POS_X), m_tUiTransform.Get_SizeFloat(E_POSITION::POS_Y), 1.f);

	_vec2 vPos = VEC_ZERO;

	Get_ParentPos(vPos);

	if (m_tUiTransform.bAnimationState)
	{
		vPos += m_tUiAnimation.vAnimPosition;
	}
	Set_Pos(_vec3(vPos.x, vPos.y, (_float)m_tUiTransform.iPosZ));
}

void CUI_Object::Update_MovingQueue(const _float & fTimeDelta)
{
	if (m_qMoving.empty())
		return;


	T_UiMoving& tMoving = m_qMoving.front();
	tMoving.fTime += fTimeDelta;
	_vec2 vPos = CUtility::Lerp_vec2(tMoving.vStart, tMoving.vStart + tMoving.vAmount, CUtility::LerpEase_float(tMoving.eEase, tMoving.fTime / tMoving.fTotalTime));
	Set_Ui_PosX((_int)vPos.x);
	Set_Ui_PosY((_int)vPos.y);

	if (tMoving.fTime >= tMoving.fTotalTime)
	{
		m_qMoving.pop();

		if (FALSE == m_qMoving.empty())
		{
			m_qMoving.front().vStart = _vec2((_float)Get_UiTransform().iPosX, (_float)Get_UiTransform().iPosY);
		}
		return;
	}


}

// ���ڷ� ���� pUi�� �ڽ��� �ڽ����� �߰��ϰ� pUi�� �θ� �ڽ����� �����Ѵ�.
void CUI_Object::Set_Child(CUI_Object * pUi)
{
	pUi->m_pParent = this;
	m_listChild.push_back(pUi);
}

// ���ڷι��� wText�� ����� �ؽ�Ʈ�� �����Ѵ�. �ؽ�Ʈ�� �ִٸ� bUseText�� True�� �����.
void CUI_Object::Set_Ui_Text(wstring wText)
{
	m_wText = wText;

	m_tUiTransform.bUseText = m_wText.length() >= 1 ? TRUE : FALSE;
}

void CUI_Object::Add_Ui_Text(wstring wText)
{
	m_wText = m_wText += wText;

	m_tUiTransform.bUseText = m_wText.length() >= 1 ? TRUE : FALSE;
}

// �θ��� ���������� �ڽ��� Position�� �̵���Ų��.
// �θ� ���� �ֻ��� UI�� ȭ���� ���������� �̵�
void CUI_Object::Set_Ui_Anchor(const E_Anchor eAnchor)
{
	if (m_pParent)
	{
		switch (eAnchor)
		{
		case Engine::E_Anchor::Anchor_TL:
			Set_Ui_PosX(-(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(+(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		case Engine::E_Anchor::Anchor_TC:
			Set_Ui_PosX(0);
			Set_Ui_PosY(+(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		case Engine::E_Anchor::Anchor_TR:
			Set_Ui_PosX(+(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(+(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		case Engine::E_Anchor::Anchor_CL:
			Set_Ui_PosX(-(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_Center:
			Set_Ui_PosX(0);
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_CR:
			Set_Ui_PosX(+(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_BL:
			Set_Ui_PosX(-(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(-(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		case Engine::E_Anchor::Anchor_BC:
			Set_Ui_PosX(0);
			Set_Ui_PosY(-(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		case Engine::E_Anchor::Anchor_BR:
			Set_Ui_PosX(+(_int)(m_pParent->Get_UiTransform().iSizeX * 0.5f));
			Set_Ui_PosY(-(_int)(m_pParent->Get_UiTransform().iSizeY * 0.5f));
			break;
		default:
			assert(FALSE);
			break;
		}
	}
	else
	{
		switch (eAnchor)
		{
		case Engine::E_Anchor::Anchor_TL:
			Set_Ui_PosX(-(_int)(WINCX * 0.5));
			Set_Ui_PosY(+(_int)(WINCY * 0.5));
			break;
		case Engine::E_Anchor::Anchor_TC:
			Set_Ui_PosX(0);
			Set_Ui_PosY(+(_int)(WINCY * 0.5));
			break;
		case Engine::E_Anchor::Anchor_TR:
			Set_Ui_PosX(+(_int)(WINCX * 0.5));
			Set_Ui_PosY(+(_int)(WINCY * 0.5));
			break;
		case Engine::E_Anchor::Anchor_CL:
			Set_Ui_PosX(-(_int)(WINCX * 0.5));
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_Center:
			Set_Ui_PosX(0);
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_CR:
			Set_Ui_PosX(+(_int)(WINCX * 0.5));
			Set_Ui_PosY(0);
			break;
		case Engine::E_Anchor::Anchor_BL:
			Set_Ui_PosX(-(_int)(WINCX * 0.5));
			Set_Ui_PosY(-(_int)(WINCY * 0.5));
			break;
		case Engine::E_Anchor::Anchor_BC:
			Set_Ui_PosX(0);
			Set_Ui_PosY(-(_int)(WINCY * 0.5));
			break;
		case Engine::E_Anchor::Anchor_BR:
			Set_Ui_PosX(+(_int)(WINCX * 0.5));
			Set_Ui_PosY(-(_int)(WINCY * 0.5));
			break;
		default:
			assert(FALSE);
			break;
		}
	}
}

// Animation�� ������ �� ����ϴ� �Լ�,	�ʱ� ������ �ʿ��� �ִϸ��̼��� �̰����� �����Ѵ�.
void CUI_Object::Set_AnimStart(const _float fDir)
{
	m_fAnimationDir = fDir;
	if (FALSE == m_tUiTransform.bAnimationState)
	{
		m_tUiTransform.bAnimationState = TRUE;
		m_tUiAnimation.iRandDir = CWall512::GetInstance()->GetValue(0, 2);
		if (0 == m_tUiAnimation.iRandDir)
			m_tUiAnimation.iRandDir = -1;

		switch (m_tUiAnimation.eAnimType)
		{
		case Engine::E_UiAnimationType::Left_To_Origin:
			m_vAnimBeginPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimEndPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimBeginPos.x -= ((_float)WINCX + m_tUiTransform.iPosX + m_tUiTransform.iSizeX * 0.5f);
			break;
		case Engine::E_UiAnimationType::Right_To_Origin:
			m_vAnimBeginPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimEndPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimBeginPos.x += ((_float)WINCX - m_tUiTransform.iPosX + m_tUiTransform.iSizeX * 0.5f);
			break;
		case Engine::E_UiAnimationType::Top_To_Origin:
			m_vAnimBeginPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimEndPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimBeginPos.y += ((_float)WINCY - m_tUiTransform.iPosY + m_tUiTransform.iSizeY * 0.5f);
			break;
		case Engine::E_UiAnimationType::Bottom_To_Origin:
			m_vAnimBeginPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimEndPos = _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY);
			m_vAnimBeginPos.y -= ((_float)WINCY + m_tUiTransform.iPosY + m_tUiTransform.iSizeY * 0.5f);
			break;
		case Engine::E_UiAnimationType::UiAnim_End:
			break;
		default:
			break;
		}
	}
}

// Texture�� UV MinMax���� �����ϴ� �Լ�
// ���� ����		#1 : Min,Max,   #2 : X, Y,  #3 : Value
void CUI_Object::Set_UV(E_MinMax eMinMax, E_POSITION ePos, _float fValue)
{
	switch (eMinMax)
	{
	case Engine::E_MinMax::Min:
		if (E_POSITION::POS_X == ePos)
		{
			m_tUiTransform.vDrawUvMin.x = fValue;
		}
		else if (E_POSITION::POS_Y == ePos)
		{
			m_tUiTransform.vDrawUvMin.y = fValue;
		}
		else
		{
			assert(FALSE);
		}
		break;
	case Engine::E_MinMax::Max:
		if (E_POSITION::POS_X == ePos)
		{
			m_tUiTransform.vDrawUvMax.x = fValue;
		}
		else if (E_POSITION::POS_Y == ePos)
		{
			m_tUiTransform.vDrawUvMax.y = fValue;
		}
		else
		{
			assert(FALSE);
		}
		break;
	default:
		assert(FALSE);
		break;
	}
}

// Mask�� MinMax���� �����ϴ� �Լ� ���ڷ� ���� vMin, vMax�� �����Ѵ�.
void CUI_Object::Set_Mask(_vec2 vMin, _vec2 vMax)
{
	m_vMaskRatio_Min = vMin;
	m_vMaskRatio_Max = vMax;
}

// UI�� Texture�� �����ϴ� �Լ� 
void CUI_Object::Set_Tex1_Tag(wstring wstrTag)
{
	m_wstrTexProtoTag_1 = wstrTag;
	m_pTexture_1 = nullptr;

	// CUtility::Print_Wstring_DMode(L"UI Texture Tag1 ���� [" + wstrTag + L"]");
	Get_Tex1_FromTextureMgr();
}

// UI�� Texture�� ProtoMgr���� Get_Texture�� ���� �����´�.
void CUI_Object::Get_Tex1_FromTextureMgr()
{
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_wstrTexProtoTag_1, &m_pTexture_1);

	if (m_pTexture_1)
	{
		// CUtility::Print_Wstring_DMode(L"UI GetTexture ����  �±� [" + m_wstrTexProtoTag_1 + L"]");
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"UI GetTexture ����  �±� [" + m_wstrTexProtoTag_1 + L"]");
	}
}

// UI�� Texture�� �����ϴ� �Լ�    
// : Tex2�� ���� ������� ����.
void CUI_Object::Set_Tex2_Tag(wstring wstrTag)
{
	m_wstrTexProtoTag_2 = wstrTag;
	m_pTexture_2 = nullptr;

	CUtility::Print_Wstring_DMode(L"UI Texture Tag ���� [" + wstrTag + L"]");

	Get_Tex2_FromTextureMgr();
}

void CUI_Object::Add_UiMoving(T_UiMoving tMoving)
{
	CUtility::Print_Wstring_DMode(L"x : " + to_wstring(tMoving.vAmount.x)); 
	CUtility::Print_Wstring_DMode(L"y : " + to_wstring(tMoving.vAmount.y));

	tMoving.vStart = _vec2((_float)Get_UiTransform().iPosX, (_float)Get_UiTransform().iPosY);
	m_qMoving.push(tMoving);
}

void CUI_Object::Set_SortingOrder(_bool bUp)
{
	if (nullptr == this->Get_Parent())
	{
		return;
	}

	CUI_Object* pParent = this->Get_Parent();

	_bool bFind = FALSE;
	_bool bStopFind = FALSE;
	_int iFindIndex = 0;
	_int iSize = (_int)pParent->Get_ChildList()->size();

	if (1 >= iSize)
	{
		CUtility::Print_Wstring_DMode(L"�θ��� �ڽ��� ������ 1�� �����Դϴ�.");
		return;
	}
	list<CUI_Object*>::iterator iter = pParent->Get_ChildList_NotConst()->begin();
	list<CUI_Object*> tempList;
	CUI_Object* pBefore = nullptr;
	CUI_Object* pNext = nullptr;
	for (; iter != pParent->Get_ChildList()->end(); ++iter)
	{
		tempList.push_back((*iter));
		if (bStopFind)
		{
			continue;
		}
		if (bFind)
		{
			pNext = (*iter);
			bStopFind = TRUE;
		}
		else if ((*iter) == this)
		{
			CUtility::Print_Wstring_DMode(L"�ڽ��� ã�ҽ��ϴ� " + to_wstring(iFindIndex));
			bFind = TRUE;
		}
		else
		{
			++iFindIndex;
			pBefore = (*iter);
		}
	}

	if (FALSE == bFind)
	{
		CUtility::Print_Wstring_DMode(L"ã�� ���߽��ϴ�.");
		return;
	}

	_int iIndex = 0;
	if (!bUp)
	{
		if (iFindIndex < iSize - 1 && pNext)
		{
			pParent->Get_ChildList_NotConst()->clear();
			list<CUI_Object*>::iterator iter = tempList.begin();
			for (; iter != tempList.end(); ++iter)
			{
				if (iIndex == iFindIndex)
				{
					pParent->Get_ChildList_NotConst()->push_back(*(++iter));
					pParent->Get_ChildList_NotConst()->push_back(*(--iter));
					++iter;
					++iIndex;
				}
				else
				{
					pParent->Get_ChildList_NotConst()->push_back(*iter);
				}
				++iIndex;
			}
		}
	}
	else
	{
		if (iFindIndex >= 1 && pBefore)
		{
			pParent->Get_ChildList_NotConst()->clear();
			list<CUI_Object*>::iterator iter = tempList.begin();
			for (; iter != tempList.end(); ++iter)
			{
				if (iIndex == iFindIndex - 1)
				{
					pParent->Get_ChildList_NotConst()->push_back(*(++iter));
					pParent->Get_ChildList_NotConst()->push_back(*(--iter));
					++iter;
					++iIndex;
				}
				else
				{
					pParent->Get_ChildList_NotConst()->push_back(*iter);
				}
				++iIndex;
			}
		}
	}
}

void CUI_Object::Set_FadeInOut(E_FADETYPE eFadeType, _float fFadeTime)
{
	m_bFadeInOut = TRUE;
	m_eFadeType = eFadeType;
	m_fFadeEndTime = fFadeTime;

	if (E_FADETYPE::FADETYPE_OUT == m_eFadeType) 
	{
		m_fFadeCountTime = m_fFadeEndTime; 
	} 
}

// UI�� Texture�� ProtoMgr���� Get_Texture�� ���� �����´�.	    
// : Tex2�� ���� ������� ����.
void CUI_Object::Get_Tex2_FromTextureMgr()
{

	CUtility::Print_Wstring_DMode(L"�ؽ��� �ε�");

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, m_wstrTexProtoTag_2, &m_pTexture_2);

	if (m_pTexture_2)
	{
		CUtility::Print_Wstring_DMode(L"UI GetTexture ����  �±� [" + m_wstrTexProtoTag_2 + L"]");
	}
	else
	{
		CUtility::Print_Wstring_DMode(L"UI GetTexture ����  �±� [" + m_wstrTexProtoTag_2 + L"]");
	}
}

// ClickEvent�� �Լ� vector<_int>�� Size��ŭ ��ȸ�ϸ鼭 Event ������Ʈ�� ã�´�.
// -1�� �θ�, +1�� ù ��° �ڽ��� ã�ư���.		ClickEvent -1-13 �� �θ��Ǻθ��� 3��° �ڽ�
CUI_Object * CUI_Object::Get_EventObject(vector<_int>& vecEvent, _int& iMaxDepth, _int iDeep)
{
	CUI_Object* Event = nullptr;

	if (iDeep >= vecEvent.size() && iDeep > 0)
		return this;
	else if (iDeep >= vecEvent.size())
		return Event;

	iMaxDepth = iDeep + 1;

	_int iIndex = vecEvent[iDeep];
	if (0 == iIndex)
		return this;

	if (-1 == iIndex)
	{
		if (nullptr == m_pParent)
			return nullptr;

		Event = m_pParent->Get_EventObject(vecEvent, iMaxDepth, iDeep + 1);
	}
	else if (0 < iIndex)
	{
		if (iIndex > (_int)m_listChild.size())
			return nullptr;

		auto& iter = m_listChild.begin();
		for (_int i = 0; i < iIndex - 1; ++i)
		{
			++iter;
		}

		Event = (*iter)->Get_EventObject(vecEvent, iMaxDepth, iDeep + 1);
	}

	return Event;
}

// �ֻ��� �θ���� ��ȸ�ϸ鼭 ��ǥ�� �� ���ش�.
void CUI_Object::Get_ParentPos(_vec2& vPos)
{
	if (m_pParent)
	{
		m_pParent->Get_ParentPos(vPos);
	}

	vPos += _vec2((_float)m_tUiTransform.iPosX, (_float)m_tUiTransform.iPosY) + Get_PivotAddPos(m_tUiTransform.eTransformPivot);

	if (m_tUiAnimation.bMyChildFollowMyAnim)
	{
		vPos += m_tUiAnimation.vAnimPosition;
	}
}

// ���ڷ� ���� iIndex��°�� �ڽ��� ��ȯ �� �ش�.
CUI_Object * CUI_Object::Get_ChildByIndex(_int iIndex)
{
	if (m_listChild.empty())
	{
		assert(FALSE);
		return nullptr;
	}

	if (iIndex >= (_int)m_listChild.size())
	{
		// ���� �Ѿ
		assert(FALSE);
		return nullptr;
	}

	auto iter = m_listChild.begin();
	for (_int i = 0; i < iIndex; ++i)
	{
		++iter;
	}

	return *iter;
}

// Pivot������ �ϱ����� Pivot�� �°� �����ǿ� ������ * 0.5f �� �� ���� �����ش�.
const _vec2 CUI_Object::Get_PivotAddPos(E_Pivot ePivot)
{
	_float fPivot_SizeX = (_float)m_tUiTransform.Get_SizeFloat(E_POSITION::POS_X) * 0.5f;
	_float fPivot_SizeY = (_float)m_tUiTransform.Get_SizeFloat(E_POSITION::POS_Y) * 0.5f;
	_vec2 vPos = _vec2(0.f, 0.f);
	switch (ePivot)
	{
	case E_Pivot::Pivot_TL:
		vPos.x += fPivot_SizeX;
		vPos.y -= fPivot_SizeY;
		break;
	case E_Pivot::Pivot_TC:
		vPos.x += 0;
		vPos.y -= fPivot_SizeY;
		break;
	case E_Pivot::Pivot_TR:
		vPos.x -= fPivot_SizeX;
		vPos.y -= fPivot_SizeY;
		break;
	case E_Pivot::Pivot_CL:
		vPos.x += fPivot_SizeX;
		vPos.y -= 0;
		break;
	case E_Pivot::Pivot_Center:
		vPos.x += 0;
		vPos.y += 0;
		break;
	case E_Pivot::Pivot_CR:
		vPos.x -= fPivot_SizeX;
		vPos.y += 0;
		break;
	case E_Pivot::Pivot_BL:
		vPos.x += fPivot_SizeX;
		vPos.y += fPivot_SizeY;
		break;
	case E_Pivot::Pivot_BC:
		vPos.x += 0;
		vPos.y += fPivot_SizeY;
		break;
	case E_Pivot::Pivot_BR:
		vPos.x -= fPivot_SizeX;
		vPos.y += fPivot_SizeY;
		break;
	default:
		assert(FALSE);
		break;
	}
	return vPos;
}

// �ڽ��� �������� ��ȯ�ϱ����� �Լ�, �ڽ��� ��ǥ�� Size * 0.5f�� �˸°� �� �� ���� ��ȯ���ش�.
const _int CUI_Object::Get_MyVoa(E_DIR_ANCHOR eDir)
{
	_int iReturn = 0;
	switch (eDir)
	{
	case Engine::E_DIR_ANCHOR::E_LEFT:
		iReturn = m_tUiTransform.iPosX - (_int)(m_tUiTransform.iSizeX * 0.5f); break;
	case Engine::E_DIR_ANCHOR::E_RIGHT:
		iReturn = m_tUiTransform.iPosX + (_int)(m_tUiTransform.iSizeX * 0.5f); break;
	case Engine::E_DIR_ANCHOR::E_TOP:
		iReturn = m_tUiTransform.iPosY + (_int)(m_tUiTransform.iSizeY * 0.5f); break;
	case Engine::E_DIR_ANCHOR::E_BOTTOM:
		iReturn = m_tUiTransform.iPosY - (_int)(m_tUiTransform.iSizeY * 0.5f); break;
	case Engine::E_DIR_ANCHOR::E_CENTER_X:
		return m_tUiTransform.iPosX; break;
	case Engine::E_DIR_ANCHOR::E_CENTER_Y:
		return m_tUiTransform.iPosY; break;
	default:						assert(FALSE);			break;
	}

	CUtility::Print_Wstring_DMode(L"Ui_Object - Get_MyVoa ����" + to_wstring(iReturn));
	return iReturn;
}

// �θ��� ������ ��ǥ�� ��ȯ���ִ� �Լ�
const _int CUI_Object::Get_ParentVoa(E_DIR_ANCHOR eDir)
{
	_vec2 vPos = VEC_ZERO;
	m_pParent->Get_ParentPos(vPos);
	if (nullptr == m_pParent)
	{
		switch (eDir)
		{
		case Engine::E_DIR_ANCHOR::E_LEFT:		return -(_int)(WINCX * 0.5f);
		case Engine::E_DIR_ANCHOR::E_RIGHT:		return +(_int)(WINCX * 0.5f);
		case Engine::E_DIR_ANCHOR::E_TOP:		return +(_int)(WINCY * 0.5f);
		case Engine::E_DIR_ANCHOR::E_BOTTOM:	return -(_int)(WINCY * 0.5f);
		case Engine::E_DIR_ANCHOR::E_CENTER_X:	return 0;
		case Engine::E_DIR_ANCHOR::E_CENTER_Y:	return 0;
		default:						assert(FALSE);			break;
		}
	}
	else
	{
		switch (eDir)
		{
		case Engine::E_DIR_ANCHOR::E_LEFT:		return (_int)vPos.x - (_int)(m_pParent->m_tUiTransform.iSizeX * 0.5f);
		case Engine::E_DIR_ANCHOR::E_RIGHT:		return (_int)vPos.x + (_int)(m_pParent->m_tUiTransform.iSizeX * 0.5f);
		case Engine::E_DIR_ANCHOR::E_TOP:		return (_int)vPos.y + (_int)(m_pParent->m_tUiTransform.iSizeY * 0.5f);
		case Engine::E_DIR_ANCHOR::E_BOTTOM:	return (_int)vPos.y - (_int)(m_pParent->m_tUiTransform.iSizeY * 0.5f);
		case Engine::E_DIR_ANCHOR::E_CENTER_X:	return (_int)vPos.x;
		case Engine::E_DIR_ANCHOR::E_CENTER_Y:	return (_int)vPos.y;
		default:						assert(FALSE);			break;
		}
	}

	CUtility::Print_Wstring_DMode(L"Ui_Object - Get_ParentVoa ���� ���� 0 ����");
	return 0;
}

// Ui�� Pivot�� ����� �������� �������ش�.
const _vec2 CUI_Object::Get_MyPivotAddPos()
{
	return Get_PivotAddPos(m_tUiTransform.eTransformPivot);
}

_bool CUI_Object::Get_IsScrollViewChild()
{
	if (m_bIsScrollView)
	{
		return TRUE;
	}

	if (m_pParent)
	{
		return m_pParent->Get_IsScrollViewChild();
	}

	return FALSE;
}

// ClickEvent�� �߰����ִ� �Լ�
void CUI_Object::Push_ClickEvent()
{
	if (m_vecClickEvent.empty())
		return;

	m_listClickEvent.push_back(m_vecClickEvent);
	m_listButtonEventType.push_back((E_UiButtonEventType)m_tUiTransform.iButtonEventType);
}

// �θ��� �������� �ڽ��� �������� �Ÿ� ���̸� ���ؼ� UV�� ������ Maskȿ���� ����.
void CUI_Object::Follow_ParentMaskUV()
{
	if (FALSE == m_bResetMask)
	{
		return;
	}
	_vec2 vMaskMin = _vec2(0.f, 0.f);
	_vec2 vMaskMax = _vec2(1.f, 1.f);

	if ((nullptr == m_pParent || FALSE == m_bMask))
	{
		Set_Mask(vMaskMin, vMaskMax);
		Set_PassNumber((_int)E_UI_ShaderPassType::Normal);
		return;
	}

	if (FALSE == m_bAfterPass)
	{
		if (m_bMask)
		{
			Set_PassNumber((_int)E_UI_ShaderPassType::Mask);
		}
		else
		{
			Set_PassNumber((_int)E_UI_ShaderPassType::ScrollView_Child);
		}
	}


	_vec2 vParent_Center = _vec2(0.f, 0.f);
	m_pParent->Get_ParentPos(vParent_Center);

	_vec2 vMy_Center = VEC_ZERO;
	Get_ParentPos(vMy_Center);

	_vec2 vParentMaskMin = _vec2(0.f, 0.f);
	_vec2 vParentMaskMax = _vec2(1.f, 1.f);
	m_pParent->Get_Mask(&vParentMaskMin, &vParentMaskMax);


	_float fParent_Left = vParent_Center.x - (m_pParent->Get_UiTransform().iSizeX * 0.5f) + ((vParentMaskMin.x) * m_pParent->Get_UiTransform().iSizeX);
	_float fParent_Right = vParent_Center.x + (m_pParent->Get_UiTransform().iSizeX * 0.5f) - ((1.f - vParentMaskMax.x) * m_pParent->Get_UiTransform().iSizeX);

	_float fDist_Left = fParent_Left - (vMy_Center.x - m_tUiTransform.iSizeX * 0.5f);
	if (fDist_Left >= 0)
	{
		vMaskMin.x = +1.f * fDist_Left / m_tUiTransform.iSizeX;
	}
	_float fDist_Right = fParent_Right - (vMy_Center.x + m_tUiTransform.iSizeX * 0.5f);
	if (fDist_Right <= 0)
	{
		vMaskMax.x = +1.f + (fDist_Right / m_tUiTransform.iSizeX);
	}

	_float fParent_Top = vParent_Center.y + (m_pParent->Get_UiTransform().iSizeY * 0.5f) - ((vParentMaskMin.y) * m_pParent->Get_UiTransform().iSizeY);
	_float fParent_Bottom = vParent_Center.y - (m_pParent->Get_UiTransform().iSizeY * 0.5f) + ((1.f - vParentMaskMax.y) * m_pParent->Get_UiTransform().iSizeY);

	_float fDist_Top = fParent_Top - (vMy_Center.y - m_tUiTransform.iSizeY * 0.5f);
	vMaskMin.y = CUtility::Clamp_float(((1.f - (fDist_Top / m_tUiTransform.iSizeY))), 0.f, 1.f);

	_float fDist_Bottom = fParent_Bottom - (vMy_Center.y + m_tUiTransform.iSizeY * 0.5f);
	vMaskMax.y = CUtility::Clamp_float((-1.f * (fDist_Bottom / m_tUiTransform.iSizeY)), 0.f, 1.f);

	Set_Mask(vMaskMin, vMaskMax);
}

// �Լ������� void�Լ� list�� �߰�
void CUI_Object::Add_Function_void(E_UiFunctionType eType, std::function<void(void)> Func)
{
	m_Funclist_void[(_int)eType].push_back(Func);

	if (m_Funclist_void[(_int)eType].size() >= 3)
	{
		CUtility::Print_Wstring_DMode(L"CUI_Object�� 1296��° �� assert �ɸ� : Function�� �ʱ�ȭ�����ʰ� ��� �ִ� ��� �߻� ����");
		assert(FALSE);
	}
}

// �Լ������� int�Լ� list�� �߰�
void CUI_Object::Add_Function_int(E_UiFunctionType eType, std::function<void(_int)> Func, _int iArg)
{
	m_Funclist_int[(_int)eType].push_back(make_pair(iArg, Func));
}

// �Լ������� float�Լ� list�� �߰�
void CUI_Object::Add_Function_float(E_UiFunctionType eType, std::function<void(_float)> Func, _float fArg)
{
	m_Funclist_float[(_int)eType].push_back(make_pair(fArg, Func));
}

// �Լ������� wstring�Լ� list�� �߰�
void CUI_Object::Add_Function_wstring(E_UiFunctionType eType, std::function<void(wstring)> Func, wstring wstrArg)
{
	m_Funclist_wstring[(_int)eType].push_back(make_pair(wstrArg, Func));
}

void CUI_Object::Add_Function_voidPtr(E_UiFunctionType eType, std::function<void(void*)> Func, void * pArg)
{
	m_Funclist_voidPtr[(_int)eType].push_back(make_pair(pArg, Func));
}

// eType�� ���� �Լ������͵��� �����Ѵ�.
void CUI_Object::Clear_Function(E_UiFunctionType eType)
{
	m_Funclist_void[(_int)eType].clear();
	m_Funclist_int[(_int)eType].clear();
	m_Funclist_float[(_int)eType].clear();
	m_Funclist_wstring[(_int)eType].clear();
	m_Funclist_voidPtr[(_int)eType].clear();
}

// eType�� �´� �Լ������͵��� �����Ѵ�.
void CUI_Object::Active_Function(E_UiFunctionType eType)
{
	for (auto& iter : m_Funclist_void[(_int)eType])
	{
		iter();
	}
	for (auto& iter : m_Funclist_int[(_int)eType])
	{
		iter.second(iter.first);
	}
	for (auto& iter : m_Funclist_float[(_int)eType])
	{
		iter.second(iter.first);
	}
	for (auto& iter : m_Funclist_wstring[(_int)eType])
	{
		iter.second(iter.first);
	}
	for (auto& iter : m_Funclist_voidPtr[(_int)eType])
	{
		iter.second(iter.first);
	}
}

// Enable�� �� �ڽĵ鵵 ��� ��������� Enable�� ����
// OnEnableAnimation�� True��� �ִϸ��̼��� �����Ѵ�.  Active_Function(OnEnable)�� �����Ѵ�.
void CUI_Object::OnEnable()
{
	m_bEnableThisFrame = TRUE;
	for (auto& iter : m_listChild)
	{
		iter->OnEnable();
	}

	if (m_bOnEnableAnimation)
	{
		Set_AnimStart(1.f);
		m_tUiAnimation.fProgNow = 0.f;
	}

	Active_Function(E_UiFunctionType::OnEnable);

	m_bFadeInOut = FALSE;
}

// OnDisable�� �� �ִϸ��̼��� �������̶�� ProgMax�� ����� �����Ų��.
void CUI_Object::OnDisable()
{
	m_tUiAnimation.fProgNow = m_tUiAnimation.fProgMax;
	Animation(m_tUiAnimation.fProgMax);
	m_tUiTransform.bAnimationState = FALSE;
	Active_Function(E_UiFunctionType::OnDisable);
	m_tUiAnimation.fProgNow = 0.f;
}

// ���콺�� �̹� �����ӿ� �浹�ϰ� �ִ��� üũ�ϴ� �Լ�
// True�� ��� OnMouse_ThisFrame()�� �����Ѵ�.
_bool CUI_Object::Is_OnMouse(POINT & ptMouse)
{
	if (TRUE == m_bMask &&
		(m_vMaskRatio_Min.x >= 1.f ||
			m_vMaskRatio_Min.y >= 1.f ||
			m_vMaskRatio_Max.x <= 0.f ||
			m_vMaskRatio_Max.y <= 0.f))
	{
		return FALSE;
	}

	_vec3 vPos = Get_Info(E_INFO::INFO_POS);

	ptMouse.x -= (LONG)(WINCX * 0.5f);
	ptMouse.y -= (LONG)(WINCY * 0.5f);
	ptMouse.y *= -1;
	_vec2 vParentPos = VEC_ZERO;


	if (m_bChildClickCheck)
	{
		list<CUI_Object*>::iterator iterCol = m_listChild.begin();
		for (; iterCol != m_listChild.end(); ++iterCol)
		{
			if (FALSE == (*iterCol)->Get_Active())
				continue;
			else if ((*iterCol)->Is_OnMouse(CUtility::Get_MousePoint(*m_pHwnd)))
			{
			}
		}
	}

	Get_ParentPos(vParentPos);
	LONG left = (LONG)(vParentPos.x - (m_tUiTransform.iSizeX * 0.5f));
	LONG right = (LONG)(vParentPos.x + (m_tUiTransform.iSizeX * 0.5f));
	LONG top = (LONG)(vParentPos.y + (m_tUiTransform.iSizeY * 0.5f));
	LONG bottom = (LONG)(vParentPos.y - (m_tUiTransform.iSizeY * 0.5f));
	if (ptMouse.x < left ||
		ptMouse.x > right ||
		ptMouse.y > top ||
		ptMouse.y < bottom)
	{
		m_bMouseOn = FALSE;
		return FALSE;
	}

	OnMouse_ThisFrame();
	m_bMouseOn = TRUE;
	return TRUE;
}

// Ŭ���Ǵ��� ���� ���콺 �浹 �Լ�  ���� ���� �ڽļӿ� �ִ� UI�� �浹�Ѵ�.
// ppOutDeepObject�� �浹�� �ڽ��� �־��ְ� LButton Down�� Up�� ���� ppOutDeepObject��� Ŭ���ߴٰ� �Ǵ��Ѵ�.
_bool CUI_Object::Is_OnMouse(POINT& ptMouse, CUI_Object** ppOutDeepObject, _int* refMostDeep, _int iDepth)
{
	if (TRUE == m_bMask &&
		(m_vMaskRatio_Min.x >= 1.f ||
			m_vMaskRatio_Min.y >= 1.f ||
			m_vMaskRatio_Max.x <= 0.f ||
			m_vMaskRatio_Max.y <= 0.f))
	{
		return FALSE;
	}
	_vec3 vPos = Get_Info(E_INFO::INFO_POS);

	ptMouse.x -= (LONG)(WINCX * 0.5f);
	ptMouse.y -= (LONG)(WINCY * 0.5f);
	ptMouse.y *= -1;
	_vec2 vParentPos = VEC_ZERO;


	if (m_bChildClickCheck)
	{
		list<CUI_Object*>::iterator iterCol = m_listChild.begin();
		for (; iterCol != m_listChild.end(); ++iterCol)
		{
			if (FALSE == (*iterCol)->Get_Active())
				continue;
			else if ((*iterCol)->Is_OnMouse(CUtility::Get_MousePoint(*m_pHwnd), ppOutDeepObject, refMostDeep, iDepth + 1))
			{
				if (ppOutDeepObject)
				{
					if (*refMostDeep <= iDepth)
					{
						*ppOutDeepObject = this;
						*refMostDeep = iDepth;
					}
				}
			}
		}
	}

	Get_ParentPos(vParentPos);
	LONG left = (LONG)(vParentPos.x - (m_tUiTransform.iSizeX * 0.5f));
	LONG right = (LONG)(vParentPos.x + (m_tUiTransform.iSizeX * 0.5f));
	LONG top = (LONG)(vParentPos.y + (m_tUiTransform.iSizeY * 0.5f));
	LONG bottom = (LONG)(vParentPos.y - (m_tUiTransform.iSizeY * 0.5f));
	if (ptMouse.x < left ||
		ptMouse.x > right ||
		ptMouse.y > top ||
		ptMouse.y < bottom)
	{
		m_bMouseOn = FALSE;
		return FALSE;
	}

	if (ppOutDeepObject)
	{
		if (*refMostDeep <= iDepth)
		{
			*ppOutDeepObject = this;
			*refMostDeep = iDepth;
		}
	}
	return TRUE;
}

// ����� ����. UI_Scroll_View���� ��ũ���� ���� ���
void CUI_Object::OnMouse_ThisFrame()
{
}

// Ui�� Ŭ������ �� ȣ��Ǵ� �Լ�
void CUI_Object::OnClick()
{
	m_bOnOff = !m_bOnOff;

	list<E_UiButtonEventType>::iterator iterEventType = m_listButtonEventType.begin();
	for (auto& iter : m_listClickEvent)
	{
		_int iMax = 0;
		CUI_Object* pUi = Get_EventObject(iter, iMax, 0);
		if (pUi)
		{
			if (E_UiButtonEventType::E_FALSE == *iterEventType)
			{
				CUtility::Print_Wstring_DMode(L"False �̺�Ʈ");
				pUi->Set_Active(FALSE);
			}
			else if (E_UiButtonEventType::E_TRUE == *iterEventType)
			{
				CUtility::Print_Wstring_DMode(L"True �̺�Ʈ");
				pUi->Set_Active(TRUE);
			}
			else if (E_UiButtonEventType::E_REFLECT == *iterEventType)
			{
				CUtility::Print_Wstring_DMode(L"Reflect �̺�Ʈ");
				pUi->Set_Active(!pUi->Get_Active());
			}
		}
		++iterEventType;
	}


	if (m_bOnClickAnimation)
	{
		Set_AnimStart(1.f);
	}

	Active_Function(E_UiFunctionType::OnClick);
	if (E_UiAnimationType::UiAnim_End == m_tUiAnimation.eAnimType)
	{

	}
}

// Ʈ���ſ����� ���۵� �Լ� : ���� �̻��
void CUI_Object::OnTrigger()
{

}

// �� ������ ����Ǵ� �ִϸ��̼� �Լ�
// m_tUiAnimation.eAnimType Ÿ�Ժ��� �ִϸ��̼��� �ٸ��� ����ȴ�.
void CUI_Object::Animation(const _float& fTimeDelta)
{
	if (FALSE == m_tUiTransform.bAnimationState)
		return;

	_float fRatio = m_tUiAnimation.fProgNow / m_tUiAnimation.fProgMax;
	fRatio = CUtility::LerpEase_float(m_tUiTransform.eEase, fRatio);
	_float fParamFloat1Ratio = m_tUiAnimation.float_Parameter_1 + (fRatio * (1.f - m_tUiAnimation.float_Parameter_1));
	m_tUiAnimation.fProgNow += fTimeDelta;

	switch (m_tUiAnimation.eAnimType)
	{
	case Engine::E_UiAnimationType::ZeroToOne_Vert:
		m_tUiTransform.fSizeRatioX = fParamFloat1Ratio;
		m_tUiTransform.fSizeRatioY = 1.f;
		break;
	case Engine::E_UiAnimationType::ZeroToOne_Hori:
		m_tUiTransform.fSizeRatioX = 1.f;
		m_tUiTransform.fSizeRatioY = fParamFloat1Ratio;
		break;
	case Engine::E_UiAnimationType::ZeroTo_Cross:
		m_tUiTransform.fSizeRatioX = fParamFloat1Ratio;
		m_tUiTransform.fSizeRatioY = fParamFloat1Ratio;
		break;
	case Engine::E_UiAnimationType::Vive_Vert:
	{
		_int iViveTime = m_tUiAnimation.int_Parameter_1;
		_int iViveLength = m_tUiAnimation.int_Parameter_2;
		_int iDir = (_int)((fRatio) * 100.f) % iViveTime;

		// ����
		if (iDir >= (_float)iViveTime / 2)
		{
			m_tUiAnimation.vAnimPosition.x -=
				iViveLength / m_tUiAnimation.fProgMax * fTimeDelta * m_tUiAnimation.iRandDir;
		}
		// ������
		else
		{
			m_tUiAnimation.vAnimPosition.x +=
				iViveLength / m_tUiAnimation.fProgMax * fTimeDelta * m_tUiAnimation.iRandDir;
		}

		break;
	}
	case Engine::E_UiAnimationType::Vive_Hori:
	{
		_int iViveTime = m_tUiAnimation.int_Parameter_1;
		_int iViveLength = m_tUiAnimation.int_Parameter_2;
		_int iDir = (_int)((fRatio) * 100.f) % iViveTime;

		// ��
		if (iDir >= (_float)iViveTime / 2)
		{
			m_tUiAnimation.vAnimPosition.y -=
				iViveLength / m_tUiAnimation.fProgMax * fTimeDelta * m_tUiAnimation.iRandDir;
		}
		// �Ʒ�
		else
		{
			m_tUiAnimation.vAnimPosition.y +=
				iViveLength / m_tUiAnimation.fProgMax * fTimeDelta * m_tUiAnimation.iRandDir;
		}

		break;
	}
	case Engine::E_UiAnimationType::Left_To_Origin:
	{
		m_tUiTransform.iPosX = (_int)CUtility::Lerp_vec2(m_vAnimBeginPos, m_vAnimEndPos, fParamFloat1Ratio).x;
		break;
	}
	case Engine::E_UiAnimationType::Right_To_Origin:
	{
		m_tUiTransform.iPosX = (_int)CUtility::Lerp_vec2(m_vAnimBeginPos, m_vAnimEndPos, fParamFloat1Ratio).x;
		break;
	}
	case Engine::E_UiAnimationType::Top_To_Origin:
	{
		m_tUiTransform.iPosY = (_int)CUtility::Lerp_vec2(m_vAnimBeginPos, m_vAnimEndPos, fParamFloat1Ratio).y;
		break;
	}
	case Engine::E_UiAnimationType::Bottom_To_Origin:
	{
		m_tUiTransform.iPosY = (_int)CUtility::Lerp_vec2(m_vAnimBeginPos, m_vAnimEndPos, fParamFloat1Ratio).y;
		break;
	}
	case Engine::E_UiAnimationType::UiAnim_End:
		break;
	default:
		break;
	}

	if (m_tUiAnimation.fProgMax <= m_tUiAnimation.fProgNow)
	{
		m_tUiTransform.fSizeRatioX = 1.f;
		m_tUiTransform.fSizeRatioY = 1.f;
		m_tUiAnimation.vAnimPosition = _vec2(0.f, 0.f);
		m_tUiAnimation.fProgNow = 0.f;
		m_tUiTransform.bAnimationState = FALSE;

		switch (m_tUiAnimation.eAnimType)
		{
		case Engine::E_UiAnimationType::Left_To_Origin:
		case Engine::E_UiAnimationType::Right_To_Origin:
		case Engine::E_UiAnimationType::Top_To_Origin:
		case Engine::E_UiAnimationType::Bottom_To_Origin:
			m_tUiTransform.iPosX = (_int)m_vAnimEndPos.x;
			m_tUiTransform.iPosY = (_int)m_vAnimEndPos.y;
			break;
		case Engine::E_UiAnimationType::UiAnim_End:
			break;
		default:
			break;
		}

		if (E_UiAnimationType::UiAnim_End != m_tUiAnimation.eAnimType)
		{
			// Active_Function(E_UiFunctionType::OnClick);
		}
	}
}

// RectBuffer, Shader�� ������Ʈ�� �����Ѵ�.
// Texture�� Get_Texture�� ���� ProgoMgr���� �����´�.
void CUI_Object::Add_Component()
{
	// RectBuffer ����
	// �⺻ ������ -0.5f, 0.5f
	m_pRectBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));

	// �ӽ� �ؽ��� ����
	Set_Tex1_Tag(L"../../Reference/Resource/Texture/Test/TexForTest.png");

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Ui")));
}

// �˸��� ���ڸ� ������̺� �Ѱ��ش�.
// m_iPassNumber�� ���� �߰��� �Ѱ��ִ� ������̺��� �ִ�. (g_vMaskUVMin, g_vMaskUVMax, g_MaskTexture)
void CUI_Object::Set_ConstantTable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// �� �� �� �� ��
	_matrix		matWorld;
	D3DXMatrixIdentity(&matWorld);
	if (m_tUiAnimation.bMyChildFollowMyAnim && m_pParent)
	{
		D3DXMatrixScaling(&matWorld, m_pParent->Get_UiTransform().fSizeRatioX, m_pParent->Get_UiTransform().fSizeRatioY, 1.f);
	}
	if (nullptr == m_pParent)
	{
		matWorld *= Get_Transform()->Get_WorldMatrix();
	}
	else
	{
		_vec2 vPos = VEC_ZERO;
		Get_ParentPos(vPos);
		matWorld *= Get_Transform()->Get_WorldMatrix();
	}
	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	if (m_pTexture_1)
	{
		pEffect->SetTexture("g_BaseTexture", m_pTexture_1);
	}
	else
	{
		Get_Tex1_FromTextureMgr();
	}

	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
	pEffect->SetFloatArray("g_vImageColor", (float*)m_ColorShaderOption, 4);

	pEffect->SetFloatArray("g_vMaskUVMin", (float*)m_vMaskRatio_Min, 2);
	pEffect->SetFloatArray("g_vMaskUVMax", (float*)m_vMaskRatio_Max, 2);

	if (m_pParent && FALSE == m_bAfterPass)
	{
		pEffect->SetTexture("g_MaskTexture", m_pParent->Get_Texture_1());
	}

	if ((_int)E_UI_ShaderPassType::Normal == m_iPassNumber)
	{

	}
	else if ((_int)E_UI_ShaderPassType::Mask == m_iPassNumber)
	{
	}
 	else if ((_int)E_UI_ShaderPassType::ScrollView_Child == m_iPassNumber)
	{
	}
	else if ((_int)E_UI_ShaderPassType::FillCircle == m_iPassNumber)
	{
		_vec2 vFill = VEC2_ZERO;
		vFill.x = cosf((m_fAngle) * Deg2Rad);
		vFill.y = -sinf((m_fAngle) * Deg2Rad);
		pEffect->SetFloatArray("g_vFillCircle", (float*)vFill, 2);

		pEffect->SetFloat("g_fFillRatio", CUtility::Clamp_float(m_fAngle, 0.f, 360.f));
	}
}

void CUI_Object::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"UI�Դϴ�");
}

void CUI_Object::FadeInOut(const _float & fTimeDelta)
{
	if (m_bFadeInOut)
	{
		switch (m_eFadeType)
		{
		case Engine::E_FADETYPE::FADETYPE_IN: 
			m_fFadeCountTime += fTimeDelta;
			m_ColorShaderOption.a = m_fFadeCountTime / m_fFadeEndTime; 
			m_tTextOption.tColor.a = m_fFadeCountTime / m_fFadeEndTime; 

			if (1.f <= m_ColorShaderOption.a)
			{
				m_bFadeInOut = FALSE; 
				m_fFadeCountTime = 0.f;
				m_fFadeEndTime = 0.f;
			}
			break;

		case Engine::E_FADETYPE::FADETYPE_OUT:
			m_fFadeCountTime -= fTimeDelta;
			m_ColorShaderOption.a = m_fFadeCountTime / m_fFadeEndTime;
			m_tTextOption.tColor.a = m_fFadeCountTime / m_fFadeEndTime;

			if (0.f >= m_ColorShaderOption.a)
			{
				m_bFadeInOut = FALSE;
				m_fFadeCountTime = 0.f;
				m_fFadeEndTime = 0.f;
			}
			break;

		case Engine::E_FADETYPE::FADETYPE_INOUT: 
			break;

		default:
			assert(0); 
			break;
		}
	}
}

// �ؽ�Ʈ Voa�� �´� ��ǥ�� ��ȯ�ϱ� ���� �뵵�� Voa�Լ�
_vec2 CUI_Object::Get_VertexOfAngle(E_VertexOfAngle eVoa)
{
	_vec2 vCenter = _vec2(Get_Info(E_INFO::INFO_POS).x, -Get_Info(E_INFO::INFO_POS).y);

	_float fSizeX = (_float)m_tUiTransform.iSizeX * 0.5f;
	_float fSizeY = (_float)m_tUiTransform.iSizeY * 0.5f;
	switch (eVoa)
	{
	case Engine::E_VertexOfAngle::TL:
		vCenter.x -= fSizeX;
		vCenter.y -= fSizeY;
		break;
	case Engine::E_VertexOfAngle::TC:
		vCenter.x += 0;
		vCenter.y -= fSizeY;
		break;
	case Engine::E_VertexOfAngle::TR:
		vCenter.x += fSizeX;
		vCenter.y -= fSizeY;
		break;
	case Engine::E_VertexOfAngle::CL:
		vCenter.x -= fSizeX;
		vCenter.y += 0;
		break;
	case Engine::E_VertexOfAngle::C:
		vCenter.x += 0;
		vCenter.y += 0;
		break;
	case Engine::E_VertexOfAngle::CR:
		vCenter.x += fSizeX;
		vCenter.y += 0;
		break;
	case Engine::E_VertexOfAngle::BL:
		vCenter.x -= fSizeX;
		vCenter.y += fSizeY;
		break;
	case Engine::E_VertexOfAngle::BC:
		vCenter.x += 0;
		vCenter.y += fSizeY;
		break;
	case Engine::E_VertexOfAngle::BR:
		vCenter.x += fSizeX;
		vCenter.y += fSizeY;
		break;
	default:
		assert(FALSE);
		break;
	}
	return vCenter;
}
