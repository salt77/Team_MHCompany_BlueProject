#pragma once
#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include "Engine_Define.h"
#include "../Include/fmod.h"

// ���� ��� ä�� ID 
enum class E_ChannelID
{
	Channel_1, Channel_2, Channel_3, Channel_4, Channel_5, Channel_6, Channel_7, Channel_8, Channel_9, Channel_10, 
	Channel_11, Channel_12, Channel_13, Channel_14, Channel_15, Channel_16, Channel_17, Channel_18, Channel_19, Channel_20, 
	Channel_21, Channel_22, Channel_23, Channel_24, Channel_25, Channel_26, Channel_27, Channel_28, Channel_29, Channel_30,

	BGM,

	Channel2_1, Channel2_2, Channel2_3, Channel2_4, Channel2_5, Channel2_6, Channel2_7, Channel2_8, Channel2_9, Channel2_10,
	Channel2_11, Channel2_12, Channel2_13, Channel2_14, Channel2_15, Channel2_16, Channel2_17, Channel2_18, Channel2_19, Channel2_20,
	Channel2_21, Channel2_22, Channel2_23, Channel2_24, Channel2_25, Channel2_26, Channel2_27, Channel2_28, Channel2_29, Channel2_30,
	
	StoryVoice,

	Channel_End
};

// �ɼǿ��� ���������� ���� ����
enum class E_SoundType
{
	FX, BGM, Voice
};

// BGM�� Fade ���� ���� 
// FadeIn : ���� �����Ҷ� 0���� 1�� ���� ����, Fadeout : ���ο� BGM�� ������ �� 1���� 0���� ���� ����, Normal : FadeIn�� ������ �� 1�� ����
enum class E_BGMMode
{
	FadeIn, FadeOut, Normal, Stop
};


class ENGINE_DLL CSound_Manager
{
public:
	static CSound_Manager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSound_Manager; 

		return m_pInstance; 
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}

private:
	CSound_Manager();
	~CSound_Manager();
	
public:
	void	Initialize(); 
	void	Release(); 
	void	Update_SoundManager(const _float& fTimeDelta);

public:
	void	PlaySound_(TCHAR* pSoundKey, E_ChannelID eID, E_SoundType eSoundType, float _volume = 1.0f);
	void	PlayStackSound(const TCHAR* pSoundKey, E_SoundType eSoundType, float _volume, float& CoolTime);
	void	PlayStackSound(const TCHAR* pSoundKey, E_SoundType eSoundType, float _volume);

public:
	void	PlayBGM(TCHAR* pSoundKey, float _volume, _bool bFade = FALSE);
	void	StopSound(E_ChannelID eID);
	void	StopAll();

	void	Set_Volume_BGM(_float fRatio);
	void	Set_Volume_FX(_float fRatio);
	void	Set_Volume_Voice(_float fRatio);

	// �浿������ �����ʴ� ����
	void	Add_Sound_NotOverlap(TCHAR* szFileName, _float fVolume);

private:
	_float	Get_SoundRatio(E_SoundType eType);
	void	LoadSoundFile(); 


	// �⺻ Radio * Volume�� ���� ������
private:
	_float	m_fBGM_Volume			= 0.5f;
	_float	m_fBGM_Ratio			= 0.1f;			
	
	_float	m_fFX_Volume			= 0.5f;
	_float	m_fFX_Ratio				= 0.1f;
	
	_float	m_fVoice_Volume			= 0.5f;
	_float	m_fVoice_Ratio			= 0.1f;


	E_BGMMode m_eBGMMode			= E_BGMMode::FadeIn;
	_float	m_fCurrentBGMVolume		= 0.f;
	_float	m_fNextBGMVolume		= 0.f;
	wstring m_wstrCurrentBGM		= L"";
	wstring m_wstrNextBGM			= L"";
	_float	m_fRatio				= 0.f;



private:
	static CSound_Manager* m_pInstance; 
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[(_int)E_ChannelID::Channel_End]; 
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM*	m_pSystem; 
	_float			fCool = 0.f;
	static int		g_iSoundStack;

	
	map<TCHAR*, _float>			m_mapThisFrameSound;
};

#endif // !__SOUNDMANAGER_H__