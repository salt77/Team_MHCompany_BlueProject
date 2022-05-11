#pragma once
#ifndef __SKILL_MANAGER_H__
#define __SKILL_MANAGER_H__

#include "Engine_Define.h"
#include "Base.h"
#include "SkillObject.h"
#include "UISkillObject.h"
#include "SkillGuideObject.h"

class ENGINE_DLL CSkill_Manager final: public CBase
{
	DECLARE_SINGLETON(CSkill_Manager)
private:
	CSkill_Manager();
	virtual ~CSkill_Manager();

public:
	virtual void Free() override;

public:
	CSkillObject*		SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _vec3 vScale = VEC_ONE);
	CSkillObject*		SetSkill(const wstring & pSkillTag, E_LAYERTAG eLayerTag, _vec3 vPos, _vec3 vRot, _float fScale);
	CUISkillObject*		SetUISkill(const wstring& pSkillTag, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _vec3 vScale = VEC_ONE);
	CUISkillObject*		SetUISkill(const wstring& pSkillTag, E_LAYERTAG eLayerTag, _vec2 vUIPos, _float fZRot, _float fScale);
	
	CSkillGuideObject*	SetSkillGuideStraight(_vec3 vStartPos, _vec3 vStartRot, _float fDistance, _float fWidth, _float fDelay, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	CSkillGuideObject*	SetSkillGuideDonut(_vec3 vStartPos, _float fMinDistance, _float fMaxDistance, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	CSkillGuideObject*	SetSkillGuideCutDonut(_vec3 vStartPos, _vec3 vStartRot, _float fMinDistance, _float fMaxDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	CSkillGuideObject*	SetSkillGuideFan(_vec3 vStartPos, _vec3 vStartRot, _float fDistance, _float fAngle, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");
	CSkillGuideObject*	SetSkillGuideCircle(_vec3 vStartPos, _float fRadius, _float fDelay, _int iVertexCount, _bool bUseDelayMesh, wstring wstrTexPath = L"");

private:
	void				Get_SkillTag(const wstring & pSkillTag, vector<wstring>& vecEffect, vector<wstring>& vecParticle);
};



#endif // ! __SKILL_MANAGER_H__