#include "Skill_Manager.h"
#include "PrefabManager.h"
#include "Scene.h"

#include "StraightGuide.h"
#include "FanGuide.h"
#include "DonutGuide.h"
#include "CutDonutGuide.h"
#include "CircleGuide.h"

USING(Engine)

IMPLEMENT_SINGLETON(CSkill_Manager)

CSkill_Manager::CSkill_Manager()
{

}

CSkill_Manager::~CSkill_Manager()
{
}

void CSkill_Manager::Free()
{

}

CSkillObject* CSkill_Manager::SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale)
{
	vector<wstring> vectorEffect, vectorParticle;

	Get_SkillTag(pSkillTag, vectorEffect, vectorParticle);

	CSkillObject* pSkillObj = nullptr;
	pSkillObj = CSkillObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, vectorEffect, vectorParticle, eLayerTag, vPos, vRot, vScale);
	if (!pSkillObj)
	{
		return nullptr;
	}
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, pSkillObj->Get_ObjTag(), pSkillObj);

	return pSkillObj;
}

CSkillObject * CSkill_Manager::SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _float fScale)
{
	return SetSkill(pSkillTag,eLayerTag,vPos,vRot,_vec3(fScale, fScale, fScale));
}

CUISkillObject * CSkill_Manager::SetUISkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale)
{
	vector<wstring> vectorEffect, vectorParticle;

	Get_SkillTag(pSkillTag, vectorEffect, vectorParticle);

	CUISkillObject* pUISkillObj = nullptr;
	pUISkillObj = CUISkillObject::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Test, vectorEffect, vectorParticle, eLayerTag, vUIPos, fZRot, vScale);
	if (!pUISkillObj)
	{
		return nullptr;
	}
	CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::GameLogic, pUISkillObj->Get_ObjTag(), pUISkillObj);

	return pUISkillObj;
}

CUISkillObject * CSkill_Manager::SetUISkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _float fScale)
{
	return SetUISkill(pSkillTag, eLayerTag, vUIPos, fZRot, _vec3(fScale, fScale, fScale));
}

CSkillGuideObject* CSkill_Manager::SetSkillGuideStraight(_vec3 vStartPos,_vec3 vStartRot, _float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CStraightGuide::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect , fDistance, fWidth, fDelay, bUseDelayMesh, wstrTexPath);

	if (pGuide)
	{
		pGuide->Set_Pos(vStartPos);
		pGuide->Set_Rotation(vStartRot);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pGuide->Get_ObjTag(), pGuide);
		pGuide->Component_Update();
	}
	

	return pGuide;
}

CSkillGuideObject* CSkill_Manager::SetSkillGuideDonut(_vec3 vStartPos, _float fMinDistance, _float fMaxDistance, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CDonutGuide::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, fMinDistance, fMaxDistance, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath);

	if (pGuide)
	{
		pGuide->Set_Pos(vStartPos);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pGuide->Get_ObjTag(), pGuide);
		pGuide->Component_Update();
	}

	
	return pGuide;
}

CSkillGuideObject* CSkill_Manager::SetSkillGuideCutDonut(_vec3 vStartPos, _vec3 vStartRot, _float fMinDistance, _float fMaxDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CCutDonutGuide::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, fMinDistance, fMaxDistance, fAngle, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath);

	if (pGuide)
	{
		pGuide->Set_Pos(vStartPos);
		pGuide->Set_Rotation(vStartRot);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pGuide->Get_ObjTag(), pGuide);
		pGuide->Component_Update();
	}


	return pGuide;
}

CSkillGuideObject* CSkill_Manager::SetSkillGuideFan(_vec3 vStartPos, _vec3 vStartRot, _float fDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CFanGuide::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, fDistance, fAngle, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath);

	if (pGuide)
	{
		pGuide->Set_Pos(vStartPos);
		pGuide->Set_Rotation(vStartRot);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pGuide->Get_ObjTag(), pGuide);
		pGuide->Component_Update();
	}


	return pGuide;
}

CSkillGuideObject* CSkill_Manager::SetSkillGuideCircle(_vec3 vStartPos, _float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath)
{
	CSkillGuideObject* pGuide = nullptr;
	pGuide = CCircleGuide::Create(CGraphicDev::GetInstance()->GetDevice(), E_OBJTAG::OBJ_Effect, fRadius, fDelay, iVertexCount, bUseDelayMesh, wstrTexPath);

	if (pGuide)
	{
		pGuide->Set_Pos(vStartPos);
		CScene::Get_MainScene()->Add_GameObject(E_LAYERTAG::Effect, pGuide->Get_ObjTag(), pGuide);
		pGuide->Component_Update();
	}

	return pGuide;
}

void CSkill_Manager::Get_SkillTag(const wstring & pSkillTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle)
{
	DWORD dwByte;

	_int iSize = 0;
	wstring wstrPath;
	wchar_t wstrTex[MAX_PATH];
	// Effect
	CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, &iSize, sizeof(_int), &dwByte);
	vecEffect.reserve(iSize);
	for (_int i = 0; i < iSize; i++)
	{
		_int iPathSize = 0;
		CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, &iPathSize, sizeof(_int), &dwByte);
		CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, wstrTex, iPathSize, &dwByte);
		wstrPath = wstrTex;

		vecEffect.emplace_back(wstrPath);
	}

	// Particle
	CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, &iSize, sizeof(_int), &dwByte);
	vecParticle.reserve(iSize);
	for (_int i = 0; i < iSize; i++)
	{
		_int iPathSize = 0;
		CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, &iPathSize, sizeof(_int), &dwByte);
		CPrefabManager::GetInstance()->ReadPrefab(pSkillTag, wstrTex, iPathSize, &dwByte);
		wstrPath = wstrTex;

		vecParticle.emplace_back(wstrPath);
	}

	CPrefabManager::GetInstance()->FileReset();
}
