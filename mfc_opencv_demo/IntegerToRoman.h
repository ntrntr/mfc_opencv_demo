#pragma once


// IntegerToRoman �Ի���

class IntegerToRoman : public CDialogEx
{
	DECLARE_DYNAMIC(IntegerToRoman)

public:
	IntegerToRoman(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~IntegerToRoman();

// �Ի�������
	enum { IDD = IDD_INTEGERTOROMAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
