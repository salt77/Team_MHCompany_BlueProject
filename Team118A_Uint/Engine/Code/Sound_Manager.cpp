#include "Sound_Manager.h"

CSound_Manager* CSound_Manager::m_pInstance = nullptr;
_int			CSound_Manager::g_iSoundStack = 0;

CSound_Manager::CSound_Manager()
{
	m_pSystem = nullptr; 
}

CSound_Manager::~CSound_Manager()
{
	Release(); 
}

// Sound_Manager�� �ʱ�ȭ �۾��� ���� m_vecBGM�� bgm�̸��� �־��ش�.
void CSound_Manager::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	
	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	FMOD_System_Init(m_pSystem, (_int)E_ChannelID::Channel_End, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();

	FMOD_Channel_SetMode(m_pChannelArr[(_int)E_ChannelID::BGM], FMOD_LOOP_NORMAL);
}

// Sound�� Release
void CSound_Manager::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

// ī�޶� ��尡 FadeIn FadeOut�� �� �� ������ ���ο� Volume���� �ٲ�
void CSound_Manager::Update_SoundManager(const _float& fTimeDelta)
{
	if (L"" == m_wstrCurrentBGM)
		return;

	switch (m_eBGMMode)
	{
	case E_BGMMode::FadeIn:
		if (m_fRatio < 1.f)
		{
			m_fRatio += fTimeDelta;
			FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], m_fCurrentBGMVolume * m_fRatio);
		}
		else
		{
			FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], m_fCurrentBGMVolume * 1.f);
			m_eBGMMode = E_BGMMode::Normal;
		}
		break;
	case E_BGMMode::FadeOut:
		if (m_fRatio > 0.f)
		{
			m_fRatio -= fTimeDelta;
			FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], m_fCurrentBGMVolume * m_fRatio);
		}
		else
		{
			m_fCurrentBGMVolume = m_fNextBGMVolume;
			m_wstrCurrentBGM = wstring(m_wstrNextBGM);
			map<TCHAR*, FMOD_SOUND*>::iterator iter;

			iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
			{
				return !lstrcmp(m_wstrNextBGM.c_str(), iter.first);
			});
			FMOD_Channel_Stop(m_pChannelArr[(_int)E_ChannelID::BGM]);
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[(_int)E_ChannelID::BGM]);
			FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], m_fCurrentBGMVolume * 0.f);
			FMOD_System_Update(m_pSystem);

			m_eBGMMode = E_BGMMode::FadeIn;
		}
		break;
	case E_BGMMode::Normal:
		break;
	case E_BGMMode::Stop:
		break;
	default:
		break;
	}

	for (auto& iter : m_mapThisFrameSound)
	{
		PlayStackSound(iter.first, E_SoundType::FX, iter.second);
	}
	m_mapThisFrameSound.clear();

}

// ���带 _volume�� ���� ����. eSoundType�� ���� ������ Ratio�� �ٸ��� ���
void CSound_Manager::PlaySound_(TCHAR * pSoundKey, E_ChannelID eID, E_SoundType eSoundType, float _volume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[(_int)eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[(_int)eID]);
	}

	_float fVolume = _volume * Get_SoundRatio(eSoundType);
	FMOD_Channel_SetVolume(m_pChannelArr[(_int)eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

// 30���� ���� ä���� ��ȸ�ϸ鼭 �������ش�.
// ���� : ���� ���带 ����ϱ����� �۾��� ���� �������ʾƵ� ��ȸ�ϸ� ����
void CSound_Manager::PlayStackSound(const TCHAR * pSoundKey, E_SoundType eSoundType, float _volume, float & CoolTime)
{
	CoolTime = 0.f;
	PlayStackSound(pSoundKey, eSoundType, _volume);
}

// 30���� ���� ä���� ��ȸ�ϸ鼭 �������ش�.
// ���� : ���� ���带 ����ϱ����� �۾��� ���� �������ʾƵ� ��ȸ�ϸ� ����
void CSound_Manager::PlayStackSound(const TCHAR * pSoundKey, E_SoundType eSoundType, float _volume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;
	FMOD_BOOL bPlay = FALSE;
	g_iSoundStack++;
	g_iSoundStack %= 30;
	
	_float fVolume = _volume * Get_SoundRatio(eSoundType);

	if (FMOD_Channel_IsPlaying(m_pChannelArr[g_iSoundStack], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[g_iSoundStack]);
	}
	FMOD_Channel_SetVolume(m_pChannelArr[g_iSoundStack], fVolume);
	FMOD_System_Update(m_pSystem);
}

// BGM�� �����ϴ� �Լ� BGM�� Loop��尡 True�� �ݺ�����ȴ�.
// ���۰� ���ÿ� FadeOut���� �����ϰ� ���� BGM�� pSoundKey�� �ٲ��ش�.
void CSound_Manager::PlayBGM(TCHAR * pSoundKey, float _volume, _bool bFade)
{
	if (m_wstrCurrentBGM == wstring(pSoundKey))
	{
		return;
	}
	else
	{
		m_eBGMMode = E_BGMMode::FadeOut;
		m_wstrNextBGM = wstring(pSoundKey);
		m_fNextBGMVolume = _volume;
	}

	if (FALSE == bFade)
	{
		m_eBGMMode = E_BGMMode::Normal;
		m_wstrCurrentBGM = wstring(pSoundKey);
		m_wstrNextBGM = wstring(pSoundKey);
		m_fCurrentBGMVolume = _volume;

		map<TCHAR*, FMOD_SOUND*>::iterator iter;

		iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

		if (iter == m_mapSound.end())
			return;

		_float fVolume = _volume * Get_SoundRatio(E_SoundType::BGM);
		FMOD_Channel_Stop(m_pChannelArr[(_int)E_ChannelID::BGM]);
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[(_int)E_ChannelID::BGM]);
		FMOD_Channel_SetMode(m_pChannelArr[(_int)E_ChannelID::BGM], FMOD_LOOP_NORMAL);
		FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], fVolume);
		FMOD_System_Update(m_pSystem);
	}

}

// eID ä���� ��� �����.
void CSound_Manager::StopSound(E_ChannelID eID)
{
	if (E_ChannelID::BGM == eID)
	{
		m_eBGMMode = E_BGMMode::Stop;
		m_fRatio = 0.f;
		m_wstrCurrentBGM = L"";
		m_wstrNextBGM = L"";
	}
	
	FMOD_Channel_Stop(m_pChannelArr[(_int)eID]);
}

// ��� ä�ο��� ����Ǵ� ���带 �����.
void CSound_Manager::StopAll()
{
	m_eBGMMode = E_BGMMode::FadeOut;
	for (int i = 0 ; i < (_int)E_ChannelID::Channel_End; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

// BGM�� ������ �����Ѵ�.
void CSound_Manager::Set_Volume_BGM(_float fRatio)
{
	m_fBGM_Volume = CUtility::Clamp_float(fRatio, 0.f, 1.f);
	FMOD_Channel_SetVolume(m_pChannelArr[(_int)E_ChannelID::BGM], m_fBGM_Volume * m_fBGM_Ratio);
}

// FX�� ������ �����Ѵ�.
void CSound_Manager::Set_Volume_FX(_float fRatio)
{
	m_fFX_Ratio = CUtility::Clamp_float(fRatio, 0.f, 1.f);
}

// Voice�� ������ �����Ѵ�.
void CSound_Manager::Set_Volume_Voice(_float fRatio)
{
	m_fVoice_Ratio = CUtility::Clamp_float(fRatio, 0.f, 1.f);
}

void CSound_Manager::Add_Sound_NotOverlap(TCHAR* szFileName, _float fVolume)
{
	m_mapThisFrameSound.emplace(szFileName, fVolume);
}

// ���带 �����ö� eType���� �����ϰ� �������� ���� �Լ�
_float CSound_Manager::Get_SoundRatio(E_SoundType eType)
{
	switch (eType)
	{
	case E_SoundType::FX:
		return m_fFX_Ratio;
	case E_SoundType::BGM:
		return m_fBGM_Ratio;
	case E_SoundType::Voice:
		return m_fVoice_Ratio;
	default:
		assert(FALSE);
		break;
	}
	return 0.1f;
}

// ../../Sound/ �����ȿ��ִ� ��� ���� ������ ��ȸ�ؼ� map�� ����ش�.
void CSound_Manager::LoadSoundFile()
{
	_finddata_t fd; 

	intptr_t handle = _findfirst("../../Sound/*.*", &fd);

	if (handle == 0)
		return; 

	int iResult = 0; 

	char szCurPath[128] = "../../Sound/";
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr; 

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			size_t iLength = strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, (int)iLength, pSoundKey, (int)iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}
