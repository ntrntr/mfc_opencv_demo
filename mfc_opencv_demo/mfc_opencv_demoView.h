
// mfc_opencv_demoView.h : Cmfc_opencv_demoView ��Ľӿ�
//

#pragma once
#include "MyTestDialog.h"

class Cmfc_opencv_demoView : public CView
{
protected: // �������л�����
	Cmfc_opencv_demoView();
	DECLARE_DYNCREATE(Cmfc_opencv_demoView)

// ����
public:
	Cmfc_opencv_demoDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cmfc_opencv_demoView();
	MyTestDialog myDioalog;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnMytest();
	afx_msg void OnIntegerToRoman();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // mfc_opencv_demoView.cpp �еĵ��԰汾
inline Cmfc_opencv_demoDoc* Cmfc_opencv_demoView::GetDocument() const
   { return reinterpret_cast<Cmfc_opencv_demoDoc*>(m_pDocument); }
#endif

