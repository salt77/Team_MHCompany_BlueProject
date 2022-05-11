#include "Hod_Cannon.h"
#include "Hod.h"
#include "UI_Manager.h"
#include "UI_Damage.h"
#include "Boss_HpBar.h"
#include "CollisionManager.h"
#include "GameManager.h"

CHod_Cannon::CHod_Cannon(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Hod_Cannon")
{
}

CHod_Cannon::CHod_Cannon(const CHod_Cannon & rhs)
	: CMonster(rhs)
{
}

CHod_Cannon::~CHod_Cannon()
{
}

CHod_Cannon * CHod_Cannon::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CHod_Cannon* pInstance = new CHod_Cannon(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHod_Cannon::Free()
{
	CMonster::Free();
}

HRESULT CHod_Cannon::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "body_Rot_01_Tan";
	m_szWeaponBoneName = "Point008";
	
	return S_OK;
}

_int CHod_Cannon::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	return RETURN_OK;
}

_int CHod_Cannon::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	//Not Show HP
	m_pHealthBar->Set_Active(false);
	m_pCombatObject->Set_Active(false);

	CHod* pHod = static_cast<CHod*>(CScene::Get_MainScene()->Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));

	if (pHod->Get_UnitDead())
	{
		m_tUnitInfo.iMaxHp = 0;
		m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
		Set_Dead(true);

		return S_OK;
	}

	Animation_Change(fTimeDelta);

	return RETURN_OK;
}

_int CHod_Cannon::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CHod_Cannon::Render_Object(_int iPass)
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

void CHod_Cannon::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CHod_Cannon::Update_FSM(_float fDeltaTime, E_LAYERTAG eTargetLayer, E_OBJTAG eObjTag)
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

void CHod_Cannon::Idle(_float fDeltaTime)
{
	m_eMainAction = E_ACTION::Normal_Idle;

	m_eCurrentAction = m_eMainAction;
}

void CHod_Cannon::OnEnable()
{
	CMonster::OnEnable();
}

void CHod_Cannon::OnDisable()
{
	CMonster::OnDisable();
}

void CHod_Cannon::OnDestroy()
{
	CMonster::OnDestroy();
}