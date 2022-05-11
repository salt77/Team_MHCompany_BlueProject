#pragma once

#ifndef __COLLISION_MANGER_H__
#define __COLLISION_MANGER_H__

#include "Engine_Define.h"
#include "Base.h"



BEGIN(Engine)

class CSphereCollider;
class CGameObject;

class ENGINE_DLL CCollisionManager :
	public CBase
{
	DECLARE_SINGLETON(CCollisionManager)

private:
	CCollisionManager();
	virtual ~CCollisionManager();

public:
	virtual void Free() override;

	//충돌체 컴포넌트 끼리 충돌시키는 함수
	_bool	Collision_CircleToCircle(CSphereCollider * pDestSphereCol, E_LAYERTAG eLayerTag, _bool bUseY = FALSE);
	_bool	Push_Sphere(CSphereCollider * pDestSphereCol, E_LAYERTAG eLayerTag);

	//레이와 스페어콜라이더를 충돌시키는 함수
	_bool	Collision_RayToSphere(CGameObject* pDestObject, _vec3 vRayStartPos, _vec3 vRayDir, _float fRayDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	//충돌체 컴포넌트 없이 충돌시키는 함수
	_bool   Collision_FanToCircle(CGameObject* pDestObject, _vec3 vStartPos, _vec3 vDir,_float fFanDegree, _float fFanDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool   Collision_FanToCircle_Guide(CGameObject* pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	_bool	Collision_StraightToCircle(CGameObject* pDestObject, _vec3 vStartPos,_vec3 vDir, _float fWidth, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool	Collision_StraightToCircle_Guide(CGameObject* pDestObject, T_GUIDE_OPTION tGuideOption, _float fDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	_bool   Collision_SpotToCircle(CGameObject* pDestObject,_vec3 vStartPos,_float fRadius,E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool   Collision_SpotToCircle_Guide(CGameObject* pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	_bool   Collision_DonutToCircle(CGameObject* pDestObject, _vec3 vStartPos, _float fMinRadius, _float fMaxRadius, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool   Collision_DonutToCircle_Guide(CGameObject* pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));

	_bool   Collision_CutDonutToCircle(CGameObject* pDestObject, _vec3 vStartPos, _vec3 vDir, _float fFanDegree, _float fMinDistance, _float fMaxDistance, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
	_bool   Collision_CutDonutToCircle_Guide(CGameObject* pDestObject, T_GUIDE_OPTION tGuideOption, E_LAYERTAG eLayerTag, void(*Col)(CGameObject*, CGameObject*));
};

END

#endif