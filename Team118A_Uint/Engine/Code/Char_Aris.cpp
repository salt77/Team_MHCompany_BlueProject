#include "Char_Aris.h"

USING(Engine)

CChar_Aris::CChar_Aris(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Aris::CChar_Aris(const CChar_Aris & rhs)
	: CCharacter(rhs)
{
}

CChar_Aris::~CChar_Aris()
{
}

CChar_Aris * CChar_Aris::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Aris*	pInstance = new CChar_Aris(pGraphicDev, eObjTag, L"Aris");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Aris::Free()
{
	CCharacter::Free();
}

void CChar_Aris::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Aris::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Aris::Ready_Object()
{
	CCharacter::Ready_Object();
	m_szBodyBoneName;
	m_szWeaponBoneName;
	Add_Component();

	return S_OK;
}

_int CChar_Aris::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Aris::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Aris::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Aris::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Aris::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Aris::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Aris::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
	Hit_Target();
	Hit_Target();
	Hit_Target();
	Hit_Target();
}
