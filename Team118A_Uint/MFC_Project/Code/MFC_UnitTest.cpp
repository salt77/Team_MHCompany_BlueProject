#include "stdafx.h"
#include "MFC_UnitTest.h"

CMFC_UnitTest::CMFC_UnitTest(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CUnit(pGraphicDev, eObjTag, wstrUnitName)
{
}

CMFC_UnitTest::CMFC_UnitTest(const CUnit & rhs)
	: CUnit(rhs)
{
}

CMFC_UnitTest::~CMFC_UnitTest()
{
}

CMFC_UnitTest * CMFC_UnitTest::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
{
	CMFC_UnitTest* pInstance = new CMFC_UnitTest(pGraphicDev, eObjTag, wstrUnitName);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMFC_UnitTest::Free()
{
	CUnit::Free();
}

HRESULT CMFC_UnitTest::Ready_Object()
{
	return CUnit::Ready_Object();
}

_int CMFC_UnitTest::Start_Object(const _float & fTimeDelta)
{
	return CUnit::Start_Object(fTimeDelta);
}

_int CMFC_UnitTest::Update_Object(const _float & fTimeDelta)
{
	return CUnit::Update_Object(fTimeDelta);
}

_int CMFC_UnitTest::LateUpdate_Object(const _float & fTimeDelta)
{
	return CUnit::LateUpdate_Object(fTimeDelta);
}

void CMFC_UnitTest::Render_Object(_int iPass)
{
	CUnit::Render_Object(iPass);
}

void CMFC_UnitTest::Set_Constantable()
{
	CUnit::Set_Constantable();

}

void CMFC_UnitTest::OnEnable()
{
	CUnit::OnEnable();
}

void CMFC_UnitTest::OnDisable()
{
	CUnit::OnDisable();
}

void CMFC_UnitTest::OnDestroy()
{
	CUnit::OnDestroy();
}