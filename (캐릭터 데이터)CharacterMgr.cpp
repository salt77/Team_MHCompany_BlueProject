#include "CharacterMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CCharacterMgr)

E_CHAR_SORT	CCharacterMgr::m_eSortType = E_CHAR_SORT::SORT_END;
CCharacterMgr::CCharacterMgr()
{
	m_vecCharInfo.reserve(50);
}

CCharacterMgr::~CCharacterMgr()
{
	Free();
}

void CCharacterMgr::Free()
{
	m_vecCharInfo.clear();
}

HRESULT CCharacterMgr::Ready_Manager()
{
	return S_OK;
}

// 특정 캐릭터 데이터 가져오기 (매개변수 이름)
T_CharInfo& CCharacterMgr::Get_CharInfo(E_CHAR_NAME eName)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin();

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			return *iter;
		}
	}


	LMSG_BOX(L"CharacterMgr Get_CharInfo() 없는 이름");
	assert(0);

	return *m_vecCharInfo.begin();
}

// 특정 캐릭터의 특정 정보만 가져오기(void* 반환 주의) 
const void * CCharacterMgr::Get_CharInfo_Detail(E_CHAR_NAME eName, E_CHAR_STRUCTALL eEnumType)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin();

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			switch (eEnumType)
			{
			case Engine::E_CHAR_STRUCTALL::CHARINFO_bGetChar:
				return &iter->bGetChar;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_bHide:
				return &iter->bHide;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eName:
				return &iter->eName;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eStar:
				return &iter->eStar;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eCombatType:
				return &iter->eCombatType;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eRole:
				return &iter->eRole;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_ePosition:
				return &iter->ePosition;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eAtkType:
				return &iter->eAtkType;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eDffType:
				return &iter->eDffType;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eWeaponType:
				return &iter->eWeaponType;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eInCityBuff:
				return &iter->eInCityBuff;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eOutDoorBuff:
				return &iter->eOutDoorBuff;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eInDoorBuff:
				return &iter->eInDoorBuff;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eSchool:
				return &iter->eSchoolType;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eGrade:
				return &iter->eGrade;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrCircle:
				return &iter->wstrCircle;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrGeneralInfo:
				return &iter->wstrGeneralInfo;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iAge:
				return &iter->iAge;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrBirthDay:
				return &iter->wstrBirthDay;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHeight:
				return &iter->iHeight;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrHobby:
				return &iter->wstrHobby;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fExSkill:
				return &iter->fExSkill;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fNormalSkill:
				return &iter->fNormalSkill;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fReinforceSkill:
				return &iter->fReinforceSkill;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fSubSkill:
				return &iter->fSubSkill;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iExSkillCost:
				return &iter->iExSkillCost;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fExSkill_Increase:
				return &iter->fExSkill_Increase;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fNormalSkill_Increase:
				return &iter->fNormalSkill_Increase;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fReinforceSkill_Increase:
				return &iter->fReinforceSkill_Increase;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fSubSkill_Increase:
				return &iter->fSubSkill_Increase;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iNowHP:
				return &iter->tUnitInfo.iNowHp;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iMaxHP:
				return &iter->tUnitInfo.iMaxHp;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iAttack:
				return &iter->tUnitInfo.iAttack;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iDeffence:
				return &iter->tUnitInfo.iDeffence;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHeal:
				return &iter->tUnitInfo.iHeal;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHitRatio:
				return &iter->tUnitInfo.iHitRatio;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iEvasion:
				return &iter->tUnitInfo.iEvasion;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCritical:
				return &iter->tUnitInfo.iCritical;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCriticalDamage:
				return &iter->tUnitInfo.iCriticalDamage;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iStability:
				return &iter->tUnitInfo.iStability;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iRange:
				return &iter->tUnitInfo.iRange;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCrowdReinforce:
				return &iter->tUnitInfo.iCrowdReinforce;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCrowdResist:
				return &iter->tUnitInfo.iCrowdResist;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCostHeal:
				return &iter->iCostHeal;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_HP:
				return &iter->iIncLevel_HP;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_ATK:
				return &iter->iIncLevel_ATK;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_DEF:
				return &iter->iIncLevel_DEF;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_HEAL:
				return &iter->iIncLevel_HEAL;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iLevel:
				return &iter->iLevel;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iExperience:
				return &iter->iExperience;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iMaxExperience:
				return &iter->iMaxExperience;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_END:
				assert(0);
				break;

			default:
				assert(0);
				break;
			}
		}
	}

	return nullptr;
}

// 특정 기준으로 정렬(매개변수 정렬 기준)
const vector<T_CharInfo>& CCharacterMgr::Get_vecCharSort(E_CHAR_SORT eSortType)
{
	m_eSortType = eSortType;

	if (E_CHAR_SORT::SORT_REVERSE == eSortType)
	{
		reverse(m_vecCharInfo.begin(), m_vecCharInfo.end());
	}
	else
	{
		sort(m_vecCharInfo.begin(), m_vecCharInfo.end(), Compare_SortType);
	}

	return m_vecCharInfo;
}

// 캐릭터 정보 갱신 
void CCharacterMgr::Set_CharInfo(E_CHAR_NAME eName, E_CHAR_STRUCTALL eEnumType, void * pValue)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin(); 

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			case Engine::E_CHAR_STRUCTALL::CHARINFO_iLevel:
				for (_uint i = iter->iLevel; i < *(_uint*)pValue; ++i)
				{
					Character_LevelUp(iter->eName[(_uint)E_CHAR_LANG::LANG_ENG]);
				}

				iter->iMaxExperience = iter->iLevel * CharExpInc;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iExperience:
				iter->iExperience = *(_uint*)pValue;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iMaxExperience:
				iter->iMaxExperience = *(_uint*)pValue;
				break;

			default:
				assert(0);
				break;
			}

			break; 
		}
	}
}

// 캐릭터가 레벨업 시 스펙 상승하는 함수(private 내부에서만 사용)
void CCharacterMgr::Character_LevelUp(E_CHAR_NAME eName)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin(); 

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			++(iter->iLevel);

			iter->tUnitInfo.iMaxHp += iter->iIncLevel_HP;
			iter->tUnitInfo.iAttack += iter->iIncLevel_ATK;
			iter->tUnitInfo.iDeffence += iter->iIncLevel_DEF;
			iter->tUnitInfo.iHeal += iter->iIncLevel_HEAL;

			break; 
		}
	}
}

// Get_vecCharSort()에서 사용하는 함수, 외부에서 사용 x
_bool CCharacterMgr::Compare_SortType(const T_CharInfo& tInfoBegin, const T_CharInfo& tInfoEnd)
{
	switch (m_eSortType)
	{
	case Engine::E_CHAR_SORT::SORT_LEVEL:
		break;

	case Engine::E_CHAR_SORT::SORT_STAR:
		return tInfoBegin.eStar > tInfoEnd.eStar;
		break;

	case Engine::E_CHAR_SORT::SORT_TIES:
		break;

	case Engine::E_CHAR_SORT::SORT_SCHOOL:
		return tInfoBegin.eSchoolType > tInfoEnd.eSchoolType;
		break;

	case Engine::E_CHAR_SORT::SORT_NAME:
		return tInfoBegin.eName[(_uint)E_CHAR_LANG::LANG_ENG] < tInfoEnd.eName[(_uint)E_CHAR_LANG::LANG_ENG];
		break;

	case Engine::E_CHAR_SORT::SORT_EXSKILL_LV:
		break;

	case Engine::E_CHAR_SORT::SORT_SKILL_LV:
		break;

	case Engine::E_CHAR_SORT::SORT_INCITY_LV:
		return tInfoBegin.eInCityBuff > tInfoEnd.eInCityBuff;
		break;

	case Engine::E_CHAR_SORT::SORT_OUTDOOR_LV:
		return tInfoBegin.eOutDoorBuff > tInfoEnd.eOutDoorBuff;
		break;

	case Engine::E_CHAR_SORT::SORT_INDOOR_LV:
		return tInfoBegin.eInDoorBuff > tInfoEnd.eInDoorBuff;
		break;

	case Engine::E_CHAR_SORT::SORT_ATKTYPE:
		return tInfoBegin.eAtkType > tInfoEnd.eAtkType;
		break;

	case Engine::E_CHAR_SORT::SORT_DFFTYPE:
		return tInfoBegin.eDffType > tInfoEnd.eDffType;
		break;

	case Engine::E_CHAR_SORT::SORT_ROLE:
		return tInfoBegin.eRole > tInfoEnd.eRole;
		break;

	case Engine::E_CHAR_SORT::SORT_WEAPONTYPE:
		return tInfoBegin.eWeaponType > tInfoEnd.eWeaponType;
		break;

	case Engine::E_CHAR_SORT::SORT_RANGE:
		return tInfoBegin.tUnitInfo.iRange > tInfoEnd.tUnitInfo.iRange;
		break;

	case Engine::E_CHAR_SORT::SORT_GETTIME:
		break;

	case Engine::E_CHAR_SORT::SORT_REVERSE:
		break;

	default:
		assert(0);
		break;
	}

	return FALSE;
}
