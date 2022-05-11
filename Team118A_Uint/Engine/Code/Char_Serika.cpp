#include "Char_Serika.h"

USING(Engine)

CChar_Serika::CChar_Serika(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Serika::CChar_Serika(const CChar_Serika & rhs)
	: CCharacter(rhs)
{
}

CChar_Serika::~CChar_Serika()
{
}

CChar_Serika * CChar_Serika::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Serika*	pInstance = new CChar_Serika(pGraphicDev, eObjTag, L"Serika");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Serika::Free()
{
	CCharacter::Free();
}

void CChar_Serika::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Serika::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Serika::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Serika::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Serika::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Serika::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Serika::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Serika::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Serika::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Serika::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
