#include "XL_ParsingMgr.h"

#include "CharacterMgr.h"
#include "Dialog_Manager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CXL_ParsingMgr)

Engine::CXL_ParsingMgr::CXL_ParsingMgr()
{
}

Engine::CXL_ParsingMgr::~CXL_ParsingMgr()
{
	Free();
}

void Engine::CXL_ParsingMgr::Free()
{
}

// 캐릭터, 다이얼로그 데이터 파싱, 매개변수는 파일 경로
HRESULT CXL_ParsingMgr::Ready_Manager(const wstring& wstrFileName, _int* iLoadingGage)
{
	if (FAILED(CharacterData_Parsing(wstrFileName, iLoadingGage)))
		return E_FAIL;

	if (FAILED(DialogData_Parsing(L"../../Reference/Resource/Xlsx/쥬신 블루아카이브 모작.xlsx", L"1장 1막", 12, iLoadingGage)))
		return E_FAIL;
	if (FAILED(DialogData_Parsing(L"../../Reference/Resource/Xlsx/쥬신 블루아카이브 모작.xlsx", L"1장 2막", 13, iLoadingGage)))
		return E_FAIL;

	return S_OK;
}

// 캐릭터 데이터 파싱(파일 경로)
HRESULT CXL_ParsingMgr::CharacterData_Parsing(const wstring& wstrFileName, _int* iLoadingGage)
{
	m_iStartRow = 2; 

	while ((_uint)E_CHAR_NAME::Name_End > m_iStartRow)
	{
		libxl::Book* book = xlCreateXMLBook();

		if (book)
		{
			libxl::Format*	pFormat = book->addFormat();
			pFormat->setNumFormat(libxl::NUMFORMAT_NUMBER_D2);

			if (iLoadingGage)
			{
				*iLoadingGage += 1;
			}
			if (book->load(wstrFileName.c_str()))
			{
				for (_uint row = m_iStartRow; row < m_iStartRow + m_iParsing_AtOnce; ++row)
				{
					T_CharInfo	tCharInfo;

					libxl::Sheet* sheet = book->getSheet(7);

					// 캐릭터 핵심 정보(이름, 포지션 등), 부가 정보(학교, 나이 등)
					if (sheet)
					{
						tCharInfo.eName[(_uint)E_CHAR_LANG::LANG_ENG] = (E_CHAR_NAME)Translation_StringToEnum(sheet->readStr(row, 1)); 
						tCharInfo.eStar = (E_CHAR_STAR)(_uint)sheet->readNum(row, 2);
						tCharInfo.eCombatType = (E_CHAR_COMBATTYPE)Translation_StringToEnum(sheet->readStr(row, 3));
						tCharInfo.eRole = (E_CHAR_ROLE)(_uint)Translation_StringToEnum(sheet->readStr(row, 4));

						tCharInfo.ePosition = (E_CHAR_POSITION)Translation_StringToEnum(sheet->readStr(row, 5));
						tCharInfo.eAtkType = (E_CHAR_ATKTYPE)Translation_StringToEnum(sheet->readStr(row, 6));
						tCharInfo.eDffType = (E_CHAR_DFFTYPE)Translation_StringToEnum(sheet->readStr(row, 7));
						tCharInfo.eWeaponType = (E_CHAR_WEAPONTYPE)Translation_StringToEnum(sheet->readStr(row, 8));

						tCharInfo.eInCityBuff = (E_CHAR_MAPBUFF)Translation_StringToEnum(sheet->readStr(row, 9));
						tCharInfo.eOutDoorBuff = (E_CHAR_MAPBUFF)Translation_StringToEnum(sheet->readStr(row, 10));
						tCharInfo.eInDoorBuff = (E_CHAR_MAPBUFF)Translation_StringToEnum(sheet->readStr(row, 11));

						tCharInfo.eSchoolType = (E_CHAR_SCHOOL)Translation_StringToEnum(sheet->readStr(row, 12));
						tCharInfo.eGrade = (E_CHAR_GRADE)(_uint)sheet->readNum(row, 13);
						tCharInfo.wstrCircle = sheet->readStr(row, 14);
						tCharInfo.wstrGeneralInfo = sheet->readStr(row, 15);

						tCharInfo.iAge = (_uint)sheet->readNum(row, 16);
						tCharInfo.wstrBirthDay = sheet->readStr(row, 17);
						tCharInfo.iHeight = (_uint)sheet->readNum(row, 18);
						tCharInfo.wstrHobby = sheet->readStr(row, 19);
						0 == sheet->readNum(row, 20) ? tCharInfo.bHide = FALSE : tCharInfo.bHide = TRUE; 
					}

					sheet = book->getSheet(8);

					// 캐릭터 스텟 정보 (일반 스텟, 스킬 정보 등)
					if (sheet)
					{
						tCharInfo.fExSkill = (_float)sheet->readNum(row, 1, &pFormat);
						tCharInfo.fNormalSkill = (_float)sheet->readNum(row, 2, &pFormat);
						tCharInfo.fReinforceSkill = (_float)sheet->readNum(row, 3);

						tCharInfo.fSubSkill = (_float)sheet->readNum(row, 4, &pFormat);
						tCharInfo.iExSkillCost = (_uint)sheet->readNum(row, 5);

						tCharInfo.fExSkill_Increase = (_float)sheet->readNum(row, 6, &pFormat);
						tCharInfo.fNormalSkill_Increase = (_float)sheet->readNum(row, 7, &pFormat);
						tCharInfo.fReinforceSkill_Increase = (_float)sheet->readNum(row, 8, &pFormat);
						tCharInfo.fSubSkill_Increase = (_float)sheet->readNum(row, 9, &pFormat);

						tCharInfo.tUnitInfo.iMaxHp = (_uint)sheet->readNum(row, 10);
						tCharInfo.tUnitInfo.iNowHp = tCharInfo.tUnitInfo.iMaxHp;
						tCharInfo.tUnitInfo.iAttack = (_uint)sheet->readNum(row, 11);
						tCharInfo.tUnitInfo.iDeffence = (_uint)sheet->readNum(row, 12);
						tCharInfo.tUnitInfo.iHeal = (_uint)sheet->readNum(row, 13);

						tCharInfo.tUnitInfo.iHitRatio = (_uint)sheet->readNum(row, 14);
						tCharInfo.tUnitInfo.iEvasion = (_uint)sheet->readNum(row, 15);
						tCharInfo.tUnitInfo.iCritical = (_uint)sheet->readNum(row, 16);
						tCharInfo.tUnitInfo.iCriticalDamage = (_uint)sheet->readNum(row, 17);

						tCharInfo.tUnitInfo.iStability = (_uint)sheet->readNum(row, 18);
						tCharInfo.tUnitInfo.iRange = (_uint)sheet->readNum(row, 19);
						tCharInfo.tUnitInfo.iCrowdReinforce = (_uint)sheet->readNum(row, 20);
						tCharInfo.tUnitInfo.iCrowdResist = (_uint)sheet->readNum(row, 21);

						tCharInfo.iCostHeal = (_uint)sheet->readNum(row, 22);
						tCharInfo.iIncLevel_HP = (_uint)sheet->readNum(row, 23);
						tCharInfo.iIncLevel_ATK = (_uint)sheet->readNum(row, 24);
						tCharInfo.iIncLevel_DEF = (_uint)sheet->readNum(row, 25);

						tCharInfo.iIncLevel_HEAL = (_uint)sheet->readNum(row, 26);
					}

					sheet = book->getSheet(11);

					// 캐릭터 스킬별 이름 
					if (sheet)
					{
						tCharInfo.wstrExSkillName = sheet->readStr(row, 1);
						tCharInfo.wstrNormalSkillName = sheet->readStr(row, 2);
						tCharInfo.wstrReinforceSkillName = sheet->readStr(row, 3);
						tCharInfo.wstrSubSkillName = sheet->readStr(row, 4);

						tCharInfo.wstrExSkillImageFile = sheet->readStr(row, 5);
						tCharInfo.wstrNormalSkillImageFile = sheet->readStr(row, 6);
						tCharInfo.wstrReinforceSkillImageFile = sheet->readStr(row, 7);
						tCharInfo.wstrSubSkillImageFile = sheet->readStr(row, 8); 

						tCharInfo.wstrExSkillDesc = sheet->readStr(row, 9); 
						tCharInfo.wstrNormalSkillDesc = sheet->readStr(row, 10); 
						tCharInfo.wstrReinforceSkillDesc = sheet->readStr(row, 11); 
						tCharInfo.wstrSubSkillDesc = sheet->readStr(row, 12); 
					}

					CCharacterMgr::GetInstance()->Emplace_CharInfo(tCharInfo); 

					// 엑셀 인덱스의 -1까지 읽어야 함 (Row값 2부터 56까지) 
					if ((_uint)E_CHAR_NAME::Name_End <= row - 1)
					{
						break; 
					}
				}
			}

			book->release();
		}

		m_iStartRow += m_iParsing_AtOnce;

		m_iStartRow = CMyMath::Clamp_int(m_iStartRow, 2, (_int)E_CHAR_NAME::Name_End);
	}

	CCharacterMgr::GetInstance()->Get_vecCharSort(E_CHAR_SORT::SORT_NAME);

	return S_OK;
}

HRESULT CXL_ParsingMgr::DialogCharInfo_Parsing(const wstring & wstrFileName, const wstring & wstrEventName, const _uint & iSheet, _int* iLoadingGage)
{
	_bool	bEnd = false;
	_uint	iStartRow = m_iCharEventStartIndex; 
	_uint	iParsing_AtOnce = 30; 

	while (!bEnd)
	{
		libxl::Book* book = xlCreateXMLBook(); 

		if (book)
		{
			libxl::Format*	pFormat = book->addFormat();
			pFormat->setNumFormat(libxl::NUMFORMAT_NUMBER_D2); 

			if (iLoadingGage)
			{
				*iLoadingGage += 1;
			}
			if (book->load(wstrFileName.c_str()))
			{
				libxl::Sheet* sheet = book->getSheet(iSheet); 

				for (_uint row = iStartRow; row < iStartRow + iParsing_AtOnce; ++row)
				{
					T_DialogCharInfo*	tDialogCharInfo = new T_DialogCharInfo; 

					if (sheet)
					{
						if (-3 == sheet->readNum(row, 0)) 
						{
							bEnd = true; 
							Safe_Delete(tDialogCharInfo);

							break; 
						} 

						tDialogCharInfo->iIndex = (_int)sheet->readNum(row, 0); 
						tDialogCharInfo->wstrCharName = sheet->readStr(row, 1); 
						tDialogCharInfo->wstrCharEvent = sheet->readStr(row, 2); 
						tDialogCharInfo->wstrCharPosChange = sheet->readStr(row, 3); 
						tDialogCharInfo->wstrUniqueness = sheet->readStr(row, 4); 

						CDialog_Manager::GetInstance()->Emplace_MapDialogCharInfo(wstrEventName, tDialogCharInfo); 
					}
				}
			}

			book->release(); 
		}

		iStartRow += iParsing_AtOnce; 
	} 

	return S_OK;
}

// 다이얼로그 파싱(파일 경로)
HRESULT CXL_ParsingMgr::DialogData_Parsing(const wstring & wstrFileName, const wstring& wstrEventName, const _uint& iSheet, _int* iLoadingGage)
{
	_bool	bEnd = false;
	_bool	bBranchParsing = false; 

	m_iStartRow_Dialog = 1; 

	while (!bEnd)
	{
		libxl::Book* book = xlCreateXMLBook();

		if (book)
		{
			libxl::Format*	pFormat = book->addFormat();
			pFormat->setNumFormat(libxl::NUMFORMAT_NUMBER_D2);

			if (iLoadingGage)
			{
				*iLoadingGage += 1;
			}

			if (book->load(wstrFileName.c_str()))
			{
				libxl::Sheet* sheet = book->getSheet(iSheet);

				for (_uint row = m_iStartRow_Dialog; row < m_iStartRow_Dialog + m_iAtOnce_Dialog; ++row)
				{
					T_DialogInfo*	tDialogInfo = new T_DialogInfo; 

					if (sheet)
					{
						if (!bBranchParsing)
						{
							tDialogInfo->iIndex = (_uint)sheet->readNum(row, 0);

							if (-1 == tDialogInfo->iIndex)
							{
								m_iStartRow_Dialog = row + 1 - m_iAtOnce_Dialog;
								bBranchParsing = true;
							}
							else if (-2 == tDialogInfo->iIndex)
							{
								Safe_Delete(tDialogInfo);
								bEnd = true; 

								break;
							}
							if (!bBranchParsing)
							{
								tDialogInfo->iBranch = (_uint)sheet->readNum(row, 1);
								tDialogInfo->wstrCharName = sheet->readStr(row, 2);
								tDialogInfo->wstrCharCircle = sheet->readStr(row, 3);
								tDialogInfo->wstrTextDialog = sheet->readStr(row, 4);
								tDialogInfo->wstrDialogFontPos = sheet->readStr(row, 5);
								tDialogInfo->iBranchOptionCount = (_uint)sheet->readNum(row, 6);
								tDialogInfo->iGotoBranchIndex = (_uint)sheet->readNum(row, 7);
								tDialogInfo->iSpecialEventIndex = (_uint)sheet->readNum(row, 8);
								tDialogInfo->wstrBackgroundFile = sheet->readStr(row, 9);
								tDialogInfo->wstrScreenEvent = sheet->readStr(row, 10);
								tDialogInfo->wstrBgmFile = sheet->readStr(row, 11);
								tDialogInfo->wstrSfxFile = sheet->readStr(row, 12); 

								if (row == m_iStartRow_Dialog) 
								{
									tDialogInfo->wstrEpisodeCount = sheet->readStr(0, 13);
									tDialogInfo->wstrEpisodeName = sheet->readStr(1, 13);
									tDialogInfo->wstrEpisodeNext = sheet->readStr(2, 13);
									m_iCharEventStartIndex = (_int)sheet->readNum(1, 14);
									tDialogInfo->wstrStorySummary = sheet->readStr(1, 16); 
								}
							}
						}
						else
						{
							tDialogInfo->iIndex = (_uint)sheet->readNum(row, 0);

							if (-2 == tDialogInfo->iIndex)
							{
								Safe_Delete(tDialogInfo); 
								bEnd = true; 

								_uint iCount = (_uint)sheet->readNum(1, 15); 

								for (_uint i = 2; i < 2 + iCount; ++i) 
								{
									CDialog_Manager::GetInstance()->Emplace_MapDialogAppearChar(wstrEventName, sheet->readStr(i, 15), iCount);
								}

								break; 
							}

							tDialogInfo->iBranch = (_int)sheet->readNum(row, 1); 
							tDialogInfo->wstrCharName = sheet->readStr(row, 2);

							if (L"Branch" == tDialogInfo->wstrCharName)
							{
								tDialogInfo->wstrBranchText_1 = sheet->readStr(row, 3);

								if (L"NULL" != sheet->readStr(row, 4))
								{
									tDialogInfo->wstrBranchText_2 = sheet->readStr(row, 4);
								}
							}
							else
							{
								tDialogInfo->wstrTextDialog = sheet->readStr(row, 4); 
							}

							tDialogInfo->wstrDialogFontPos = sheet->readStr(row, 5);
							tDialogInfo->iBranchOptionCount = (_uint)sheet->readNum(row, 6);
							tDialogInfo->iGotoBranchIndex = (_uint)sheet->readNum(row, 7);
							tDialogInfo->iSpecialEventIndex = (_uint)sheet->readNum(row, 8);
							tDialogInfo->wstrBgmFile = sheet->readStr(row, 11); 
						}
					}

					CDialog_Manager::GetInstance()->Emplace_MapDialogInfo(wstrEventName, tDialogInfo);
				}
			}

			book->release(); 
		}

		m_iStartRow_Dialog += m_iAtOnce_Dialog;
	}

	DialogCharInfo_Parsing(wstrFileName, wstrEventName, iSheet, iLoadingGage);
	CDialog_Manager::GetInstance()->Set_EndParsing(wstrEventName); 

	return S_OK;
}

// String 2 Enum(파싱할 때만 쓰는 함수)
_uint CXL_ParsingMgr::Translation_StringToEnum(const wstring & wstrName, E_Translation_Type eType)
{
	switch (eType)
	{
#pragma region Character
		//////////////////////////// Translation_캐릭터 관련 정보 
	case Engine::E_Translation_Type::Translation_CharInfo:
		if (L"NULL" == wstrName)
			return 0;

		// 캐릭터 이름 
		else if (L"Neru" == wstrName || L"네루" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Neru;
		else if (L"Nonomi" == wstrName || L"노노미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Nonomi;
		else if (L"Mashiro" == wstrName || L"마시로" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Mashiro;
		else if (L"Maki" == wstrName || L"마키" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Maki;
		else if (L"Mutsuki" == wstrName || L"무츠키" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Mutsuki;
		else if (L"Saya" == wstrName || L"사야" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Saya;
		else if (L"Serina" == wstrName || L"세리나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Serina;
		else if (L"Serika" == wstrName || L"세리카" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Serika;
		else if (L"Shun" == wstrName || L"슌" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Shun;
		else if (L"Sumire" == wstrName || L"스미레" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Sumire;
		else if (L"Suzumi" == wstrName || L"스즈미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Suzumi;
		else if (L"Shiroko" == wstrName || L"시로코" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Shiroko;
		else if (L"Shimiko" == wstrName || L"시미코" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Shimiko;
		else if (L"Shizuko" == wstrName || L"시즈코" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Shizuko;
		else if (L"Aru" == wstrName || L"아루" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Aru;
		else if (L"Asuna" == wstrName || L"아스나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Asuna;
		else if (L"Ayane" == wstrName || L"아야네" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Ayane;
		else if (L"Airi" == wstrName || L"아이리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Airi;
		else if (L"Izuna" == wstrName || L"이즈나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Izuna;
		else if (L"Akane" == wstrName || L"아카네" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Akane;
		else if (L"Akari" == wstrName || L"아카리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Akari;
		else if (L"Eimi" == wstrName || L"에이미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Eimi;
		else if (L"Yoshimi" == wstrName || L"요시미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Yoshimi;
		else if (L"Utaha" == wstrName || L"우타하" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Utaha;
		else if (L"Yuuka" == wstrName || L"유우카" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Yuuka;
		else if (L"Iori" == wstrName || L"이오리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Iori;
		else if (L"Izumi" == wstrName || L"이즈미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Izumi;
		else if (L"Juri" == wstrName || L"주리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Juri;
		else if (L"Zunko" == wstrName || L"준코" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Zunko;
		else if (L"Tsurugi" == wstrName || L"츠루기" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Tsurugi;
		else if (L"Tsubaki" == wstrName || L"츠바키" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Tsubaki;
		else if (L"Chinatsu" == wstrName || L"치나츠" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Chinatsu;
		else if (L"Chise" == wstrName || L"치세" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Chise;
		else if (L"Karin" == wstrName || L"카린" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Karin;
		else if (L"Kayoko" == wstrName || L"카요코" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Kayoko;
		else if (L"Kotama" == wstrName || L"코타마" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Kotama;
		else if (L"Kotori" == wstrName || L"코토리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Kotori;
		else if (L"Pina" == wstrName || L"피나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Pina;
		else if (L"Hanae" == wstrName || L"하나에" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hanae;
		else if (L"Hare" == wstrName || L"하레" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hare;
		else if (L"Haruna" == wstrName || L"하루나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Haruna;
		else if (L"Haruka" == wstrName || L"하루카" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Haruka;
		else if (L"Hasumi" == wstrName || L"하스미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hasumi;
		else if (L"Hoshino" == wstrName || L"호시노" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hoshino;
		else if (L"Fuuka" == wstrName || L"후우카" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Fuuka;
		else if (L"Hina" == wstrName || L"히나" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hina;
		else if (L"Hibiki" == wstrName || L"히비키" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hibiki;
		else if (L"Hihumi" == wstrName || L"히후미" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Hihumi;
		else if (L"Midori" == wstrName || L"미도리" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Midori;
		else if (L"Aris" == wstrName || L"아리스" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Aris;
		else if (L"Yuzu" == wstrName || L"유즈" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Yuzu;
		else if (L"Momoi" == wstrName || L"모모이" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Momoi;
		else if (L"Nodoka" == wstrName || L"노도카" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Nodoka;
		else if (L"Wakamo" == wstrName || L"와카모" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Wakamo;
		else if (L"Azusa" == wstrName || L"아즈사" == wstrName)
			return (_uint)E_CHAR_NAME::Name_Azusa;


		// 
		else if (L"Striker" == wstrName)
			return (_uint)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER;
		else if (L"Special" == wstrName)
			return (_uint)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL;
		else if (L"탱커" == wstrName)
			return (_uint)E_CHAR_ROLE::ROLE_TANKER;
		else if (L"딜러" == wstrName)
			return (_uint)E_CHAR_ROLE::ROLE_DEALER;
		else if (L"힐러" == wstrName)
			return (_uint)E_CHAR_ROLE::ROLE_HEALER;
		else if (L"서포터" == wstrName)
			return (_uint)E_CHAR_ROLE::ROLE_SUPPORTER;
		else if (L"FRONT" == wstrName)
			return (_uint)E_CHAR_POSITION::POSITION_FRONT;
		else if (L"MIDDLE" == wstrName)
			return (_uint)E_CHAR_POSITION::POSITION_MIDDLE;
		else if (L"BACK" == wstrName)
			return (_uint)E_CHAR_POSITION::POSITION_BACK;
		else if (L"폭발" == wstrName)
			return (_uint)E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION;
		else if (L"관통" == wstrName)
			return (_uint)E_CHAR_ATKTYPE::ATKTYPE_PIERCE;
		else if (L"신비" == wstrName)
			return (_uint)E_CHAR_ATKTYPE::ATKTYPE_MYSTERY;
		else if (L"중장갑" == wstrName)
			return (_uint)E_CHAR_DFFTYPE::DFFTYPE_HEAVY;
		else if (L"경장갑" == wstrName)
			return (_uint)E_CHAR_DFFTYPE::DFFTYPE_LIGHT;
		else if (L"특수장갑" == wstrName)
			return (_uint)E_CHAR_DFFTYPE::DFFTYPE_SPECICAL;
		else if (L"SG" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_SG;
		else if (L"SMG" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG;
		else if (L"HG" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_HG;
		else if (L"GL" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_GL;
		else if (L"SR" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_SR;
		else if (L"MG" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_MG;
		else if (L"AR" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_AR;
		else if (L"MT" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_MT;
		else if (L"RL" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_RL;
		else if (L"RG" == wstrName)
			return (_uint)E_CHAR_WEAPONTYPE::WEAPONTYPE_RG;
		else if (L"SS" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_SS;
		else if (L"S" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_S;
		else if (L"A" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_A;
		else if (L"B" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_B;
		else if (L"C" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_C;
		else if (L"D" == wstrName)
			return (_uint)E_CHAR_MAPBUFF::MAPBUFF_D;
		else if (L"아비도스 고등학교" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_ABYDOS;
		else if (L"트리니티 종합학원" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_TRINITY;
		else if (L"게한나 학원" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_GEHENNA;
		else if (L"밀레니엄 사이언스 스쿨" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_MILLENIUM;
		else if (L"백귀야행 연합학원" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_HYAKKIYAKO;
		else if (L"산해경" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_SHANHAIJING;
		else if (L"붉은겨울 연방학원" == wstrName)
			return (_uint)E_CHAR_SCHOOL::SCHOOL_REDWINTER;
		break;

#pragma endregion

#pragma region Dialog

		//////////////////////////// Translation_대화문 관련 정보 
	case Engine::E_Translation_Type::Translation_DialogInfo:
		// 캐릭터 이벤트 
		if (L"NULL" == wstrName)
			return 0;
		else if (L"Fade_In" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_FadeIn;
		else if (L"Fade_Out" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_FadeOut;
		else if (L"UpDown_Shake" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_Updown_Shake;
		else if (L"LeftRight_Shake" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_LeftRight_Shake;
		else if (L"Beep" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_Beep;
		else if (L"All_Char_FadeOut" == wstrName)
			return (_uint)E_Dialog_CharEvent::CharEvent_AllChar_FadeOut;

		// 캐릭터 위치 변경 
		else if (L"Center_Popup" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Center_Popup;
		else if (L"Left_Popup" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Left_Popup;
		else if (L"Right_Popup" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Right_Popup;
		else if (L"LC_Popup" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_LC_Popup;
		else if (L"RC_Popup" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_RC_Popup;
		else if (L"Center" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Center;
		else if (L"Left" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Left;
		else if (L"Right" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_Right;
		else if (L"LC" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_LC;
		else if (L"RC" == wstrName)
			return (_uint)E_Dialog_PosChange::PosChange_RC;

		// 배경 
		else if (L"Background_Nochange" == wstrName)
			return (_uint)E_Dialog_Background::Background_NoChange;
		else if (L"Background_PpachinkoAlley" == wstrName)
			return (_uint)E_Dialog_Background::Background_PpachinkoAlley;

		// 스크린 이벤트 
		else if (L"Screen_FadeIn" == wstrName)
			return (_uint)E_Dialog_ScreenEvent::ScreenEvent_FadeIn;
		else if (L"Screen_FadeOut" == wstrName)
			return (_uint)E_Dialog_ScreenEvent::ScreenEvent_FadeOut;

		// Bgm 
		else if (L"On" == wstrName)
			return (_uint)E_Dialog_Bgm::Bgm_ON;
		else if (L"Off" == wstrName)
			return (_uint)E_Dialog_Bgm::Bgm_OFF;
		else if (L"NC_Bgm" == wstrName)
			return (_uint)E_Dialog_Bgm::Bgm_NC;

		break;

#pragma endregion 

	default:
		assert(0);
		break;
	}

	return 0;
}

// Enum 2 String(매개변수 Enum값, Enum타입(이름, 전투타입 등))
wstring CXL_ParsingMgr::Translation_EnumToString(const _uint & iEnumValue, const E_CHAR_ENUMTYPE& eEnumType)
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
		case Engine::E_CHAR_NAME::Name_Nodoka:
			return L"Nodoka";
		case Engine::E_CHAR_NAME::Name_Momoi:
			return L"Momoi";
		case Engine::E_CHAR_NAME::Name_Midori:
			return L"Midori";
		case Engine::E_CHAR_NAME::Name_Aris:
			return L"Aris";
		case Engine::E_CHAR_NAME::Name_Azusa:
			return L"Azusa";
		case Engine::E_CHAR_NAME::Name_Wakamo:
			return L"Wakamo";
		case Engine::E_CHAR_NAME::Name_Yuzu:
			return L"Yuzu";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_KORNAME:
		switch ((E_CHAR_NAME)iEnumValue)
		{
		case Engine::E_CHAR_NAME::Name_Neru:
			return L"네루";
		case Engine::E_CHAR_NAME::Name_Nonomi:
			return L"노노미";
		case Engine::E_CHAR_NAME::Name_Mashiro:
			return L"마시로";
		case Engine::E_CHAR_NAME::Name_Maki:
			return L"마키";
		case Engine::E_CHAR_NAME::Name_Mutsuki:
			return L"무츠키";
		case Engine::E_CHAR_NAME::Name_Saya:
			return L"사야";
		case Engine::E_CHAR_NAME::Name_Serina:
			return L"세리나";
		case Engine::E_CHAR_NAME::Name_Serika:
			return L"세리카";
		case Engine::E_CHAR_NAME::Name_Shun:
			return L"슌";
		case Engine::E_CHAR_NAME::Name_Sumire:
			return L"스미레";
		case Engine::E_CHAR_NAME::Name_Suzumi:
			return L"스즈미";
		case Engine::E_CHAR_NAME::Name_Shiroko:
			return L"시로코";
		case Engine::E_CHAR_NAME::Name_Shimiko:
			return L"시미코";
		case Engine::E_CHAR_NAME::Name_Shizuko:
			return L"시즈코";
		case Engine::E_CHAR_NAME::Name_Aru:
			return L"아루";
		case Engine::E_CHAR_NAME::Name_Asuna:
			return L"아스나";
		case Engine::E_CHAR_NAME::Name_Ayane:
			return L"아야네";
		case Engine::E_CHAR_NAME::Name_Airi:
			return L"아이리";
		case Engine::E_CHAR_NAME::Name_Izuna:
			return L"이즈나";
		case Engine::E_CHAR_NAME::Name_Akane:
			return L"아카네";
		case Engine::E_CHAR_NAME::Name_Akari:
			return L"아카리";
		case Engine::E_CHAR_NAME::Name_Eimi:
			return L"에이미";
		case Engine::E_CHAR_NAME::Name_Yoshimi:
			return L"요시미";
		case Engine::E_CHAR_NAME::Name_Utaha:
			return L"우타하";
		case Engine::E_CHAR_NAME::Name_Yuuka:
			return L"유우카";
		case Engine::E_CHAR_NAME::Name_Iori:
			return L"이오리";
		case Engine::E_CHAR_NAME::Name_Izumi:
			return L"이즈미";
		case Engine::E_CHAR_NAME::Name_Juri:
			return L"주리";
		case Engine::E_CHAR_NAME::Name_Zunko:
			return L"준코";
		case Engine::E_CHAR_NAME::Name_Tsurugi:
			return L"츠루기";
		case Engine::E_CHAR_NAME::Name_Tsubaki:
			return L"츠바키";
		case Engine::E_CHAR_NAME::Name_Chinatsu:
			return L"치나츠";
		case Engine::E_CHAR_NAME::Name_Chise:
			return L"치세";
		case Engine::E_CHAR_NAME::Name_Karin:
			return L"카린";
		case Engine::E_CHAR_NAME::Name_Kayoko:
			return L"카요코";
		case Engine::E_CHAR_NAME::Name_Kotama:
			return L"코타마";
		case Engine::E_CHAR_NAME::Name_Kotori:
			return L"코토리";
		case Engine::E_CHAR_NAME::Name_Pina:
			return L"피나";
		case Engine::E_CHAR_NAME::Name_Hanae:
			return L"하나에";
		case Engine::E_CHAR_NAME::Name_Hare:
			return L"하레";
		case Engine::E_CHAR_NAME::Name_Haruna:
			return L"하루나";
		case Engine::E_CHAR_NAME::Name_Haruka:
			return L"하루카";
		case Engine::E_CHAR_NAME::Name_Hasumi:
			return L"하스미";
		case Engine::E_CHAR_NAME::Name_Hoshino:
			return L"호시노";
		case Engine::E_CHAR_NAME::Name_Fuuka:
			return L"후우카";
		case Engine::E_CHAR_NAME::Name_Hina:
			return L"히나";
		case Engine::E_CHAR_NAME::Name_Hibiki:
			return L"히비키";
		case Engine::E_CHAR_NAME::Name_Hihumi:
			return L"히후미";
		case Engine::E_CHAR_NAME::Name_Nodoka:
			return L"노도카";
		case Engine::E_CHAR_NAME::Name_Momoi:
			return L"모모이";
		case Engine::E_CHAR_NAME::Name_Midori:
			return L"미도리";
		case Engine::E_CHAR_NAME::Name_Aris:
			return L"아리스";
		case Engine::E_CHAR_NAME::Name_Azusa:
			return L"아즈사";
		case Engine::E_CHAR_NAME::Name_Wakamo:
			return L"와카모";
		case Engine::E_CHAR_NAME::Name_Yuzu:
			return L"유즈";
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
			return L"탱커";

		case E_CHAR_ROLE::ROLE_DEALER:
			return L"딜러";

		case E_CHAR_ROLE::ROLE_HEALER:
			return L"힐러";

		case E_CHAR_ROLE::ROLE_SUPPORTER:
			return L"서포터";
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
			return L"폭발";

		case E_CHAR_ATKTYPE::ATKTYPE_PIERCE:
			return L"관통";

		case E_CHAR_ATKTYPE::ATKTYPE_MYSTERY:
			return L"신비";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_DFFTYPE:
		switch ((E_CHAR_DFFTYPE)iEnumValue)
		{
		case E_CHAR_DFFTYPE::DFFTYPE_HEAVY:
			return L"중장갑";

		case E_CHAR_DFFTYPE::DFFTYPE_LIGHT:
			return L"경장갑";

		case E_CHAR_DFFTYPE::DFFTYPE_SPECICAL:
			return L"특수장갑";
		}
		break;

	case Engine::E_CHAR_ENUMTYPE::CNUM_WEAPONTYPE:
		switch ((E_CHAR_WEAPONTYPE)iEnumValue)
		{
		case E_CHAR_WEAPONTYPE::WEAPONTYPE_AR:
			return L"AR";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_GL:
			return L"GL";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_HG:
			return L"HG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_MG:
			return L"MG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_MT:
			return L"MT";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SG:
			return L"SG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SMG:
			return L"SMG";

		case E_CHAR_WEAPONTYPE::WEAPONTYPE_SR:
			return L"SR";

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

	case Engine::E_CHAR_ENUMTYPE::CNUM_OTHERS:
		switch ((E_CHAR_SORT)iEnumValue)
		{
		case E_CHAR_SORT::SORT_ATKTYPE:
			return L"공격타입";

		case E_CHAR_SORT::SORT_DFFTYPE:
			return L"방어타입";

		case E_CHAR_SORT::SORT_EXSKILL_LV:
			return L"EX스킬 LV";

		case E_CHAR_SORT::SORT_GETTIME:
			return L"획득시기";

		case E_CHAR_SORT::SORT_INCITY_LV:
			return L"시가전";

		case E_CHAR_SORT::SORT_INDOOR_LV:
			return L"실내전";

		case E_CHAR_SORT::SORT_LEVEL:
			return L"레벨";

		case E_CHAR_SORT::SORT_NAME:
			return L"이름";

		case E_CHAR_SORT::SORT_OUTDOOR_LV:
			return L"야전";

		case E_CHAR_SORT::SORT_RANGE:
			return L"사거리";

		case E_CHAR_SORT::SORT_ROLE:
			return L"역할";

		case E_CHAR_SORT::SORT_SCHOOL:
			return L"학교";

		case E_CHAR_SORT::SORT_SKILL_LV:
			return L"스킬LV";

		case E_CHAR_SORT::SORT_STAR:
			return L"의 수";

		case E_CHAR_SORT::SORT_TIES:
			return L"인연LV";

		case E_CHAR_SORT::SORT_WEAPONTYPE:
			return L"무기종류";
		}
		break;

	default:
		assert(0);
		break;
	}

	return L"NULL";
}

wstring CXL_ParsingMgr::Translation_CharName(const E_CHAR_NAME & eName)
{
	switch (eName)
	{
	case E_CHAR_NAME::Name_Neru:	return L"네루";
	case E_CHAR_NAME::Name_Nonomi:	return L"노노미";
	case E_CHAR_NAME::Name_Nodoka:	return L"노도카";
	case E_CHAR_NAME::Name_Mashiro: return L"마시로";
	case E_CHAR_NAME::Name_Maki:	return L"마키";
	case E_CHAR_NAME::Name_Momoi:	return L"모모이";
	case E_CHAR_NAME::Name_Mutsuki: return L"무츠키";
	case E_CHAR_NAME::Name_Midori:	return L"미도리";
	case E_CHAR_NAME::Name_Saya:	return L"사야";
	case E_CHAR_NAME::Name_Serina:	return L"세리나";
	case E_CHAR_NAME::Name_Serika:	return L"세리카";
	case E_CHAR_NAME::Name_Shun:	return L"슌";
	case E_CHAR_NAME::Name_Sumire:	return L"스미레";
	case E_CHAR_NAME::Name_Suzumi:	return L"스즈미";
	case E_CHAR_NAME::Name_Shiroko:	return L"시로코";
	case E_CHAR_NAME::Name_Shimiko:	return L"시미코";
	case E_CHAR_NAME::Name_Shizuko:	return L"시즈코";
	case E_CHAR_NAME::Name_Aru:		return L"아루";
	case E_CHAR_NAME::Name_Aris:	return L"아리스";
	case E_CHAR_NAME::Name_Asuna:	return L"아스나";
	case E_CHAR_NAME::Name_Ayane:	return L"아야네";
	case E_CHAR_NAME::Name_Airi:	return L"아이리";
	case E_CHAR_NAME::Name_Azusa:	return L"아즈나";
	case E_CHAR_NAME::Name_Akane:	return L"아카네";
	case E_CHAR_NAME::Name_Akari:	return L"아카리";
	case E_CHAR_NAME::Name_Eimi:	return L"에이미";
	case E_CHAR_NAME::Name_Wakamo:	return L"와카모";
	case E_CHAR_NAME::Name_Yoshimi:	return L"요시미";
	case E_CHAR_NAME::Name_Utaha:	return L"우타하";
	case E_CHAR_NAME::Name_Yuuka:	return L"유우카";
	case E_CHAR_NAME::Name_Yuzu:	return L"유주";
	case E_CHAR_NAME::Name_Iori:	return L"이오리";
	case E_CHAR_NAME::Name_Izuna:	return L"이즈나";
	case E_CHAR_NAME::Name_Izumi:	return L"이즈미";
	case E_CHAR_NAME::Name_Juri:	return L"주리";
	case E_CHAR_NAME::Name_Zunko:	return L"준코";
	case E_CHAR_NAME::Name_Tsurugi:	return L"츠루기";
	case E_CHAR_NAME::Name_Tsubaki:	return L"츠바키";
	case E_CHAR_NAME::Name_Chinatsu:return L"치나츠";
	case E_CHAR_NAME::Name_Chise:	return L"치세";
	case E_CHAR_NAME::Name_Karin:	return L"카린";
	case E_CHAR_NAME::Name_Kayoko:	return L"카요코";
	case E_CHAR_NAME::Name_Kotama:	return L"코타마";
	case E_CHAR_NAME::Name_Kotori:	return L"코토리";
	case E_CHAR_NAME::Name_Pina:	return L"피나";
	case E_CHAR_NAME::Name_Hanae:	return L"하나에";
	case E_CHAR_NAME::Name_Hare:	return L"하레";
	case E_CHAR_NAME::Name_Haruna:	return L"하루나";
	case E_CHAR_NAME::Name_Haruka:	return L"하루카";
	case E_CHAR_NAME::Name_Hasumi:	return L"하스미";
	case E_CHAR_NAME::Name_Hoshino:	return L"호시노";
	case E_CHAR_NAME::Name_Fuuka:	return L"후우카";
	case E_CHAR_NAME::Name_Hina:	return L"히나";
	case E_CHAR_NAME::Name_Hibiki:	return L"히비키";
	case E_CHAR_NAME::Name_Hihumi:	return L"히후미";
	default:
		assert(0); 
		break;
	}

	return L"잘못 입력함";
} 
