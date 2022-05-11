#include "Transform.h"

USING(Engine)

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_vAngle(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_wstrComTag = ComTag_Transform;
}

CTransform::CTransform(const CTransform & rhs)
	:CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
{
}

CTransform::~CTransform()
{
}

CComponent * CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}

void CTransform::Save(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_WriteVec3(hFile, m_vInfo[(_int)E_INFO::INFO_POS], dwByte);
	CUtility::Data_WriteVec3(hFile, m_vScale, dwByte);
	CUtility::Data_WriteVec3(hFile, m_vAngle, dwByte);
}

void CTransform::Load(HANDLE hFile, DWORD & dwByte)
{
	CUtility::Data_ReadVec3(hFile, m_vInfo[(_int)E_INFO::INFO_POS], dwByte);
	CUtility::Data_ReadVec3(hFile, m_vScale, dwByte);
	CUtility::Data_ReadVec3(hFile, m_vAngle, dwByte);
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform*		pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < (_int)E_INFO::INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

_int CTransform::Update_Component(const _float & fTimeDelta)
{
	// 월드 행렬 초기화
	D3DXMatrixIdentity(&m_matWorld);

	_int iInfoEnd = (_int)E_INFO::INFO_POS;

	// 인포 초기화
	for (_uint i = 0; i < (_uint)iInfoEnd; ++i) 
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	// 크기적용
	for (_uint i = 0; i < (_uint)iInfoEnd; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전적용
	_matrix		matRot[(_int)E_ROTATION::ROT_END];
	D3DXMatrixRotationX(&matRot[(_int)E_ROTATION::ROT_X], m_vAngle.x * Deg2Rad);
	D3DXMatrixRotationY(&matRot[(_int)E_ROTATION::ROT_Y], m_vAngle.y * Deg2Rad);
	D3DXMatrixRotationZ(&matRot[(_int)E_ROTATION::ROT_Z], m_vAngle.z * Deg2Rad);

	for (_uint i = 0; i < (_uint)iInfoEnd; ++i)
	{
		for (_uint j = 0; j < (_int)E_ROTATION::ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	

	// 위치 변환은 Set_Pos 함수를 통해서 직접 대입하기 때문에 다른 벡터들을 가공할 이유가 없다
	for (_uint i = 0; i < (_int)E_INFO::INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

const _vec3 CTransform::Get_Info(E_INFO eType) const
{
	_vec3 vOutInfo;
	memcpy(&vOutInfo, &m_matWorld.m[(_int)eType][0], sizeof(_vec3));
	return vOutInfo;
}

const _matrix CTransform::Get_WorldMatrix_Billboard()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 뷰의 역행렬

	_matrix	matInverView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matInverView);
	matInverView._41 = 0;
	matInverView._42 = 0;
	matInverView._43 = 0;
	D3DXMatrixInverse(&matInverView, NULL, &matInverView);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 위치 정보
		
	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	
	_matrix matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, m_vAngle.y * Deg2Rad, m_vAngle.x * Deg2Rad, m_vAngle.z * Deg2Rad);

	_matrix matPosition;
	_vec3 vPos = m_vInfo[(_int)E_INFO::INFO_POS];
	D3DXMatrixTranslation(&matPosition, vPos.x, vPos.y, vPos.z);

	/////////////////////////////////////////////////////////////////////////////////////////////////

	return  matScale * matRot * matInverView * matPosition;
}

void CTransform::Set_Pos(_vec3 vPos)
{
	m_vInfo[(_int)E_INFO::INFO_POS] = vPos; 
}

void CTransform::Set_Pos(E_POSITION ePosition, _float fPos)
{
	switch (ePosition)
	{
	case E_POSITION::POS_X:
		m_vInfo[_int(E_INFO::INFO_POS)].x = fPos;
		break;
	case E_POSITION::POS_Y:
		m_vInfo[_int(E_INFO::INFO_POS)].y = fPos;
		break;
	case E_POSITION::POS_Z:
		m_vInfo[_int(E_INFO::INFO_POS)].z = fPos;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Add_Pos(_vec3 vAddPos)
{
	m_vInfo[(_int)E_INFO::INFO_POS] += vAddPos;
}

void CTransform::Add_Pos(E_POSITION ePosition, _float fAddPos)
{
	switch (ePosition)
	{
	case E_POSITION::POS_X:
		m_vInfo[(_int)E_INFO::INFO_POS].x += fAddPos;
		break;
	case E_POSITION::POS_Y:
		m_vInfo[(_int)E_INFO::INFO_POS].y += fAddPos;
		break;
	case E_POSITION::POS_Z:
		m_vInfo[(_int)E_INFO::INFO_POS].z += fAddPos;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Move_Pos(const _vec3 & pDir, const _float & fSpeed, const _float & fTimeDelta)
{
	m_vInfo[(_int)E_INFO::INFO_POS] += pDir * fSpeed * fTimeDelta;
}

void CTransform::Set_Rotation(_vec3 vRots)
{
	m_vAngle = vRots;
}

void CTransform::Set_Rotation(_float fDegX, _float fDegY, _float fDegZ)
{
	m_vAngle = _vec3(fDegX, fDegY, fDegZ); 
}

void CTransform::Set_Rotation(E_ROTATION eRotation, _float fDeg)
{
	switch (eRotation)
	{
	case E_ROTATION::ROT_X:
		m_vAngle.x = fDeg;
		break;
	case E_ROTATION::ROT_Y:
		m_vAngle.y = fDeg;
		break;
	case E_ROTATION::ROT_Z:
		m_vAngle.z = fDeg;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Add_Rotation(_vec3 vAddAngle)
{
	m_vAngle += vAddAngle;
}

void CTransform::Add_Rotation(E_ROTATION eRotation, _float fDeg)
{
	switch (eRotation)
	{
	case E_ROTATION::ROT_X:
		m_vAngle.x += fDeg;
		break;
	case E_ROTATION::ROT_Y:
		m_vAngle.y += fDeg;
		break;
	case E_ROTATION::ROT_Z:
		m_vAngle.z += fDeg;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Set_Scale(_vec3 vScale)
{
	m_vScale = vScale;
}

void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	 m_vScale = _vec3(fScaleX, fScaleY, fScaleZ); 
}

void CTransform::Set_Scale(E_SCALE eScale, _float fScale)
{
	switch (eScale)
	{
	case E_SCALE::SCALE_X:
		m_vScale.x = fScale;
		break;
	case E_SCALE::SCALE_Y:
		m_vScale.y = fScale;
		break;
	case E_SCALE::SCALE_Z:
		m_vScale.z = fScale;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Add_Scale(_vec3 vAddScale)
{
	m_vAngle += vAddScale;
}

void CTransform::Add_Scale(E_SCALE eScale, _float fAddScale)
{
	switch (eScale)
	{
	case E_SCALE::SCALE_X:
		m_vScale.x += fAddScale;
		break;
	case E_SCALE::SCALE_Y:
		m_vScale.y += fAddScale;
		break;
	case E_SCALE::SCALE_Z:
		m_vScale.z += fAddScale;
		break;
	default:
		assert(FALSE);
		break;
	}
}

void CTransform::Set_Look(_vec3 vTargetPos)
{
	
	//y 회전,x 회전으로 치환
	_vec3 vPos = m_vInfo[(_int)E_INFO::INFO_POS];
		
	
 	_vec3 vTargetLook = vTargetPos - vPos;
	
	
	_vec3 vZLook = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vZLook, &vZLook);
	_vec3 vYLook = _vec3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&vYLook, &vYLook);
	
	_vec3 vZTargetLook = _vec3(vTargetLook.x,0.f,vTargetLook.z);
	D3DXVec3Normalize(&vZTargetLook, &vZTargetLook);
	_float fDis =sqrtf(vTargetLook.x*vTargetLook.x + vTargetLook.z*vTargetLook.z);
	_vec3 vYTargetLook = _vec3(0.f,vTargetLook.y,fDis);
	D3DXVec3Normalize(&vYTargetLook, &vYTargetLook);

	_float fZAngle = D3DXVec3Dot(&vZLook, &vZTargetLook);
	_float fYAngle = D3DXVec3Dot(&vYLook, &vYTargetLook);
	fZAngle = acosf(fZAngle);
	fYAngle = acosf(fYAngle);

	_vec3 vZCross;
	_vec3 vYCross;
	D3DXVec3Cross(&vZCross,&vZLook, &vZTargetLook);
	D3DXVec3Cross(&vYCross, &vYLook,&vYTargetLook);

	if (vZCross.y < 0.f)
	{
		fZAngle = -fZAngle;
	}

	if (vYCross.x < 0.f)
	{
		fYAngle = -fYAngle;
	}

	m_vAngle.y = fZAngle*Rad2Deg;
	m_vAngle.x = fYAngle*Rad2Deg;
	m_vAngle.z = 0.f;
	
	
	//_vec3 vPos = m_vInfo[(_int)E_INFO::INFO_POS];

	////이방식으로 할 경우 룩함수를 뒤집어야 정상적으로 나옴
	//_vec3 vTargetLook = vPos - vTargetPos;
	//D3DXVec3Normalize(&vTargetLook, &vTargetLook);
	//_vec3 vUp = { 0.f,1.f,0.f };
	//_vec3 vAt = vPos + vTargetLook;

	//_matrix matLook;
	//D3DXMatrixLookAtLH(&matLook, &vPos, &vAt, &vUp);
	//D3DXMatrixInverse(&matLook, 0, &matLook);

	//_vec3 vRight = _vec3(matLook._11, matLook._12, matLook._13);
	//D3DXVec3Normalize(&vRight, &vRight);
	//_vec3 vObjUp = _vec3(matLook._21, matLook._22, matLook._23);
	//D3DXVec3Normalize(&vObjUp, &vObjUp);
	//_vec3 vLook = _vec3(matLook._31, matLook._32, matLook._33);
	//D3DXVec3Normalize(&vLook, &vLook);
	//
	//m_vInfo[(_int)E_INFO::INFO_RIGHT] = vRight;
	//m_vInfo[(_int)E_INFO::INFO_UP] = vObjUp;
	//m_vInfo[(_int)E_INFO::INFO_LOOK] = vLook;

	
}

void CTransform::Set_Direction(_vec3 vLook)
{
	Set_Look(m_vInfo[(_int)E_INFO::INFO_POS] + vLook);
}

void CTransform::Set_ParentMatrix(const _matrix * pParent)
{
	m_matWorld *= *pParent;
}
