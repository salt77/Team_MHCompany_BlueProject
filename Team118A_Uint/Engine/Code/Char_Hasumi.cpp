#include "Char_Hasumi.h"

USING(Engine)

CChar_Hasumi::CChar_Hasumi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Hasumi::CChar_Hasumi(const CChar_Hasumi & rhs)
	: CCharacter(rhs)
{
}

CChar_Hasumi::~CChar_Hasumi()
{
}

CChar_Hasumi * CChar_Hasumi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Hasumi*	pInstance = new CChar_Hasumi(pGraphicDev, eObjTag, L"Hasumi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Hasumi::Free()
{
	CCharacter::Free();
}

void CChar_Hasumi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Hasumi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Hasumi::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Hasumi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Hasumi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Hasumi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Hasumi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Hasumi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Hasumi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Hasumi::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
