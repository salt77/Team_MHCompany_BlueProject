#include "Warning_Mgr.h"
#include "Scene.h"
USING(Engine)

IMPLEMENT_SINGLETON(CWarning_Mgr)

CWarning_Mgr::CWarning_Mgr()
{
}


CWarning_Mgr::~CWarning_Mgr()
{
}

void CWarning_Mgr::Free()
{
}

void CWarning_Mgr::Create_Circle(CWarning * pOrigin, _float fStartAngle, _float fStackAngle, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _float fRadiusBegin, _float fRadiusEnd, _vec4 vBegin, _vec4 vEnd)
{
	for (_int i = 0; i < iCount; ++i)
	{
		_float fRatio = (i) / (_float)(iCount - 1);
		if (iCount == 1)
			fRatio = 1.f;

		CWarning* pWarn = CWarning::Create(CGraphicDev::GetInstance()->GetDevice());
		pWarn->Copy(pOrigin);

		_float fStackDelay = (fDelay * i) + fBaseDelay;
		pWarn->Set_Delay(fStackDelay);

		_float fRadius = CUtility::Lerp_float(fRadiusBegin, fRadiusEnd, fRatio);
		_vec3 vAddPos = _vec3(cosf(Deg2Rad * (fStartAngle + (i * fStackAngle))), 0.f, -sinf(Deg2Rad * (fStartAngle + (i * fStackAngle)))) * fRadius;
		vAddPos += _vec3(0.f, (i * 0.001f), 0.f);
		pWarn->Get_MaxEffect()->Set_Effect_Pos(vAddPos + vCenterPos);
		pWarn->Get_NowEffect()->Set_Effect_Pos(vAddPos + vCenterPos + _vec3(0.f, 0.02f, 0.f));

		_vec4 vColor = CUtility::Lerp_vec4(vBegin, vEnd, fRatio);
		pWarn->Get_MaxEffect()->Get_Effect_Info().vColor_End = vColor;
		pWarn->Get_NowEffect()->Get_Effect_Info().vColor_End = vColor;

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn);
	}
}

void CWarning_Mgr::Create_Line_OneWay(CWarning * pOrigin, _float fAngle, _float fDistance, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _vec4 vBegin, _vec4 vEnd)
{
	for (_int i = 0; i < iCount; ++i)
	{
		_float fRatio = (i) / (_float)(iCount - 1);
		if (iCount == 1)
			fRatio = 1.f;

		CWarning* pWarn = CWarning::Create(CGraphicDev::GetInstance()->GetDevice());
		pWarn->Copy(pOrigin);

		_float fStackDelay = (fDelay * i) + fBaseDelay;
		pWarn->Set_Delay(fStackDelay);

		_vec3 vAddPos = _vec3(cosf(Deg2Rad * (fAngle)), 0.f, -sinf(Deg2Rad * (fAngle))) * (fDistance * i);
		vAddPos += _vec3(0.f, (i * 0.001f), 0.f);
		pWarn->Get_MaxEffect()->Set_Effect_Pos(vAddPos + vCenterPos);
		pWarn->Get_NowEffect()->Set_Effect_Pos(vAddPos + vCenterPos + _vec3(0.f, 0.02f, 0.f));

		_vec4 vColor = CUtility::Lerp_vec4(vBegin, vEnd, fRatio);
		pWarn->Get_MaxEffect()->Get_Effect_Info().vColor_End = vColor;
		pWarn->Get_NowEffect()->Get_Effect_Info().vColor_End = vColor;

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn);
	}
}

void CWarning_Mgr::Create_Line_TwoWay(CWarning * pOrigin, _float fAngle, _float fDistance, _int iCount, _float fBaseDelay, _float fDelay, _vec3 vCenterPos, _vec4 vBegin, _vec4 vEnd)
{
	for (_int i = 0; i < iCount - 1; ++i)
	{
		_float fRatio = (i) / (_float)(iCount - 1);
		if (iCount == 1)
			fRatio = 1.f;

		// - 방향 
		CWarning* pWarn_Minus = CWarning::Create(CGraphicDev::GetInstance()->GetDevice());
		pWarn_Minus->Copy(pOrigin);

		_float fStackDelay = (fDelay * i) + fBaseDelay;
		pWarn_Minus->Set_Delay(fStackDelay);

		_vec3 vAddPos = _vec3(cosf(Deg2Rad * (fAngle)), 0.f, -sinf(Deg2Rad * (fAngle))) * -(fDistance * i);
		vAddPos += _vec3(0.f, (i * 0.001f), 0.f);
		pWarn_Minus->Get_MaxEffect()->Set_Effect_Pos(vAddPos + vCenterPos);
		pWarn_Minus->Get_NowEffect()->Set_Effect_Pos(vAddPos + vCenterPos + _vec3(0.f, 0.02f, 0.f));

		_vec4 vColor = CUtility::Lerp_vec4(vBegin, vEnd, fRatio);
		pWarn_Minus->Get_MaxEffect()->Get_Effect_Info().vColor_End = vColor;
		pWarn_Minus->Get_NowEffect()->Get_Effect_Info().vColor_End = vColor;

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn_Minus);

		// + 방향 
		CWarning* pWarn_Plus = CWarning::Create(CGraphicDev::GetInstance()->GetDevice());
		pWarn_Plus->Copy(pOrigin);

		pWarn_Plus->Set_Delay(fStackDelay);

		vAddPos = _vec3(cosf(Deg2Rad * (fAngle)), 0.f, -sinf(Deg2Rad * (fAngle))) * +(fDistance * i);
		vAddPos += _vec3(0.f, (i * 0.001f), 0.f);
		pWarn_Plus->Get_MaxEffect()->Set_Effect_Pos(vAddPos + vCenterPos);
		pWarn_Plus->Get_NowEffect()->Set_Effect_Pos(vAddPos + vCenterPos + _vec3(0.f, 0.02f, 0.f));

		vColor = CUtility::Lerp_vec4(vBegin, vEnd, fRatio);
		pWarn_Plus->Get_MaxEffect()->Get_Effect_Info().vColor_End = vColor;
		pWarn_Plus->Get_NowEffect()->Get_Effect_Info().vColor_End = vColor;

		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, E_OBJTAG::OBJ_Effect, pWarn_Plus);
	}
}
