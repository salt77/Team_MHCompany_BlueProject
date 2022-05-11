#ifndef __Engine_Function_H__
#define __Engine_Function_H__

namespace Engine
{
	template <typename T>
	DWORD Safe_Release(T& pointer)			
	{
		DWORD	dwRefCnt = 0;
		if (NULL != pointer)
		{
			dwRefCnt = pointer->Release();

			if (dwRefCnt == 0)
				pointer = NULL;		// 댕글링 포인터를 만들지 않기 위함
		}
		return dwRefCnt;
	}

	template <typename T>
	void Safe_Single_Destory(T& pointer)
	{
		if (NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete(T& pointer)
	{
		if (NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer)
	{
		if (NULL != pointer)
		{
			delete[] pointer;
			pointer = NULL;
		}
	}
	//////////////////////////////////////////////////////////////////
	/////////////////////////////Functor//////////////////////////////

	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const wstring& pTag)
			: m_pTargetTag(pTag)
		{
		}
		~CTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(const T& pair)
		{
			if (0==lstrcmpW(m_pTargetTag.c_str(),pair.first.c_str()))
			{
				return true;
			}

			return false;
		}

	private:
		const wstring		m_pTargetTag;
	};

	class CEnumTag_Finder
	{
	public:
		explicit CEnumTag_Finder(const int& iTag)
			: m_iTargetTag(iTag)
		{
		}
		~CEnumTag_Finder() {		}
	public:
		template<typename T>
		bool operator()(T& pair)
		{
			if (int(m_iTargetTag) == int(pair.first))
			{
				return true;
			}

			return false;
		}

	private:
		int		m_iTargetTag;
	};

	class CDeleteObj
	{
	public:
		CDeleteObj() {}
		~CDeleteObj() {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			unsigned long dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		CDeleteMap() {}
		~CDeleteMap() {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			unsigned long dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};
}


#endif // __Engine_Function_H__
