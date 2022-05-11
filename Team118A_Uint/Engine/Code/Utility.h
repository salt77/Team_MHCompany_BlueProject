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
	static	int		Clamp_int(const int& iValue, int iLow, int iHigh);	//	Low, High ���̷� Value ����
	static float Lerp_float(float fStart, float fEnd, float fRatio, bool bClamp = TRUE);			//	���� ���� ���� ����
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
	// ���콺 ��ġ ��ȯ
	static POINT	Get_MousePoint(HWND hWnd);

	// ���ڿ��� ��ȸ�ؼ� ��� ���ڰ� ���ڶ�� True ��ȯ, �ƴϸ� False ��ȯ
	static _bool	Check_IsDigit(const wstring& szString);

	// ����� ��� ������ ��ȸ�ؼ� _findData�� ��ȯ���ش�
	static intptr_t	FindData_ChoiceType(const char * szRelativePath, const char * szType, _finddata_t * _findData);

	// wstring�� char* ������ ��ȯ���ش�.
	static void		TCHAR_TO_CHAR(const wstring& wstr, char* szChar);

	// char* �� wstring ������ ��ȯ���ش�.
	static wstring	CHAR_TO_WSTRING(const char * szchar);

	// ���ڸ� ǥ���ϴ� WString�� . �� �������� ���� iDemical��ġ��ŭ �Ҽ��������� ���� �ڸ���.
	static wstring	RoundWString(const wstring& wstr, _int iDemical);

	// �ڵ� �ٳѱ�
	static wstring Auto_Line(wstring wstrFullText, wstring wstrCheckText, _int iSize);
	
	// Debug��忡���� ���� wstring�� ����Ѵ�.
	static void Print_Wstring_DMode(wstring wstrText);

	// Ʈ�����̸��� E_Trigger�� �°� �����ؼ� �������ش�.
	static wstring Get_TriggerName(E_Trigger eTriggerIndex);

	// ���⺤�͸� ������ ������ Degree��ŭ ȸ���Ѵ�. 
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