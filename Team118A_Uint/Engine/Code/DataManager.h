#pragma once

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CDataManager : public CBase
{
	DECLARE_SINGLETON(CDataManager)
private:
	CDataManager();
	virtual ~CDataManager();

public:
	virtual void	Free() override;
public:
	void			Set_UserInfo(T_USERINFO& tUserInfo)		{ m_tUserInfo = tUserInfo;													}

	void			Get_CurrentDate(T_DATEINFO* pDateInfoOut);
	void			Get_ConvertDate(T_DATEINFO* pDateInfoOut, time_t tTime);
	_int			Get_DateDiffrence(T_DATEINFO tPastDate, T_DATEINFO tFutureDate);
	const T_USERINFO& Get_UserInfo()						{ return m_tUserInfo;														}
	const WCHAR*	Get_NickName()							{ return m_tUserInfo.wszNickName;											}
	const _uint&	Get_Level()								{ return m_tUserInfo.iLevel;												}
	const _uint&	Get_EXP()								{ return m_tUserInfo.iUserEXP;												}
	const _uint&	Get_Credit()							{ return m_tUserInfo.iCredit;												}
	const _uint&	Get_Eligma()							{ return m_tUserInfo.iEligma;												}
	const _uint&	Get_AP()								{ return m_tUserInfo.iAP;													}
	const _uint&	Get_BluePyroxene()						{ return m_tUserInfo.iBluePyroxene;											}
	const _int		Get_RangeByWeaponType(E_CHAR_WEAPONTYPE eWeaponType);
	const _int		Get_MaxAmmoByWeaponType(E_CHAR_WEAPONTYPE eWeaponType);

	void			Set_NickName(wstring wstrNickName)		{ lstrcpy(m_tUserInfo.wszNickName, wstrNickName.c_str());					}
	void			Set_Level(_uint iLevel)					{ m_tUserInfo.iLevel = iLevel;												}
	void			Set_EXP(_uint iEXP)						{ m_tUserInfo.iUserEXP = iEXP;												}
	void			Set_Credit(_uint iCredit)				{ m_tUserInfo.iCredit = iCredit;											}
	void			Set_Eligma(_uint iEligma)				{ m_tUserInfo.iEligma = iEligma;											}
	void			Set_AP(_uint iAP)						{ m_tUserInfo.iAP = iAP;													}
	void			Set_BluePyroxene(_uint iBluePyroxene)	{ m_tUserInfo.iBluePyroxene = iBluePyroxene;								}
	void			Set_LobbyChar(E_CHAR_NAME eName)		{ m_tUserInfo.eLobby_Char = eName;											}
	void			Set_HaveChar(_int iChar, _bool bState);

	void			Add_Credit(_int iCredit)				{ m_tUserInfo.iCredit			+= iCredit;									}
	void			Add_AP(_int iAP)						{ m_tUserInfo.iAP				+= iAP;										}
	void			Add_BluePyroxene(_int iBluePyroxene)	{ m_tUserInfo.iBluePyroxene		+= iBluePyroxene;							}
	void			Add_Eligma(_int iEligma)				{ m_tUserInfo.iEligma			+= iEligma;									}

public:
	void			Initialize_Item(vector<T_ItemInfo>* pVecItemInfo);
	void			Initialize_Equip(vector<T_EquipInfo>* pVecEquipInfo);
	void			Initialize_Furniture(vector<T_FurnitureInfo>* pVecFurnitureInfo);

private:
	T_USERINFO		m_tUserInfo;
};

END

#endif