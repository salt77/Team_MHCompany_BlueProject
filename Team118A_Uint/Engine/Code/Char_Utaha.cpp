#include "Char_Utaha.h"

USING(Engine)

CChar_Utaha::CChar_Utaha(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Utaha::CChar_Utaha(const CChar_Utaha & rhs)
	: CCharacter(rhs)
{
}

CChar_Utaha::~CChar_Utaha()
{
}

CChar_Utaha * CChar_Utaha::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Utaha*	pInstance = new CChar_Utaha(pGraphicDev, eObjTag, L"Utaha");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Utaha::Free()
{
	CCharacter::Free();
}

void CChar_Utaha::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Utaha::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Utaha::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Utaha::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Utaha::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Utaha::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Utaha::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Utaha::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Utaha::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Utaha::Trigger_Attack_1()
{
	Hit_Target();

}
