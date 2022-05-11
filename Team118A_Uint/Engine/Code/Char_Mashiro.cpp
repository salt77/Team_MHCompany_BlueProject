#include "Char_Mashiro.h"

USING(Engine)

CChar_Mashiro::CChar_Mashiro(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Mashiro::CChar_Mashiro(const CChar_Mashiro & rhs)
	: CCharacter(rhs)
{
}

CChar_Mashiro::~CChar_Mashiro()
{
}

CChar_Mashiro * CChar_Mashiro::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Mashiro*	pInstance = new CChar_Mashiro(pGraphicDev, eObjTag, L"Mashiro");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Mashiro::Free()
{
	CCharacter::Free();
}

void CChar_Mashiro::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Mashiro::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Mashiro::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Mashiro::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Mashiro::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Mashiro::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Mashiro::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Mashiro::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Mashiro::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Mashiro::Trigger_Attack_1()
{
	Hit_Target();
}
