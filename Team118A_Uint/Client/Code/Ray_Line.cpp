#include "stdafx.h"
#include "Ray_Line.h"


CRay_Line::CRay_Line(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	: CGameObject(pGraphicDev, eObjTag)
{
}

CRay_Line::CRay_Line(const CRay_Line & rhs)
	: CGameObject(rhs)
{
}

CRay_Line::~CRay_Line()
{
}

CRay_Line * CRay_Line::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CRay_Line*	pInstance = new CRay_Line(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CRay_Line::Free()
{
	Safe_Release(m_pLine);

	CGameObject::Free();
}


HRESULT CRay_Line::Ready_Object()
{
	CGameObject::Ready_Object();

	Add_Component();

	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_pLine->SetWidth(3.f);
	m_pLine->SetAntialias(TRUE);

	return S_OK;
}

_int CRay_Line::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Start_Object(fTimeDelta);

	return iExit;
}

_int CRay_Line::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);

	return iExit;
}

_int CRay_Line::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	Management Add_RenderGroup(E_RENDER_ID::RENDER_ALPHA, this);

	return iExit;
}

// // 라인 생성, 삭제 
// void CRay_Line::Render_Object()
// {
// 	Set_Constantable();
// 
// 	m_pShaderCom->BeginShader();
// 	m_pShaderCom->BeginPass(m_iPassNumber);
// 
// 	_matrix	 matView, matProj;
// 
// 	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
// 	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
// 
// 	for (size_t i = 0; i < 2; ++i)
// 	{
// 		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
// 
// 		if (m_vPoint[i].z <= 0.1f)
// 			m_vPoint[i].z = 0.1f;
// 
// 		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
// 	}
// 
// 	_matrix matWorld;
// 	m_pLine->Begin();
// 
// 	m_pLine->DrawTransform(m_vPoint, 2, /*&(matView * matProj)*/D3DXMatrixIdentity(&matWorld), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
// 
// 	m_pLine->End();
// 
// 	m_pShaderCom->EndPass();
// 	m_pShaderCom->EndShader();
// }

void CRay_Line::Add_Component()
{
	CComponent*		pComponent = nullptr;

	// Shader
	m_pShaderCom = static_cast<CShader*>(Emplace_Component(ComTag_Shader, Management Clone_Prototype(L"Proto_Shader_Base")));
}

void CRay_Line::Set_Constantable()
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
void CRay_Line::Create_Line(_vec3 * vPoint)
{
	m_vPoint[0] = vPoint[0]; 
	m_vPoint[1] = vPoint[1]; 
}
