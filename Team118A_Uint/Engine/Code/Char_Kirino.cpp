#include "Char_Kirino.h"

USING(Engine)

CChar_Kirino::CChar_Kirino(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Kirino::CChar_Kirino(const CChar_Kirino & rhs)
	: CCharacter(rhs)
{
}

CChar_Kirino::~CChar_Kirino()
{
}

CChar_Kirino * CChar_Kirino::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Kirino*	pInstance = new CChar_Kirino(pGraphicDev, eObjTag, L"Kirino");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Kirino::Free()
{
	CCharacter::Free();
}

void CChar_Kirino::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Kirino::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Kirino::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Kirino::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Kirino::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Kirino::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Kirino::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Kirino::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Kirino::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Kirino::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
