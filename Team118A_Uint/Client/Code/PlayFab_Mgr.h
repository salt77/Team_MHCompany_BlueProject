#pragma once
#ifndef __PlayFab_Mgr_H__
#define __PlayFab_Mgr_H__

#include "Base.h"
#include "Engine_Define.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include "playfab/PlayFabError.h"
#include "playfab/PlayFabPluginManager.h"

using namespace PlayFab;
using namespace ClientModels;
using namespace std;

BEGIN(Engine)

class CPlayFab : public CBase
{
public:
	enum class E_Leaderboard { BossScore, Hiero };
	string m_strLeaderboard[2] = { "BossScore", "Boss_Hieronymus" };

	DECLARE_SINGLETON(CPlayFab)
private:
	CPlayFab();
	~CPlayFab();

public:
	virtual void			Free() override;
	void					Setting_PlayFab();

	// �ܺ�ȣ�� �α���, �α׾ƿ�
public:
	void					Login();
	void					Register();

	// �α���, ȸ������, �α׾ƿ�
public:
	void					Try_LoginWithPlayFab();
	void					Try_AutoRegister();
	void					Try_Register();
	void					Try_LogOut();

	// ���� ������ ����, �ε�
public:
	void					Save_UserInfo();
	void					Load_UserInfo();
	void					Setting_NewUserInfo();

	// ��ŷ
public:
	void					Get_Ranking();
	void					Get_CurrentUserInfo();

	// ģ���߰�
public:
	void					Add_Friend();
	void					Get_FreindList();

	// ��� �ý���
public:
	//void					Create_Group();
	//void					Delete_Group();
	//void					Get_GroupIDList();
	//void					Get_GroupMemberList();
	//void					ApplyToGroup();
	//void					KickGroupMember();



private:
	void					Set_Stat_String(string sKey, string sValue);
	string					Get_Stat_String(string sPlayFabId, string sKey);

	map<string, string>		Get_Stat_AllData(string sPlayFabId);

	void					Set_Stat_Leaderboard(string sName, int iValue);
	int						Get_Stat_Leaderboard(string sName);
	void					Get_Leaderboard(E_Leaderboard eLeaderboard);


	void					Get_AccountInfo();

private:
	void					Print_Error(const PlayFabError& error);


public:
	string					m_strMyID			= StrNone;
	string					m_strUserName		= "";
	string					m_strPassword		= "";
	string					m_strAutoRegister_UserName = "";
	string					m_strAutoRegister_Password = "";
	bool					m_bFinished			= FALSE;
	bool					m_bFirstRegister	= FALSE;
	bool					m_bAccountInfo		= FALSE;
	int						m_iGetStat			= 0;
	map<string, string>		m_mapData;
	GetAccountInfoResult	m_tAccountInfo;






	// Template�� �̿��ؼ� �ڵ� ����ȭ�ϱ� (�����ϴ� ��)
	template<typename ResType> using MyProcessApiCallback = void(*)(const ResType& result, void* userData);
};

END

#endif //__PlayFab_Mgr_H__