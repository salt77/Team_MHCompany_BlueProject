#include "GameManager.h"
#include "DataManager.h"
#include "Wall512.h"
#include "EffectObject.h"
#include "ParticleObject.h"
#include "UI_Manager.h"
//STRKER
#include "Char_Mutsuki.h"
#include "Char_Shun.h"
#include "Char_Maki.h"
#include "Char_Neru.h"
#include "Char_Aru.h"
#include "Char_Aris.h"
#include "Char_Nodoka.h"
#include "Char_Momoi.h"
#include "Char_Azusa.h"
#include "Char_Chise.h"
#include "Char_Eimi.h"
#include "Char_Hasumi.h"
#include "Char_Hina.h"
#include "Char_Iori.h"
#include "Char_Izumi.h"
#include "Char_Kirino.h"
#include "Char_Akane.h"
#include "Char_Serika.h"
#include "Char_Sumire.h"
#include "Char_Pina.h"
#include "Char_Nodoka.h"
#include "Char_Suzumi.h"
#include "Char_Utaha.h"
#include "Char_Wakamo.h"
#include "Char_Utaha.h"
#include "Char_Yoshimi.h"
#include "Char_Yuzu.h"
#include "Char_Zunko.h"
//SPECIAL  
#include "Char_Chinatsu.h"
#include "Char_Hibiki.h"
#include "Char_Kayoko.h"
#include "Char_Karin.h"
#include "Char_Mashiro.h"
#include "Char_Airi.h"
#include "Char_Juri.h"
USING(Engine)

IMPLEMENT_SINGLETON(CGameManager)

E_ItemSortType CGameManager::m_eItemSortType = E_ItemSortType::Basic;
CGameManager::CGameManager()
{ 
	ZeroMemory(m_iReaderIndex, sizeof(m_iReaderIndex));
}


CGameManager::~CGameManager()
{
	Free();
}

void CGameManager::Free()
{
	Safe_Release(m_pTileField);

	for (_int i = 0; i < (_int)E_RangeEffect::RE_End; ++i)
	{
		Safe_Release(m_pRangeEffect[i]);
	}

	LogOut();
	Release_Formation();
	Release_WorldResource();
}

void CGameManager::Ready_GameManager()
{
	if (m_bReady)
	{
		return;
	}

	m_bReady = TRUE;
	Ready_Formtion();
	Initialize_Formation();
	Login_Account();

	// 월드를 미리 세팅
	Initialize_WorldResource();

	if (nullptr == m_pRangeEffect[(_int)E_RangeEffect::Circle_OutLine])
	{
		m_pRangeEffect[(_int)E_RangeEffect::Circle_OutLine] = CEffectObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, VEC_ZERO, VEC_ZERO, L"Circle_Range.effect");
	}
}

void CGameManager::Update_GameManager(const _float & fTimeDelta)
{
	if (m_pNextWorld)
	{
		m_pNextWorld->Update_World(fTimeDelta);
	}
	
	if (m_bBattleIng)
	{
		// 코스트 증가
		CGameManager::GetInstance()->Add_Cost(fTimeDelta);
	}

	if (m_bSkillOnMouse)
	{
		m_pRangeEffect[(_int)E_RangeEffect::Circle_OutLine]->Update_Object(fTimeDelta);
		m_pRangeEffect[(_int)E_RangeEffect::Circle_OutLine]->LateUpdate_Object(fTimeDelta);
	}

	if (CKeyManager::GetInstance()->Key_Pressing(KEY_G) && CKeyManager::GetInstance()->Key_Down(KEY_H))
	{
		m_bPlayerInvn = !m_bPlayerInvn;
	}

}

void CGameManager::Login_Account()
{
	CDataManager::GetInstance()->Initialize_Item(&m_tAll_Item);
	CDataManager::GetInstance()->Initialize_Equip(&m_tAll_Equip);
	CDataManager::GetInstance()->Initialize_Furniture(&m_tAll_Furniture);

	CUI_Manager::GetInstance()->Set_LobbyInfoUI();
}

void CGameManager::LogOut()
{
	if (m_fpPlayFab_Logout)
	{
		m_fpPlayFab_Logout();
	}
}

#pragma region DataManager함수

void CGameManager::Get_CurrentDate(T_DATEINFO * pDateInfoOut)
{
	CDataManager::GetInstance()->Get_CurrentDate(pDateInfoOut);
}

_int CGameManager::Get_DateDiffrence(T_DATEINFO tPastDate, T_DATEINFO tFutureDate)
{
	return CDataManager::GetInstance()->Get_DateDiffrence(tPastDate, tFutureDate);
}

const WCHAR * CGameManager::Get_NickName()
{
	return CDataManager::GetInstance()->Get_NickName();
}

const _uint & CGameManager::Get_Level()
{
	return CDataManager::GetInstance()->Get_Level();
}

const _uint & CGameManager::Get_EXP()
{
	return CDataManager::GetInstance()->Get_EXP();
}

const _uint & CGameManager::Get_Credit()
{
	return CDataManager::GetInstance()->Get_Credit();
}

const _uint & CGameManager::Get_Eligma()
{
	return CDataManager::GetInstance()->Get_Eligma(); 
}

const _uint & CGameManager::Get_AP()
{
	return CDataManager::GetInstance()->Get_AP();
}

const _uint & CGameManager::Get_BluePyroxene()
{
	return CDataManager::GetInstance()->Get_BluePyroxene();
}

void CGameManager::Set_NickName(wstring wstrNickName)
{
	CDataManager::GetInstance()->Set_NickName(wstrNickName);
}

void CGameManager::Set_Level(_uint iLevel)
{
	CDataManager::GetInstance()->Set_Level(iLevel);
}

void CGameManager::Set_EXP(_uint iEXP)
{
	CDataManager::GetInstance()->Set_EXP(iEXP);
}

void CGameManager::Set_Credit(_uint iCredit)
{
	CDataManager::GetInstance()->Set_Credit(iCredit);
}

void CGameManager::Set_Eligma(_uint iEligma)
{
	CDataManager::GetInstance()->Set_Eligma(iEligma); 
}

void CGameManager::Set_AP(_uint iAP)
{
	CDataManager::GetInstance()->Set_AP(iAP);
}

void CGameManager::Set_BluePyroxene(_uint iBluePyroxene)
{
	CDataManager::GetInstance()->Set_BluePyroxene(iBluePyroxene);
}

void CGameManager::Add_Credit(_int iCredit)
{
	CDataManager::GetInstance()->Add_Credit(iCredit);
}

void CGameManager::Add_Eligma(_int iEligma)
{
	CDataManager::GetInstance()->Add_Eligma(iEligma); 
}

const T_USERINFO & CGameManager::Get_UserInfo()
{
	return CDataManager::GetInstance()->Get_UserInfo();
}

void CGameManager::Initialize_WorldResource()
{
	// 월드를 로드하지 않았을 때만 실행
	if (FALSE == m_mapWorldCreators.empty())
		return;

	_finddata_t findData;
	intptr_t lHandle = 0;
	intptr_t lResult = 0;
	lHandle = CUtility::FindData_ChoiceType("../../Reference/Data/Data_World/", "*.*", &findData);

	while (-1 != lResult)
	{
		wchar_t szFullPath[MAX_PATH] = L"";
		wchar_t szName[MAX_PATH] = L"";

		int nLen = MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, findData.name, (int)strlen(findData.name), szName, nLen);

		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
			CWorldCreator* pWorld = CWorldCreator::Create(szName);
			auto iter = m_mapWorldCreators.find(pWorld->Get_WorldMod());
			if (iter == m_mapWorldCreators.end())
			{
				list<CWorldCreator*> listTemp;
				m_mapWorldCreators.emplace(pWorld->Get_WorldMod(), listTemp);
				iter = m_mapWorldCreators.find(pWorld->Get_WorldMod());
			}
			iter->second.push_back(pWorld);
		}
		lResult = _findnext(lHandle, &findData);
	}

	_findclose(lHandle);
}

void CGameManager::Release_WorldResource()
{
	for (auto& mapWorld : m_mapWorldCreators)
	{
		for (auto& World : mapWorld.second)
		{
			Safe_Release(World);
		}
		mapWorld.second.clear();
	}
	m_mapWorldCreators.clear();

}

void CGameManager::Set_EndBattle()
{
	m_bBattleIng = FALSE;
	CUI_Manager::GetInstance()->Setting_Combat_Victory();
 	CUI_Manager::GetInstance()->Active_Combat_Victory();
}

void CGameManager::Set_IsTileFiledBattle(_bool bState)
{
	m_bIsTileFiledBattle = bState;

	if (m_bIsTileFiledBattle)
	{

	}
	else
	{

	}

}

vector<_vec3> CGameManager::Get_AliveStrikersPosition()
{
	vector<_vec3> vecPos;

	for (auto& iter : (*Get_CurrentSquad())[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER])
	{
		if (nullptr == iter)
			continue;

		if (FALSE == iter->Get_UnitDead())
		{
			vecPos.push_back(iter->Get_SphereCollider()->GetPos());
		}
	}
	return vecPos;
}

void CGameManager::Add_Cost(const _float & fIncrease)
{
	m_fBattleCost += fIncrease;

	m_fBattleCost = CUtility::Clamp_float(m_fBattleCost, 0.f, 10.f);
}

void CGameManager::Add_PlayerMascot(E_SQUAD eSquad)
{
	if (!m_pTileField)
	{
		return;
	}

	_int iReaderIndex = m_iReaderIndex[(_int)eSquad];
	E_CHAR_NAME eReaderName = m_mapFormation[(_int)eSquad][(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][iReaderIndex]->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
	wstring wstrName = CCharacterMgr::GetInstance()->Translation_EnumToString((_int)eReaderName, E_CHAR_ENUMTYPE::CNUM_NAME);

	CMascot* pMascot = CMascot::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player, eSquad, wstrName);
	if (!pMascot)
	{
		return;
	}

	m_pTileField->Add_Squad_Mascot(pMascot);
}

vector<E_SQUAD> CGameManager::Get_TileFieldSquad()
{
	if (!m_pTileField)
	{
		return vector<E_SQUAD>();
	}

	return m_pTileField->Get_Squad();
}

void CGameManager::End_War(_bool bWin)
{
	if (!m_pTileField)
	{
		return;
	}

	m_pTileField->End_War(bWin);

}


// 아이템 Compare 함수
_bool CGameManager::Sort_ItemFilter(T_ItemInfo& pSrc, T_ItemInfo& pDst)
{
	switch (m_eItemSortType)
	{
	case Engine::E_ItemSortType::Basic:
		return FALSE;
		break;
	case Engine::E_ItemSortType::Name:
		return FALSE;
		break;
	case Engine::E_ItemSortType::Amount:
	{
		if (pSrc.iCount >= pDst.iCount)
			return TRUE;
	}
		
		break;
	case Engine::E_ItemSortType::Effect:
	{
		if (pSrc.iItemEffect >= pDst.iItemEffect)
			return TRUE;
	}
		break;
	case Engine::E_ItemSortType::E_End:
		break;
	default:
		break;
	}

	return FALSE;
}

// bFilter가 True이고 iCount가 1 이상인 아이템만 vecItem에 집어넣고
// eSortType 으로 재정렬해준다.
void CGameManager::Get_Inventory(_bool* bFilter, E_ItemSortType eSortType, vector<T_ItemInfo*>* vecItem)
{
	vecItem->clear();
	for (_int i = 0; i < (_int)m_tAll_Item.size(); ++i)
	{
		if (bFilter[(_int)m_tAll_Item[i].eItemFilter])
		{
			if (1 <= m_tAll_Item[i].iCount)
			{
				vecItem->push_back(&m_tAll_Item[i]);
			}
		}
	}

	// 정렬
	m_eItemSortType = eSortType;
	switch (m_eItemSortType)
	{
	case Engine::E_ItemSortType::Basic:
		sort(vecItem->begin(), vecItem->end());
		break;
	case Engine::E_ItemSortType::Name:
		// sort(vecItem->begin(), vecItem->end());
		break;
	case Engine::E_ItemSortType::Amount:
		Sort_Item(vecItem);
		break;
	case Engine::E_ItemSortType::Effect:
		Sort_Item(vecItem);
		break;
	default:
		assert(FALSE);
		break;
	}
}

// SortType에 따라 버블정렬해주는 함수
void CGameManager::Sort_Item(vector<T_ItemInfo*>* vecItem)
{
	T_ItemInfo* pTemp = nullptr;
	for (_int i = 0; i < vecItem->size(); ++i)
	{
		for (_int j = 0; j < vecItem->size(); ++j)
		{
			if (i == j)
				continue;

			switch (m_eItemSortType)
			{
			case Engine::E_ItemSortType::Basic:
				break;
			case Engine::E_ItemSortType::Name:
				break;
			case Engine::E_ItemSortType::Amount:
				if ((*vecItem)[i]->iCount > (*vecItem)[j]->iCount)
				{
					pTemp = (*vecItem)[i];
					(*vecItem)[i] = (*vecItem)[j];
					(*vecItem)[j] = pTemp;
				}
				break;
			case Engine::E_ItemSortType::Effect:
				if ((*vecItem)[i]->iItemEffect > (*vecItem)[j]->iItemEffect)
				{
					pTemp = (*vecItem)[i];
					(*vecItem)[i] = (*vecItem)[j];
					(*vecItem)[j] = pTemp;
				}
				break;
			case Engine::E_ItemSortType::E_End:
				break;
			default:
				break;
			}
		}
	}
}

void CGameManager::Ready_Formtion()
{
	for (_int i = 0; i < (_int)E_SQUAD::SQUAD_END; ++i)
	{
		vector<CCharacter*> vecStriker;
		vecStriker.resize(4);
		for (_int j = 0; j < 4; ++j)
		{
			vecStriker[j] = nullptr;
		}
		m_mapFormation[i].emplace((_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER, vecStriker);

		vector<CCharacter*> vecSpecial;
		vecSpecial.resize(2);
		for (_int j = 0; j < 2; ++j)
		{
			vecSpecial[j] = nullptr;
		}
		m_mapFormation[i].emplace((_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL, vecSpecial);
	}
}

void CGameManager::Release_Formation()
{
	for (auto& mapSquad : m_mapFormation)
	{
		for (auto& tPair : mapSquad)
		{
			for (auto& pChar : tPair.second)
			{
				Safe_Release(pChar);
			}
			tPair.second.clear();
		}
		mapSquad.clear();
	}

	Set_CurrentSquadEnum(E_SQUAD::SQUAD_1);
}

void CGameManager::Initialize_Formation()
{
	CCharacter* pChar = nullptr;
	for (_int i = 0; i < 4; ++i)
	{
		if (i == 0)
		{
			pChar = CChar_Wakamo::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
			Add_Formation(pChar, E_SQUAD::SQUAD_1);
		}
		if (i == 1)
		{
			pChar = CChar_Sumire::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
			Add_Formation(pChar, E_SQUAD::SQUAD_1);
		}
		if (i == 2)
		{
			pChar = CChar_Suzumi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
			Add_Formation(pChar, E_SQUAD::SQUAD_1);
		}
		if (i == 3)
		{
			pChar = CChar_Pina::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
			Add_Formation(pChar, E_SQUAD::SQUAD_1);
		}

		//if (i == 0)
		//{
		//	pChar = CChar_Neru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
		//	Add_Formation(pChar, E_SQUAD::SQUAD_1);
		//}
		//if (i == 1)
		//{
		//	pChar = CChar_Hina::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
		//	Add_Formation(pChar, E_SQUAD::SQUAD_1);
		//}
		//if (i == 2)
		//{
		//	pChar = CChar_Shun::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
		//	Add_Formation(pChar, E_SQUAD::SQUAD_1);
		//}
		//if (i == 3)
		//{
		//	pChar = CChar_Aru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);
		//	Add_Formation(pChar, E_SQUAD::SQUAD_1);
		//}

	}

	Set_CurrentSquadEnum(E_SQUAD::SQUAD_1);
}

_bool CGameManager::Add_Formation(CCharacter * pCharacter, E_SQUAD eSquad)
{
	if (!pCharacter)
	{
		Safe_Release(pCharacter);
		return FALSE;
	}
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		Safe_Release(pCharacter);
		return FALSE;
	}
	_int iMaxSize;

	_int iCombatType = (_int)pCharacter->Get_CharInfo().eCombatType;
	if (iCombatType == (_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		iMaxSize = 4;
	}
	else if(iCombatType == (_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL)
	{
		iMaxSize = 2;
	}
	else
	{
		Safe_Release(pCharacter);
		return FALSE;
	}
	
	for (_int i = 0; i < iMaxSize; ++i)
	{
		if (!m_mapFormation[(_int)eSquad][iCombatType][i])
		{
			m_mapFormation[(_int)eSquad][iCombatType][i] = pCharacter;

			m_mapCurrentSquad = &m_mapFormation[(_int)eSquad];

			return TRUE;
		}
	}

	Safe_Release(pCharacter);
	return FALSE;
}

_bool CGameManager::Add_Formation(E_CHAR_NAME eCharName, E_SQUAD eSquad, E_CHAR_COMBATTYPE eCombatType, _int iIndex)
{
	CCharacter* pChar = nullptr;

	if (iIndex < 0)
	{
		return FALSE;
	}
	if (eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		if (iIndex >= 4)
		{
			return FALSE;
		}
		pChar = Create_CharacterByEnum(eCharName);
		if (pChar)
		{
			m_mapFormation[(_int)eSquad][(_int)eCombatType][iIndex] = pChar;
		}
	}
	else
	{
		if (iIndex >= 2)
		{
			return FALSE;
		}
		pChar = Create_CharacterByEnum(eCharName);
		if (pChar)
		{
			m_mapFormation[(_int)eSquad][(_int)eCombatType][iIndex] = pChar;
		}
	}

	return TRUE;
}

_bool CGameManager::Add_Formation(E_CHAR_NAME eCharName, E_SQUAD eSquad)
{
	CCharacter* pChar = nullptr;
	
	pChar = Create_CharacterByEnum(eCharName);
	if (!pChar)
	{
		return FALSE;
	}

	return Add_Formation(pChar, eSquad);
}
                   
E_SQUAD CGameManager::DuplicateCheck_Character(E_CHAR_NAME eCharName, E_SQUAD eSquadStart, E_SQUAD eSquadEnd, E_SQUAD eCurrentSquad)
{
	if ((_int)eCharName < 0 || (_int)eCharName >= (_int)E_CHAR_NAME::Name_End)
	{
		return E_SQUAD::SQUAD_END;
	}


	for (_int i = (_int)eSquadStart; i <= (_int)eSquadEnd; ++i)
	{
		if (i == (_int)eCurrentSquad)
		{
			continue;
		}

		CCharacter* pChar = nullptr;
		for (_int j = 0; j < 4; ++j)
		{
			pChar = m_mapFormation[i][(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][j];
			if (pChar)
			{
				if (pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG] == eCharName)
				{
					return E_SQUAD(i);
				}
			}
		}
		for (_int j = 0; j < 2; ++j)
		{
			pChar = m_mapFormation[i][(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][j];
			if (pChar)
			{
				if (pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG] == eCharName)
				{
					return E_SQUAD(i);
				}
			}
		}
	}

	return E_SQUAD::SQUAD_END;
}

void CGameManager::Spawn_Formation(E_SQUAD eSquad)
{
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return;
	}

	for (auto& tPair : m_mapFormation[(_int)eSquad])
	{
		
		for (auto& pChar : tPair.second)
		{
			if (pChar)
			{
				if (tPair.first == (_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
				{
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player, pChar);
					pChar->AddRef();
					pChar->Set_Dead(FALSE);
				}
				else
				{
					CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Special, pChar);
					pChar->AddRef();
					pChar->Set_Dead(FALSE);
				}
			}
		}
	}
}

void CGameManager::DeleteStudent(E_CHAR_NAME eCharName, E_SQUAD eStartSquad, E_SQUAD eEndSquad)
{
	E_CHAR_COMBATTYPE eCombatType = CCharacterMgr::GetInstance()->Get_CharInfo(eCharName).eCombatType;

	for (_int i = (_int)eStartSquad; i <= (_int)eEndSquad; ++i)
	{
		_int iIndex = 0;
		for (auto& pChar : m_mapFormation[i][(_int)eCombatType])
		{
			if (pChar)
			{
				E_CHAR_NAME eStudentCharName = pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG];
				if (eStudentCharName == eCharName)
				{
					pChar->Set_Dead(TRUE);
					Safe_Release(pChar);
					m_mapFormation[i][(_int)eCombatType][iIndex] = nullptr;
					return;
				}
			}
			++iIndex;
		}
		
	}
}

CCharacter * CGameManager::Create_CharacterByEnum(E_CHAR_NAME eCharName) 
{
	CCharacter* pChar = nullptr;

	switch (eCharName)
	{
		//스트라이커
	case Engine::E_CHAR_NAME::Name_Neru:		pChar = CChar_Neru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Shun:		pChar = CChar_Shun::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Serika:		pChar = CChar_Serika::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Maki:		pChar = CChar_Maki::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Akane:		pChar = CChar_Akane::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Mutsuki:		pChar = CChar_Mutsuki::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Aru:			pChar = CChar_Aru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Eimi:		pChar = CChar_Eimi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Iori:		pChar = CChar_Iori::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Izumi:		pChar = CChar_Izumi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Hasumi:		pChar = CChar_Hasumi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Hina:		pChar = CChar_Hina::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Chise:		pChar = CChar_Chise::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Sumire:		pChar = CChar_Sumire::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Suzumi:		pChar = CChar_Suzumi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Pina:		pChar = CChar_Pina::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Kayoko:		pChar = CChar_Kayoko::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);	break;
	case Engine::E_CHAR_NAME::Name_Momoi:		pChar = CChar_Momoi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
	case Engine::E_CHAR_NAME::Name_Zunko:		pChar = CChar_Zunko::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);     break;
	case Engine::E_CHAR_NAME::Name_Wakamo:		pChar = CChar_Wakamo::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);    break;
	case Engine::E_CHAR_NAME::Name_Aris:		pChar = CChar_Aris::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Player);		break;
		//스페셜
	case Engine::E_CHAR_NAME::Name_Mashiro:		pChar = CChar_Mashiro::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;
	case Engine::E_CHAR_NAME::Name_Airi:		pChar = CChar_Airi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);		break;
	case Engine::E_CHAR_NAME::Name_Juri:		pChar = CChar_Juri::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);		break;
	case Engine::E_CHAR_NAME::Name_Chinatsu:	pChar = CChar_Chinatsu::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;
	case Engine::E_CHAR_NAME::Name_Karin:		pChar = CChar_Karin::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;
	case Engine::E_CHAR_NAME::Name_Hibiki:		pChar = CChar_Hibiki::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;
	case Engine::E_CHAR_NAME::Name_Utaha:		pChar = CChar_Utaha::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;
	case Engine::E_CHAR_NAME::Name_Yoshimi:		pChar = CChar_Yoshimi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Special);	break;

		//아직 미분류
	case Engine::E_CHAR_NAME::Name_Serina:
		break;
	case Engine::E_CHAR_NAME::Name_Saya:
		break;
	case Engine::E_CHAR_NAME::Name_Nonomi:
		break;
	case Engine::E_CHAR_NAME::Name_Shiroko:
		break;
	case Engine::E_CHAR_NAME::Name_Shimiko:
		break;
	case Engine::E_CHAR_NAME::Name_Shizuko:
		break;
	case Engine::E_CHAR_NAME::Name_Asuna:
		break;
	case Engine::E_CHAR_NAME::Name_Ayane:
		break;
	case Engine::E_CHAR_NAME::Name_Izuna:
		break;
	case Engine::E_CHAR_NAME::Name_Akari:
		break;
	case Engine::E_CHAR_NAME::Name_Yuuka:
		break;
	case Engine::E_CHAR_NAME::Name_Tsurugi:
		break;
	case Engine::E_CHAR_NAME::Name_Tsubaki:
		break;
	case Engine::E_CHAR_NAME::Name_Kotama:
		break;
	case Engine::E_CHAR_NAME::Name_Kotori:
		break;
	case Engine::E_CHAR_NAME::Name_Hanae:
		break;
	case Engine::E_CHAR_NAME::Name_Hare:
		break;
	case Engine::E_CHAR_NAME::Name_Haruna:
		break;
	case Engine::E_CHAR_NAME::Name_Haruka:
		break;
	case Engine::E_CHAR_NAME::Name_Hoshino:
		break;
	case Engine::E_CHAR_NAME::Name_Fuuka:
		break;
	case Engine::E_CHAR_NAME::Name_Hihumi:
		break;
	case Engine::E_CHAR_NAME::Name_End:
		break;
	default:
		break;
	}

	return pChar;
}

void CGameManager::Reset_Squad(E_SQUAD eSquad)
{
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return;
	}

	for (auto& tPair : m_mapFormation[(_int)eSquad])
	{
		for (auto& pChar : tPair.second)
		{
			if (pChar)
			{
				pChar->Reset();
			}
		}
	}

	// 현재 스쿼드를 리셋스쿼드로
	m_mapCurrentSquad = &m_mapFormation[(_int)eSquad];
}

void CGameManager::Swap_Character(E_SQUAD eSquad, E_CHAR_COMBATTYPE eCombatType, _int iDestIndex, _int iSrcIndex)
{
	if ((_int)eSquad < 0 || eSquad >= E_SQUAD::SQUAD_END)
	{
		return;
	}
	if (iDestIndex < 0 || iSrcIndex < 0)
	{
		return;
	}
	if (eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER)
	{
		if (iDestIndex >= 4|| iSrcIndex >= 4)
		{
			return;
		}
	}
	else if (eCombatType == E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL)
	{
		if (iDestIndex >= 2 || iSrcIndex >= 2)
		{
			return;
		}
	}
	else
	{
		return;
	}

	CCharacter* pCharTemp = nullptr;
	pCharTemp = m_mapFormation[(_int)eSquad][(_int)eCombatType][iDestIndex];
	m_mapFormation[(_int)eSquad][(_int)eCombatType][iDestIndex] = m_mapFormation[(_int)eSquad][(_int)eCombatType][iSrcIndex];
	m_mapFormation[(_int)eSquad][(_int)eCombatType][iSrcIndex] = pCharTemp;
}

map<_int, vector<CCharacter*>>* CGameManager::Get_Squad(E_SQUAD eSquad)
{
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return nullptr;
	}

	return &m_mapFormation[(_int)eSquad];
}

// 주의사항 Null일 수 있다. 가져갈 때 예외처리 해야함.
vector<CCharacter*>* CGameManager::Get_CurrentSquad_Strikers()
{
	return &((*m_mapCurrentSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER]);
}

vector<CCharacter*> CGameManager::Get_CurrentSquad_Alive_Strikers()
{
	vector<CCharacter*>* pCurrentStrikers = Get_CurrentSquad_Strikers();
	vector<CCharacter*> vecAliveStriker;
	for (auto iter : (*pCurrentStrikers))
	{
		if (nullptr == iter || iter->Get_UnitDead())
			continue;

		vecAliveStriker.push_back(iter);
	}

	return vecAliveStriker;
}

void CGameManager::Set_CurrentSquadEnum(E_SQUAD eCurrentSquad)
{
	m_eCurrentSquad = eCurrentSquad;
}

_int CGameManager::GetReaderIndex(E_SQUAD eSquad)
{
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return -1;
	}
	
	return m_iReaderIndex[(_int)eSquad];
}

void CGameManager::SetReaderIndex(E_SQUAD eSquad, _int iNewReaderIndex)
{
	if ((_int)eSquad < 0 || (_int)eSquad >= (_int)E_SQUAD::SQUAD_END)
	{
		return;
	}
	if (iNewReaderIndex < 0 || iNewReaderIndex >= 4)
	{
		return;
	}

	m_iReaderIndex[(_int)eSquad] = iNewReaderIndex;
}

void CGameManager::Initialize_SkillOrder()
{
	m_vecBattleSkillOrder.clear();
	// 0 = 0번째 슬롯에있는 스트라이커
	// 10 = 0번째 슬롯에있는 스폐셜
	for (_int i = 0; i < 4; ++i)
	{
		if ((*m_mapCurrentSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][i])
		{
			m_vecBattleSkillOrder.push_back(i);
		}
	}

	for (_int i = 0; i < 2; ++i)
	{
		if ((*m_mapCurrentSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL][i])
		{
			m_vecBattleSkillOrder.push_back(10 + i);
		}
	}

	_int iMaxSize = (_int)m_vecBattleSkillOrder.size();
	for (_int i = 0; i < iMaxSize * 3; ++i)
	{
		_int iFirstRand = CWall512::GetInstance()->GetValue(0, iMaxSize);
		_int iSecondRand = CWall512::GetInstance()->GetValue(0, iMaxSize);

		_int iTemp = m_vecBattleSkillOrder[iFirstRand];
		m_vecBattleSkillOrder[iFirstRand] = m_vecBattleSkillOrder[iSecondRand];
		m_vecBattleSkillOrder[iSecondRand] = iTemp;
	}

  	for (_int i = iMaxSize - 1; i >= 3; --i)
	{
		m_qSkillOrder.push(m_vecBattleSkillOrder[i]);
		m_vecBattleSkillOrder.pop_back();
	}
}

void CGameManager::Use_Skill(CCharacter* pChar)
{
	pChar->Use_ExSkill();
	m_fBattleCost -= pChar->Get_CharInfo().iExSkillCost;

	m_qSkillOrder.push(m_vecBattleSkillOrder[m_iLastPickSkillSlot]);

	_int iNext = m_qSkillOrder.front();
	m_qSkillOrder.pop();

	m_vecBattleSkillOrder[m_iLastPickSkillSlot] = iNext;

	// CGameManager::GetInstance()->Get_RangeEffect(E_RangeEffect::Circle_OutLine)->Set_Pos(pChar->Get_Position());
}

CUnit * CGameManager::Get_SkillUnit(_int iSkillSlot)
{
	if (iSkillSlot + 1 > m_vecBattleSkillOrder.size())
		return nullptr;

	_int iWho = m_vecBattleSkillOrder[iSkillSlot];
	_int iUnitIndex = iWho % 10;
	_int iNeedCost = 100;

	CCharacter* pChar = nullptr;
	if (iWho < 10)
	{
		pChar = static_cast<CCharacter*>((*m_mapCurrentSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][iUnitIndex]);
		iNeedCost = pChar->Get_CharInfo().iExSkillCost;
	}
	else
	{
		pChar = static_cast<CCharacter*>((*m_mapCurrentSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER][iUnitIndex]);
		iNeedCost = pChar->Get_CharInfo().iExSkillCost;
	}
	
	// 코스트가 없으면 불가능
	if (m_fBattleCost < pChar->Get_CharInfo().iExSkillCost)
	{
		pChar = nullptr;
	}

	m_iLastPickSkillSlot = iSkillSlot;
	return pChar;
}

void CGameManager::Collect_Update()
{
	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		if (Get_UserInfo().bCharacter[i])
		{
			CCharacterMgr::GetInstance()->Set_Collect(E_CHAR_NAME(i));
		}
	}
}
