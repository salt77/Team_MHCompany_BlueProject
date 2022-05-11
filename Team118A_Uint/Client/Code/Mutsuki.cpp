#include "stdafx.h"
#include "Mutsuki.h"

#include "Management.h"
#include "Camera_TPS.h"

#include "Sukeban_SMG.h"
#include "Ray_Line.h"
#include "Bullet.h"

USING(Engine)

CMutsuki::CMutsuki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CMutsuki::CMutsuki(const CMutsuki & rhs)
	: CCharacter(rhs)
{
}

CMutsuki::~CMutsuki()
{
}

CMutsuki * CMutsuki::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
{
	CMutsuki*	pInstance = new CMutsuki(pGraphicDev, eObjTag, wstrUnitName);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMutsuki::Free()
{
	CCharacter::Free();
}

void CMutsuki::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CMutsuki::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CMutsuki::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	m_eUnitMode = E_UnitMode::TPS;
	return S_OK;
}

_int CMutsuki::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_TPS);

	return iExit;
}

_int CMutsuki::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	if (Management Key_Down(KEY_B))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_TPS);
	}
	else if (Management Key_Down(KEY_N))
	{
		CCamera::Swap_MainCamera(E_MAINCAM::MAINCAM_DYNAMIC);
	}

	m_fCountingTime += fTimeDelta;

	Key_Input();
	Mouse_Move();

	Animation_Control(fTimeDelta);

	return iExit;
}

_int CMutsuki::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	Management Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);
	Management Add_RenderGroup(E_RENDER_ID::RENDER_SHADOW, this);

	return iExit;
}

void CMutsuki::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CMutsuki::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CMutsuki::Key_Input()
{
	_vec3	vLookDir = CCamera::m_pMainCam->Get_CamLookNormalize();
	_vec3	vRightDir = CCamera::m_pMainCam->Get_CamRight();

	if (m_pTpsCam)
		m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Normal);

	// 스킬 
	if (m_eCur_Char_Action <= E_CHAR_ACTION::C_ACTION_SKILL)
	{
		if (Management Key_Down(KEY_G))
		{
			Skill();
		}
	}

	// 재장전 
	if (m_eCur_Char_Action <= E_CHAR_ACTION::C_ACTION_RELOAD)
	{
		if (Management Key_Down(KEY_R))
		{
			Reload();
		}
	}

	// 일반 공격 
	if (m_eCur_Char_Action <= E_CHAR_ACTION::C_ACTION_ATTACK)
	{
		if (Management Key_Pressing(KEY_LBUTTON))
		{
			if (E_ACTION::Stand_Shoot_Start == m_eCurrentAction ||
				E_ACTION::Stand_Delay == m_eCurrentAction ||
				E_ACTION::Stand_Shoot == m_eCurrentAction)
			{
				Stand_Fire();
			}
			else
			{
				Fire();
			}
		}
		else if (Management Key_Up(KEY_RBUTTON))
		{
			if (E_ACTION::Stand_Shoot_Start == m_eCurrentAction ||
				E_ACTION::Stand_Delay == m_eCurrentAction ||
				E_ACTION::Stand_Shoot == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Stand_Shoot_End);
				m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_STAND;
			}
		}
		else if (Management Key_Pressing(KEY_RBUTTON))
		{
			if (E_CHAR_ACTION::C_ACTION_STAND == m_eCur_Char_Action ||
				E_ACTION::Stand_Shoot_Start == m_eCurrentAction ||
				E_ACTION::Stand_Delay == m_eCurrentAction ||
				E_ACTION::Stand_Shoot == m_eCurrentAction)
			{
				Stand_Aiming();
			}
			else
			{
				Aiming();
			}
		}
		else if (m_eCur_Char_Action == E_CHAR_ACTION::C_ACTION_ATTACK)
		{
			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
		}
	}

	// 엄폐 
	if (m_eCur_Char_Action <= E_CHAR_ACTION::C_ACTION_STAND)
	{
		if (Management Key_Down(KEY_Q))
		{
			Stand();
		}
	}

	// Idle, 이동 
	if (m_eCur_Char_Action <= E_CHAR_ACTION::C_ACTION_MOVE)
	{
		if (Management Key_Pressing(KEY_W))
		{
			m_vMoveDir = vLookDir;
		}
		else if (Management Key_Pressing(KEY_S))
		{
			m_vMoveDir = -vLookDir;
		}
		else if (Management Key_Pressing(KEY_A))
		{
			m_vMoveDir = -vRightDir;
		}
		else if (Management Key_Pressing(KEY_D))
		{
			m_vMoveDir = vRightDir;
		}
		else
		{
			m_vMoveDir = VEC_ZERO;
		}

		if (VEC_ZERO != m_vMoveDir)
		{
			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_MOVE;
		}
		else
		{
			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
		}
	}
}

void CMutsuki::Mouse_Move()
{
	m_pTpsCam = static_cast<CCamera_TPS*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Camera_TPS));
	assert(m_pTpsCam);

	_vec2 vMouseMove  = m_pTpsCam->Get_MouseMove();

	if (E_CHAR_ACTION::C_ACTION_STAND != m_eCur_Char_Action && 
		E_ACTION::Stand_Reload != m_eCurrentAction)
	{
		if (vMouseMove.x != 0.f)
		{
			if (m_pTpsCam->Get_MouseFix())
			{
				Add_Rotation(E_ROTATION::ROT_Y, vMouseMove.x / 10.f);
			}
		}
	}
}

void CMutsuki::Animation_Control(const _float& fTimeDelta)
{
	if (nullptr == m_pDMeshCom)
		return;

	m_vFrontDir = Get_Info(E_INFO::INFO_LOOK);
	m_vRightDir = Get_Info(E_INFO::INFO_RIGHT);

	Animation_Change(fTimeDelta);
	Animation_End(fTimeDelta);

	switch (m_eCur_Char_Action)
	{
	case Engine::E_CHAR_ACTION::C_ACTION_IDLE:
		Set_ActionByEnum(E_ACTION::Normal_Idle);
		m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_MOVE:
		Movement(fTimeDelta);
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_STAND:
		if (m_pTpsCam)
		{
			m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Stand);
		}
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_ATTACK:
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_SKILL:
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_RELOAD:
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_DEAD:
		break;

	case Engine::E_CHAR_ACTION::C_ACTION_END:
		break;

	default:
		assert(0);
		break;
	}
}

void CMutsuki::Animation_End(const _float & fTimeDelta)
{
	if (m_pDMeshCom->Is_AnimationsetFinish())
	{
		switch (m_eCur_Char_Action)
		{
		case Engine::E_CHAR_ACTION::C_ACTION_IDLE:
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_MOVE:
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_STAND:
			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
			if (E_ACTION::Move_End_Stand == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Stand_Idle);
			}
			else if (E_ACTION::Stand_Idle == m_eCurrentAction ||
					 E_ACTION::Stand_Shoot_End == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Stand_Idle);
			}

			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_STAND;
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_ATTACK:
			if (E_ACTION::Normal_Shoot_Start == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Normal_Delay);
			}
			else if (E_ACTION::Stand_Shoot_Start == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Stand_Delay);
			}

			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK;
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_RELOAD:
			Reload_Ammo();
			if (E_ACTION::Stand_Reload == m_eCurrentAction)
			{
				Set_ActionByEnum(E_ACTION::Stand_Idle);
				m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_STAND;
			}
			else
			{
				m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
			}
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_SKILL:
			m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
			break;

		case Engine::E_CHAR_ACTION::C_ACTION_DEAD:
			break;

		default:
			break;
		}
	}
}

void CMutsuki::Movement(const _float & fTimeDelta)
{
	if (VEC_ZERO != m_vMoveDir)
	{
		m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_MOVE;

		m_vMoveDir.y = Get_Info(E_INFO::INFO_POS).y;

		D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);
		Move_Pos(m_vMoveDir, m_fMoveSpeed, fTimeDelta);

		Set_ActionByEnum(E_ACTION::Move_Ing);
	}
	else
	{
		m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
	}
}

void CMutsuki::Shoot_Bullet()
{
	if (0.05f <= m_fCountingTime)
	{
		m_fCountingTime = 0.f;
		--m_iAmmo;

		_vec3	vMyPos = Get_Info(E_INFO::INFO_POS);
		_vec3	vFrontDir = m_vFrontDir;
		Rotate_DirVector(Management GetFloatValue(-5.f, 5.f), VEC_UP, &vFrontDir);
		Rotate_DirVector(Management GetFloatValue(-5.f, 5.f), VEC_RIGHT, &vFrontDir);

		CGameObject*	pGameObject = CBullet::Create(m_pGraphicDev, vMyPos, vFrontDir, E_OBJTAG::OBJ_Test);
		assert(pGameObject);
		Management Add_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Test, pGameObject);
	}
}

void CMutsuki::Fire()
{
	_bool bDelay = FALSE;

	if (Management Key_Pressing(KEY_RBUTTON))
	{
		m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Aiming);
	}

	if (0 < m_iAmmo)
	{
		if (0.05f <= m_fCountingTime)
		{
			m_fCountingTime = 0.f;
			--m_iAmmo;

			_vec3	vMyPos = Get_Info(E_INFO::INFO_POS);
			_vec3	vFrontDir = m_vFrontDir;
			Rotate_DirVector(Management GetFloatValue(-5.f, 5.f), VEC_UP, &vFrontDir);
			Rotate_DirVector(Management GetFloatValue(-5.f, 5.f), VEC_RIGHT, &vFrontDir);

			Render_Line(&vMyPos, &vFrontDir);

			BOOL	bHit = 0;

			CSukeban_SMG*		pEnemy = static_cast<CSukeban_SMG*>(Management Get_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster));
			CSphereCollider*	pCollider = static_cast<CSphereCollider*>(pEnemy->Get_Component(ComTag_Collider));
			D3DXIntersect(pCollider->Get_MeshCollider(), &vMyPos, &vFrontDir, &bHit, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

			//if (bHit)
			//{
			//	pEnemy->Get_Damage(10, FALSE, m_tCharInfo.eAtkType);
			//}
		}
	}
	else
	{
		bDelay = TRUE;
	}

	if (bDelay)
	{
		Set_ActionByEnum(E_ACTION::Normal_Delay);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Normal_Shoot);
	}

	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK;
}

void CMutsuki::Fire2()
{
	_bool bDelay = FALSE;

	if (Management Key_Pressing(KEY_RBUTTON))
	{
		m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Aiming);
	}

	if (0 < m_iAmmo)
	{
		Shoot_Bullet(); 
	}
	else
	{
		bDelay = TRUE;
	}

	if (bDelay)
	{
		Set_ActionByEnum(E_ACTION::Normal_Delay);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Normal_Shoot);
	}
	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK;
}

void CMutsuki::Stand_Fire()
{
	_bool bDelay = FALSE;

	if (Management Key_Pressing(KEY_RBUTTON))
	{
		m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Aiming);
	}

	if (0 < m_iAmmo)
	{
		Shoot_Bullet(); 
	}
	else
	{
		bDelay = TRUE;
	}

	if (bDelay)
	{
		Set_ActionByEnum(E_ACTION::Stand_Delay);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Stand_Shoot);
	}
	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK; 
}

void CMutsuki::Aiming()
{
	if (E_ACTION::Normal_Delay != m_eCurrentAction &&
		E_ACTION::Normal_Shoot != m_eCurrentAction)
	{
		Set_ActionByEnum(E_ACTION::Normal_Shoot_Start);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Normal_Delay);
	}

	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK;

	m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Aiming);
}

void CMutsuki::Stand_Aiming()
{
	if (E_ACTION::Stand_Delay != m_eCurrentAction &&
		E_ACTION::Stand_Shoot != m_eCurrentAction)
	{
		Set_ActionByEnum(E_ACTION::Stand_Shoot_Start);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Stand_Delay);
	}

	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_ATTACK; 

	m_pTpsCam->Set_TpsCamMode(CCamera_TPS::TPS_Aiming); 
}

void CMutsuki::Reload()
{
	if (m_eCur_Char_Action == E_CHAR_ACTION::C_ACTION_STAND)
	{
		Set_ActionByEnum(E_ACTION::Stand_Reload);
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Normal_Reload);
	}

	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_RELOAD;
}

void CMutsuki::Stand()
{
	if (m_eCur_Char_Action == E_CHAR_ACTION::C_ACTION_STAND)
	{
		m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_IDLE;
	}
	else
	{
		Set_ActionByEnum(E_ACTION::Move_End_Stand);
		m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_STAND;
	}
}

void CMutsuki::Skill()
{
	Set_ActionByEnum(E_ACTION::ExSkill);
	m_eCur_Char_Action = E_CHAR_ACTION::C_ACTION_SKILL;
}

void CMutsuki::Rotate_DirVector(const _float & fDegree, const _vec3& vAxis, _vec3 * vLook)
{
	D3DXVec3Normalize(vLook, vLook);

	_matrix		 matRotate;
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixRotationAxis(&matRotate, &vAxis, D3DXToRadian(fDegree));
	D3DXVec3TransformNormal(vLook, vLook, &matRotate);
	D3DXVec3Normalize(vLook, vLook);
}

void CMutsuki::Render_Line(const _vec3 * vStartPos, const _vec3 * vDir)
{
	_vec3	vPos[2];
	vPos[0] = *vStartPos;
	vPos[1] = *vStartPos + (*vDir * 10.f);

	static_cast<CRay_Line*>(Management Get_GameObject(E_LAYERTAG::GameLogic, E_OBJTAG::OBJ_Test2))->Create_Line(vPos);
}
