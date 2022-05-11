#pragma once

#ifndef __XL_Parsing_H__
#define __XL_Parsing_H__

#include "Engine_Define.h"
#include "libxl.h"

BEGIN(Engine)

class ENGINE_DLL CXL_ParsingMgr : public CBase
{
	DECLARE_SINGLETON(CXL_ParsingMgr)

private:
	CXL_ParsingMgr();
	virtual ~CXL_ParsingMgr();

public:
	virtual	void			Free() override;

public:
	HRESULT					Ready_Manager(const wstring& wstrFileName, _int* iLoadingGage);
	HRESULT					CharacterData_Parsing(const wstring& wstrFileName, _int* iLoadingGage);
	HRESULT					DialogData_Parsing(const wstring& wstrFileName, const wstring& wstrEventName, const _uint& iSheet, _int* iLoadingGage); 
	HRESULT					DialogCharInfo_Parsing(const wstring& wstrFileName, const wstring& wstrEventName, const _uint& iSheet, _int* iLoadingGage);

	_uint					Translation_StringToEnum(const wstring& wstrName, E_Translation_Type eType = E_Translation_Type::Translation_CharInfo);
	wstring					Translation_EnumToString(const _uint& iEnumValue, const E_CHAR_ENUMTYPE& eEnumType);
	wstring					Translation_CharName(const E_CHAR_NAME& eName); 

private:
	_uint					m_iStartRow = 2;
	_uint					m_iParsing_AtOnce = 4;

	_uint					m_iStartRow_Dialog = 1;
	_uint					m_iAtOnce_Dialog = 15;

	_int					m_iCharEventStartIndex = -1; 
};

END
#endif // __XL_Parsing_H__
