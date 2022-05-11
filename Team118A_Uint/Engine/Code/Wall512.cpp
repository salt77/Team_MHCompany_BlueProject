#include "Wall512.h"
IMPLEMENT_SINGLETON(CWall512)

CWall512::CWall512()
{
	//시드값 자동 생성
	index = 0;
	unsigned int s = static_cast<unsigned int>(time(NULL));

	for (int i = 0; i < 16; i++)
	{
		state[i] = s;
		s += s + DIFFER_VALUE;
	}
}

CWall512::CWall512(unsigned int nSeed)
{
	//시드 값을 받아 초기화
	index = 0;
	unsigned int s = nSeed;

	for (int i = 0; i < 16; i++)
	{
		state[i] = s;
		s += s + DIFFER_VALUE;
	}
}

CWall512::~CWall512()
{

}

//	unsigned int값 랜덤하게 반환
unsigned int CWall512::GetValue()
{
	unsigned int a, b, c, d;

	a = state[index];
	c = state[(index + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = state[(index + 9) & 15];
	c ^= (c >> 11);
	a = state[index] = b ^ c;
	d = a ^ ((a << 5) & 0xda442d24U);
	index = (index + 15) & 15;
	a = state[index];
	state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
	return state[index];
}

//	unsigned int값 랜덤하게 반환 범위 Min ~ (Max - 1) 
unsigned int CWall512::GetValue(unsigned int nMinValue, unsigned int nMaxValue)
{
	return nMinValue + (GetValue() % (nMaxValue - nMinValue));
}

//	unsigned int값 랜덤하게 반환 범위 0 ~ (Max - 1) 
unsigned int CWall512::GetValue(unsigned int nMaxValue)
{
	return GetValue() % nMaxValue;
}

_bool CWall512::GetRandBoolean()
{
	_int iRand = CWall512::GetInstance()->GetValue(0, 2);
	if (iRand == 0)
		return TRUE;

	return FALSE;
}


//	float값 랜덤하게 반환
float CWall512::GetFloatValue()
{
	unsigned int nRandValue = GetValue();


	union { unsigned long ul; float f; } p;
	p.ul = (((nRandValue *= 16807) & 0x007fffff) - 1) | 0x3f800000;

	return p.f - 1.f;
}

// Float값 랜덤하게 반환 범위 Min ~ (Max - 1)
float CWall512::GetFloatValue(float fMinValue, float fMaxValue)
{
	return fMinValue + GetFloatValue() * (fMaxValue - fMinValue);
}

// float값 랜덤하게 반환 범위 (0 ~ Max)
float CWall512::GetFloatValue(float fMaxValue)
{
	return GetFloatValue() * fMaxValue;
}
