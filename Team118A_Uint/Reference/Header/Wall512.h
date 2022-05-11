#pragma once
#ifndef __WALL512_H__
#define __WALL512_H__
#include "Engine_Define.h"
#include <ctime>

/*
랜덤 값을 반환해주는 클래스
작성자 : 김민현

*/
BEGIN(Engine)

class ENGINE_DLL CWall512
{
	DECLARE_SINGLETON(CWall512)
	enum
	{
		DIFFER_VALUE = 100,
	};

public:
	CWall512();

	explicit CWall512(unsigned int nSeed);

	~CWall512();

	unsigned int GetValue();

	unsigned int GetValue(unsigned int nMinValue, unsigned int nMaxValue);

	unsigned int GetValue(unsigned int nMaxValue);

	_bool	GetRandBoolean();

	

	float GetFloatValue();

	float GetFloatValue(float fMinValue, float fMaxValue);

	float GetFloatValue(float fMaxValue);

protected:
	unsigned long state[16];
	unsigned int index;
};

END

#endif // !__WALL512_H__