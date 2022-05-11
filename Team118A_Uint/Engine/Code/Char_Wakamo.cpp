#include "Char_Wakamo.h"
#include "Bullet_Normal.h"
#include "ParticleObject.h"
USING(Engine)

CChar_Wakamo::CChar_Wakamo(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Wakamo::CChar_Wakamo(const CChar_Wakamo & rhs)
	: CCharacter(rhs)
{
}

CChar_Wakamo::~CChar_Wakamo()
{
}

CChar_Wakamo * CChar_Wakamo::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Wakamo*	pInstance = new CChar_Wakamo(pGraphicDev, eObjTag, L"Wakamo");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Wakamo::Free()
{
	CCharacter::Free();
}

void CChar_Wakamo::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Wakamo::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Wakamo::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Wakamo::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Wakamo::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Wakamo::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Wakamo::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Wakamo::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Wakamo::End_Battle_Sound(const _bool & bWin)
{
}

void CChar_Wakamo::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Wakamo::Begin_Battle_Sound()
{
}

void CChar_Wakamo::Trigger_Attack_1()
{
	Hit_Target();

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

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vWeaponPos, Get_Angle(), L"Shot_Effect_1.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE);

	_uint	iRand = CWall512::GetInstance()->GetValue(0, 2); 
	iRand ? CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_01.wav", m_fSoundVolume) :
		CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Common_SR_05.wav", m_fSoundVolume); 
}

void CChar_Wakamo::Trigger_ExSkill()
{
	CSkill_Manager::GetInstance()->SetSkill(L"Wakamo_Ex_Start.skill", E_LAYERTAG::Effect, Get_Position(), Get_Angle(), VEC_ONE);
	Invoke_Add(1.3f, bind(&CChar_Wakamo::ExSound, this));
}

// 1타
void CChar_Wakamo::Trigger_1()
{
	Create_ExBullet();
}

// 2타
void CChar_Wakamo::Trigger_2()
{
	Create_ExBullet();
}

// 3타
void CChar_Wakamo::Trigger_3()
{
	Create_ExBullet();
}

// 4타
void CChar_Wakamo::Trigger_4()
{
	Create_ExBullet();
}

// 회전
void CChar_Wakamo::Trigger_5()
{
}

// 5타
void CChar_Wakamo::Trigger_6()
{
	Create_ExBullet(TRUE);
}

// 2.5타
void CChar_Wakamo::Trigger_7()
{
	Create_ExBullet();
}

void CChar_Wakamo::Trigger_8()
{
	_float fAngle = Get_Angle().y;
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Wakamo_Ex.camdat", L"Wakamo2", Get_Position(), fAngle);
}

// ExCutin
void CChar_Wakamo::Trigger_9()
{
	_float fAngle = Get_Angle().y;
	CActionCam_Mgr::GetInstance()->Start_ActionCam(L"Wakamo_Ex.camdat", L"Wakamo", Get_Position(), fAngle);
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Wakamo_Ex_Cut_in.wav", 1.f);
}

void CChar_Wakamo::ExSound()
{
	CSound_Manager::GetInstance()->Add_Sound_NotOverlap(L"SFX_Wakamo_Ex.wav", 1.f);
}

void CChar_Wakamo::Create_ExBullet(_bool bIsLast)
{
	if (m_pTarget)
	{
		_vec3 vToTargetPos = m_pTarget->Get_Position() - Get_Position();
		D3DXVec3Normalize(&vToTargetPos, &vToTargetPos);
		CBullet_Normal* pBullet = CBullet_Normal::Create(m_pGraphicDev, Get_WeaponPos(0.f, 0.f, 0.f), vToTargetPos, m_tUnitInfo, m_tCharInfo.eAtkType, E_LAYERTAG::Monster);
		pBullet->Set_Speed(12.f);
		pBullet->Set_ParticleObject(CParticleObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, L"Wakamo_Ex_Bullet.particle"));

		if (bIsLast)
		{
			pBullet->Set_AfterSkillName(L"Wakamo_Hit_After_Last.skill");
		}
		else
		{
			pBullet->Set_AfterSkillName(L"Wakamo_Hit_After.skill");
		}
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::AttackObj_Player, E_OBJTAG::OBJ_AtkObj, pBullet);
	}
}
