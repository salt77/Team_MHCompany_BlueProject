#include "Utility.h"
#include "GraphicDev.h"


CUtility::CUtility()
{
}


CUtility::~CUtility()
{
}


///////////				Math					///////////
int CUtility::Clamp_int(const int & iValue, int iLow, int iHigh)
{
	return CMyMath::Clamp_int(iValue, iLow, iHigh);
}

float CUtility::Lerp_float(float fStart, float fEnd, float fRatio, bool bClamp)
{
	return CMyMath::Lerp_float(fStart, fEnd, fRatio, bClamp);
}

float CUtility::LerpEase_float(Engine::E_EASE eType, float x)
{
	return CMyMath::LerpEase_float(eType, x);
}

float CUtility::Clamp_float(const float & fValue, float fLow, float fHigh)
{
	return CMyMath::Clamp_float(fValue, fLow, fHigh);
}

D3DXVECTOR2 CUtility::Lerp_vec2(D3DXVECTOR2 vStart, D3DXVECTOR2 vEnd, float fRatio, bool bClamp)
{
	return CMyMath::Lerp_vec2(vStart, vEnd, fRatio, bClamp);
}

D3DXVECTOR3 CUtility::Lerp_vec3(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fRatio, bool bClamp)
{
	return CMyMath::Lerp_vec3(vStart, vEnd, fRatio, bClamp);
}

float CUtility::Distance_vec3(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd)
{
	return CMyMath::Distance_vec3(vStart, vEnd);
}

float CUtility::GetAngleToTarget_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	return CMyMath::GetAngleToTarget_vec3(vDir, vToTargetDir);
}

bool CUtility::CheckFront_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	return CMyMath::CheckFront_vec3(vDir, vToTargetDir);
}

bool CUtility::CheckRight_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir, D3DXVECTOR3 vUp)
{
	return CMyMath::CheckRight_vec3(vDir, vToTargetDir, vUp);
}

D3DXVECTOR3 CUtility::GetNormal_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	return CMyMath::GetNormal_vec3(vDir, vToTargetDir);
}

D3DXVECTOR4 CUtility::Lerp_vec4(D3DXVECTOR4 vStart, D3DXVECTOR4 vEnd, float fRatio, bool bClamp)
{
	return CMyMath::Lerp_vec4(vStart, vEnd, fRatio, bClamp);
}

D3DXVECTOR3 CUtility::Bezier(_float fTime, D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3, D3DXVECTOR3 P4)
{
	return CMyMath::Bezier(fTime, P1, P2, P3, P4);
}








///////////			DataParsing					///////////


void CUtility::Data_WriteBoolean(HANDLE _hFile, const bool & _boolean, DWORD & dwByte)
{
	return CDataParsing::Data_WriteBoolean(_hFile, _boolean, dwByte);
}

void CUtility::Data_WriteWString(HANDLE _hFile, const wstring & _wstrValue, DWORD & dwByte)
{
	return CDataParsing::Data_WriteWString(_hFile, _wstrValue, dwByte);
}

void CUtility::Data_WriteInt(HANDLE _hFile, const int & _iValue, DWORD & dwByte)
{
	return CDataParsing::Data_WriteInt(_hFile, _iValue, dwByte);
}

void CUtility::Data_WriteUInt(HANDLE _hFile, const unsigned int & _iValue, DWORD & dwByte)
{
	return CDataParsing::Data_WriteUInt(_hFile, _iValue, dwByte);
}

void CUtility::Data_WriteFloat(HANDLE _hFile, const float & _fValue, DWORD & dwByte)
{
	return CDataParsing::Data_WriteFloat(_hFile, _fValue, dwByte);
}

void CUtility::Data_WriteVec3(HANDLE _hFile, const D3DXVECTOR3 & _vValue, DWORD & dwByte)
{
	return CDataParsing::Data_WriteVec3(_hFile, _vValue, dwByte);
}

void CUtility::Data_ReadBoolean(HANDLE _hFile, bool & _boolean, DWORD & dwByte)
{
	return CDataParsing::Data_ReadBoolean(_hFile, _boolean, dwByte);
}

void CUtility::Data_ReadWString(HANDLE _hFile, wstring & _wstrValue, DWORD & dwByte)
{
	return CDataParsing::Data_ReadWString(_hFile, _wstrValue, dwByte);
}

void CUtility::Data_ReadInt(HANDLE _hFile, int & _iValue, DWORD & dwByte)
{
	return CDataParsing::Data_ReadInt(_hFile, _iValue, dwByte);
}

void CUtility::Data_ReadUInt(HANDLE _hFile, unsigned int & _iValue, DWORD & dwByte)
{
	return CDataParsing::Data_ReadUInt(_hFile, _iValue, dwByte);
}

void CUtility::Data_ReadFloat(HANDLE _hFile, float & _fValue, DWORD & dwByte)
{
	return CDataParsing::Data_ReadFloat(_hFile, _fValue, dwByte);
}

void CUtility::Data_ReadVec3(HANDLE _hFile, D3DXVECTOR3 & _vValue, DWORD & dwByte)
{
	return CDataParsing::Data_ReadVec3(_hFile, _vValue, dwByte);
}

///////////			Calculator					///////////


E_NumberFontType CUtility::Calc_Damage(E_CHAR_ATKTYPE eAttType, E_CHAR_DFFTYPE eDffType, _int& iDamageRef)
{
	return CCalculator::Calc_Damage(eAttType, eDffType, iDamageRef);
}





///////////			Temp						///////////


POINT CUtility::Get_MousePoint(HWND hWnd)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	return ptMouse;
}

_bool CUtility::Check_IsDigit(const wstring & szString)
{
	bool bFind = TRUE;
	int iSize = (int)szString.length();
	wstring wstrTemp;
	for (int i = 0; i < iSize; ++i)
	{
		if (szString[0] == L'-')
		{
		}
		else if (isdigit(szString[i]))
		{
			wstrTemp.append(to_wstring(szString[i] - 48));
		}
		else
		{
			bFind = FALSE;
			break;
		}
	}

	if (FALSE == bFind)
	{
		return FALSE;
	}

	return TRUE;
}

intptr_t CUtility::FindData_ChoiceType(const char * szRelativePath, const char * szType, _finddata_t * _findData)
{
	char szPath[MAX_PATH] = "";
	strcpy_s(szPath, szRelativePath);
	strcat_s(szPath, szType);

	intptr_t lHandle = (intptr_t)_findfirst(szPath, _findData);

	NULL_CHECK_RETURN(lHandle, 0);

	return lHandle;
}

void CUtility::TCHAR_TO_CHAR(const wstring & wstr, char * szChar)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szChar, len, nullptr, nullptr);
}

wstring CUtility::CHAR_TO_WSTRING(const char * szchar)
{
	wchar_t* tmp = nullptr;
	int len = MultiByteToWideChar(CP_ACP, 0, szchar, -1, nullptr, 0);
	tmp = new wchar_t[len + 1];

	// ...
	MultiByteToWideChar(CP_ACP, 0, szchar, -1, tmp, len);
	wstring returnString = wstring(tmp);
	Safe_Delete_Array(tmp);
	return returnString;
}

wstring CUtility::RoundWString(const wstring & wstr, _int iDemical)
{
	_int iCheck = (_int)wstr.rfind(L".");
	if (iCheck < 0)
	{
		wstring wstrResult = wstr;
		if (iDemical > -1)
			wstrResult.append(L".");
	
		for (_int i = 0; i < iDemical; ++i)
		{
			wstrResult += L"0";
		}
		return wstrResult;
	}
	else
	{
	}

	return wstr.substr(0, iCheck + iDemical + 1);
}

wstring CUtility::Auto_Line(wstring wstrFullText, wstring wstrCheckText, _int iSize)
{
	wstring wstrResultText = wstrFullText;

	wstring wstrSearchText = wstrFullText;

	wstring wstrLeftText = L"";

	while (TRUE)
	{
		_int iCheck = (_int)wstrSearchText.find(wstrCheckText);

		if (iCheck < 0)
		{
			return wstrLeftText + wstrSearchText;
			break;
		}

		wstrLeftText += wstrSearchText.substr(0, iCheck + 1);
		if ((_int)wstrLeftText.size() > iSize)
		{
			wstrLeftText += L"\n";
			wstrSearchText = wstrSearchText.substr(iCheck + 1);
			break;
		}
		else
		{
			wstrSearchText = wstrSearchText.substr(iCheck + 1);
		}
	}


	wstring wstrRightText = Auto_Line(wstrSearchText, wstrCheckText, iSize);

	wstrResultText = wstrLeftText + wstrRightText;

	return wstrResultText;

}

void CUtility::Print_Wstring_DMode(wstring wstrText)
{
#ifdef _DEBUG
		char szText[MAX_PATH];
		TCHAR_TO_CHAR(wstrText, szText);
		cout << szText << endl;
#endif 
}

wstring CUtility::Get_TriggerName(E_Trigger eTriggerIndex)
{
	switch (eTriggerIndex)
	{
	case Engine::E_Trigger::Attack_1:	return L"Attack_1";
	case Engine::E_Trigger::Attack_2:	return L"Attack_2";
	case Engine::E_Trigger::Attack_3:	return L"Attack_3";
	case Engine::E_Trigger::Dead:		return L"Dead";
	case Engine::E_Trigger::Reload:		return L"Reload";
	case Engine::E_Trigger::ExSkill:	return L"ExSkill";
	case Engine::E_Trigger::Buff_1:		return L"Buff_1";
	case Engine::E_Trigger::Buff_2:		return L"Buff_2";
	case Engine::E_Trigger::Trg_1:		return L"Trg_1";
	case Engine::E_Trigger::Trg_2:		return L"Trg_2";
	case Engine::E_Trigger::Trg_3:		return L"Trg_3";
	case Engine::E_Trigger::Trg_4:		return L"Trg_4";
	case Engine::E_Trigger::Trg_5:		return L"Trg_5";
	case Engine::E_Trigger::Trg_6:		return L"Trg_6";
	case Engine::E_Trigger::Trg_7:		return L"Trg_7";
	case Engine::E_Trigger::Trg_8:		return L"Trg_8";
	case Engine::E_Trigger::Trg_9:		return L"Trg_9";
	case Engine::E_Trigger::Trigger_End: assert(FALSE);
	default: assert(FALSE);
	}

	return L"";
}

_vec3 CUtility::Rotate_DirVector(const _float & fDegree, const _vec3 & vAxis, _vec3 * vLook)
{
	D3DXVec3Normalize(vLook, vLook);

	_matrix		 matRotate;
	D3DXMatrixIdentity(&matRotate);
	D3DXMatrixRotationAxis(&matRotate, &vAxis, D3DXToRadian(fDegree));
	D3DXVec3TransformNormal(vLook, vLook, &matRotate);
	D3DXVec3Normalize(vLook, vLook);

	return *vLook; 
}

_vec3 CUtility::Ui2World(_float fX, _float fY)
{
	_float fProjX = (fX / WINCX)*2.f - 1.f;
	_float fProjY = (fY / WINCY)*2.f - 1.f;

	_vec3 vProjPos = _vec3(fProjX, fProjY, 0.5f);

	_matrix matInvProj;
	_matrix matInvView;

	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matInvProj);
	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matInvView);

	D3DXMatrixInverse(&matInvProj, 0, &matInvProj);
	D3DXMatrixInverse(&matInvView, 0, &matInvView);

	_vec3 vViewPos;
	D3DXVec3TransformCoord(&vViewPos, &vProjPos, &matInvProj);
	_vec3 vWorldPos;
	D3DXVec3TransformCoord(&vWorldPos, &vViewPos, &matInvView);

	return vWorldPos;
}

_vec2 CUtility::World2Ui(_vec3 vPos)
{
	_matrix matView, matProj;
	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	CGraphicDev::GetInstance()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vViewPos;
	D3DXVec3TransformCoord(&vViewPos, &vPos, &matView);
	_vec3 vProjPos;
	D3DXVec3TransformCoord(&vProjPos, &vViewPos, &matProj);

	_float fUIPosX = vProjPos.x * _float(WINCX >> 1);
	_float fUIPosY = vProjPos.y * _float(WINCY >> 1);

	return _vec2(fUIPosX, fUIPosY);
}

// 프로젝트 폴더에 있는 TestForIni.ini파일에 카테고리, 변수를 만들고 값을 넣으면 해당 값을 반환해준다. 
// 프로젝트 실행도중에도 값변경 가능.
wstring CUtility::GetIni(wstring Category, wstring wstrArg)
{
	_tchar wstrTemp[MAX_PATH]{};

	GetPrivateProfileStringW(Category.c_str(), wstrArg.c_str(), L"읽기 실패", wstrTemp, sizeof(wstrTemp), L"../../TestForIni.ini");
	// 1.카테고리 이름, 2.변수 이름, 3.설정값이 없을 때 불러올 값, 4. 값을 저장할 변수, 5. 불러올 사이즈, 6.불러올 경로

	return wstrTemp;
}

vector<string> CUtility::Split(string s, string divid)
{
	vector<string> v;
	int start = 0;
	int d = (int)s.find(divid);
	while (d != -1) {
		v.push_back(s.substr(start, d - start));
		start = d + 1;
		d = (int)s.find(divid, start);
	}
	v.push_back(s.substr(start, d - start));

	return v;
}

string CUtility::Wstring2String(wstring wstr)
{
	string strTemp;
	strTemp.assign(wstr.begin(), wstr.end());
	return strTemp;
}

wstring CUtility::String2Wstring(string str)
{
	wstring wstrTemp;
	wstrTemp.assign(str.begin(), str.end());
	return wstrTemp;
}


wstring CUtility::Get_CombatStatusPath(E_Combat_Status eStatus)
{
	wstring wstrPath = L"../../Reference/Resource/Texture/UI/Combat/";
	switch (eStatus)
	{
	case Engine::E_Combat_Status::B_DEF:						wstrPath.append(L"Buff_DEF"); break;
	case Engine::E_Combat_Status::B_Dodge:						wstrPath.append(L"Buff_Dodge"); break;
	case Engine::E_Combat_Status::B_DotHeal:					wstrPath.append(L"Buff_DotHeal"); break;
	case Engine::E_Combat_Status::B_HealEffectiveness:			wstrPath.append(L"Buff_HealEffectiveness"); break;
	case Engine::E_Combat_Status::B_HealPower:					wstrPath.append(L"Buff_HealPower"); break;
	case Engine::E_Combat_Status::B_HIT:						wstrPath.append(L"Buff_HIT"); break;
	case Engine::E_Combat_Status::B_MAXHP:						wstrPath.append(L"Buff_MAXHP"); break;
	case Engine::E_Combat_Status::B_Mind:						wstrPath.append(L"Buff_Mind"); break;
	case Engine::E_Combat_Status::B_MoveSpeed:					wstrPath.append(L"Buff_MoveSpeed"); break;
	case Engine::E_Combat_Status::B_OppressionPower:			wstrPath.append(L"Buff_OppressionPower"); break;
	case Engine::E_Combat_Status::B_OppressionResist:			wstrPath.append(L"Buff_OppressionResist"); break;
	case Engine::E_Combat_Status::B_Range:						wstrPath.append(L"Buff_Range"); break;
	case Engine::E_Combat_Status::B_Shield:						wstrPath.append(L"Buff_Shield"); break;
	case Engine::E_Combat_Status::B_View:						wstrPath.append(L"Buff_View"); break;
	case Engine::E_Combat_Status::CC_Blind:						wstrPath.append(L"CC_Blind"); break;
	case Engine::E_Combat_Status::CC_Charmed:					wstrPath.append(L"CC_Charmed"); break;
	case Engine::E_Combat_Status::CC_Confusion:					wstrPath.append(L"CC_Confusion"); break;
	case Engine::E_Combat_Status::CC_Emp:						wstrPath.append(L"CC_Emp"); break;
	case Engine::E_Combat_Status::CC_Fear:						wstrPath.append(L"CC_Fear"); break;
	case Engine::E_Combat_Status::CC_Inoperative:				wstrPath.append(L"CC_Inoperative"); break;
	case Engine::E_Combat_Status::CC_MindControl:				wstrPath.append(L"CC_MindControl"); break;
	case Engine::E_Combat_Status::CC_Paralysis:					wstrPath.append(L"CC_Paralysis"); break;
	case Engine::E_Combat_Status::CC_Provoke:					wstrPath.append(L"CC_Provoke"); break;
	case Engine::E_Combat_Status::CC_Slience:					wstrPath.append(L"CC_Silence"); break;
	case Engine::E_Combat_Status::CC_Stunned:					wstrPath.append(L"CC_Stunned"); break;
	case Engine::E_Combat_Status::DB_AmmoCount:					wstrPath.append(L"Debuff_AmmoCount"); break;
	case Engine::E_Combat_Status::DB_ATK:						wstrPath.append(L"Debuff_ATK"); break;
	case Engine::E_Combat_Status::DB_AttackSpeed:				wstrPath.append(L"Debuff_AttackSpeed"); break;
	case Engine::E_Combat_Status::DB_Bleed:						wstrPath.append(L"Debuff_Bleed"); break;
	case Engine::E_Combat_Status::DB_BlockRate:					wstrPath.append(L"Debuff_BlockRate"); break;
	case Engine::E_Combat_Status::DB_BulletTypeDamage:			wstrPath.append(L"Debuff_BulletTypeDamage"); break;
	case Engine::E_Combat_Status::DB_Burn:						wstrPath.append(L"Debuff_Burn"); break;
	case Engine::E_Combat_Status::DB_Chill:						wstrPath.append(L"Debuff_Chill"); break;
	case Engine::E_Combat_Status::DB_ConcentratedTarget:		wstrPath.append(L"Debuff_ConcentratedTarget"); break;
	case Engine::E_Combat_Status::DB_CriticalChance:			wstrPath.append(L"Debuff_CriticalChance"); break;
	case Engine::E_Combat_Status::DB_CriticalChanceResistPoint:	wstrPath.append(L"Debuff_CriticalChanceResistPoint"); break;
	case Engine::E_Combat_Status::DB_CriticalDamage:			wstrPath.append(L"Debuff_CriticalDamage"); break;
	case Engine::E_Combat_Status::DB_DamagedRatio:				wstrPath.append(L"Debuff_DamagedRatio"); break;
	case Engine::E_Combat_Status::DB_DamageRatio:				wstrPath.append(L"Debuff_DamageRatio"); break;
	case Engine::E_Combat_Status::DB_DEF:						wstrPath.append(L"Debuff_DEF"); break;
	case Engine::E_Combat_Status::DB_Dodge:						wstrPath.append(L"Debuff_Dodge"); break;
	case Engine::E_Combat_Status::DB_DotHeal:					wstrPath.append(L"Debuff_Dot"); break;
	case Engine::E_Combat_Status::DB_HealEffectiveness:			wstrPath.append(L"Debuff_HealEffectiveness"); break;
	case Engine::E_Combat_Status::DB_HealPower:					wstrPath.append(L"Debuff_HealPower"); break;
	case Engine::E_Combat_Status::DB_HIT:						wstrPath.append(L"Debuff_HIT"); break;
	case Engine::E_Combat_Status::DB_MAXHP:						wstrPath.append(L"Debuff_MAXHP"); break;
	case Engine::E_Combat_Status::DB_Mind:						wstrPath.append(L"Debuff_Mind"); break;
	case Engine::E_Combat_Status::DB_MoveSpeed:					wstrPath.append(L"Debuff_MoveSpeed"); break;
	case Engine::E_Combat_Status::DB_OppressionPower:			wstrPath.append(L"Debuff_OppressionPower"); break;
	case Engine::E_Combat_Status::DB_OppressionResist:			wstrPath.append(L"Debuff_OppressionResist"); break;
	case Engine::E_Combat_Status::DB_Range:						wstrPath.append(L"Debuff_Poison"); break;
	case Engine::E_Combat_Status::DB_Shield:					wstrPath.append(L"Debuff_Range"); break;
	case Engine::E_Combat_Status::DB_View:						wstrPath.append(L"Debuff_View"); break;
	case Engine::E_Combat_Status::Spe_Fury:						wstrPath.append(L"Special_Fury"); break;
	case Engine::E_Combat_Status::Spe_Immortal:	 				wstrPath.append(L"Special_Immortal"); break;
	default:
		assert(FALSE);
		break;
	}

	wstrPath.append(L".png");

	return wstrPath;
}
