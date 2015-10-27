#pragma once


// SubSense 对话框

class SubSense : public CDialog
{
	DECLARE_DYNAMIC(SubSense)

public:
	SubSense(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SubSense();

// 对话框数据
	enum { IDD = IDD_SUBSENSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSubsense();
	void DrawPicToHDC(IplImage* img, UINT ID);
	void DrawPicToHDC(cv::Mat& img, UINT ID);
	void DoSubsense(CString pathName, bool openCameral = false);
	afx_msg void OnBnClickedButtonBreak();
private:
	bool isStop;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	UINT m_nTimer;
public:
	afx_msg void OnBnClickedButtonStart1();
	afx_msg void OnBnClickedButtonEnd1();
	afx_msg void OnBnClickedButtonStart2();
	afx_msg void OnBnClickedButtonend2();
	static UINT __cdecl MyThreadFun(LPVOID pParam);
	afx_msg void OnBnClickedButtonSuspend();
	afx_msg void OnBnClickedButtonResume();
	CWinThread* mythread;
	int m_k;
};
