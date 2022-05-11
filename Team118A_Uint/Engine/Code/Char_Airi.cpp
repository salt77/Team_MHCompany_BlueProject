#include "Char_Airi.h"

USING(Engine)

CChar_Airi::CChar_Airi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Airi::CChar_Airi(const CChar_Airi & rhs)
	: CCharacter(rhs)
{
}

CChar_Airi::~CChar_Airi()
{
}

CChar_Airi * CChar_Airi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Airi*	pInstance = new CChar_Airi(pGraphicDev, eObjTag, L"Airi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Airi::Free()
{
	CCharacter::Free();
}

void CChar_Airi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Airi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Airi::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Airi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Airi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Airi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Airi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Airi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Airi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Airi::Trigger_Attack_1()
{
	Hit_Target();
}
