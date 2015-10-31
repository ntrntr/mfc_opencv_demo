// IntegerToRoman.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "IntegerToRoman.h"
#include "afxdialogex.h"
#include "string.h"

// IntegerToRoman �Ի���

IMPLEMENT_DYNAMIC(IntegerToRoman, CDialogEx)

IntegerToRoman::IntegerToRoman(CWnd* pParent /*=NULL*/)
	: CDialogEx(IntegerToRoman::IDD, pParent)
	, m_integer(2015)
{

}

IntegerToRoman::~IntegerToRoman()
{
}

void IntegerToRoman::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_EDIT1, m_integer);
//	DDV_MinMaxInt(pDX, m_integer, 0, 5000);
}


BEGIN_MESSAGE_MAP(IntegerToRoman, CDialogEx)
	//ON_COMMAND(ID_Integer_To_Roman, &IntegerToRoman::OnIntegerToRoman)
	ON_BN_CLICKED(IDC_BUTTON2, &IntegerToRoman::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &IntegerToRoman::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &IntegerToRoman::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &IntegerToRoman::OnBnClickedButton3)
END_MESSAGE_MAP()


// IntegerToRoman ��Ϣ�������


//void IntegerToRoman::OnIntegerToRoman()
//{
//	// TODO: �ڴ���������������
//	// TODO: �ڴ���������������
//	IntegerToRoman* dlg;
//	dlg = new IntegerToRoman();
//	if (dlg != NULL)
//	{
//		BOOL ret = dlg->Create(IDD_INTEGERTOROMAN, this);
//		if (!ret)
//		{
//			MessageBox("create failed");
//		}
//		//CRect clientRect,dlgRect;
//		//GetClientRect(&clientRect);								//��ȡ�ͻ������ڴ�С
//		//ClientToScreen(clientRect);								//ת��Ϊ��Ļ����
//		//pDialog->GetWindowRect(&dlgRect);						//��ȡ�Ի��򴰿ڴ�С
//		////�ƶ��Ի��򴰿�
//		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
//		dlg->ShowWindow(SW_SHOW);						//��ʾ�Ի���
//		//szOut="��������ָ��λ����ʾ��ģ̬�Ի���";
//		//Invalidate(true);//������ʾ
//	}
//	else
//	{
//		MessageBox("create failed");
//	}
//}


CString IntegerToRoman::intToRoman(int num)
{
	if (num == 0)
	{
		return "";
	}
	if (num > 4000)
	{
		return "���4000";
	}
	std::string res;
	const int radix[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
	const std::string symbom[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"} ;
	for (int i = 0; num > 0; ++i)
	{
		int count = num / radix[i];
		num = num % radix[i];
		for (int j = 0; j < count; ++j)
		{
			res += symbom[i];
		}
	}
	CString tmp(res.c_str());
	return tmp;
}


void IntegerToRoman::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));
}


void IntegerToRoman::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//UpdateData();
	CString tmp;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(tmp);
	m_integer = _ttoi(tmp);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));
}


void IntegerToRoman::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_integer = 0;
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));

}


void IntegerToRoman::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString source;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(source);
	//�ı����ݱ�����source������
	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(source));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
}
