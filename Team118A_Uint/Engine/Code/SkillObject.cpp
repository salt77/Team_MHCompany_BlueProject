#include "SkillObject.h"
#include "EffectObject.h"
#include "ParticleObject.h"
#include "Scene.h"
USING(Engine)

CSkillObject::CSkillObject(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CGameObject(pGraphicDev, eObjTag)
{
}

CSkillObject::CSkillObject(const CSkillObject & rhs)
	:CGameObject(rhs)
{
}

CSkillObject::~CSkillObject()
{
}

CSkillObject * CSkillObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale)
{
	CSkillObject* pInstance = new CSkillObject(pGraphicDev, eObjTag);
	
	if (FAILED(pInstance->Ready_Object(vecEffect, vecParticle, eLayerTag, vPos, vRot, vScale)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkillObject::Free()
{
	for (auto& pObj : m_vecSkillObject)
	{
		if (pObj->Get_ObjTag() == E_OBJTAG::OBJ_Particle)
		{
			static_cast<CParticleObject*>(pObj)->Set_ParticleDead();
		}
		else
		{
			pObj->Set_Dead(TRUE);
		}
		Safe_Release(pObj);
	}
	m_vecSkillObject.clear();
	CGameObject::Free();
}

HRESULT CSkillObject::Ready_Object(vector<wstring>& vecEffect, vector<wstring>& vecParticle, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale)
{
	CGameObject::Ready_Object();

	m_vecSkillObject.reserve(vecEffect.size() + vecParticle.size());

	Set_Pos(vPos);
	Set_Rotation(vRot);
	Set_Scale(vScale);

	for (size_t i = 0; i < vecEffect.size(); i++)
	{
		CGameObject* pGameObject = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), vecEffect[i]);
		static_cast<CEffectObject*>(pGameObject)->Set_Pos(Get_Position());
		static_cast<CEffectObject*>(pGameObject)->Set_Rotation(Get_Angle());
		static_cast<CEffectObject*>(pGameObject)->Set_Scale(Get_Scale());
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
		CScene::Get_MainScene()->Add_GameObject(eLayerTag, E_OBJTAG::OBJ_Particle, pGameObject);
		pGameObject->AddRef();
		m_vecSkillObject.emplace_back(pGameObject);
	}
	return S_OK;
}

_int CSkillObject::Start_Object(const _float & fTimeDelta)
{
	return CGameObject::Start_Object(fTimeDelta);
}

_int CSkillObject::Update_Object(const _float & fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	

	return RETURN_OK;
}

_int CSkillObject::LateUpdate_Object(const _float & fTimeDelta)
{
	auto iter = m_vecSkillObject.begin();
	while (iter != m_vecSkillObject.end())
	{
		if ((*iter)->Get_Dead())
		{
			Safe_Release(*iter);
			iter = m_vecSkillObject.erase(iter);		
		}
		else
		{
			++iter;
		}
	}

	if (m_vecSkillObject.empty())
	{
		m_bDead = TRUE;
	}

	for (auto& pObj : m_vecSkillObject)
	{
		if (E_OBJTAG::OBJ_Effect == pObj->Get_ObjTag())
		{
			static_cast<CEffectObject*>(pObj)->Set_Effect_Pos(Get_Position());
			static_cast<CEffectObject*>(pObj)->Set_Effect_Rotation(Get_Angle());
			static_cast<CEffectObject*>(pObj)->Set_Effect_Scale(Get_Scale());
		}
		else
		{
			pObj->Set_Pos(Get_Position());
			pObj->Set_Rotation(Get_Angle());
			pObj->Set_Scale(Get_Scale());
		}
	}
	
	return CGameObject::LateUpdate_Object(fTimeDelta);
}

void CSkillObject::Render_Object(_int iPass)
{

}


