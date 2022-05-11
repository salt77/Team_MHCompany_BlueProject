#include "Char_Chise.h"

USING(Engine)

CChar_Chise::CChar_Chise(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Chise::CChar_Chise(const CChar_Chise & rhs)
	: CCharacter(rhs)
{
}

CChar_Chise::~CChar_Chise()
{
}

CChar_Chise * CChar_Chise::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Chise*	pInstance = new CChar_Chise(pGraphicDev, eObjTag, L"Chise");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Chise::Free()
{
	CCharacter::Free();
}

void CChar_Chise::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Chise::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Chise::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Chise::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Chise::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Chise::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Chise::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Chise::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Chise::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Chise::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
