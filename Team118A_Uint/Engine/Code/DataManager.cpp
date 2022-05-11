#include "DataManager.h"
#include <time.h>
#include "Wall512.h"
USING(Engine)

IMPLEMENT_SINGLETON(CDataManager)

CDataManager::CDataManager()
{
	//가챠시스템 구현 전까지 디버깅 용
	ZeroMemory(&m_tUserInfo.bCharacter,sizeof(_bool)*(_int)E_CHAR_NAME::Name_End);

	//스트라이커
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Maki]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Neru]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Shun]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Aru]			= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Akane]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Serika]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Chise]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Eimi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Hasumi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Hina]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Iori]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Izumi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Mutsuki]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Pina]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Sumire]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Suzumi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Momoi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Zunko]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Aris]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Wakamo]		= TRUE;
	// m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Yuzu]		= TRUE;
	// m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Nodoka]	= TRUE;
	// m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Wakamo]	= TRUE;
	// m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Midori]	= TRUE;
	// m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Azusa]	= TRUE;

	//스페셜
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Utaha]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Yoshimi]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Chinatsu]	= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Hibiki]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Juri]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Kayoko]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Karin]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Mashiro]		= TRUE;
	m_tUserInfo.bCharacter[(_int)E_CHAR_NAME::Name_Airi]		= TRUE;
}


CDataManager::~CDataManager()
{
	Free();
}

void CDataManager::Free()
{
}

//T_DATEINFO 의 주소를 받아서 현재 날짜를 연월일시분초 단위로 값을 채워주는 함수
void CDataManager::Get_CurrentDate(T_DATEINFO * pDateInfoOut)
{
	time_t curTime = time(NULL);
	struct tm tLocal;
	localtime_s(&tLocal, &curTime);
	pDateInfoOut->iYear = tLocal.tm_year + 1900;
	pDateInfoOut->iMonth = tLocal.tm_mon + 1;
	pDateInfoOut->iDay = tLocal.tm_mday;
	pDateInfoOut->iHour = tLocal.tm_hour;
	pDateInfoOut->iMinute = tLocal.tm_min;
	pDateInfoOut->iSecond = tLocal.tm_sec;
}

void CDataManager::Get_ConvertDate(T_DATEINFO * pDateInfoOut, time_t tTime)
{
	struct tm tLocal;
	localtime_s(&tLocal, &tTime);
	pDateInfoOut->iYear = tLocal.tm_year + 1900;
	pDateInfoOut->iMonth = tLocal.tm_mon + 1;
	pDateInfoOut->iDay = tLocal.tm_mday;
	pDateInfoOut->iHour = tLocal.tm_hour;
	pDateInfoOut->iMinute = tLocal.tm_min;
	pDateInfoOut->iSecond = tLocal.tm_sec;
}

//두 T_DATINFO 구조체를 받아서 두 날짜의 차이를 초단위로 반환하는 함수
_int CDataManager::Get_DateDiffrence(T_DATEINFO tPastDate, T_DATEINFO tFutureDate)
{
	struct tm tPastLocal;
	struct tm tFutureLocal;

	tPastLocal.tm_year = tPastDate.iYear - 1900;
	tPastLocal.tm_mon = tPastDate.iMonth - 1;
	tPastLocal.tm_mday = tPastDate.iDay;
	tPastLocal.tm_hour = tPastDate.iHour;
	tPastLocal.tm_min = tPastDate.iMinute;
	tPastLocal.tm_sec = tPastDate.iSecond;
	tPastLocal.tm_isdst = 0;

	tFutureLocal.tm_year = tFutureDate.iYear - 1900;
	tFutureLocal.tm_mon = tFutureDate.iMonth - 1;
	tFutureLocal.tm_mday = tFutureDate.iDay;
	tFutureLocal.tm_hour = tFutureDate.iHour;
	tFutureLocal.tm_min = tFutureDate.iMinute;
	tFutureLocal.tm_sec = tFutureDate.iSecond;
	tFutureLocal.tm_isdst = 0;

	time_t Start=mktime(&tPastLocal);
	time_t End=mktime(&tFutureLocal);
	_int iSecDiff= (_int)difftime(End, Start);
	return iSecDiff;
}

const _int CDataManager::Get_RangeByWeaponType(E_CHAR_WEAPONTYPE eWeaponType)
{
	switch (eWeaponType)
	{
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_NULL:	CUtility::Print_Wstring_DMode(L"WeaponType_Null로 인해 return Range 500"); return 500;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:		return 300;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:		return 450;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_HG:		return 600;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:		return 850;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:		return 1100;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:		return 800;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:		return 700;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MT:		return 1300;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RL:		return 950;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RG:		return 1050;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_END:		CUtility::Print_Wstring_DMode(L"WeaponType_End로 인해 return Range 500"); return 500;
	}
	return 500;
}

const _int CDataManager::Get_MaxAmmoByWeaponType(E_CHAR_WEAPONTYPE eWeaponType)
{
	switch (eWeaponType)
	{
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_NULL:	CUtility::Print_Wstring_DMode(L"WeaponType_Null로 인해 return MaxAmmo 10"); return 10;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:		return 4;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:		return 30;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_HG:		return 8;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:		return 2;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:		return 3;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:		return 40;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:		return 24;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_MT:		return 1;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RL:		return 1;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_RG:		return 1;
	case Engine::E_CHAR_WEAPONTYPE::WEAPONTYPE_END:		CUtility::Print_Wstring_DMode(L"WeaponType_End로 인해 return MaxAmmo 10"); return 10;
	}
	return 10;
}


void CDataManager::Set_HaveChar(_int iChar, _bool bState)
{
	m_tUserInfo.bCharacter[iChar] = bState;
}

// 게임 시작하면서 모든 Item의 값을 넣어준다.
void CDataManager::Initialize_Item(vector<T_ItemInfo>* pVecItemInfo)
{
	(*pVecItemInfo).reserve((_int)E_ItemTag::ItemTag_End);
	(*pVecItemInfo).resize((_int)E_ItemTag::ItemTag_End);

	// 제영
	(*pVecItemInfo)[(_int)E_ItemTag::네루_엘레프] = T_ItemInfo(E_ItemTag::네루_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Neru.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"네루의 엘레프", L"네루의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::노노미_엘레프] = T_ItemInfo(E_ItemTag::노노미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Nonomi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"노노미의 엘레프", L"노노미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::마시로_엘레프] = T_ItemInfo(E_ItemTag::마시로_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Mashiro.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"마시로의 엘레프", L"마시로의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::마키_엘레프] = T_ItemInfo(E_ItemTag::마키_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Maki.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"마키의 엘레프", L"마키의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::무츠키_엘레프] = T_ItemInfo(E_ItemTag::무츠키_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Mutsuki.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"무츠키의 엘레프", L"무츠키의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::사야_엘레프] = T_ItemInfo(E_ItemTag::사야_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Saya.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"사야의 엘레프", L"사야의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::세리나_엘레프] = T_ItemInfo(E_ItemTag::세리나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Serina.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"세리나의 엘레프", L"세리나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::세리카_엘레프] = T_ItemInfo(E_ItemTag::세리카_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Serika.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"세리카의 엘레프", L"세리카의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::슌_엘레프] = T_ItemInfo(E_ItemTag::슌_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Shun.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"슌의 엘레프", L"슌의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::스미레_엘레프] = T_ItemInfo(E_ItemTag::스미레_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Sumire.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"스미레의 엘레프", L"스미레의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::스즈미_엘레프] = T_ItemInfo(E_ItemTag::스즈미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Suzumi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"스즈미의 엘레프", L"스즈미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::시로코_엘레프] = T_ItemInfo(E_ItemTag::시로코_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Shiroko.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"시로코의 엘레프", L"시로코의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::시미코_엘레프] = T_ItemInfo(E_ItemTag::시미코_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Shimiko.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"시미코의 엘레프", L"시미코의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::시즈코_엘레프] = T_ItemInfo(E_ItemTag::시즈코_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Shizuko.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"시즈코의 엘레프", L"시즈코의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아루_엘레프] = T_ItemInfo(E_ItemTag::아루_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Aru.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아루의 엘레프", L"아루의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아스나_엘레프] = T_ItemInfo(E_ItemTag::아스나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Asuna.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아스나의 엘레프", L"아스나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아야네_엘레프] = T_ItemInfo(E_ItemTag::아야네_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Ayane.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아야네의 엘레프", L"아야네의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아이리_엘레프] = T_ItemInfo(E_ItemTag::아이리_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Airi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아이리의 엘레프", L"아이리의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::이즈나_엘레프] = T_ItemInfo(E_ItemTag::이즈나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Izuna.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"이즈나의 엘레프", L"이즈나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아카네_엘레프] = T_ItemInfo(E_ItemTag::아카네_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Akane.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아카네의 엘레프", L"아카네의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::아카리_엘레프] = T_ItemInfo(E_ItemTag::아카리_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Akari.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"아카리의 엘레프", L"아카리의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::에이미_엘레프] = T_ItemInfo(E_ItemTag::에이미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Eimi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"에이미의 엘레프", L"에이미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::요시미_엘레프] = T_ItemInfo(E_ItemTag::요시미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Yoshimi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"요시미의 엘레프", L"요시미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::우타하_엘레프] = T_ItemInfo(E_ItemTag::우타하_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Utaha.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"우타하의 엘레프", L"우타하의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::유우카_엘레프] = T_ItemInfo(E_ItemTag::유우카_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Yuuka.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"유우카의 엘레프", L"유우카의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::이오리_엘레프] = T_ItemInfo(E_ItemTag::이오리_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Iori.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"이오리의 엘레프", L"이오리의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::이즈미_엘레프] = T_ItemInfo(E_ItemTag::이즈미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Izumi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"이즈미의 엘레프", L"이즈미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::주리_엘레프] = T_ItemInfo(E_ItemTag::주리_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Juri.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"주리의 엘레프", L"주리의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::준코_엘레프] = T_ItemInfo(E_ItemTag::준코_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Zunko.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"준코의 엘레프", L"준코의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::츠루기_엘레프] = T_ItemInfo(E_ItemTag::츠루기_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Tsurugi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"츠루기의 엘레프", L"츠루기의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::츠바키_엘레프] = T_ItemInfo(E_ItemTag::츠바키_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Tsubaki.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"츠바키의 엘레프", L"츠바키의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::치나츠_엘레프] = T_ItemInfo(E_ItemTag::치나츠_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Chinatsu.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"치나츠의 엘레프", L"치나츠의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::치세_엘레프] = T_ItemInfo(E_ItemTag::치세_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Chise.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"치세의 엘레프", L"치세의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::카린_엘레프] = T_ItemInfo(E_ItemTag::카린_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Karin.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"카린의 엘레프", L"카린의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::카요코_엘레프] = T_ItemInfo(E_ItemTag::카요코_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Kayoko.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"카요코의 엘레프", L"카요코의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::코타마_엘레프] = T_ItemInfo(E_ItemTag::코타마_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Kotama.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"코타마의 엘레프", L"코타마의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::코토리_엘레프] = T_ItemInfo(E_ItemTag::코토리_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Kotori.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"코토리의 엘레프", L"코토리의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::피나_엘레프] = T_ItemInfo(E_ItemTag::피나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Pina.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"피나의 엘레프", L"피나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::하나에_엘레프] = T_ItemInfo(E_ItemTag::하나에_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hanae.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"하나에의 엘레프", L"하나에의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::하레_엘레프] = T_ItemInfo(E_ItemTag::하레_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hare.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"하레의 엘레프", L"하레의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::하루나_엘레프] = T_ItemInfo(E_ItemTag::하루나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Haruna.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"하루나의 엘레프", L"하루나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::하루카_엘레프] = T_ItemInfo(E_ItemTag::하루카_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Haruka.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"하루카의 엘레프", L"하루카의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::하스미_엘레프] = T_ItemInfo(E_ItemTag::하스미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hasumi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"하스미의 엘레프", L"하스미의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::호시노_엘레프] = T_ItemInfo(E_ItemTag::호시노_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hoshino.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"호시노의 엘레프", L"호시노의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::후우카_엘레프] = T_ItemInfo(E_ItemTag::후우카_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Fuuka.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"후우카의 엘레프", L"후우카의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::히나_엘레프] = T_ItemInfo(E_ItemTag::히나_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hina.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"히나의 엘레프", L"히나의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::히비키_엘레프] = T_ItemInfo(E_ItemTag::히비키_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hibiki.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"히비키의 엘레프", L"히비키의 신비가 담겨 있는 엘레프", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::히후미_엘레프] = T_ItemInfo(E_ItemTag::히후미_엘레프, L"../../Reference/Resource/Texture/UI/SecretStone/Item_Icon_SecretStone_Hihumi.png", E_Money::엘리그마, E_ItemFilter::엘레프, L"히후미의 엘레프", L"히후미의 신비가 담겨 있는 엘레프", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::선물_에이팟프로] = T_ItemInfo(E_ItemTag::선물_에이팟프로, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_2.png", E_Money::Money_End, E_ItemFilter::선물, L"에이~팟 프로", L"최근 쥬신의 학생들 사이에서 인기리에 유행중인 무선 이어폰. 용돈으로 사기엔 제법 비싸다.평균가 알바비 3달 치.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_금단의사랑] = T_ItemInfo(E_ItemTag::선물_금단의사랑, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_3.png", E_Money::Money_End, E_ItemFilter::선물, L"금단의 사랑", L"인기가 높은 연애 소설가 '정의훈' 선생님의 최신작! 맺어지는 것이 허용되지 않는 금단의 사랑을 다룬 문제작!", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_게임잡지] = T_ItemInfo(E_ItemTag::선물_게임잡지, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_4.png", E_Money::Money_End, E_ItemFilter::선물, L"게임 잡지", L"정보라면 그 어떤 사소한 것도 놓치지 않는다. 쥬신 최신 게임 발매 소식부터, 고전 게임 공략, 게임 굿즈 발매 소식 등이 수록된 게임 잡지", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_쿠키세트] = T_ItemInfo(E_ItemTag::선물_쿠키세트, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_8.png", E_Money::Money_End, E_ItemFilter::선물, L"고급스러운 쿠키 세트", L"부드러운 달콤함이 일품인 슈뢰딩거의 고급스러운 쿠키 세트. 쿠키 대신 다른 것이 들어 있을 확률도 나름대로 있다.열어 보지 않으면 현상은 확정되지 않는다", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_천체망원경] = T_ItemInfo(E_ItemTag::선물_천체망원경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_10.png", E_Money::Money_End, E_ItemFilter::선물, L"천체망원경", L"키보토스 공식천문관측협회에서 표준 관측용 취미 장비로 채택하고 있는 천체망원경.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_전투식량] = T_ItemInfo(E_ItemTag::선물_전투식량, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Event_Token_0.png", E_Money::Money_End, E_ItemFilter::선물, L"MX-레이션 C형 디저트 풍미", L"칼로리야말로 전투력!", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_말차맛라무네] = T_ItemInfo(E_ItemTag::선물_말차맛라무네, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_13.png", E_Money::Money_End, E_ItemFilter::선물, L"말차맛 라무네", L"독특한 풍미를 가진 말차맛 탄산음료. 초록빛의 상쾌함을 맛보세요!", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_젤리즈쿠션] = T_ItemInfo(E_ItemTag::선물_젤리즈쿠션, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_14.png", E_Money::Money_End, E_ItemFilter::선물, L"젤리즈 쿠션", L"고전 게임 '후루젤리' 시리즈의 주인공을 마침내 굿즈화! 같은 색 젤리가 젤리를 잡아먹는 기믹 또한 충실히 구현!", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_오피트] = T_ItemInfo(E_ItemTag::선물_오피트, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_16.png", E_Money::Money_End, E_ItemFilter::선물, L"O-오피트", L"O-링을 활용해, 간편하게 집에서 운동을 즐기기 위한 도구. 근력운동, 다이어트, 무엇이든 가능!", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::선물_곰인형] = T_ItemInfo(E_ItemTag::선물_곰인형, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Favor_20.png", E_Money::Money_End, E_ItemFilter::선물, L"리본이 달린 곰인형", L"평범하지만 사랑스러움이 느껴지는 곰인형. 귀여운 것을 좋아하는 학생들 사이에서는 제법 유명한 메이커 제품.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::강화석_설계도] = T_ItemInfo(E_ItemTag::강화석_설계도, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Enhance_D.png", E_Money::크레딧, E_ItemFilter::강화석, L"강화설계도", L"강화설계도", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::강화석_하급] = T_ItemInfo(E_ItemTag::강화석_하급, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Enhance_0.png", E_Money::크레딧, E_ItemFilter::강화석, L"강화석", L"강화석", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::강화석_일반] = T_ItemInfo(E_ItemTag::강화석_일반, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Enhance_1.png", E_Money::크레딧, E_ItemFilter::강화석, L"강화석", L"강화석", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::강화석_상급] = T_ItemInfo(E_ItemTag::강화석_상급, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Enhance_2.png", E_Money::크레딧, E_ItemFilter::강화석, L"강화석", L"강화석", 0, 500);
	(*pVecItemInfo)[(_int)E_ItemTag::강화석_최상급] = T_ItemInfo(E_ItemTag::강화석_최상급, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Enhance_3.png", E_Money::크레딧, E_ItemFilter::강화석, L"강화석", L"강화석", 0, 2000);

	(*pVecItemInfo)[(_int)E_ItemTag::아로나의_스탬프] = T_ItemInfo(E_ItemTag::아로나의_스탬프, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Event_Token_3.png", E_Money::크레딧, E_ItemFilter::코인, L"아로나의 스탬프", L"아로나가 준 스탬프다. 좋은걸로 바꿀 수 있을 것 같다.", 0, 2000);

	// 민현
	(*pVecItemInfo)[(_int)E_ItemTag::전술대회_코인] = T_ItemInfo(E_ItemTag::전술대회_코인, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_ArenaCoin.png", E_Money::크레딧, E_ItemFilter::코인, L"전술대회 코인", L"전술 대회 상점에서 사용 가능한 코인", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::총력전_코인] = T_ItemInfo(E_ItemTag::총력전_코인, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_RaidCoin.png", E_Money::크레딧, E_ItemFilter::코인, L"총력전 코인", L"총력전 상점에서 사용 가능한 코인", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::기초전술교육BD_선택권] = T_ItemInfo(E_ItemTag::기초전술교육BD_선택권, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Selection_0.png", E_Money::코인_총력전, E_ItemFilter::소모품, L"기초 전술 교육 BD 선택권", L"원하는 전술 교육 BD를 선택할 수 있는 선택권", 0, 1);
	(*pVecItemInfo)[(_int)E_ItemTag::기초기술노트_선택권] = T_ItemInfo(E_ItemTag::기초기술노트_선택권, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Selection_0.png", E_Money::코인_총력전, E_ItemFilter::소모품, L"기초 기술 노트 선택권", L"원하는 기술 노트를 선택할 수 있는 선택권", 0, 1);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_산해경] = T_ItemInfo(E_ItemTag::기초_기술노트_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Shanhaijing_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(산해경)", L"산해경 고급중학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_산해경] = T_ItemInfo(E_ItemTag::일반_기술노트_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Shanhaijing_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(산해경)", L"산해경 고급중학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_산해경] = T_ItemInfo(E_ItemTag::상급_기술노트_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Shanhaijing_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(산해경)", L"산해경 고급중학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_산해경] = T_ItemInfo(E_ItemTag::최상급_기술노트_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Shanhaijing_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(산해경)", L"산해경 고급중학교의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_밀레니엄] = T_ItemInfo(E_ItemTag::기초_기술노트_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Millennium_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(밀레니엄)", L"밀레니엄 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_밀레니엄] = T_ItemInfo(E_ItemTag::일반_기술노트_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Millennium_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(밀레니엄)", L"밀레니엄 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_밀레니엄] = T_ItemInfo(E_ItemTag::상급_기술노트_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Millennium_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(밀레니엄)", L"밀레니엄 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_밀레니엄] = T_ItemInfo(E_ItemTag::최상급_기술노트_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Millennium_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(밀레니엄)", L"밀레니엄 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_아비도스] = T_ItemInfo(E_ItemTag::기초_기술노트_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Abydos_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(아비도스)", L"아비도스 고등학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_아비도스] = T_ItemInfo(E_ItemTag::일반_기술노트_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Abydos_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(아비도스)", L"아비도스 고등학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_아비도스] = T_ItemInfo(E_ItemTag::상급_기술노트_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Abydos_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(아비도스)", L"아비도스 고등학교의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_아비도스] = T_ItemInfo(E_ItemTag::최상급_기술노트_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Abydos_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(아비도스)", L"아비도스 고등학교의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_게헨나] = T_ItemInfo(E_ItemTag::기초_기술노트_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Gehenna_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(게헨나)", L"게헨나 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_게헨나] = T_ItemInfo(E_ItemTag::일반_기술노트_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Gehenna_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(게헨나)", L"게헨나 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_게헨나] = T_ItemInfo(E_ItemTag::상급_기술노트_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Gehenna_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(게헨나)", L"게헨나 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_게헨나] = T_ItemInfo(E_ItemTag::최상급_기술노트_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Gehenna_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(게헨나)", L"게헨나 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_트리니티] = T_ItemInfo(E_ItemTag::기초_기술노트_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Trinity_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_트리니티] = T_ItemInfo(E_ItemTag::일반_기술노트_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Trinity_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_트리니티] = T_ItemInfo(E_ItemTag::상급_기술노트_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Trinity_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_트리니티] = T_ItemInfo(E_ItemTag::최상급_기술노트_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Trinity_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_백귀야행] = T_ItemInfo(E_ItemTag::기초_기술노트_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Hyakkiyako_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_백귀야행] = T_ItemInfo(E_ItemTag::일반_기술노트_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Hyakkiyako_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_백귀야행] = T_ItemInfo(E_ItemTag::상급_기술노트_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Hyakkiyako_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_백귀야행] = T_ItemInfo(E_ItemTag::최상급_기술노트_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_Hyakkiyako_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(트리니티)", L"트리니티 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_기술노트_겨울연방] = T_ItemInfo(E_ItemTag::기초_기술노트_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_RedWinter_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 기술 노트\n(겨울연방)", L"겨울연방 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_기술노트_겨울연방] = T_ItemInfo(E_ItemTag::일반_기술노트_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_RedWinter_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 기술 노트\n(겨울연방)", L"겨울연방 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_기술노트_겨울연방] = T_ItemInfo(E_ItemTag::상급_기술노트_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_RedWinter_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 기술 노트\n(겨울연방)", L"겨울연방 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_기술노트_겨울연방] = T_ItemInfo(E_ItemTag::최상급_기술노트_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_SkillBook_RedWinter_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 기술 노트\n(겨울연방)", L"겨울연방 학원의 전술 핵심이 요약된 비법 노트.", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_산해경] = T_ItemInfo(E_ItemTag::기초_전술교육BD_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Shanhaijing_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(산해경)", L"산해경 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_산해경] = T_ItemInfo(E_ItemTag::일반_전술교육BD_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Shanhaijing_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(산해경)", L"산해경 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_산해경] = T_ItemInfo(E_ItemTag::상급_전술교육BD_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Shanhaijing_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(산해경)", L"산해경 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_산해경] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_산해경, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Shanhaijing_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(산해경)", L"산해경 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_밀레니엄] = T_ItemInfo(E_ItemTag::기초_전술교육BD_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Millennium_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(밀레니엄)", L"밀레니엄 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_밀레니엄] = T_ItemInfo(E_ItemTag::일반_전술교육BD_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Millennium_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(밀레니엄)", L"밀레니엄 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_밀레니엄] = T_ItemInfo(E_ItemTag::상급_전술교육BD_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Millennium_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(밀레니엄)", L"밀레니엄 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_밀레니엄] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_밀레니엄, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Millennium_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(밀레니엄)", L"밀레니엄 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_아비도스] = T_ItemInfo(E_ItemTag::기초_전술교육BD_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Abydos_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(아비도스)", L"아비도스 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_아비도스] = T_ItemInfo(E_ItemTag::일반_전술교육BD_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Abydos_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(아비도스)", L"아비도스 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_아비도스] = T_ItemInfo(E_ItemTag::상급_전술교육BD_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Abydos_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(아비도스)", L"아비도스 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_아비도스] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_아비도스, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Abydos_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(아비도스)", L"아비도스 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_게헨나] = T_ItemInfo(E_ItemTag::기초_전술교육BD_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Gehenna_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(게헨나)", L"게헨나 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_게헨나] = T_ItemInfo(E_ItemTag::일반_전술교육BD_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Gehenna_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(게헨나)", L"게헨나 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_게헨나] = T_ItemInfo(E_ItemTag::상급_전술교육BD_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Gehenna_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(게헨나)", L"게헨나 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_게헨나] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_게헨나, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Gehenna_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(게헨나)", L"게헨나 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_트리니티] = T_ItemInfo(E_ItemTag::기초_전술교육BD_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Trinity_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_트리니티] = T_ItemInfo(E_ItemTag::일반_전술교육BD_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Trinity_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_트리니티] = T_ItemInfo(E_ItemTag::상급_전술교육BD_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Trinity_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_트리니티] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_트리니티, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Trinity_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_백귀야행] = T_ItemInfo(E_ItemTag::기초_전술교육BD_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Hyakkiyako_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_백귀야행] = T_ItemInfo(E_ItemTag::일반_전술교육BD_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Hyakkiyako_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_백귀야행] = T_ItemInfo(E_ItemTag::상급_전술교육BD_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Hyakkiyako_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_백귀야행] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_백귀야행, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_Hyakkiyako_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(트리니티)", L"트리니티 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);

		
	(*pVecItemInfo)[(_int)E_ItemTag::기초_전술교육BD_겨울연방] = T_ItemInfo(E_ItemTag::기초_전술교육BD_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_RedWinter_0.png", E_Money::코인_총력전, E_ItemFilter::재료, L"기초 전술 교육 BD\n(겨울연방)", L"겨울연방 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_전술교육BD_겨울연방] = T_ItemInfo(E_ItemTag::일반_전술교육BD_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_RedWinter_1.png", E_Money::코인_총력전, E_ItemFilter::재료, L"일반 전술 교육 BD\n(겨울연방)", L"겨울연방 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_전술교육BD_겨울연방] = T_ItemInfo(E_ItemTag::상급_전술교육BD_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_RedWinter_2.png", E_Money::코인_총력전, E_ItemFilter::재료, L"상급 전술 교육 BD\n(겨울연방)", L"겨울연방 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_전술교육BD_겨울연방] = T_ItemInfo(E_ItemTag::최상급_전술교육BD_겨울연방, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_ExSkill_RedWinter_3.png", E_Money::코인_총력전, E_ItemFilter::재료, L"최상급 전술 교육 BD\n(겨울연방)", L"겨울연방 학생들의 전수 활동을 영상으로 기록한 교육용 BD", 0, 0);


	(*pVecItemInfo)[(_int)E_ItemTag::기초_활동보고서] = T_ItemInfo(E_ItemTag::기초_활동보고서, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_ExpItem_0.png", E_Money::코인_전술대회, E_ItemFilter::재료, L"기초_활동보고서", L"학생들의 동아리 활동 기록이 담겨 있는 보고서. 수많은 학생들의\n경험이 녹아들어 있다.", 1000, 10);
	(*pVecItemInfo)[(_int)E_ItemTag::일반_활동보고서] = T_ItemInfo(E_ItemTag::일반_활동보고서, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_ExpItem_1.png", E_Money::코인_전술대회, E_ItemFilter::재료, L"일반_활동보고서", L"학생들의 동아리 활동 기록이 담겨 있는 보고서. 수많은 학생들의\n경험이 녹아들어 있다.", 100, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::상급_활동보고서] = T_ItemInfo(E_ItemTag::상급_활동보고서, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_ExpItem_2.png", E_Money::코인_전술대회, E_ItemFilter::재료, L"상급_활동보고서", L"학생들의 동아리 활동 기록이 담겨 있는 보고서. 수많은 학생들의\n경험이 녹아들어 있다.", 10, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::최상급_활동보고서] = T_ItemInfo(E_ItemTag::최상급_활동보고서, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_ExpItem_3.png", E_Money::코인_전술대회, E_ItemFilter::재료, L"최상급_활동보고서", L"학생들의 동아리 활동 기록이 담겨 있는 보고서. 수많은 학생들의\n경험이 녹아들어 있다.", 3, 500);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_수정_하니와] = T_ItemInfo(E_ItemTag::온전한_수정_하니와, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_CrystalHaniwa_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 수정 하니와", L"수정 하니와이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_수정_하니와] = T_ItemInfo(E_ItemTag::마모된_수정_하니와, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_CrystalHaniwa_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 수정 하니와", L"수정 하니와이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_수정_하니와] = T_ItemInfo(E_ItemTag::파손된_수정_하니와, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_CrystalHaniwa_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 수정 하니와", L"수정 하니와이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::수정_하니와_조각] = T_ItemInfo(E_ItemTag::수정_하니와_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_CrystalHaniwa_0.png", E_Money::크레딧, E_ItemFilter::재료, L"수정 하니와 조각	", L"수정 하니와이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_보이니치_사본] = T_ItemInfo(E_ItemTag::온전한_보이니치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Voynich_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 보이니치 사본", L"유명한 보이니치의 사본이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_보이니치_사본] = T_ItemInfo(E_ItemTag::마모된_보이니치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Voynich_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 보이니치 사본", L"유명한 보이니치의 사본이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_보이니치_사본] = T_ItemInfo(E_ItemTag::파손된_보이니치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Voynich_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 보이니치 사본", L"유명한 보이니치의 사본이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::보이니치_사본_조각] = T_ItemInfo(E_ItemTag::보이니치_사본_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Voynich_0.png", E_Money::크레딧, E_ItemFilter::재료, L"보이니치 사본 조각 ", L"유명한 보이니치의 사본이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_안티키테라_장치] = T_ItemInfo(E_ItemTag::온전한_안티키테라_장치, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Antikythera_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 안티키테라 장치", L"안티키테라의 장치이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_안티키테라_장치] = T_ItemInfo(E_ItemTag::마모된_안티키테라_장치, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Antikythera_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 안티키테라 장치", L"안티키테라의 장치이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_안티키테라_장치] = T_ItemInfo(E_ItemTag::파손된_안티키테라_장치, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Antikythera_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 안티키테라 장치", L"안티키테라의 장치이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::안티키테라_장치_조각] = T_ItemInfo(E_ItemTag::안티키테라_장치_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Antikythera_0.png", E_Money::크레딧, E_ItemFilter::재료, L"안티키테라 장치 조각 ", L"안티키테라의 장치이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_에테르_조각] = T_ItemInfo(E_ItemTag::온전한_에테르_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Ether_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 에테르 조각", L"에테르 가루를 압착해 만든 에테르 조각이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_에테르_조각] = T_ItemInfo(E_ItemTag::마모된_에테르_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Ether_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 에테르 조각", L"에테르 가루를 압착해 만든 에테르 조각이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_에테르_조각] = T_ItemInfo(E_ItemTag::파손된_에테르_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Ether_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 에테르 조각", L"에테르 가루를 압착해 만든 에테르 조각이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::에테르_조각_조각] = T_ItemInfo(E_ItemTag::에테르_조각_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Ether_0.png", E_Money::크레딧, E_ItemFilter::재료, L"에테르 조각 조각 ", L"에테르 가루를 압착해 만든 에테르 조각이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_로혼치_사본] = T_ItemInfo(E_ItemTag::온전한_로혼치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Rohonc_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 로혼치 사본", L"곳곳에 낙서가 가득한 로혼치 사본이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_로혼치_사본] = T_ItemInfo(E_ItemTag::마모된_로혼치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Rohonc_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 로혼치 사본", L"곳곳에 낙서가 가득한 로혼치 사본이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_로혼치_사본] = T_ItemInfo(E_ItemTag::파손된_로혼치_사본, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Rohonc_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 로혼치 사본", L"곳곳에 낙서가 가득한 로혼치 사본이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::로혼치_사본_조각] = T_ItemInfo(E_ItemTag::로혼치_사본_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Rohonc_0.png", E_Money::크레딧, E_ItemFilter::재료, L"로혼치 사본 조각 ", L"곳곳에 낙서가 가득한 로혼치 사본이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_만드라고라] = T_ItemInfo(E_ItemTag::온전한_만드라고라, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Mandragora_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 만드라고라   ", L"단단하게 여문 만드라고라다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::커다란_만드라고라] = T_ItemInfo(E_ItemTag::커다란_만드라고라, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Mandragora_2.png", E_Money::크레딧, E_ItemFilter::재료, L"커다란 만드라고라   ", L"단단하게 여문 만드라고라다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::만드라고라_새싹] = T_ItemInfo(E_ItemTag::만드라고라_새싹, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Mandragora_1.png", E_Money::크레딧, E_ItemFilter::재료, L"만드라고라 새싹     ", L"단단하게 여문 만드라고라다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::만드라고라_씨앗_조각] = T_ItemInfo(E_ItemTag::만드라고라_씨앗_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Mandragora_0.png", E_Money::크레딧, E_ItemFilter::재료, L"만드라고라 씨앗 조각", L"단단하게 여문 만드라고라다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_님루드_렌즈] = T_ItemInfo(E_ItemTag::온전한_님루드_렌즈, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nimrud_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 님루드 렌즈", L"님루드사의 렌즈이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_님루드_렌즈] = T_ItemInfo(E_ItemTag::마모된_님루드_렌즈, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nimrud_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 님루드 렌즈", L"님루드사의 렌즈이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_님루드_렌즈] = T_ItemInfo(E_ItemTag::파손된_님루드_렌즈, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nimrud_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 님루드 렌즈", L"님루드사의 렌즈이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::님루드_렌즈_조각] = T_ItemInfo(E_ItemTag::님루드_렌즈_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nimrud_0.png", E_Money::크레딧, E_ItemFilter::재료, L"님루드 렌즈 조각 ", L"님루드사의 렌즈이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::고순도_볼프세크_철조각] = T_ItemInfo(E_ItemTag::고순도_볼프세크_철조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Wolfsegg_3.png", E_Money::크레딧, E_ItemFilter::재료, L"고순도 볼프세크", L"제련을 통해 만들어진 볼프세크 강철. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::저순도_볼프세크_철조각] = T_ItemInfo(E_ItemTag::저순도_볼프세크_철조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Wolfsegg_2.png", E_Money::크레딧, E_ItemFilter::재료, L"저순도 볼프세크", L"제련을 통해 만들어진 볼프세크 강철. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::볼프세크_철광석] = T_ItemInfo(E_ItemTag::볼프세크_철광석, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Wolfsegg_1.png", E_Money::크레딧, E_ItemFilter::재료, L"볼프세크 철광석", L"제련중 떨어진 볼프세크 철광석. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::볼프세크_철조각] = T_ItemInfo(E_ItemTag::볼프세크_철조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Wolfsegg_0.png", E_Money::크레딧, E_ItemFilter::재료, L"볼프세크 철조각", L"제련중 떨어진 볼프세크 조각. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_파에스토스_원반] = T_ItemInfo(E_ItemTag::온전한_파에스토스_원반, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Phaistos_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 파에스토스 원반", L"파에스토스의 원반이다. 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_파에스토스_원반] = T_ItemInfo(E_ItemTag::마모된_파에스토스_원반, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Phaistos_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 파에스토스 원반", L"파에스토스의 원반이다. 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_파에스토스_원반] = T_ItemInfo(E_ItemTag::파손된_파에스토스_원반, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Phaistos_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 파에스토스 원반", L"파에스토스의 원반이다. 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::파에스토스_원반_조각] = T_ItemInfo(E_ItemTag::파에스토스_원반_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Phaistos_0.png", E_Money::크레딧, E_ItemFilter::재료, L"파에스토스 원반 조각", L"파에스토스의 원반이다. 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::온전한_네브라_디스크] = T_ItemInfo(E_ItemTag::온전한_네브라_디스크, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nebra_3.png", E_Money::크레딧, E_ItemFilter::재료, L"온전한 네브라 디스크", L"네브라 디스크에서 입수한 디스크. . 각종 제조 및 강화에 사용된다.", 0, 100);
	(*pVecItemInfo)[(_int)E_ItemTag::마모된_네브라_디스크] = T_ItemInfo(E_ItemTag::마모된_네브라_디스크, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nebra_2.png", E_Money::크레딧, E_ItemFilter::재료, L"마모된 네브라 디스크", L"네브라 디스크에서 입수한 디스크. . 각종 제조 및 강화에 사용된다.", 0, 50);
	(*pVecItemInfo)[(_int)E_ItemTag::파손된_네브라_디스크] = T_ItemInfo(E_ItemTag::파손된_네브라_디스크, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nebra_1.png", E_Money::크레딧, E_ItemFilter::재료, L"파손된 네브라 디스크", L"네브라 디스크에서 입수한 디스크. . 각종 제조 및 강화에 사용된다.", 0, 20);
	(*pVecItemInfo)[(_int)E_ItemTag::네브라_디스크_조각] = T_ItemInfo(E_ItemTag::네브라_디스크_조각, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_Material_Nebra_0.png", E_Money::크레딧, E_ItemFilter::재료, L"네브라 디스크 조각", L"네브라 디스크에서 입수한 디스크. . 각종 제조 및 강화에 사용된다.", 0, 10);

	(*pVecItemInfo)[(_int)E_ItemTag::부스터_티켓] = T_ItemInfo(E_ItemTag::부스터_티켓, L"../../Reference/Resource/Texture/UI/Item/Item_Icon_CraftSkipItem.png", E_Money::청휘석, E_ItemFilter::재료, L"부스터 티켓", L"크래프트 챔버의 제작 소요 시간을 없애주는 부스터 티켓.", 0, 0);

	// 테스트용 
	for (_int i = 0; i < 169; i++)
	{
		if ((*pVecItemInfo)[i].wstrImagePath == L"")
		{
			CUtility::Print_Wstring_DMode(to_wstring(i));
		}

		(*pVecItemInfo)[i].iCount += CWall512::GetInstance()->GetValue(0, 100);

	}
}

// 게임 시작하면서 모든 장비아이템의 값을 넣어준다.
void CDataManager::Initialize_Equip(vector<T_EquipInfo>* pVecEquipInfo)
{
	(*pVecEquipInfo).reserve((_int)E_EquipTag::Equip_End);
	(*pVecEquipInfo).resize((_int)E_EquipTag::Equip_End);

	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_무지캡모자] = T_EquipInfo(E_EquipTag::Hat_무지캡모자, E_EquipType::EquipType_Attack, E_EquipTier::Tier_1, L"무지 캡모자", L"스포츠웨어 브랜드, <서벌>에서 출시한 남색의 무지 캡모자. 다양한 종류의 배지를 달아 개성을 강조할 수 있기 때문에 패션용으로도 애용되고 있다.", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_니트털모자] = T_EquipInfo(E_EquipTag::Hat_니트털모자, E_EquipType::EquipType_Attack, E_EquipTier::Tier_2, L"니트 털모자", L"아웃도어 브랜드, <마나슬루>에서 여고생들을 겨냥하여 만든 니트 털모자. 보온성은 다소 떨어지지만 귀여운 디자인 덕분에 인기가 높다.", 0, 10.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_빅브라더페도라] = T_EquipInfo(E_EquipTag::Hat_빅브라더페도라, E_EquipType::EquipType_Attack, E_EquipTier::Tier_3, L"빅 브라더 페도라", L"<모모 프렌즈>의 캐릭터, 빅 브라더 인형이 달린 작은 페도라. 이 모자를 쓰면 갑자기 빅 브라더의 시선이 느껴진다고 한다.", 0, 15.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_리본베레모] = T_EquipInfo(E_EquipTag::Hat_리본베레모, E_EquipType::EquipType_Attack, E_EquipTier::Tier_4, L"리본 베레모", L"트리니티 학원의 교모로도 사용되는 흰색의 리본 베레모. 착용자의 말에 의하면 베레모의 볼륨감을 예쁘게 살리는 것이 생각보다 어렵다고 한다.", 0, 20.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_방탄헬멧] = T_EquipInfo(E_EquipTag::Hat_방탄헬멧, E_EquipType::EquipType_Attack, E_EquipTier::Tier_5, L"방탄 헬멧", L"머리를 향해 날아오는 온갖 물건을 막아낼 수 있는 튼튼한 방탄 헬멧. 외피는 부드러운 면 소재로 제작되어 패치 등을 붙여 꾸밀 수 있다.", 0, 27.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hat_프릴미니햇] = T_EquipInfo(E_EquipTag::Hat_프릴미니햇, E_EquipType::EquipType_Attack, E_EquipTier::Tier_6, L"프릴 미니햇", L"", 0, 5.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_스포츠용장갑] = T_EquipInfo(E_EquipTag::Gloves_스포츠용장갑, E_EquipType::EquipType_Attack, E_EquipTier::Tier_1, L"스포츠용 장갑", L"쉽게 미끄러지지 않도록 울퉁불퉁한 겉감을 안쪽에 덧댄 스포츠용 장갑. 재질에 난연소재가 섞여있어 뜨거운 총신을 만질 때도 안심할 수 있다.", 0, 4.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_니트벙어리장갑] = T_EquipInfo(E_EquipTag::Gloves_니트벙어리장갑, E_EquipType::EquipType_Attack, E_EquipTier::Tier_2, L"니트 벙어리 장갑", L"아웃도어 브랜드, <마나슬루>에서 여고생들을 겨냥하여 만든 니트 벙어리 장갑. 보온성은 다소 떨어지지만 귀여운 디자인 덕분에 인기가 높다.", 0, 8.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_페로로오븐장갑] = T_EquipInfo(E_EquipTag::Gloves_페로로오븐장갑, E_EquipType::EquipType_Attack, E_EquipTier::Tier_3, L"페로로 오븐 장갑", L"<모모 프렌즈>의 마스코트 캐릭터, 페로로의 디자인을 따 만들어진 흰색의 오븐 장갑. 이 오븐 장갑을 끼면 갑자기 괴상한 댄스를 추고 싶은 욕망에 휩싸이게 된다는 소문이 있다.", 0, 12.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_가죽글러브] = T_EquipInfo(E_EquipTag::Gloves_가죽글러브, E_EquipType::EquipType_Attack, E_EquipTier::Tier_4, L"가죽 글러브", L"고급 가죽 원단을 사용하여 만들어진 가죽 글러브. 내구도가 그리 높지 않기 때문에 주로 예식용으로 사용된다.", 0, 16.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_택티컬글러브] = T_EquipInfo(E_EquipTag::Gloves_택티컬글러브, E_EquipType::EquipType_Attack, E_EquipTier::Tier_5, L"택티컬 글러브", L"방염 소재를 사용하여 튼튼하게 만들어진 택티컬 글러브. 뜨겁게 가열된 총신을 붙잡아도 데일 위험이 없다.", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Gloves_레이스장갑] = T_EquipInfo(E_EquipTag::Gloves_레이스장갑, E_EquipType::EquipType_Attack, E_EquipTier::Tier_6, L"레이스 장갑", L"", 0, 5.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_핑크스니커즈] = T_EquipInfo(E_EquipTag::Shoes_핑크스니커즈, E_EquipType::EquipType_Attack, E_EquipTier::Tier_1, L"핑크 스니커즈", L"", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_어그부츠] = T_EquipInfo(E_EquipTag::Shoes_어그부츠, E_EquipType::EquipType_Attack, E_EquipTier::Tier_2, L"어그 부츠", L"", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_핑키파카슬리퍼] = T_EquipInfo(E_EquipTag::Shoes_핑키파카슬리퍼, E_EquipType::EquipType_Attack, E_EquipTier::Tier_3, L"핑키파카 슬리퍼", L"", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_앤티크에나멜로퍼] = T_EquipInfo(E_EquipTag::Shoes_앤티크에나멜로퍼, E_EquipType::EquipType_Attack, E_EquipTier::Tier_4, L"앤티크 에나멜 로퍼", L"", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_전술부츠] = T_EquipInfo(E_EquipTag::Shoes_전술부츠, E_EquipType::EquipType_Attack, E_EquipTier::Tier_5, L"전술 부츠", L"", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Shoes_힐펌프스] = T_EquipInfo(E_EquipTag::Shoes_힐펌프스, E_EquipType::EquipType_Attack, E_EquipTier::Tier_6, L"힐 펌프스", L"", 0, 5.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_서벌금속배지] = T_EquipInfo(E_EquipTag::Badge_서벌금속배지, E_EquipType::EquipType_HP, E_EquipTier::Tier_1, L"서벌 금속 배지", L"", 0, 500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_마나슬루펠트배지] = T_EquipInfo(E_EquipTag::Badge_마나슬루펠트배지, E_EquipType::EquipType_HP, E_EquipTier::Tier_2, L"마나슬루 펠트 배지", L"아웃도어 전문 브랜드 <마나슬루>의 로고가 인쇄된 펠트 배지. 부드러운 펠트로 제작되어 방한용품과도 안성맞춤.", 0, 1000.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_앵그리아델리원형배지] = T_EquipInfo(E_EquipTag::Badge_앵그리아델리원형배지, E_EquipType::EquipType_HP, E_EquipTier::Tier_3, L"앵그리 아델리 원형 배지", L"<모모 프렌즈>의 앵그리 아델리가 그려진 원형 배지. 오랫동안 쳐다보고 있노라면 어쩐지 기분이 나빠진다…….", 0, 1500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_베로니카자수배지] = T_EquipInfo(E_EquipTag::Badge_베로니카자수배지, E_EquipType::EquipType_HP, E_EquipTier::Tier_4, L"베로니카 자수 배지", L"", 0, 2000.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_카제야마벨크로패치] = T_EquipInfo(E_EquipTag::Badge_카제야마벨크로패치, E_EquipType::EquipType_HP, E_EquipTier::Tier_5, L"카제야마 벨크로 패치", L"", 0, 3500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Badge_코코데빌배지] = T_EquipInfo(E_EquipTag::Badge_코코데빌배지, E_EquipType::EquipType_HP, E_EquipTier::Tier_6, L"코코데빌 배지", L"", 0, 4750.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_방수스포츠백] = T_EquipInfo(E_EquipTag::Bag_방수스포츠백, E_EquipType::EquipType_HP, E_EquipTier::Tier_1, L"방수 스포츠백", L"<서벌>에서 제작된 방수 스포츠백. 물에 젖지 않는 소재로 만들어졌기 때문에 강이나 바다에 가져가도 안심할 수 있다.", 0, 375.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_방한용크로스백] = T_EquipInfo(E_EquipTag::Bag_방한용크로스백, E_EquipType::EquipType_HP, E_EquipTier::Tier_2, L"방한용 크로스백", L"<마나슬루>에서 제작한 방한용 크로스백. 두터운 안감이 덧대어져 있어 도시락이나 음료 등을 식지않게 보온해준다.", 0, 750.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_페로로백팩] = T_EquipInfo(E_EquipTag::Bag_페로로백팩, E_EquipType::EquipType_HP, E_EquipTier::Tier_3, L"페로로 백팩", L"거대한 페로로가 인쇄된 백팩. 수납공간이 넓어 페로로 매니아가 아닌 사람들에게도 큰 인기를 끌고 있다.", 0, 1125.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_감색스쿨백] = T_EquipInfo(E_EquipTag::Bag_감색스쿨백, E_EquipType::EquipType_HP, E_EquipTier::Tier_4, L"감색 스쿨백", L"트리니티 학원의 교칙에 맞춰 제작된 감색의 스쿨백. 심플하고 고급스러운 디자인 덕분에 다른 학교의 학생들에게도 인기가 높다.", 0, 1500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_전술란도셀] = T_EquipInfo(E_EquipTag::Bag_전술란도셀, E_EquipType::EquipType_HP, E_EquipTier::Tier_5, L"전술 란도셀", L"", 0, 3500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Bag_데빌윙토트백] = T_EquipInfo(E_EquipTag::Bag_데빌윙토트백, E_EquipType::EquipType_HP, E_EquipTier::Tier_6, L"데빌 윙 토트백", L"", 0, 3750.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_테니스헤어밴드] = T_EquipInfo(E_EquipTag::Hairclip_테니스헤어밴드, E_EquipType::EquipType_HP, E_EquipTier::Tier_1, L"테니스 헤어밴드", L"스포츠웨어 브랜드, <서벌>의 테니스 헤어밴드. 운동 중 땀이 흘러내리는 것을 방지할 수 있도록 흡습력이 높은 소재로 만들어졌다.", 0, 250.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_헤어슈슈] = T_EquipInfo(E_EquipTag::Hairclip_헤어슈슈, E_EquipType::EquipType_HP, E_EquipTier::Tier_2, L"헤어 슈슈", L"새틴 재질의 탄력성이 좋은 슈슈. 따듯한 국물 음식을 먹을 때는 이 슈슈로 머리를 묶어두는 게 좋다.", 0, 500.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_모모헤어핀] = T_EquipInfo(E_EquipTag::Hairclip_모모헤어핀, E_EquipType::EquipType_HP, E_EquipTier::Tier_3, L"모모 헤어핀", L"<모모톡>의 아이콘인 복숭아 모양의 브로치가 달린 헤어핀. 브로치를 문지르면 은은한 복숭아 향이 난다.", 0, 750.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_날개헤어핀] = T_EquipInfo(E_EquipTag::Hairclip_날개헤어핀, E_EquipType::EquipType_HP, E_EquipTier::Tier_4, L"날개 헤어핀", L"명품 브랜드 <베로니카>에서 천사의 날개를 모티브로 삼아 제작한 헤어핀. 정작 트리니티 학생들에게는 인기가 없는 편이다.", 0, 1000.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_다목적헤어핀] = T_EquipInfo(E_EquipTag::Hairclip_다목적헤어핀, E_EquipType::EquipType_HP, E_EquipTier::Tier_5, L"다목적 헤어핀", L"", 0, 2000.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Hairclip_코우모리헤어핀] = T_EquipInfo(E_EquipTag::Hairclip_코우모리헤어핀, E_EquipType::EquipType_HP, E_EquipTier::Tier_6, L"코우모리 헤어핀", L"", 0, 3250.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Necklace_블루투스목걸이] = T_EquipInfo(E_EquipTag::Necklace_블루투스목걸이, E_EquipType::EquipType_Utility, E_EquipTier::Tier_1, L"블루투스 목걸이", L"착용자의 체온이나 심박수 등을 측정할 수 있는 블루투스 목걸이. 운동량을 쉽게 파악할 수 있기 때문에 다이어트를 시도하는 사람들에게는 필수품처럼 애용된다.", 0, 5.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Necklace_눈꽃팬던트] = T_EquipInfo(E_EquipTag::Necklace_눈꽃팬던트, E_EquipType::EquipType_Utility, E_EquipTier::Tier_2, L"눈꽃 펜던트", L"눈꽃 모양의 장식이 달린 은색의 펜던트. 눈꽃 장식을 오래 쳐다보고 있으면 어쩐지 서늘한 느낌이 든다.", 0, 10.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Necklace_니콜라이로켓] = T_EquipInfo(E_EquipTag::Necklace_니콜라이로켓, E_EquipType::EquipType_Utility, E_EquipTier::Tier_3, L"니콜라이 로켓", L"<모모 프렌즈>의 캐릭터 Mr.니콜라이의 얼굴이 매달린 로켓. 코 부분을 누르면 사진을 넣을 수 있는 작은 수납 공간이 열린다.", 0, 15.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Necklace_십자가초커] = T_EquipInfo(E_EquipTag::Necklace_십자가초커, E_EquipType::EquipType_Utility, E_EquipTier::Tier_4, L"십자가 초커", L"명품 브랜드 <베로니카>에서 출시된 십자가 장식이 달린 초커. 심플하지만 세련된 멋이 담긴 아이템으로 인기가 높다.", 0, 20.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Necklace_군번줄] = T_EquipInfo(E_EquipTag::Necklace_군번줄, E_EquipType::EquipType_Utility, E_EquipTier::Tier_5, L"군번줄", L"이하 생략", 0, 26.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Charm_교통안전부적] = T_EquipInfo(E_EquipTag::Charm_교통안전부적, E_EquipType::EquipType_Utility, E_EquipTier::Tier_1, L"교통안전 부적", L"교통안전을 기원하는 마음을 담아 만든 부적. 흔들어보면 금속제의 무언가가 안에서 짤랑거리는 소리가 난다.", 0, 50.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Charm_발열팩] = T_EquipInfo(E_EquipTag::Charm_발열팩, E_EquipType::EquipType_Utility, E_EquipTier::Tier_2, L"발열팩", L"몇 번 주무르고 흔들면 따듯한 온기를 발산하는 손바닥 크기의 발열팩. 혹한의 환경에서 이보다 더 쓸모있는 부적은 없다.", 0, 100.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Charm_페로로의깃털] = T_EquipInfo(E_EquipTag::Charm_페로로의깃털, E_EquipType::EquipType_Utility, E_EquipTier::Tier_3, L"페로로의 깃털", L"커다랗고 하얀 깃털을 달아 만든 부적. 깃털의 출처는 모모 프렌즈의 중대한 영업 기밀이다.", 0, 150.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Charm_십자가] = T_EquipInfo(E_EquipTag::Charm_십자가, E_EquipType::EquipType_Utility, E_EquipTier::Tier_4, L"십자가", L"트리니티 학생들이 애용하는 십자가 형태의 부적. 가방에 매다는 액세서리로도 잘 어울린다.", 0, 200.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Charm_카모플라쥬다루마] = T_EquipInfo(E_EquipTag::Charm_카모플라쥬다루마, E_EquipType::EquipType_Utility, E_EquipTier::Tier_5, L"카모플라쥬 다루마", L"", 0, 260.f);

	(*pVecEquipInfo)[(_int)E_EquipTag::Watch_방수디지털시계] = T_EquipInfo(E_EquipTag::Watch_방수디지털시계, E_EquipType::EquipType_Utility, E_EquipTier::Tier_1, L"방수 디지털 시계", L"스포츠웨어 브랜드 <서벌>에서 제작된 디지털 시계. 방수 사양이기 때문에 수영을 하는 동안 착용하고 있어도 고장날 염려가 없다.", 0, 50.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Watch_가죽손목시계] = T_EquipInfo(E_EquipTag::Watch_가죽손목시계, E_EquipType::EquipType_Utility, E_EquipTier::Tier_2, L"가죽 손목 시계", L"아웃도어 브랜드 <마나슬루>에서 제작된 가죽 재질의 손목 시계. 가죽으로 마감된 안감이 손목이 차가워지는 것을 방지한다.", 0, 100.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Watch_웨이브캣손목시계] = T_EquipInfo(E_EquipTag::Watch_웨이브캣손목시계, E_EquipType::EquipType_Utility, E_EquipTier::Tier_3, L"웨이브캣 손목 시계", L"<모모 프렌즈>의 고양이 캐릭터 웨이브캣이 그려진 손목 시계. 시간에 맞추어 시계 바늘 역할을 하는 웨이브캣의 양팔이 기괴하게 뒤틀린다.", 0, 150.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Watch_엔티크회중시계] = T_EquipInfo(E_EquipTag::Watch_엔티크회중시계, E_EquipType::EquipType_Utility, E_EquipTier::Tier_4, L"앤티크 회중 시계", L"<베로니카>의 로고가 새겨진 앤티크 풍의 회중 시계. 손목시계에 비해 시간을 확인하기 불편하다는 단점이 있지만, 패션 아이템으로 인기가 높다.", 0, 200.f);
	(*pVecEquipInfo)[(_int)E_EquipTag::Watch_방진형손목시계] = T_EquipInfo(E_EquipTag::Watch_방진형손목시계, E_EquipType::EquipType_Utility, E_EquipTier::Tier_5, L"방진형 손목 시계", L"", 0, 260.f);

}

// 게임 시작하면서 모든 가구아이템의 값을 넣어준다.
void CDataManager::Initialize_Furniture(vector<T_FurnitureInfo>* pVecFurnitureInfo)
{
	(*pVecFurnitureInfo).reserve((_int)E_FurnitureTag::Furniture_End);
	(*pVecFurnitureInfo).resize((_int)E_FurnitureTag::Furniture_End);

	//Default
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목의자]	=						T_FurnitureInfo(E_FurnitureTag::원목의자,						E_FurnitureStar::STAR_1,	L"원목 의자",							L"마감이 우수한 원목 의자. 습기와 오염에 강하여 오래 사용할 수 있다.", 0, 20,											L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Chair_01.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"원목 의자.X",								L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목보조의자] =					T_FurnitureInfo(E_FurnitureTag::원목보조의자,						E_FurnitureStar::STAR_1,	L"원목 보조의자",							L"단단한 보조 의자. 나사를 강하게 조일수록 더 안전하게 사용할 수 있다.", 0, 20,											L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Chair_02.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"원목 보조의자.X",							L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목수납장] =						T_FurnitureInfo(E_FurnitureTag::원목수납장,						E_FurnitureStar::STAR_1,	L"원목 수납장",							L"넓은 수납공간이 장점인 원목 수납장. 자랑하고 싶은 물건은 보여주고 감추고 싶은 물건은 숨길 수 있다.", 0, 20,				L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Bookshelf.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"원목 수납장.X",							L"가구",				L"옷장");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목수납테이블] =					T_FurnitureInfo(E_FurnitureTag::원목수납테이블,					E_FurnitureStar::STAR_1,	L"원목 수납 테이블",						L"작은 물건을 보관하기 좋은 수납 테이블. 서랍이 있어 테이블 위가 깔끔해진다.", 0, 20,										L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Sidetable.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"원목 수납 테이블.X"	,						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목대형테이블] =					T_FurnitureInfo(E_FurnitureTag::원목대형테이블,					E_FurnitureStar::STAR_1,	L"원목 대형 테이블",						L"클래식한 디자인의 테이블. 원목 소재가 오래 지속되는 아름다움을 준다.", 0, 20,											L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Table.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"원목 대형 테이블.X"	,						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::파랑색심플카펫] =					T_FurnitureInfo(E_FurnitureTag::파랑색심플카펫,					E_FurnitureStar::STAR_2,	L"파랑색 심플 카펫",						L"심플한 파랑색 카펫. 색과 형태가 모든 공간에 완벽하게 어울린다.", 0, 120,												L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Carpet.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"[C]파랑색 심플 카펫.X",						L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::화이트보드] =						T_FurnitureInfo(E_FurnitureTag::화이트보드,						E_FurnitureStar::STAR_2,	L"화이트보드",							L"메모가 붙은 화이트보드. 집중해서 보면 학생들의 사소한 일상을 알 수 있다.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Board.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"[W]화이트보드.X",							L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::커튼창문] =						T_FurnitureInfo(E_FurnitureTag::커튼창문,						E_FurnitureStar::STAR_1,	L"커튼 창문",							L"커튼이 달린 창문. 창밖에서 바람이 들어오면 평화로움이 느껴진다.", 0, 20,												L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Window.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Default/",						L"[W]커튼 창문.X",							L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::내츄럴나무바닥] =					T_FurnitureInfo(E_FurnitureTag::내츄럴나무바닥,					E_FurnitureStar::STAR_1,	L"내츄럴 나무 바닥",						L"나무 바닥. 따뜻하고 자연스러운 느낌을 준다.", 0, 20,																	L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Tile.png",						L"../../Reference/Resource/Map/x[Cafe]x_Default/",							L"내츄럴 나무 바닥.X"	,						L"내장",				L"바닥");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::내츄럴나무벽] =					T_FurnitureInfo(E_FurnitureTag::내츄럴나무벽,						E_FurnitureStar::STAR_2,	L"내츄럴 나무 벽",						L"내츄럴한 나무 벽. 포인트 컬러가 지루함 없이 단정한 공간을 만들어준다.", 0, 120,											L"../../Reference/Resource/Texture/UI/Furniture/Default/My_Defaultroom_Wall_Right.png",					L"../../Reference/Resource/Map/x[Cafe]x_Default/",							L"내츄럴 나무 벽_Right.X"	,					L"내장",				L"벽지");

	//Momo
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::바리스타페로로인형] =				T_FurnitureInfo(E_FurnitureTag::바리스타페로로인형,				E_FurnitureStar::STAR_3,	L"바리스타 페로로 인형",					L"거대한 페로로 인형. 팬케이크를 들고 있는 바리스타 페로로가 의아한 느낌을 준다.", 0, 350,								L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Peroroacc_01.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"바리스타 페로로 인형.X",					L"장식",				L"소품");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::모모프렌즈굿즈가포함된수납케이스] = T_FurnitureInfo(E_FurnitureTag::모모프렌즈굿즈가포함된수납케이스,	E_FurnitureStar::STAR_3,	L"모모프렌즈 굿즈가 포함된 수납케이스",	L"모모프렌즈의 대표 굿즈가 포함된 정리함. 인형 커피 문방구 등등 뭐든 들어있다.", 0, 350,									L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Shelf_01.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"모모프렌즈 굿즈가 포함된 수납케이스.X",		L"가구",				L"옷장");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::모모프렌즈네온전광판] =			T_FurnitureInfo(E_FurnitureTag::모모프렌즈네온전광판,				E_FurnitureStar::STAR_2,	L"모모프렌즈 네온 전광판",				L"노란 포인트의 네온 전광판. 페로로 얼굴로 벽을 멋지게 장식할 수 있다.", 0, 120,											L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wallacc_01.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"[W]모모프렌즈 네온 전광판.X",				L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::우드카운터바] =					T_FurnitureInfo(E_FurnitureTag::우드카운터바,						E_FurnitureStar::STAR_2,	L"우드 카운터 바",						L"주문 카운터에 적합한 바 테이블. 진열된 디저트들을 보면 주문하고 싶어진다.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Bar_01.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"우드 카운터 바.X",							L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::나무키친카운터] =					T_FurnitureInfo(E_FurnitureTag::나무키친카운터,					E_FurnitureStar::STAR_3,	L"나무 키친 카운터",						L"내구성이 뛰어난 키친 카운터. 넓은 수납공간은 자신만의 레시피를 추구하기에 딱.", 0, 350,									L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Bar_02.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"나무 키친 카운터.X"	,						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원목수납음료세트] =				T_FurnitureInfo(E_FurnitureTag::원목수납음료세트,					E_FurnitureStar::STAR_2,	L"원목수납 음료 세트",					L"다양한 취향에 맞출 수 있도록 준비된 음료 세트. 궁극의 배합비율의 음료를 목표로 자신만의 레시피를 만들어 보자.", 0, 120,	L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Bar_03.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"원목수납 음료 세트.X",						L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::배색페로로의자] =					T_FurnitureInfo(E_FurnitureTag::배색페로로의자,					E_FurnitureStar::STAR_2,	L"배색 페로로 의자",						L"페로로 인형이 놓인 배색 의자. 페로로와 나란히 앉아 여유로운 시간을 즐길 수 있다.", 0, 120,								L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Chair_03.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"배색 페로로 의자.X",						L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::배색싱글의자] =					T_FurnitureInfo(E_FurnitureTag::배색싱글의자,						E_FurnitureStar::STAR_1,	L"배색 싱글 의자",						L"배색 감각이 돋보이는 싱글 의자. 단단하고 푹신하여 안락하게 앉을 수 있다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Chair_01.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"배색 싱글 의자.X",							L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::배색더블의자] =					T_FurnitureInfo(E_FurnitureTag::배색더블의자,						E_FurnitureStar::STAR_1,	L"배색 더블 의자",						L"2명까지 사용 가능한 더블 의자. 오래 앉아도 계속 편안한 것이 큰 장점이다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Chair_02.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"배색 더블 의자.X",							L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::컬러이단화분] =					T_FurnitureInfo(E_FurnitureTag::컬러이단화분,						E_FurnitureStar::STAR_1,	L"컬러 이단 화분",						L"이단으로 나뉜 화분. 이것만 놓으면 나만의 정원을 가질 수 있다.", 0, 50,													L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Flowerpot.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"컬러 이단 화분.X",							L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::하얀커피테이블] =					T_FurnitureInfo(E_FurnitureTag::하얀커피테이블,					E_FurnitureStar::STAR_1,	L"하얀 커피 테이블",						L"커피를 놓을 수 있는 테이블. 한가로운 오후 티타임을 즐길 수 있다.", 0, 50,												L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Table_01.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"하얀 커피 테이블.X"	,						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::하얀디저트테이블] =				T_FurnitureInfo(E_FurnitureTag::하얀디저트테이블,					E_FurnitureStar::STAR_1,	L"하얀 디저트 테이블",					L"디저트가 있는 낮은 테이블. 누군가와 함께 즐겁게 대화하기 좋은 테이블.", 0, 50,											L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Table_02.png",						L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"하얀 디저트 테이블.X",						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::모모프렌즈메뉴벽장식] =			T_FurnitureInfo(E_FurnitureTag::모모프렌즈메뉴벽장식,				E_FurnitureStar::STAR_1,	L"모모프렌즈 메뉴 벽장식",				L"카페 메뉴 벽 장식.	모모프렌즈 메뉴판과 환한 조명은 귀여운 분위기를 연출해준다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wall_01.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"[W]모모프렌즈 메뉴 벽장식.X",				L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::모모프렌즈액자기둥] =				T_FurnitureInfo(E_FurnitureTag::모모프렌즈액자기둥,				E_FurnitureStar::STAR_1,	L"모모프렌즈 액자 기둥",					L"모모프렌즈 카페와 관련된 사진들이 전시된 기둥. 시즌 한정 시그니처 컷을 볼 수 있다.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wall_03.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"모모프렌즈 액자 기둥.X",					L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::화분선반] =						T_FurnitureInfo(E_FurnitureTag::화분선반,						E_FurnitureStar::STAR_1,	L"화분선반",								L"화분이 올려진 선반.	 벽에 시각적인 액센트를 가져온다.", 0, 50,															L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wallshelf_01.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"화분선반.X",								L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::페로로세트선반] =					T_FurnitureInfo(E_FurnitureTag::페로로세트선반,					E_FurnitureStar::STAR_1,	L"페로로 세트 선반",						L"페로로 인형이 놓인 선반. 페로로 인형과 작은 화분이 아기자기함을 더해준다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wallshelf_02.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"페로로 세트 선반.X"	,						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::유리창] =							T_FurnitureInfo(E_FurnitureTag::유리창,							E_FurnitureStar::STAR_1,	L"유리창",								L"개방적인 분위기를 주는 유리창. 커피를 마시며 카페 밖의 경치를 한가로이 즐겨보자!", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wall_04.png",							L"../../Reference/Resource/Obstacle/x[Cafe]x_Momo/",						L"[W]유리창.X",								L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::파란색폴리싱바닥] =				T_FurnitureInfo(E_FurnitureTag::파란색폴리싱바닥,					E_FurnitureStar::STAR_2,	L"파란색 폴리싱 바닥",					L"폴리싱 바닥. 배색 패턴이 지루함 없이 고급스러운 느낌을 준다.", 0, 120,													L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Tile.png",							L"../../Reference/Resource/Map/x[Cafe]x_Momo/",								L"파란색 폴리싱 바닥.X",						L"내장",				L"바닥");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::파란타일벽] =						T_FurnitureInfo(E_FurnitureTag::파란타일벽,						E_FurnitureStar::STAR_3,	L"파란 타일 벽",							L"패턴 모양의 타일 벽. 심플하면서도 고급스러운 분위기를 선사한다.", 0, 350,												L"../../Reference/Resource/Texture/UI/Furniture/Momo/My_Cafe_01_B_Wall_Right.png",						L"../../Reference/Resource/Map/x[Cafe]x_Momo/",								L"파란 타일 벽_Right.X",						L"내장",				L"벽지");

	//Gamecenter
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::댄싱스타의게임기] =				T_FurnitureInfo(E_FurnitureTag::댄싱스타의게임기,					E_FurnitureStar::STAR_3,	L"댄싱스타의 게임기",						L"댄스 리듬 게임을 즐길 수 있는 게임기. 오늘 밤의 주인공이 지금 정해지려고 하고 있다.", 0, 350,							L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Dancingstar.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"댄싱스타의 게임기.X",						L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::레이스게임기] =					T_FurnitureInfo(E_FurnitureTag::레이스게임기,						E_FurnitureStar::STAR_3,	L"레이스 게임기",							L"홀로그램으로 즐기는 하이 스피드 레이스 게임기. 실감나는 레이스를 즐길 수 있다.", 0, 350,								L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Speedrace.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"레이스게임기.X"	,							L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::에어하키게임기] =					T_FurnitureInfo(E_FurnitureTag::에어하키게임기,					E_FurnitureStar::STAR_3,	L"에어하키 게임기",						L"에어하키를 즐길 수 있는 게임기. 생각보다 운동 효과는 있다.", 0, 350,													L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Hockey.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"에어하키 게임기.X",							L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::카세트테이프테이블] =				T_FurnitureInfo(E_FurnitureTag::카세트테이프테이블,				E_FurnitureStar::STAR_2,	L"카세트테이프 테이블",					L"카세트테이프 모양의 테이블. 그때의 낭만을 완벽하게 재현할 수 있다.", 0, 120,											L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Table_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"카세트테이프 테이블.X",						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::배색멀티티비] =					T_FurnitureInfo(E_FurnitureTag::배색멀티티비,						E_FurnitureStar::STAR_2,	L"배색 멀티 티비",						L"레트로풍의 멀티 티비. 티비 시청과 게임 플레이 둘 다 즐길 수 있다!", 0, 120,											L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Tv.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"배색 멀티 티비.X",							L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::컬러풀자판기] =					T_FurnitureInfo(E_FurnitureTag::컬러풀자판기,						E_FurnitureStar::STAR_2,	L"컬러풀 자판기",							L"다양한 간식을 판매하는 자판기. 어느새 동전을 넣고 있는 자신을 볼 수 있다.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Vendingmachine_01.png",	L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"컬러풀 자판기.X",							L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::젤리즈크레인게임] =				T_FurnitureInfo(E_FurnitureTag::젤리즈크레인게임,					E_FurnitureStar::STAR_2,	L"젤리즈 크레인 게임",					L"제리즈 인형을 딸 수 있는 크레인 게임. 난이도가 너무 높으면 악명이 높다.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Dollcrane.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"젤리즈 크레인 게임.X",						L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::레드아케이드게임기] =				T_FurnitureInfo(E_FurnitureTag::레드아케이드게임기,				E_FurnitureStar::STAR_1,	L"레드 아케이드 게임기",					L"복고풍 아케이드 게임기. FIGHT 글자를 보는 순간 투지가 불타오른다.", 0, 50,												L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Gamemachine_01.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"레드 아케이드 게임기.X",					L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::파랑색아케이드게임기] =			T_FurnitureInfo(E_FurnitureTag::파랑색아케이드게임기,				E_FurnitureStar::STAR_1,	L"파랑색 아케이드 게임기",				L"고전 아케이드 게임기. 분노의 힘으로! 앵그리 아델리와 함께 코인을 향해 달리자!", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Gamemachine_02.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"파랑색 아케이드 게임기.X",					L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::보라색아케이드게임기] =			T_FurnitureInfo(E_FurnitureTag::보라색아케이드게임기,				E_FurnitureStar::STAR_1,	L"보라색 아케이드 게임기",				L"복고풍 아케이드 게임기. 쏟아지는 소행성을 피하면서 고양이를 살려냥!냥냥냥!", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Gamemachine_03.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"보라색 아케이드 게임기.X",					L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::젤리즈네온전광판] =				T_FurnitureInfo(E_FurnitureTag::젤리즈네온전광판,					E_FurnitureStar::STAR_1,	L"젤리즈 네온 전광판",					L"젤리즈 전광판. 젤리즈의 귀여움을 마음껏 감상할 수 있다.", 0, 50,														L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Wall_Acc_02.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"젤리즈 네온 전광판.X",						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::컬러풀패턴카페트] =				T_FurnitureInfo(E_FurnitureTag::컬러풀패턴카페트,					E_FurnitureStar::STAR_1,	L"컬러풀 패턴 카페트",					L"컬러풀한 패턴 카페트. 장인의 기술로 제직된 러그에서는 아름다운 독창성이 느껴진다.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Carpet_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"[C]컬러풀 패턴 카페트.X",					L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::핑크색심플의자] =					T_FurnitureInfo(E_FurnitureTag::핑크색심플의자,					E_FurnitureStar::STAR_1,	L"팽크색 심플 의자",						L"심플한 핑크색 스툴. 한정된 공간에 많이 배치할 수 있어 공간 절약에 도움이 된다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Chair_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"핑크색 심플 스툴.X"	,						L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::관엽식물] =						T_FurnitureInfo(E_FurnitureTag::관엽식물,						E_FurnitureStar::STAR_1,	L"관엽식물",								L"훌륭하게 자라고 있는 관엽식물. 눈에도 부드럽고 공기 청정의 효과도 느낄 수 있을 것이다.", 0, 50,							L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Pot_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"관엽식물.X",								L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::흰색심플소파] =					T_FurnitureInfo(E_FurnitureTag::흰색심플소파,						E_FurnitureStar::STAR_1,	L"흰색 심플 소파",						L"쿠션이 놓인 심플한 소파. 고탄성의 충전재가 학생들의 몸을 편안하게 해준다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Sofa_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"흰색심플소파.X"	,							L"가구",				L"침대");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::아케이드네온전광판] =				T_FurnitureInfo(E_FurnitureTag::아케이드네온전광판,				E_FurnitureStar::STAR_1,	L"아케이드 네온 전광판",					L"아케이드의 네온 전광판. 화려한 네온으로 벽을 멋지게 장식할 수 있다.", 0, 50,											L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Wall_Acc_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"[W]아케이드 네온 전광판.X",					L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::작은창문] =						T_FurnitureInfo(E_FurnitureTag::작은창문,						E_FurnitureStar::STAR_1,	L"작은 창문",							L"환기를 위한 작은 창문. 작은 크기여도 엄청난 효과를 보여준다.", 0, 50,													L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Window_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_GameCenter/",					L"[W]작은 창문.X",							L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::모눈타일바닥] =					T_FurnitureInfo(E_FurnitureTag::모눈타일바닥,						E_FurnitureStar::STAR_2,	L"모눈 타일 바닥",						L"배색 모눈 타일 바닥. 공간 전체에 세련된 느낌을 더해준다.", 0, 120,														L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Tile_2X2.png",			L"../../Reference/Resource/Map/x[Cafe]x_GameCenter/",						L"모눈 타일 바닥.X",							L"내장",				L"바닥");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::패턴타일벽] =						T_FurnitureInfo(E_FurnitureTag::패턴타일벽,						E_FurnitureStar::STAR_3,	L"패턴 타일 벽",							L"네온 벽걸이 조명이 포인트인 타일 벽. 공간 전체를 감싸는 세련된 감각을 맛볼 수 있다.", 0, 350,							L"../../Reference/Resource/Texture/UI/Furniture/GameCenter/My_Gamecenter_01_P_Wall_Right.png",			L"../../Reference/Resource/Map/x[Cafe]x_GameCenter/",						L"패턴 타일 벽_Right.X",						L"내장",				L"벽지");

	//Valentine
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인티테이블] =				T_FurnitureInfo(E_FurnitureTag::발렌타인티테이블,					E_FurnitureStar::STAR_3,	L"발렌타인 티 테이블",					L"호화로운 발렌타인 티 테이블. 친구들과 고급스러운 애프터눈 티 세트를 즐겨보자!", 0, 350,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Table_02.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 티 테이블.X",						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인하트선물상자] =			T_FurnitureInfo(E_FurnitureTag::발렌타인하트선물상자,				E_FurnitureStar::STAR_3,	L"발렌타인 하트 선물 상자",				L"발렌타인데이 한정으로 장식된 하트 선물상자. 로맨틱한 분위기를 연출할 수 있다.", 0, 350,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Acc_03.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 하트 선물 상자.X",					L"장식",				L"소품");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::초콜릿스위츠모형] =				T_FurnitureInfo(E_FurnitureTag::초콜릿스위츠모형,					E_FurnitureStar::STAR_1,	L"초콜렛 스위츠 모형",					L"핑크 포인트의 대형 스위츠 모형. 달콤하고 로맨틱한 포토존을 쉽게 꾸밀 수 있다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Acc_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"초콜릿 스위츠 모형.X",						L"장식",				L"소품");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::대형초콜릿파운틴] =				T_FurnitureInfo(E_FurnitureTag::대형초콜릿파운틴,					E_FurnitureStar::STAR_1,	L"대형 초콜릿 파운틴",					L"초콜릿이 분수처럼 흐르는 발렌타인 기계. 친구들과 가볍게 초콜릿 퐁듀를 즐길 수 있다.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Acc_02.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"대형 초콜릿 파운틴.X",						L"장식",				L"소품");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인핑크스위츠모형] =			T_FurnitureInfo(E_FurnitureTag::발렌타인핑크스위츠모형,			E_FurnitureStar::STAR_2,	L"발렌타인 핑크 스위츠 모형",				L"핑크색으로 귀여운 발렌타인 용의 큰 디저트 모형. 달콤하고 로맨틱한 연출에 최적.", 0, 120,								L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Acc_04.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 핑크 스위츠 모형.X",				L"장식",				L"소품");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인스위츠오븐] =				T_FurnitureInfo(E_FurnitureTag::발렌타인스위츠오븐,				E_FurnitureStar::STAR_1,	L"발렌타인 스위츠 오븐",					L"다양한 디저트를 만들 수 있는 스위츠 오븐. 내구성이 뛰어나 오래 사용할 수 있다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Bar_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 스위츠 오븐.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인디저트조리대] =			T_FurnitureInfo(E_FurnitureTag::발렌타인디저트조리대,				E_FurnitureStar::STAR_1,	L"발렌타인 디저트 조리대",				L"간단한 구조의 발렌타인을 위한 부엌 카운터. 가로로 넓어 음식 만들기에 딱.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Bar_02.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 디저트 조리대.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인스위츠바카운터] =			T_FurnitureInfo(E_FurnitureTag::발렌타인스위츠바카운터,			E_FurnitureStar::STAR_2,	L"발렌타인 스위츠 바 카운터",				L"디저트를 주문할 수 있는 발렌타인 바 카운터. 금색으로 장식되어 있어 조금 호화로운 느낌.", 0, 120,							L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Bar_03.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 스위츠 바 카운터.X",				L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인스위트수납바] =			T_FurnitureInfo(E_FurnitureTag::발렌타인스위트수납바,				E_FurnitureStar::STAR_1,	L"발렌타인 스위트 수납 바",				L"수납이 용이한 스위트 바. 공간 활용을 하기에 좋다.", 0, 50,															L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Bar_04.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 스위트 수납 바.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::갈색격자무늬카펫] =				T_FurnitureInfo(E_FurnitureTag::갈색격자무늬카펫,					E_FurnitureStar::STAR_1,	L"갈색 격자무늬 카펫",					L"갈색 격자무늬 카펫. 독창적인 디자인으로 고즈넉한 분위기를 연출할 수 있다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Carpet.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"[C]갈색 격자무늬 카펫.X",					L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인싱글의자] =				T_FurnitureInfo(E_FurnitureTag::발렌타인싱글의자,					E_FurnitureStar::STAR_1,	L"발렌타인 싱글 의자",					L"우아한 발렌타인 싱글 의자. 독창적인 디자인으로 제품 특유의 개성이 돋보인다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Chair.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 싱글 의자.X",						L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::초콜릿파티션] =					T_FurnitureInfo(E_FurnitureTag::초콜릿파티션,						E_FurnitureStar::STAR_1,	L"초콜릿 파티션",							L"발렌타인의 초콜릿 모양의 파티션. 캐주얼한 분위기에서 독립된 공간을 만드는 데 적합.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Partition.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"초콜릿 파티션.X",							L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인디저트쇼케이스] =			T_FurnitureInfo(E_FurnitureTag::발렌타인디저트쇼케이스,			E_FurnitureStar::STAR_1,	L"발렌타인 디저트 쇼케이스",				L"발렌타인 용 디저트를 진열하는 쇼케이스. 예쁜 프레임이 디저트를 더 돋보이게한다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Showcase.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 디저트 쇼케이스.X",				L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인쿠키소파] =				T_FurnitureInfo(E_FurnitureTag::발렌타인쿠키소파,					E_FurnitureStar::STAR_1,	L"발렌타인 쿠키 소파",					L"쿠키 쿠션 포인트의 발렌타인 소파. 독특한 디자인으로 달콤한 분위기를 더해준다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Sofa.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 쿠키 소파.X",						L"가구",				L"침대");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인멀티테이블] =				T_FurnitureInfo(E_FurnitureTag::발렌타인멀티테이블,				E_FurnitureStar::STAR_1,	L"발렌타인 멀티 테이블",					L"배색이 돋보이는 발렌타인 멀티 테이블. 간결한 실루엣이 깔끔하고 단정한 매력을 준다.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Table_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"발렌타인 멀티 테이블.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::포인트조명기둥] =					T_FurnitureInfo(E_FurnitureTag::포인트조명기둥,					E_FurnitureStar::STAR_1,	L"포인트 조명 기둥",						L"조명으로 꾸며진 포인트 기둥. 주황빛 조명으로 공간에 포인트를 준다.", 0, 50,												L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Wall_Acc_01.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"포인트 조명 기둥.X",						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::스위트초콜릿벽장식] =				T_FurnitureInfo(E_FurnitureTag::스위트초콜릿벽장식,				E_FurnitureStar::STAR_2,	L"스위트 초콜릿 벽 장식",					L"시그니처 로고의 대형 초콜릿 벽 장식. 딸기 초콜릿 디자인은 상큼한 분위기를 연출해준다.", 0, 120,							L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Wall_Acc_02.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"[W]스위트 초콜릿 벽 장식.X"	,				L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::초콜릿네온전광판] =				T_FurnitureInfo(E_FurnitureTag::초콜릿네온전광판,					E_FurnitureStar::STAR_1,	L"초콜릿 네온 전광판",					L"시그니처 로고의 초콜릿 네온 전광판. 흰색과 분홍색의 귀여운 색 조합이 돋보인다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Wall_Acc_03.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"[W]초콜릿 네온 전광판.X",					L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::수제초콜릿모형] =					T_FurnitureInfo(E_FurnitureTag::수제초콜릿모형,					E_FurnitureStar::STAR_1,	L"수제 초콜릿 모형",						L"발렌타인의 수제 초콜릿 모형. 발렌타인데이 날에 이벤트 스페이스의 공간을 연출할 때에는 필수 아이템.", 0, 50,				L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Wall_Acc_04.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"[W]수제 초콜릿 모형.X",						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인초콜릿창문] =				T_FurnitureInfo(E_FurnitureTag::발렌타인초콜릿창문,				E_FurnitureStar::STAR_1,	L"발렌타인 초콜릿 창문",					L"초콜릿이 흘러내리는 창문. 독특한 디자인으로 풍성한 공간을 연출하기에 좋다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Window.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Valentine/",					L"[W]발렌타인 초콜릿 창문.X",					L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인분홍색바닥] =				T_FurnitureInfo(E_FurnitureTag::발렌타인분홍색바닥,				E_FurnitureStar::STAR_2,	L"발렌타인 분홍색 바닥",					L"분홍 배색의 폴리싱 바닥. 로맨틱한 분위기를 연출할 수 있다.", 0, 120,													L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Tile_2x2.png",			L"../../Reference/Resource/Map/x[Cafe]x_Valentine/",						L"발렌타인 분홍색 바닥.X",					L"내장",				L"바닥");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::발렌타인초콜릿벽] =				T_FurnitureInfo(E_FurnitureTag::발렌타인초콜릿벽,					E_FurnitureStar::STAR_3,	L"발렌타인 초콜릿 벽",					L"초콜릿 디자인의 발렌타인 벽. 초콜릿의 무광 마감이 훌륭한 일품.", 0, 350,												L"../../Reference/Resource/Texture/UI/Furniture/Valentine/My_Chocolatecafe_01_P_Wall_Right.png",		L"../../Reference/Resource/Map/x[Cafe]x_Valentine/",						L"발렌타인 초콜릿 벽_Right.X"	,				L"내장",				L"벽지");

	//Poolparty
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::캐노피리조트소파] =				T_FurnitureInfo(E_FurnitureTag::캐노피리조트소파,					E_FurnitureStar::STAR_3,	L"캐노피 리조트 소파",					L"상쾌한 색상의 천이 달린 소파. 선선한 바람에 흔들리는 장식을 보며 천천히 이 시간을 맛보자.", 0, 350,						L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Sofa_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"캐노피 리조트 소파.X",						L"가구",				L"침대");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::컬러풀한대형풀] =					T_FurnitureInfo(E_FurnitureTag::컬러풀한대형풀,					E_FurnitureStar::STAR_3,	L"컬러풀한 대형 풀",						L"모모프렌즈 물놀이 세트가 들어간 대형 풀. 두 번 다시 오지 않을 이 여름을 온 힘으로, 마음껏 즐기자!", 0, 350,				L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Pool.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"컬러풀한 대형 풀.X",						L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::튜브가들어간소형풀] =				T_FurnitureInfo(E_FurnitureTag::튜브가들어간소형풀,				E_FurnitureStar::STAR_3,	L"튜브가 들어간 소형 풀",					L"모모프렌즈 튜브 등이 세트인 소형 풀. 모모프렌즈와 즐겁게 놀자!", 0, 350,												L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Minipool.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"튜브가 들어간 소형 풀.X",					L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::여름디저트세트테이블] =			T_FurnitureInfo(E_FurnitureTag::여름디저트세트테이블,				E_FurnitureStar::STAR_3,	L"여름 디저트 세트 테이블",				L"맛있는 디저트가 잔뜩 차려진 테이블. 새콤달콤한 여름의 한 때를 있는 힘껏 즐기자!", 0, 350,								L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Table_02.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"여름 디저트 세트 테이블.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::레트로한삼단앰프] =				T_FurnitureInfo(E_FurnitureTag::레트로한삼단앰프,					E_FurnitureStar::STAR_2,	L"레트로한 삼단 앰프",					L"어딘가 그리운 생김새의 앰프. 이 앰프라면 무슨 파티든 확실히 흥이 오를 것.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Musicbox.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"레트로한 삼단 앰프.X",						L"가구",				L"기타_가구");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::여름파라솔세트] =					T_FurnitureInfo(E_FurnitureTag::여름파라솔세트,					E_FurnitureStar::STAR_2,	L"여름 파라솔 세트",						L"여름을 시원하게 즐기기 위한 파라솔. 파라솔 밑에서 느긋하게 시간을 보내자.", 0, 120,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Parasol.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"여름 파라솔 세트.X",						L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::접이식리조트테이블] =				T_FurnitureInfo(E_FurnitureTag::접이식리조트테이블,				E_FurnitureStar::STAR_1,	L"접이식 리조트 테이블",					L"남국풍의 접이식 테이블. 지금이라면 상큼한 레몬 주스도 딸려옵니다. 친구들과의 담소 등에 부디.", 0, 50,						L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Table_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"접이식 리조트 테이블.X",					L"가구",				L"테이블");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::리조트풍화분] =					T_FurnitureInfo(E_FurnitureTag::리조트풍화분,						E_FurnitureStar::STAR_1,	L"리조트풍 화분",							L"열대지역의 식물이 심어진 화분. 이걸 두는 것만으로도 방이 마치 휴양지처럼 느껴진다.", 0, 50,								L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Pot_03.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"리조트풍 화분.X",							L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::하얀색몬스테라화분] =				T_FurnitureInfo(E_FurnitureTag::하얀색몬스테라화분,				E_FurnitureStar::STAR_1,	L"하얀색 몬스테라 화분",					L"몬스테라가 심어진 하얀색 화분. 남국스러운 그 분위기가 기분을 진정시켜준다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Pot_02.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"하얀색 몬스테라 화분.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::심플한직사각형화분] =				T_FurnitureInfo(E_FurnitureTag::심플한직사각형화분,				E_FurnitureStar::STAR_1,	L"심플한 직사각형 화분",					L"심플한 하얀색 화분. 식물의 색채와 합쳐져 어떤 공간도 조금 신선한 느낌으로.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Pot_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"심플한 직사각형 화분.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::푸른색우드파티션] =				T_FurnitureInfo(E_FurnitureTag::푸른색우드파티션,					E_FurnitureStar::STAR_1,	L"푸른색 우드 파티션",					L"파랗게 물들인 나무로 만든 파티션. 공간을 산뜻하게 나눌 수 있다.", 0, 50,												L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Partition.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"푸른색 우드 파티션.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::바다산책세트] =					T_FurnitureInfo(E_FurnitureTag::바다산책세트,						E_FurnitureStar::STAR_1,	L"바다 산책 세트",						L"바다에 갈 때 빠뜨릴 수 없는 용품을 모아둔 세트. 잔뜩 껴안고 바다로 향하자!", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Pack.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"바다 산책 세트.X",							L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::접이식해변의자] =					T_FurnitureInfo(E_FurnitureTag::접이식해변의자,					E_FurnitureStar::STAR_1,	L"접이식 해변 의자",						L"작게 접을 수 있는, 여름에 딱인 해변 의자. 콤팩트한 사이즈이므로 어지간한 공간에도 둘 수 있다.", 0, 50,					L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Chair.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"접이식 해변 의자.X",						L"가구",				L"의자");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::리조트풍창문] =					T_FurnitureInfo(E_FurnitureTag::리조트풍창문,						E_FurnitureStar::STAR_2,	L"리조트풍 창문",							L"안쪽으로 휴양지의 풍경이 보이는 유리창. 우선 기분만이라도 상쾌한 여름 풍경을 맛보자!", 0, 120,							L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Window_02.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[W]리조트풍 창문.X"	,						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::여름네온전광판] =					T_FurnitureInfo(E_FurnitureTag::여름네온전광판,					E_FurnitureStar::STAR_2,	L"여름 네온 전광판",						L"여름을 멋지게 연출하는 전광판. 최고의 파티가 시작된다!", 0, 120,														L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Wall_Acc_03.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[W]여름 네온 전광판.X",						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::트로피컬벽걸이아트] =				T_FurnitureInfo(E_FurnitureTag::트로피컬벽걸이아트,				E_FurnitureStar::STAR_2,	L"트로피컬 벽걸이 아트",					L"남국 기분이 나는 그림이 그려진 벽 장식. 밝은 조명과 어우러져 무심코 손그늘을 만들 것만 같다.", 0, 120,					L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Wall_Acc_02.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[C]트로피컬 벽걸이 아트.X",					L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::격자무늬창문] =					T_FurnitureInfo(E_FurnitureTag::격자무늬창문,						E_FurnitureStar::STAR_1,	L"격자무늬 창문",							L"세로로 긴 격자무늬 창문. 산뜻한 배색이 공간에 싫지 않은 고급스러움을 내준다.", 0, 50,									L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Window_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[W]격자무늬 창문.X",						L"장식",				L"벽장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::트로피컬아트프레임] =				T_FurnitureInfo(E_FurnitureTag::트로피컬아트프레임,				E_FurnitureStar::STAR_1,	L"트로피컬 아트 프레임",					L"남국 기분이 나는 그림이 그려진 프레임. 이걸 바라보는 것만으로도 기분은 남국의 휴양지.", 0, 50,							L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Wall_Acc_01.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[C]트로피컬 아트 프레임.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::원형스트라이프카펫] =				T_FurnitureInfo(E_FurnitureTag::원형스트라이프카펫,				E_FurnitureStar::STAR_1,	L"원형 스트라이프 카펫",					L"하양과 파랑 줄무늬가 입혀진 둥근 카펫. 공간을 기분 좋고 시원하게 꾸며준다.", 0, 50,										L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Carpet_02.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[C]원형 스트라이프 카펫.X",					L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::스트라이프카펫] =					T_FurnitureInfo(E_FurnitureTag::스트라이프카펫,					E_FurnitureStar::STAR_1,	L"스트라이프 카펫",						L"하양과 파랑 줄무늬 카펫. 어쩐지 시원한 기분을 느끼게 한다.", 0, 50,													L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Carpet_01.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_PoolParty/",					L"[C]스트라이프 카펫.X",						L"장식",				L"바닥장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::블루포인트포세린바닥] =			T_FurnitureInfo(E_FurnitureTag::블루포인트포세린바닥,				E_FurnitureStar::STAR_2,	L"블루 포인트 포세린 바닥",				L"다이아 문양 포인트의 포세린 바닥. 무광의 고급스러움에 블루 포인트가 추가되었다.", 0, 120,								L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Tile.png",					L"../../Reference/Resource/Map/x[Cafe]x_PoolParty/",						L"블루 포인트 포세린 바닥.X",					L"내장",				L"바닥");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::여름파도무늬벽] =					T_FurnitureInfo(E_FurnitureTag::여름파도무늬벽,					E_FurnitureStar::STAR_3,	L"여름 파도 무늬 벽",						L"여름에 딱 맞는 파도 무늬 벽. 실내인데도 마치 해변에 있는 듯한 감각을 느끼게 해준다.", 0, 350,							L"../../Reference/Resource/Texture/UI/Furniture/PoolParty/My_Poolparty_01_B_Wall_Right.png",			L"../../Reference/Resource/Map/x[Cafe]x_PoolParty/",						L"여름 파도 무늬 벽_Right.X",					L"내장",				L"벽지");

	//Trophy
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::비나트로피_동] =					T_FurnitureInfo(E_FurnitureTag::비나트로피_동,					E_FurnitureStar::STAR_1,	L"비나 트로피 동",						L"비나 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Binah_City_Bronze.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"비나 트로피_동.X",							L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::비나트로피_실버] =					T_FurnitureInfo(E_FurnitureTag::비나트로피_실버,					E_FurnitureStar::STAR_1,	L"비나 트로피 실버",						L"비나 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Binah_City_Silver.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"비나 트로피_실버.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::비나트로피_골드] =					T_FurnitureInfo(E_FurnitureTag::비나트로피_골드,					E_FurnitureStar::STAR_2,	L"비나 트로피 골드",						L"비나 총력전의 최종 보상.", 0, 120,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Binah_City_Gold.png",					L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"비나 트로피_골드.X"	,						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::비나트로피_플래티넘] =				T_FurnitureInfo(E_FurnitureTag::비나트로피_플래티넘,				E_FurnitureStar::STAR_3,	L"비나 트로피 플래티넘",					L"비나 총력전의 최종 보상.", 0, 350,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Binah_City_Platinum.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"비나 트로피_플래티넘.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::헤세드트로피_동] =					T_FurnitureInfo(E_FurnitureTag::헤세드트로피_동,					E_FurnitureStar::STAR_1,	L"헤세드 트로피 동",						L"헤세드 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Chesed_Indoor_Bronze.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"헤세드 트로피_동.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::헤세드트로피_실버] =				T_FurnitureInfo(E_FurnitureTag::헤세드트로피_실버,				E_FurnitureStar::STAR_1,	L"헤세드 트로피 실버",					L"헤세드 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Chesed_Indoor_Silver.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"헤세드 트로피_실버.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::헤세드트로피_골드] =				T_FurnitureInfo(E_FurnitureTag::헤세드트로피_골드,				E_FurnitureStar::STAR_2,	L"헤세드 트로피 골드",					L"헤세드 총력전의 최종 보상.", 0, 120,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Chesed_Indoor_Gold.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"헤세드 트로피_골드.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::헤세드트로피_플래티넘] =			T_FurnitureInfo(E_FurnitureTag::헤세드트로피_플래티넘,			E_FurnitureStar::STAR_3,	L"헤세드 트로피 플래티넘",				L"헤세드 총력전의 최종 보상.", 0, 350,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Chesed_Indoor_Platinum.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"헤세드 트로피_플래티넘.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::히에로니무스트로피_동] =			T_FurnitureInfo(E_FurnitureTag::히에로니무스트로피_동,			E_FurnitureStar::STAR_1,	L"히에로니무스 트로피 동",				L"히에로니무스 총력전의 최종 보상.", 0, 50,																			L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Hieronymus_Indoor_Bronze.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"히에로니무스 트로피_동.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::히에로니무스트로피_실버] =			T_FurnitureInfo(E_FurnitureTag::히에로니무스트로피_실버,			E_FurnitureStar::STAR_1,	L"히에로니무스 트로피 실버",				L"히에로니무스 총력전의 최종 보상.", 0, 50,																			L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Hieronymus_Indoor_Silver.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"히에로니무스 트로피_실버.X",				L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::히에로니무스트로피_골드] =			T_FurnitureInfo(E_FurnitureTag::히에로니무스트로피_골드,			E_FurnitureStar::STAR_2,	L"히에로니무스 트로피 골드",				L"히에로니무스 총력전의 최종 보상.", 0, 120,																			L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Hieronymus_Indoor_Gold.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"히에로니무스 트로피_골드.X",				L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::히에로니무스트로피_플래티넘] =		T_FurnitureInfo(E_FurnitureTag::히에로니무스트로피_플래티넘,		E_FurnitureStar::STAR_3,	L"히에로니무스 트로피 플래티넘",			L"히에로니무스 총력전의 최종 보상.", 0, 350,																			L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Hieronymus_Indoor_Platinum.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"히에로니무스 트로피_플래티넘.X",				L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::카이텐트로피_동] =					T_FurnitureInfo(E_FurnitureTag::카이텐트로피_동,					E_FurnitureStar::STAR_1,	L"카이텐 트로피 동",						L"카이텐 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Kaiten_Outdoor_Bronze.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"카이텐 트로피_동.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::카이텐트로피_실버] =				T_FurnitureInfo(E_FurnitureTag::카이텐트로피_실버,				E_FurnitureStar::STAR_1,	L"카이텐 트로피 실버",					L"카이텐 총력전의 최종 보상.", 0, 50,																					L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Kaiten_Outdoor_Silver.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"카이텐 트로피_실버.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::카이텐트로피_골드] =				T_FurnitureInfo(E_FurnitureTag::카이텐트로피_골드,				E_FurnitureStar::STAR_2,	L"카이텐 트로피 골드",					L"카이텐 총력전의 최종 보상.", 0, 120,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Kaiten_Outdoor_Gold.png",				L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"카이텐 트로피_골드.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::카이텐트로피_플래티넘] =			T_FurnitureInfo(E_FurnitureTag::카이텐트로피_플래티넘,			E_FurnitureStar::STAR_3,	L"카이텐 트로피 플래티넘",				L"카이텐 총력전의 최종 보상.", 0, 350,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Kaiten_Outdoor_Platinum.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"카이텐 트로피_플래티넘.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::시로쿠로트로피_동] =				T_FurnitureInfo(E_FurnitureTag::시로쿠로트로피_동,				E_FurnitureStar::STAR_1,	L"시로쿠로 트로피 동",					L"시로쿠로 총력전의 최종 보상.", 0, 50,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Shirokuro_Indoor_Bronze.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"시로쿠로 트로피_동.X",						L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::시로쿠로트로피_실버] =				T_FurnitureInfo(E_FurnitureTag::시로쿠로트로피_실버,				E_FurnitureStar::STAR_1,	L"시로쿠로 트로피 실버",					L"시로쿠로 총력전의 최종 보상.", 0, 50,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Shirokuro_Indoor_Silver.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"시로쿠로 트로피_실버.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::시로쿠로트로피_골드] =				T_FurnitureInfo(E_FurnitureTag::시로쿠로트로피_골드,				E_FurnitureStar::STAR_2,	L"시로쿠로 트로피 골드",					L"시로쿠로 총력전의 최종 보상.", 0, 120,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Shirokuro_Indoor_Gold.png",			L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"시로쿠로 트로피_골드.X",					L"장식",				L"기타_장식");
	(*pVecFurnitureInfo)[(_int)E_FurnitureTag::시로쿠로트로피_플래티넘] =			T_FurnitureInfo(E_FurnitureTag::시로쿠로트로피_플래티넘,			E_FurnitureStar::STAR_3,	L"시로쿠로 트로피 플래티넘",				L"시로쿠로 총력전의 최종 보상.", 0, 350,																				L"../../Reference/Resource/Texture/UI/Furniture/Trophy/My_Trophy_Shirokuro_Indoor_Platinum.png",		L"../../Reference/Resource/Obstacle/x[Cafe]x_Trophy/",						L"시로쿠로 트로피_플래티넘.X",				L"장식",				L"기타_장식");
}
