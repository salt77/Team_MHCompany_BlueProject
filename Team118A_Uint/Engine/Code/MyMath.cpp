#include "MyMath.h"

int				CMyMath::m_iTemp = 0;
float			CMyMath::m_fTemp = 0.f;
D3DXVECTOR3		CMyMath::m_vTemp = VEC_ZERO;

CMyMath::CMyMath()
{
}


CMyMath::~CMyMath()
{
}

int CMyMath::Clamp_int(const int & iValue, int iLow, int iHigh)
{
	m_iTemp = 0;

	if (iValue < iLow)
	{
		m_iTemp = iLow;
	}
	else if (iValue > iHigh)
	{
		m_iTemp = iHigh;
	}
	else
	{
		m_iTemp = iValue;
	}

	return m_iTemp;
}

float CMyMath::Lerp_float(float fStart, float fEnd, float fRatio, bool bClamp)
{
	if (bClamp)
	{
		fRatio = fRatio > 1.f ? 1.f : fRatio;
		fRatio = fRatio < 0.f ? 0.f : fRatio;
	}

	fStart = fStart * (1.f - fRatio);
	fEnd = fEnd * (fRatio);
	return fStart + fEnd;
}

float CMyMath::LerpEase_float(E_EASE eType, float x)
{
	// easeInOutCirc
	switch (eType)
	{
	case Engine::E_EASE::InSine:
		return 1 - cos((x * PI) / 2);
		break;
	case Engine::E_EASE::OutSine:
		return sin((x * PI) / 2);
		break;
	case Engine::E_EASE::InOutSine:
		return -(cos(PI * x) - 1) / 2;
		break;
	case Engine::E_EASE::InCubic:
		return x * x * x;
		break;
	case Engine::E_EASE::OutCubic:
		return 1 - pow(1 - x, 3);
		break;
	case Engine::E_EASE::InOutCubic:
		return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
		break;
	case Engine::E_EASE::InQuint:
		return x * x * x * x * x;
		break;
	case Engine::E_EASE::OutQuint:
		return 1 - pow(1 - x, 5);
		break;
	case Engine::E_EASE::InOutQuint:
		return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
		break;
	case Engine::E_EASE::InCirc:
		return 1 - sqrt(1 - pow(x, 2));
		break;
	case Engine::E_EASE::OutCirc:
		return sqrt(1 - pow(x - 1, 2));
		break;
	case Engine::E_EASE::InOutCirc:
		return x < 0.5
			? (1 - sqrt(1 - pow(2 * x, 2))) / 2
			: (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
		break;
	case Engine::E_EASE::InQuad:
		return x * x;
		break;
	case Engine::E_EASE::OutQuad:
		return 1 - (1 - x) * (1 - x);
		break;
	case Engine::E_EASE::InOutQuad:
		return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
		break;
	case Engine::E_EASE::InQuart:
		return x * x * x * x;
		break;
	case Engine::E_EASE::OutQuart:
		return 1 - pow(1 - x, 4);
		break;
	case Engine::E_EASE::InOutQuart:
		return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
		break;
	case Engine::E_EASE::InExpo:
		return x == 0 ? 0 : powf(2, 10 * x - 10);
		break;
	case Engine::E_EASE::OutExpo:
		return x == 1 ? 1 : 1 - powf(2, -10 * x);
		break;
	case Engine::E_EASE::InOutExpo:
		return x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5 ? powf(2, 20 * x - 10) / 2
			: (2 - powf(2, -20 * x + 10)) / 2;
		break;
	case Engine::E_EASE::InBack:
	{
		const double c1 = 1.70158;
		const double c3 = c1 + 1;

		return (float)c3 * x * x * x - (float)c1 * x * x;
		break;
	}
	case Engine::E_EASE::OutBack:
	{
		const double c1 = 1.70158;
		const double c3 = c1 + 1;

		return 1 + (float)c3 * pow(x - 1, 3) + (float)c1 * pow(x - 1, 2);
		break;
	}
	case Engine::E_EASE::InOutBack:
	{
		const double c1 = 1.70158;
		const double c2 = c1 * 1.525;

		return x < 0.5
			? (pow(2 * x, 2) * (((float)c2 + 1) * 2 * x - (float)c2)) / 2
			: (pow(2 * x - 2, 2) * (((float)c2 + 1) * (x * 2 - 2) + (float)c2) + 2) / 2;
		break;
	}
	default:
		break;
	}

	return 0.f;
}

float CMyMath::Clamp_float(const float& fValue, float fLow, float fHigh)
{
	m_fTemp = 0.f;

	if (fValue < fLow)
	{
		m_fTemp = fLow;
	}
	else if (fValue > fHigh)
	{
		m_fTemp = fHigh;
	}
	else
	{
		m_fTemp = fValue;
	}

	return m_fTemp;
}

D3DXVECTOR2 CMyMath::Lerp_vec2(D3DXVECTOR2 vStart, D3DXVECTOR2 vEnd, float fRatio, bool bClamp)
{
	if (bClamp)
	{
		fRatio = fRatio > 1.f ? 1.f : fRatio;
		fRatio = fRatio < 0.f ? 0.f : fRatio;
	}

	vStart.x = vStart.x * (1.f - fRatio);
	vEnd.x = vEnd.x * (fRatio);

	vStart.y = vStart.y * (1.f - fRatio);
	vEnd.y = vEnd.y * (fRatio);

	return vStart + vEnd;
}

D3DXVECTOR3 CMyMath::Lerp_vec3(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fRatio, bool bClamp)
{
	if (bClamp)
	{
		fRatio = fRatio > 1.f ? 1.f : fRatio;
		fRatio = fRatio < 0.f ? 0.f : fRatio;
	}

	vStart.x = vStart.x * (1.f - fRatio);
	vEnd.x = vEnd.x * (fRatio);

	vStart.y = vStart.y * (1.f - fRatio);
	vEnd.y = vEnd.y * (fRatio);

	vStart.z = vStart.z * (1.f - fRatio);
	vEnd.z = vEnd.z * (fRatio);

	return vStart + vEnd;
}

float CMyMath::Distance_vec3(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd)
{
	m_fTemp = 0.f;
	m_fTemp = sqrtf(powf((vEnd.x - vStart.x), 2) + powf((vEnd.y - vStart.y), 2) + powf((vEnd.z - vStart.z), 2));

	return m_fTemp;
}

// Degree 반환
float CMyMath::GetAngleToTarget_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	vDir			= *D3DXVec3Normalize(&vDir, &vDir);
	vToTargetDir	= *D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	float fDot = D3DXVec3Dot(&vDir, &vToTargetDir);
	
	fDot = acosf(fDot);

	return fDot*Rad2Deg;
}

// Target이 앞이면 TRUE 반환
bool CMyMath::CheckFront_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	if (GetAngleToTarget_vec3(vDir, vToTargetDir) <= 90.f)
	{
		return TRUE;
	}

	return FALSE;
}

// vDir : 플레이어 방향 벡터, 
// Target이 오른쪽이면 TRUE 반환
bool CMyMath::CheckRight_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir, D3DXVECTOR3 vUp)
{
	vDir = *D3DXVec3Normalize(&vDir, &vDir);
	vToTargetDir = *D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	if (0.f <= D3DXVec3Dot(&vUp, D3DXVec3Cross(&m_vTemp, &vToTargetDir, &vDir)))
	{
		return TRUE;
	}

	return FALSE;
}

// 두 방향벡터의 수직하는 방향벡터(법선 벡터) 를 반환
D3DXVECTOR3 CMyMath::GetNormal_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir)
{
	vDir = *D3DXVec3Normalize(&vDir, &vDir);
	vToTargetDir = *D3DXVec3Normalize(&vToTargetDir, &vToTargetDir);

	return *D3DXVec3Cross(&m_vTemp, &vDir, &vToTargetDir);
}

D3DXVECTOR3 CMyMath::GetLookRotation(D3DXVECTOR3 vPos, D3DXVECTOR3 vTargetPos)
{
	_vec3 vResultAngle;
	//y 회전,x 회전으로 치환
	_vec3 vTargetLook = vTargetPos - vPos;


	_vec3 vZLook = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vZLook, &vZLook);
	_vec3 vYLook = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vYLook, &vYLook);

	_vec3 vZTargetLook = _vec3(vTargetLook.x, 0.f, vTargetLook.z);
	D3DXVec3Normalize(&vZTargetLook, &vZTargetLook);
	_float fDis = sqrtf(vTargetLook.x*vTargetLook.x + vTargetLook.z*vTargetLook.z);
	_vec3 vYTargetLook = _vec3(0.f, vTargetLook.y, fDis);
	D3DXVec3Normalize(&vYTargetLook, &vYTargetLook);

	_float fZAngle = D3DXVec3Dot(&vZLook, &vZTargetLook);
	_float fYAngle = D3DXVec3Dot(&vYLook, &vYTargetLook);
	fZAngle = acosf(fZAngle);
	fYAngle = acosf(fYAngle);

	_vec3 vZCross;
	_vec3 vYCross;
	D3DXVec3Cross(&vZCross, &vZLook, &vZTargetLook);
	D3DXVec3Cross(&vYCross, &vYLook, &vYTargetLook);

	if (vZCross.y < 0.f)
	{
		fZAngle = -fZAngle;
	}

	if (vYCross.x < 0.f)
	{
		fYAngle = -fYAngle;
	}

	vResultAngle.y = fZAngle*Rad2Deg;
	vResultAngle.x = fYAngle*Rad2Deg;
	vResultAngle.z = 0.f;

	return vResultAngle;
}

bool CMyMath::Get_InCamera(D3DXVECTOR3 vPos, D3DXMATRIX matView, D3DXMATRIX matProj)
{	
	D3DXVECTOR3 vNearPos[4] = { D3DXVECTOR3(-1.f, 1.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), D3DXVECTOR3(-1.f, -1.f, 0.f), D3DXVECTOR3(1.f, -1.f, 0.f) };
	D3DXVECTOR3 vFarPos[4] = { D3DXVECTOR3(-1.f, 1.f, 1.f), D3DXVECTOR3(1.f, 1.f, 1.f), D3DXVECTOR3(-1.f, -1.f, 1.f), D3DXVECTOR3(1.f, -1.f, 1.f) };

	D3DXVECTOR3 vProjPos = vPos;

	D3DXVec3TransformCoord(&vProjPos, &vProjPos, &matView);

	D3DXVec3TransformCoord(&vProjPos, &vProjPos, &matProj);
	
	if (vProjPos.x < -1.f || vProjPos.x > 1.f || vProjPos.y < -1.f || vProjPos.y > 1.f || vProjPos.z < 0.f || vProjPos.z > 1.f)
	{
		return FALSE;
	}

	return TRUE;
}


D3DXVECTOR4 CMyMath::Lerp_vec4(D3DXVECTOR4 vStart, D3DXVECTOR4 vEnd, float fRatio, bool bClamp)
{
	if (bClamp)
	{
		fRatio = fRatio > 1.f ? 1.f : fRatio;
		fRatio = fRatio < 0.f ? 0.f : fRatio;
	}

	vStart.x = vStart.x * (1.f - fRatio);
	vEnd.x = vEnd.x * (fRatio);

	vStart.y = vStart.y * (1.f - fRatio);
	vEnd.y = vEnd.y * (fRatio);

	vStart.z = vStart.z * (1.f - fRatio);
	vEnd.z = vEnd.z * (fRatio);

	vStart.w = vStart.w * (1.f - fRatio);
	vEnd.w = vEnd.w * (fRatio);

	return vStart + vEnd;
}

D3DXVECTOR3 CMyMath::Bezier(float fTime, D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3, D3DXVECTOR3 P4)
{
	// 방정식.
	fTime = Clamp_float(fTime, 0.f, 1.f);

	return pow((1.f - fTime), 3) * P1
		+ pow((1.f - fTime), 2) * 3 * fTime * P2
		+ pow(fTime, 2) * 3 * (1 - fTime) * P3
		+ pow(fTime, 3) * P4;
}
