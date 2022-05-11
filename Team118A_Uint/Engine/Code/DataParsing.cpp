#include "DataParsing.h"



CDataParsing::CDataParsing()
{
}


CDataParsing::~CDataParsing()
{
}

//	Bool값 데이터 저장
void CDataParsing::Data_WriteBoolean(HANDLE _hFile, const bool & _boolean, DWORD & dyByte)
{
	WriteFile(_hFile, &_boolean, sizeof(bool), &dyByte, nullptr);
}

//	Wstring값 데이터 저장
void CDataParsing::Data_WriteWString(HANDLE _hFile, const wstring & _wstrValue, DWORD & dwByte)
{
	int iSize = (int(_wstrValue.length()) + 1) * sizeof(TCHAR);

	WriteFile(_hFile, &iSize, sizeof(int), &dwByte, nullptr);
	WriteFile(_hFile, _wstrValue.c_str(), iSize, &dwByte, nullptr);
}

//	Int값 데이터 저장
void CDataParsing::Data_WriteInt(HANDLE _hFile, const int & _iValue, DWORD & dwByte)
{
	WriteFile(_hFile, &_iValue, sizeof(int), &dwByte, nullptr);
}

//	Uint값 데이터 저장
void CDataParsing::Data_WriteUInt(HANDLE _hFile, const unsigned int & _iValue, DWORD & dwByte)
{
	WriteFile(_hFile, &_iValue, sizeof(unsigned int), &dwByte, nullptr);
}

//	Float값 데이터 저장
void CDataParsing::Data_WriteFloat(HANDLE _hFile, const float & _fValue, DWORD & dwByte)
{
	WriteFile(_hFile, &_fValue, sizeof(float), &dwByte, nullptr);
}

//	Vec3값 데이터 저장
void CDataParsing::Data_WriteVec3(HANDLE _hFile, const D3DXVECTOR3 & _vValue, DWORD & dwByte)
{
	WriteFile(_hFile, &_vValue, sizeof(D3DXVECTOR3), &dwByte, nullptr);
}

//	Bool값 데이터 로드
void CDataParsing::Data_ReadBoolean(HANDLE _hFile, bool & _boolean, DWORD & dwByte)
{
	ReadFile(_hFile, &_boolean, sizeof(bool), &dwByte, nullptr);
}

//	Wstring값 데이터 로드
void CDataParsing::Data_ReadWString(HANDLE _hFile, wstring & _wstrValue, DWORD & dwByte)
{
	int iSize;
	wchar_t wstr[MAX_PATH];

	ReadFile(_hFile, &iSize, sizeof(int), &dwByte, nullptr);
	ReadFile(_hFile, wstr, iSize, &dwByte, nullptr);
	_wstrValue = wstr;
}

//	Int값 데이터 로드
void CDataParsing::Data_ReadInt(HANDLE _hFile, int & _iValue, DWORD & dwByte)
{
	ReadFile(_hFile, &_iValue, sizeof(int), &dwByte, nullptr);
}

//	Uint값 데이터 로드
void CDataParsing::Data_ReadUInt(HANDLE _hFile, unsigned int & _iValue, DWORD & dwByte)
{
	ReadFile(_hFile, &_iValue, sizeof(unsigned int), &dwByte, nullptr);
}

//	Float값 데이터 로드
void CDataParsing::Data_ReadFloat(HANDLE _hFile, float & _fValue, DWORD & dwByte)
{
	ReadFile(_hFile, &_fValue, sizeof(float), &dwByte, nullptr);
}

//	Vec3값 데이터 로드
void CDataParsing::Data_ReadVec3(HANDLE _hFile, D3DXVECTOR3 & _vValue, DWORD & dwByte)
{
	ReadFile(_hFile, &_vValue, sizeof(D3DXVECTOR3), &dwByte, nullptr);
}