
// MFC_ProjectView.cpp : CMFC_ProjectView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFC_Project.h"
#endif

#include "MFC_ProjectDoc.h"
#include "MFC_ProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_ProjectView

IMPLEMENT_DYNCREATE(CMFC_ProjectView, CView)

BEGIN_MESSAGE_MAP(CMFC_ProjectView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFC_ProjectView ����/�Ҹ�

CMFC_ProjectView::CMFC_ProjectView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMFC_ProjectView::~CMFC_ProjectView()
{
}

BOOL CMFC_ProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFC_ProjectView �׸���

void CMFC_ProjectView::OnDraw(CDC* /*pDC*/)
{
	CMFC_ProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFC_ProjectView �μ�

BOOL CMFC_ProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFC_ProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFC_ProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFC_ProjectView ����

#ifdef _DEBUG
void CMFC_ProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_ProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_ProjectDoc* CMFC_ProjectView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_ProjectDoc)));
	return (CMFC_ProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_ProjectView �޽��� ó����
