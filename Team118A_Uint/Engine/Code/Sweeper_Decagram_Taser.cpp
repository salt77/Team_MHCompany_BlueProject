#include "Sweeper_Decagram_Taser.h"
#include "UI_Damage.h"
//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CSweeper_Decagram_Taser::CSweeper_Decagram_Taser(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"Sweeper_Decagram_Taser")
{
	
}

CSweeper_Decagram_Taser::CSweeper_Decagram_Taser(const CSweeper_Decagram_Taser & rhs)
	:CMonster(rhs)
{
}

CSweeper_Decagram_Taser::~CSweeper_Decagram_Taser()
{
}

CSweeper_Decagram_Taser * CSweeper_Decagram_Taser::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSweeper_Decagram_Taser* pInstance = new CSweeper_Decagram_Taser(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSweeper_Decagram_Taser::Free()
{
	CMonster::Free();
}

HRESULT CSweeper_Decagram_Taser::Ready_Object()
{
	CMonster::Ready_Object();
	m_szBodyBoneName = "Point_Com";
	m_szWeaponBoneName = "bone_Lhand";
	Set_AmmoMax(30);

	m_tUnitInfo.iAttack = 100;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 3000;
	m_tUnitInfo.iNowHp = 3000;
	m_tUnitInfo.iRange = 150;
	m_tUnitInfo.iStability;
	return S_OK;
}

_int CSweeper_Decagram_Taser::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CSweeper_Decagram_Taser::Update_Object(const _float & fTimeDelta)
{
	if (E_UnitMode::None == m_eUnitMode)
	{
		assert(FALSE);
	}

	Update_FSM(fTimeDelta, E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);
	Animation_Change(fTimeDelta);

	_int iExit = CGameObject::Update_Object(fTimeDelta);

	if (m_pDMeshCom)
	{
		_bool bRender = TRUE;
		if (m_pCombatObject)
		{
			m_pCombatObject->Update_Object(fTimeDelta);
		}
		if (E_ACTION::Vital_Death != m_eCurrentAction)
		{
			if (m_bAnimPlay)
			{
				m_pDMeshCom->Play_Animation(fTimeDelta);
			}
		}
		else
		{
			if (FALSE == Is_AnimFinished())
			{
				m_pDMeshCom->Play_Animation(fTimeDelta);
			}
			else
			{
				m_fDeadShaderFilter += fTimeDelta * 1.f;
			}
		}

		if (1.f > m_fDeadShaderFilter)
		{
			CRenderer::GetInstance()->Add_InstanceGroup(E_INSTANCE_RENDER_ID::RENDER_INSTANCE_NONALPHA, this);
			//CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
			//CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);
		}
	}
	else
	{
		Find_DMesh();
	}

	Update_Trigger(fTimeDelta);
	Update_Collider(fTimeDelta);


	return iExit;

	return RETURN_OK;
}

_int CSweeper_Decagram_Taser::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CSweeper_Decagram_Taser::Render_Object(_int iPass)
{
	if (nullptr == m_pDMeshCom)
	{
		return;
	}

	Set_Constantable();

	m_pShaderCom->BeginShader();

	m_pShaderCom->BeginPass(ShaderPass(E_ShaderPass::DMesh, iPass));

	m_pDMeshCom->Render_Meshes(m_pShaderCom);

	if (0 == iPass)
	{
		m_pShaderCom->ChangePass(ShaderPass(E_ShaderPass::Debug, iPass));

		m_pSphereCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
		m_pWeaponCollider->Render_Collider(m_pShaderCom->Get_EffectHandle());
	}

	m_pShaderCom->EndPass();
	m_pShaderCom->EndShader();
}

void CSweeper_Decagram_Taser::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSweeper_Decagram_Taser::OnEnable()
{
	CMonster::OnEnable();
}

void CSweeper_Decagram_Taser::OnDisable()
{
	CMonster::OnDisable();
}

void CSweeper_Decagram_Taser::OnDestroy()
{
	CMonster::OnDestroy();
}

void CSweeper_Decagram_Taser::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook		= *D3DXVec3Normalize(&vDirLook,		&Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight		= *D3DXVec3Normalize(&vDirRight,	&Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp		= *D3DXVec3Normalize(&vDirUp,		&Get_Info(E_INFO::INFO_UP));
	_float fLookRatio	= 0.4f;
	_float fRightRatio	= 0.0f;
	_float fUpRatio		= 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook	* fLookRatio;
	vWeaponPos += vDirRight	* fRightRatio;
	vWeaponPos += vDirUp	* fUpRatio;

	CSkill_Manager::GetInstance()->SetSkill(L"Test_Minhyun_1231_Fire_00.skill", E_LAYERTAG::Effect, vWeaponPos, Get_Angle());
}