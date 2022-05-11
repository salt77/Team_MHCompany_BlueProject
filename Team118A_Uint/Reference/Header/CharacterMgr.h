#pragma once

#ifndef __CharacterMgr_H__
#define __CharacterMgr_H__

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCharacterMgr : public CBase
{
	DECLARE_SINGLETON(CCharacterMgr)

private:
	CCharacterMgr();
	virtual ~CCharacterMgr();

public:
	virtual void				Free() override;

public:
	HRESULT						Ready_Manager();

public:
	T_CharInfo&					Get_CharInfo(E_CHAR_NAME eName); 
	const void*					Get_CharInfo_Detail(E_CHAR_NAME eName, E_CHAR_STRUCTALL eEnumType);
	vector<T_CharInfo>&			Get_vecCharInfo() { return m_vecCharInfo; }
	const vector<T_CharInfo>&	Get_vecCharSort(E_CHAR_SORT eSortType);
	vector<T_CharInfo>			Get_vecCharInfo_Random();

	void						Set_Collect(E_CHAR_NAME eName);
	void						Set_CharInfo(E_CHAR_NAME eName, E_CHAR_STRUCTALL eEnumType, void* pValue);

public:
	void						Emplace_CharInfo(T_CharInfo tCharInfo) { m_vecCharInfo.emplace_back(tCharInfo); }
	wstring						Translation_EnumToString(const _uint& iEnumValue, const E_CHAR_ENUMTYPE& eEnumType);

private:
	void						Character_LevelUp(E_CHAR_NAME eName);
	static _bool				Compare_SortType(const T_CharInfo& tInfoBegin, const T_CharInfo& tInfoEnd);

private:
	static E_CHAR_SORT			m_eSortType;

	// 디폴트 = 영어 이름 기준 정렬
	vector<T_CharInfo>			m_vecCharInfo;
};

END

#endif // __CharacterMgr_H__