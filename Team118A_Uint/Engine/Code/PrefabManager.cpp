#include "PrefabManager.h"


USING(Engine)

IMPLEMENT_SINGLETON(CPrefabManager)

CPrefabManager::CPrefabManager()
{
	LoadFile();
}


CPrefabManager::~CPrefabManager()
{
	Free();
}

//불러온 데이터를 모두 해제하는 함수
void CPrefabManager::Free()
{
	for (auto& tPair : m_mapPrefabData)
	{
		tPair.second.clear();
		tPair.second.shrink_to_fit();
	}
	m_mapPrefabData.clear();
	FileReset();
}

//파일을 읽어서 데이터를 보관하는 함수
void CPrefabManager::LoadFile()
{
	Free();

	LoadFolder("..\\..\\Reference\\Data\\Data_Particle\\");
	LoadFolder("..\\..\\Reference\\Data\\Data_Effect\\");
	LoadFolder("..\\..\\Reference\\Data\\Data_Skill\\");
}

//파일명에 해당하는 데이터를 사이즈만큼 읽어서 pDwordByte에 읽은 사이즈와 pOut에 읽은 값을 반환하는 함수 ()
_bool CPrefabManager::ReadPrefab(wstring wstrFileName, void * pOut, size_t sizeSize, DWORD * pDwordByte)
{
	if (wstrFileName != m_wstrCurrentFileName)
	{
		m_vecCurrentFile = GetFileData(wstrFileName);
		if (m_vecCurrentFile.empty())
		{
			LMSG_BOX(L"로드하지 않은 파일 이름 입력.");
			return FALSE;
		}

		m_iReadCount = 0;
		m_wstrCurrentFileName = wstrFileName;
	}

	_int iPrevReadCount = m_iReadCount;
	m_iReadCount += _int(sizeSize);

	if (_int(m_vecCurrentFile.size()) < m_iReadCount)
	{
		_int iMaxSize = (_int)m_vecCurrentFile.size()-iPrevReadCount;
		
		*pDwordByte = (DWORD)iMaxSize;
		memcpy(pOut, &m_vecCurrentFile[iPrevReadCount], iMaxSize);
		return TRUE;
	}
	else
	{
		
		memcpy(pOut, &m_vecCurrentFile[iPrevReadCount], sizeSize);
		
		*pDwordByte = (DWORD)sizeSize;
	}

	if (_int(m_vecCurrentFile.size()) == m_iReadCount)
	{
		FileReset();
		//파일 끝까지 읽었을 때
		return TRUE;
	}

	return FALSE;
}

//현재 읽고있는 파일 정보를 초기화하는 함수
void CPrefabManager::FileReset()
{
	m_wstrCurrentFileName = L"";
	m_iReadCount = 0;
	m_vecCurrentFile.clear();
}

//파일명으로 현재 보관중인 데이터에 해당 파일이 있는지 확인후 반환하는 함수
vector<char> CPrefabManager::GetFileData(wstring wstrFileName)
{
	auto iter = find_if(m_mapPrefabData.begin(), m_mapPrefabData.end(), CTag_Finder(wstrFileName));
	if (m_mapPrefabData.end() == iter)
	{
		return vector<char>();
	}

	return (iter->second);
}

//폴더의 경로를 입력받아 해당 폴더안의 모든파일을 데이터로 보관하는 함수
void CPrefabManager::LoadFolder(string strFolder)
{
	string searching = strFolder;
	searching += "*.*";

	vector<string> vecFileName;

	_finddata_t findData;
	intptr_t handle = _findfirst(searching.c_str(), &findData);  //현재 폴더 내 모든 파일을 찾는다.
	

	if (handle == -1)
	{
		return;
	}


	int result = 0;

	do
	{
		if (strcmp(".", findData.name) && strcmp("..", findData.name))
		{
		
			vecFileName.push_back(findData.name);
		}
		result = _findnext(handle, &findData);
	} while (result != -1);

	_findclose(handle);

	for (auto& strFileName : vecFileName)
	{
		string strFullPath = strFolder+strFileName;
		

		vector<char> vecByte;

	
		HANDLE hFile = CreateFile(CUtility::CHAR_TO_WSTRING(strFullPath.c_str()).c_str(), GENERIC_READ, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			return;
		}
		DWORD dwByte=0;
		char cByte=0;
		
		while (TRUE)
		{
			ReadFile(hFile, &cByte, sizeof(char), &dwByte, nullptr);
			if (0 == dwByte)
			{
				break;
			}

			vecByte.emplace_back(cByte);
		}

		CloseHandle(hFile);
		m_mapPrefabData.emplace(CUtility::CHAR_TO_WSTRING(strFileName.c_str()),vecByte);
	}

	return;
}
