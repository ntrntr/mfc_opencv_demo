
// mfc_opencv_demo.h : mfc_opencv_demo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cmfc_opencv_demoApp:
// �йش����ʵ�֣������ mfc_opencv_demo.cpp
//

class Cmfc_opencv_demoApp : public CWinApp
{
public:
	Cmfc_opencv_demoApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMymenuSubsense();
};

extern Cmfc_opencv_demoApp theApp;
