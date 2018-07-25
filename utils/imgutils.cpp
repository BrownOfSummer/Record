/*************************************************************************
    > File Name: imgutils.cpp
    > Author: li_pengju
    > Mail: 
    > Copyright: All Rights Reserved
    > Created Time: 2018-07-12 14:24:02
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp>
#include "imgutils/CImg.h"
#include "imgutils/mat.h"
using namespace std;
//using namespace cv;
// Mat32f is [0-1] and Mat is [0-255]
inline Mat32f Mat2Mat32f(cv::Mat& mat)
{
	cv::Mat temp;
	mat.convertTo(temp, CV_32FC3, 1.0/255);
	
	int w = mat.cols;
	int h = mat.rows;
	int c = 3;
	
	Mat32f res(h, w, 3);

	//#define MAT(ROW, COL) ((float*)(temp.ptr<float>((ROW)) + (COL)* sizeof(float) * c))
	for(int y = 0; y < h; y++)
	{
		float *row_dst = res.ptr(y);
		float *row_src = temp.ptr<float>(y);
		for(int x = 0; x < w; x++)
		{
			*(row_dst + x * 3 + 0) = *(row_src + x * 3 + 2);
			*(row_dst + x * 3 + 1) = *(row_src + x * 3 + 1);
			*(row_dst + x * 3 + 2) = *(row_src + x * 3 + 0);
		}
		//memcpy(row_dst, row_src, w * sizeof(float) * 3);
	}
	
	return res;
}
inline void Mat32f2Mat(const Mat32f& mat, cv::Mat& im)
{
	
	int w = mat.width();
	int h = mat.height();
	int c = mat.channels();
	
	im = cv::Mat::zeros(h,w,CV_32FC3);
	
	
	#define IM(ROW, COL) (float*)(im.data + im.step  * (ROW) + (COL)* sizeof(float) * 3)
	
	for(int y = 0; y < h; y++)
	{
		const float *row = mat.ptr(y);
		for(int x = 0; x < w; x++)
		{
			*(IM(y, x) + 2) = *(row + x * 3 + 0);
			*(IM(y, x) + 1) = *(row + x * 3 + 1);
			*(IM(y, x) + 0) = *(row + x * 3 + 2);
		}
		//memcpy(IM(y, 0), row, w * sizeof(float)*3);
	}
}
int main()
{
    return 0;
}
