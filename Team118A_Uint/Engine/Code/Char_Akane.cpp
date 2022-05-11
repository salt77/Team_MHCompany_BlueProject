#include "Char_Akane.h"

USING(Engine)

CChar_Akane::CChar_Akane(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Akane::CChar_Akane(const CChar_Akane & rhs)
	: CCharacter(rhs)
{
}

CChar_Akane::~CChar_Akane()
{
}

CChar_Akane * CChar_Akane::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Akane*	pInstance = new CChar_Akane(pGraphicDev, eObjTag, L"Akane");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Akane::Free()
{
	CCharacter::Free();
}

void CChar_Akane::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Akane::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Akane::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Akane::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Akane::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Akane::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Akane::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Akane::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Akane::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Akane::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
