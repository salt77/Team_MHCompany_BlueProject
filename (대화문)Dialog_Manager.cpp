// (5인 팀) 블루아카이브 모작에서의 스토리 대화문 자동화 코드 
#include "Dialog_Manager.h"

#include "Scene.h" 

#include "UI_Manager.h"
#include "TimeScale_Manager.h" 
#include "FontMgr.h"
#include "XL_ParsingMgr.h" 
#include "Sound_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDialog_Manager)

CDialog_Manager::CDialog_Manager()
{
}

CDialog_Manager::~CDialog_Manager()
{
	Free();
}

void CDialog_Manager::Free()
{
	m_wstrEventName = L"";

	Clear_Prototype();
	Clear_Map();
	Nullptr_DialogUI();

	Safe_Release(m_pGraphicDev);
}

HRESULT CDialog_Manager::Ready_DialogManager()
{
	m_pGraphicDev = CGraphicDev::GetInstance()->GetDevice();
	m_pGraphicDev->AddRef();

	return S_OK;
}

_int CDialog_Manager::Update_DialogManager(const _float& fTimeDelta)
{
	return RETURN_OK;
}

// 다이얼로그 이벤트 시작(매개변수 이벤트 이름)
void CDialog_Manager::Start_DialogEvent(const wstring& wstrEventName)
{
	// 데이터 없으면 함수 리턴 
	if (m_mapDialogInfo.end() == m_mapDialogInfo.find(wstrEventName))
	{
		return;
	}

	CSound_Manager::GetInstance()->StopSound(E_ChannelID::BGM);

	if (!m_pDialogUI)
	{
		m_pDialogEpisode = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]대화문_제목.uidat");
		assert(m_pDialogEpisode);
		m_pDialogEnd = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]대화문_종료_컨티뉴.uidat");
		assert(m_pDialogEnd);
		m_pDialogUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]Dialogue.uidat");
		assert(m_pDialogUI);
		m_pFadeInOutUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]FadeInOut.uidat");
		assert(m_pFadeInOutUI);
		m_pBranch_1 = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]Branch_1.uidat");
		assert(m_pBranch_1);
		m_pBranch_2 = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]Branch_2.uidat");
		assert(m_pBranch_2);
		m_pButtonUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]대화문_버튼들.uidat");
		assert(m_pButtonUI);
		m_pEmoticonUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]이모티콘.uidat");
		assert(m_pEmoticonUI);
		m_pSkipAnswerUI = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]대화문스킵창.uidat"); 
		assert(m_pSkipAnswerUI); 
		CUI_Object*		pUiMenuButton = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Dialog/[Fixed]대화문메뉴버튼.uidat"); 
		assert(pUiMenuButton); 

		m_pCharacterUI = CUI_Object::Create(m_pGraphicDev, CUI_Manager::GetInstance()->Get_Hwnd(), E_OBJTAG::OBJ_UI);
		m_pCharacterUI->Set_ImageAlpha(0.f);

		for (_uint i = 0; i < m_mapAppearCharacter[wstrEventName].size(); ++i)
		{
			CUI_Object*		pChar = CUI_Manager::GetInstance()->Load_UI_Prefab(L"../../Reference/Data/Data_UI/Prefab/Standing_Char/[Fixed]" + m_mapAppearCharacter[wstrEventName][i] + L".uidat");
			assert(pChar);
			m_pCharacterUI->Set_Child(pChar);
		}

		m_pDialogUI->Get_ChildByIndex(0)->Set_Child(m_pCharacterUI);
		m_pDialogUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Child(m_pBranch_1);
		m_pDialogUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Child(m_pBranch_2);
		m_pDialogUI->Get_ChildByIndex(0)->Set_Child(m_pButtonUI);
		m_pButtonUI->Get_ChildByIndex(0)->Set_Child(pUiMenuButton); 

		for (_uint i = 0; i < m_pDialogUI->Get_ChildByIndex(0)->Get_ChildList()->size(); ++i)
		{
			m_pCharacterUI->Set_SortingOrder(TRUE);
		}

		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pDialogUI->Get_ObjTag(), m_pDialogUI);
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pDialogEnd->Get_ObjTag(), m_pDialogEnd);
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pDialogEpisode->Get_ObjTag(), m_pDialogEpisode);
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pFadeInOutUI->Get_ObjTag(), m_pFadeInOutUI);
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pEmoticonUI->Get_ObjTag(), m_pEmoticonUI);
		CScene::Get_MainScene()->Get_Layer(E_LAYERTAG::UI_Lobby)->Add_GameObject(m_pSkipAnswerUI->Get_ObjTag(), m_pSkipAnswerUI); 
	}

	m_bAutoMode = FALSE;
	m_bDialogStartBrake = FALSE;
	m_bDialogEndBrake = FALSE;
	m_bDialogStartEpisodeEnd = FALSE;

	m_wstrEventName = wstrEventName;
	m_iDialogIndex = 0;

	m_pBranch_1->Set_Active(FALSE);
	m_pBranch_2->Set_Active(FALSE);
	m_pDialogEnd->Set_Active(FALSE);
	m_pFadeInOutUI->Set_Active(FALSE);
	m_pCharacterUI->Set_Active(FALSE);
	m_pButtonUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE); 
	m_pSkipAnswerUI->Set_Active(FALSE); 

	m_pDialogEpisode->Set_ImageAlpha(0.f);
	m_pDialogEnd->Set_ImageAlpha(0.f);
	m_pFadeInOutUI->Set_ImageAlpha(0.f);

	m_pCharacterUI->Set_Ui_Pos(VEC2_ZERO);

	m_pBranch_1->Add_Function_int(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Return_BranchAnswer, this, 1), 1);
	m_pBranch_2->Get_ChildByIndex(0)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Return_BranchAnswer, this, 1), 1);
	m_pBranch_2->Get_ChildByIndex(1)->Add_Function_int(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Return_BranchAnswer, this, 2), 2); 
	m_pButtonUI->Get_ChildByIndex(0)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Click_MenuButton, this)); 
	m_pButtonUI->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, 
		[&]() -> void { m_bAutoMode = !m_bAutoMode;
	m_pButtonUI->Get_ChildByIndex(1)->Set_ImageColor(m_bAutoMode ? _color(1.f, 1.f, 0.f, 1.f) : _color(1.f, 1.f, 1.f, 1.f)); });

	m_pButtonUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_ChildByIndex(1)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Click_SkipButtonOnMenu, this)); 
	m_pSkipAnswerUI->Get_ChildByIndex(3)->Add_Function_void(E_UiFunctionType::OnClick, bind(&CDialog_Manager::Skip_Dialog, this)); 
	m_pSkipAnswerUI->Get_ChildByIndex(4)->Add_Function_void(E_UiFunctionType::OnClick, [&]() -> void { m_pSkipAnswerUI->Set_Active(FALSE); });
	
	Load_Prototype();
	Dialog_Event();
}

void CDialog_Manager::End_Dialog()
{
	m_wstrEventName = L"";

	Clear_DialogUI();
}

void CDialog_Manager::Set_EndParsing(const wstring& wstrEventName)
{
	m_mapDialogInfo[wstrEventName].resize(m_mapDialogInfo[wstrEventName].size());
	m_mapDialogInfo[wstrEventName].shrink_to_fit();
}

// 이벤트에 사용된 파싱 데이터들 삭제 
void CDialog_Manager::Clear_Map()
{
	map<const wstring, vector<T_DialogInfo*>>::iterator		iter_Map = m_mapDialogInfo.begin();
	map<const wstring, multimap<_uint, T_DialogCharInfo*>>::iterator	iter_Char = m_mapDialogCharInfo.begin();

	for (; iter_Map != m_mapDialogInfo.end(); ++iter_Map)
	{
		vector<T_DialogInfo*>::iterator		iter_Vec = iter_Map->second.begin();

		for (; iter_Vec != iter_Map->second.end(); ++iter_Vec)
		{
			Safe_Delete(*iter_Vec);
		}
	}

	for (; iter_Char != m_mapDialogCharInfo.end(); ++iter_Char)
	{
		multimap<_uint, T_DialogCharInfo*>::iterator	iter_mapChar = iter_Char->second.begin();

		for (; iter_mapChar != iter_Char->second.end(); ++iter_mapChar)
		{
			Safe_Delete(iter_mapChar->second);
		}
	}

	m_mapAppearCharacter.clear();
	m_mapDialogInfo.clear();
	m_mapDialogCharInfo.clear();
}

void CDialog_Manager::Nullptr_DialogUI()
{
	m_pSkipAnswerUI = nullptr; 
	m_pEmoticonUI = nullptr;
	m_pFadeInOutUI = nullptr;
	m_pDialogEpisode = nullptr;
	m_pDialogEnd = nullptr;
	m_pDialogUI = nullptr;
}

void CDialog_Manager::Clear_DialogUI()
{
	if (m_pSkipAnswerUI)
	{
		m_pSkipAnswerUI->Set_Dead(TRUE); 
		m_pSkipAnswerUI = nullptr; 
	}
	if (m_pEmoticonUI)
	{
		m_pEmoticonUI->Set_Dead(TRUE);
		m_pEmoticonUI = nullptr;
	}
	if (m_pFadeInOutUI)
	{
		m_pFadeInOutUI->Set_Dead(TRUE);
		m_pFadeInOutUI = nullptr;
	}
	if (m_pDialogEpisode)
	{
		m_pDialogEpisode->Set_Dead(TRUE);
		m_pDialogEpisode = nullptr;
	}
	if (m_pDialogEnd)
	{
		m_pDialogEnd->Set_Dead(TRUE);
		m_pDialogEnd = nullptr;
	}
	if (m_pDialogUI)
	{
		m_pDialogUI->Set_Dead(TRUE);
		m_pDialogUI = nullptr;
	}
}

void CDialog_Manager::Emplace_MapDialogAppearChar(const wstring & wstrEventName, const wstring & wstrCharName, const _uint& iSize)
{
	if (m_mapAppearCharacter[wstrEventName].empty())
	{
		m_mapAppearCharacter[wstrEventName].reserve(iSize);
	}

	m_mapAppearCharacter[wstrEventName].emplace_back(wstrCharName);
}

// XL_ParsingMgr에서 파싱된 데이터 받아오기 
void CDialog_Manager::Emplace_MapDialogInfo(const wstring & wstrEventName, T_DialogInfo * tDialogInfo, const _uint& iSize)
{
	if (m_mapDialogInfo[wstrEventName].empty())
	{
		m_mapDialogInfo[wstrEventName].reserve(iSize);
	}

	m_mapDialogInfo[wstrEventName].emplace_back(tDialogInfo);
}

// 파싱된 캐릭터 데이터 가져오기 
void CDialog_Manager::Emplace_MapDialogCharInfo(const wstring & wstrEventName, T_DialogCharInfo * tDialogCharInfo)
{
	m_mapDialogCharInfo[wstrEventName].emplace(pair<_uint, T_DialogCharInfo*>(tDialogCharInfo->iIndex, tDialogCharInfo));
}

// Renderer에서 대화 폰트 출력 
void CDialog_Manager::Render_DialogFont()
{
	if (!Dialog_StartEpisode())
	{
		return;
	}

	if (L"" != m_wstrEventName)
	{
		if (-1 == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iIndex)
		{
			if (!Dialog_EndEpisode())
			{
				return; 
			}

			End_Dialog();

			return;
		}

		m_fTimeCount += CTimeScale_Manager::GetInstance()->Get_TimeDelta();
		m_fClickDelay -= CTimeScale_Manager::GetInstance()->Get_TimeDelta();

		Click_Event();
		Middle_DialogFont();
		Update_Emoticon();

		if (L"NULL" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
		{
			return;
		}

		wstring		wstrText = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog;

		wstrText = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog.substr(0, m_iTextSize);

		if (L"Branch" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
		{
			if (L"Name_Special" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
			{
				CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName, Dialog_CharName, (_color)C_White, 17, 28, FW_NORMAL, FALSE));
			}
			else
			{
				E_CHAR_NAME		eCharName = (E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName, E_Translation_Type::Translation_CharInfo);

				CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, CXL_ParsingMgr::GetInstance()->Translation_CharName(eCharName), Dialog_CharName, (_color)C_White, 17, 28, FW_NORMAL, FALSE));
			}
		}

		if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharCircle)
		{
			CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Medium, m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharCircle, Dialog_CharCircle, (_color)C_LightBlue, 11, 21, FW_NORMAL, FALSE));
		}

		if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog)
		{
			if (m_iTextSize >= (_uint)m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog.length() &&
				m_bAutoMode)
			{
				if (m_fAutoNextDelay <= m_fTimeCount)
				{
					if (L"Branch" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
					{
						m_fTimeCount = 0.f;

						Dialog_Event_AfterClick();	// 다이얼로그 인덱스 증가 
					}
				}
			}
			else if (m_fTextDelay <= m_fTimeCount)
			{
				m_fTimeCount = 0.f;

				++m_iTextSize;
			}

			if (!Branch_Event())
			{
				return;
			}

			if (L"Normal" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrDialogFontPos)
			{
				wstrText = CUtility::Auto_Line(wstrText, L" ", 35);
				CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Medium, wstrText, Dialog_Text, (_color)C_White, 13, 22, FW_NORMAL, FALSE));
			}
		}
		else
		{
			if (1.f <= m_fTimeCount &&
				E_DIALOG_LOCK::DIALOG_BRANCH != m_eDialogLock)
			{
				++m_iDialogIndex;

				Dialog_Event();
			}
		}

		if (m_iDialogIndex < (_uint)m_mapDialogInfo[m_wstrEventName].size())
		{
			Key_Input();
		}

		_uint	iSize = (_uint)m_mapDialogInfo[m_wstrEventName].size() - 1;

		m_iTextSize = CUtility::Clamp_int(m_iTextSize, 0, (_int)m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog.length());
		m_iDialogIndex = CUtility::Clamp_int(m_iDialogIndex, 0, iSize);
	}
}

// 에피소드 시작 시 처리 
_bool CDialog_Manager::Dialog_StartEpisode()
{
	if (!m_bDialogStartEpisodeEnd &&
		m_pDialogEpisode)
	{
		if (!m_bDialogStartBrake)
		{
			m_fTimeCount = 0.f;
			m_bDialogStartBrake = TRUE;

			m_pDialogEpisode->Set_Active(TRUE);
			m_pDialogEpisode->Set_ImageAlpha(0.f);
			m_pDialogEpisode->Get_ChildByIndex(0)->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][0]->wstrEpisodeCount);
			m_pDialogEpisode->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][0]->wstrEpisodeName);
			m_pDialogEpisode->Set_TextAlpha(0.f);
			m_pDialogEpisode->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fFadeInOutTime);
		}
		else
		{
			if (1.f <= m_pDialogEpisode->Get_ImageColor().a)
			{
				m_fTimeCount += CTimeScale_Manager::GetInstance()->Get_TimeDelta();

				if (1.f <= m_fTimeCount)
				{
					m_pDialogEpisode->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, m_fFadeInOutTime);
				}
			}
			else if (0.f >= m_pDialogEpisode->Get_ImageColor().a &&
					 1.f <= m_fTimeCount)
			{
				m_bDialogStartEpisodeEnd = TRUE;
				m_pDialogEpisode->Set_Active(FALSE);
				m_fTimeCount = 0.f;

				if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBgmFile)
				{
					CSound_Manager::GetInstance()->PlayBGM((wchar_t *)m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBgmFile.c_str(), 1.f);
				}

				return TRUE;
			}
		}

		return FALSE;
	}

	return TRUE;
}

// 에피소드 종료 시 처리 
_bool CDialog_Manager::Dialog_EndEpisode()
{
	if (m_pDialogEnd)
	{
		if (!m_bDialogEndBrake)
		{
			m_bDialogEndBrake = TRUE;
			m_fTimeCount = 0.f; 

			m_pDialogEpisode->Set_ImageAlpha(0.f);
			m_pDialogEnd->Set_Active(TRUE);
			m_pDialogEnd->Set_ImageAlpha(0.f);
			m_pDialogEnd->Set_TextAlpha(0.f);
			m_pDialogEnd->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fFadeInOutTime);

			m_pCharacterUI->Set_Active(FALSE);
		}
		else
		{
			if (1.f <= m_pDialogEnd->Get_ImageColor().a)
			{
				m_fTimeCount += CTimeScale_Manager::GetInstance()->Get_TimeDelta();

				m_pDialogEpisode->Set_Active(TRUE); 

				if (m_fFadeInOutTime >= m_fTimeCount) 
				{
					m_pDialogEpisode->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fFadeInOutTime);
				}
				
				m_pDialogEpisode->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][0]->wstrEpisodeNext);
				m_pDialogEpisode->Get_ChildByIndex(0)->Set_Ui_Text(L"다음화");

				if (1.f <= m_pDialogEpisode->Get_ImageColor().a &&
					m_fFadeInOutTime * 2.f <= m_fTimeCount)
				{
					m_pDialogEpisode->Set_ImageAlpha(0.f);

					Skip_Dialog(); 

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	return TRUE;
}

// 이모티콘 이벤트 업데이트 
void CDialog_Manager::Update_Emoticon()
{
	if (!m_pEmoticonUI)
	{
		return;
	}

	m_fEmoticonCountTime += CTimeScale_Manager::GetInstance()->Get_TimeDelta();

	if (m_eCurEmoticon != m_ePreEmoticon)
	{
		for (_uint i = 0; i < Emoticon_End; ++i)
		{
			m_pEmoticonUI->Get_ChildByIndex(i)->Set_Active(FALSE);
		}

		m_fEmoticonCountTime = 0.f;
		m_ePreEmoticon = m_eCurEmoticon;
	}

	if (m_fEmoticonSurviveDelay <= m_fEmoticonCountTime)
	{
		m_fEmoticonCountTime = 0.f;
		m_eCurEmoticon = Emoticon_End;

		for (_uint i = 0; i < Emoticon_End; ++i)
		{
			m_pEmoticonUI->Get_ChildByIndex(i)->Set_Active(FALSE);
		}
	}

	switch (m_eCurEmoticon)
	{
	case CDialog_Manager::Emoticon_Idea:
		break;

	case CDialog_Manager::Emoticon_Exclamation:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Exclamation)->Set_Active(TRUE);
		break;

	case CDialog_Manager::Emoticon_Anxiety:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Anxiety)->Set_Active(TRUE);
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Anxiety)->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fShortFadeInOutTime);
		break;

	case CDialog_Manager::Emoticon_Note:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Note)->Set_Active(TRUE);
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Note)->Add_UiMoving(T_UiMoving(_vec2(-50.f, 0.f), m_fEmoticonSurviveDelay, E_EASE::OutQuint));
		break;

	case CDialog_Manager::Emoticon_Question_Red:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Question_Red)->Set_Active(TRUE);
		break;

	case CDialog_Manager::Emoticon_Question_Green:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Question_Green)->Set_Active(TRUE);
		break;

	case CDialog_Manager::Emoticon_Sweat:
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Sweat)->Set_Active(TRUE);
		m_pEmoticonUI->Get_ChildByIndex(Emoticon_Sweat)->Add_UiMoving(T_UiMoving(_vec2(0.f, -50.f), m_fEmoticonSurviveDelay, E_EASE::OutQuint));
		break;

	case CDialog_Manager::Emoticon_End:
		break;

	default:
		assert(0);
		break;
	}
}

// 분기 이벤트 
_bool CDialog_Manager::Branch_Event()
{
	if (L"Branch" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
	{
		m_eDialogLock = E_DIALOG_LOCK::DIALOG_BRANCH;

		wstring		wstrText = m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrTextDialog;

		wstrText = CUtility::Auto_Line(wstrText, L" ", 35);
		CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Medium, wstrText, Dialog_Text, (_color)C_White, 13, 22, FW_NORMAL, FALSE));

		Dialog_Event();

		if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrCharName)
		{
			if (L"Name_Special" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
			{
				CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrCharName, Dialog_CharName, (_color)C_White, 17, 28, FW_NORMAL, FALSE));
			}
			else
			{
				E_CHAR_NAME		eCharName = (E_CHAR_NAME)CXL_ParsingMgr::GetInstance()->Translation_StringToEnum(m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrCharName, E_Translation_Type::Translation_CharInfo);

				CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Bold, CXL_ParsingMgr::GetInstance()->Translation_CharName(eCharName), Dialog_CharName, (_color)C_White, 17, 28, FW_NORMAL, FALSE));
			}
		}
		if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrCharCircle)
		{
			CFontMgr::GetInstance()->Render_FontEveryFrame(T_RENDERFONT(FontTag_경기천년제목_Medium, m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->wstrCharCircle, Dialog_CharCircle, (_color)C_LightBlue, 11, 21, FW_NORMAL, FALSE));
		}

		switch (m_mapDialogInfo[m_wstrEventName][m_iDialogPreIndex]->iBranchOptionCount)
		{
		case 1:
			m_pBranch_1->Set_Active(TRUE);
			m_pBranch_1->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBranchText_1);
			break;

		case 2:
			m_pBranch_2->Set_Active(TRUE);
			m_pBranch_2->Get_ChildByIndex(0)->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBranchText_1);
			m_pBranch_2->Get_ChildByIndex(1)->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBranchText_2);
			break;

		case 3:
			m_pBranch_1->Set_Active(TRUE);
			m_pBranch_1->Set_Ui_Text(L"선택지 3 - 아직 처리되지 않음");
			break;

		default:
			assert(0);
			break;
		}

		return FALSE;
	}

	return TRUE;
}

// 키 입력 감지 
void CDialog_Manager::Key_Input()
{
	if (CKeyManager::GetInstance()->Key_Down(KEY_LBUTTON) &&
		E_DIALOG_LOCK::DIALOG_BRANCH != m_eDialogLock)
	{
		// Next Event에 대한 처리 필요 
		if (m_iTextSize >= _uint(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog.length()))
		{
			!m_bClickCheck ? m_bClickCheck = TRUE : m_bClickCheck;

			Dialog_Event_BeforeClick();

			if (E_DIALOG_LOCK::DIALOG_END == m_eDialogLock)
			{
				Dialog_Event_AfterClick();
			}
		}
		else
		{
			m_iTextSize = _uint(m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrTextDialog.length());
		}
	}
}

// 클릭 이벤트 
void CDialog_Manager::Click_Event()
{
	switch (m_eDialogLock)
	{
	case Engine::E_DIALOG_LOCK::DIALOG_BRANCH:
		break;

	case Engine::E_DIALOG_LOCK::DIALOG_FADEINOUT:
		if (1.f < m_pFadeInOutUI->Get_ImageColor().a)
		{
			m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, m_fFadeInOutTime);
		}
		else if (0.f > m_pFadeInOutUI->Get_ImageColor().a)
		{
			m_pFadeInOutUI->Set_Active(FALSE);

			m_eDialogLock = E_DIALOG_LOCK::DIALOG_END;
			m_bClickCheck = FALSE;

			Dialog_Event_AfterClick();
		}
		break;

	case Engine::E_DIALOG_LOCK::DIALOG_END:
		break;

	default:
		assert(0);
		break;
	}
}

// 클릭 이벤트(전, 후로 나뉨)
void CDialog_Manager::Dialog_Event_BeforeClick()
{
	if (L"FadeOut" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrScreenEvent ||
		L"FadeInOut" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrScreenEvent)
	{
		if (!m_pFadeInOutUI->Get_Active())
		{
			m_pFadeInOutUI->Set_Active(TRUE);
			m_pFadeInOutUI->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fFadeInOutTime);
		}

		m_eDialogLock = E_DIALOG_LOCK::DIALOG_FADEINOUT;
	}
}

// 클릭 이벤트(전, 후로 나뉨)
void CDialog_Manager::Dialog_Event_AfterClick()
{
	if (0.f < m_fClickDelay)
	{
		return;
	}

	m_iTextSize = 0;
	m_eCurEmoticon = Emoticon_End;

	if (0 != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iBranchOptionCount)
	{
		m_iDialogPreIndex = m_iDialogIndex;					// 브랜치 이벤트용 변수 저장 
	}

	if (0 != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iGotoBranchIndex)
	{
		m_iDialogIndex = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iGotoBranchIndex;
		Dialog_Event();

		if (L"Branch" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrCharName)
		{
			Dialog_CharEvent(); 
		}
	}
	else
	{
		m_fClickDelay = m_fClickOriginDelay;
		++m_iDialogIndex;

		Dialog_Event();
		Dialog_CharEvent();
	}
}

// 대화문 도중 특정 이벤트 발생 시 실행(Bgm, FadeIn, FadeOut 등)
void CDialog_Manager::Dialog_Event()
{
	if (m_iDialogIndex >= m_mapDialogInfo[m_wstrEventName].size())
	{
		Clear_DialogUI();

		return;
	}

	assert(m_pDialogUI);

	if (L"" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBackgroundFile &&
		L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBackgroundFile &&
		m_pDialogUI)
	{
		if (L"FadeIn" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrScreenEvent ||
			L"FadeInOut" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrScreenEvent)
		{
			m_pDialogUI->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Background/" + m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBackgroundFile);

			if (L"FadeIn" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrScreenEvent)
			{
				m_pDialogUI->Get_ChildByIndex(0)->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, 3.f);
			}
		}
		else
		{
			m_pDialogUI->Get_ChildByIndex(0)->Set_Tex1_Tag(L"../../Reference/Resource/Texture/UI/Background/" + m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBackgroundFile);
		}
	}

	// BGM 
	if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBgmFile)
	{
		if (L"Stop" == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBgmFile)
		{
			CSound_Manager::GetInstance()->StopSound(E_ChannelID::BGM);
		}
		else if (!m_pDialogEpisode->Get_Active())
		{
			m_fSoundVolumeRatio = 1.f;

			CSound_Manager::GetInstance()->Set_Volume_BGM(1.f);
			CSound_Manager::GetInstance()->PlayBGM((wchar_t *)m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrBgmFile.c_str(), 1.f);
		}
	}

	// SFX 
	if (L"NULL" != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrSfxFile)
	{
		CSound_Manager::GetInstance()->PlaySound_((wchar_t *)m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->wstrSfxFile.c_str(), E_ChannelID::Channel_1, E_SoundType::FX, 0.8f);
	}
}

// 캐릭터 이벤트 처리 
void CDialog_Manager::Dialog_CharEvent()
{
	if (-1 == m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iSpecialEventIndex ||
		!m_pEmoticonUI)
	{
		return;
	}

	m_pCharacterUI->Set_Active(TRUE);

	_uint	iEventIndex = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iSpecialEventIndex;

	multimap<_uint, T_DialogCharInfo*>::iterator	iter_Begin = m_mapDialogCharInfo[m_wstrEventName].lower_bound(iEventIndex);

	for (; iter_Begin != m_mapDialogCharInfo[m_wstrEventName].upper_bound(iEventIndex); ++iter_Begin)
	{
		CUI_Object*		pCharUI = nullptr;

		for (_uint i = 0; i < m_pCharacterUI->Get_ChildList()->size(); ++i)
		{
			if (iter_Begin->second->wstrCharName == m_pCharacterUI->Get_ChildByIndex(i)->Get_ChildByIndex(0)->Get_Name())
			{
				pCharUI = m_pCharacterUI->Get_ChildByIndex(i)->Get_ChildByIndex(0);

				break;
			}
		}

		assert(pCharUI);

		pCharUI->Get_Parent()->Set_Active(TRUE);

		// Event 
		if (L"FadeIn" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fShortFadeInOutTime);
			pCharUI->Get_ChildByIndex(0)->Set_FadeInOut(E_FADETYPE::FADETYPE_IN, m_fShortFadeInOutTime);

			if (L"Left" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharLeft);
			}
			else if (L"Center" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharCenter);
			}
			else if (L"Right" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharRight);
			}
		}
		else if (L"FadeOut" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, m_fShortFadeInOutTime);
			pCharUI->Get_ChildByIndex(0)->Set_FadeInOut(E_FADETYPE::FADETYPE_OUT, m_fShortFadeInOutTime);
		}
		else if (L"Shake_LeftRight" == iter_Begin->second->wstrCharEvent)
		{
			for (_uint i = 0; i < m_iShakeCount; ++i)
			{
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(-10.f, 0.f), 0.05f, E_EASE::OutQuint));
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(10.f, 0.f), 0.05f, E_EASE::OutQuint));
			}
		}
		else if (L"Shake_UpDown" == iter_Begin->second->wstrCharEvent)
		{
			for (_uint i = 0; i < m_iShakeCount; ++i)
			{
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, 10.f), 0.05f, E_EASE::OutQuint));
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, -10.f), 0.05f, E_EASE::OutQuint));
			}
		}
		else if (L"UpDown" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, -30.f), 0.5f, E_EASE::OutQuint));
			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, 30.f), 0.5f, E_EASE::OutQuint));
		}
		else if (L"UpDown_Twice" == iter_Begin->second->wstrCharEvent)
		{
			for (_uint i = 0; i < 2; ++i)
			{
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, -30.f), 0.15f, E_EASE::OutQuint));
				pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(_vec2(0.f, 30.f), 0.15f, E_EASE::OutQuint));
			}
		}
		else if (L"PopUp" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Set_ImageAlpha(1.f);
			pCharUI->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);

			if (L"Center" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharCenter);
			}
			else if (L"Left" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharLeft);
			}
			else if (L"Right" == iter_Begin->second->wstrCharPosChange)
			{
				pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharRight);
			}
		}
		else if (L"PopDown" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Set_ImageAlpha(0.f);
			pCharUI->Get_ChildByIndex(0)->Set_ImageAlpha(0.f);
		}
		else if (L"Emphasis" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Set_Active(FALSE);
			pCharUI->Get_Parent()->Get_ChildByIndex(1)->Set_Active(TRUE);
		}
		else if (L"Standard" == iter_Begin->second->wstrCharEvent)
		{
			pCharUI->Get_Parent()->Get_ChildByIndex(1)->Set_Active(FALSE);
			pCharUI->Set_Active(TRUE);
		}

		// Position Change 
		if (L"Left" == iter_Begin->second->wstrCharPosChange)
		{
			_vec2	vAmount = _vec2(Dialog_CharLeft.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharLeft.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);

			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Center" == iter_Begin->second->wstrCharPosChange)
		{
			_vec2	vAmount = _vec2(Dialog_CharCenter.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharCenter.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);

			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Right" == iter_Begin->second->wstrCharPosChange)
		{
			_vec2	vAmount = _vec2(Dialog_CharRight.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharRight.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);

			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Left_Out" == iter_Begin->second->wstrCharPosChange)
		{
			_vec2	vAmount = _vec2(Dialog_CharLeftOut.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharLeftOut.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);

			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Right_Out" == iter_Begin->second->wstrCharPosChange)
		{
			_vec2	vAmount = _vec2(Dialog_CharRightOut.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharRightOut.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);

			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Left_In" == iter_Begin->second->wstrCharPosChange)
		{
			pCharUI->Set_ImageAlpha(1.f);
			pCharUI->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);
			pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharLeftOut);

			_vec2	vAmount = _vec2(Dialog_CharLeft.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharLeft.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);
			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}
		else if (L"Right_In" == iter_Begin->second->wstrCharPosChange)
		{
			pCharUI->Set_ImageAlpha(1.f);
			pCharUI->Get_ChildByIndex(0)->Set_ImageAlpha(1.f);
			pCharUI->Get_Parent()->Set_Ui_Pos(Dialog_CharRightOut);

			_vec2	vAmount = _vec2(Dialog_CharRight.x - pCharUI->Get_Parent()->Get_UiTransform().iPosX, Dialog_CharRight.y - pCharUI->Get_Parent()->Get_UiTransform().iPosY);
			pCharUI->Get_Parent()->Add_UiMoving(T_UiMoving(vAmount, 1.f, E_EASE::OutQuint));
		}

		// Emoticon || Uniqueness 
		if (L"생각" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Idea;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Think.wav", E_ChannelID::Channel_2, E_SoundType::FX, 0.7f);
		}
		else if (L"느낌표" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Exclamation;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Exclaim.wav", E_ChannelID::Channel_3, E_SoundType::FX, 0.7f);
		}
		else if (L"알람" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Alarm;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Respond.wav", E_ChannelID::Channel_4, E_SoundType::FX, 0.7f);
		}
		else if (L"어지러움" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Anxiety;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Angry.wav", E_ChannelID::Channel_5, E_SoundType::FX, 0.7f);
		}
		else if (L"땀" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Sweat;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Sweat.wav", E_ChannelID::Channel_6, E_SoundType::FX, 0.7f);
		}
		else if (L"물음표(빨강)" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Question_Red;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Question.wav", E_ChannelID::Channel_7, E_SoundType::FX, 0.7f);
		}
		else if (L"물음표(초록)" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Question_Green;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Question.wav", E_ChannelID::Channel_8, E_SoundType::FX, 0.7f);
		}
		else if (L"음표" == iter_Begin->second->wstrUniqueness)
		{
			m_eCurEmoticon = Emoticon_Note;
			m_pEmoticonUI->Get_ChildByIndex(m_eCurEmoticon)->Set_Ui_Pos(pCharUI->Get_Parent()->Get_Ui_Pos() + Dialog_Emoticon);
			CSound_Manager::GetInstance()->PlaySound_(L"SFX_Emoticon_Motion_Music.wav", E_ChannelID::Channel_9, E_SoundType::FX, 0.7f);
		}
		else
		{
			m_fEmoticonCountTime = 0.f;
		}
	}
}

// 대화문 이벤트 스킵 
void CDialog_Manager::Skip_Dialog() 
{
	m_wstrEventName = L""; 
	m_fTimeCount = 0.f; 
	m_fClickDelay = m_fClickOriginDelay; 
	m_iDialogIndex = 0; 

	Clear_DialogUI();

	CUI_Manager::GetInstance()->Set_LobbyHighPanelActive();
	CSound_Manager::GetInstance()->StopSound(E_ChannelID::BGM);
	CSound_Manager::GetInstance()->PlayBGM(L"BGM_시스템_스토리.mp3", 0.1f);
}

// 분기문 선택 
void CDialog_Manager::Return_BranchAnswer(_uint iBranchNumber)
{
	Dialog_CharEvent();

	if (1 == iBranchNumber)
	{
		if (0 != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iBranchOptionCount)
		{
			m_iDialogIndex = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iBranchOptionCount;
		}
	}
	else if (2 == iBranchNumber)
	{
		if (0 != m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iGotoBranchIndex)
		{
			m_iDialogIndex = m_mapDialogInfo[m_wstrEventName][m_iDialogIndex]->iGotoBranchIndex;
		}
	}

	m_iReactionBranch = iBranchNumber;
	m_iDialogPreIndex = 0;

	m_pBranch_1->Set_Active(FALSE);
	m_pBranch_2->Set_Active(FALSE);

	m_eDialogLock = E_DIALOG_LOCK::DIALOG_END;
}

void CDialog_Manager::Click_MenuButton()
{
	if (m_pButtonUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Get_Active()) 
	{
		m_pButtonUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(FALSE);
	}
	else
	{
		m_pButtonUI->Get_ChildByIndex(0)->Get_ChildByIndex(0)->Set_Active(TRUE);
	}
}

void CDialog_Manager::Click_SkipButtonOnMenu()
{
	m_pSkipAnswerUI->Get_Active() ? m_pSkipAnswerUI->Set_Active(FALSE) : m_pSkipAnswerUI->Set_Active(TRUE); 
	m_pSkipAnswerUI->Get_ChildByIndex(1)->Set_Ui_Text(m_mapDialogInfo[m_wstrEventName][0]->wstrEpisodeName); 
	m_pSkipAnswerUI->Get_ChildByIndex(2)->Set_Ui_Text(CUtility::Auto_Line(m_mapDialogInfo[m_wstrEventName][0]->wstrStorySummary, L" ", 25)); 
}

void CDialog_Manager::Click_SkipCancel()
{
	m_pSkipAnswerUI->Set_Active(FALSE);
}
