#include "Char_Juri.h"

USING(Engine)

CChar_Juri::CChar_Juri(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Juri::CChar_Juri(const CChar_Juri & rhs)
	: CCharacter(rhs)
{
}

CChar_Juri::~CChar_Juri()
{
}

CChar_Juri * CChar_Juri::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Juri*	pInstance = new CChar_Juri(pGraphicDev, eObjTag, L"Juri");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Juri::Free()
{
	CCharacter::Free();
}

void CChar_Juri::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Juri::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Juri::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Juri::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Juri::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Juri::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Juri::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Juri::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Juri::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Juri::Trigger_Attack_1()
{
	Hit_Target();
}
