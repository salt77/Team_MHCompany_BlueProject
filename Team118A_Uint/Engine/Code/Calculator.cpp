#include "Calculator.h"


CCalculator::CCalculator()
{
}

CCalculator::~CCalculator()
{
}

E_NumberFontType CCalculator::Calc_Damage(E_CHAR_ATKTYPE eAttType, E_CHAR_DFFTYPE eDffType, _int & iDamageRef)
{
	// �Ϲ� Ÿ�� ����
	if (E_CHAR_ATKTYPE::ATKTYPE_NORMAL == eAttType && E_CHAR_DFFTYPE::DFFTYPE_LIGHT == eDffType)
	{
		return E_NumberFontType::Normal;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_NORMAL == eAttType && E_CHAR_DFFTYPE::DFFTYPE_HEAVY == eDffType)
	{
		return E_NumberFontType::Normal;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_NORMAL == eAttType && E_CHAR_DFFTYPE::DFFTYPE_SPECICAL == eDffType)
	{
		return E_NumberFontType::Normal;
	}

	// ���� Ÿ�� ����
	else if (E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION == eAttType && E_CHAR_DFFTYPE::DFFTYPE_LIGHT == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 2.f);
		return E_NumberFontType::Weak;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION == eAttType && E_CHAR_DFFTYPE::DFFTYPE_HEAVY == eDffType)
	{
		return E_NumberFontType::Normal;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_EXPLOSION == eAttType && E_CHAR_DFFTYPE::DFFTYPE_SPECICAL == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 0.5f);
		return E_NumberFontType::Resist;
	}

	// ���� Ÿ�� ����
	else if (E_CHAR_ATKTYPE::ATKTYPE_PIERCE == eAttType && E_CHAR_DFFTYPE::DFFTYPE_LIGHT == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 0.5f);
		return E_NumberFontType::Resist;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_PIERCE == eAttType && E_CHAR_DFFTYPE::DFFTYPE_HEAVY == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 2.f);
		return E_NumberFontType::Weak;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_PIERCE == eAttType && E_CHAR_DFFTYPE::DFFTYPE_SPECICAL == eDffType)
	{
		return E_NumberFontType::Normal;
	}

	// �ź� Ÿ�� ����
	else if (E_CHAR_ATKTYPE::ATKTYPE_MYSTERY == eAttType && E_CHAR_DFFTYPE::DFFTYPE_LIGHT == eDffType)
	{
		return E_NumberFontType::Normal;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_MYSTERY == eAttType && E_CHAR_DFFTYPE::DFFTYPE_HEAVY == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 0.5f);
		return E_NumberFontType::Resist;
	}
	else if (E_CHAR_ATKTYPE::ATKTYPE_MYSTERY == eAttType && E_CHAR_DFFTYPE::DFFTYPE_SPECICAL == eDffType)
	{
		iDamageRef = (_int)(iDamageRef * 2.f);
		return E_NumberFontType::Weak;
	}

	
	return E_NumberFontType::Normal;
}
