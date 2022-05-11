#include "Char_Karin.h"

USING(Engine)

CChar_Karin::CChar_Karin(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Karin::CChar_Karin(const CChar_Karin & rhs)
	: CCharacter(rhs)
{
}

CChar_Karin::~CChar_Karin()
{
}

CChar_Karin * CChar_Karin::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Karin*	pInstance = new CChar_Karin(pGraphicDev, eObjTag, L"Karin");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Karin::Free()
{
	CCharacter::Free();
}

void CChar_Karin::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Karin::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Karin::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Karin::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Karin::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Karin::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Karin::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Karin::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Karin::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Karin::Trigger_Attack_1()
{
	Hit_Target();
}
