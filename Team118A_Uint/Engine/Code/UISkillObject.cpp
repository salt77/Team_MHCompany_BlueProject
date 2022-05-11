#include "UISkillObject.h"
#include "EffectObject.h"
#include "ParticleObject.h"
#include "Scene.h"

USING(Engine)

CUISkillObject::CUISkillObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CSkillObject(pGraphicDev, eObjTag)
{
}

CUISkillObject::CUISkillObject(const CUISkillObject & rhs)
	: CSkillObject(rhs)
{
}

CUISkillObject::~CUISkillObject()
{
}

CUISkillObject * CUISkillObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale)
{
	CUISkillObject* pInstance = new CUISkillObject(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object(vecEffect, vecParticle, eLayerTag, vUIPos, fZRot, vScale)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUISkillObject::Free()
{
	CSkillObject::Free();
	
}

HRESULT CUISkillObject::Ready_Object(vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale)
{
	CGameObject::Ready_Object();

	m_vecSkillObject.reserve(vecEffect.size() + vecParticle.size());

	m_vUIPos = vUIPos;
	m_fZRot = fZRot;
	_vec3 vPos = CUtility::Ui2World(vUIPos.x, vUIPos.y);

	Set_Pos(vPos);

	_matrix matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	
	Set_Look(_vec3(matInvView._41, matInvView._42, matInvView._43));
	Set_Rotation(E_ROTATION::ROT_Z, m_fZRot);
	Set_Scale(vScale);

	for (size_t i = 0; i < vecEffect.size(); i++)
	{
		CGameObject* pGameObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), vecEffect[i]);
		static_cast<CEffectObject*>(pGameObject)->Set_Effect_Pos(Get_Position());
		static_cast<CEffectObject*>(pGameObject)->Set_Effect_Rotation(Get_Angle());
		static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(Get_Scale());
		static_cast<CEffectObject*>(pGameObject)->Set_ShowWindow(TRUE);
		assert(pGameObject);

		CScene::Get_MainScene()->Add_GameObject(eLayerTag, E_OBJTAG::OBJ_Effect, pGameObject);
		pGameObject->AddRef();
		m_vecSkillObject.emplace_back(pGameObject);

		
	}

	for (size_t i = 0; i < vecParticle.size(); i++)
	{
		CGameObject* pGameObject = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, vecParticle[i]);
		assert(pGameObject);
		pGameObject->Set_Pos(Get_Position());
		pGameObject->Set_Rotation(Get_Angle());
		pGameObject->Set_Scale(Get_Scale());
		static_cast<CParticleObject*>(pGameObject)->Set_ShowWindow(TRUE);
		CScene::Get_MainScene()->Add_GameObject(eLayerTag, E_OBJTAG::OBJ_Particle, pGameObject);
		pGameObject->AddRef();
		m_vecSkillObject.emplace_back(pGameObject);
	}
	return S_OK;
}

_int CUISkillObject::Start_Object(const _float & fTimeDelta)
{
	return CSkillObject::Start_Object(fTimeDelta);
}

_int CUISkillObject::Update_Object(const _float & fTimeDelta)
{
	CSkillObject::Update_Object(fTimeDelta);
	return RETURN_OK;
}

_int CUISkillObject::LateUpdate_Object(const _float & fTimeDelta)
{
	Update_UISkill(fTimeDelta);
	return CSkillObject::LateUpdate_Object(fTimeDelta);
}

void CUISkillObject::Render_Object(_int iPass)
{

}

void CUISkillObject::Set_UIPos(_vec2 vUIPos)
{
	m_vUIPos = vUIPos;
	Set_Pos(CUtility::Ui2World(m_vUIPos.x, m_vUIPos.y));
}

void CUISkillObject::Update_UISkill(_float fTimeDelta)
{
	m_vUIPos += m_vDir * m_fSpeed * fTimeDelta;
	m_fZRot += m_fRotSpeed * fTimeDelta;
	_vec3 vPos = CUtility::Ui2World(m_vUIPos.x, m_vUIPos.y);

	Set_Pos(vPos);

	_matrix matInvView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInvView);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	Set_Look(_vec3(matInvView._41, matInvView._42, matInvView._43));
	Set_Rotation(E_ROTATION::ROT_Z, m_fZRot);
}


