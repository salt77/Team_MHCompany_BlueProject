#include "WorldCreator.h"
#include "StageMap.h"
#include "Obstacle_Combat.h"
#include "GameManager.h"
#include "Scene.h"
#include "UI_Manager.h"
#include "XL_ParsingMgr.h"
#include "TimeScale_Manager.h" 
//몬스터 목록

//특별의뢰 몬스터
#include "BoxCat.h"

#include "Cook_GenbuStore_AR.h"
#include "Cook_GenbuStore_MG.h"
#include "Cook_GenbuStore_SG.h"
#include "Droid_Helmet_AR.h"
#include "Droid_Helmet_RL.h"
#include "Droid_Helmet_SMG.h"
#include "Droid_HelmetBoss_SMG.h"

#include "Sukeban_SMG.h"
#include "Sukeban_MG.h"
#include "Sukeban_SR.h"
#include "HeavyTank01_Helmet_Cannon.h"

#include "TechSoldier_Decagram_AR.h"
#include "Sweeper_Decagram_Taser.h"
#include "Yukata_StreetGhostes_RL.h"
#include "Droid_Yoheki_Shield.h"
#include "Drone_Decagram_Missile.h"
#include "Drone_Decagram_Vulcan.h"
#include "EnemyTSS_Aparche_Blackmarket.h"
#include "EnemyTSS_Crusader_PMC.h"
#include "FoodTruck_GenbuStore_MG.h"
#include "FX_Utaha_Original_Turret.h"
#include "GasMask_Arius_HMG.h"
#include "GasMask_Arius_SMG.h"
#include "HeavyTank_Decagram_Cannon.h"
#include "Kaitenranger_Black_HG.h"
#include "Kaitenranger_Green_HMG.h"
#include "Kaitenranger_Pink_RL.h"
#include "Kaitenranger_Red_AR.h"
#include "Kaitenranger_Yellow_SR.h"

#include "Terror_Droid_Mimesis_AR.h"
#include "Terror_Droid_Mimesis_RL.h"
#include "Terror_Droid_Mimesis_Shield.h"
#include "Terror_GasMask_Arius_GL.h"
#include "Terror_GasMask_Arius_HMG.h"
#include "Terror_GasMask_Arius_SMG.h"

#include "Yukata_StreetGhostes_MG.h"
#include "Yukata_StreetGhostes_RL.h"
#include "Yukata_StreetGhostes_SG.h"
#include "Yukata_StreetGhostes_SG_Turret.h"
#include "YukataHanya_StreetGhostes_SR.h"
#include "YukataHanya_StreetGhostes_SR_Cannon.h"
#include "YukataHanya_StreetGhostes_SR_Turret.h"

#include "Yustina_AR.h"
#include "Yustina_SMG.h"
#include "Yustina_SR.h"

#include "KumabotPapa_Slumpia_SG.h"
#include "LightTank_Helmet_Vulcan.h"
#include "RoyalGuard_RedWinter_AR.h"
#include "RoyalGuard_RedWinter_GL.h"
#include "RoyalGuard_RedWinter_SMG.h"
#include "RoyalGuard_RedWinterSergeant_AR.h"
#include "SchoolGirl01_Sukeban_SMG.h"
#include "SchoolGirl01_SukebanSwim_SMG.h"
#include "SchoolGirl02_Sukeban_SR.h"
#include "SchoolGirl02_SukebanSwim_SR.h"
#include "SchoolGirl03_Sukeban_HMG.h"
#include "SchoolGirl03_SukebanSwim_MG.h"
#include "SchoolGirlBoss02_SukebanSwim_MG.h"
#include "Soldier_Blackmarket_RL.h"
#include "Soldier_Fuuki_MG.h"
#include "Soldier_Helmet_AR.h"
#include "Soldier_Helmet_SG.h"
#include "Soldier_Helmet_SR.h"
#include "Soldier_Kaiserpmc_HG.h"
#include "Soldier_RedWinterYoheki_SG.h"
#include "Soldier_RedWinterYoheki_SR.h"
#include "Soldier_RWY_AR.h"
#include "Soldier_YohekiPink_MG.h"
#include "Soldier_YohekiPink_SMG.h"
#include "Goliath_Decagram_Vulcan.h"

#include "Shiro.h" 
#include "Kuro.h" 
#include "Hieronymus.h"
#include "Binah.h"
#include "Binah_Tail.h"
#include "Hod.h"

#include "Char_Neru.h"
#include "Char_Aru.h"
#include "Char_Suzumi.h"
#include "Char_Momoi.h"
#include "Char_Hina.h"
#include "Char_Shun.h"
#include "Char_Akane.h"
#include "Char_Sumire.h"
#include "Char_Serika.h"
#include "Char_Maki.h"
#include "Char_Mutsuki.h"

USING(Engine)

//저장된 월드를 불러오는 생성자 
CWorldCreator::CWorldCreator()
{
}

CWorldCreator::~CWorldCreator()
{
}

CWorldCreator * CWorldCreator::Create(wstring wstrWorldName)
{
	CWorldCreator* pInstance = new CWorldCreator();

	if (FAILED(pInstance->Ready_World(wstrWorldName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorldCreator::Free()
{
	for (auto& pObj : m_vecMapObject)
	{
		Safe_Release(pObj);
	}
	m_vecMapObject.clear();
	for (auto& pObj : m_vecObstacleObject)
	{
		Safe_Release(pObj);
	}
	m_vecObstacleObject.clear();

	for (auto& iter : m_vecGroupInfo)
	{
		iter.clear();
	}
	m_vecGroupInfo.clear();

	for (auto& iter : m_vecGroupPos)
	{
		iter.clear();
	}
	m_vecGroupPos.clear();

	for (auto& iter : m_vecGroupRot)
	{
		iter.clear();
	}
	m_vecGroupRot.clear();

	m_vecMonsterPool.clear();
	m_vecStartPos.clear();


	_int iFrontLineSize = (_int)m_qFrontLines.size();
	for (_int i = 0; i < iFrontLineSize; ++i)
	{
		vector<CUnit*>* pVec = m_qFrontLines.front();
		for (auto& iter : (*pVec))
		{
			Safe_Release(iter);
		}
		pVec->clear();

		Safe_Delete(pVec);
		m_qFrontLines.pop();
	}
}


HRESULT CWorldCreator::Ready_World(wstring wstrWorldName)
{
	Set_WorldName(wstrWorldName);

	HRESULT hResult = Load_World(wstrWorldName);

	return hResult;
}

void CWorldCreator::Update_World(const _float & fTimeDelta)
{
	vector<CGameObject*>* pVecMonster = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster);
	if (nullptr == pVecMonster)
		return;

	_bool bFrontLineAllDead = TRUE;
	for (auto& iter : (*pVecMonster))
	{
		if (FALSE == static_cast<CUnit*>(iter)->Get_UnitDead())
		{
			bFrontLineAllDead = FALSE;
			break;
		}
	}
	_bool bStrikersAllDead = (_bool)CGameManager::GetInstance()->Get_AliveStrikersPosition().empty();

	if (bFrontLineAllDead && m_bWorldBattleIng)
	{
		if (m_qFrontLines.empty())
		{
			if (!m_bBattleEndDelay)
			{
				m_bBattleEndDelay = TRUE;
				CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, m_fBattleEndDelay);
			}
			else
			{
				m_fBattleEndDelay -= fTimeDelta; 

				if (0.f >= m_fBattleEndDelay)
				{
					m_bWorldBattleIng = FALSE;
					if (FALSE == m_bIsTileFieldWorld)
					{
						CGameManager::GetInstance()->Set_BattleIng(FALSE);
						CGameManager::GetInstance()->Set_IsBattleWin(TRUE);
						CGameManager::GetInstance()->Set_EndBattle();
					}

					if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
					{
						vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player);

						for (_uint i = 0; i < vecObject->size(); ++i)
						{
							if (static_cast<CCharacter*>((*vecObject)[i])->Get_Reader())
							{
								static_cast<CCharacter*>((*vecObject)[i])->End_Battle_Sound(TRUE);

								break;
							}
						}
					}
				}
			}
			
			return;
		}
		Spawn_Frontline();
	}
	else if (bStrikersAllDead && m_bWorldBattleIng)
	{
		if (!m_bBattleEndDelay) 
		{
			m_bBattleEndDelay = TRUE;
			CTimeScale_Manager::GetInstance()->Set_TimeScale_Duration(0.1f, m_fBattleEndDelay);
		}
		else 
		{
			m_fBattleEndDelay -= fTimeDelta; 

			if (0.f >= m_fBattleEndDelay)
			{
				m_bWorldBattleIng = FALSE;
				CGameManager::GetInstance()->Set_BattleIng(FALSE);
				CGameManager::GetInstance()->Set_IsBattleWin(FALSE);
				CGameManager::GetInstance()->Set_EndBattle(); 

				if (CScene::Get_MainScene()->IsExist_Obj(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player))
				{
					vector<CGameObject*>*	vecObject = CScene::Get_MainScene()->Get_vecGameObject(E_LAYERTAG::Character, E_OBJTAG::OBJ_Player); 

					for (_uint i = 0; i < vecObject->size(); ++i) 
					{
						if (static_cast<CCharacter*>((*vecObject)[i])->Get_Reader()) 
						{
							static_cast<CCharacter*>((*vecObject)[i])->End_Battle_Sound(FALSE);

							break; 
						}
					}
				}
			}
		}

		return;
	}
}

void CWorldCreator::Ready_FrontLine()
{
	CGameManager::GetInstance()->Set_NextWorld(this);
	CGameManager::GetInstance()->Set_BattleIng(TRUE);

	while (!m_vecGroupInfo.empty())
	{
		vector<CUnit*>* vecTemp = new vector<CUnit*>;

		_int iSize = _int(m_vecGroupInfo.front().size());

		if (CGameManager::GetInstance()->m_bUserBattle)
		{
			while (!m_qUserUnitLevel.empty())
				m_qUserUnitLevel.pop();

			for (_int i = 0; i < 4; i++)
			{
				if (CGameManager::GetInstance()->m_iEnemyUserSquad[i] != -1)
				{
					wstring wstrName = CXL_ParsingMgr::GetInstance()->Translation_EnumToString(CGameManager::GetInstance()->m_iEnemyUserSquad[i], E_CHAR_ENUMTYPE::CNUM_NAME);
					m_qUserUnitLevel.push(CGameManager::GetInstance()->m_iEnemyUserSquadLevel[i]);
					vecTemp->push_back(NameTranslateSpawn(wstrName, m_vecGroupPos.front()[i], m_vecGroupRot.front()[i], m_vecGroupInfo.front()[i]));
				}
			}
		}
		else
		{
			for (_int i = 0; i < iSize; ++i)
			{
				vecTemp->push_back(NameTranslateSpawn(m_vecGroupInfo.front()[i].wszName, m_vecGroupPos.front()[i], m_vecGroupRot.front()[i], m_vecGroupInfo.front()[i]));
			}
		}

		auto iterGroupInfo = m_vecGroupInfo.begin();
		auto iterGroupPos = m_vecGroupPos.begin();
		auto iterGroupRot = m_vecGroupRot.begin();
		m_vecGroupInfo.erase(iterGroupInfo);
		m_vecGroupPos.erase(iterGroupPos);
		m_vecGroupRot.erase(iterGroupRot);
		m_qFrontLines.push(vecTemp);

		CGameManager::GetInstance()->m_bUserBattle = FALSE;
	}

}


_vec3 CWorldCreator::Get_StartPos(_int iIndex)
{
	if (iIndex < 0 || iIndex >= (_int)m_vecStartPos.size())
	{
		CUtility::Print_Wstring_DMode(L"WorldCreator - 시작지점 인덱스가 초과되었습니다");
		return _vec3(0.f, 0.f, 0.f);
	}

	return m_vecStartPos[iIndex];
}

vector<T_MONSTER_INFO>& CWorldCreator::Get_MonsterInfo()
{
	return m_vecMonsterPool;
}

_int CWorldCreator::Get_HighLevelMonster()
{
	_uint iHigh = 0;
	for (_int i = 0; i < (_int)m_vecMonsterPool.size(); ++i)
	{
		if (iHigh < m_vecMonsterPool[i].iLevel)
			iHigh = m_vecMonsterPool[i].iLevel;
	}
	return iHigh;
}

void CWorldCreator::Set_WorldName(wstring wstrWorldName)
{
	m_wstrWorldName = wstrWorldName;
}

void CWorldCreator::Reset_World()
{
	if (CGameManager::GetInstance()->m_bUserBattle)
	{
		Load_World(L"UserBattle.world");
	}
	else	// 일반 전투 
	{
		Load_World(m_wstrWorldName);
	}
}


HRESULT CWorldCreator::Load_World(wstring wstrWorldName)
{
	Free();

	wstring wstrLocalPath = L"../../Reference/Data/Data_World/" + wstrWorldName;

	HANDLE hFile = CreateFile(wstrLocalPath.c_str(), GENERIC_READ, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}

	DWORD dwByte;

	_int iSize;
	//지형 불러오기
	CUtility::Data_ReadInt(hFile, iSize, dwByte);

	for (int i = 0; i < iSize; ++i)
	{
		_int iObjTag = 0;
		wstring wstrName;
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile, wstrName, dwByte);
		CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
		CUtility::Data_ReadInt(hFile, iObjTag, dwByte);


		CStageMap* pMap;
		pMap = CStageMap::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Map, wstrPath, wstrName);
		pMap->Load(hFile, dwByte);
		m_vecMapObject.emplace_back(pMap);
	}

	CUtility::Data_ReadInt(hFile, iSize, dwByte);

	for (int i = 0; i < iSize; ++i)
	{
		_int iObjTag = 0;
		wstring wstrName;
		wstring wstrPath;
		CUtility::Data_ReadWString(hFile, wstrName, dwByte);
		CUtility::Data_ReadWString(hFile, wstrPath, dwByte);
		CUtility::Data_ReadInt(hFile, iObjTag, dwByte);

		CObstacle_Combat* pObstacle;
		pObstacle = CObstacle_Combat::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Obstacle, wstrPath, wstrName);
		pObstacle->Load(hFile, dwByte);
		m_vecObstacleObject.emplace_back(pObstacle);
	}


	//몬스터 풀 정보 불러오기
	CUtility::Data_ReadInt(hFile, iSize, dwByte);

	for (_int i = 0; i < iSize; ++i)
	{
		T_MONSTER_INFO tMonsterInfo;
		ZeroMemory(&tMonsterInfo, sizeof(T_MONSTER_INFO));
		ReadFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);
		m_vecMonsterPool.emplace_back(tMonsterInfo);

	}

	//몬스터 그룹 정보 불러오기
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	_int iSize2;
	for (_int i = 0; i < iSize; ++i)
	{
		CUtility::Data_ReadInt(hFile, iSize2, dwByte);
		_vec3 vPos;
		_vec3 vRot;
		T_MONSTER_INFO tMonsterInfo;

		m_vecGroupInfo.emplace_back(vector<T_MONSTER_INFO>());
		m_vecGroupPos.emplace_back(vector<_vec3>());
		m_vecGroupRot.emplace_back(vector<_vec3>());
		for (_int j = 0; j < iSize2; ++j)
		{
			CUtility::Data_ReadVec3(hFile, vPos, dwByte);
			CUtility::Data_ReadVec3(hFile, vRot, dwByte);
			m_vecGroupPos[i].emplace_back(vPos);
			m_vecGroupRot[i].emplace_back(vRot);
			ReadFile(hFile, &tMonsterInfo, sizeof(T_MONSTER_INFO), &dwByte, nullptr);
			m_vecGroupInfo[i].emplace_back(tMonsterInfo);
		}
	}

	//스타팅 위치 불러오기
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	_vec3 vPos;
	for (_int i = 0; i < iSize; ++i)
	{
		CUtility::Data_ReadVec3(hFile, vPos, dwByte);
		m_vecStartPos.emplace_back(vPos);
	}


	//월드 모드, 타입 불러오기
	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	m_eWorldMod = E_WORLD_MOD(iSize);

	CUtility::Data_ReadInt(hFile, iSize, dwByte);
	m_eWorldType = E_WORLD_TYPE(iSize);

	CloseHandle(hFile);

	return S_OK;
}

void CWorldCreator::Spawn_World()
{
	m_bWorldBattleIng = TRUE;
	Ready_FrontLine();
	for (auto& pObj : m_vecMapObject)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Map, E_OBJTAG::OBJ_Map, pObj);
	}
	m_vecMapObject.clear();
	for (auto& pObj : m_vecObstacleObject)
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Obstacle, E_OBJTAG::OBJ_Obstacle, pObj);
	}
	m_vecObstacleObject.clear();

	CGameManager::GetInstance()->Initialize_SkillOrder();
	CUI_Manager::GetInstance()->Active_Combat_Ing();
}

void CWorldCreator::Spawn_Frontline()
{
	if (m_qFrontLines.empty())
	{
		return;
	}

	CGameManager::GetInstance()->Set_NextWorld(this);
	CGameManager::GetInstance()->Set_BattleIng(TRUE);

	vector<CUnit*>* pVecFront = m_qFrontLines.front();

	for (auto& iter : (*pVecFront))
	{
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Monster, E_OBJTAG::OBJ_Monster, iter);
	}
	Safe_Delete(pVecFront);
	m_qFrontLines.pop();
}

CUnit* CWorldCreator::NameTranslateSpawn(wstring wstrName, _vec3 vPos, _vec3 vRot, T_MONSTER_INFO& tMonsterInfo)
{
	CUnit* pUnit = nullptr;
	_bool bMonster = TRUE;
	if (wstrName == L"Sukeban_MG")									pUnit = CSukeban_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Sukeban_SMG")							pUnit = CSukeban_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Sukeban_SR")								pUnit = CSukeban_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"HeavyTank01_Helmet_Cannon")				pUnit = CHeavyTank01_Helmet_Cannon::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"BoxCat") 								pUnit = CBoxCat::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Cook_GenbuStore_AR")						pUnit = CCook_GenbuStore_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Cook_GenbuStore_MG")						pUnit = CCook_GenbuStore_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Cook_GenbuStore_SG")						pUnit = CCook_GenbuStore_SG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Droid_Yoheki_Shield") 					pUnit = CDroid_Yoheki_Shield::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Droid_Helmet_AR") 						pUnit = CDroid_Helmet_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Droid_Helmet_RL")						pUnit = CDroid_Helmet_RL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Droid_Helmet_SMG")						pUnit = CDroid_Helmet_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Droid_HelmetBoss_SMG")					pUnit = CDroid_HelmetBoss_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"Terror_Droid_Mimesis_AR")				pUnit = CTerror_Droid_Mimesis_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Terror_Droid_Mimesis_RL")				pUnit = CTerror_Droid_Mimesis_RL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Terror_Droid_Mimesis_Shield")			pUnit = CTerror_Droid_Mimesis_Shield::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Terror_GasMask_Arius_GL")				pUnit = CTerror_GasMask_Arius_GL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Terror_GasMask_Arius_HMG")				pUnit = CTerror_GasMask_Arius_HMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Terror_GasMask_Arius_SMG")				pUnit = CTerror_GasMask_Arius_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Gasmask_Arius_HMG")						pUnit = CGasmask_Arius_HMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);


	else if (wstrName == L"Yukata_StreetGhostes_MG")				pUnit = CYukata_StreetGhostes_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Yukata_StreetGhostes_RL")				pUnit = CYukata_StreetGhostes_RL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Yukata_StreetGhostes_SG")				pUnit = CYukata_StreetGhostes_SG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Yukata_StreetGhostes_SG_Turret")			pUnit = CYukata_StreetGhostes_SG_Turret::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"YukataHanya_StreetGhostes_SR")			pUnit = CYukataHanya_StreetGhostes_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"YukataHanya_StreetGhostes_SR_Cannon")	pUnit = CYukataHanya_StreetGhostes_SR_Cannon::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"YukataHanya_StreetGhostes_SR_Turret")	pUnit = CYukataHanya_StreetGhostes_SR_Turret::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Yustina_AR")								pUnit = CYustina_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Yustina_SMG")							pUnit = CYustina_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Yustina_SR")								pUnit = CYustina_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"TechSoldier_Decagram_AR")				pUnit = CTechSoldier_Decagram_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Sweeper_Decagram_Taser")					pUnit = CSweeper_Decagram_Taser::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);


	else if (wstrName == L"Drone_Decagram_Missile")					pUnit = CDrone_Decagram_Missile::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Drone_Decagram_Vulcan")					pUnit = CDrone_Decagram_Vulcan::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"EnemyTSS_Aparche_Blackmarket")			pUnit = CEnemyTSS_Aparche_Blackmarket::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"EnemyTSS_Crusader_PMC")					pUnit = CEnemyTSS_Crusader_PMC::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"FoodTruck_GenbuStore_MG")				pUnit = CFoodTruck_GenbuStore_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"FX_Utaha_Original_Turret")				pUnit = CFX_Utaha_Original_Turret::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"HeavyTank_Decagram_Cannon")				pUnit = CHeavyTank_Decagram_Cannon::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Kaitenranger_Black_HG")					pUnit = CKaitenranger_Black_HG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Kaitenranger_Green_HMG")					pUnit = CKaitenranger_Green_HMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Kaitenranger_Pink_RL")					pUnit = CKaitenranger_Pink_RL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Kaitenranger_Red_AR")					pUnit = CKaitenranger_Red_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Kaitenranger_Yellow_SR")					pUnit = CKaitenranger_Yellow_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"KumabotPapa_Slumpia_SG")					pUnit = CKumabotPapa_Slumpia_SG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"LightTank_Helmet_Vulcan")				pUnit = CLightTank_Helmet_Vulcan::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"RoyalGuard_RedWinter_AR")				pUnit = CRoyalGuard_RedWinter_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"RoyalGuard_RedWinter_GL")				pUnit = CRoyalGuard_RedWinter_GL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"RoyalGuard_RedWinter_SMG")				pUnit = CRoyalGuard_RedWinter_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"RoyalGuard_RedWinterSergeant_AR")		pUnit = CRoyalGuard_RedWinterSergeant_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"SchoolGirl01_Sukeban_SMG")				pUnit = CSchoolGirl01_Sukeban_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"SchoolGirl02_Sukeban_SR")				pUnit = CSchoolGirl02_Sukeban_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"SchoolGirl03_Sukeban_HMG")				pUnit = CSchoolGirl03_Sukeban_HMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"SchoolGirl01_SukebanSwim_SMG")			pUnit = CSchoolGirl01_SukebanSwim_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"SchoolGirl02_SukebanSwim_SR")			pUnit = CSchoolGirl02_SukebanSwim_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"SchoolGirl03_SukebanSwim_MG")			pUnit = CSchoolGirl03_SukebanSwim_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"SchoolGirlBoss02_SukebanSwim_MG")		pUnit = CSchoolGirlBoss02_SukebanSwim_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"Soldier_Blackmarket_RL")					pUnit = CSoldier_Blackmarket_RL::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"Soldier_Fuuki_MG")						pUnit = CSoldier_Fuuki_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	
	else if (wstrName == L"Soldier_Helmet_AR")						pUnit = CSoldier_Helmet_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_Helmet_SG")						pUnit = CSoldier_Helmet_SG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_Helmet_SR")						pUnit = CSoldier_Helmet_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_Kaiserpmc_HG")					pUnit = CSoldier_Kaiserpmc_HG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_RedWinterYoheki_SG")				pUnit = CSoldier_RedWinterYoheki_SG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_RedWinterYoheki_SR")				pUnit = CSoldier_RedWinterYoheki_SR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_RWY_AR")							pUnit = CSoldier_RWY_AR::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_YohekiPink_MG")					pUnit = CSoldier_YohekiPink_MG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Soldier_YohekiPink_SMG")					pUnit = CSoldier_YohekiPink_SMG::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);

	else if (wstrName == L"Goliath_Decagram_Vulcan")				pUnit = CGoliath_Decagram_Vulcan::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster); 

	else if (wstrName == L"Shiro")									pUnit = CShiro::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster); 
	else if (wstrName == L"Kuro")									pUnit = CKuro::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Hieronymus")								pUnit = CHieronymus::Create(CGraphicDev::GetInstance()-> GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Binah")									pUnit = CBinah::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
	else if (wstrName == L"Hod")									pUnit = CHod::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);

	//캐릭터
	else if (wstrName == L"Neru")
	{
		pUnit = CChar_Neru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Maki")
	{
		pUnit = CChar_Maki::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Aru")
	{
		pUnit = CChar_Aru::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Mutsuki")
	{
		pUnit = CChar_Mutsuki::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Serika")
	{
		pUnit = CChar_Serika::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Shun")
	{
		pUnit = CChar_Shun::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Akane")
	{
		pUnit = CChar_Akane::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Hina")
	{
		pUnit = CChar_Hina::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Hina")
	{
		pUnit = CChar_Momoi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Suzumi")
	{
		pUnit = CChar_Suzumi::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}
	else if (wstrName == L"Sumire")
	{
		pUnit = CChar_Sumire::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Monster);
		bMonster = FALSE;
	}

	if (!pUnit)
	{
		LMSG_BOX(L"WorldCreator 604번째 줄 - 유닛이 없습니다. else if(wstrNam == L'캐릭터 이름') 추가하세요 ");
		return nullptr;
	}

	if (bMonster)
	{
		static_cast<CMonster*>(pUnit)->Set_MonsterInfo(tMonsterInfo);
	}
	else
	{
		static_cast<CCharacter*>(pUnit)->Set_TargetLayerTag(E_LAYERTAG::Character);
		static_cast<CCharacter*>(pUnit)->Set_TargetObjTag(E_OBJTAG::OBJ_Player);
		static_cast<CCharacter*>(pUnit)->m_pHealthBar->Set_ImageColor(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		pUnit->Reset();
		_int iLevel = m_qUserUnitLevel.front();
		m_qUserUnitLevel.pop();
		static_cast<CCharacter*>(pUnit)->LevelUpByLevel(iLevel);
	}

	if (wstrName != L"Binah")
	{
		pUnit->Set_Pos(vPos);
		pUnit->Set_Rotation(vRot);
	}

	return static_cast<CUnit*>(pUnit);
}

