#include "Char_Hibiki.h"

USING(Engine)

CChar_Hibiki::CChar_Hibiki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Hibiki::CChar_Hibiki(const CChar_Hibiki & rhs)
	: CCharacter(rhs)
{
}

CChar_Hibiki::~CChar_Hibiki()
{
}

CChar_Hibiki * CChar_Hibiki::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Hibiki*	pInstance = new CChar_Hibiki(pGraphicDev, eObjTag, L"Hibiki");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Hibiki::Free()
{
	CCharacter::Free();
}

void CChar_Hibiki::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Hibiki::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Hibiki::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Hibiki::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Hibiki::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Hibiki::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Hibiki::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Hibiki::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Hibiki::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Hibiki::Trigger_Attack_1()
{
	Hit_Target();
}
