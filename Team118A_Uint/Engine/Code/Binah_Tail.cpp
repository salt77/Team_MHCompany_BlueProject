#include "Binah_Tail.h"
#include "Binah.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "Boss_HpBar.h"
#include "CollisionManager.h"
#include "GameManager.h"

CBinah_Tail::CBinah_Tail(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Binah_Tail")
{
}

CBinah_Tail::CBinah_Tail(const CBinah_Tail & rhs)
	: CMonster(rhs)
{
}

CBinah_Tail::~CBinah_Tail()
{
}

CBinah_Tail * CBinah_Tail::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CBinah_Tail* pInstance = new CBinah_Tail(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinah_Tail::Free()
{
	CMonster::Free();
}

HRESULT CBinah_Tail::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Pivot2";
	m_szWeaponBoneName = "Pivot2";
	
	return S_OK;
}

_int CBinah_Tail::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CBinah_Tail::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	//Not Show HP
	m_pHealthBar->Set_Active(false);
	m_pCombatObject->Set_Active(false);

	CBinah* pBinah = static_cast<CBinah*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (pBinah->Get_UnitDead())
	{
		m_tUnitInfo.iMaxHp = 0;
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
		Set_Dead(true);

		return S_OK;
	}

	Animation_Change(fTimeDelta);

	return RETURN_OK;
}

_int CBinah_Tail::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CBinah_Tail::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CBinah_Tail::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CBinah_Tail::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
{
	if (!m_pDMeshCom)
		return;

	if (Get_UnitDead())
	{
		FSM_FunctionSetting(&CUnit::Dead, nullptr);
	}

	if (nullptr == m_pTarget || m_pTarget->Get_UnitDead())
	{
		Find_Target_InRange(eTargetLayer, eObjTag);
	}

	if (nullptr == m_pTarget)
	{
		FSM_FunctionRelease();
	}

	if (FALSE == m_bStateLock)
	{
		if (nullptr != m_pTarget)
		{
			FSM_FunctionSetting(&CUnit::Idle, nullptr);
		}
	}
	else
	{
		if (m_fpLockFunction)
		{
			((*this).*m_fpLockFunction)(fDeltaTime);
		}
		else
		{
			if (Is_AnimFinished())
			{
				m_bStateLock = FALSE;
			}
		}
	}
}

void CBinah_Tail::Idle(_float fDeltaTime)
{
	m_eMainAction = E_ACTION::Normal_Idle;

	m_eCurrentAction = m_eMainAction;
}

void CBinah_Tail::OnEnable()
{
	CMonster::OnEnable();
}

void CBinah_Tail::OnDisable()
{
	CMonster::OnDisable();
}

void CBinah_Tail::OnDestroy()
{
	CMonster::OnDestroy();
}