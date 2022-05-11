#include "Char_Zunko.h"
#include "CollisionManager.h"
USING(Engine)

CChar_Zunko::CChar_Zunko(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Zunko::CChar_Zunko(const CChar_Zunko & rhs)
	: CCharacter(rhs)
{
}

CChar_Zunko::~CChar_Zunko()
{
}

CChar_Zunko * CChar_Zunko::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Zunko*	pInstance = new CChar_Zunko(pGraphicDev, eObjTag, L"Zunko");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Zunko::Free()
{
	CCharacter::Free();
}

void CChar_Zunko::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Zunko::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Zunko::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Zunko::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Zunko::Update_Object(const _float & fTimeDelta)
{
	if (m_pShootObject)
	{
		if (m_pShootObject->Get_Dead())
		{
			Safe_Release(m_pShootObject);
			m_pShootObject = nullptr;
		}
		else
		{
			m_pShootObject->Set_Pos(Get_WeaponPos(0, 0, 0.f));

		}
	}
	
	

	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Zunko::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);
	//10
	//1.6ÃÊ
	if (m_pGroundObject)
	{
		m_fExDistance += fTimeDelta * 7.f;
		CCollisionManager::GetInstance()->Collision_StraightToCircle(this, Get_Position(), Get_Info(E_INFO::INFO_LOOK), 3.f, m_fExDistance, E_LAYERTAG::Monster, ColMonster);
	}

	return iExit;
}

void CChar_Zunko::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Zunko::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Zunko::ColEx(CGameObject * pMonster)
{
	for (auto& pObj : m_vecColObject)
	{
		if (pObj == pMonster)
		{
			return;
		}
	}

	m_vecColObject.emplace_back(pMonster);
	static_cast<CUnit*>(pMonster)->Set_Damage(m_tUnitInfo, m_tCharInfo.eAtkType);
}

void CChar_Zunko::Set_GuideSkill()
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CSkill_Manager::GetInstance()->SetSkillGuideStraight(Get_Position(), Get_Angle(), 10.f, 3.f, 9999.f, FALSE, L"../../Reference/Resource/Texture/Test/GuideTexBlue.png");
	if (pGuide)
	{
		m_vecGuideSkill.emplace_back(pGuide);
	}
}


void CChar_Zunko::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Zunko::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}

void CChar_Zunko::Trigger_ExSkill()
{
	if (m_pShootObject)
	{
		m_pShootObject->Set_Dead(TRUE);
		Safe_Release(m_pShootObject);
	
	}
	m_pShootObject = CSkill_Manager::GetInstance()->SetSkill(L"CharSkill_Zunko_Exs_Shot.skill", E_LAYERTAG::Effect, Get_WeaponPos(0.f, 0.f, 0.f), Get_Angle(), VEC_ONE*0.3f);
	if (m_pShootObject)
	{
		m_pShootObject->AddRef();
	}

	if (m_pGroundObject)
	{
		m_pGroundObject->Set_Dead(TRUE);
		Safe_Release(m_pGroundObject);
		
	}

	m_pGroundObject = CSkill_Manager::GetInstance()->SetSkill(L"CharSkill_Zunko_Exs_Ground.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE*1.f);
	if (m_pGroundObject)
	{
		m_pGroundObject->AddRef();
		m_fExDistance = 0.f;
		m_vecColObject.clear();

	}
}

void CChar_Zunko::Trigger_1()
{

	if (m_pShootObject)
	{
		m_pShootObject->Set_Dead(TRUE);
		Safe_Release(m_pShootObject);
		m_pShootObject = nullptr;
	}

	if (m_pGroundObject)
	{
		m_pGroundObject->Set_Dead(TRUE);
		Safe_Release(m_pGroundObject);
		m_pGroundObject = nullptr;
	}
}

void CChar_Zunko::Trigger_9()
{
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Zunko_Ex.camdat", L"EX1", Get_Position(), Get_Angle().y);
}

void CChar_Zunko::Trigger_8()
{
	CActionCam_Mgr::GetInstance()->Stop_ActionCam();
}

void CChar_Zunko::GuideSkillSetting(_vec3 vPickPos)
{
	_vec3 vDir = vPickPos - Get_Position();

	for (auto& pGuide : m_vecGuideSkill)
	{
		pGuide->Set_Pos(Get_Position());
		pGuide->Set_Rotation(CMyMath::GetLookRotation(VEC_ZERO, vDir));
	}
}

void CChar_Zunko::ColMonster(CGameObject * pThis, CGameObject * pMonster)
{
	static_cast<CChar_Zunko*>(pThis)->ColEx(pMonster);
}


