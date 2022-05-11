#pragma once
#ifndef __MATH_H__
#define __MATH_H__

#include "Engine_Define.h"

class CMyMath 
{
public:
	CMyMath();
	~CMyMath();

public:
	//	단위		: int
	static int			Clamp_int(const int& iValue, int iLow, int iHigh);	//	Low, High 사이로 Value 보정


	//	단위 : Float 
public:
	static float			Lerp_float(float fStart, float fEnd, float fRatio, bool bClamp = TRUE);			//	숫자 간의 선형 보간
	static float			LerpEase_float(Engine::E_EASE eType, float x);
	static float			Clamp_float(const float& fValue, float fLow, float fHigh);	//	Low, High 사이로 Value 보정


	//	단위 : Vector2
public:
	static D3DXVECTOR2		Lerp_vec2(D3DXVECTOR2 vStart, D3DXVECTOR2 vEnd, float fRatio, bool bClamp = TRUE);


	//	단위 : Vector3
public:
	static D3DXVECTOR3		Lerp_vec3(const D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fRatio, bool bClamp = TRUE);
	// 클램프 Vec3 
	static float			Distance_vec3(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd);
	static float			GetAngleToTarget_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);
	static bool				CheckFront_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);
	static bool				CheckRight_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir, D3DXVECTOR3 vUp = VEC_UP);
	static D3DXVECTOR3		GetNormal_vec3(D3DXVECTOR3 vDir, D3DXVECTOR3 vToTargetDir);
	
	static D3DXVECTOR3		GetLookRotation(D3DXVECTOR3 vPos, D3DXVECTOR3 vTargetPos);

	static bool				Get_InCamera(D3DXVECTOR3 vPos, D3DXMATRIX matView, D3DXMATRIX matProj);
public:
	static D3DXVECTOR4		Lerp_vec4(D3DXVECTOR4 vStart, D3DXVECTOR4 vEnd, float fRatio, bool bClamp = TRUE);
	// Dir To Angle
	// Angle To Dir


public:
	static D3DXVECTOR3	Bezier(float fTime, D3DXVECTOR3 P1, D3DXVECTOR3 P2, D3DXVECTOR3 P3, D3DXVECTOR3 P4);

private:
	static int		m_iTemp;
	static float	m_fTemp;
	static D3DXVECTOR3 m_vTemp;
};

#endif // !__MATH_H__