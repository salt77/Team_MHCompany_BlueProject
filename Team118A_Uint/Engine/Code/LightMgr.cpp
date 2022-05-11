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

// ������ �޾ƿͼ� ����Ʈ�� ����� �����ϰ�, HRESULT�� ��  �������θ� ��ȯ
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

// �ε���(�⺻ 0)�� �������� ��ȯ�Ѵ�. ���� �ϳ��� ������ assert 
D3DLIGHT9* Engine::CLightMgr::Get_Light(const _uint& iIndex /*= 0*/)
{
	if (0 > m_iLightTarget || !m_LightList.size())
	{
		// ���� �ϳ��� �������� �ʴ� ����
		assert(FALSE);
	}

	auto	iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Light();
}

// ���� �ϳ��� �ִ��� bool�� ��ȯ
_bool CLightMgr::IsLightExist()
{
	if (m_LightList.size()) 
	{
		return TRUE;
	}

	return FALSE;
}

// ���� ����Ʈ �Ŵ����� �������� ������ ���� �� ������ ��ȣ�� ����
void CLightMgr::Set_LightTarget(_int iIndex)
{
	if (!m_LightList.size()) 
	{
		// ���� �ϳ��� ������ index = -1 (null or error)
		m_iLightTarget = -1;
		return;
	}
	
	if (iIndex < m_LightList.size())
	{
		// �ε����� ���� �������� �۴�(�����ϴ�)
		m_iLightTarget = iIndex;
		return;
	}
	else
	{
		// �ε����� ���� �������� ũ�� 
		m_iLightTarget = -1;
		return;
	}
}

// _vec3�� ���� ��ġ�� ��ȯ ( direct light�� ��� : ���� ������ġ�� �ƴ� �� ����� ���� ���ǵ� ��ġ�� )
_vec3 CLightMgr::Get_LightPos()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	
	return pLightInfo->Position;
}

// _vec3�� ���� ��ġ�� ���� ( direct light�� ��� : ���� ������ġ�� �ƴ� �� ����� ���� ���ǵ� ��ġ�� )
void CLightMgr::Set_LightPos(_vec3 vPos)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Position = vPos;
}

// _vec3�� ���� ���ߴ� ������ ��ȯ
_vec3 CLightMgr::Get_LightDir()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	
	return pLightInfo->Direction;
}

// _vec3�� ���� ���ߴ� ������ ����
void CLightMgr::Set_LightDir(_vec3 vDir)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Direction = vDir;
}

// D3DLIGHTTYPE�� ���� Ÿ���� ��ȯ (direct, point)
D3DLIGHTTYPE CLightMgr::Get_LightType()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Type;
}

// D3DLIGHTTYPE�� ���� Ÿ���� ���� (direct, point)
void CLightMgr::Set_LightType(D3DLIGHTTYPE eLightType)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Type = eLightType;
}

// D3DXCOLOR�� ���� ���� Diffuse ���� ��ȯ
D3DXCOLOR CLightMgr::Get_LightDiffuse()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Diffuse;
}

// D3DXCOLOR�� ���� ���� Diffuse ���� ����
void CLightMgr::Set_LightDiffuse(D3DXCOLOR Diffuse)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Diffuse = Diffuse;
}

// D3DXCOLOR�� ���� ���� Specular ���� ��ȯ
D3DXCOLOR CLightMgr::Get_LightSpecular()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Specular;
}

// D3DXCOLOR�� ���� ���� Specular ���� ����
void CLightMgr::Set_LightSpecular(D3DXCOLOR Specular)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Specular = Specular;
}

// D3DXCOLOR�� ���� ���� Ambient ���� ��ȯ
D3DXCOLOR CLightMgr::Get_LightAmbient()
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	return pLightInfo->Ambient;
}

// D3DXCOLOR�� ���� ���� Ambient ���� ����
void CLightMgr::Set_LightAmbient(D3DXCOLOR Ambient)
{
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);

	pLightInfo->Ambient = Ambient;
}

// D3DXCOLOR�� ������� ���� ��ȯ
D3DXCOLOR CLightMgr::Get_LightBackBufferColor()
{
	return m_vBackBufferColor;
}

// D3DXCOLOR�� ������� ���� ����
void CLightMgr::Set_LightBackBufferColor(D3DXCOLOR BackBufferColor)
{
	m_vBackBufferColor = BackBufferColor;
}

// ������ ���� �������� fDis ��ŭ ������ ���� ���Ѵ�.
_vec3 CLightMgr::Get_LightTargetPos(_float fDis)
{
	// ������
	D3DLIGHT9* pLightInfo = Get_Light(m_iLightTarget);
	_vec3 vPos_Eye = pLightInfo->Position;
	_vec3 vPos_Normal_Dir;
	D3DXVec3Normalize(&vPos_Normal_Dir, &_vec3(pLightInfo->Direction));

	return vPos_Eye + vPos_Normal_Dir * fDis;
}

// vPos���� fDis ��ŭ ������ ���� ���� ��ġ�Ѵ�.
void CLightMgr::Set_LightTargetPos(_vec3 vPos, _float fDis)
{
	// ������
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
	// �̵� �ӵ�
	_float fSpeed = 1.f;

	// �̵� ����
	_vec3 vLighMoveDir = Get_Light(m_iLightTarget)->Direction;
	D3DXVec3Normalize(&vLighMoveDir, &vLighMoveDir);

	// ���� ���� (�̵� ������ �Ÿ��̹Ƿ� ���� ���ϴ� ����� �ݴ�)
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

	// �ε��� ��ȣ�� ���� ������ ã�´�.
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

	// �ε��� ��ȣ�� ���� ������ ã�´�.
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

// _matrix�� ���� �� ����� ��ȯ (���� ��ǥ�� �������� ���)
_matrix CLightMgr::Get_LightView(const _uint& iIndex)
{
	// ������
	D3DLIGHT9 tLightInfo = *Get_Light(iIndex);
	
	// �� Eye
	_vec3 vPos_Eye = tLightInfo.Position;

	// �� At
	_float fX_At = vPos_Eye.x + tLightInfo.Direction.x;
	_float fY_At = vPos_Eye.y + tLightInfo.Direction.y;
	_float fZ_At = vPos_Eye.z + tLightInfo.Direction.z;

	_vec3 vPos_At = _vec3(fX_At, fY_At, fZ_At);

	// ���� �� ���
	_matrix matLightView;
	D3DXMatrixLookAtLH(&matLightView, &vPos_Eye, &vPos_At, &VEC_UP);
	
	return matLightView;
}

// _matrix�� ���� ���� ����� ��ȯ (ī�޶�� ������ �������� ���� ����� �������)
_matrix CLightMgr::Get_LightProg()
{
	_matrix matLightProj;

	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DXToRadian(60.f), WINCX / (_float)WINCY, 0.1f, 1000.f);
	
	return matLightProj;
}

// ����Ʈ�� ��ȸ�ϸ� ���� �������Ѵ�.
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

		// ��ġ ����
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

	// God Ray üũ �� ������Ʈ
	Check_GodLight_Main();

}

void CLightMgr::Render_Light_Mgr()
{		
#pragma region �ʱ� ����
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
	// ��� ���

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

	// �¾�o -> Z�� ������� �ʰ� �׸���. -> ������ �������� ������.		( ����, �׸��� ������ �¾��� ��Ȱ�� �ϴ� �� ��ǥ�� �������� �տ� ���� �� �����Ƿ� )
	// �¾�x -> Z�� ����ϰ� �׸���.		-> ������ �������� ������ �ʴ´�. ( ���� ��ġ�� ������ ��� -> Light ó�� ��� )
	
	//_int iPass = tGodLight.bSun ? 1 : 0;
	//m_pShader->BeginPass(iPass);

	m_pShader->BeginPass(0);

	m_pBuffer->Render_Buffer();

	m_pShader->EndPass();
	m_pShader->EndShader();
}

