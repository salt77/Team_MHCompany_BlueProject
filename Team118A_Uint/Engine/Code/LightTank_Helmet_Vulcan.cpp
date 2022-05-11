#include "LightTank_Helmet_Vulcan.h"

#include "UI_Damage.h"
#include "Bullet_Normal_Monster.h" 
#include "Bullet_Parabola_Monster.h" 

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CLightTank_Helmet_Vulcan::CLightTank_Helmet_Vulcan(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev, eObjTag, L"LightTank_Helmet_Vulcan")
{
}

CLightTank_Helmet_Vulcan::CLightTank_Helmet_Vulcan(const CLightTank_Helmet_Vulcan & rhs)
	: CMonster(rhs)
{
}

CLightTank_Helmet_Vulcan::~CLightTank_Helmet_Vulcan()
{
}

CLightTank_Helmet_Vulcan * CLightTank_Helmet_Vulcan::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CLightTank_Helmet_Vulcan* pInstance = new CLightTank_Helmet_Vulcan(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightTank_Helmet_Vulcan::Free()
{
	CMonster::Free();
}

HRESULT CLightTank_Helmet_Vulcan::Ready_Object()
{
	CMonster::Ready_Object();

	m_szBodyBoneName = "Bone_Body";
	m_szWeaponBoneName = "bone_gun";

	Set_AmmoMax(1);

	m_tUnitInfo.iAttack = 100;
	m_tUnitInfo.iCritical;
	m_tUnitInfo.iCriticalDamage;
	m_tUnitInfo.iCrowdReinforce;
	m_tUnitInfo.iCrowdResist;
	m_tUnitInfo.iDeffence;
	m_tUnitInfo.iEvasion;
	m_tUnitInfo.iHeal;
	m_tUnitInfo.iHitRatio;
	m_tUnitInfo.iMaxHp = 15000;
	m_tUnitInfo.iNowHp = m_tUnitInfo.iMaxHp;
	m_tUnitInfo.iRange = 800;
	m_tUnitInfo.iStability;

	return S_OK;
}

_int CLightTank_Helmet_Vulcan::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);

	Create_Collider(&m_pFlashBangCol, L"Com_Collider_FlashBang", 0.1f);

	return RETURN_OK;
}

_int CLightTank_Helmet_Vulcan::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	Collider_SetMatrix(m_pFlashBangCol, m_szBoneFlashBang);

	return RETURN_OK;
}

_int CLightTank_Helmet_Vulcan::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);

	return RETURN_OK;
}

void CLightTank_Helmet_Vulcan::Render_Object(_int iPass)
{
	CMonster::Render_Object(iPass);
}

void CLightTank_Helmet_Vulcan::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CLightTank_Helmet_Vulcan::OnEnable()
{
	CMonster::OnEnable();
}

void CLightTank_Helmet_Vulcan::OnDisable()
{
	CMonster::OnDisable();
}

void CLightTank_Helmet_Vulcan::OnDestroy()
{
	CMonster::OnDestroy();
}

void CLightTank_Helmet_Vulcan::Trigger_Attack_1()
{
	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Smoke_2.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, m_pWeaponCollider->GetPos(), Get_Angle(), L"Tank_Fire_Normal.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE * 2.f);

	pGameObject = CBullet_Normal_Monster::Create(m_pGraphicDev,
												 vWeaponPos,
												 *D3DXVec3Normalize(&m_vOutTemp, &(m_pTarget->Get_Position() - vWeaponPos)),
												 m_tUnitInfo,
												 m_tMonsterInfo.eAtkType,
												 CBullet_Normal_Monster::Move_TankBullet);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_HeavyTank_Cannon_NomalAttack_02.wav", m_fSoundVolume);
}

void CLightTank_Helmet_Vulcan::Trigger_1()
{
	if (VEC_ZERO == m_vTargetPos)
	{
		return;
	}

	CGameObject*	pGameObject = CBullet_Parabola_Monster::Create(m_pGraphicDev,
																   m_pFlashBangCol->GetPos(),
																   m_vTargetPos,
																   Get_Info(E_INFO::INFO_LOOK),
																   m_tUnitInfo,
																   m_tMonsterInfo.eAtkType);
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Monster, E_OBJTAG::OBJ_AtkObj, pGameObject);
}

void CLightTank_Helmet_Vulcan::Trigger_7()
{
	//_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	//iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Appear_01.wav", m_fSoundVolume) :
	//	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Appear_02.wav", m_fSoundVolume); 
}

void CLightTank_Helmet_Vulcan::Trigger_9()
{
	if (!m_pTarget)
	{
		return;
	}

	m_vTargetPos = m_pTarget->Get_Position();
	CSkill_Manager::GetInstance()->SetSkillGuideCircle(m_vTargetPos, 2.f, 1.3f, 30, TRUE);
}

void CLightTank_Helmet_Vulcan::Trigger_Dead()
{
	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2);
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Death_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_GuardTower_Death_02.wav", m_fSoundVolume);
}
