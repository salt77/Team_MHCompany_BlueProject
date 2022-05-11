#pragma once
#ifndef __PREFAB_MANAGER_H__
#define __PREFAB_MANAGER_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPrefabManager :
	public CBase
{
	DECLARE_SINGLETON(CPrefabManager)
private:
	CPrefabManager();
	virtual ~CPrefabManager();

public:
	virtual void Free() override;



	void LoadFile();

	_bool ReadPrefab(wstring wstrFileName, void* pOut,size_t sizeSize,DWORD* pDwordByte);
	void FileReset();
private:
	void LoadFolder(string strFolder);
	vector<char>	GetFileData(wstring wstrFileName);
private:
	map<wstring, vector<char>> m_mapPrefabData;
	
	wstring m_wstrCurrentFileName=L"";
	_int	m_iReadCount=0;
	vector<char> m_vecCurrentFile;
};

END

#endif