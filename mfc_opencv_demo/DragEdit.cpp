// DragEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "DragEdit.h"


// DragEdit

IMPLEMENT_DYNAMIC(DragEdit, CEdit)

DragEdit::DragEdit()
{

	filePath = _T("");
}

DragEdit::~DragEdit()
{
}


BEGIN_MESSAGE_MAP(DragEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// DragEdit ��Ϣ�������




int DragEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	DragAcceptFiles(TRUE);
		return 0;
}


void DragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: Add your message handler code here and/or call default
	WORD wNumFilesDropped = DragQueryFile(hDropInfo, -1, NULL, 0);//ͬʱ�Ϸ��ļ�����Ŀ
	CString firstFile="";
	for (WORD x = 0 ; x < wNumFilesDropped; x++) //ֻ��¼��һ���϶����ļ�
	{
		WORD wPathnameSize = DragQueryFile(hDropInfo, x, NULL, 0);
		char * npszFile = (char *) LocalAlloc(LPTR, wPathnameSize += 1);//�����ڴ�
		if (npszFile == NULL) 
			continue;
		DragQueryFile(hDropInfo, x, npszFile, wPathnameSize);//��ȡ�Ϸŵ��ļ���
		if (firstFile=="")
			firstFile=npszFile;//��¼�ļ���
		LocalFree(npszFile);//�ͷ��ڴ�
	}
	DragFinish(hDropInfo);//�ϷŽ���
	SetWindowText(firstFile);//�༭����ʾ
	//CEdit::OnDropFiles(hDropInfo);
	filePath = firstFile;
	//CEdit::OnDropFiles(hDropInfo);
}
