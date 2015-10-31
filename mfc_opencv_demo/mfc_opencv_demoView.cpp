
// mfc_opencv_demoView.cpp : Cmfc_opencv_demoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_MYTEST, &Cmfc_opencv_demoView::OnMytest)
	ON_COMMAND(ID_Integer_To_Roman, &Cmfc_opencv_demoView::OnIntegerToRoman)
END_MESSAGE_MAP()

// Cmfc_opencv_demoView 构造/析构

Cmfc_opencv_demoView::Cmfc_opencv_demoView()
{
	// TODO: 在此处添加构造代码

}

Cmfc_opencv_demoView::~Cmfc_opencv_demoView()
{
}

BOOL Cmfc_opencv_demoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cmfc_opencv_demoView 绘制

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
	// TODO: 在此处为本机数据添加绘制代码
}


// Cmfc_opencv_demoView 打印

BOOL Cmfc_opencv_demoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cmfc_opencv_demoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cmfc_opencv_demoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Cmfc_opencv_demoView 诊断

#ifdef _DEBUG
void Cmfc_opencv_demoView::AssertValid() const
{
	CView::AssertValid();
}

void Cmfc_opencv_demoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cmfc_opencv_demoDoc* Cmfc_opencv_demoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cmfc_opencv_demoDoc)));
	return (Cmfc_opencv_demoDoc*)m_pDocument;
}
#endif //_DEBUG


// Cmfc_opencv_demoView 消息处理程序




void Cmfc_opencv_demoView::OnMytest()
{
	// TODO: 在此添加命令处理程序代码
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
		//GetClientRect(&clientRect);								//获取客户区窗口大小
		//ClientToScreen(clientRect);								//转换为屏幕坐标
		//pDialog->GetWindowRect(&dlgRect);						//获取对话框窗口大小
		////移动对话框窗口
		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
		dlg->ShowWindow(SW_SHOW);						//显示对话框
		//szOut="创建并在指定位置显示非模态对话框";
		//Invalidate(true);//更新显示
	}
	else
	{
		MessageBox("create failed");
	}
}


void Cmfc_opencv_demoView::OnIntegerToRoman()
{
	// TODO: 在此添加命令处理程序代码
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
		//GetClientRect(&clientRect);								//获取客户区窗口大小
		//ClientToScreen(clientRect);								//转换为屏幕坐标
		//pDialog->GetWindowRect(&dlgRect);						//获取对话框窗口大小
		////移动对话框窗口
		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
		dlg->ShowWindow(SW_SHOW);						//显示对话框
		//szOut="创建并在指定位置显示非模态对话框";
		//Invalidate(true);//更新显示
	}
	else
	{
		MessageBox("create failed");
	}
}
