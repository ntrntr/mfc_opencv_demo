
// minimalistic foreground-background segmentation sample, based off OpenCV's bgfg_segm sample

#include "BackgroundSubtractorSuBSENSE.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <direct.h>

using namespace cv;
using namespace std;
static void help() {
    printf("\nMinimalistic example of foreground-background segmentation in a video sequence using\n"
            "OpenCV's BackgroundSubtractor interface; will analyze frames from the default camera\n"
            "or from a specified file.\n\n"
            "Usage: \n"
            "  ./bgfg_segm [--camera]=<use camera, true/false>, [--file]=<path to file> \n\n");
}

//const char* keys = {
//    "{c  |camera   |false     | use camera or not}"
//    "{f  |file     |canon_pic//in%06d.jpg | movie file path  }"
//};

const char* keys = {
		//"{c  |camera   |false     | use camera or not}"
		"{f  |file     |"" | movie file path  }"
		"{o  |outfile  |"" | out file path  }"
};

int main1(int argc, const char** argv) {
    help();
	/*vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PXM_BINARY);
	compression_params.push_back(1);
    cv::CommandLineParser parser(argc, argv, keys);*/
    //const bool bUseDefaultCamera = parser.get<bool>("camera");
	const bool bUseDefaultCamera = false;
	/*const std::string sVideoFilePath = parser.get<std::string>("file");

	const string sOutPath = parser.get<string>("outfile");*/
	const string sVideoFilePath = argv[1];
	const string sOutPath = argv[2];
	cout<<"sVideoFilePath = "<<sVideoFilePath<<endl;
	cout<<"sOutPath = "<<sOutPath<<endl;
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
            printf("Could not open default camera.\n");
        else
            printf("Could not open video file at '%s'.\n",sVideoFilePath.c_str());
        return -1;
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
	time_t t_start,t_end;
	clock_t tbeg,tend;
	t_start = time(NULL);
	char str[256];
	string mypath;
	int count = 1;
    for(;;) {
        oVideoInput >> oCurrInputFrame;
        if(oCurrInputFrame.empty())
            break;
        //oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(k<=100)); // lower rate in the early frames helps bootstrap the model when foreground is present
		tbeg = clock();
		oBGSAlg(oCurrInputFrame,oCurrSegmMask,double(100));
        oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
		tend = clock();
        /*imshow("input",oCurrInputFrame);
        imshow("segmentation mask",oCurrSegmMask);
        imshow("reconstructed background",oCurrReconstrBGImg);*/
		sprintf(str,"bin%06d.pgm",count++);
		if (sOutPath.size() == 0)
		{
			mypath = str;
		}
		else
		{
			/*if (sOutPath[sOutPath.size() - 1] != '//' )
			{
				mypath = sOutPath + "//" + str;
			}
			else
			{*/
				mypath = sOutPath + str;
			//}
		}
		cout<<mypath<<"\t"<<(double)(tend - tbeg) / CLOCKS_PER_SEC <<"秒"<<endl;
		imwrite(mypath,oCurrSegmMask);
		
		
		//27 == ESC
        /*if(cv::waitKey(1)==27)
            break;*/
		//cv::waitKey(0);
    }
	t_end = time(NULL);
	cout<<difftime(t_end, t_start)<<" s"<<endl;
    return 0;
}

