#pragma once
#ifndef __WARNING_MGR_H__
#define __WARNING_MGR_H__

#include "Engine_Define.h"
#include "Base.h"
#include "Warning.h"

BEGIN(Engine)

class ENGINE_DLL CWarning_Mgr : public CBase
{
	DECLARE_SINGLETON(CWarning_Mgr)

private:
	CWarning_Mgr();
	virtual ~CWarning_Mgr();

public:
	virtual void Free() override;


public:
	// 원형각도로 생기는 탄막
	void	Create_Circle(CWarning* pOrigin, _float fStartAngle, _float fStackAngle, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _float fRadiusBegin, _float fRadiusEnd, _vec4 vBegin, _vec4 vEnd);

	// 직선(단방향)으로 생기는 탄막
	void	Create_Line_OneWay(CWarning* pOrigin, _float fAngle, _float fDistance, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _vec4 vBegin, _vec4 vEnd);

	// 직선(양방향)으로 생기는 탄막
	void	Create_Line_TwoWay(CWarning* pOrigin, _float fAngle, _float fDistance, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _vec4 vBegin, _vec4 vEnd);
};

END

#endif // !__WARNING_MGR_H__