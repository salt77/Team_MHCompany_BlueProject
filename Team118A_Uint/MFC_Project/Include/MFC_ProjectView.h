
// MFC_ProjectView.h : CMFC_ProjectView Ŭ������ �������̽�
//

#pragma once


class CMFC_ProjectView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFC_ProjectView();
	DECLARE_DYNCREATE(CMFC_ProjectView)

// Ư���Դϴ�.
public:
	CMFC_ProjectDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMFC_ProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MFC_ProjectView.cpp�� ����� ����
inline CMFC_ProjectDoc* CMFC_ProjectView::GetDocument() const
   { return reinterpret_cast<CMFC_ProjectDoc*>(m_pDocument); }
#endif

