// MyTestDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "mfc_opencv_demo.h"
#include "MyTestDialog.h"
#include "afxdialogex.h"

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
	DDX_Control(pDX, IDC_EDIT2, size);
	DDX_Text(pDX, IDC_EDIT2, mySize);
	DDV_MinMaxInt(pDX, mySize, 1, 20);
}


BEGIN_MESSAGE_MAP(MyTestDialog, CDialog)
	ON_BN_CLICKED(IDC_OpenImage, &MyTestDialog::OnBnClickedOpenimage)
	ON_BN_CLICKED(IDC_OpenImageMat, &MyTestDialog::OnBnClickedOpenimagemat)
	ON_BN_CLICKED(IDC_Vibe, &MyTestDialog::OnBnClickedVibe)
	ON_BN_CLICKED(IDC_BUTTON_Start, &MyTestDialog::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_Suspend, &MyTestDialog::OnBnClickedButtonSuspend)
	ON_BN_CLICKED(IDC_BUTTON_Resume, &MyTestDialog::OnBnClickedButtonResume)
	ON_BN_CLICKED(IDC_BUTTON_End3, &MyTestDialog::OnBnClickedButtonEnd3)
	ON_WM_DROPFILES()
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
	cvReleaseImage(&img2);
	CRect drawRect;
	drawRect.SetRect(rect.TopLeft().x, rect.TopLeft().y, rect.TopLeft().x + img->width - 1, rect.TopLeft().y + img->height - 1);
	cimg.DrawToHDC(hDc, &drawRect);
	ReleaseDC(pDC);
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


void MyTestDialog::OnBnClickedOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	IplImage* image = NULL;
	if (image)
	{
		cvReleaseImage(&image);
	}

	char szFilter[] = "BMP files(*.bmp)JPG files(*.jpg)|*.bmp;*.jpg|All files(*.*)|*.*||";
	CFileDialog ofd(TRUE, NULL, NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, szFilter);
	if (ofd.DoModal() == IDOK)
	{
		image = cvLoadImage(ofd.GetPathName(), CV_LOAD_IMAGE_ANYCOLOR);
		DrawPicToHDC(image, IDC_STATIC);	
	}
	cvReleaseImage(&image);
	
}


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
	return TRUE;
}


void MyTestDialog::DrawPicToHDC(cv::Mat& img, UINT ID)
{
	//清空
	CStatic* pStatic = (CStatic*)GetDlgItem(ID);
	pStatic->SetBitmap(NULL);
	CRect rect;
	pStatic->GetClientRect(&rect);
	pStatic->GetDC()->FillSolidRect(rect.left ,rect.top ,rect.Width(),rect.Height(), RGB(240, 240, 240));
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
		pbmi->bmiHeader.biBitCount = 24;
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


void MyTestDialog::OnBnClickedOpenimagemat()
{
	// TODO: 在此添加控件通知处理程序代码

	cv::Mat matImage;
	char szFilter[] = "BMP files(*.bmp)JPG files(*.jpg)|*.bmp;*.jpg|All files(*.*)|*.*||";
	CFileDialog ofd(TRUE, NULL, NULL, OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, szFilter);
	if (ofd.DoModal() == IDOK)
	{
		matImage = cv::imread(ofd.GetPathName().GetBuffer(0), 1);
		DrawPicToHDC(matImage, IDC_STATIC);
	}
	matImage.release();
}


void MyTestDialog::OnBnClickedVibe()
{
	// TODO: 在此添加控件通知处理程序代码
	//按钮已经删除了
	CString strPath;
	GetDlgItemText(IDC_EDIT1, strPath);
	if (strPath.IsEmpty())
	{
		MessageBox("empty!!!");
		return;
	}
	DoMyVibe(strPath);

}


void MyTestDialog::DoMyVibe(CString& filePath, bool openCamera)
{
	//已经不再调用这个，可以删除了
	//lpll
	IplImage* pFrame = NULL;
	IplImage* segmap = NULL;
	IplImage* simple[20] = {NULL};
	CvCapture* pCapture = NULL;
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
	const int patchsize = mySize;
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
	string sOutPath;
	char str[256];
	int count = 1;
	//cvNamedWindow("video", 1);
	//cvNamedWindow("segment map", 1);
	//cvMoveWindow("video", 30, 0);
	//cvMoveWindow("segment map", 690, 0);
	//打开摄像头
	if (openCamera == true)
	{
		if (!(pCapture = cvCaptureFromCAM(-1)))
		{
			MessageBox("can not open camera.\n");
			return;
		}
	}
	else
	{
			//open video
			if (!(pCapture = cvCaptureFromFile(filePath.GetBuffer(0))))
			{
				CString tmp;
				tmp.Format("can not open video file %s\n", filePath);
				MessageBox(tmp, "请输入正确的文件");

				return;
			}
			sOutPath = filePath.GetBuffer(0);
			//cout<<sOutPath<<endl;
			//_mkdir(sOutPath.c_str());
	}

	srand((int)time(NULL));
	//get every image of the video
	clock_t start, finish;
	while (pFrame = cvQueryFrame(pCapture))
	{
		start = clock();
		++nFrmNum;
		++T;
		//gaussian filter to smooth
		cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 3);
		//get image data
		data = (uchar*)pFrame->imageData;
		//the first image
		if (nFrmNum == 1)
		{
			//N = 20
			for (index = 0; index < N; ++index)
			{
				simple[index] = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
				simdata[index] = (uchar*)simple[index]->imageData;

			}

			segmap = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			sg = (uchar*) segmap->imageData;
			height = pFrame->height;
			width = pFrame->width;
			//step is the bytes of each line
			step = pFrame->widthStep;
			//chanels ,for rgb is 3
			chanels = pFrame->nChannels;
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
						sg[y * segmap->widthStep + x] = 0;
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
						sg[y * segmap->widthStep + x] = 255;
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
		DrawPicToHDC(pFrame, IDC_STATIC);
		DrawPicToHDC(segmap, IDC_STATIC1);

		CString tmp;
		tmp.Format("当前帧： %d", nFrmNum);
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(tmp);
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
		finish = clock();
		if (finish - start < (1000 / 35))
		{
			Sleep((1000 / 35) - (finish - start));
		}
	}

	cvReleaseImage(&pFrame);
	cvReleaseImage(&segmap);
	cvReleaseImage(simple);
	cvReleaseCapture(&pCapture);
}


void MyTestDialog::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End3)->EnableWindow(TRUE);
	isContinue = true;
	mythread = AfxBeginThread(MyTestDialog::DoVibe, this);
	

}


void MyTestDialog::OnBnClickedButtonSuspend()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_End3)->EnableWindow(FALSE);
	mythread->SuspendThread();
}


void MyTestDialog::OnBnClickedButtonResume()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End3)->EnableWindow(TRUE);
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
	IplImage* pFrame = NULL;
	IplImage* segmap = NULL;
	IplImage* simple[20] = {NULL};
	CvCapture* pCapture = NULL;
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
	string sOutPath;
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
		if (!(pCapture = cvCaptureFromCAM(-1)))
		{
			pDlg->MessageBox("can not open camera.\n");
			return 0;
		}
	}
	else
	{
		//open video
		if (!(pCapture = cvCaptureFromFile(filePath.GetBuffer(0))))
		{
			CString tmp;
			tmp.Format("can not open video file %s\n", filePath);
			pDlg->MessageBox(tmp, "请输入正确的文件");

			return 0;
		}
		sOutPath = filePath.GetBuffer(0);
		//cout<<sOutPath<<endl;
		//_mkdir(sOutPath.c_str());
	}

	srand((int)time(NULL));
	//get every image of the video
	clock_t start, finish;
	while (pFrame = cvQueryFrame(pCapture))
	{
		
		start = clock();
		++nFrmNum;
		++T;
		//gaussian filter to smooth
		cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 3);
		//get image data
		data = (uchar*)pFrame->imageData;
		//the first image
		if (nFrmNum == 1)
		{
			//N = 20
			for (index = 0; index < N; ++index)
			{
				simple[index] = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
				simdata[index] = (uchar*)simple[index]->imageData;

			}

			segmap = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			sg = (uchar*) segmap->imageData;
			height = pFrame->height;
			width = pFrame->width;
			//step is the bytes of each line
			step = pFrame->widthStep;
			//chanels ,for rgb is 3
			chanels = pFrame->nChannels;
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
						sg[y * segmap->widthStep + x] = 0;
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
						sg[y * segmap->widthStep + x] = 255;
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
		pDlg->DrawPicToHDC(pFrame, IDC_STATIC);
		pDlg->DrawPicToHDC(segmap, IDC_STATIC1);

		CString tmp;
		tmp.Format("当前帧： %d", nFrmNum);
		pDlg->GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(tmp);
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
		finish = clock();
		if (finish - start < (1000 / 35))
		{
			Sleep((1000 / 35) - (finish - start));
		}
		if (!pDlg->isContinue)
		{
			break;
		}
	}
	if (pDlg->isContinue)
	{
		//这个内存泄露怎么解决啊
		cvReleaseImage(&pFrame);

	}
	cvReleaseImage(&segmap);
	cvReleaseImage(simple);
	cvReleaseCapture(&pCapture);
	TerminateThread(pDlg->mythread, 0);
	return 0;
}


void MyTestDialog::OnBnClickedButtonEnd3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_Start)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_Suspend)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Resume)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_End3)->EnableWindow(FALSE);
	isContinue = false;
	//TerminageThread
	//mythread->ExitInstance();
	PostThreadMessage(mythread->m_nThreadID,0,0,0); 
}


void MyTestDialog::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//要创建一个edit类型的
	//然后dropfiles这里没消息
	if (hDropInfo)
	{
		int nDrag;
		nDrag = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if (nDrag == 1)
		{
			TCHAR Path[MAX_PATH + 1] = {0};
			int len = DragQueryFile(hDropInfo, 0, Path, MAX_PATH);
			GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(Path);
		}
		else
		{
			MessageBox("只要一个文件");
		}
	}
	//CDialog::OnDropFiles(hDropInfo);
}
