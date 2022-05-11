#include "Char_Nodoka.h"

USING(Engine)

CChar_Nodoka::CChar_Nodoka(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Nodoka::CChar_Nodoka(const CChar_Nodoka & rhs)
	: CCharacter(rhs)
{
}

CChar_Nodoka::~CChar_Nodoka()
{
}

CChar_Nodoka * CChar_Nodoka::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Nodoka*	pInstance = new CChar_Nodoka(pGraphicDev, eObjTag, L"Nodoka");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Nodoka::Free()
{
	CCharacter::Free();
}

void CChar_Nodoka::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Nodoka::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Nodoka::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Nodoka::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Nodoka::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Nodoka::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Nodoka::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Nodoka::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Nodoka::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Nodoka::Trigger_Attack_1()
{
	Hit_Target();
}
