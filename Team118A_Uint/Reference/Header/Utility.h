#pragma once
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Engine_Define.h"
#include "Base.h"
#include "MyMath.h"
#include "Calculator.h"

#include "DataParsing.h"

BEGIN(Engine)

class ENGINE_DLL CUtility : public CBase
{
private:
	CUtility();
	~CUtility();

	///////////				Math					///////////
public:
	static	int		Clamp_int(const int& iValue, int iLow, int iHigh);	//	Low, High 사이로 Value 보정
	static float Lerp_float(float fStart, float fEnd, float fRatio, bool bClamp = TRUE);			//	숫자 간의 선형 보간
	static float LerpEase_float(Engine::E_EASE eType, float x);
	static float Clamp_float(const float& fValue, float fLow, float fHigh);


	static D3DXVECTOR2		Lerp_vec2(D3DXVECTOR2 vStart, D3DXVECTOR2 vEnd, float fRatio, bool bClamp = TRUE);

public:
	static D3DXVECTOR3		Lerp_vec3(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fRatio, bool bClamp = TRUE);
	static float			Distance_vec3(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd);
	static float			GetAngleToTarget_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);
	static bool				CheckFront_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);
	static bool				CheckRight_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir, D3DXVECTOR3 vUp = VEC_UP);
	static D3DXVECTOR3		GetNormal_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);


	static D3DXVECTOR4		Lerp_vec4(D3DXVECTOR4 vStart, D3DXVECTOR4 vEnd, float fRatio, bool bClamp = TRUE);

public:
	static D3DXVECTOR3		Bezier(_float fTime, D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3, D3DXVECTOR3 P4);

	 ///////////			DataParsing				///////////
public:
	static void Data_WriteBoolean(HANDLE _hFile, const bool& _boolean, DWORD& dyByte);
	static void Data_WriteWString(HANDLE _hFile, const wstring& _wstrValue, DWORD& dwByte);
	static void Data_WriteInt(HANDLE _hFile, const int& _iValue, DWORD& dwByte);
	static void Data_WriteUInt(HANDLE _hFile, const unsigned int& _iValue, DWORD& dwByte);
	static void Data_WriteFloat(HANDLE _hFile, const float& _fValue, DWORD& dwByte);
	static void Data_WriteVec3(HANDLE _hFile, const D3DXVECTOR3& _vValue, DWORD& dwByte);

public:
	static void Data_ReadBoolean(HANDLE _hFile, bool& _boolean, DWORD& dwByte);
	static void Data_ReadWString(HANDLE _hFile, wstring& _wstrValue, DWORD& dwByte);
	static void Data_ReadInt(HANDLE _hFile, int& _iValue, DWORD& dwByte);
	static void Data_ReadUInt(HANDLE _hFile, unsigned int& _iValue, DWORD& dwByte);
	static void Data_ReadFloat(HANDLE _hFile, float& _fValue, DWORD& dwByte);
	static void Data_ReadVec3(HANDLE _hFile, D3DXVECTOR3& _vValue, DWORD& dwByte);


	///////////				Calculator			///////////
public:
	static E_NumberFontType	Calc_Damage(E_CHAR_ATKTYPE eAttType, E_CHAR_DFFTYPE eDffType, _int& iDamageRef);



	///////////				Utility					///////////
public:
	// 마우스 위치 반환
	static POINT	Get_MousePoint(HWND hWnd);

	// 문자열을 순회해서 모든 문자가 숫자라면 True 반환, 아니면 False 반환
	static _bool	Check_IsDigit(const wstring& szString);

	// 경로의 모든 파일을 순회해서 _findData에 반환해준다
	static intptr_t	FindData_ChoiceType(const char * szRelativePath, const char * szType, _finddata_t * _findData);

	// wstring을 char* 형으로 변환해준다.
	static void		TCHAR_TO_CHAR(const wstring& wstr, char* szChar);

	// char* 을 wstring 형으로 변환해준다.
	static wstring	CHAR_TO_WSTRING(const char * szchar);

	// 숫자를 표현하는 WString을 . 을 기준으로 나눠 iDemical수치만큼 소수점밑으로 값을 자른다.
	static wstring	RoundWString(const wstring& wstr, _int iDemical);

	// 자동 줄넘김
	static wstring Auto_Line(wstring wstrFullText, wstring wstrCheckText, _int iSize);
	
	// Debug모드에서만 들어온 wstring을 출력한다.
	static void Print_Wstring_DMode(wstring wstrText);

	// 트리거이름을 E_Trigger에 맞게 번역해서 내보내준다.
	static wstring Get_TriggerName(E_Trigger eTriggerIndex);

	// 방향벡터를 임의의 축으로 Degree만큼 회전한다. 
	static _vec3	Rotate_DirVector(const _float& fDegree, const _vec3& vAxis, _vec3* vLook);

	static _vec3	Ui2World(_float fX, _float fY);

	static _vec2	World2Ui(_vec3 vPos);

	static wstring	GetIni(wstring Category, wstring wstrArg);

	static vector<string> Split(string s, string divid);

	static string	Wstring2String(wstring wstr);
	static wstring	String2Wstring(string wstr);


public:
	static wstring Get_CombatStatusPath(E_Combat_Status eStatus);

};

END

#endif // !__UTILITY_H__