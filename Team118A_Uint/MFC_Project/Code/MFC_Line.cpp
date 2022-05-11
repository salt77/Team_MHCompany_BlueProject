#include "stdafx.h"
#include "MFC_Line.h"


CMFC_Line::CMFC_Line(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
	m_vecPoint.reserve(100);
}

CMFC_Line::CMFC_Line(const CMFC_Line & rhs)
	: CGameObject(rhs)
{
}

CMFC_Line::~CMFC_Line()
{
}

CMFC_Line * CMFC_Line::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CMFC_Line*	pInstance = new CMFC_Line(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMFC_Line::Free()
{
	Safe_Release(m_pLine);

	CGameObject::Free();
}


HRESULT CMFC_Line::Ready_Object()
{
	CGameObject::Ready_Object();

	Add_Component();

	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_pLine->SetWidth(3.f);
	m_pLine->SetAntialias(TRUE);

	return S_OK;
}

_int CMFC_Line::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);

	return iExit;
}

_int CMFC_Line::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

_int CMFC_Line::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Management Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);

	return iExit;
}

// 라인 생성, 삭제 
void CMFC_Line::Render_Object(_int iPass)
{
	Set_Constantable();

	m_pShaderCom->BeginShader();
	m_pShaderCom->BeginPass(m_iPassNumber);

	_matrix	 matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3*	vPoint = new _vec3[m_vecPoint.size()];

	for (size_t i = 0; i < m_vecPoint.size(); ++i)
	{
		vPoint[i] = m_vecPoint[i];
	}

	for (size_t i = 0; i < m_vecPoint.size(); ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	_matrix matWorld;
	m_pLine->Begin();

	m_pLine->DrawTransform(vPoint, (_ulong)m_vecPoint.size(), /*&(matView * matProj)*/D3DXMatrixIdentity(&matWorld), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pLine->End();

	Safe_Delete_Array(vPoint);

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CMFC_Line::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
}

void CMFC_Line::Set_Constantable()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	_matrix		matWorld, matView, matProj;

	matWorld = Get_Transform()->Get_WorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
}

// 위치값을 받아와서 vector에 저장 
void CMFC_Line::Emplace_Line(_vec3	vPoint)
{
	m_vecPoint.emplace_back(vPoint);
}
