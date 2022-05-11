#include "Char_Yoshimi.h"

USING(Engine)

CChar_Yoshimi::CChar_Yoshimi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Yoshimi::CChar_Yoshimi(const CChar_Yoshimi & rhs)
	: CCharacter(rhs)
{
}

CChar_Yoshimi::~CChar_Yoshimi()
{
}

CChar_Yoshimi * CChar_Yoshimi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Yoshimi*	pInstance = new CChar_Yoshimi(pGraphicDev, eObjTag, L"Yoshimi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Yoshimi::Free()
{
	CCharacter::Free();
}

void CChar_Yoshimi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Yoshimi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Yoshimi::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Yoshimi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Yoshimi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Yoshimi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Yoshimi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Yoshimi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Yoshimi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Yoshimi::Trigger_Attack_1()
{
	Hit_Target();
}
