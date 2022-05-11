#include "AttackObj_Monster.h"


CAttackObj_Monster::CAttackObj_Monster(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_ATKOBJ_TYPE eAtkObjType)
	: CAttackObj(pGraphicDev, eObjTag, eAtkObjType)
{
}

CAttackObj_Monster::CAttackObj_Monster(const CAttackObj_Monster & rhs)
	: CAttackObj(rhs)
{
}

CAttackObj_Monster::~CAttackObj_Monster()
{
}

void CAttackObj_Monster::Free()
{
	CAttackObj::Free();
}

void CAttackObj_Monster::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte);
}

void CAttackObj_Monster::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte);
}


HRESULT CAttackObj_Monster::Ready_Object()
{
	CAttackObj::Ready_Object();

	return S_OK;
}

_int CAttackObj_Monster::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta);

	return iExit;
}

_int CAttackObj_Monster::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	return iExit;
}

_int CAttackObj_Monster::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CAttackObj_Monster::Render_Object(_int iPass)
{
	CAttackObj::Render_Object(iPass);
}

void CAttackObj_Monster::Debug_This_In_Console()
{
}

void CAttackObj_Monster::Set_Constantable()
{
}

void CAttackObj_Monster::OnEnable()
{
	CAttackObj::OnEnable();
}

void CAttackObj_Monster::OnDisable()
{
	CAttackObj::OnDisable();
}

void CAttackObj_Monster::OnDestroy()
{
	CAttackObj::OnDestroy();
}

void CAttackObj_Monster::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest);
}
