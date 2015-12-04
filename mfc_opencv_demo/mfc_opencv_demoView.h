
// mfc_opencv_demoView.h : Cmfc_opencv_demoView 类的接口
//

#pragma once
#include "MyTestDialog.h"

class Cmfc_opencv_demoView : public CView
{
protected: // 仅从序列化创建
	Cmfc_opencv_demoView();
	DECLARE_DYNCREATE(Cmfc_opencv_demoView)

// 特性
public:
	Cmfc_opencv_demoDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cmfc_opencv_demoView();
	MyTestDialog myDioalog;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnMytest();
	afx_msg void OnIntegerToRoman();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // mfc_opencv_demoView.cpp 中的调试版本
inline Cmfc_opencv_demoDoc* Cmfc_opencv_demoView::GetDocument() const
   { return reinterpret_cast<Cmfc_opencv_demoDoc*>(m_pDocument); }
#endif

