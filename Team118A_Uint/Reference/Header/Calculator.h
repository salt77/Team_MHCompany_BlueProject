#pragma once
#ifndef __CCALCULATOR_H__
#define __CCALCULATOR_H__

#include "Engine_Define.h"
#include "Engine_Enum.h"

USING(Engine)
class CCalculator
{
public:
	CCalculator();
	~CCalculator();


	static E_NumberFontType		Calc_Damage(E_CHAR_ATKTYPE eAttType, E_CHAR_DFFTYPE eDffType, _int& iDamageRef);

private:
	static int		m_iTemp;
	static float	m_fTemp;
	static D3DXVECTOR3 m_vTemp;
};

#endif // !__CCALCULATOR_H__