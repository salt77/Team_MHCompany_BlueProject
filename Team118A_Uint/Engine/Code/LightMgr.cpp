#include "LightMgr.h"
#include "ProtoMgr.h"
#include "GraphicDev.h"
#include "KeyManager.h"
#include "RenderTargetMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr()
{
	
}

Engine::CLightMgr::~CLightMgr()
{
	Free();
}

// 정보를 받아와서 라이트를 만들어 저장하고, HRESULT로 빛  성공여부를 반환
HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pLight, E_FAIL);
	
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
	
	m_LightList.push_back(pLight);

	m_pLightCameraPos_Ori = pLightInfo->Position;
	m_pLightCameraPos_At = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

void Engine::CLightMgr::Free()
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pDebugTexture);
	Safe_Release(m_pLightTexture);
	Safe_Release(m_pShader);	
}

// 인덱스(기본 0)의 빛정보를 반환한다. 빛이 하나도 없으면 assert 
D3DLIGHT9* Engine::CLightMgr::Get_Light(const _uint& iIndex /*= 0*/)
{
	if (0 > m_iLightTarget || !m_LightList.size())
	{
		// 빛이 하나도 존재하지 않는 상태
		assert(FALSE);
	}

	auto	iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();
}

// 빛이 하나라도 있는지 bool로 반환
_bool CLightMgr::IsLightExist()
{
	if (m_LightList.size()) 
	{
		return TRUE;
	}

	return FALSE;
}

// 현재 라이트 매니저가 메인으로 가지고 있을 빛 정보의 번호를 설정
void CLightMgr::Set_LightTarget(_int iIndex)
{
	if (!m_LightList.size()) 
	{
		// 빛이 하나도 없으면 index = -1 (null or error)
		m_iLightTarget = -1;
		return;
	}
	
	if (iIndex < m_LightList.size())
	{
		// 인덱스가 빛의 개수보다 작다(적합하다)
		m_iLightTarget = iIndex;
		return;
	}
	else
	{
		// 인덱스가 빛의 개수보다 크면 
		m_iLightTarget = -1;
		return;
	}
}

// _vec3로 빛의 위치를 반환 ( direct light의 경우 : 빛의 실제위치가 아닌 빛 계산을 위해 정의된 위치임 )
_vec3 CLightMgr::Get_LightPos()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	
	return pLightInfo->Position;
}

// _vec3로 빛의 위치를 변경 ( direct light의 경우 : 빛의 실제위치가 아닌 빛 계산을 위해 정의된 위치임 )
void CLightMgr::Set_LightPos(_vec3 vPos)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Position = vPos;
}

// _vec3로 빛이 비추는 방향을 반환
_vec3 CLightMgr::Get_LightDir()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	
	return pLightInfo->Direction;
}

// _vec3로 빛이 비추는 방향을 변경
void CLightMgr::Set_LightDir(_vec3 vDir)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Direction = vDir;
}

// D3DLIGHTTYPE로 빛의 타임을 반환 (direct, point)
D3DLIGHTTYPE CLightMgr::Get_LightType()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Type;
}

// D3DLIGHTTYPE로 빛의 타임을 변경 (direct, point)
void CLightMgr::Set_LightType(D3DLIGHTTYPE eLightType)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Type = eLightType;
}

// D3DXCOLOR로 현재 빛의 Diffuse 색을 반환
D3DXCOLOR CLightMgr::Get_LightDiffuse()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Diffuse;
}

// D3DXCOLOR로 현재 빛의 Diffuse 색을 변경
void CLightMgr::Set_LightDiffuse(D3DXCOLOR Diffuse)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Diffuse = Diffuse;
}

// D3DXCOLOR로 현재 빛의 Specular 색을 반환
D3DXCOLOR CLightMgr::Get_LightSpecular()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Specular;
}

// D3DXCOLOR로 현재 빛의 Specular 색을 변경
void CLightMgr::Set_LightSpecular(D3DXCOLOR Specular)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Specular = Specular;
}

// D3DXCOLOR로 현재 빛의 Ambient 색을 반환
D3DXCOLOR CLightMgr::Get_LightAmbient()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Ambient;
}

// D3DXCOLOR로 현재 빛의 Ambient 색을 변경
void CLightMgr::Set_LightAmbient(D3DXCOLOR Ambient)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Ambient = Ambient;
}

// D3DXCOLOR로 백버퍼의 색을 반환
D3DXCOLOR CLightMgr::Get_LightBackBufferColor()
{
	return m_vBackBufferColor;
}

// D3DXCOLOR로 백버퍼의 색을 변경
void CLightMgr::Set_LightBackBufferColor(D3DXCOLOR BackBufferColor)
{
	m_vBackBufferColor = BackBufferColor;
}

// 빛에서 빛의 방향으로 fDis 만큼 떨어진 점을 구한다.
_vec3 CLightMgr::Get_LightTargetPos(_float fDis)
{
	// 빛정보
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	_vec3 vPos_Eye = pLightInfo->Position;
	_vec3 vPos_Normal_Dir;
	D3DXVec3Normalize(&vPos_Normal_Dir, &_vec3(pLightInfo->Direction));

	return vPos_Eye + vPos_Normal_Dir * fDis;
}

// vPos에서 fDis 만큼 떨어진 점에 빛을 위치한다.
void CLightMgr::Set_LightTargetPos(_vec3 vPos, _float fDis)
{
	// 빛정보
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	_vec3 vPos_Normal_Dir;
	D3DXVec3Normalize(&vPos_Normal_Dir, &_vec3(pLightInfo->Direction));

	pLightInfo->Position = vPos - vPos_Normal_Dir * fDis;	
}

void CLightMgr::Set_LightDis(_float fDis)
{
	_float fDistanceDif = fDis - m_fLightCameraDis;

	m_fLightCameraDis = fDis;
	
	Apply_LightDis(fDistanceDif);
}

void CLightMgr::Add_LightDis(_float fDis)
{
	m_fLightCameraDis += fDis;

	Apply_LightDis(fDis);
}

void CLightMgr::Apply_LightDis(_float fDis)
{
	// 이동 속도
	_float fSpeed = 1.f;

	// 이동 방향
	_vec3 vLighMoveDir = Get_Light(m_iLightTarget)->Direction;
	D3DXVec3Normalize(&vLighMoveDir, &vLighMoveDir);

	// 정도 설정 (이동 방향은 거리이므로 빛이 향하는 방향과 반대)
	vLighMoveDir *= fDis * fSpeed * -1.f;


	D3DVECTOR& LighthtPos = Get_Light(m_iLightTarget)->Position;
	LighthtPos = (_vec3)LighthtPos + vLighMoveDir;
}

_int CLightMgr::Create_GodLight_New(_vec3 vPos, _vec4 vColor, _float fSize)
{
	_int iSize = (_int)m_listGodLight.size();

	T_GodLight tGodLight;
	tGodLight.vPosition = vPos;
	tGodLight.vColor = vColor;
	tGodLight.fSize = fSize;

	m_listGodLight.emplace_back(tGodLight);

	return iSize;
}

void CLightMgr::Set_GodLight_Index_Main(_int iIndex)
{
	if (iIndex >= m_listGodLight.size()) 
	{
		return;
	}

	m_iGodLightTarget = iIndex;
}

T_GodLight* CLightMgr::Get_GodLight_Info(_int iTargetIndex)
{
	if (-1 == iTargetIndex) 
	{
		iTargetIndex = m_iGodLightTarget;
	}

	if (0 > iTargetIndex || iTargetIndex >= m_listGodLight.size())
	{
		iTargetIndex = 0;
	}

	Check_GodLight_Main();

	// 인덱스 번호를 가진 정보를 찾는다.
	auto iter = m_listGodLight.begin();
	for (_int i = 0; i < iTargetIndex; i++)
	{
		iter++;
	}

	T_GodLight* pGodLight = &(*iter);
	
	return pGodLight;
}

void CLightMgr::Set_GodLight_Info(_int iTargetIndex, T_GodLight tInfo)
{
	if (0 > iTargetIndex || iTargetIndex >= m_listGodLight.size())
	{
		iTargetIndex = 0;
	}

	Check_GodLight_Main();

	// 인덱스 번호를 가진 정보를 찾는다.
	auto iter = m_listGodLight.begin();
	for (_int i = 0; i < iTargetIndex; i++)
	{
		iter++;
	}

	(*iter) = tInfo;
}

void CLightMgr::Reset_GodLight_List()
{
	m_iGodLightTarget = 0;

	m_listGodLight.clear();

	Check_GodLight_Main();
}

void CLightMgr::Check_GodLight_Main()
{
	if (0 >= m_listGodLight.size())
	{
		m_listGodLight.emplace_back(T_GodLight());
	}

	T_GodLight& tGodLight = *(m_listGodLight.begin());

	tGodLight.vPosition = Get_Light(m_iLightTarget)->Position;

	D3DXCOLOR vColor = Get_Light(m_iLightTarget)->Diffuse;
	tGodLight.vColor = _vec4(vColor.r, vColor.g, vColor.b, vColor.a);

	_float fCamDis = D3DXVec3Length(&(CCamera::m_pMainCam->Get_CamEye() - tGodLight.vPosition)) * 0.1f;
	tGodLight.fSize = 1.f * fCamDis;

	tGodLight.bSun = TRUE;
	
}

// _matrix로 빛의 뷰 행렬을 반환 (빛의 좌표와 방향으로 계산)
_matrix CLightMgr::Get_LightView(const _uint& iIndex)
{
	// 빛정보
	D3DLIGHT9 tLightInfo = *Get_Light(iIndex);
	
	// 빛 Eye
	_vec3 vPos_Eye = tLightInfo.Position;

	// 빛 At
	_float fX_At = vPos_Eye.x + tLightInfo.Direction.x;
	_float fY_At = vPos_Eye.y + tLightInfo.Direction.y;
	_float fZ_At = vPos_Eye.z + tLightInfo.Direction.z;

	_vec3 vPos_At = _vec3(fX_At, fY_At, fZ_At);

	// 빛의 뷰 행렬
	_matrix matLightView;
	D3DXMatrixLookAtLH(&matLightView, &vPos_Eye, &vPos_At, &VEC_UP);
	
	return matLightView;
}

// _matrix로 빛의 투영 행렬을 반환 (카메라와 동일한 설정으로 투영 행렬을 계산했음)
_matrix CLightMgr::Get_LightProg()
{
	_matrix matLightProj;

	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DXToRadian(60.f), WINCX / (_float)WINCY, 0.1f, 1000.f);
	
	return matLightProj;
}

// 리스트를 순회하며 빛을 랜더링한다.
void Engine::CLightMgr::Render_Light(LPD3DXEFFECT& pEffect)
{
	for (auto& iter : m_LightList)
		iter->Render_Light(pEffect);
}

void CLightMgr::Update_Light_Mgr(const _float & fTimeDelta)
{
	if (CKeyManager::GetInstance()->Key_Down(KEY_Z))
	{
		//m_bLightMove = !m_bLightMove;
	}	

	///////////////////////////////////////////////////////////////////////////////////////
	
	if (m_bLightMove)
	{
		//_vec3 vCameraPos_Old = m_pLightCameraPos;
		//_vec3 vCameraPos_Cur = CCamera::m_pMainCam->Get_CamEye();
		//_vec3 vCameraMovePos = vCameraPos_Cur - vCameraPos_Old;
		//
		////_vec3 vMovePos = _vec3(vCameraMovePos.x, vCameraMovePos.y, vCameraMovePos.z);
		//
		//
		////LighthtPos = (_vec3)LighthtPos + vCameraMovePos;
		//
		//m_pLightCameraPos_Ori = m_pLightCameraPos_Ori + vCameraMovePos;

		// 위치 변경
		D3DVECTOR& LighthtPos = Get_Light(m_iLightTarget)->Position;
		LighthtPos = m_pLightCameraPos_Ori + CCamera::m_pMainCam->Get_CamAt();
	}

	//m_pLightCameraPos = CCamera::m_pMainCam->Get_CamEye();
	//m_pLightCameraPos_At = CCamera::m_pMainCam->Get_CamAt();



	//if (m_bLightMove) 
	//{
	//	_vec3 vCameraPos_Old	= m_pLightCameraPos;
	//	_vec3 vCameraPos_Cur	= CCamera::m_pMainCam->Get_CamEye();	
	//	_vec3 vCameraMovePos	= vCameraPos_Cur - vCameraPos_Old;
	//	
	//	//_vec3 vMovePos = _vec3(vCameraMovePos.x, vCameraMovePos.y, vCameraMovePos.z);
	//	
	//	D3DVECTOR& LighthtPos = Get_Light(m_iLightTarget)->Position;
	//
	//	//LighthtPos = (_vec3)LighthtPos + vCameraMovePos;
	//
	//	m_pLightCameraPos_Ori = m_pLightCameraPos_Ori + vCameraMovePos;
	//
	//	LighthtPos = m_pLightCameraPos_Ori + vCameraMovePos + m_pLightCameraPos_At;
	//}
	//
	//m_pLightCameraPos		= CCamera::m_pMainCam->Get_CamEye();
	//m_pLightCameraPos_At	= CCamera::m_pMainCam->Get_CamAt();	

	// God Ray 체크 및 업데이트
	Check_GodLight_Main();

}

void CLightMgr::Render_Light_Mgr()
{		
#pragma region 초기 설정
	if (!m_pGraphicDev)
	{
		m_pGraphicDev = CGraphicDev::GetInstance()->GetDevice();
	}

	if (!m_pBuffer)
	{
		m_pBuffer = static_cast<CRectBuffer*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Buffer_RectTex"));
	}

	if (!m_pDebugTexture)
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/LisghtMgr_Sun.png", &m_pDebugTexture);
	}

	if (!m_pLightTexture)
	{
		CProtoMgr::GetInstance()->Get_Texture(m_pGraphicDev, L"../../Reference/Resource/Effect/LisghtMgr_Light.png", &m_pLightTexture);
	}

	if (!m_pShader)
	{
		m_pShader = static_cast<CShader*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Shader_LightMgr"));
	}

	if (!m_pGraphicDev || !m_pBuffer || !m_pDebugTexture || !m_pShader)
	{
		return;
	}
#pragma endregion 
	
	LPD3DXEFFECT pEffect = m_pShader->Get_EffectHandle();
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 행렬 계산

	_matrix	matView, matInverView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixIdentity(&matInverView);
	matInverView = matView;
	matInverView._41 = 0;
	matInverView._42 = 0;
	matInverView._43 = 0;
	D3DXMatrixInverse(&matInverView, NULL, &matInverView);	
	
	T_GodLight tGodLight = *Get_GodLight_Info(m_iGodLightTarget);

	_matrix matTrans;
	_vec3 vPos = tGodLight.vPosition;
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
		
	_matrix matScale;	
	D3DXMatrixScaling(&matScale, tGodLight.fSize, tGodLight.fSize, tGodLight.fSize);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////

	_matrix matWolrd = matScale * matInverView * matTrans *  matView * matProj;
	pEffect->SetMatrix("g_matWorld", &matWolrd);
	pEffect->SetVector("g_vColor", &tGodLight.vColor);

	if (m_pLightTexture)
	{
		pEffect->SetTexture("g_BaseTexture", m_pLightTexture);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	m_pShader->BeginShader();

	// 태양o -> Z를 계산하지 않고 그린다. -> 무조건 지형으로 가린다.		( 이유, 그림자 때문에 태양의 역활을 하는 빛 좌표가 지형보다 앞에 있을 수 있으므로 )
	// 태양x -> Z를 계산하고 그린다.		-> 무조건 지형으로 가리지 않는다. ( 빛의 위치와 지형을 계산 -> Light 처럼 사용 )
	
	//_int iPass = tGodLight.bSun ? 1 : 0;
	//m_pShader->BeginPass(iPass);

	m_pShader->BeginPass(0);

	m_pBuffer->Render_Buffer();

	m_pShader->EndPass();
	m_pShader->EndShader();
}

