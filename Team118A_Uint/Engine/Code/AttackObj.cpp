#include "AttackObj.h"

#include "CollisionManager.h"
#include "Trail_Fix.h"
#include "EffectObject.h"

USING(Engine)

CAttackObj::CAttackObj(LPDIRECT3DDEVICE9 pGraphicDev, E_LAYERTAG eTargetLayerTag)
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_AtkObj)
	, m_eTargetLayerTag(eTargetLayerTag)
{
}

CAttackObj::CAttackObj(const CAttackObj & rhs)
	: CGameObject(rhs)
{
}

CAttackObj::~CAttackObj()
{
}

void CAttackObj::Free()
{
	if (m_pTrail)
	{
		m_pTrail->Set_ParentDead(TRUE);
	}
	if (m_pEffectObject)
	{
		Safe_Release(m_pEffectObject); 
	}
	if (m_pParticle)
	{
		m_pParticle->Set_ParticleDead();
		Safe_Release(m_pParticle);
	}
	CGameObject::Free(); 
}

void CAttackObj::Save(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Save(hFile, dwByte); 
}

void CAttackObj::Load(HANDLE hFile, DWORD & dwByte)
{
	CGameObject::Load(hFile, dwByte); 
}

HRESULT CAttackObj::Ready_Object()
{
	CGameObject::Ready_Object(); 

	return S_OK;
}

_int CAttackObj::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Start_Object(fTimeDelta); 

	return iExit;
}

_int CAttackObj::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pSphereColliderCom)
	{
		m_pSphereColliderCom->SetMatrix(Get_Transform()->Get_WorldMatrix());
	}

	if(Get_IsDelayed())
	{
		return iExit;
	}

	m_fNowLifeTime -= fTimeDelta;

	if (0.f > m_fNowLifeTime)
	{
		Set_Dead(TRUE);

		if (m_pEffectObject)
		{
			m_pEffectObject->Set_Dead(TRUE); 
		}
		if (m_pParticle)
		{
			m_pParticle->Set_Dead(TRUE); 
		}
	}

	return iExit;
}

_int CAttackObj::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CGameObject::LateUpdate_Object(fTimeDelta);
	
	if (m_pSphereColliderCom && (E_LAYERTAG::Monster == m_eTargetLayerTag || E_LAYERTAG::Character == m_eTargetLayerTag))
	{
		CCollisionManager::GetInstance()->Collision_CircleToCircle(m_pSphereColliderCom, m_eTargetLayerTag, m_bColUseY);
	}

	return iExit;
}

void CAttackObj::Render_Object(_int iPass)
{
	CGameObject::Render_Object(iPass);
}

void CAttackObj::Debug_This_In_Console()
{
}

void CAttackObj::Set_Constantable()
{
}

void CAttackObj::On_Collision(CGameObject * pDest)
{
	CGameObject::On_Collision(pDest);
}
