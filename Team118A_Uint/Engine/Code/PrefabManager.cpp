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

//�ҷ��� �����͸� ��� �����ϴ� �Լ�
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

//������ �о �����͸� �����ϴ� �Լ�
void CPrefabManager::LoadFile()
{
	Free();

	LoadFolder("..\\..\\Reference\\Data\\Data_Particle\\");
	LoadFolder("..\\..\\Reference\\Data\\Data_Effect\\");
	LoadFolder("..\\..\\Reference\\Data\\Data_Skill\\");
}

//���ϸ� �ش��ϴ� �����͸� �����ŭ �о pDwordByte�� ���� ������� pOut�� ���� ���� ��ȯ�ϴ� �Լ� ()
_bool CPrefabManager::ReadPrefab(wstring wstrFileName, void * pOut, size_t sizeSize, DWORD * pDwordByte)
{
	if (wstrFileName != m_wstrCurrentFileName)
	{
		m_vecCurrentFile = GetFileData(wstrFileName);
		if (m_vecCurrentFile.empty())
		{
			LMSG_BOX(L"�ε����� ���� ���� �̸� �Է�.");
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
		//���� ������ �о��� ��
		return TRUE;
	}

	return FALSE;
}

//���� �а��ִ� ���� ������ �ʱ�ȭ�ϴ� �Լ�
void CPrefabManager::FileReset()
{
	m_wstrCurrentFileName = L"";
	m_iReadCount = 0;
	m_vecCurrentFile.clear();
}

//���ϸ����� ���� �������� �����Ϳ� �ش� ������ �ִ��� Ȯ���� ��ȯ�ϴ� �Լ�
vector<char> CPrefabManager::GetFileData(wstring wstrFileName)
{
	auto iter = find_if(m_mapPrefabData.begin(), m_mapPrefabData.end(), CTag_Finder(wstrFileName));
	if (m_mapPrefabData.end() == iter)
	{
		return vector<char>();
	}

	return (iter->second);
}

//������ ��θ� �Է¹޾� �ش� �������� ��������� �����ͷ� �����ϴ� �Լ�
void CPrefabManager::LoadFolder(string strFolder)
{
	string searching = strFolder;
	searching += "*.*";

	vector<string> vecFileName;

	_finddata_t findData;
	intptr_t handle = _findfirst(searching.c_str(), &findData);  //���� ���� �� ��� ������ ã�´�.
	

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
