#pragma once
#ifndef __DATAPARSING_H__
#define __DATAPARSING_H__

#include "Engine_Define.h"
#include "Engine_Enum.h"
class CDataParsing
{
public:
	CDataParsing();
	~CDataParsing();

public:
	static void Data_WriteBoolean	(HANDLE _hFile, const bool& _boolean,			DWORD& dwByte);
	static void Data_WriteWString	(HANDLE _hFile, const wstring& _wstrValue,		DWORD& dwByte);
	static void Data_WriteInt		(HANDLE _hFile, const int& _iValue,				DWORD& dwByte);
	static void Data_WriteUInt		(HANDLE _hFile, const unsigned int& _iValue,	DWORD& dwByte);
	static void Data_WriteFloat		(HANDLE _hFile, const float& _fValue,			DWORD& dwByte);
	static void Data_WriteVec3		(HANDLE _hFile, const D3DXVECTOR3& _vValue,		DWORD& dwByte);

public:
	static void Data_ReadBoolean	(HANDLE _hFile, bool& _boolean,					DWORD& dwByte);
	static void Data_ReadWString	(HANDLE _hFile, wstring& _wstrValue,			DWORD& dwByte);
	static void Data_ReadInt		(HANDLE _hFile, int& _iValue,					DWORD& dwByte);
	static void Data_ReadUInt		(HANDLE _hFile, unsigned int& _iValue,			DWORD& dwByte);
	static void Data_ReadFloat		(HANDLE _hFile, float& _fValue,					DWORD& dwByte);
	static void Data_ReadVec3		(HANDLE _hFile, D3DXVECTOR3& _vValue,			DWORD& dwByte);



};

#endif // !__DATAPARSING_H__