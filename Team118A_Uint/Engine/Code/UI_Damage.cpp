#include "UI_Damage.h"

#include "ProtoMgr.h"
#include "Renderer.h"

#include "RectBuffer.h"


CUI_Damage::CUI_Damage(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag, _float fLifeTotal, E_NumberFontType eNumberType, _int iNumber, _bool bCritical)
	: CUI_Object(pGraphicDev, pHwnd, eObjTag)
{
	m_fLifeTotal = fLifeTotal;
	m_fLifeTime = fLifeTotal;

	m_eNumberType = eNumberType;
	m_iNumber = iNumber;
	m_bCritical = bCritical;
}

CUI_Damage::CUI_Damage(const CGameObject & rhs)
	: CUI_Object(rhs)
{

}

CUI_Damage::~CUI_Damage()
{
}

CUI_Damage * CUI_Damage::Create(LPDIRECT3DDEVICE9 pGraphicDev, HWND * pHwnd, E_OBJTAG eObjTag, _float fLifeTotal, E_NumberFontType eNumberType, _int iNumber, _bool bCritical)
{
	CUI_Damage* pInstance = new CUI_Damage(pGraphicDev, pHwnd, eObjTag, fLifeTotal, eNumberType, iNumber, bCritical);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage::Free()
{
	CUI_Object::Free();
}

void CUI_Damage::Save(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Save(hFile, dwByte);

	CUtility::Data_WriteInt(hFile, (_int)m_eNumberType, dwByte);
}

void CUI_Damage::Load(HANDLE hFile, DWORD & dwByte)
{
	CUI_Object::Load(hFile, dwByte);

	_int iNumberType = 0;
	CUtility::Data_ReadInt(hFile, iNumberType, dwByte);
	m_eNumberType = (E_NumberFontType)iNumberType;
}

CUI_Damage * CUI_Damage::Clone()
{
	return new CUI_Damage(*this);
}

HRESULT CUI_Damage::Ready_Object()
{
	CGameObject::Ready_Object();
	m_wstrUITag = L"CUI_Damage";

	m_fLifeTime = m_fLifeTotal;

	if (m_bCritical)
	{
		m_fFontSizeRT = 1.3f;
	}
	if (E_NumberFontType::Resist == m_eNumberType)
	{
		m_fFontSizeRT = 0.7f;
	}

	Add_Component();

	return S_OK;
}

_int CUI_Damage::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CUI_Object::Start_Object(fTimeDelta);

	return iExit;
}

_int CUI_Damage::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	Animation(fTimeDelta);
	m_fLifeTime -= fTimeDelta;

	m_ColorShaderOption.a = 0.3f + (m_fLifeTime / m_fLifeTotal) * 2.f;
	if (0.f > m_fLifeTime)
	{
		Set_Dead(TRUE);
	}

	return iExit;
}

_int CUI_Damage::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	if (m_tUiTransform.bUseText)
	{
		m_iNumber = _wtoi(m_wText.c_str());
	}

	Set_Scale(m_tUiTransform.Get_SizeFloat(E_POSITION::POS_X) * m_fFontSizeRT, m_tUiTransform.Get_SizeFloat(E_POSITION::POS_Y) * m_fFontSizeRT, 1.f);
	_vec2 vPos = VEC_ZERO;

	Get_ParentPos(vPos);

	if (m_tUiTransform.bAnimationState)
	{
		vPos += m_tUiAnimation.vAnimPosition;
	}
	Set_Pos(_vec3(vPos.x, vPos.y, (_float)m_tUiTransform.iPosZ));


	if (nullptr == m_pParent)
	{
		CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_UI, this);
	}

	return iExit;
}

void CUI_Damage::Render_Object(_int iPass)
{
	if (TRUE == m_bMask &&
		(m_vMaskRatio_Min.x >= 1.f ||
			m_vMaskRatio_Min.y >= 1.f ||
			m_vMaskRatio_Max.x <= 0.f ||
			m_vMaskRatio_Max.y <= 0.f))
	{
		return;
	}

	if (CRenderer::GetInstance()->Get_CutIn())
		return;

	if ((m_bBlock || m_bImmune || m_bMiss) && !m_tUiTransform.bUseText)
	{
		Set_ConstantTable_ImmuneBlockMiss(0);
		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);

		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();
		return;
	}

	_int iCopyNumber = m_iNumber;
	_int iDemical = 0;
	list<_int> listNum;
	while (iCopyNumber > 10)
	{
		iDemical++;
		listNum.push_front(iCopyNumber % 10);
		iCopyNumber /= 10;
	}
	listNum.push_front(iCopyNumber % 10);

	if (listNum.empty())
		return;

	_int iMax = (_int)listNum.size();

	m_iPassNumber = (_int)E_UI_ShaderPassType::Normal;
	if (m_bCritical)
	{
		Set_ConstantTable_Critical(iMax);
		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);
	
		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();
	
	}

	if (E_NumberFontType::Resist == m_eNumberType && !m_tUiTransform.bUseText)
	{
		Set_ConstantTable_Resist(iMax);
		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);

		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();

	}
	else if (E_NumberFontType::Weak == m_eNumberType && !m_tUiTransform.bUseText)
	{
		Set_ConstantTable_Weak(iMax);
		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);

		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();

	}

	_int iCount = 0;
	for(auto& iter : listNum)
	{
		Set_ConstantTable(iMax, iCount, iter);

		m_pShaderCom->BeginShader();
		m_pShaderCom->BeginPass(m_iPassNumber);
		m_pRectBufferCom->Render_Buffer();
		m_pShaderCom->EndPass();
		m_pShaderCom->EndShader();
		++iCount;
	}

	m_bStart = FALSE;
}

void CUI_Damage::Child_Render_Object()
{
	Follow_ParentMaskUV();

	if (FALSE == m_bStart)
	{
		Render_Object();
	}
	m_bStart = FALSE;
}

void CUI_Damage::Debug_This_In_Console()
{
	CUtility::Print_Wstring_DMode(L"데미지입니다.");
}

void CUI_Damage::Start_Font_Animation()
{
	if (TRUE == m_tUiTransform.bAnimationState)
		return;

	m_tUiTransform.bAnimationState = TRUE;
}

void CUI_Damage::Animation(const _float & fTimeDelta)
{
	_float fRatioX = CUtility::LerpEase_float(E_EASE::OutQuart, 1.f - (m_fLifeTime / m_fLifeTotal));
	_float fRatioY = CUtility::LerpEase_float(E_EASE::OutBack,	1.f - (m_fLifeTime / m_fLifeTotal));
	m_tUiAnimation.vAnimPosition.x = m_tUiTransform.iSizeX * fRatioX;
	m_tUiAnimation.vAnimPosition.y = m_tUiTransform.iSizeY * 0.7f * fRatioY;
}

void CUI_Damage::Add_Component()
{
	// RectBuffer 세팅
	m_pRectBufferCom = static_cast<CRectBuffer*>(Emplace_Component(ComTag_RectBuffer, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex")));

	// 임시 텍스쳐 설정
	wstrFullPath[(_int)E_NumberFontType::White] = L"../../Reference/Resource/Texture/UI/Font/D_Font.png";
	wstrFullPath[(_int)E_NumberFontType::Mini] = L"../../Reference/Resource/Texture/UI/Font/D_Font_Mini.png";
	wstrFullPath[(_int)E_NumberFontType::Normal] = L"../../Reference/Resource/Texture/UI/Font/D_Font_Normal.png";
	wstrFullPath[(_int)E_NumberFontType::Resist] = L"../../Reference/Resource/Texture/UI/Font/D_Font_Resist.png";
	wstrFullPath[(_int)E_NumberFontType::Weak] = L"../../Reference/Resource/Texture/UI/Font/D_Font_Weak.png";

	_int iCount = (_int)E_NumberFontType::White;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrFullPath[iCount], &m_pTexture_Font[iCount]);   iCount++;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrFullPath[iCount], &m_pTexture_Font[iCount]);	iCount++;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrFullPath[iCount], &m_pTexture_Font[iCount]);	iCount++;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrFullPath[iCount], &m_pTexture_Font[iCount]);	iCount++;
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, wstrFullPath[iCount], &m_pTexture_Font[iCount]);	iCount++;

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/Critical_Back.png", &m_pTexture_Critical);
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/ImgFont_Weak.png", &m_pTexture_Weak);
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/ImgFont_Resist.png", &m_pTexture_Resist);

	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/Imagefont_Immune_Unit.png", &m_pTexture_Immune);
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/Imagefont_Miss_Unit.png", &m_pTexture_Miss);
	CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Texture/UI/Font/Imagefont_Block.png", &m_pTexture_Block);

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_Ui")));
}

void CUI_Damage::Set_ConstantTable(_int iMax, _int iCount, _int iNum)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// 스 자 이 공 부
	_matrix		matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
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
	matWorld._41 += -((iMax * 0.5f) - iCount) * m_tUiTransform.iSizeX * 0.9f * m_fFontSizeRT + m_tUiAnimation.vAnimPosition.x;
	matWorld._42 += m_tUiAnimation.vAnimPosition.y;
	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);
	pEffect->SetFloatArray("g_vImageColor", (float*)m_ColorShaderOption, 4);

	pEffect->SetTexture("g_BaseTexture", m_pTexture_Font[(_int)m_eNumberType]);

	switch (iNum)
	{
	case 0:
		m_tUiTransform.vDrawUvMin.x = 0.0f; m_tUiTransform.vDrawUvMax.x = 0.21f; m_tUiTransform.vDrawUvMin.y = 0.0f; m_tUiTransform.vDrawUvMax.y = 0.5f;
		break;
	case 1:
		m_tUiTransform.vDrawUvMin.x = 0.21f; m_tUiTransform.vDrawUvMax.x = 0.4f; m_tUiTransform.vDrawUvMin.y = 0.0f; m_tUiTransform.vDrawUvMax.y = 0.5f;
		break;
	case 2:
		m_tUiTransform.vDrawUvMin.x = 0.4f; m_tUiTransform.vDrawUvMax.x = 0.6f; m_tUiTransform.vDrawUvMin.y = 0.0f; m_tUiTransform.vDrawUvMax.y = 0.5f;
		break;
	case 3:
		m_tUiTransform.vDrawUvMin.x = 0.6f; m_tUiTransform.vDrawUvMax.x = 0.8f; m_tUiTransform.vDrawUvMin.y = 0.0f; m_tUiTransform.vDrawUvMax.y = 0.5f;
		break;
	case 4:
		m_tUiTransform.vDrawUvMin.x = 0.8f; m_tUiTransform.vDrawUvMax.x = 1.0f; m_tUiTransform.vDrawUvMin.y = 0.0f; m_tUiTransform.vDrawUvMax.y = 0.5f;
		break;
	case 5:
		m_tUiTransform.vDrawUvMin.x = 0.0f; m_tUiTransform.vDrawUvMax.x = 0.195f; m_tUiTransform.vDrawUvMin.y = 0.5f; m_tUiTransform.vDrawUvMax.y = 1.0f;
		break;
	case 6:
		m_tUiTransform.vDrawUvMin.x = 0.20f; m_tUiTransform.vDrawUvMax.x = 0.4f; m_tUiTransform.vDrawUvMin.y = 0.5f; m_tUiTransform.vDrawUvMax.y = 1.0f;
		break;	
	case 7:
		m_tUiTransform.vDrawUvMin.x = 0.41f; m_tUiTransform.vDrawUvMax.x = 0.6f; m_tUiTransform.vDrawUvMin.y = 0.5f; m_tUiTransform.vDrawUvMax.y = 1.0f;
		break;
	case 8:
		m_tUiTransform.vDrawUvMin.x = 0.6f; m_tUiTransform.vDrawUvMax.x = 0.8f; m_tUiTransform.vDrawUvMin.y = 0.5f; m_tUiTransform.vDrawUvMax.y = 1.0f;
		break;
	case 9:
		m_tUiTransform.vDrawUvMin.x = 0.8f; m_tUiTransform.vDrawUvMax.x = 1.0f; m_tUiTransform.vDrawUvMin.y = 0.5f; m_tUiTransform.vDrawUvMax.y = 1.0f;
		break;
	default:
		break;
	}
	m_tUiTransform.vDrawUvMin.x += 0.001f;
	m_tUiTransform.vDrawUvMax.x -= 0.001f;
	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
}

void CUI_Damage::Set_ConstantTable_Critical(_int iMax)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// 스 자 이 공 부
	_matrix		matWorld, matView, matProj;
	D3DXMatrixScaling(&matWorld, m_fFontSizeRT * 3.f, m_fFontSizeRT * 1.5f, (_float)m_tUiTransform.iPosZ);

	matWorld *= Get_Transform()->Get_WorldMatrix();
	matWorld._41 += m_tUiAnimation.vAnimPosition.x - (iMax * 0.2f) * m_tUiTransform.iSizeX * m_fFontSizeRT;
	matWorld._42 += m_tUiAnimation.vAnimPosition.y;

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	D3DXCOLOR colorCritical = D3DXCOLOR(1.f, 0.f, 0.f, m_ColorShaderOption.a);
	pEffect->SetFloatArray("g_vImageColor", (float*)colorCritical, 4);

	pEffect->SetTexture("g_BaseTexture", m_pTexture_Critical);

	m_tUiTransform.vDrawUvMin = _vec2(0.f, 0.f);
	m_tUiTransform.vDrawUvMax = _vec2(1.f, 1.f);

	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
}

void CUI_Damage::Set_ConstantTable_Weak(_int iMax)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// 스 자 이 공 부
	_matrix		matWorld, matView, matProj;
	D3DXMatrixScaling(&matWorld, 156.f / 58.f, 58.f / 58.f, 1.f);

	matWorld *= Get_Transform()->Get_WorldMatrix();
	matWorld._41 += m_tUiAnimation.vAnimPosition.x - (iMax * 0.2f) * m_tUiTransform.iSizeX * m_fFontSizeRT;
	matWorld._42 += m_tUiAnimation.vAnimPosition.y + m_tUiTransform.iSizeY * m_fFontSizeRT * 1.3f;

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	pEffect->SetFloatArray("g_vImageColor", (float*)m_ColorShaderOption, 4);

	pEffect->SetTexture("g_BaseTexture", m_pTexture_Weak);

	m_tUiTransform.vDrawUvMin = _vec2(0.f, 0.f);
	m_tUiTransform.vDrawUvMax = _vec2(1.f, 1.f);

	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
}

void CUI_Damage::Set_ConstantTable_Resist(_int iMax)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// 스 자 이 공 부
	_matrix		matWorld, matView, matProj;
	D3DXMatrixScaling(&matWorld, 95.f / 31.f, 31.f / 31.f, 1.f);
	matWorld *= Get_Transform()->Get_WorldMatrix();
	matWorld._41 += m_tUiAnimation.vAnimPosition.x - (iMax * 0.2f) * m_tUiTransform.iSizeX * m_fFontSizeRT;
	matWorld._42 += m_tUiAnimation.vAnimPosition.y + m_tUiTransform.iSizeY * m_fFontSizeRT * 1.3f;

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	pEffect->SetFloatArray("g_vImageColor", (float*)m_ColorShaderOption, 4);

	pEffect->SetTexture("g_BaseTexture", m_pTexture_Resist);

	m_tUiTransform.vDrawUvMin = _vec2(0.f, 0.f);
	m_tUiTransform.vDrawUvMax = _vec2(1.f, 1.f);

	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
}

void CUI_Damage::Set_ConstantTable_ImmuneBlockMiss(_int iMax)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	// 스 자 이 공 부
	_matrix		matWorld, matView, matProj;
	if (m_bImmune)
	{
		D3DXMatrixScaling(&matWorld, 174.f / 44.f, 44.f / 44.f, 1.f);
	}
	else
	{
		D3DXMatrixScaling(&matWorld, 154.f / 44.f, 44.f / 44.f, 1.f);
	}
	matWorld *= Get_Transform()->Get_WorldMatrix();
	matWorld._41 += m_tUiAnimation.vAnimPosition.x - (iMax * 0.2f) * m_tUiTransform.iSizeX * m_fFontSizeRT;
	matWorld._42 += m_tUiAnimation.vAnimPosition.y + m_tUiTransform.iSizeY * m_fFontSizeRT * 1.3f;

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix orthoMat;
	D3DXMatrixIdentity(&orthoMat);
	pEffect->SetMatrix("g_matView", &orthoMat);
	D3DXMatrixOrthoLH(&orthoMat, (float)WINCX, (float)WINCY, 0, 10);
	pEffect->SetMatrix("g_matProj", &orthoMat);

	pEffect->SetFloatArray("g_vImageColor", (float*)m_ColorShaderOption, 4);

	if (m_bBlock)
	{
		pEffect->SetTexture("g_BaseTexture", m_pTexture_Block);
	}
	else if (m_bImmune)
	{
		pEffect->SetTexture("g_BaseTexture", m_pTexture_Immune);
	}
	else if (m_bMiss)
	{
		pEffect->SetTexture("g_BaseTexture", m_pTexture_Miss);
	}

	m_tUiTransform.vDrawUvMin = _vec2(0.f, 0.f);
	m_tUiTransform.vDrawUvMax = _vec2(1.f, 1.f);

	pEffect->SetFloatArray("g_vDrawUVMin", (float*)m_tUiTransform.vDrawUvMin, 2);
	pEffect->SetFloatArray("g_vDrawUVMax", (float*)m_tUiTransform.vDrawUvMax, 2);
}
