#include "Char_Yuzu.h"

USING(Engine)

CChar_Yuzu::CChar_Yuzu(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrUnitName)
	: CCharacter(pGraphicDev, eObjTag, wstrUnitName)
{
}

CChar_Yuzu::CChar_Yuzu(const CChar_Yuzu & rhs)
	: CCharacter(rhs)
{
}

CChar_Yuzu::~CChar_Yuzu()
{
}

CChar_Yuzu * CChar_Yuzu::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag)
{
	CChar_Yuzu*	pInstance = new CChar_Yuzu(pGraphicDev, eObjTag, L"Yuzu");

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CChar_Yuzu::Free()
{
	CCharacter::Free();
}

void CChar_Yuzu::Save(HANDLE hFile, DWORD & dwByte)
{
}

void CChar_Yuzu::Load(HANDLE hFile, DWORD & dwByte)
{
}


HRESULT CChar_Yuzu::Ready_Object()
{
	CCharacter::Ready_Object();

	m_szBodyBoneName = "Bip001 Pelvis";
	m_szWeaponBoneName = "Bip001_Weapon";

	Add_Component();

	return S_OK;
}

_int CChar_Yuzu::Start_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Start_Object(fTimeDelta);

	return iExit;
}

_int CChar_Yuzu::Update_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::Update_Object(fTimeDelta);

	return iExit;
}

_int CChar_Yuzu::LateUpdate_Object(const _float & fTimeDelta)
{
	_int iExit = CCharacter::LateUpdate_Object(fTimeDelta);

	return iExit;
}

void CChar_Yuzu::Render_Object(_int iPass)
{
	CCharacter::Render_Object(iPass);
}

void CChar_Yuzu::Set_Constantable()
{
	CCharacter::Set_Constantable();
}

void CChar_Yuzu::Add_Component()
{
	CComponent*		pComponent = nullptr;
}

void CChar_Yuzu::Begin_Battle_Sound()
{
}

void CChar_Yuzu::Trigger_Attack_1()
{
	Hit_Target();

	_vec3 vDirLook = *D3DXVec3Normalize(&vDirLook, &Get_Info(E_INFO::INFO_LOOK));
	_vec3 vDirRight = *D3DXVec3Normalize(&vDirRight, &Get_Info(E_INFO::INFO_RIGHT));
	_vec3 vDirUp = *D3DXVec3Normalize(&vDirUp, &Get_Info(E_INFO::INFO_UP));
	_float fLookRatio = 0.4f;
	_float fRightRatio = 0.0f;
	_float fUpRatio = 0.05f;

	_vec3 vWeaponPos = m_pWeaponCollider->GetPos();
	vWeaponPos += vDirLook * fLookRatio;
	vWeaponPos += vDirRight * fRightRatio;
	vWeaponPos += vDirUp * fUpRatio;

	CGameObject*	pGameObject = CEffectObject::Create(m_pGraphicDev, E_OBJTAG::OBJ_Effect, vWeaponPos, Get_Angle(), L"Shot_Effect_1.effect");
	assert(pGameObject);
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pGameObject);
	static_cast<CEffectObject*>(pGameObject)->Set_Effect_Scale(VEC_ONE);
}
