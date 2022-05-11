#include "Char_Kayoko.h"

USING(Engine)

CChar_Kayoko::CChar_Kayoko(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Kayoko::CChar_Kayoko(const CChar_Kayoko & rhs)
	: CCharacter(rhs)
{
}

CChar_Kayoko::~CChar_Kayoko()
{
}

CChar_Kayoko * CChar_Kayoko::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Kayoko*	pInstance = new CChar_Kayoko(pGraphicDev, eObjTag, L"Kayoko");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Kayoko::Free()
{
	CCharacter::Free();
}

void CChar_Kayoko::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Kayoko::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Kayoko::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Kayoko::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Kayoko::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Kayoko::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Kayoko::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Kayoko::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Kayoko::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Kayoko::Trigger_Attack_1()
{
	Hit_Target();
}
