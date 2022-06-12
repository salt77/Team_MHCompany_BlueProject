#pragma once
#ifndef __Dialog_Manager_H__
#define __Dialog_Manager_H__

#include "Engine_Define.h"
#include "Base.h"

#include "UI_Object.h"

USING(Engine)

class ENGINE_DLL CDialog_Manager : public CBase
{
	DECLARE_SINGLETON(CDialog_Manager)

private:
	enum E_Emoticon
	{
		Emoticon_Alarm, Emoticon_Idea, Emoticon_Question_Red, Emoticon_Question_Green,
		Emoticon_Note, Emoticon_Sweat, Emoticon_Anxiety, Emoticon_Exclamation,
		Emoticon_End
	};

private:
	CDialog_Manager();
	virtual ~CDialog_Manager();

public:
	virtual void		Free() override;

public:
	HRESULT				Ready_DialogManager();
	_int				Update_DialogManager(const _float& fTimeDelta);

public:
	void				Emplace_MapDialogAppearChar(const wstring& wstrEventName, const wstring& wstrCharName, const _uint& iSize = 0);
	void				Emplace_MapDialogInfo(const wstring& wstrEventName, T_DialogInfo* tDialogInfo, const _uint& iSize = 0);
	void				Emplace_MapDialogCharInfo(const wstring& wstrEventName, T_DialogCharInfo* tDialogCharInfo);
	void				Render_DialogFont();
	void				Start_DialogEvent(const wstring& wstrEventName);
	void				End_Dialog();

	void				Clear_DialogUI();

public:
	void				Set_EndParsing(const wstring& wstrEventName); 

private:
	void				Clear_Map();
	void				Nullptr_DialogUI();

	_bool				Dialog_StartEpisode();
	_bool				Dialog_EndEpisode();
	void				Update_Emoticon();
	_bool				Branch_Event();
	void				Key_Input();
	void				Click_Event();
	void				Dialog_Event_BeforeClick();
	void				Dialog_Event_AfterClick();
	void				Dialog_Event();
	void				Dialog_CharEvent();
	void				Skip_Dialog(); 

	void				Return_BranchAnswer(_uint iBranchNumber);
	void				Click_MenuButton(); 
	void				Click_SkipButtonOnMenu(); 
	void				Click_SkipCancel(); 

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;

	_bool				m_bAutoMode = FALSE;
	_bool				m_bClickCheck = FALSE;
	_bool				m_bDialogStartBrake = FALSE;
	_bool				m_bDialogEndBrake = FALSE;
	_bool				m_bDialogStartEpisodeEnd = FALSE;

	_uint				m_iDialogIndex = 0;
	_uint				m_iDialogPreIndex = 0;
	_uint				m_iTextSize = 0;
	_uint				m_iReactionBranch = 1;
	_uint				m_iShakeCount = 6;

	_float				m_fTimeCount = 0.f;
	_float				m_fClickOriginDelay = 0.75f;
	_float				m_fClickDelay = m_fClickOriginDelay;
	_float				m_fTextDelay = 0.03f;
	_float				m_fAutoNextDelay = 1.f;
	_float				m_fMiddleFontTurnOver = 1.5f;
	_float				m_fFadeInOutTime = 1.5f;
	_float				m_fShortFadeInOutTime = 0.6f;
	_float				m_fEmoticonCountTime = 0.f;
	_float				m_fEmoticonSurviveDelay = 1.15f;
	_float				m_fEmoticonBlinkDelay = 0.5f;
	_float				m_fEmoticonBlinkCountTime = 0.f;
	_float				m_fSoundVolumeRatio = 1.f;

	wstring				m_wstrEventName = L"";

	E_DIALOG_LOCK		m_eDialogLock = E_DIALOG_LOCK::DIALOG_END;
	E_Emoticon			m_eCurEmoticon = E_Emoticon::Emoticon_End;
	E_Emoticon			m_ePreEmoticon = E_Emoticon::Emoticon_End;

	map<const wstring, vector<wstring>>						m_mapAppearCharacter;
	map<const wstring, vector<T_DialogInfo*>>				m_mapDialogInfo;
	map<const wstring, multimap<_uint, T_DialogCharInfo*>>	m_mapDialogCharInfo;

	CUI_Object*			m_pDialogEpisode = nullptr;
	CUI_Object*			m_pDialogEnd = nullptr;
	CUI_Object*			m_pDialogUI = nullptr;
	CUI_Object*			m_pFadeInOutUI = nullptr;
	CUI_Object*			m_pBranch_1 = nullptr;
	CUI_Object*			m_pBranch_2 = nullptr;
	CUI_Object*			m_pButtonUI = nullptr;
	CUI_Object*			m_pCharacterUI = nullptr;
	CUI_Object*			m_pEmoticonUI = nullptr;
	CUI_Object*			m_pSkipAnswerUI = nullptr; 
};

#endif //__Dialog_Manager_H__
