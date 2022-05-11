#include "Char_Mutsuki.h"

USING(Engine)

CChar_Mutsuki::CChar_Mutsuki(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Mutsuki::CChar_Mutsuki(const CChar_Mutsuki & rhs)
	: CCharacter(rhs)
{
}

CChar_Mutsuki::~CChar_Mutsuki()
{
}

CChar_Mutsuki * CChar_Mutsuki::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Mutsuki*	pInstance = new CChar_Mutsuki(pGraphicDev, eObjTag, L"Mutsuki"); 

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Mutsuki::Free()
{
	CCharacter::Free();
}

void CChar_Mutsuki::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Mutsuki::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Mutsuki::Ready_Object()
{
	CCharacter::Ready_Object();

	Add_Component();

	return S_OK;
}

_int CChar_Mutsuki::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Mutsuki::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Mutsuki::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Mutsuki::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Mutsuki::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Mutsuki::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Mutsuki::Trigger_Attack_1()
{
	CCharacter::Trigger_Attack_1(); 

	Hit_Target(); 

	Create_Effect(L"Test_Minhyun_1231_Fire_00.skill", 0.f, 0.f, 0.6f);
}
