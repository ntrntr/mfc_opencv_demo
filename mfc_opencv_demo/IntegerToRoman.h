#pragma once


// IntegerToRoman 对话框

class IntegerToRoman : public CDialogEx
{
	DECLARE_DYNAMIC(IntegerToRoman)

public:
	IntegerToRoman(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IntegerToRoman();

// 对话框数据
	enum { IDD = IDD_INTEGERTOROMAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnIntegerToRoman();
	int m_integer;
	CString intToRoman(int num);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
};
