
// mfc_opencv_demoView.cpp : Cmfc_opencv_demoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "mfc_opencv_demo.h"
#endif

#include "mfc_opencv_demoDoc.h"
#include "mfc_opencv_demoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MyTestDialog.h"
#include "IntegerToRoman.h"
// Cmfc_opencv_demoView

IMPLEMENT_DYNCREATE(Cmfc_opencv_demoView, CView)

BEGIN_MESSAGE_MAP(Cmfc_opencv_demoView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_MYTEST, &Cmfc_opencv_demoView::OnMytest)
	ON_COMMAND(ID_Integer_To_Roman, &Cmfc_opencv_demoView::OnIntegerToRoman)
END_MESSAGE_MAP()

// Cmfc_opencv_demoView ����/����

Cmfc_opencv_demoView::Cmfc_opencv_demoView()
{
	// TODO: �ڴ˴���ӹ������

}

Cmfc_opencv_demoView::~Cmfc_opencv_demoView()
{
}

BOOL Cmfc_opencv_demoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cmfc_opencv_demoView ����

void Cmfc_opencv_demoView::OnDraw(CDC* /*pDC*/)
{
	Cmfc_opencv_demoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//IplImage* test;
	//test = cvLoadImage("D://blue.bmp");
	//cvNamedWindow("hell world",1);
	//cvShowImage("hell world",test);
	////cvWaitKey(0);
	//cvDestroyWindow("hell world");
	//cvReleaseImage(&test);
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cmfc_opencv_demoView ��ӡ

BOOL Cmfc_opencv_demoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cmfc_opencv_demoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cmfc_opencv_demoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// Cmfc_opencv_demoView ���

#ifdef _DEBUG
void Cmfc_opencv_demoView::AssertValid() const
{
	CView::AssertValid();
}

void Cmfc_opencv_demoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmfc_opencv_demoDoc* Cmfc_opencv_demoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmfc_opencv_demoDoc)));
	return (Cmfc_opencv_demoDoc*)m_pDocument;
}
#endif //_DEBUG


// Cmfc_opencv_demoView ��Ϣ�������




void Cmfc_opencv_demoView::OnMytest()
{
	// TODO: �ڴ���������������
	MyTestDialog* dlg;
	dlg = new MyTestDialog();
	if (dlg != NULL)
	{
		BOOL ret = dlg->Create(IDD_MYTESTDIALOG, this);
		if (!ret)
		{
			MessageBox("create failed");
		}
		//CRect clientRect,dlgRect;
		//GetClientRect(&clientRect);								//��ȡ�ͻ������ڴ�С
		//ClientToScreen(clientRect);								//ת��Ϊ��Ļ����
		//pDialog->GetWindowRect(&dlgRect);						//��ȡ�Ի��򴰿ڴ�С
		////�ƶ��Ի��򴰿�
		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
		dlg->ShowWindow(SW_SHOW);						//��ʾ�Ի���
		//szOut="��������ָ��λ����ʾ��ģ̬�Ի���";
		//Invalidate(true);//������ʾ
	}
	else
	{
		MessageBox("create failed");
	}
}


void Cmfc_opencv_demoView::OnIntegerToRoman()
{
	// TODO: �ڴ���������������
	IntegerToRoman* dlg;
	dlg = new IntegerToRoman();
	if (dlg != NULL)
	{
		BOOL ret = dlg->Create(IDD_INTEGERTOROMAN, this);
		if (!ret)
		{
			MessageBox("create failed");
		}
		//CRect clientRect,dlgRect;
		//GetClientRect(&clientRect);								//��ȡ�ͻ������ڴ�С
		//ClientToScreen(clientRect);								//ת��Ϊ��Ļ����
		//pDialog->GetWindowRect(&dlgRect);						//��ȡ�Ի��򴰿ڴ�С
		////�ƶ��Ի��򴰿�
		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
		dlg->ShowWindow(SW_SHOW);						//��ʾ�Ի���
		//szOut="��������ָ��λ����ʾ��ģ̬�Ի���";
		//Invalidate(true);//������ʾ
	}
	else
	{
		MessageBox("create failed");
	}
}
