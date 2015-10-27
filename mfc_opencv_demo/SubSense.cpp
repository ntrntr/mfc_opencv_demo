// SubSense.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "SubSense.h"
#include "afxdialogex.h"
#include "BackgroundSubtractorSuBSENSE.h"
using namespace cv;
using namespace std;

// SubSense 对话框

IMPLEMENT_DYNAMIC(SubSense, CDialog)

SubSense::SubSense(CWnd* pParent /*=NULL*/)
	: CDialog(SubSense::IDD, pParent)
	, m_k(100)
{
	isStop = false;
	m_nTimer = 0;
}

SubSense::~SubSense()
{
}

void SubSense::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_k);
	DDV_MinMaxInt(pDX, m_k, 0, 100);
}


BEGIN_MESSAGE_MAP(SubSense, CDialog)
//	ON_BN_CLICKED(IDC_Subsense, &SubSense::OnBnClickedSubsense)
	ON_BN_CLICKED(IDC_BUTTON_Break, &SubSense::OnBnClickedButtonBreak)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Start1, &SubSense::OnBnClickedButtonStart1)
	ON_BN_CLICKED(IDC_BUTTON_End1, &SubSense::OnBnClickedButtonEnd1)
	ON_BN_CLICKED(IDC_BUTTON_Start2, &SubSense::OnBnClickedButtonStart2)
	ON_BN_CLICKED(IDC_BUTTON_end2, &SubSense::OnBnClickedButtonend2)
	ON_BN_CLICKED(IDC_BUTTON_Suspend, &SubSense::OnBnClickedButtonSuspend)
	ON_BN_CLICKED(IDC_BUTTON_Resume, &SubSense::OnBnClickedButtonResume)
END_MESSAGE_MAP()


// SubSense 消息处理程序


BOOL SubSense::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("D://dataset//dataset//cameraJitter//traffic//input//in%06d.jpg");
	return TRUE;
	// 异常: OCX 属性页应返回 FALSE
}


void SubSense::OnBnClickedSubsense()
{
	//已废弃
	// TODO: 在此添加控件通知处理程序代码
	CString tmp;
	GetDlgItemText(IDC_EDIT1, tmp);
	if (tmp.IsEmpty())
	{
		MessageBox("path is empty!!!");
	}
	DoSubsense(tmp);

}

void SubSense::DrawPicToHDC(IplImage* img, UINT ID)
{
	//清空
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	//pStatic->SetBitmap(NULL);
	CDC* pDC = GetDlgItem(ID)->GetDC();
	CRect rect;
	pStatic->GetClientRect(&rect);
	//pStatic->GetDC()->FillSolidRect(rect.left ,rect.top ,rect.Width(),rect.Height(), RGB(240, 240, 240));
	////
	HDC hDc = pDC->GetSafeHdc();
	//CRect rect;
	//GetDlgItem(ID)->GetClientRect(&rect);
	float widthRatio = (float)rect.Width() / img->width;
	float heightRatio = (float)rect.Height() / img->height;
	float resRatio = widthRatio < heightRatio ? widthRatio: heightRatio;
	int resWidth = img->width * resRatio;
	int resHeight = img->height * resRatio;
	IplImage* img2=cvCreateImage(cvSize(resWidth, resHeight),img->depth, img->nChannels);
	cvResize(img, img2, CV_INTER_NN);

	img = img2;
	CvvImage cimg;
	cimg.CopyOf(img);
	cvReleaseImage(&img2);
	CRect drawRect;
	drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + img->width - 1, rect.TopLeft().y + img->height - 1);
	cimg.DrawToHDC(hDc, &drawRect);
	ReleaseDC(pDC);
}
void SubSense::DrawPicToHDC(cv::Mat& img, UINT ID)
{
	//清空
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	//pStatic->SetBitmap(NULL);
	CRect rect;
	pStatic->GetClientRect(&rect);
	//pStatic->GetDC()->FillSolidRect(rect.left ,rect.top ,rect.Width(),rect.Height(), RGB(240, 240, 240));
	////

	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	//调整大小
	////////
	float widthRatio = (float)rect.Width() / img.cols;
	float heightRatio = (float)rect.Height() / img.rows;
	float resRatio = widthRatio < heightRatio ? widthRatio: heightRatio;
	int resWidth = img.cols * resRatio;
	int resHeight = img.rows * resRatio;
	cv::resize(img, img, cv::Size(resWidth, resHeight));
	CRect drawRect;
	drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + img.cols - 1, rect.TopLeft().y + img.rows - 1);
	///////
	IplImage* tmpimg = &img.operator IplImage();
	CvvImage iimg;
	iimg.CopyOf(tmpimg);
	iimg.DrawToHDC(hDC, &drawRect);
	ReleaseDC(pDC);
	iimg.Destroy();
}


void SubSense::DoSubsense(CString pathName, bool openCameral)
{
	isStop = false;
	const bool bUseDefaultCamera = openCameral;
	const string sVideoFilePath = pathName;
	//const string sOutPath = argv[2];
	//cout<<"sVideoFilePath = "<<sVideoFilePath<<endl;
	//cout<<"sOutPath = "<<sOutPath<<endl;
	//cout<<sOutPath<<endl;
    cv::VideoCapture oVideoInput;
    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;
    if(bUseDefaultCamera) {
        oVideoInput.open(0);
        oVideoInput >> oCurrInputFrame;
    }
    else {
        oVideoInput.open(sVideoFilePath);
        oVideoInput >> oCurrInputFrame;
        oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
    }
    //parser.printParams();
    if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
        if(bUseDefaultCamera)
            MessageBox("Could not open default camera.\n");
        else
		{
			CString tmp;
			tmp.Format("Could not open video file at '%s'.\n",sVideoFilePath.c_str());
			MessageBox(tmp);
			return;
		}
            
        
    }
    oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
    oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());
    cv::Mat oSequenceROI(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI to the algorithm (ex: for CDnet, use ROI.bmp)
    /*cv::namedWindow("input",cv::WINDOW_NORMAL);
    cv::namedWindow("segmentation mask",cv::WINDOW_NORMAL);
    cv::namedWindow("reconstructed background",cv::WINDOW_NORMAL);*/
	//cv::namedWindow("test",cv::WINDOW_NORMAL);
    BackgroundSubtractorSuBSENSE oBGSAlg;
    oBGSAlg.initialize(oCurrInputFrame,oSequenceROI);
	//calcaue time,time只是精确到秒，要毫秒用clock(), start = clock() ...程序... end = clock, 时间为double(end - start) / CLOCKS_PER_SEC;
	//time_t t_start,t_end;
	clock_t tbeg,tend;
	//t_start = time(NULL);
	char str[256];
	string mypath;
	int count = 1;
    for(;isStop != true;) {
        oVideoInput >> oCurrInputFrame;
        if(oCurrInputFrame.empty())
            break;
        //oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
		tbeg = clock();
		oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(m_k));
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
		tend = clock();

		DrawPicToHDC(oCurrInputFrame, IDC_STATIC1);
		DrawPicToHDC(oCurrSegmMask, IDC_STATIC2);
		CString tmp;
		tmp.Format("当前帧： %d, 处理时长: %d ms", count++, tend - tbeg);
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);
		if (tend - tbeg < (1000 / 35))
		{
			Sleep((1000 / 35) - (tend - tbeg));
		}
        /*imshow("input",oCurrInputFrame);
        imshow("segmentation mask",oCurrSegmMask);
        imshow("reconstructed background",oCurrReconstrBGImg);*/
		//sprintf(str,"bin%06d.pgm",count++);
		//if (sOutPath.size() == 0)
		//{
		//	mypath = str;
		//}
		//else
		//{

		//		mypath = sOutPath + str;

		//}
		//cout<<mypath<<"\t"<<(double)(tend - tbeg) / CLOCKS_PER_SEC <<"秒"<<endl;
		//imwrite(mypath,oCurrSegmMask);
		
		
		//27 == ESC
        /*if(cv::waitKey(1)==27)
            break;*/
		//cv::waitKey(0);
    }
	//t_end = time(NULL);
	//cout<<difftime(t_end, t_start)<<" s"<<endl;
}


void SubSense::OnBnClickedButtonBreak()
{
	//已废弃
	// TODO: 在此添加控件通知处理程序代码
	isStop = true;
}


void SubSense::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		CString tmp;
		GetDlgItemText(IDC_EDIT1, tmp);
		if (tmp.IsEmpty())
		{
			MessageBox("path is empty!!!");
		}
		else
		{
			SubSense* pDlg = (SubSense*)this;
			CString tmp;
			pDlg->GetDlgItemText(IDC_EDIT1, tmp);
			if (tmp.IsEmpty())
			{
				pDlg->MessageBox("path is empty!!!");
				//		return 0;
			}
			const bool bUseDefaultCamera = false;
			const string sVideoFilePath = tmp;
			cv::VideoCapture oVideoInput;
			cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;
			if(bUseDefaultCamera) {
				oVideoInput.open(0);
				oVideoInput >> oCurrInputFrame;
			}
			else {
				oVideoInput.open(sVideoFilePath);
				oVideoInput >> oCurrInputFrame;
				oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
			}
			//parser.printParams();
			if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
				if(bUseDefaultCamera)
					pDlg->MessageBox("Could not open default camera.\n");
				else
				{
					CString tmp;
					tmp.Format("Could not open video file at '%s'.\n",sVideoFilePath.c_str());
					pDlg->MessageBox(tmp);
					//		return 0;
				}


			}
			oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
			oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());
			cv::Mat oSequenceROI(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI 
			BackgroundSubtractorSuBSENSE oBGSAlg;
			oBGSAlg.initialize(oCurrInputFrame,oSequenceROI);
			//calcaue time,time只是精确到秒，要毫秒用clock(), start = clock() ...程序... end = clock, 时间为double(end - start) / CLOCKS_PER_SEC;
			//time_t t_start,t_end;
			clock_t tbeg,tend;
			//t_start = time(NULL);
			char str[256];
			string mypath;
			int count = 1;
			while (pDlg->isStop == false)
			{
				oVideoInput >> oCurrInputFrame;
				if(oCurrInputFrame.empty())
					break;
				//oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
				tbeg = clock();
				oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(m_k));
				oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
				tend = clock();

				pDlg->DrawPicToHDC(oCurrInputFrame, IDC_STATIC1);
				pDlg->DrawPicToHDC(oCurrSegmMask, IDC_STATIC2);
				CString tmp;
				tmp.Format("当前帧： %d, 处理时长: %d ms", count++, tend - tbeg);
				pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);

			}
		}

	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}

}


void SubSense::OnBnClickedButtonStart1()
{
	//已经不是用
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(FALSE);
	if (!m_nTimer)
	{
		m_nTimer = SetTimer(1, 1000, NULL);
	}
	GetDlgItem(IDC_BUTTON_End1)->EnableWindow(TRUE);
	UpdateData(FALSE);
}


void SubSense::OnBnClickedButtonEnd1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End1)->EnableWindow(FALSE);
	if (m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
	GetDlgItem(IDC_BUTTON_Start1)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(TRUE);

}


void SubSense::OnBnClickedButtonStart2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(TRUE);
	isStop = false;	
	mythread = AfxBeginThread(SubSense::MyThreadFun, this);
	

}


void SubSense::OnBnClickedButtonend2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(FALSE);
	isStop = true;
}


UINT SubSense::MyThreadFun(LPVOID pParam)
{
	SubSense* pDlg = (SubSense*)pParam;
	CString tmp;
	pDlg->GetDlgItemText(IDC_EDIT1, tmp);
	if (tmp.IsEmpty())
	{
		pDlg->MessageBox("path is empty!!!");
		return 0;
	}
	const bool bUseDefaultCamera = false;
	const string sVideoFilePath = tmp;
    cv::VideoCapture oVideoInput;
    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;
    if(bUseDefaultCamera) {
        oVideoInput.open(0);
        oVideoInput >> oCurrInputFrame;
    }
    else {
        oVideoInput.open(sVideoFilePath);
        oVideoInput >> oCurrInputFrame;
        oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
    }
    //parser.printParams();
    if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
        if(bUseDefaultCamera)
            pDlg->MessageBox("Could not open default camera.\n");
        else
		{
			CString tmp;
			tmp.Format("Could not open video file at '%s'.\n",sVideoFilePath.c_str());
			pDlg->MessageBox(tmp);
			return 0;
		}
            
        
    }
    oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
    oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());
    cv::Mat oSequenceROI(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI to the algorithm (ex: for CDnet, use ROI.bmp)
    /*cv::namedWindow("input",cv::WINDOW_NORMAL);
    cv::namedWindow("segmentation mask",cv::WINDOW_NORMAL);
    cv::namedWindow("reconstructed background",cv::WINDOW_NORMAL);*/
	//cv::namedWindow("test",cv::WINDOW_NORMAL);
    BackgroundSubtractorSuBSENSE oBGSAlg;
    oBGSAlg.initialize(oCurrInputFrame,oSequenceROI);
	//calcaue time,time只是精确到秒，要毫秒用clock(), start = clock() ...程序... end = clock, 时间为double(end - start) / CLOCKS_PER_SEC;
	//time_t t_start,t_end;
	clock_t tbeg,tend;
	//t_start = time(NULL);
	char str[256];
	string mypath;
	int count = 1;
	while (pDlg->isStop == false)
	{
        oVideoInput >> oCurrInputFrame;
        if(oCurrInputFrame.empty())
            break;
        //oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
		tbeg = clock();
		oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(100));
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
		tend = clock();

		pDlg->DrawPicToHDC(oCurrInputFrame, IDC_STATIC1);
		pDlg->DrawPicToHDC(oCurrSegmMask, IDC_STATIC2);
		CString tmp;
		tmp.Format("当前帧： %d, 处理时长: %d ms", count++, tend - tbeg);
		pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);
		if (tend - tbeg < (1000 / 35))
		{
			Sleep((1000 / 35) - (tend - tbeg));
		}
    }
	pDlg->GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_end2)->EnableWindow(FALSE);
	TerminateThread(pDlg->mythread, 0);
	return 0;
}


void SubSense::OnBnClickedButtonSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(FALSE);
	mythread->SuspendThread();
}


void SubSense::OnBnClickedButtonResume()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_end2)->EnableWindow(TRUE);
	mythread->ResumeThread();
}
