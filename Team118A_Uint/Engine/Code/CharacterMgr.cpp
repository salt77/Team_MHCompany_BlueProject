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
	assert(FALSE);

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

vector<T_CharInfo> CCharacterMgr::Get_vecCharInfo_Random()
{
	for (_uint i = 0; i < m_vecCharInfo.size(); ++i)
	{
		//i == 3 || i == 6 || i == 9
		if (i % 2 == 0)
		{
			m_vecCharInfo[i].bGetChar = TRUE;
		}
	}

	return m_vecCharInfo;
}

// (민우) : 캐릭터를 보유정보를 갱신 
void CCharacterMgr::Set_Collect(E_CHAR_NAME eName)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin();

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			(*iter).bGetChar = TRUE;
		}
	}
}

// 캐릭터 정보 갱신 
void CCharacterMgr::Set_CharInfo(E_CHAR_NAME eName, E_CHAR_STRUCTALL eEnumType, void * pValue)
{
	vector<T_CharInfo>::iterator	iter = m_vecCharInfo.begin(); 

	for (; iter != m_vecCharInfo.end(); ++iter)
	{
		if (eName == iter->eName[(_uint)E_CHAR_LANG::LANG_ENG])
		{
			switch (eEnumType)
			{
			case Engine::E_CHAR_STRUCTALL::CHARINFO_bGetChar:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_bHide:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eName:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eStar:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eCombatType:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eRole:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_ePosition:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eAtkType:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eDffType:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eWeaponType:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eInCityBuff:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eOutDoorBuff:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eInDoorBuff:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eSchool:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_eGrade:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrCircle:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrGeneralInfo:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iAge:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrBirthDay:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHeight:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_wstrHobby:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fExSkill:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fNormalSkill:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fReinforceSkill:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fSubSkill:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iExSkillCost:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fExSkill_Increase:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fNormalSkill_Increase:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fReinforceSkill_Increase:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_fSubSkill_Increase:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iNowHP:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iMaxHP:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iAttack:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iDeffence:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHeal:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iHitRatio:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iEvasion:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCritical:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCriticalDamage:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iStability:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iRange:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCrowdReinforce:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCrowdResist:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iCostHeal:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_HP:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_ATK:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_DEF:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iIncLevel_HEAL:
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iLevel:
				for (_uint i = iter->iLevel; i < *(_uint*)pValue; ++i)
				{
					Character_LevelUp(iter->eName[(_uint)E_CHAR_LANG::LANG_ENG]);
				}

				//iter->iLevel = *(_uint*)pValue;
				iter->iMaxExperience = iter->iLevel * CharExpInc;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iExperience:
				iter->iExperience = *(_uint*)pValue;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_iMaxExperience:
				iter->iMaxExperience = *(_uint*)pValue;
				break;

			case Engine::E_CHAR_STRUCTALL::CHARINFO_END:
				assert(0);
				break;

			default:
				assert(0);
				break;
			}

			break; 
		}
	}
}

// Enum 2 String(매개변수 Enum값, Enum타입(이름, 전투타입 등))
wstring CCharacterMgr::Translation_EnumToString(const _uint & iEnumValue, const E_CHAR_ENUMTYPE & eEnumType)
{
	switch (eEnumType)
	{
	case Engine::E_CHAR_ENUMTYPE::CNUM_NAME:
		switch ((E_CHAR_NAME)iEnumValue)
		{
		case Engine::E_CHAR_NAME::Name_Neru:
			return L"Neru";
		case Engine::E_CHAR_NAME::Name_Nonomi:
			return L"Nonomi";
		case Engine::E_CHAR_NAME::Name_Mashiro:
			return L"Mashiro";
		case Engine::E_CHAR_NAME::Name_Maki:
			return L"Maki";
		case Engine::E_CHAR_NAME::Name_Mutsuki:
			return L"Mutsuki";
		case Engine::E_CHAR_NAME::Name_Saya:
			return L"Saya";
		case Engine::E_CHAR_NAME::Name_Serina:
			return L"Serina";
		case Engine::E_CHAR_NAME::Name_Serika:
			return L"Serika";
		case Engine::E_CHAR_NAME::Name_Shun:
			return L"Shun";
		case Engine::E_CHAR_NAME::Name_Sumire:
			return L"Sumire";
		case Engine::E_CHAR_NAME::Name_Suzumi:
			return L"Suzumi";
		case Engine::E_CHAR_NAME::Name_Shiroko:
			return L"Shiroko";
		case Engine::E_CHAR_NAME::Name_Shimiko:
			return L"Shimiko";
		case Engine::E_CHAR_NAME::Name_Shizuko:
			return L"Shizuko";
		case Engine::E_CHAR_NAME::Name_Aru:
			return L"Aru";
		case Engine::E_CHAR_NAME::Name_Asuna:
			return L"Asuna";
		case Engine::E_CHAR_NAME::Name_Ayane:
			return L"Ayane";
		case Engine::E_CHAR_NAME::Name_Airi:
			return L"Airi";
		case Engine::E_CHAR_NAME::Name_Izuna:
			return L"Izuna";
		case Engine::E_CHAR_NAME::Name_Akane:
			return L"Akane";
		case Engine::E_CHAR_NAME::Name_Akari:
			return L"Akari";
		case Engine::E_CHAR_NAME::Name_Eimi:
			return L"Eimi";
		case Engine::E_CHAR_NAME::Name_Yoshimi:
			return L"Yoshimi";
		case Engine::E_CHAR_NAME::Name_Utaha:
			return L"Utaha";
		case Engine::E_CHAR_NAME::Name_Yuuka:
			return L"Yuuka";
		case Engine::E_CHAR_NAME::Name_Iori:
			return L"Iori";
		case Engine::E_CHAR_NAME::Name_Izumi:
			return L"Izumi";
		case Engine::E_CHAR_NAME::Name_Juri:
			return L"Juri";
		case Engine::E_CHAR_NAME::Name_Zunko:
			return L"Zunko";
		case Engine::E_CHAR_NAME::Name_Tsurugi:
			return L"Tsurugi";
		case Engine::E_CHAR_NAME::Name_Tsubaki:
			return L"Tsubaki";
		case Engine::E_CHAR_NAME::Name_Chinatsu:
			return L"Chinatsu";
		case Engine::E_CHAR_NAME::Name_Chise:
			return L"Chise";
		case Engine::E_CHAR_NAME::Name_Karin:
			return L"Karin";
		case Engine::E_CHAR_NAME::Name_Kayoko:
			return L"Kayoko";
		case Engine::E_CHAR_NAME::Name_Kotama:
			return L"Kotama";
		case Engine::E_CHAR_NAME::Name_Kotori:
			return L"Kotori";
		case Engine::E_CHAR_NAME::Name_Pina:
			return L"Pina";
		case Engine::E_CHAR_NAME::Name_Hanae:
			return L"Hanae";
		case Engine::E_CHAR_NAME::Name_Hare:
			return L"Hare";
		case Engine::E_CHAR_NAME::Name_Haruna:
			return L"Haruna";
		case Engine::E_CHAR_NAME::Name_Haruka:
			return L"Haruka";
		case Engine::E_CHAR_NAME::Name_Hasumi:
			return L"Hasumi";
		case Engine::E_CHAR_NAME::Name_Hoshino:
			return L"Hoshino";
		case Engine::E_CHAR_NAME::Name_Fuuka:
			return L"Fuuka";
		case Engine::E_CHAR_NAME::Name_Hina:
			return L"Hina";
		case Engine::E_CHAR_NAME::Name_Hibiki:
			return L"Hibiki";
		case Engine::E_CHAR_NAME::Name_Hihumi:
			return L"Hihumi";
		case Engine::E_CHAR_NAME::Name_Midori:
			return	L"Midori";
		case Engine::E_CHAR_NAME::Name_Aris:
			return L"Aris";
		case Engine::E_CHAR_NAME::Name_Yuzu:
			return L"Yuzu";
		case Engine::E_CHAR_NAME::Name_Momoi:
			return L"Momoi";
		case Engine::E_CHAR_NAME::Name_Azusa:
			return L"Azusa";
		case Engine::E_CHAR_NAME::Name_Wakamo:
			return L"Wakamo";
		case Engine::E_CHAR_NAME::Name_Nodoka:
			return L"Nodoka";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_STAR:
		switch ((E_CHAR_STAR)iEnumValue)
		{
		case E_CHAR_STAR::STAR_1:
			return L"1";

		case E_CHAR_STAR::STAR_2:
			return L"2";

		case E_CHAR_STAR::STAR_3:
			return L"3";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_COMBATTYPE:
		switch ((E_CHAR_COMBATTYPE)iEnumValue)
		{
		case  E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER:
			return L"Striker";

		case  E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL:
			return L"Special";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_ROLE:
		switch ((E_CHAR_ROLE)iEnumValue)
		{
		case E_CHAR_ROLE::ROLE_TANKER:
			return L"Tanker";

		case E_CHAR_ROLE::ROLE_DEALER:
			return L"Dealer";

		case E_CHAR_ROLE::ROLE_HEALER:
			return L"Healer";

		case E_CHAR_ROLE::ROLE_SUPPORTER:
			return L"Supporter";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_POSITION:
		switch ((E_CHAR_POSITION)iEnumValue)
		{
		case E_CHAR_POSITION::POSITION_FRONT:
			return L"Front";

		case E_CHAR_POSITION::POSITION_MIDDLE:
			return L"Middle";

		case E_CHAR_POSITION::POSITION_BACK:
			return L"Back";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_ATKTYPE:
		switch ((E_CHAR_ATKTYPE)iEnumValue)
		{
		case E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION:
			return L"Explosion";

		case E_CHAR_ATKTYPE::ATKTYPE_PIERCE:
			return L"Pierce";

		case E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:
			return L"Mystery";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_DFFTYPE:
		switch ((E_CHAR_DFFTYPE)iEnumValue)
		{
		case E_CHAR_DFFTYPE::DFFTYPE_HEAVY:
			return L"Heavy";

		case E_CHAR_DFFTYPE::DFFTYPE_LIGHT:
			return L"Light";

		case E_CHAR_DFFTYPE::DFFTYPE_SPECICAL:
			return L"Special";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_WEAPONTYPE:
		switch ((E_CHAR_WEAPONTYPE)iEnumValue)
		{
		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:
			return L"SG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:
			return L"SMG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:
			return L"GL";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:
			return L"SR";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:
			return L"MG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:
			return L"AR";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_RL:
			return L"RL";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_RG:
			return L"RG";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_MAPBUFF:
		switch ((E_CHAR_MAPBUFF)iEnumValue)
		{
		case E_CHAR_MAPBUFF::MAPBUFF_SS:
			return L"SS";

		case E_CHAR_MAPBUFF::MAPBUFF_S:
			return L"S";

		case E_CHAR_MAPBUFF::MAPBUFF_A:
			return L"A";

		case E_CHAR_MAPBUFF::MAPBUFF_B:
			return L"B";

		case E_CHAR_MAPBUFF::MAPBUFF_C:
			return L"C";

		case E_CHAR_MAPBUFF::MAPBUFF_D:
			return L"D";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_SCHOOL:
		switch ((E_CHAR_SCHOOL)iEnumValue)
		{
		case E_CHAR_SCHOOL::SCHOOL_ABYDOS:
			return L"아비도스 고등학교";

		case E_CHAR_SCHOOL::SCHOOL_TRINITY:
			return L"트리니티 종합학원";

		case E_CHAR_SCHOOL::SCHOOL_GEHENNA:
			return L"게헨나 학원";

		case E_CHAR_SCHOOL::SCHOOL_MILLENIUM:
			return L"밀레니엄 사이언스 스쿨";

		case E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO:
			return L"백귀야행 연합학원";

		case E_CHAR_SCHOOL::SCHOOL_SHANHAIJING:
			return L"산해경";

		case E_CHAR_SCHOOL::SCHOOL_REDWINTER:
			return L"붉은겨울 연방학원";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_GRADE:
		switch ((E_CHAR_GRADE)iEnumValue)
		{
		case E_CHAR_GRADE::GRADE_FIRST:
			return L"1학년";

		case E_CHAR_GRADE::GRADE_SECOND:
			return L"2학년";

		case E_CHAR_GRADE::GRADE_THIRD:
			return L"3학년";
		}
		break;

	default:
		assert(0);
		break;
	}

	return L"NULL";
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
			//iter->eSkillLvArr;
			//iter->fExSkill;
			//iter->fNormalSkill;
			//iter->fReinforceSkill;
			//iter->fSubSkill;
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
