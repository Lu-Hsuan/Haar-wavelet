// haar_wavelet.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void wavelet_tr(Mat src, Mat &dst, int depth , int channel) {
	int i, j, k;
	int height, width;
	if (channel > 0) {
		height = src.rows;
		width = src.cols * channel;
		if(channel == 1)
			cvtColor(src, src, COLOR_BGR2GRAY);
		//src.convertTo(src, CV_32FC1);
		Mat img_temp = Mat::zeros(src.size(), src.type());
		Mat temp = Mat::zeros(src.size(), src.type());
		//col 
		int width_pos = width / 2;
		int height_pos = height / 2;
		printf("%d %d", height, width);
		//cvtColor(lena, lena,COLOR_BGR2GRAY);
		int depthe;

		for (depthe = 0; depthe < depth; depthe++) {
			for (i = 0; i < height; i++) {
				float* dst_ptr = src.ptr<float>(i);
				float* dst_ptr_t = img_temp.ptr<float>(i);
				for (j = 0; j < width / (2 * channel); j++) {
					for (k = 0; k < channel; k++) {
						//cout << dst_ptr[j] << endl;
						dst_ptr_t[channel * j + k] = (dst_ptr[channel * 2 * j + k] + dst_ptr[channel * 2 * j + channel + k]) / 2;
						dst_ptr_t[channel * j + width_pos + k] = (dst_ptr[channel * 2 * j + k] - dst_ptr[channel * 2 * j + channel + k]) / 2;
					}
					//dst_ptr_t[j + width_pos+k] = dst_ptr[j] ;
				}
			}
			//if (depthe == 0)break;
			//temp = img_temp.clone();
			for (i = 0; i < height / 2; i++) {
				float* dst_ptr_0 = img_temp.ptr<float>(2 * i);
				float* dst_ptr_1 = img_temp.ptr<float>(2 * i + 1);
				float* dst_ptr = temp.ptr<float>(i);
				float* dst_ptr_height = temp.ptr<float>(i + height_pos);
				for (j = 0; j < width; j++) {
					dst_ptr[j] = (dst_ptr_0[j] + dst_ptr_1[j]) / 2;
					dst_ptr_height[j] = (dst_ptr_0[j] - dst_ptr_1[j]) / 2;
				}
			}
			width >>= 1;
			height >>= 1;
			width_pos >>= 1;
			height_pos >>= 1;
			src = temp;
			img_temp = temp.clone();
			imshow("lena", img_temp);
			waitKey();
		}
		dst = temp.clone();
	}
}
void wavelet_inv_tr(Mat src, Mat& dst, int depth, int channel) {
	int i, j, k;
	int height, width;
	float z_l, z_h;
	height = (src.rows) >> depth;
	width = (src.cols) >> depth;
	int depthe;
	Mat img_temp = Mat::zeros(src.size(), src.type());
	Mat temp = src.clone();

	for (depthe = 0; depthe < depth; depthe++) {
		for (i = 0; i < height; i++) {
			float* dst_ptr_l = src.ptr<float>(i);
			float* dst_ptr_h = src.ptr<float>(i+height);
			float* dst_ptr_x = img_temp.ptr<float>(i*2);
			float* dst_ptr_y = img_temp.ptr<float>(i*2+1);
			for (j = 0; j < width * 2; j++) {
				for (k = 0; k < channel; k++) {
					z_l = dst_ptr_l[channel * j + k];
					z_h = dst_ptr_h[channel * j + k];
					dst_ptr_x[channel * j + k] = z_l + z_h;
					dst_ptr_y[channel * j + k] = z_l - z_h;
				}
			}
		}
		//temp = img_temp.clone();
		for (i = 0; i < height*2; i++) {
			float* dst_ptr_z = img_temp.ptr<float>(i);
			float* dst_ptr_t = temp.ptr<float>(i);
			for (j = 0; j < width; j++) {
				for (k = 0; k < channel; k++) {
					z_l = dst_ptr_z[channel * j + k];
					z_h = dst_ptr_z[channel * j + k + width * channel];
					dst_ptr_t[channel * j * 2 + k ] = z_l + z_h;
					dst_ptr_t[channel * j * 2 + k + channel] = z_l - z_h;
				}
			}
		}
		height <<= 1;
		width <<= 1;
		//convertScaleAbs(temp, temp);
		src = temp;

		imshow("lena_inv", temp);
		waitKey();
		//img_temp = temp.clone();
	}
	temp.copyTo(dst);
	//convertScaleAbs(dst, dst);
}
int main()
{
	
	Mat lena;
	Mat img_temp;
	Mat show;
	lena = imread("lena.png");
	lena.convertTo(lena, CV_32FC3,1/255.0);
	int depth = 0;
	cout << "Input Depth for wavelet: ";
	cin >> depth;
	wavelet_tr(lena, img_temp, depth, 3); // (Src , Dst , depth , channel)
	cout << img_temp.size() << img_temp.type() << endl;
	//imshow("lena",img_temp);
	//waitKey();
	wavelet_inv_tr(img_temp, show ,depth, 3);
	//namedWindow("lena_inv", WINDOW_NORMAL | WINDOW_KEEPRATIO| WINDOW_GUI_EXPANDED);
	imshow("lena_inv", show);
	waitKey();
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
/**/
