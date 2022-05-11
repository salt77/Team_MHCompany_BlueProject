#include "Char_Maki.h"

USING(Engine)

CChar_Maki::CChar_Maki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Maki::CChar_Maki(const CChar_Maki & rhs)
	: CCharacter(rhs)
{
}

CChar_Maki::~CChar_Maki()
{
}

CChar_Maki * CChar_Maki::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Maki*	pInstance = new CChar_Maki(pGraphicDev, eObjTag, L"Maki");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Maki::Free()
{
	CCharacter::Free();
}

void CChar_Maki::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Maki::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Maki::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	return S_OK;
}

_int CChar_Maki::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Maki::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Maki::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Maki::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Maki::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Maki::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Maki::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1();

	Hit_Target();
}
