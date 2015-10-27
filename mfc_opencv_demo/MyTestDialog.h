#pragma once
#include "afxwin.h"



#define   WIDTHBYTES(bits) (((bits)+31)/32*4)//用于使图像宽度所占字节数为4byte的倍数

// MyTestDialog 对话框

class MyTestDialog : public CDialog
{
	DECLARE_DYNAMIC(MyTestDialog)

public:
	MyTestDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyTestDialog();

// 对话框数据
	enum { IDD = IDD_MYTESTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOpenimage();
	virtual BOOL OnInitDialog();
	void DrawPicToHDC(IplImage* img, UINT ID);
	void DrawPicToHDC(cv::Mat& img, UINT ID);
	void drawpic(IplImage* img, unsigned int id);
	afx_msg void OnBnClickedOpenimagemat();
	afx_msg void OnBnClickedVibe();
	void DoMyVibe(CString& filePath, bool openCamera = false);
	CEdit size;
	int mySize;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSuspend();
	afx_msg void OnBnClickedButtonResume();
private:
	CWinThread* mythread;
	bool isContinue;
public:
	// 	static UINT __cdecl MyThreadFun(LPVOID pParam);
	static UINT DoVibe(LPVOID pParam);
	afx_msg void OnBnClickedButtonEnd3();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
