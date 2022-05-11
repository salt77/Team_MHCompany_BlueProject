#include "Warning.h"
#include "ParticleObject.h"
#include "Scene.h"
#include "GameManager.h"
CWarning::CWarning(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, E_OBJTAG::OBJ_Effect)
{
}

CWarning::CWarning(const CWarning & rhs)
	: CGameObject(rhs)
{
}

CWarning::~CWarning()
{
}

CWarning * CWarning::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWarning* pInstance = new CWarning(pGraphicDev);

	if (E_FAIL == pInstance->Ready_Object())
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWarning::Free()
{
	Safe_Release(m_pMaxRangeEffect);
	Safe_Release(m_pNowRangeEffect);

	CGameObject::Free();
}

HRESULT CWarning::Ready_Object()
{
	CGameObject::Ready_Object();
	
	m_pairSound.first = L"None";
	return S_OK;
}

_int CWarning::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::Update_Object(fTimeDelta);


	if(0.f < m_fDelay)
	{
		m_fDelay -= fTimeDelta;
		return iExit;
	}
	m_fNowTimer += fTimeDelta;


	m_pNowRangeEffect->Update_Object(fTimeDelta);
	m_pMaxRangeEffect->Update_Object(fTimeDelta);
	
	return iExit;
}

_int CWarning::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CGameObject::LateUpdate_Object(fTimeDelta);

	m_pNowRangeEffect->LateUpdate_Object(fTimeDelta);
	m_pMaxRangeEffect->LateUpdate_Object(fTimeDelta);

	_float fRatio = m_fNowTimer / m_fTotalTimer;
	m_pNowRangeEffect->Set_Effect_Scale(VEC_ONE * m_fRadius * fRatio);

	if (fRatio >= 1.f)
	{
		if (m_bDamage_OnOff)
		{
			Set_DamageToPlayer();
		}
		if (L"None" != m_wstrAfterSkillName)
		{
			_vec3 vPos = m_pNowRangeEffect->Get_Effect_Pos() + m_vAfterSkill_AddPos;
			CSkill_Manager::GetInstance()->SetSkill(m_wstrAfterSkillName, E_LAYERTAG::Effect2, vPos, VEC_ZERO, m_vAfterSkill_Scale);
		}
		if (L"None" != m_pairSound.first)
		{
			CSound_Manager::GetInstance()->Add_Sound_NotOverlap(m_pairSound.first, m_pairSound.second);
		}

		Set_Dead(TRUE);
	}

	return iExit;
}

void CWarning::Render_Object(_int iPass)
{
	if (0.f < m_fDelay)
		return;

	CGameObject::Render_Object(iPass);
}

void CWarning::Set_Warning(E_WarnType eOutType, E_WarnType eInnerType, _vec3 vPos, _float fTotalTime, _float fDelay, _float fRadius, D3DXCOLOR colorWarning)
{
	m_fTotalTimer		= fTotalTime;
	m_fRadius			= fRadius * 0.5f;
	m_fDelay			= fDelay;

	// 매개변수로 들어온 WarnType에 맞게 생성
	switch (eOutType)
	{
	case E_WarnType::Circle_Fill:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle.effect");
		break;
	case E_WarnType::Circle_Magic:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Magic.effect");
		break;
	case E_WarnType::Circle_InsideFill:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Inside.effect");
		break;
	case E_WarnType::Circle_OutDot:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_OutDot.effect");
		break;
	case E_WarnType::Circle_Wave:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Wave.effect");
		break;
	case E_WarnType::Circle_Lava:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Lava.effect");
		break;
	case E_WarnType::Circle_Lightning:
		m_pMaxRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Lightning.effect");
		break;
	default:
		break;
	}

	switch (eInnerType)
	{
	case E_WarnType::Circle_Fill:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle.effect");
		break;
	case E_WarnType::Circle_Magic:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Magic.effect");
		break;
	case E_WarnType::Circle_InsideFill:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Inside.effect");
		break;
	case E_WarnType::Circle_OutDot:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_OutDot.effect");
		break;
	case E_WarnType::Circle_Wave:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Circle_Wave.effect");
		break;
	case E_WarnType::Circle_Lava:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Lava.effect");
		break;
	case E_WarnType::Circle_Lightning:
		m_pNowRangeEffect = Engine::CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, Get_Position(), Get_Angle(), L"Warning_Lightning.effect");
		break;
	default:
		break;
	}

	// 생성 후 적용
	if (m_pMaxRangeEffect)
	{
		m_pMaxRangeEffect->Set_Effect_Pos(vPos);
		m_pMaxRangeEffect->Set_Effect_Scale(VEC_ONE * m_fRadius);
		m_pMaxRangeEffect->Get_Effect_Info().fLifeTime = fTotalTime;
		memcpy(&m_pMaxRangeEffect->Get_Effect_Info().vColor_Start, &colorWarning, sizeof(_vec4));
		memcpy(&m_pMaxRangeEffect->Get_Effect_Info().vColor_End, &colorWarning, sizeof(_vec4));
	}

	if (m_pNowRangeEffect)
	{
		m_pNowRangeEffect->Set_Effect_Pos(vPos + _vec3(0.f, 0.02f, 0.f));
		m_pNowRangeEffect->Get_Effect_Info().fLifeTime = fTotalTime;
		memcpy(&m_pNowRangeEffect->Get_Effect_Info().vColor_Start, &colorWarning, sizeof(_vec4));
		memcpy(&m_pNowRangeEffect->Get_Effect_Info().vColor_End, &D3DXCOLOR(1.f, 0.f, 0.f, 1.f), sizeof(_vec4));
	}
}

void CWarning::Copy(CWarning * pWarn)
{
	m_pNowRangeEffect		= CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, pWarn->Get_NowEffect()->Get_Effect_Info());
	m_pMaxRangeEffect		= CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, pWarn->Get_MaxEffect()->Get_Effect_Info());

	m_pNowRangeEffect->Set_Effect_Scale(pWarn->Get_NowEffect()->Get_Effect_Scale());
	m_pMaxRangeEffect->Set_Effect_Scale(pWarn->Get_MaxEffect()->Get_Effect_Scale());

	m_fDelay				= pWarn->m_fDelay;
	m_fTotalTimer			= pWarn->m_fTotalTimer;
	m_fNowTimer				= pWarn->m_fNowTimer;
	m_fRadius				= pWarn->m_fRadius;
	m_tUnitInfo				= pWarn->m_tUnitInfo;
	m_bDamage_OnOff			= pWarn->m_bDamage_OnOff;

	m_wstrAfterSkillName	= pWarn->m_wstrAfterSkillName;
	m_vAfterSkill_AddPos	= pWarn->m_vAfterSkill_AddPos;
	m_vAfterSkill_Scale		= pWarn->m_vAfterSkill_Scale;

	for (auto& iter : pWarn->m_vecCombatStatus)
	{
		m_vecCombatStatus.push_back(iter);
	}

	m_pairSound = pWarn->m_pairSound;
}

void CWarning::Set_Delay(const _float & fDelay)
{
	m_fDelay = fDelay;
}

void CWarning::Set_DamageToPlayer()
{
	vector<CCharacter*> vecPlayer = CGameManager::GetInstance()->Get_CurrentSquad_Alive_Strikers();
	
	for (auto& iter : vecPlayer)
	{
		if (m_fRadius >= CUtility::Distance_vec3(m_pNowRangeEffect->Get_Effect_Pos() + m_vAfterSkill_AddPos, iter->Get_Position()))
		{
			iter->Set_Damage(m_tUnitInfo, m_eAtkType);
			for (auto& iterCombat : m_vecCombatStatus)
			{
				iter->Add_CombatStatus(iterCombat);
			}
		}
	}
}

void CWarning::Set_Damage(_int iDamage)
{
	m_bDamage_OnOff = TRUE;
	m_tUnitInfo.iAttack = iDamage;
}

void CWarning::Set_AfterSkillName(wstring wstrSkillName, _vec3 vAddPos, _vec3 vScale)
{
	m_wstrAfterSkillName	= wstrSkillName;
	m_vAfterSkill_AddPos	= vAddPos;
	m_vAfterSkill_Scale		= vScale;
}

void CWarning::Set_SoundName(TCHAR* szFirst, _float fSecond)
{
	m_pairSound = make_pair(szFirst, fSecond);
}
