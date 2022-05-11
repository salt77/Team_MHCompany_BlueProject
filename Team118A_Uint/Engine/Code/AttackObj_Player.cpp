#include "AttackObj_Player.h"


CAttackObj_Player::CAttackObj_Player(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, E_ATKOBJ_TYPE eAtkObjType)
	: CAttackObj(pGraphicDev, eObjTag, eAtkObjType)
{
}

CAttackObj_Player::CAttackObj_Player(const CAttackObj_Player & rhs)
	: CAttackObj(rhs)
{
}

CAttackObj_Player::~CAttackObj_Player()
{
}

void CAttackObj_Player::Free()
{
	CAttackObj::Free(); 
}

void CAttackObj_Player::Save(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Save(hFile, dwByte); 
}

void CAttackObj_Player::Load(HANDLE hFile, DWORD & dwByte)
{
	CAttackObj::Load(hFile, dwByte); 
}


HRESULT CAttackObj_Player::Ready_Object()
{
	CAttackObj::Ready_Object(); 

	return S_OK;
}

_int CAttackObj_Player::Start_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Start_Object(fTimeDelta); 

	return iExit;
}

_int CAttackObj_Player::Update_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::Update_Object(fTimeDelta);

	return iExit;
}

_int CAttackObj_Player::LateUpdate_Object(const _float & fTimeDelta)
{
	_int	iExit = CAttackObj::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CAttackObj_Player::Render_Object(_int iPass)
{
	CAttackObj::Render_Object(iPass);
}

void CAttackObj_Player::Debug_This_In_Console()
{
}

void CAttackObj_Player::Set_Constantable()
{
}

void CAttackObj_Player::OnEnable()
{
	CAttackObj::OnEnable(); 
}

void CAttackObj_Player::OnDisable()
{
	CAttackObj::OnDisable(); 
}

void CAttackObj_Player::OnDestroy()
{
	CAttackObj::OnDestroy(); 
}

void CAttackObj_Player::On_Collision(CGameObject * pDest)
{
	CAttackObj::On_Collision(pDest); 
}
