#include "Char_Chinatsu.h"

USING(Engine)

CChar_Chinatsu::CChar_Chinatsu(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Chinatsu::CChar_Chinatsu(const CChar_Chinatsu & rhs)
	: CCharacter(rhs)
{
}

CChar_Chinatsu::~CChar_Chinatsu()
{
}

CChar_Chinatsu * CChar_Chinatsu::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Chinatsu*	pInstance = new CChar_Chinatsu(pGraphicDev, eObjTag, L"Chinatsu");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Chinatsu::Free()
{
	CCharacter::Free();
}

void CChar_Chinatsu::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Chinatsu::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Chinatsu::Ready_Object()
{
	m_szBodyBoneName = "Bip001_Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";
	CCharacter::Ready_Object();
	Add_Component();

	return S_OK;
}

_int CChar_Chinatsu::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Chinatsu::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Chinatsu::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Chinatsu::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Chinatsu::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Chinatsu::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Chinatsu::Trigger_Attack_1()
{
	Hit_Target();
}
