#include "Char_Azusa.h"

USING(Engine)

CChar_Azusa::CChar_Azusa(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Azusa::CChar_Azusa(const CChar_Azusa & rhs)
	: CCharacter(rhs)
{
}

CChar_Azusa::~CChar_Azusa()
{
}

CChar_Azusa * CChar_Azusa::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Azusa*	pInstance = new CChar_Azusa(pGraphicDev, eObjTag, L"Azusa");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Azusa::Free()
{
	CCharacter::Free();
}

void CChar_Azusa::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Azusa::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Azusa::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Azusa::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Azusa::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Azusa::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Azusa::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Azusa::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Azusa::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Azusa::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
