#include "stdafx.h"
#include "PlayFab_Mgr.h"
#include "DataManager.h"
#include "Wall512.h"
#include "GameManager.h"
#include "UI_Manager.h"
#include <iostream>

USING(Engine)

IMPLEMENT_SINGLETON(CPlayFab)

// 주의 사항 
// UserName		: 한글 x
// Passworld	: 6 ~ 100자
// DisplayName	: 3 ~ 25자

CPlayFab::CPlayFab()
{
}


CPlayFab::~CPlayFab()
{
	Free();
}

void CPlayFab::Free()
{
}

void CPlayFab::Setting_PlayFab()
{
	//PlayFabSettings::threadedCallbacks = TRUE;
	//PlayFabPluginManager::SetPlugin((std::shared_ptr < IPlayFabPlugin >) nullptr, PlayFabPluginContract::PlayFab_Transport);
	CGameManager::GetInstance()->m_fpPlayFab_Logout = bind(&CPlayFab::Try_LogOut, this);

	CUI_Manager::GetInstance()->m_pStrID					= &m_strUserName;
	CUI_Manager::GetInstance()->m_pStrPassword				= &m_strPassword;
	CUI_Manager::GetInstance()->m_fpPlayFab_Login			= bind(&CPlayFab::Login, this);
	CUI_Manager::GetInstance()->m_fpPlayFab_Register		= bind(&CPlayFab::Register, this);
	CUI_Manager::GetInstance()->m_fpPlayFab_UpdateRankInfo	= bind(&CPlayFab::Get_Ranking, this);
	CUI_Manager::GetInstance()->m_fpPlayFab_AddFriend		= bind(&CPlayFab::Add_Friend, this);
}

void CPlayFab::Login()
{
	Try_LoginWithPlayFab();
}

void CPlayFab::Register()
{
	Try_Register();
}

void CPlayFab::Try_LoginWithPlayFab()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
	PlayFabSettings::staticSettings->titleId = PlayFab_TitleID;

	LoginWithPlayFabRequest request;
	request.TitleId = PlayFab_TitleID;
	if (m_bFirstRegister)
	{
		request.Username = m_strAutoRegister_UserName;
		request.Password = m_strAutoRegister_Password;
	}
	else
	{
		request.Username = m_strUserName;
		request.Password = m_strPassword;
		m_strAutoRegister_UserName = m_strUserName;
		m_strAutoRegister_Password = m_strPassword;
	}

	cout << "로그인 요청" << endl;
	PlayFabClientAPI::LoginWithPlayFab(request,
		[&](const LoginResult& result, void* customData) -> void
	{
		cout << "로그인 성공" << endl;
		m_bFinished = TRUE;
		m_strMyID = result.PlayFabId;
		if (FALSE == m_bFirstRegister)
		{
			Load_UserInfo();
		}
		Get_AccountInfo();
		Get_Ranking();
		Get_FreindList();
		while (PlayFabClientAPI::Update() != 0)
			Sleep(1);

		CUI_Manager::GetInstance()->Register_Set_Announce(6.f, L"로그인 성공 (정보 불러오기 중)\n" + CUtility::String2Wstring(m_tAccountInfo.AccountInfo->Username) + L"님 접속을 환영합니다");

	},
		[&](const PlayFabError& error, void* customData) -> void {	CUtility::Print_Wstring_DMode(L"로그인 실패 -> 자동 회원가입.\n");		Try_AutoRegister(); }
	);


}

void CPlayFab::Try_AutoRegister()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
	RegisterPlayFabUserRequest request;
	request.TitleId = PlayFab_TitleID;
	request.Username = m_strAutoRegister_UserName;
	request.Password = m_strAutoRegister_Password;

	request.DisplayName = m_strAutoRegister_UserName;
	request.RequireBothUsernameAndEmail = FALSE;

	m_bFirstRegister = TRUE;
	PlayFabClientAPI::RegisterPlayFabUser(request,
		[&](const RegisterPlayFabUserResult& result, void* customData)	-> void
	{
		cout << "자동 회원가입 성공" << endl;
		m_strMyID = result.PlayFabId;
		Try_LoginWithPlayFab();
		Setting_NewUserInfo();
		m_bFirstRegister = FALSE;
	},
		[&](const PlayFabError& error, void* customData)				-> void {	cout << "자동 회원가입 실패" << endl;		Print_Error(error);		}
	);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
}

void CPlayFab::Try_Register()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
	RegisterPlayFabUserRequest request;
	request.TitleId = PlayFab_TitleID;
	request.Username = m_strUserName;
	request.Password = m_strPassword;
	request.DisplayName = m_strUserName;
	request.RequireBothUsernameAndEmail = FALSE;

	m_bFirstRegister = TRUE;
	PlayFabClientAPI::RegisterPlayFabUser(request,
		[&](const RegisterPlayFabUserResult& result, void* customData)	-> void
	{
		cout << "회원가입 성공" << endl;
		Try_LoginWithPlayFab();
		CUI_Manager::GetInstance()->Register_Set_Announce(2.f, L"회원가입 성공");
	},
		[&](const PlayFabError& error, void* customData)				-> void {	cout << "회원가입 실패" << endl;		Print_Error(error);		}
	);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
}

void CPlayFab::Try_LogOut()
{
	if (FALSE == PlayFabClientAPI::IsClientLoggedIn())
	{
		return;
	}

	CPlayFab::GetInstance()->Save_UserInfo();

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	Sleep(300);
}


void CPlayFab::Save_UserInfo()
{
	string strCharBool;
	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		strCharBool.append(to_string((_int)CDataManager::GetInstance()->Get_UserInfo().bCharacter[i]));
		strCharBool.append(",");
	}
	Set_Stat_String("CharInfo_Have", strCharBool);

	string strCharLevel;
	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		strCharLevel.append(to_string((_int)CCharacterMgr::GetInstance()->Get_CharInfo((E_CHAR_NAME)i).iLevel));
		strCharLevel.append(",");
	}
	Set_Stat_String("CharInfo_Level", strCharLevel);


	string strUserInfo;
	CDataManager::GetInstance()->Set_Credit(500000);
	CDataManager::GetInstance()->Set_AP(CDataManager::GetInstance()->Get_UserInfo().iLevel * 10);
	CDataManager::GetInstance()->Set_BluePyroxene(50000);
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iLevel) + ",");
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iUserEXP) + ",");
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iAP) + ",");
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iCredit) + ",");
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iBluePyroxene) + ",");
	strUserInfo.append(to_string(CDataManager::GetInstance()->Get_UserInfo().iEligma) + ",");
	strUserInfo.append(to_string((_int)(CDataManager::GetInstance()->Get_UserInfo().fBGM_Ratio * 100)) + ",");
	strUserInfo.append(to_string((_int)(CDataManager::GetInstance()->Get_UserInfo().fFX_Ratio * 100)) + ",");
	strUserInfo.append(to_string((_int)(CDataManager::GetInstance()->Get_UserInfo().fVoice_Ratio * 100)) + ",");
	strUserInfo.append(to_string((_int)CDataManager::GetInstance()->Get_UserInfo().eLobby_Char) + ",");
	wstring wstrName = wstring(CDataManager::GetInstance()->Get_UserInfo().wszNickName);
	string Name = CUtility::Wstring2String(wstrName);
	strUserInfo.append(Name + "");

	Set_Stat_String("Account_Info", strUserInfo);


	for (_int i = 0; i < (_int)E_SQUAD::SQUAD_END; ++i)
	{
		map<_int, vector<CCharacter*>>* pSquad = CGameManager::GetInstance()->Get_Squad((E_SQUAD)i);
		vector<CCharacter*> vecStriker = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_STRIKER];
		vector<CCharacter*> vecSpecial = (*pSquad)[(_int)E_CHAR_COMBATTYPE::COMBATTYPE_SPECIAL];

		string strSquadData = "";
		for (_int j = 0; j < 6; j++)
		{
			CCharacter* pChar = nullptr;
			pChar = (j < 4) ? vecStriker[j] : vecSpecial[j - 4];

			if (nullptr == pChar)
			{
				strSquadData.append("N_Empty");
				strSquadData.append(",");
				strSquadData.append("L_Empty");
				strSquadData.append(",");
				strSquadData.append("S_Empty");
			}
			else
			{
				strSquadData.append(to_string((_int)pChar->Get_CharInfo().eName[(_int)E_CHAR_LANG::LANG_ENG]));
				strSquadData.append(",");
				strSquadData.append(to_string((_int)pChar->Get_CharInfo().iLevel));
				strSquadData.append(",");
				strSquadData.append(to_string((_int)pChar->Get_CharInfo().eStar));
			}

			if (j < 5)
			{
				strSquadData.append(",");
			}
		}

		string strSquadName = "Squad";
		strSquadName.append(to_string(i));
		Set_Stat_String(strSquadName, strSquadData);
	}

	if (m_bFirstRegister)
	{
		Set_Stat_Leaderboard("BossScore", CWall512::GetInstance()->GetValue(0, 10000000));
		Set_Stat_Leaderboard("Boss_Hieronymus", CWall512::GetInstance()->GetValue(0, 100000));
	}

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
}

void CPlayFab::Load_UserInfo()
{
	T_USERINFO tUserInfo;

	string strCharBool = Get_Stat_String(m_strMyID, "CharInfo_Have");
	vector<string> vecCharBool = CUtility::Split(strCharBool, ",");
	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		if (vecCharBool.size() <= i)
			break;

		_bool bHave = vecCharBool[i] == "1" ? TRUE : FALSE;
		tUserInfo.bCharacter[i] = bHave;
	}

	string strCharLevel = Get_Stat_String(m_strMyID, "CharInfo_Level");
	vector<string> vecCharLevel = CUtility::Split(strCharLevel, ",");
	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		if (vecCharLevel.size() <= i)
			break;
	
		_int iLevel = (atoi)(vecCharLevel[i].c_str());
		CCharacterMgr::GetInstance()->Get_CharInfo(E_CHAR_NAME(i)).iLevel = iLevel;
	}


	string strUserInfo = Get_Stat_String(m_strMyID, "Account_Info");
	vector<string> vecUserInfo = CUtility::Split(strUserInfo, ",");

	if (9 <= vecUserInfo.size())
	{
		tUserInfo.iLevel			= atoi(vecUserInfo[0].c_str());
		tUserInfo.iUserEXP			= atoi(vecUserInfo[1].c_str());
		tUserInfo.iAP				= atoi(vecUserInfo[2].c_str());
		tUserInfo.iCredit			= atoi(vecUserInfo[3].c_str());
		tUserInfo.iBluePyroxene		= atoi(vecUserInfo[4].c_str());
		tUserInfo.iEligma			= atoi(vecUserInfo[5].c_str());
		tUserInfo.fBGM_Ratio		= (_float)atof(vecUserInfo[6].c_str())		* 0.01f;
		tUserInfo.fFX_Ratio			= (_float)atof(vecUserInfo[7].c_str())		* 0.01f;
		tUserInfo.fVoice_Ratio		= (_float)atof(vecUserInfo[8].c_str())		* 0.01f;
		tUserInfo.eLobby_Char		= (E_CHAR_NAME)atoi(vecUserInfo[9].c_str());

		wstring Nickname			= CUtility::String2Wstring(vecUserInfo[10]);
		lstrcpy(tUserInfo.wszNickName, Nickname.c_str());
	}


	CGameManager::GetInstance()->Release_Formation();
	CGameManager::GetInstance()->Ready_Formtion();
	for (_int i = 0; i < (_int)E_SQUAD::SQUAD_END; ++i)
	{
		string strSquadName = "Squad";
		strSquadName.append(to_string(i));
		string strSquadData = Get_Stat_String(m_strMyID, strSquadName).c_str();

		while (PlayFabClientAPI::Update() != 0)
			Sleep(1);

		vector<string> vecString = CUtility::Split(strSquadData, ",");


		for (_int j = 0; j < (_int)vecString.size(); j += 3)
		{
			if ("N_Empty" != vecString[j])
			{
				CGameManager::GetInstance()->Add_Formation((E_CHAR_NAME)atoi(vecString[j].c_str()), (E_SQUAD)i);
			}
		}
	}

	CDataManager::GetInstance()->Set_UserInfo(tUserInfo);



	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	if (m_bAccountInfo)
	{
		wstring wstrUserName = CUtility::String2Wstring(m_tAccountInfo.AccountInfo->Username);
		CDataManager::GetInstance()->Set_NickName(wstrUserName);
	}

	CUtility::Print_Wstring_DMode(L"tUserInfo.iLevel - " + to_wstring(tUserInfo.iLevel));
	CUtility::Print_Wstring_DMode(L"tUserInfo.iCredit - " + to_wstring(tUserInfo.iCredit));
	CUtility::Print_Wstring_DMode(L"tUserInfo.iBluePyroxene - " + to_wstring(tUserInfo.iBluePyroxene));
	CUtility::Print_Wstring_DMode(L"tUserInfo.iEligma - " + to_wstring(tUserInfo.iEligma));
	CUtility::Print_Wstring_DMode(L"Get_NickName() - " + wstring(CDataManager::GetInstance()->Get_NickName()));

	CGameManager::GetInstance()->Login_Account();
	CUI_Manager::GetInstance()->Create_Ui_Lobby();
}

void CPlayFab::Setting_NewUserInfo()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	CDataManager::GetInstance()->Set_BluePyroxene(0);
	CDataManager::GetInstance()->Set_AP(100);
	CDataManager::GetInstance()->Set_EXP(0);
	CDataManager::GetInstance()->Set_Eligma(0);
	CDataManager::GetInstance()->Set_Level(CWall512::GetInstance()->GetValue(1, 100));
	CDataManager::GetInstance()->Set_Credit(100000);
	CDataManager::GetInstance()->Set_LobbyChar((E_CHAR_NAME)CWall512::GetInstance()->GetValue(0, 5));
	//CDataManager::GetInstance()->Set_NickName(CUtility::String2Wstring(m_strUserName));


	CGameManager::GetInstance()->Release_Formation();
	CGameManager::GetInstance()->Ready_Formtion();
	CGameManager::GetInstance()->Add_Formation(E_CHAR_NAME::Name_Neru, E_SQUAD::SQUAD_1);
	CGameManager::GetInstance()->Add_Formation(E_CHAR_NAME::Name_Aru, E_SQUAD::SQUAD_1);

	for (_int i = 0; i < (_int)E_CHAR_NAME::Name_End; ++i)
	{
		CCharacterMgr::GetInstance()->Get_CharInfo(E_CHAR_NAME(i)).iLevel = CWall512::GetInstance()->GetValue(1, 30);
	}

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	if (m_bAccountInfo)
	{
		wstring wstrUserName = CUtility::String2Wstring(m_tAccountInfo.AccountInfo->Username);
		CDataManager::GetInstance()->Set_NickName(wstrUserName);
	}

	CGameManager::GetInstance()->Login_Account();

	Save_UserInfo();
	CUI_Manager::GetInstance()->Create_Ui_Lobby();

}

void CPlayFab::Get_Ranking()
{
	if (FALSE == PlayFabClientAPI::IsClientLoggedIn())
	{
		CUI_Manager::GetInstance()->m_pRankingPanel->Set_Active(FALSE);
		return;
	}


	GetLeaderboardRequest			request;
	PlayerProfileViewConstraints	profile;
	request.StartPosition			= 0;
	request.StatisticName			= m_strLeaderboard[(_int)E_Leaderboard::Hiero];
	request.MaxResultsCount			= 50;
	profile.ShowDisplayName			= TRUE;
	profile.ShowStatistics			= TRUE;
	profile.ShowLastLogin			= TRUE;
	request.ProfileConstraints		= profile;

	PlayFabClientAPI::GetLeaderboard(request,
		[&](const GetLeaderboardResult& result, void* customData) -> void
	{
		list<PlayFab::ClientModels::PlayerLeaderboardEntry> curBoard = result.Leaderboard;

		vector<T_PlayfabInfo> vecRank;

		_int iPlayerPoint	= 0;
		_int iPlayerRank	= 0;
		for (auto& iter : curBoard)
		{
			T_PlayfabInfo Info;
			//cout << "───────────" << endl;
			//cout << iter.Profile->Statistics.begin()->Name << " :\t" << iter.Profile->Statistics.begin()->Value << endl;
			//cout << "DisplayName :\t" << iter.DisplayName << endl;
			//cout << "PlayFabId :\t" << iter.PlayFabId << endl;
			//cout << "Position :\t" << iter.Position << endl;
			//cout << "StatValue :\t" << iter.StatValue << endl << endl;
			for (auto& statistic : iter.Profile->Statistics)
			{
				if ("BossScore" == statistic.Name)
				{
					Info.iBossScore[0] = statistic.Value;
				}
			}

			Info.tLastLogin = iter.Profile->LastLogin;
			Info.strPlayFabID = iter.PlayFabId;
			Info.wstrNickname = CUtility::String2Wstring(iter.DisplayName);
			Info.iRank = iter.Position + 1;

			map<string, string> mapUserData = Get_Stat_AllData(iter.PlayFabId);
			string UserInfo = mapUserData["Account_Info"];
			vector<string> vecUserInfo = CUtility::Split(UserInfo, ",");
			if (9 <= (_int)vecUserInfo.size())
			{
				Info.iLobbyChar = (atoi)(vecUserInfo[9].c_str());
				Info.iAccountLevel = (atoi)(vecUserInfo[0].c_str());
			}

			if (mapUserData.find("Squad0") != mapUserData.end())
			{
				vector<string> vecSquadData = CUtility::Split(mapUserData["Squad0"], ",");
				for (_int i = 0; i < (_int)vecSquadData.size(); i += 3)
				{
					if (vecSquadData[i] == "N_Empty")
					{
						Info.iSquadChar_Enum[i / 3]		= -1;
						Info.iSquadChar_Level[i / 3]	= -1;
						Info.iSquadChar_Star[i / 3]		= -1;
					}
					else
					{
						Info.iSquadChar_Enum[i / 3]		= (atoi)(vecSquadData[i].c_str());
						Info.iSquadChar_Level[i / 3]	= (atoi)(vecSquadData[i + 1].c_str());
						Info.iSquadChar_Star[i / 3]		= (atoi)(vecSquadData[i + 2].c_str());
					}
				}
			}
			else
			{
				for (_int i = 0; i < 6; ++i)
				{
					Info.iSquadChar_Enum[i]			= -1;
					Info.iSquadChar_Level[i]		= -1;
					Info.iSquadChar_Star[i]			= -1;
				}
			}

			while (PlayFabClientAPI::Update() != 0)
				Sleep(1);

			vecRank.push_back(Info);

			if (iter.PlayFabId == m_strMyID)
			{
				iPlayerPoint = Info.iBossScore[0];
				iPlayerRank = Info.iRank;
			}
		}

		while (PlayFabClientAPI::Update() != 0)
			Sleep(1);

		CUI_Manager::GetInstance()->Ranking_Set_Rank(vecRank, iPlayerPoint, iPlayerRank);

		while (PlayFabClientAPI::Update() != 0)
			Sleep(1);

		CUI_Manager::GetInstance()->Ranking_UI_Setting();
	},
		[&](const PlayFabError& error, void* customData)					-> void {	printf_s("리더보드 실패.\n"); Print_Error(error);			}
	);


}

void CPlayFab::Get_CurrentUserInfo()
{
	// 추후 제작
}

void CPlayFab::Add_Friend()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
	CUtility::Print_Wstring_DMode(L"Add_Friend 들어옴");
	AddFriendRequest request;

	request.FriendPlayFabId = CUI_Manager::GetInstance()->m_strFriendPlayFabId;

	PlayFabClientAPI::AddFriend(request,
		[&](const AddFriendResult& result, void* customData)		-> void
	{
		Get_FreindList();
	},
		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Add_Friend 실패.\n" << endl; Print_Error(error);	Get_FreindList();	}
	);
}


void CPlayFab::Get_FreindList()
{
	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
	CUtility::Print_Wstring_DMode(L"Get_FreindList 들어옴");

	GetFriendsListRequest request;

	PlayFabClientAPI::GetFriendsList(request, 
		[&](const GetFriendsListResult& result, void* customData)		-> void
	{

		vector<T_PlayfabInfo> vecInfo;

		for (auto& iter : result.Friends)
		{
			T_PlayfabInfo Info;

			map<string, string> mapUserData = Get_Stat_AllData(iter.FriendPlayFabId);
			string UserInfo = mapUserData["Account_Info"];
			vector<string> vecUserInfo = CUtility::Split(UserInfo, ",");
			if (9 <= (_int)vecUserInfo.size())
			{
				Info.iLobbyChar = (atoi)(vecUserInfo[9].c_str());
				Info.iAccountLevel = (atoi)(vecUserInfo[0].c_str());
			}

			//Info.tLastLogin = iter.Profile->LastLogin;
			Info.strPlayFabID = iter.FriendPlayFabId;
			Info.wstrNickname = CUtility::String2Wstring(iter.Username);

			if (mapUserData.find("Squad0") != mapUserData.end())
			{
				vector<string> vecSquadData = CUtility::Split(mapUserData["Squad0"], ",");
				for (_int i = 0; i < (_int)vecSquadData.size(); i += 3)
				{
					if (vecSquadData[i] == "N_Empty")
					{
						Info.iSquadChar_Enum[i / 3] = -1;
						Info.iSquadChar_Level[i / 3] = -1;
						Info.iSquadChar_Star[i / 3] = -1;
					}
					else
					{
						Info.iSquadChar_Enum[i / 3] = (atoi)(vecSquadData[i].c_str());
						Info.iSquadChar_Level[i / 3] = (atoi)(vecSquadData[i + 1].c_str());
						Info.iSquadChar_Star[i / 3] = (atoi)(vecSquadData[i + 2].c_str());
					}
				}
			}
			vecInfo.push_back(Info);
		}

		while (PlayFabClientAPI::Update() != 0)
			Sleep(1);
		
		CUI_Manager::GetInstance()->FriendList_Set(vecInfo);
		CUtility::Print_Wstring_DMode(L"Get_FreindList Success");
	},
		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Get_FreindList 실패.\n" << endl; Print_Error(error);		}
	);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);
}

//void CPlayFab::Create_Group()
//{
//	while (PlayFabClientAPI::Update() != 0)
//		Sleep(1);
//	GroupsModels::CreateGroupRequest request;
//	request.GroupName = m_strAutoRegister_UserName;
//
//	PlayFabGroupsAPI::CreateGroup(request,
//		[&](const GroupsModels::CreateGroupResponse& result, void* customData) -> void
//	{
//		cout << "Create_Group 성공.\n" << endl;
//	},
//		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Create_Group 실패.\n" << endl; Print_Error(error);		}
//	);
//
//}
//
//void CPlayFab::Delete_Group()
//{
//	while (PlayFabClientAPI::Update() != 0)
//		Sleep(1);
//	GroupsModels::DeleteGroupRequest request;
//	request.Group;
//
//	PlayFabGroupsAPI::DeleteGroup(request,
//		[&](const GroupsModels::EmptyResponse& result, void* customData) -> void
//	{
//		cout << "Create_Group 성공.\n" << endl;
//	},
//		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Create_Group 실패.\n" << endl; Print_Error(error);		}
//	);
//}
//
//void CPlayFab::Get_GroupIDList()
//{
//	while (PlayFabClientAPI::Update() != 0)
//		Sleep(1);
//	GroupsModels::ListMembershipRequest request;
//
//	PlayFabGroupsAPI::ListMembership(request,
//		[&](const GroupsModels::ListMembershipResponse& result, void* customData) -> void
//	{
//		cout << "Get_GroupList 성공.\n" << endl;
//		for (auto& iter : result.Groups)
//		{
//			cout << "GroupName : " << iter.GroupName << endl;
//
//			GroupsModels::ListGroupMembersRequest requestMember;
//			requestMember.Group = iter.Group;
//			PlayFabGroupsAPI::ListGroupMembers(requestMember,
//				[&](const GroupsModels::ListGroupMembersResponse& resultMember, void* customData2) -> void
//			{
//				for (auto& iterRole : resultMember.Members)
//				{
//					cout << "RoleID : " << iterRole.RoleId << endl;
//					cout << "RoleName : " << iterRole.RoleName << endl;
//					for (auto& iterMember : iterRole.Members)
//					{
//						cout << "ID : " << iterMember.Key->Id << endl;
//					}
//				}
//			},
//				[&](const PlayFabError& error2, void* customData2)					-> void {	cout << "ListGroupMembersRequest 실패.\n" << endl; Print_Error(error2);		}
//			);
//		}
//	},
//		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Get_GroupList 실패.\n" << endl; Print_Error(error);		}
//	);
//}
//
//EntityKey EntityKeyMaker(string entityId)
//{
//	EntityKey key;
//	key.Id = entityId;
//
//	return key;
//}
//
//void CPlayFab::Get_GroupMemberList()
//{
//
//}
//
//void CPlayFab::ApplyToGroup()
//{
//	GroupsModels::ApplyToGroupRequest request;
//
//	request.Entity;
//
//}
//
//void CPlayFab::KickGroupMember()
//{
//	GroupsModels::RemoveMembersRequest request;
//	request.Group;
//
//	PlayFabGroupsAPI::RemoveMembers(request,
//		[&](const GroupsModels::EmptyResponse& result, void* customData) -> void
//	{
//
//	},
//		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Get_GroupList 실패.\n" << endl; Print_Error(error);		}
//	);
//}

void CPlayFab::Set_Stat_String(string sKey, string sValue)
{
	UpdateUserDataRequest request;
	request.Data.emplace(sKey, sValue);
	request.Permission = UserDataPermission::UserDataPermissionPublic;

	PlayFabClientAPI::UpdateUserData(request,
		[&](const UpdateUserDataResult& result, void* customData)			-> void
	{
		cout << "Set_Stat_String 성공.\n" << endl;
	},
		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Set_Stat_String 실패.\n" << endl; Print_Error(error);		}
	);
}

string CPlayFab::Get_Stat_String(string sPlayFabId, string sKey)
{
	string sData = StrNone;
	GetUserDataRequest request;
	request.PlayFabId = sPlayFabId;
	PlayFabClientAPI::GetUserData(request,
		[&](const GetUserDataResult& result, void* customData)	-> void
	{
		printf_s("Get_Stat_String 성공.\n");
		if (result.Data.find(sKey) != result.Data.end())
		{
			sData = result.Data.find(sKey)->second.Value;
		}
	},
		[&](const PlayFabError& error, void* customData)					-> void {	printf_s("Get_Stat_String 실패.\n"); Print_Error(error);			}
	);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	return sData;
}

map<string, string> CPlayFab::Get_Stat_AllData(string sPlayFabId)
{
	m_mapData.clear();
	GetUserDataRequest request;
	request.PlayFabId = sPlayFabId;
	PlayFabClientAPI::GetUserData(request,
		[&](const GetUserDataResult& result, void* customData)	-> void
	{
		printf_s("Get_Stat_AllData 성공.\n");
		for (auto iter : result.Data)
		{
			m_mapData.emplace(iter.first, iter.second.Value);
		}
	},
		[&](const PlayFabError& error, void* customData)					-> void {	printf_s("Get_Stat_AllData 실패.\n"); Print_Error(error);			}
	);

	while (PlayFabClientAPI::Update() != 0)
		Sleep(1);

	return m_mapData;
}

void CPlayFab::Set_Stat_Leaderboard(string sName, int iValue)
{
	UpdatePlayerStatisticsRequest StatisticsRequest;
	StatisticUpdate up;
	up.StatisticName = sName;
	up.Value = iValue;
	StatisticsRequest.Statistics.push_back(up);

	PlayFabClientAPI::UpdatePlayerStatistics(
		StatisticsRequest,
		[&](const UpdatePlayerStatisticsResult& result, void* customData) -> void {	},
		[&](const PlayFabError& error, void* customData)				  -> void { cout << "Set_Stat 실패" << endl;		Print_Error(error);	}
	);
}

int CPlayFab::Get_Stat_Leaderboard(string sName)
{
	int iGetInt = 0;
	GetPlayerStatisticsRequest request;
	PlayFabClientAPI::GetPlayerStatistics(
		request,
		[&](const GetPlayerStatisticsResult& result, void* customData)		-> void
	{
		for (auto iter : result.Statistics)
		{
			if (sName == iter.StatisticName)
			{
				iGetInt = iter.Value;
				m_iGetStat = iter.Value;
			}
		}
	},
		[&](const PlayFabError& error, void* customData)					-> void {	/*cout << "Get_Stat 실패" << endl;*/ Print_Error(error);			}
	);

	return iGetInt;
}

void CPlayFab::Get_Leaderboard(E_Leaderboard eLeaderboard)
{
	GetLeaderboardRequest request;
	request.StartPosition = 0;
	request.StatisticName = m_strLeaderboard[(int)eLeaderboard];
	request.MaxResultsCount = 20;
	PlayerProfileViewConstraints profile;
	profile.ShowDisplayName = TRUE;
	profile.ShowStatistics = TRUE;
	request.ProfileConstraints = profile;

	PlayFabClientAPI::GetLeaderboard(request,
		[&](const GetLeaderboardResult& result, void* customData) -> void
	{
		printf_s("리더보드 성공.\n");
		list<PlayFab::ClientModels::PlayerLeaderboardEntry> curBoard = result.Leaderboard;
		for (auto& iter : curBoard)
		{
			/*
			cout << "───────────" << endl;
			cout << iter.Profile->Statistics.begin()->Name << " :\t" << iter.Profile->Statistics.begin()->Value << endl;
			cout << "DisplayName :\t" << iter.DisplayName << endl;
			cout << "PlayFabId :\t" << iter.PlayFabId << endl;
			cout << "Position :\t" << iter.Position << endl;
			cout << "StatValue :\t" << iter.StatValue << endl << endl;
			*/
		}
	},
		[&](const PlayFabError& error, void* customData)					-> void {	printf_s("리더보드 실패.\n"); Print_Error(error);			}
	);
}

void CPlayFab::Get_AccountInfo()
{
	m_bAccountInfo = FALSE;

	GetAccountInfoRequest request;
	request.PlayFabId = m_strMyID;

	PlayFabClientAPI::GetAccountInfo(request,
		[&](const GetAccountInfoResult& result, void* customData) -> void
	{
		m_tAccountInfo = result;
		m_bAccountInfo = TRUE;
	},
		[&](const PlayFabError& error, void* customData)					-> void {	cout << "Get_AccountInfo 실패.\n" << endl; Print_Error(error);			}
	);
}

void CPlayFab::Print_Error(const PlayFabError & error)
{
	cout << "Begin-----------------" << endl;
	cout << "에러 코드 : " + (int)error.ErrorCode << endl;
	cout << "에러 이름 : " + error.ErrorName << endl;
	cout << "에러 메세지 : " + error.ErrorMessage << endl;
	cout << "End-----------------" << endl;
}
