#include "Char_Iori.h"

USING(Engine)

CChar_Iori::CChar_Iori(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Iori::CChar_Iori(const CChar_Iori & rhs)
	: CCharacter(rhs)
{
}

CChar_Iori::~CChar_Iori()
{
}

CChar_Iori * CChar_Iori::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Iori*	pInstance = new CChar_Iori(pGraphicDev, eObjTag, L"Iori");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Iori::Free()
{
	CCharacter::Free();
}

void CChar_Iori::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Iori::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Iori::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Iori::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Iori::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Iori::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Iori::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Iori::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Iori::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Iori::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
