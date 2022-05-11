#include "CollisionManager.h"



USING(Engine)

#include "Collider.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
	Free();
}

void CCollisionManager::Free()
{
}

//������ݶ��̴��� �޾ƿͼ� �ش� ���̾��±��� �� �浹ü�� ���� �浹�ÿ� �Լ������� ���ڿ� �־��� �Լ��� ȣ�����ִ� �Լ�
_bool CCollisionManager::Collision_CircleToCircle(CSphereCollider * pDestSphereCol, E_LAYERTAG eDestLayerTag, _bool bUseY)
{
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eDestLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vDestPos = pDestSphereCol->GetPos();
	if (FALSE == bUseY)
	{
		vDestPos.y = 0.f;
	}

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{	
			if (pObj->Get_Component(ComTag_Collider) && 
				E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));
			if (nullptr == pSrcSphereCol)
			{
				break;
			}

			_float fCheckDis = pDestSphereCol->GetRadius() + pSrcSphereCol->GetRadius();

			_vec3 vSrcPos=pSrcSphereCol->GetPos();
			if (FALSE == bUseY)
			{
				vSrcPos.y = 0.f;
			}
			_float fDistance=D3DXVec3Length(&(vDestPos - vSrcPos));
			
			if (fDistance < fCheckDis)
			{
				if (!pDestSphereCol->ColListCheck(pSrcSphereCol->GetParent())) //�̹� �浹�Ǿ��� ������Ʈ�� ��� �������� ����
				{
					pDestSphereCol->GetParent()->On_Collision(pSrcSphereCol->GetParent());
					pSrcSphereCol->GetParent()->On_Collision(pDestSphereCol->GetParent());
					pDestSphereCol->AddColList(pSrcSphereCol->GetParent());
					bResult = TRUE;
				}
			}
			
		}
	}
	// vecObject=Management Get_vecGameObject(L"");

	return bResult;
}

_bool CCollisionManager::Push_Sphere(CSphereCollider * pDestSphereCol, E_LAYERTAG eLayerTag)
{
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vDestPos = pDestSphereCol->GetPos();

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (pObj->Get_Component(ComTag_Collider) &&
				E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}

			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			if (nullptr == pSrcSphereCol)
			{
				break;
			}

			_float fCheckDis = pDestSphereCol->GetRadius() + pSrcSphereCol->GetRadius();

			_vec3 vSrcPos = pSrcSphereCol->GetPos();

			_float fDistance = D3DXVec3Length(&(vDestPos - vSrcPos));
			_vec3 vPushDir = {};
			_float vRadius = {};

			if (fDistance <= fCheckDis)
			{
				vPushDir = vDestPos - vSrcPos;
				vRadius = fCheckDis;

				D3DXVec3Normalize(&vPushDir, &vPushDir);

				vSrcPos += vPushDir * vRadius;

				vSrcPos.y *= 0.f;

				pDestSphereCol->GetParent()->Set_Pos(vSrcPos);

				bResult = TRUE;
			}
		}
	}

	return bResult;
}

_bool CCollisionManager::Collision_RayToSphere(CGameObject * pDestObject, _vec3 vRayStartPos, _vec3 vRayDir, _float fRayDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);


	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			CGameObject* pSrcObject = nullptr;
			if (pSrcSphereCol->PickingCollider(vRayStartPos, vRayDir, fRayDistance, &pSrcObject))
			{
				Col(pDestObject, pSrcObject);
				bResult = TRUE;
			}
		}
	}
	return bResult;
}

//��������, ����, ��ä�� ����, ���� ���� �޾Ƽ� �ش� ��ä�ð� ���̾��±��� ���浹ü�� ���ؼ� �浹�� �Լ������� ���ڿ� �־��� �Լ��� ȣ�����ִ� �Լ�
_bool CCollisionManager::Collision_FanToCircle(CGameObject * pDestObject, _vec3 vStartPos, _vec3 vDir, _float fFanDegree, _float fFanDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vStartPosition = vStartPos;
	vStartPosition.y = 0.f;
	

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			_vec3 vSrcPos = pSrcSphereCol->GetPos();
			vSrcPos.y = 0.f;

			_float fDistance = D3DXVec3Length(&(vSrcPos - vStartPosition));
			
			
			if (fDistance < fFanDistance+pSrcSphereCol->GetRadius())
			{
				_vec3 vTargetLook = vSrcPos - vStartPosition;
				_float fDegree = CMyMath::GetAngleToTarget_vec3(vDir, vTargetLook);
				if (fDegree < fFanDegree*0.5f)
				{
					if (Col)
					{
						Col(pDestObject, pSrcSphereCol->GetParent());
					}
					bResult = true;
				}
			}
		}
	}
	return bResult;
}

_bool CCollisionManager::Collision_FanToCircle_Guide(CGameObject * pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	

	_vec3 vStartPosition = tGuideOption.vPosition;
	_float fFanDistance = tGuideOption.fMaxDistance;
	_float fFanDegree = tGuideOption.fAngle;

	_vec3 vLook = VEC_LOOK;
	_matrix matRot;
	_vec3 vDir;
	D3DXMatrixRotationYawPitchRoll(&matRot, tGuideOption.vRotation.y * Deg2Rad, tGuideOption.vRotation.x * Deg2Rad, tGuideOption.vRotation.z * Deg2Rad);
	D3DXVec3TransformNormal(&vDir, &vLook, &matRot);
	D3DXVec3Normalize(&vDir, &vDir);

	return Collision_FanToCircle(pDestObject, vStartPosition, vDir, fFanDegree, fFanDistance, eLayerTag, Col);
}

//��������, ����, ����, �ʺ� ���� �޾Ƽ� �ش� ���������� ���̾��±��� ���浹ü�� ���ؼ� �浹�� �Լ������� ���ڿ� �־��� �Լ��� ȣ�����ִ� �Լ�
_bool CCollisionManager::Collision_StraightToCircle(CGameObject * pDestObject, _vec3 vStartPos, _vec3 vDir,_float fWidth, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vStartPosition = vStartPos;
	vStartPosition.y = 0.f;

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			_vec3 vSrcPos = pSrcSphereCol->GetPos();
			vSrcPos.y = 0.f;
			_vec3 vTargetLook = vSrcPos - vStartPosition;
			_float fTargetDis = D3DXVec3Length(&vTargetLook);

			if (fTargetDis < fDistance + pSrcSphereCol->GetRadius())
			{
				_float fDegree = CMyMath::GetAngleToTarget_vec3(vDir, vTargetLook);
				_float fCheckDis = sinf(fDegree*Deg2Rad)*fTargetDis;
				if (fCheckDis < (fWidth*0.5f) + pSrcSphereCol->GetRadius())
				{
					if (Col)
					{
						Col(pDestObject, pSrcSphereCol->GetParent());
					}
					bResult = true;
				}
			}
		
		}
	}
	return bResult;
}

_bool CCollisionManager::Collision_StraightToCircle_Guide(CGameObject * pDestObject, T_GUIDE_OPTION tGuideOption, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{

	_vec3 vLook = VEC_LOOK;
	_matrix matRot;
	_vec3 vDir;
	D3DXMatrixRotationYawPitchRoll(&matRot, tGuideOption.vRotation.y * Deg2Rad, tGuideOption.vRotation.x * Deg2Rad, tGuideOption.vRotation.z * Deg2Rad);
	D3DXVec3TransformNormal(&vDir, &vLook, &matRot);
	D3DXVec3Normalize(&vDir, &vDir);

	return Collision_StraightToCircle(pDestObject, tGuideOption.vPosition, vDir, tGuideOption.fWidth, tGuideOption.fMaxDistance, eLayerTag, Col);
}

//��������, ���������̸� �޾Ƽ� �ش� ���� ������ ���̾��±��� ���浹ü�� ���ؼ� �浹�� �Լ������� ���ڿ� �־��� �Լ��� ȣ�����ִ� �Լ�
_bool CCollisionManager::Collision_SpotToCircle(CGameObject * pDestObject, _vec3 vStartPos, _float fRadius, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vStartPosition = vStartPos;
	vStartPosition.y = 0.f;

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			_vec3 vSrcPos = pSrcSphereCol->GetPos();
			vSrcPos.y = 0.f;

			_float fDistance = D3DXVec3Length(&(vSrcPos - vStartPosition));
			_float fCheckDis = fRadius + pSrcSphereCol->GetRadius();
			if (fDistance < fCheckDis)
			{
				if (Col)
				{
					Col(pDestObject, pSrcSphereCol->GetParent());
				}
				bResult = true;
			}
		}
	}

	return bResult;
}

_bool CCollisionManager::Collision_SpotToCircle_Guide(CGameObject * pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	return Collision_SpotToCircle(pDestObject, tGuideOption.vPosition, tGuideOption.fMaxDistance, eLayerTag, Col);
}

_bool CCollisionManager::Collision_DonutToCircle(CGameObject * pDestObject, _vec3 vStartPos, _float fMinRadius, _float fMaxRadius, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vStartPosition = vStartPos;
	vStartPosition.y = 0.f;

	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			_vec3 vSrcPos = pSrcSphereCol->GetPos();
			vSrcPos.y = 0.f;

			_float fDistance = D3DXVec3Length(&(vSrcPos - vStartPosition));
			_float fCheckDis = fMaxRadius + pSrcSphereCol->GetRadius();
			_float fCheckMinDis = fMinRadius - pSrcSphereCol->GetRadius();
			if (fDistance < fCheckDis && fDistance >= fCheckMinDis)
			{
				if (Col)
				{
					Col(pDestObject, pSrcSphereCol->GetParent());
				}
				bResult = true;
			}
		}
	}

	return bResult;
}

_bool CCollisionManager::Collision_DonutToCircle_Guide(CGameObject * pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{

	return Collision_DonutToCircle(pDestObject, tGuideOption.vPosition, tGuideOption.fMinDistance, tGuideOption.fMaxDistance, eLayerTag, Col);
}

_bool CCollisionManager::Collision_CutDonutToCircle(CGameObject * pDestObject, _vec3 vStartPos, _vec3 vDir, _float fFanDegree, _float fMinDistance, _float fMaxDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	NULL_CHECK_RETURN(Col, FALSE);
	_bool bResult = FALSE;
	map<E_OBJTAG, vector<CGameObject*>>* pMapObject = CScene::Get_MainScene()->Get_LayerMapObject(eLayerTag);
	NULL_CHECK_RETURN(pMapObject, FALSE);

	_vec3 vStartPosition = vStartPos;
	vStartPosition.y = 0.f;


	for (auto& tPair : *pMapObject)
	{
		for (auto& pObj : tPair.second)
		{
			if (E_COLTAG::COL_SPHERE != static_cast<CCollider*>(pObj->Get_Component(ComTag_Collider))->GetColliderTag())
			{
				break;
			}
			CSphereCollider* pSrcSphereCol = static_cast<CSphereCollider*>(pObj->Get_Component(ComTag_Collider));

			_vec3 vSrcPos = pSrcSphereCol->GetPos();
			vSrcPos.y = 0.f;

			_float fDistance = D3DXVec3Length(&(vSrcPos - vStartPosition));


			if (fDistance < fMaxDistance + pSrcSphereCol->GetRadius() && fDistance >= fMinDistance - pSrcSphereCol->GetRadius())
			{
				_vec3 vTargetLook = vSrcPos - vStartPosition;
				_float fDegree = CMyMath::GetAngleToTarget_vec3(vDir, vTargetLook);
				if (fDegree < fFanDegree*0.5f)
				{
					if (Col)
					{
						Col(pDestObject, pSrcSphereCol->GetParent());
					}
					bResult = true;
				}
			}
		}
	}
	return bResult;
}

_bool CCollisionManager::Collision_CutDonutToCircle_Guide(CGameObject * pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject *, CGameObject *))
{
	_vec3 vLook = VEC_LOOK;
	_matrix matRot;
	_vec3 vDir;
	D3DXMatrixRotationYawPitchRoll(&matRot, tGuideOption.vRotation.y * Deg2Rad, tGuideOption.vRotation.x * Deg2Rad, tGuideOption.vRotation.z * Deg2Rad);
	D3DXVec3TransformNormal(&vDir, &vLook, &matRot);
	D3DXVec3Normalize(&vDir, &vDir);

	return Collision_CutDonutToCircle(pDestObject, tGuideOption.vPosition, vDir, tGuideOption.fAngle, tGuideOption.fMinDistance, tGuideOption.fMaxDistance, eLayerTag, Col);
}

