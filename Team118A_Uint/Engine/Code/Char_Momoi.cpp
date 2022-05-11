#include "Char_Momoi.h"
#include "CollisionManager.h"
#include "ParticleObject.h"
#include "Monster.h"
USING(Engine)

CChar_Momoi::CChar_Momoi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Momoi::CChar_Momoi(const CChar_Momoi & rhs)
	: CCharacter(rhs)
{
}

CChar_Momoi::~CChar_Momoi()
{
}

CChar_Momoi * CChar_Momoi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Momoi*	pInstance = new CChar_Momoi(pGraphicDev, eObjTag, L"Momoi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Momoi::Free()
{
	Safe_Release(m_pExsObject);
	CCharacter::Free();
}

void CChar_Momoi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Momoi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Momoi::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Momoi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Momoi::Update_Object(const _float & fTimeDelta)
{
	if (m_pExsObject)
	{
		if (m_pExsObject->Get_Dead())
		{
			Safe_Release(m_pExsObject);
			m_pExsObject = nullptr;
		}
		else
		{
			m_pExsObject->Set_Pos(Get_WeaponPos(0, 0, 0.f));
			m_pExsObject->Set_Rotation(Get_Angle());

		}
	}
	_int iExit = CCharacter::Update_Object(fTimeDelta);
	
	return iExit;
}

_int CChar_Momoi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Momoi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Momoi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Momoi::Set_GuideSkill()
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CSkill_Manager::GetInstance()->SetSkillGuideFan(Get_Position(), Get_Angle(), 10.f, 60.f, 9999.f, 10, FALSE, L"../../Reference/Resource/Texture/Test/GuideTexBlue.png");
	if (pGuide)
	{
		m_vecGuideSkill.emplace_back(pGuide);
	}
}

void CChar_Momoi::ColMonster(CGameObject * pThis, CGameObject * pMonster)
{
	CParticleObject* pHitParticle = CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Particle, L"MomoiHit.particle");
	if (pHitParticle)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pHitParticle->Get_ObjTag(), pHitParticle);
		pHitParticle->Set_Scale(_vec3(2.f, 2.f, 2.f));
		pHitParticle->Set_Pos(pMonster->Get_Position());
	}

	static_cast<CUnit*>(pMonster)->Set_Damage(static_cast<CUnit*>(pThis)->Get_UnitInfo(), static_cast<CCharacter*>(pThis)->Get_CharInfo().eAtkType);
}

void CChar_Momoi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Momoi::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}

void CChar_Momoi::Trigger_Attack_3()
{
	
	_vec3 vLook = Get_Info(E_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	CCollisionManager::GetInstance()->Collision_FanToCircle(this, Get_Position(), vLook, 60.f, 10.f,E_LAYERTAG::Monster, ColMonster);
}

void CChar_Momoi::Trigger_ExSkill()
{
	
	if (m_pExsObject)
	{
		m_pExsObject->Set_Dead(TRUE);
		Safe_Release(m_pExsObject);
	}
	m_pExsObject = CSkill_Manager::GetInstance()->SetSkill(L"Momoi_Exs_Shot_0120.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.f), Get_Angle(), VEC_ONE * 0.3f);
	if (m_pExsObject)
	{
		m_pExsObject->AddRef();
	}
}

void CChar_Momoi::Trigger_1()
{
	if (m_pExsObject)
	{
		m_pExsObject->Set_Dead(TRUE); 
		Safe_Release(m_pExsObject);
		m_pExsObject = nullptr;
	}

}

void CChar_Momoi::Trigger_9()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Momoi_Ex.camdat", L"MomoiEX", Get_Position(), Get_Angle().y);
}

void CChar_Momoi::Trigger_8()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Momoi_Ex.camdat", L"MomoiBack", Get_Position(), Get_Angle().y);
}

void CChar_Momoi::Trigger_7()
{
	CActionCam_Mgr::GetInstance()->Stop_ActionCam(); 
}

void CChar_Momoi::GuideSkillSetting(_vec3 vPickPos)
{
	_vec3 vDir = vPickPos - Get_Position();

	for (auto& pGuide : m_vecGuideSkill)
	{
		pGuide->Set_Pos(Get_Position());
		pGuide->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));
	}
}

