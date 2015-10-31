// IntegerToRoman.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "IntegerToRoman.h"
#include "afxdialogex.h"
#include "string.h"

// IntegerToRoman 对话框

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


// IntegerToRoman 消息处理程序


//void IntegerToRoman::OnIntegerToRoman()
//{
//	// TODO: 在此添加命令处理程序代码
//	// TODO: 在此添加命令处理程序代码
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
//		//GetClientRect(&clientRect);								//获取客户区窗口大小
//		//ClientToScreen(clientRect);								//转换为屏幕坐标
//		//pDialog->GetWindowRect(&dlgRect);						//获取对话框窗口大小
//		////移动对话框窗口
//		//pDialog->MoveWindow(clientRect.left+30, clientRect.top+50,dlgRect.Width(),dlgRect.Height());
//		dlg->ShowWindow(SW_SHOW);						//显示对话框
//		//szOut="创建并在指定位置显示非模态对话框";
//		//Invalidate(true);//更新显示
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
		return "最大到4000";
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
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));
}


void IntegerToRoman::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
	//UpdateData();
	CString tmp;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(tmp);
	m_integer = _ttoi(tmp);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));
}


void IntegerToRoman::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_integer = 0;
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(intToRoman(m_integer));

}


void IntegerToRoman::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString source;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(source);
	//文本内容保存在source变量中
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
