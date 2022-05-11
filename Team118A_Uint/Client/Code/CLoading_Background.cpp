#include "stdafx.h"
#include "../Include/stdafx.h"
#include "CLoading_Background.h"

CLoading_Background::CLoading_Background(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CLoading_Background::CLoading_Background(const CLoading_Background & rhs)
	: CGameObject(rhs)
{
}

CLoading_Background::~CLoading_Background()
{
}

CLoading_Background * CLoading_Background::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CLoading_Background* pInstance = new CLoading_Background(pGraphicDev, eObjTag);

	if (pInstance->Ready_Object())
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Background::Free()
{
	while (!m_qImageStruct.empty())
	{
		Safe_Delete(m_qImageStruct.front());
		m_qImageStruct.pop();
	}
	CGameObject::Free();
}

HRESULT CLoading_Background::Ready_Object()
{
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	Add_Component();

	return S_OK;
}

_int CLoading_Background::Start_Object(const _float & fTimeDelta)
{
	return CGameObject::Start_Object(fTimeDelta);
}

_int CLoading_Background::Update_Object(const _float & fTimeDelta)
{
	_float fLoadingRatio = g_iLoadingGage / 3200.f;
	Check_Image(fTimeDelta);

	_float fPosX = 0.f;
	_float fPosY = 0.f;

	_float fRatio = 1.f - (m_qImageStruct.front()->fTiming / m_qImageStruct.front()->fMaxTime);
	fRatio *= m_qImageStruct.front()->fRatio;
	switch (m_eAnchor)
	{
	case Engine::E_Anchor::Anchor_TL:
		fPosX = CUtility::Lerp_float(-WINCX * m_fExpansion, WINCX * m_fExpansion, fRatio);
		break;
	case Engine::E_Anchor::Anchor_TC:
		break;
	case Engine::E_Anchor::Anchor_TR:
		break;
	case Engine::E_Anchor::Anchor_CL:
		fPosX = CUtility::Lerp_float(-WINCX * m_fExpansion, WINCX * m_fExpansion, fRatio);
		break;
	case Engine::E_Anchor::Anchor_Center:
		m_fSizeX += WINCX * 0.1f * fTimeDelta;
		m_fSizeY += WINCY * 0.1f * fTimeDelta;
		break;
	case Engine::E_Anchor::Anchor_CR:
		fPosX = CUtility::Lerp_float(-WINCX * m_fExpansion, WINCX * m_fExpansion, fRatio);
		break;
	case Engine::E_Anchor::Anchor_BL:
		fPosX = CUtility::Lerp_float(-WINCX * m_fExpansion, WINCX * m_fExpansion, fRatio);
		break;
	case Engine::E_Anchor::Anchor_BC:
		break;
	case Engine::E_Anchor::Anchor_BR:
		fPosX = CUtility::Lerp_float(+WINCX * m_fExpansion, WINCX * m_fExpansion, fRatio);
		break;
	}

	m_pBackground->Set_Ui_Pos(_vec2(fPosX, fPosY) * 0.5f);

	m_pBackground->Set_Ui_SizeX((_int)m_fSizeX);
	m_pBackground->Set_Ui_SizeY((_int)m_fSizeY);

	_int iLoadingPercent = (_int)(g_iLoadingGage / 33.f);
	iLoadingPercent = CUtility::Clamp_int(iLoadingPercent, 0, 100);

	for (_int i = 0; i < 5; ++i)
	{
		m_pLoadingText_OutLine->Get_ChildByIndex(i)->Set_Ui_Text(L"게임 데이터를 초기화 중입니다 ㆍㆍㆍ [Data Initialize..." + to_wstring(iLoadingPercent) + L"%]");
	}

	m_pLogo->Set_Mask(_vec2(0.f, 0.f), _vec2(fLoadingRatio, 1.f));
	return CGameObject::Update_Object(fTimeDelta);
}

_int CLoading_Background::LateUpdate_Object(const _float & fTimeDelta)
{
	return CGameObject::LateUpdate_Object(fTimeDelta);
}

void CLoading_Background::Render_Object(_int iPass)
{

}

void CLoading_Background::Add_Component()
{
	m_pBackground = CUI_Object::Create(m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
	m_pBackground->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Loading_Animation/1.jpg");
	m_pBackground->Set_Order_In_Layer(9999);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pBackground);


	m_fSizeX = WINCX * (1.f + m_fExpansion);
	m_fSizeY = WINCY * (1.f + m_fExpansion);
	m_pBackground->Set_Ui_SizeX((_int)m_fSizeX);
	m_pBackground->Set_Ui_SizeY((_int)m_fSizeY);

	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/1.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/2.jpg", E_Anchor::Anchor_Center, 0.3f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/3.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/4.jpg", E_Anchor::Anchor_CR, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/5.jpg", E_Anchor::Anchor_Center, 0.3f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/6.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/7.jpg", E_Anchor::Anchor_CR, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/8.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/9.jpg", E_Anchor::Anchor_CR, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/10.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/11.jpg", E_Anchor::Anchor_CR, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/12.jpg", E_Anchor::Anchor_CL, 0.6f));
	m_qImageStruct.push(new T_LoadingImages(2.5f, L"../../Reference/Resource/Texture/UI/Loading_Animation/13.jpg", E_Anchor::Anchor_CR, 0.6f));

	m_qImageStruct.front()->fTiming = -0.1f;

	m_p15Age = CUI_Object::Create(m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
	m_p15Age->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/BaseImage/15세이용가.png");
	m_p15Age->Set_Order_In_Layer(10000);
	m_p15Age->Set_Ui_Pos(_vec2(550.f, -280.f));
	m_p15Age->Set_Ui_SizeX(60);
	m_p15Age->Set_Ui_SizeY(73);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_p15Age);

	m_pLoadingText_OutLine = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Font_OutLine.uidat");
	m_pLoadingText_OutLine->Set_ImageAlpha(0.f);
	m_pLoadingText_OutLine->Set_Order_In_Layer(10001);
	m_pLoadingText_OutLine->Set_Ui_Pos(_vec2(-300.f , -300.f));
	for (_int i = 0; i < 5; ++i)
	{
		m_pLoadingText_OutLine->Get_ChildByIndex(i)->Set_TextAlign(E_TextAlign::Left);
		m_pLoadingText_OutLine->Get_ChildByIndex(i)->Set_Ui_FontSize(12);
		m_pLoadingText_OutLine->Get_ChildByIndex(i)->Set_Ui_Text(L"게임 데이터를 초기화 중입니다 ㆍㆍㆍ");
	}
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pLoadingText_OutLine);


	m_pLogo = CUI_Object::Create(m_pGraphicDev, &g_hWnd, E_OBJTAG::OBJ_UI);
	m_pLogo->Set_Tex1_Tag(L"../../Reference/Resource/Texture/Logo/Logo_Kor.png");
	m_pLogo->Set_Ui_Pos(_vec2(-460.f, 280.f));
	m_pLogo->Set_Ui_SizeX(300);
	m_pLogo->Set_Ui_SizeY(128);
	m_pLogo->Set_Order_In_Layer(10002);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::UI, E_OBJTAG::OBJ_UI, m_pLogo);
	m_pLogo->Set_ResetMask(FALSE);
}

void CLoading_Background::Check_Image(const _float& fTimeDelta)
{
	T_LoadingImages* pImageStruct = m_qImageStruct.front();

	pImageStruct->fTiming -= fTimeDelta;

	if (pImageStruct->fTiming < 0.f)
	{
		pImageStruct->Reset();
		m_qImageStruct.pop();
		m_qImageStruct.push(pImageStruct);
		m_eAnchor = m_qImageStruct.front()->eStartAnchor;


		_float fPosX = WINCX * m_fExpansion;
		_float fPosY = WINCY * m_fExpansion;

		m_fSizeX = WINCX * (1.f + m_fExpansion);
		m_fSizeY = WINCY * (1.f + m_fExpansion);

		switch (m_eAnchor)
		{
		case Engine::E_Anchor::Anchor_TL:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(-fPosX, fPosY));		break;
		case Engine::E_Anchor::Anchor_TC:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(0.f, fPosY));		break;
		case Engine::E_Anchor::Anchor_TR:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(fPosX, fPosY));		break;
		case Engine::E_Anchor::Anchor_CL:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(-fPosX, 0.f));		break;
		case Engine::E_Anchor::Anchor_Center:	
			m_pBackground->Set_Ui_Pos(0.5f * _vec2(0.f, 0.f));	
			m_fSizeX = WINCX * 1.f;	
			m_fSizeY = WINCY * 1.f;  
			break;
		case Engine::E_Anchor::Anchor_CR:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(fPosX, 0.f));		break;
		case Engine::E_Anchor::Anchor_BL:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(-fPosX, -fPosY));	break;
		case Engine::E_Anchor::Anchor_BC:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(0.f, -fPosY));		break;
		case Engine::E_Anchor::Anchor_BR:		m_pBackground->Set_Ui_Pos(0.5f * _vec2(fPosX, -fPosY));		break;
		}

		m_pBackground->Set_Ui_SizeX((_int)m_fSizeX);
		m_pBackground->Set_Ui_SizeY((_int)m_fSizeY);

		m_pBackground->Set_Tex1_Tag(m_qImageStruct.front()->wstrFullPath);
	}

}
