// MyTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "MyTestDialog.h"
#include "afxdialogex.h"
#include "atlimage.h"

#include "CvvImage.h"

#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>

// MyTestDialog 对话框
#include <stdio.h>
#include <cxcore.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include "YzxCodeBook.h"
//subsense
#include "BackgroundSubtractorSuBSENSE.h"
using namespace cv;
using namespace std;
//For connected components:
const int CVCONTOUR_APPROX_LEVEL = 2;   // Approx.threshold - the bigger it is, the simpler is the boundary
const int CVCLOSE_ITR = 1;				// How many iterations of erosion and/or dilation there should be
//#define CVPERIMSCALE 4			// image (width+height)/PERIMSCALE.  If contour length < this, delete that contour

//For learning background
const double COLORDIST_THREAD = 0.0;
#define  cvConvert(src, dst) cvConvertScale((src), (dst), 1.0)
using namespace std;
using namespace cv;
int random(int a, int b)
{
	//get num [a, a+b)
	return rand() % (b - a) + a;
}

IMPLEMENT_DYNAMIC(MyTestDialog, CDialog)

MyTestDialog::MyTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MyTestDialog::IDD, pParent)
	, m_radio(0)
	, m_subsense_k(100)
	, m_isSave(FALSE)
	, m_pImg(NULL)
	, m_cal_speed(30)
{

	mySize = 3;
	isContinue = true;
}

MyTestDialog::~MyTestDialog()
{
}

void MyTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_vibe_size, mySize);
	DDV_MinMaxInt(pDX, mySize, 1, 20);
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Text(pDX, IDC_EDIT_subsense_k, m_subsense_k);
	DDV_MinMaxInt(pDX, m_subsense_k, 0, 100);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_isSave);
	DDX_Control(pDX, IDC_EDIT1, m_ctlEdit1);
	DDX_Text(pDX, IDC_EDIT_cal_speed, m_cal_speed);
	DDV_MinMaxInt(pDX, m_cal_speed, 1, 1000);
}


BEGIN_MESSAGE_MAP(MyTestDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Start, &MyTestDialog::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_Suspend, &MyTestDialog::OnBnClickedButtonSuspend)
	ON_BN_CLICKED(IDC_BUTTON_Resume, &MyTestDialog::OnBnClickedButtonResume)
	ON_BN_CLICKED(IDC_BUTTON_End, &MyTestDialog::OnBnClickedButtonEnd)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_EDIT_vibe_size, &MyTestDialog::OnEnChangeEditvibesize)
	ON_EN_CHANGE(IDC_EDIT_subsense_k, &MyTestDialog::OnEnChangeEditsubsensek)
END_MESSAGE_MAP()


// MyTestDialog 消息处理程序


void MyTestDialog::DrawPicToHDC(IplImage* img, UINT ID)
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
	
	CRect drawRect;
	drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + img->width - 1, rect.TopLeft().y + img->height - 1);
	cimg.DrawToHDC(hDc, &drawRect);
	ReleaseDC(pDC);
	cvReleaseImage(&img2);
	//cv::imshow("view", img);
	//path1
	//cvShowImage("view",img);

	//path2
//	著作权归作者所有。
//		商业转载请联系作者获得授权，非商业转载请注明出处。
//		作者：匿名用户
//链接：http://www.zhihu.com/question/29611790/answer/45053299
//	来源：知乎

	//cv::Mat img2;
	//cv::Mat img1(img);
	//
	//CRect rect;
	//GetDlgItem(ID)->GetClientRect(&rect);
	//cv::Rect dst(rect.left,rect.top,rect.right,rect.bottom);
	//cv::resize(img1,img2,cv::Size(rect.Width(),rect.Height()));
	//imshow("view",img2);
	//unsigned int m_buffer[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256];	
	//BITMAPINFO* m_bmi = (BITMAPINFO*) m_buffer;
	//BITMAPINFOHEADER* m_bmih = &(m_bmi->bmiHeader);
	//memset(m_bmih, 0, sizeof(*m_bmih));
	//m_bmih->biSize = sizeof(BITMAPINFOHEADER);
	//m_bmih->biWidth = img2.cols;
	//m_bmih->biHeight = -img2.rows;           // 在自下而上的位图中 高度为负
	//m_bmih->biPlanes = 1;
	//m_bmih->biCompression = BI_RGB;
	//m_bmih->biBitCount = 8 * img2.channels();

	//CDC *pDC = GetDlgItem(ID)->GetDC();	
	//::StretchDIBits(
	//	pDC->GetSafeHdc(),
	//	0, 0, rect.Width(), rect.Height(),
	//	0, 0, rect.Width(), rect.Height(),
	//	img2.data,
	//	(BITMAPINFO*) m_bmi,
	//	DIB_RGB_COLORS,
	//	SRCCOPY
	//	);
	//ReleaseDC(pDC);

}


//void MyTestDialog::OnBnClickedOpenimage()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	IplImage* image = NULL;
//	if (image)
//	{
//		cvReleaseImage(&image);
//	}
//
//	char szFilter[] = "BMP files(*.bmp)JPG files(*.jpg)|*.bmp;*.jpg|All files(*.*)|*.*||";
//	CFileDialog ofd(TRUE, NULL, NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, szFilter);
//	if (ofd.DoModal() == IDOK)
//	{
//		image = cvLoadImage(ofd.GetPathName(), CV_LOAD_IMAGE_ANYCOLOR);
//		DrawPicToHDC(image, IDC_STATIC1);	
//	}
//	cvReleaseImage(&image);
//	
//}


BOOL MyTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	//path1
	//cv::namedWindow("view", cv::WINDOW_AUTOSIZE);
	//HWND hWnd = (HWND)cvGetWindowHandle("view");
	//HWND hParent = ::GetParent(hWnd);
	//::SetParent(hWnd, GetDlgItem(IDC_STATIC)->m_hWnd);
	//::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("D://dataset//dataset//cameraJitter//traffic//input//in%06d.jpg");
	GetDlgItem(IDC_EDIT_output_path)->SetWindowTextA("D://dataset//results//cameraJitter//traffic//");
	return TRUE;
}


void MyTestDialog::DrawPicToHDC(cv::Mat& img, UINT ID)
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
	Mat showImage;
	float widthRatio = (float)rect.Width() / img.cols;
	float heightRatio = (float)rect.Height() / img.rows;
	float resRatio = widthRatio < heightRatio ? widthRatio: heightRatio;
	int resWidth = img.cols * resRatio;
	int resHeight = img.rows * resRatio;
	cv::resize(img, showImage, cv::Size(resWidth, resHeight));
	CRect drawRect;
	drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + showImage.cols - 1, rect.TopLeft().y + showImage.rows - 1);
	///////
	IplImage* tmpimg = &showImage.operator IplImage();
	CvvImage iimg;
	iimg.CopyOf(tmpimg);
	iimg.DrawToHDC(hDC, &drawRect);
	ReleaseDC(pDC);
	iimg.Destroy();
}


void MyTestDialog::drawpic(IplImage* img, unsigned int id)
{

		BYTE *g_pBits;
		HDC g_hMemDC;
		HBITMAP g_hBmp, g_hOldBmp;
		CDC *pDC;
		CStatic *pic;
		int width, height;
		CRect rect;

		pDC = GetDlgItem(id)->GetDC();
		pic = (CStatic*)GetDlgItem(id);
		pic->GetClientRect(&rect);
		width = rect.Width();
		height = rect.Height();

		g_hMemDC = ::CreateCompatibleDC(pDC->m_hDC);//创建兼容DC

		BYTE bmibuf[sizeof(BITMAPINFO)+256 * sizeof(RGBQUAD)];
		memset(bmibuf, 0, sizeof(bmibuf));
		BITMAPINFO *pbmi = (BITMAPINFO*)bmibuf;

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biWidth = img->width;
		pbmi->bmiHeader.biHeight = img->height;
		pbmi->bmiHeader.biPlanes = 1;
		pbmi->bmiHeader.biBitCount = 8 * img->nChannels;
		pbmi->bmiHeader.biCompression = BI_RGB;

		g_hBmp = ::CreateDIBSection(g_hMemDC, pbmi, DIB_RGB_COLORS, (void**)&g_pBits, 0, 0);//创建应用程序可以直接写入的、与设备无关的位图（DIB）
		g_hOldBmp = (HBITMAP)::SelectObject(g_hMemDC, g_hBmp);//复原兼容DC数据
		BitBlt(g_hMemDC, 0, 0, width, height, pDC->m_hDC, 0, 0, SRCCOPY);

		//修改图像内容:g_pBits
		int l_width = WIDTHBYTES(img->width* pbmi->bmiHeader.biBitCount);
		for (int row = 0; row < img->height; row++)
			memcpy(&g_pBits[row*l_width], &img->imageData[ (img->height - row - 1)*l_width], l_width);

		TransparentBlt(pDC->m_hDC, 0, 0, width, height, g_hMemDC, 0, 0, img->width, img->height, RGB(0, 0, 0));
		SelectObject(g_hMemDC, g_hOldBmp);

		//释放内存资源
		ReleaseDC(pDC);
		DeleteDC(g_hMemDC);
		DeleteObject(pic);
		DeleteObject(g_hBmp);
		DeleteObject(g_hOldBmp);

}





void MyTestDialog::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End)->EnableWindow(TRUE);
	UpdateData(TRUE);
	isContinue = true;
	switch (m_radio)
	{
	case 0:
		mythread = AfxBeginThread(MyTestDialog::DoVibe, this);
		break;
	case 1:
		mythread = AfxBeginThread(MyTestDialog::MyThreadSubsense, this);
		break;
	case 2:
		mythread = AfxBeginThread(MyTestDialog::DoCodeBook, this);
		break;
	default:
		break;
	}
}


void MyTestDialog::OnBnClickedButtonSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_End)->EnableWindow(FALSE);
	mythread->SuspendThread();
}


void MyTestDialog::OnBnClickedButtonResume()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End)->EnableWindow(TRUE);
	mythread->ResumeThread();
}


UINT MyTestDialog::DoVibe(LPVOID pParam)
{

	MyTestDialog* pDlg = (MyTestDialog*)pParam;
	CString filePath;
	pDlg->GetDlgItemText(IDC_EDIT1, filePath);
	if (filePath.IsEmpty())
	{
		pDlg->MessageBox("empty!!!");
		return -1;
	}
	//lpll
	CString tmp;
	pDlg->GetDlgItemText(IDC_EDIT_output_path, tmp);
	const string sOutPath = tmp.GetBuffer(0);
	Mat pFrame;
	Mat segmap;
	Mat simple[20];
	VideoCapture oVideoInput;
	int nFrmNum = 0;
	int T = 0;
	//para
	//number of samples per pixel
	const int N = 20;
	//radius of the sphere 
	const double R = 20;
	// number of close samples for being part of the background(bg)
	const int min = 2;
	//amount of random subsampling
	const int ts = 16;

	//patchsize
	const int patchsize = pDlg->mySize;
	vector<int> c_xoff(patchsize);
	vector<int> c_yoff(patchsize);
	for (int tmp = -patchsize / 2, i = 0; i < patchsize; ++i, ++tmp)
	{
		c_xoff[i] = tmp;
		c_yoff[i] = tmp;
	}

	int height, width, step, chanels;
	uchar* data;
	uchar* simdata[N];
	uchar* sg;
	int change = 0;
	//index
	int index;
	//light threshold
	double beta;
	//foreground count , judge light change
	int fgcount = 0;
	int i, j , x, y;
	char str[256];
	int count = 1;
	//cvNamedWindow("video", 1);
	//cvNamedWindow("segment map", 1);
	//cvMoveWindow("video", 30, 0);
	//cvMoveWindow("segment map", 690, 0);
	//if (argc > 3)
	//{
	//	fprintf(stderr,"[video_file_name]\n");
	//	return -1;
	//}

	//if (argc == 3)
	//{
	//	//open video
	//	if (!(pCapture = cvCaptureFromFile(argv[1])))
	//	{
	//		fprintf(stderr, "can not open video file %s\n", argv[1]);
	//		return -2;
	//	}
	//	sOutPath = argv[2];
	//	cout<<sOutPath<<endl;
	//	_mkdir(sOutPath.c_str());
	//}
	////open camera
	//if (argc == 1)
	//{
	//	if (!(pCapture = cvCaptureFromCAM(-1)))
	//	{
	//		fprintf(stderr, "can not open camera.\n");
	//		return -2;
	//	}
	//}

	//打开摄像头
	const bool openCamera = false;
	if (openCamera == true)
	{
		oVideoInput.open(0);
		if (!(oVideoInput.isOpened()))
		{
			pDlg->MessageBox("can not open camera.\n");
			return 0;
		}
	}
	else
	{
		//open video
		oVideoInput.open(filePath.GetBuffer(0));
		if (!oVideoInput.isOpened())
		{
			CString tmp;
			tmp.Format("can not open video file %s\n", filePath);
			pDlg->MessageBox(tmp, "请输入正确的文件");

			return 0;
		}
		//sOutPath = filePath.GetBuffer(0);
		//cout<<sOutPath<<endl;
		//_mkdir(sOutPath.c_str());
	}

	srand((int)time(NULL));
	//get every image of the video
	clock_t start, finish;
	while (TRUE)
	{
		oVideoInput>>pFrame;
		if (pFrame.empty())
		{
			break;
		}
		start = clock();
		++nFrmNum;
		++T;
		//gaussian filter to smooth
		GaussianBlur(pFrame, pFrame, Size(3,3), 0);
		//cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 3);
		//get image data
		data = (uchar*)pFrame.data;
		//the first image
		if (nFrmNum == 1)
		{
			//N = 20
			for (index = 0; index < N; ++index)
			{
				simple[index].create(pFrame.size(), CV_8UC3);
				//simple[index] = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
				simdata[index] = (uchar*)simple[index].data;

			}
			segmap.create(pFrame.size(), CV_8UC1);
			//segmap = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			sg = (uchar*) segmap.data;
			height = pFrame.rows;
			width = pFrame.cols;
			//step is the bytes of each line
			step = pFrame.step[0];
			//chanels ,for rgb is 3
			chanels = pFrame.channels();
			//light threshold, 55% of the image change to foregrond, means have a light change
			beta = 0.55 * height * width;
		}
		//init
		if (T == 1)
		{
			//every pixel have 20 samples,not random
			for (index = 0; index < N; ++index)
			{
				for (i = 0; i < height; ++i)
				{
					for (j = 0; j < width; ++j)
					{
						int tmp = random(0,patchsize);
						x = i + c_yoff[tmp];
						//cout<<"tmp = "<<tmp <<"\t"<<"c_yoff[tmp] = "<<c_yoff[tmp]<<endl;
						if (x < 0)
						{
							x = -x;
						}
						if (x >= height)
						{
							x = 2 * (height - 1) - height;
						}
						tmp = random(0,patchsize);
						y = j + c_xoff[tmp];
						//cout<<"tmp = "<<tmp <<"\t"<<"c_xoff[tmp] = "<<c_xoff[tmp]<<endl;
						if (y < 0)
						{
							y = -y;
						}
						if (y >= width)
						{
							y = 2 * (width - 1) - width;
						}
						for (int k = 0; k < 3; ++k)
						{
							//data = image data;
							simdata[index][i * step + j * chanels + k] = data[x * step + y * chanels + k];
						}

					}
				}
			}
		}
		if (T != 1 /*&& nFrmNum%3 == 2*/)
		{
			double a[3] = {0, 0, 0};
			for (x = 1; x < width - 1; ++x)
			{
				for (y = 1; y < height - 1; ++y)
				{
					//y height, x width 
					//compare pixel to background model
					int count = 0;
					double dist = 0;
					index = 0;
					while ( count < min && index < N)
					{
						//euclidean distance computation
						a[0] = data[y * step + x * chanels + 0] - simdata[index][y * step + x * chanels + 0];
						a[1] = data[y * step + x * chanels + 1] - simdata[index][y * step + x * chanels + 1];
						a[2] = data[y * step + x * chanels + 2] - simdata[index][y * step + x * chanels + 2];

						dist = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
						if (dist < R)
						{
							++count;
						}
						++index;
					}
					//classify pixel and update model
					if (count >= min)
					{
						//store that image[x][y] to background
						sg[y * segmap.step[0] + x * segmap.step[1]] = 0;
						//sg[y * step + x * chanels + 1] = 0;
						//sg[y * step + x * chanels + 2] = 0;
						//1 / ts to change
						int rdm = random(0, ts); 
						if (rdm == 0)
						{
							rdm = random(0, N);
							simdata[rdm][y * step + x * chanels + 0] = data[y * step + x * chanels + 0];
							simdata[rdm][y * step + x * chanels + 1] = data[y * step + x * chanels + 1];
							simdata[rdm][y * step + x * chanels + 2] = data[y * step + x * chanels + 2];
						}
						//update neighboring pixel model
						rdm = random(0, ts);
						if (rdm == 0)
						{
							int xng = 0;
							int yng = 0;
							while (xng == 0 && yng == 0)
							{
								xng = random(-1, 2); //[-1,1]
								yng = random(-1, 2); //[-1,1]
							}
							rdm = random(0, ts);
							//x∈[1,width -2],y∈[1,height -2],so will not out of range
							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 0] = data[y * step + x * chanels + 0];
							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 1] = data[y * step + x * chanels + 1];
							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 2] = data[y * step + x * chanels + 2];
						}

					}
					else
					{
						// count < min
						//store that image[x][y] = foreground;
						sg[y * segmap.step[0] + x * segmap.step[1]] = 255;
						//sg[y * step + x * chanels + 1] = 255;
						//sg[y * step + x * chanels + 2] = 255;
						++fgcount;
					}
				}
			}
			if (fgcount > beta)
			{
				T = 0;
			}
			//next frame
			fgcount = 0;
		}
		pDlg->modifyMask(segmap);
		//IplImage* tmps = &segmap.operator IplImage();
		//pDlg->cvConnectedComponents(tmps);
		pDlg->DrawPicToHDC(pFrame, IDC_STATIC1);
		pDlg->DrawPicToHDC(segmap, IDC_STATIC2);
		//pDlg->drawpic(pFrame, IDC_STATIC1);
		//pDlg->drawpic(segmap, IDC_STATIC2);
		//IplImage* tmp1 = &pFrame.operator IplImage();
		//IplImage* tmp2 = segmap.
		//Mat tmp3;
		//cvtColor(segmap, tmp3, CV_GRAY2RGB);
	//	segmap.convertTo(segmap,CV_8UC3);
	//	IplImage* tmp2 = &tmp3.operator IplImage();
	//	pDlg->drawpic(tmp1, IDC_STATIC1);
	//	pDlg->drawpic(tmp2, IDC_STATIC2);
		CString tmp;
		finish = clock();
		tmp.Format("当前帧： %d, 耗时：%d ms", nFrmNum, finish - start);
		pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(tmp);

		if (pDlg->m_isSave)
		{
			tmp.Format("bin%06d.pgm",nFrmNum);
			imwrite(sOutPath + tmp.GetBuffer(0),segmap);
		}
		//cvShowImage("video", pFrame);
		//cvShowImage("segment map", segmap);
		//char* str;
		//sprintf(str,"bin%06d.pgm",count++);
		//imwrite(sOutPath + str, segmap);
		//string mypath = sOutPath + str;
		//cvSaveImage(str, segmap);
		//sprintf()
		//string tmp = sOutPath + str;
		//cout<<tmp<<endl;
		//cvSaveImage(tmp.c_str(), segmap);
		//if (cvWaitKey(2) >= 0)
		//{
		//	break;
		//}
		//waitKey(0);
		
		if (finish - start < (1000 / pDlg->m_cal_speed))
		{
			Sleep((1000 / pDlg->m_cal_speed) - (finish - start));
		}
		if (!pDlg->isContinue)
		{
			break;
		}
	}
	pDlg->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_End)->EnableWindow(FALSE);
	pDlg->isContinue = true;
	//TerminateThread(pDlg->mythread, 0);
	return 0;
}


void MyTestDialog::OnBnClickedButtonEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End)->EnableWindow(FALSE);
	isContinue = false;
	//TerminageThread
	//mythread->ExitInstance();
	PostThreadMessage(mythread->m_nThreadID,0,0,0); 
}




void MyTestDialog::OnEnChangeEditvibesize()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

//#1015

	// TODO:  在此添加控件通知处理程序代码
	/*CString tmp;
	GetDlgItem(IDC_EDIT_vibe_size)->GetWindowTextA(tmp);
	mySize = _ttoi(tmp);*/
	//GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);
	UpdateData(true);
	CString tmp;
	tmp.Format("vibe current size = %d",mySize);
	GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);

}


void MyTestDialog::OnEnChangeEditsubsensek()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

//#1015
	UpdateData(true);
	CString tmp;
	tmp.Format("subsense current k = %d",m_subsense_k);
	GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);
	// TODO:  在此添加控件通知处理程序代码
}


UINT MyTestDialog::MyThreadSubsense(LPVOID pParam)
{
	MyTestDialog* pDlg = (MyTestDialog*)pParam;
	CString tmp;
	pDlg->GetDlgItemText(IDC_EDIT1, tmp);
	if (tmp.IsEmpty())
	{
		pDlg->MessageBox("path is empty!!!");
		return 0;
	}
	const bool bUseDefaultCamera = false;
	const string sVideoFilePath = tmp.GetBuffer(0);
	tmp.ReleaseBuffer();
	pDlg->GetDlgItemText(IDC_EDIT_output_path, tmp);
	const string sOutPath = tmp.GetBuffer(0);
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
	while (TRUE)
	{
        oVideoInput >> oCurrInputFrame;
        if(oCurrInputFrame.empty())
            break;
        //oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
		tbeg = clock();
		oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(pDlg->m_subsense_k));
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
		tend = clock();

		pDlg->DrawPicToHDC(oCurrInputFrame, IDC_STATIC1);
		pDlg->DrawPicToHDC(oCurrSegmMask, IDC_STATIC2);
		//pDlg->DrawMatUseCImage(oCurrInputFrame, IDC_STATIC1);
		//pDlg->DrawMatUseCImage(oCurrSegmMask, IDC_STATIC2);
		//pDlg->DrawMatUseBitMap(oCurrInputFrame, IDC_STATIC1);
		//pDlg->DrawMatUseBitMap(oCurrSegmMask, IDC_STATIC2);
		CString tmp;
		tmp.Format("当前帧： %d, sunsense处理时长: %d ms", count++, tend - tbeg);
		pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowTextA(tmp);
		
		if (pDlg->m_isSave)
		{
			tmp.Format("bin%06d.pgm",count - 1);
			imwrite(sOutPath + tmp.GetBuffer(0),oCurrSegmMask);
		}
		if (tend - tbeg < (1000 / pDlg->m_cal_speed))
		{
			Sleep((1000 / pDlg->m_cal_speed) - (tend - tbeg));
		}
		if (!pDlg->isContinue)
		{
			break;
		}
    }
	pDlg->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_End)->EnableWindow(FALSE);
	pDlg->isContinue = true;
	return 0;
}


void MyTestDialog::DrawMatUseCImage(cv::Mat& img, UINT ID)
{
	CImage CI;
	int w = img.cols;
	int h = img.rows;
	int channels = img.channels();
	CI.Destroy();
	CI.Create(w, h, 8 * channels);
	
	//赋值
	RGBQUAD* ColorTable;
	int MaxColors = 256;
	ColorTable = new RGBQUAD[MaxColors];
	CI.GetColorTable(0, MaxColors, ColorTable);
	for (int i = 0; i < MaxColors; ++i)
	{
		ColorTable->rgbBlue = (BYTE)i;
		ColorTable->rgbGreen = (BYTE)i;
		ColorTable->rgbRed = (BYTE)i;
		
	}
	CI.SetColorTable(0, MaxColors, ColorTable);
	delete[] ColorTable;

	if (channels == 1)
	{
		//gray image
		uchar* ps;
		uchar* pimg = (uchar*)CI.GetBits();
		int step = CI.GetPitch();
		for (int i = 0; i < h; ++i)
		{
			ps = img.ptr<uchar>(i);
			for (int j = 0; j < w; ++j)
			{
				*(pimg + i*step + j) = ps[j];
			}
		}
	}
	else if (channels == 3)
	{
		//gray image
		uchar* ps;
		uchar* pimg = (uchar*)CI.GetBits();
		int step = CI.GetPitch();
		for (int i = 0; i < h; ++i)
		{
			ps = img.ptr<uchar>(i);
			for (int j = 0; j < w; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					*(pimg + i*step + j * 3 + k) = ps[j*3 + k];
				}
				
			}
		}
	}
	//show
	//清空
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	CRect rect;
	pStatic->GetClientRect(&rect);
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	//way1
	//CI.StretchBlt(hDC, rect, SRCCOPY);
	//  [11/3/2015 yzx]
	//调整大小
	float widthRatio = (float)rect.Width() / img.cols;
	float heightRatio = (float)rect.Height() / img.rows;
	float resRatio = widthRatio < heightRatio ? widthRatio: heightRatio;
	CRect drawRect;
	if (resRatio<1)
	{
		int resWidth = img.cols * resRatio;
		int resHeight = img.rows * resRatio;
		drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + resWidth - 1, rect.TopLeft().y + resHeight - 1);
	}
	else
	{
		drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + img.cols - 1, rect.TopLeft().y + img.rows - 1);
	}
	//SetStretchBltMode(hDC, COLORONCOLOR);
	CI.Draw(hDC, drawRect);
	ReleaseDC(pDC);
	CI.Destroy();
	//way2
	//SetStretchBltMode(hDC, COLORONCOLOR);
	//CI.StretchBlt(hDC, rect, SRCCOPY);

	//way3
	//CI.Draw(hDC, 0,0);


	//iimg.DrawToHDC(hDC, &drawRect);
	//调整大小
	////////
	//Mat showImage;
	//float widthRatio = (float)rect.Width() / img.cols;
	//float heightRatio = (float)rect.Height() / img.rows;
	//float resRatio = widthRatio < heightRatio ? widthRatio: heightRatio;
	//int resWidth = img.cols * resRatio;
	//int resHeight = img.rows * resRatio;
	//cv::resize(img, showImage, cv::Size(resWidth, resHeight));
	//CRect drawRect;
	//drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + showImage.cols - 1, rect.TopLeft().y + showImage.rows - 1);
	///////
	
}


void MyTestDialog::DrawMatUseBitMap(cv::Mat& img, UINT ID)
{
	//清空
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	CRect rect;
	pStatic->GetClientRect(&rect);
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	cv::Size m_sizeShow;
	m_sizeShow = cv::Size(rect.Width(), rect.Height());
	Mat matImgTmp;
	if (img.size() != m_sizeShow)
	{
		matImgTmp = Mat(m_sizeShow, CV_8UC3);
		resize(img, matImgTmp, m_sizeShow, 0, 0, INTER_AREA);
	}
	else
	{
		matImgTmp = img.clone();
	}

	cv::flip(matImgTmp, matImgTmp, 0);

	if (m_pImg)
	{
		m_pImg->ReleaseDC();
		delete m_pImg;
		m_pImg = NULL;
	}

	m_pImg = new CImage;
	m_pImg->Create(m_sizeShow.width, m_sizeShow.height, 24);

	//header bitmapinfo
	//  [11/3/2015 yzx]
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 24;
	bitInfo.bmiHeader.biWidth = m_sizeShow.width;
	bitInfo.bmiHeader.biHeight = m_sizeShow.height;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant =
		bitInfo.bmiHeader.biClrUsed =
		bitInfo.bmiHeader.biSizeImage =
		bitInfo.bmiHeader.biXPelsPerMeter =
		bitInfo.bmiHeader.biYPelsPerMeter = 0;
	//Add header and OpenCV image’s data to m_pImg
	StretchDIBits(m_pImg->GetDC(), 0, 0,
		m_sizeShow.width, m_sizeShow.height, 0, 0,
		m_sizeShow.width, m_sizeShow.height,
		matImgTmp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);

	//display
	m_pImg->BitBlt(hDC, 0, 0);

	if (m_pImg)
	{
		m_pImg->ReleaseDC();
		delete m_pImg;
		m_pImg = NULL;
	}
	ReleaseDC(pDC);
	//cv::Size m_sizeShow = cv::Size(rect.right, rect.bottom);
}
//void MyTestDialog::OnBnClickedOpenimagemat()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	cv::Mat matImage;
//	char szFilter[] = "BMP files(*.bmp)JPG files(*.jpg)|*.bmp;*.jpg|All files(*.*)|*.*||";
//	CFileDialog ofd(TRUE, NULL, NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, szFilter);
//	if (ofd.DoModal() == IDOK)
//	{
//		matImage = cv::imread(ofd.GetPathName().GetBuffer(0), 1);
//		DrawPicToHDC(matImage, IDC_STATIC1);
//	}
//	matImage.release();
//}


//void MyTestDialog::OnBnClickedVibe()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//按钮已经删除了
//	CString strPath;
//	GetDlgItemText(IDC_EDIT1, strPath);
//	if (strPath.IsEmpty())
//	{
//		MessageBox("empty!!!");
//		return;
//	}
//	//DoMyVibe(strPath);
//
//}


//void MyTestDialog::DoMyVibe(CString& filePath, bool openCamera)
//{
//	//已经不再调用这个，可以删除了
//	//lpll
//	IplImage* pFrame = NULL;
//	IplImage* segmap = NULL;
//	IplImage* simple[20] = {NULL};
//	CvCapture* pCapture = NULL;
//	int nFrmNum = 0;
//	int T = 0;
//	//para
//	//number of samples per pixel
//	const int N = 20;
//	//radius of the sphere 
//	const double R = 20;
//	// number of close samples for being part of the background(bg)
//	const int min = 2;
//	//amount of random subsampling
//	const int ts = 16;
//
//	//patchsize
//	const int patchsize = mySize;
//	vector<int> c_xoff(patchsize);
//	vector<int> c_yoff(patchsize);
//	for (int tmp = -patchsize / 2, i = 0; i < patchsize; ++i, ++tmp)
//	{
//		c_xoff[i] = tmp;
//		c_yoff[i] = tmp;
//	}
//
//	int height, width, step, chanels;
//	uchar* data;
//	uchar* simdata[N];
//	uchar* sg;
//	int change = 0;
//	//index
//	int index;
//	//light threshold
//	double beta;
//	//foreground count , judge light change
//	int fgcount = 0;
//	int i, j , x, y;
//	string sOutPath;
//	char str[256];
//	int count = 1;
//	//cvNamedWindow("video", 1);
//	//cvNamedWindow("segment map", 1);
//	//cvMoveWindow("video", 30, 0);
//	//cvMoveWindow("segment map", 690, 0);
//	//打开摄像头
//	if (openCamera == true)
//	{
//		if (!(pCapture = cvCaptureFromCAM(-1)))
//		{
//			MessageBox("can not open camera.\n");
//			return;
//		}
//	}
//	else
//	{
//			//open video
//			if (!(pCapture = cvCaptureFromFile(filePath.GetBuffer(0))))
//			{
//				CString tmp;
//				tmp.Format("can not open video file %s\n", filePath);
//				MessageBox(tmp, "请输入正确的文件");
//
//				return;
//			}
//			sOutPath = filePath.GetBuffer(0);
//			//cout<<sOutPath<<endl;
//			//_mkdir(sOutPath.c_str());
//	}
//
//	srand((int)time(NULL));
//	//get every image of the video
//	clock_t start, finish;
//	while (pFrame = cvQueryFrame(pCapture))
//	{
//		start = clock();
//		++nFrmNum;
//		++T;
//		//gaussian filter to smooth
//		cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 3);
//		//get image data
//		data = (uchar*)pFrame->imageData;
//		//the first image
//		if (nFrmNum == 1)
//		{
//			//N = 20
//			for (index = 0; index < N; ++index)
//			{
//				simple[index] = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
//				simdata[index] = (uchar*)simple[index]->imageData;
//
//			}
//
//			segmap = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
//			sg = (uchar*) segmap->imageData;
//			height = pFrame->height;
//			width = pFrame->width;
//			//step is the bytes of each line
//			step = pFrame->widthStep;
//			//chanels ,for rgb is 3
//			chanels = pFrame->nChannels;
//			//light threshold, 55% of the image change to foregrond, means have a light change
//			beta = 0.55 * height * width;
//		}
//		//init
//		if (T == 1)
//		{
//			//every pixel have 20 samples,not random
//			for (index = 0; index < N; ++index)
//			{
//				for (i = 0; i < height; ++i)
//				{
//					for (j = 0; j < width; ++j)
//					{
//						int tmp = random(0,patchsize);
//						x = i + c_yoff[tmp];
//						//cout<<"tmp = "<<tmp <<"\t"<<"c_yoff[tmp] = "<<c_yoff[tmp]<<endl;
//						if (x < 0)
//						{
//							x = -x;
//						}
//						if (x >= height)
//						{
//							x = 2 * (height - 1) - height;
//						}
//						tmp = random(0,patchsize);
//						y = j + c_xoff[tmp];
//						//cout<<"tmp = "<<tmp <<"\t"<<"c_xoff[tmp] = "<<c_xoff[tmp]<<endl;
//						if (y < 0)
//						{
//							y = -y;
//						}
//						if (y >= width)
//						{
//							y = 2 * (width - 1) - width;
//						}
//						for (int k = 0; k < 3; ++k)
//						{
//							//data = image data;
//							simdata[index][i * step + j * chanels + k] = data[x * step + y * chanels + k];
//						}
//
//					}
//				}
//			}
//		}
//		if (T != 1 /*&& nFrmNum%3 == 2*/)
//		{
//			double a[3] = {0, 0, 0};
//			for (x = 1; x < width - 1; ++x)
//			{
//				for (y = 1; y < height - 1; ++y)
//				{
//					//y height, x width 
//					//compare pixel to background model
//					int count = 0;
//					double dist = 0;
//					index = 0;
//					while ( count < min && index < N)
//					{
//						//euclidean distance computation
//						a[0] = data[y * step + x * chanels + 0] - simdata[index][y * step + x * chanels + 0];
//						a[1] = data[y * step + x * chanels + 1] - simdata[index][y * step + x * chanels + 1];
//						a[2] = data[y * step + x * chanels + 2] - simdata[index][y * step + x * chanels + 2];
//
//						dist = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
//						if (dist < R)
//						{
//							++count;
//						}
//						++index;
//					}
//					//classify pixel and update model
//					if (count >= min)
//					{
//						//store that image[x][y] to background
//						sg[y * segmap->widthStep + x] = 0;
//						//sg[y * step + x * chanels + 1] = 0;
//						//sg[y * step + x * chanels + 2] = 0;
//						//1 / ts to change
//						int rdm = random(0, ts); 
//						if (rdm == 0)
//						{
//							rdm = random(0, N);
//							simdata[rdm][y * step + x * chanels + 0] = data[y * step + x * chanels + 0];
//							simdata[rdm][y * step + x * chanels + 1] = data[y * step + x * chanels + 1];
//							simdata[rdm][y * step + x * chanels + 2] = data[y * step + x * chanels + 2];
//						}
//						//update neighboring pixel model
//						rdm = random(0, ts);
//						if (rdm == 0)
//						{
//							int xng = 0;
//							int yng = 0;
//							while (xng == 0 && yng == 0)
//							{
//								xng = random(-1, 2); //[-1,1]
//								yng = random(-1, 2); //[-1,1]
//							}
//							rdm = random(0, ts);
//							//x∈[1,width -2],y∈[1,height -2],so will not out of range
//							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 0] = data[y * step + x * chanels + 0];
//							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 1] = data[y * step + x * chanels + 1];
//							simdata[rdm][(y + yng) * step + (x + xng) * chanels + 2] = data[y * step + x * chanels + 2];
//						}
//
//					}
//					else
//					{
//						// count < min
//						//store that image[x][y] = foreground;
//						sg[y * segmap->widthStep + x] = 255;
//						//sg[y * step + x * chanels + 1] = 255;
//						//sg[y * step + x * chanels + 2] = 255;
//						++fgcount;
//					}
//				}
//			}
//			if (fgcount > beta)
//			{
//				T = 0;
//			}
//			//next frame
//			fgcount = 0;
//		}
//		DrawPicToHDC(pFrame, IDC_STATIC);
//		DrawPicToHDC(segmap, IDC_STATIC1);
//
//		CString tmp;
//		tmp.Format("当前帧： %d", nFrmNum);
//		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(tmp);
//		//cvShowImage("video", pFrame);
//		//cvShowImage("segment map", segmap);
//		//char* str;
//		//sprintf(str,"bin%06d.pgm",count++);
//		//imwrite(sOutPath + str, segmap);
//		//string mypath = sOutPath + str;
//		//cvSaveImage(str, segmap);
//		//sprintf()
//		//string tmp = sOutPath + str;
//		//cout<<tmp<<endl;
//		//cvSaveImage(tmp.c_str(), segmap);
//		//if (cvWaitKey(2) >= 0)
//		//{
//		//	break;
//		//}
//		//waitKey(0);
//		finish = clock();
//		if (finish - start < (1000 / 35))
//		{
//			Sleep((1000 / 35) - (finish - start));
//		}
//	}
//
//	cvReleaseImage(&pFrame);
//	cvReleaseImage(&segmap);
//	cvReleaseImage(simple);
//	cvReleaseCapture(&pCapture);
//}

void MyTestDialog::modifyMask(cv::Mat& mask)
{
	morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::Mat(),cv::Point(0,0),1);
	erode(mask, mask, cv::Mat(), cv::Point(0,0), 1);
	medianBlur(mask, mask, 3);
}


void MyTestDialog::cvConnectedComponents(IplImage * mask, int poly1_hull0 , float perimScale, int* num, CvRect* bbs, CvPoint* centers)
{
	static CvMemStorage*	mem_storage	= NULL;
	static CvSeq*			contours	= NULL;
	//CLEAN UP RAW MASK
	cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_OPEN, CVCLOSE_ITR );
	cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_CLOSE, CVCLOSE_ITR );

	//FIND CONTOURS AROUND ONLY BIGGER REGIONS
	if( mem_storage==NULL ) mem_storage = cvCreateMemStorage(0);
	else cvClearMemStorage(mem_storage);

	CvContourScanner scanner = cvStartFindContours(mask,mem_storage,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	CvSeq* c;
	int numCont = 0;
	while( (c = cvFindNextContour( scanner )) != NULL )
	{
		double len = cvContourPerimeter( c );
		double q = (mask->height + mask->width) /perimScale;   //calculate perimeter len threshold
		if( len < q ) //Get rid of blob if it's perimeter is too small
		{
			cvSubstituteContour( scanner, NULL );
		}
		else //Smooth it's edges if it's large enough
		{
			CvSeq* c_new;
			if(poly1_hull0) //Polygonal approximation of the segmentation
				c_new = cvApproxPoly(c,sizeof(CvContour),mem_storage,CV_POLY_APPROX_DP, CVCONTOUR_APPROX_LEVEL,0);
			else //Convex Hull of the segmentation
				c_new = cvConvexHull2(c,mem_storage,CV_CLOCKWISE,1);
			cvSubstituteContour( scanner, c_new );
			numCont++;
		}
	}
	contours = cvEndFindContours( &scanner );

	// PAINT THE FOUND REGIONS BACK INTO THE IMAGE
	cvZero( mask );
	IplImage *maskTemp;
	//CALC CENTER OF MASS AND OR BOUNDING RECTANGLES
	if(num != NULL)
	{
		int N = *num, numFilled = 0, i=0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);
		for(i=0, c=contours; c != NULL; c = c->h_next,i++ )
		{
			if(i < N) //Only process up to *num of them
			{
				cvDrawContours(maskTemp,c,CV_CVX_WHITE, CV_CVX_WHITE,-1,CV_FILLED,8);
				//Find the center of each contour
				if(centers != NULL)
				{
					cvMoments(maskTemp,&moments,1);
					M00 = cvGetSpatialMoment(&moments,0,0);
					M10 = cvGetSpatialMoment(&moments,1,0);
					M01 = cvGetSpatialMoment(&moments,0,1);
					centers[i].x = (int)(M10/M00);
					centers[i].y = (int)(M01/M00);
				}
				//Bounding rectangles around blobs
				if(bbs != NULL)
				{
					bbs[i] = cvBoundingRect(c);
				}
				cvZero(maskTemp);
				numFilled++;
			}
			//Draw filled contours into mask
			cvDrawContours(mask,c,CV_CVX_WHITE,CV_CVX_WHITE,-1,CV_FILLED,8); //draw to central mask
		} //end looping over contours
		*num = numFilled;
		cvReleaseImage( &maskTemp);
	}
	//ELSE JUST DRAW PROCESSED CONTOURS INTO THE MASK
	else
	{
		for( c=contours; c != NULL; c = c->h_next )
		{
			cvDrawContours(mask,c,CV_CVX_WHITE, CV_CVX_BLACK,-1,CV_FILLED,8);
		}
	}
}


UINT MyTestDialog::DoCodeBook(LPVOID pParam)
{
	//  [11/5/2015 yzx]
//////////////////////////////////////////////////////////////////////////
	MyTestDialog* pDlg = (MyTestDialog*)pParam;
	CString filePath;
	pDlg->GetDlgItemText(IDC_EDIT1, filePath);
	if (filePath.IsEmpty())
	{
		pDlg->MessageBox("empty!!!");
		return -1;
	}
	//lpll
	CString tmp;
	pDlg->GetDlgItemText(IDC_EDIT_output_path, tmp);
	const string sOutPath = tmp.GetBuffer(0);
//////////////////////////////////////////////////////////////////////////
	Mat rawImage;
	Mat yuvImage;
	Mat maskCodeBook;
	codeBook* cb;
	unsigned cbBounds[CHANNELS];
	uchar* pRGB;
	uchar* pColor;
	int imageLen;
	int nChannels = CHANNELS;
	int minMod[CHANNELS];
	int maxMod[CHANNELS];
	//cv::namedWindow("Raw");
	//cv::namedWindow("CodeBook");
	VideoCapture oVideoInput;
	//"D://dataset//dataset//intermittentObjectMotion//streetLight//input//in%%06d.jpg" "D://dataset//results//intermittentObjectMotion//streetLight//"
	oVideoInput.open(filePath.GetBuffer(0));
	if (!(oVideoInput.isOpened()))
	{
		//fprintf(stderr, "can not open video file %s\n", argv[1]);
		pDlg->MessageBox("can not open camera.\n");
		return 0;
	}
	oVideoInput >> rawImage;
	oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
	yuvImage.create(rawImage.size(), CV_8UC3);
	maskCodeBook.create(rawImage.size(), CV_8UC1);
	maskCodeBook = cv::Scalar_<uchar>(255);
	imageLen = rawImage.rows * rawImage.cols;
	cb = new codeBook[imageLen];
	YzxCodeBook yzxcodebook;
	yzxcodebook.initialize(rawImage);
	for (int i = 0; i < imageLen; ++i)
	{
		cb[i].numEntries = 0;
	}
	for (int i = 0; i < nChannels; ++i)
	{
		cbBounds[i] = 10;
		minMod[i] = 20;
		maxMod[i] = 20;
	}
	int i = 1;
	clock_t start, finish;
	while (TRUE)
	{
		oVideoInput>>rawImage;
		if (rawImage.empty())
		{
			break;
		}
		GaussianBlur(rawImage, rawImage, Size(3,3), 0);
		start = clock();
		cvtColor(rawImage, yuvImage, CV_BGR2YCrCb);
		//
		//yuvImage = rawImage.clone();
		if (i <= 30)
		{
			pColor = (uchar*)yuvImage.data;
			pRGB = (uchar*)rawImage.data;
			for (int c = 0; c < imageLen; ++c)
			{
				yzxcodebook.cvupdateCodeBook(pColor,pRGB, cb[c], cbBounds, nChannels);
				pColor += 3;
				pRGB += 3;
			}
			if ( i == 30)
			{
				for (int c = 0; c < imageLen; ++c)
				{
					yzxcodebook.cvclearStaleEntries(cb[c]);
				}
			}
		}
		else
		{
			uchar maskPixelCodeBook;
			pColor = (uchar*)yuvImage.data;
			pRGB = (uchar*)rawImage.data;
			uchar* pMask = (uchar*)maskCodeBook.data;
			for (int c = 0; c < imageLen; ++c)
			{
				maskPixelCodeBook = yzxcodebook.cvbackgroundDiff(pColor, pRGB, cb[c], nChannels, minMod, maxMod);
				*pMask++ = maskPixelCodeBook;
				pColor += 3;
				pRGB += 3;
			}
			if (i % 30 == 0)
			{
				for (int c = 0; c < imageLen; ++c)
				{
					yzxcodebook.cvclearStaleEntries(cb[c]);
				}
			}
		}
		//imshow("Raw", rawImage);
		//imshow("code book pre", maskCodeBook);
		//IplImage* test = &maskCodeBook.operator IplImage();
		//cvconnectedComponents(test);
		yzxcodebook.cvconnectedComponents3(maskCodeBook);
		//yzxcodebook.modifyMask(maskCodeBook);
		pDlg->DrawPicToHDC(rawImage, IDC_STATIC1);
		pDlg->DrawPicToHDC(maskCodeBook, IDC_STATIC2);
		CString tmp;
		finish = clock();
		tmp.Format("当前帧： %d, 耗时：%d ms", i, finish - start);
		pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(tmp);

		if (pDlg->m_isSave)
		{
			tmp.Format("bin%06d.pgm",i);
			imwrite(sOutPath + tmp.GetBuffer(0),maskCodeBook);
		}
		//imshow("CodeBook", maskCodeBook);
		//if (waitKey(30) == 27)
		//{
		//	break;
		//}
		i++;
		if (finish - start < (1000 / pDlg->m_cal_speed))
		{
			Sleep((1000 / pDlg->m_cal_speed) - (finish - start));
		}
		if (!pDlg->isContinue)
		{
			break;
		}
	}
	delete []cb;
	pDlg->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BUTTON_End)->EnableWindow(FALSE);
	return 0;

}
