#pragma once
#include "afxwin.h"
#include "DragEdit.h"


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
	
//	afx_msg void OnBnClickedOpenimage();
	virtual BOOL OnInitDialog();
	void DrawPicToHDC(IplImage* img, UINT ID);
	void DrawPicToHDC(cv::Mat& img, UINT ID);
	void drawpic(IplImage* img, unsigned int id);
//	afx_msg void OnBnClickedOpenimagemat();
//	afx_msg void OnBnClickedVibe();
//	void DoMyVibe(CString& filePath, bool openCamera = false);
	//CEdit size;
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
	afx_msg void OnBnClickedButtonEnd();
//	afx_msg void OnDropFiles(HDROP hDropInfo);
	int m_radio;
	afx_msg void OnEnChangeEditvibesize();
	int m_subsense_k;
	afx_msg void OnEnChangeEditsubsensek();
	static UINT MyThreadSubsense(LPVOID pParam);
	BOOL m_isSave;
//	DragEdit m_ctlEdit1;
	DragEdit m_ctlEdit1;
	void DrawMatUseCImage(cv::Mat& img, UINT ID);
	void DrawMatUseBitMap(cv::Mat& img, UINT ID);
	CImage* m_pImg;
	void modifyMask(cv::Mat& mask);
	void cvConnectedComponents(IplImage *mask, int poly1_hull0 = 1, float perimScale = 4.0 , int *num = NULL, CvRect *bbs = NULL, CvPoint *centers = NULL);
	static UINT DoCodeBook(LPVOID pParam);
	int m_cal_speed;
};
