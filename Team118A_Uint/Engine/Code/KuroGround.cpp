#include "KuroGround.h"
#include "Scene.h"
#include "Renderer.h"
#include "SMesh.h"
#include "ProtoMgr.h"


USING(Engine)


CKuroGround::CKuroGround(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath)
	: CStaticObject(pGraphicDev, eObjTag, wstrSMeshFullPath),
	m_wstrMeshFullPath(wstrSMeshFullPath)
{
}

CKuroGround::CKuroGround(const CKuroGround& rhs)
	: CStaticObject(rhs),
	m_wstrMeshFullPath(rhs.m_wstrMeshFullPath)
{
}

CKuroGround::~CKuroGround()
{
}

CKuroGround * CKuroGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, E_OBJTAG eObjTag, wstring wstrSMeshFullPath)
{
	CKuroGround*	pInstance = new CKuroGround(pGraphicDev, eObjTag, wstrSMeshFullPath);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CKuroGround::Free()
{

	CStaticObject::Free();
}

void CKuroGround::Save(HANDLE hFile, DWORD & dwByte)
{
	CStaticObject::Save(hFile, dwByte);
}

void CKuroGround::Load(HANDLE hFile, DWORD & dwByte)
{
	CStaticObject::Load(hFile, dwByte);
}



_int CKuroGround::Update_Object(const _float& fTimeDelta)
{
	Update_Gravity(fTimeDelta);
	_int iExit = CGameObject::Update_Object(fTimeDelta);
	
	CRenderer::GetInstance()->Add_RenderGroup(E_RENDER_ID::RENDER_NONALPHA, this);

	return iExit;
}

void CKuroGround::Set_GroundDead()
{
	m_bGravityOn = TRUE;
}

void CKuroGround::On_Collision(CGameObject * pDest)
{
}

void CKuroGround::Update_Gravity(_float fTimeDelta)
{
	if (!m_bGravityOn)
	{
		return;
	}


	m_fGravitySpeed += fTimeDelta * m_fGravityAcc;
	Add_Pos(_vec3(0.f, -m_fGravitySpeed, 0.f));
	

	if (Get_Position().y < -50.f)
	{
		m_bDead = TRUE;
	}
}
 
