#pragma once
#ifndef __ActionCam_Mgr_H__
#define __ActionCam_Mgr_H__

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"
#include "Camera.h"

// Object
class CCamControl_Eye; 
class CCamControl_At; 

USING(Engine)

class ENGINE_DLL CActionCam_Mgr : public CBase
{
	DECLARE_SINGLETON(CActionCam_Mgr)

public:
	enum E_ActionCam_Mode
	{
		ActionCam_Normal, ActionCam_Subject, ActionCam_Observe, 
		ActionCam_End 
	};

private:
	CActionCam_Mgr();
	virtual ~CActionCam_Mgr();
	virtual void		Free() override;

public:
	HRESULT				Ready_ActionCamManager(LPDIRECT3DDEVICE9 pGraphicDev);
	_int				Update_ActionCamManager(const _float& fTimeDelta);

	HRESULT				Load_ActionCam(const wstring& wstrFileName);
	void				Start_ActionCam(const wstring& wstrFileName, const wstring& wstrAction, _vec3 vPos, _float fYAngle, E_ActionCam_Mode eMode = ActionCam_Subject);
	void				Start_ActionCam_NotLoad(const wstring& wstrFileName, const wstring& wstrAction, _vec3 vPos, _float fYAngle = 0.f , E_ActionCam_Mode eMode = ActionCam_Subject);
	void				Stop_ActionCam(); 
	//	Scene을 넘어갈 때 Map 청소 
	void				Clear_MapActionCam();

public:
	void				Scene_Change();
	void				Set_ClearAllPoint();

private:
	void				MoveOn_Line();
	void				Eye_Line();
	void				At_Line();
	void				Make_Spline(_vec3* vOutPos, const _vec3& vPos0, const _vec3& vPos1, const _vec3& vPos2, const _vec3& vPos3);


private:
	_vec3				Get_Adjust_CamPos(_vec3 vOrigin);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	CScene*				m_pScene = nullptr;
	CCamera*			m_pMainCam = nullptr; 

	_uint				m_iCurLineIndex = 1;

	_float				m_fTimeDelta = 0.f; 
	_float				m_fProgressCam = 0.f; 
	_float				m_fTime = 0.f; 

	_vec3				m_vLinePos = VEC_ZERO;
	_vec3				m_vLineAtPos = VEC_ZERO; 

	wstring				m_wstrCurFile = L"";
	wstring				m_wstrCurEvent = L""; 

	E_ActionCam_Mode	m_eCurActionMode = ActionCam_End;

	map<wstring, map<wstring, vector<_float>>>				m_dbMapSectionSpeed;

	map<wstring, map<wstring, vector<CCamControl_Eye*>>>	m_dbMapActionEye;
	map<wstring, map<wstring, vector<CCamControl_At*>>>		m_dbMapActionAt;

	_float	m_fAngle_Rot_Y = 0.f;
	_vec3	m_vAddPos = VEC_ZERO;
};

#endif //__ActionCam_Mgr_H__