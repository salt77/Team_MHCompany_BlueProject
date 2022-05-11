#include "Char_Izumi.h"

USING(Engine)

CChar_Izumi::CChar_Izumi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Izumi::CChar_Izumi(const CChar_Izumi & rhs)
	: CCharacter(rhs)
{
}

CChar_Izumi::~CChar_Izumi()
{
}

CChar_Izumi * CChar_Izumi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Izumi*	pInstance = new CChar_Izumi(pGraphicDev, eObjTag, L"Izumi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Izumi::Free()
{
	CCharacter::Free();
}

void CChar_Izumi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Izumi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Izumi::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Izumi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Izumi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Izumi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Izumi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Izumi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Izumi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Izumi::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
