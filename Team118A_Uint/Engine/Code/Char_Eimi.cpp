#include "Char_Eimi.h"

USING(Engine)

CChar_Eimi::CChar_Eimi(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Eimi::CChar_Eimi(const CChar_Eimi & rhs)
	: CCharacter(rhs)
{
}

CChar_Eimi::~CChar_Eimi()
{
}

CChar_Eimi * CChar_Eimi::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Eimi*	pInstance = new CChar_Eimi(pGraphicDev, eObjTag, L"Eimi");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Eimi::Free()
{
	CCharacter::Free();
}

void CChar_Eimi::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Eimi::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Eimi::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Eimi::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Eimi::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Eimi::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Eimi::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Eimi::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Eimi::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Eimi::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
