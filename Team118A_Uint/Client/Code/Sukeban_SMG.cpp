#include "stdafx.h"
#include "../Include/stdafx.h"
#include "Sukeban_SMG.h"
#include "UI_Damage.h"

//생성할때 자신의 태그를 입력해서 자신의 메쉬를 불러오도록 함
CSukeban_SMG::CSukeban_SMG(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
	:CMonster(pGraphicDev,eObjTag, L"Sukeban_SMG")
{
}

CSukeban_SMG::CSukeban_SMG(const CSukeban_SMG & rhs)
	:CMonster(rhs)
{
}

CSukeban_SMG::~CSukeban_SMG()
{
}

CSukeban_SMG * CSukeban_SMG::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CSukeban_SMG* pInstance = new CSukeban_SMG(pGraphicDev, eObjTag);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSukeban_SMG::Free()
{
	CMonster::Free();
}

// 아직은 전부 깡통
HRESULT CSukeban_SMG::Ready_Object()
{
	CMonster::Ready_Object();
	return S_OK;
}

_int CSukeban_SMG::Start_Object(const _float & fTimeDelta)
{
	CMonster::Start_Object(fTimeDelta);
	return RETURN_OK;
}

_int CSukeban_SMG::Update_Object(const _float & fTimeDelta)
{
	CMonster::Update_Object(fTimeDelta);

	return RETURN_OK;
}

_int CSukeban_SMG::LateUpdate_Object(const _float & fTimeDelta)
{
	CMonster::LateUpdate_Object(fTimeDelta);
	return RETURN_OK;
}

void CSukeban_SMG::Set_Constantable()
{
	CMonster::Set_Constantable();
}

void CSukeban_SMG::OnEnable()
{
	CMonster::OnEnable();
}

void CSukeban_SMG::OnDisable()
{
	CMonster::OnDisable();
}

void CSukeban_SMG::OnDestroy()
{
	CMonster::OnDestroy();
}
