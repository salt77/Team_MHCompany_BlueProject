CBase::CBase()
	: m_dwRefCnt(0)
{

}
CBase::~CBase()
{

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}
unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}

/*
클래스 안에서 멤버 변수와 메서드의 등장 순서는 다음을 따른다.

friend 클래스들

public 메서드들
protected 메서드들
private 메서드들

protected 변수들
private 변수들
public 변수들

*/