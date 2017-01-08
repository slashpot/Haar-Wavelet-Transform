#include <iostream>
#include <string>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace std;
using namespace cv;

int stage = 0;
int decompositeTimes;

void Decomposite(Mat source);
void Recnstruct(Mat LL, Mat HL, Mat LH, Mat HH);

int main() {
	Mat source = imread("Images/NoPartyForCaoDong.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	source.convertTo(source, CV_32F, 1 / 255.0);

	cout << "How many times do you want to decomposite: ";
	cin >> decompositeTimes;
	Decomposite(source);
}

void Decomposite(Mat source) {
	Mat L = Mat::zeros(source.rows, source.cols / 2, CV_32F);
	Mat H = Mat::zeros(source.rows, source.cols / 2, CV_32F);

	for (int y = 0; y < source.rows; y++) {
		for (int x = 0; x <= source.cols - 2; x+=2) {
			float a = source.at<float>(y, x);
			float b = source.at<float>(y, x + 1);
			L.at<float>(y, x / 2) = (a + b) / 2.0f;
			H.at<float>(y, x / 2) = (a - b) / 2.0f;
		}
	}

	Mat LL = Mat::zeros(source.rows / 2, source.cols / 2, CV_32F);
	Mat HL = Mat::zeros(source.rows / 2, source.cols / 2, CV_32F);
	Mat LH = Mat::zeros(source.rows / 2, source.cols / 2, CV_32F);
	Mat HH = Mat::zeros(source.rows / 2, source.cols / 2, CV_32F);

	for (int y = 0; y <= L.rows - 2; y += 2) {
		for (int x = 0; x < L.cols; x++) {
			float a = L.at<float>(y, x);
			float b = L.at<float>(y + 1, x);
			LL.at<float>(y / 2, x) = (a + b) / 2.0f;
			LH.at<float>(y / 2, x) = (a - b) / 2.0f;
		} 
	}

	for (int y = 0; y <= H.rows - 2; y += 2) {
		for (int x = 0; x < H.cols; x++) {
			float a = H.at<float>(y, x);
			float b = H.at<float>(y + 1, x);
			HL.at<float>(y / 2, x) = (a + b) / 2.0f;
			HH.at<float>(y / 2, x) = (a - b) / 2.0f;
		}
	}

	Mat result = Mat::zeros(source.rows, source.cols, CV_32F);

	LL.copyTo(result(Rect(0, 0, LL.cols, LL.rows)));
	HL.copyTo(result(Rect(HL.cols, 0, HL.cols, HL.rows)));
	LH.copyTo(result(Rect(0, LH.rows, LH.cols, LH.rows)));
	HH.copyTo(result(Rect(HH.cols, HH.rows, HH.cols, HH.rows)));

	stage++;
	string windowName = "Decomposition_stage " + to_string(stage);
	Mat savedImg;
	result.convertTo(savedImg, CV_8U, 255);
	imwrite("Images/" + windowName + ".png", savedImg);

	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, result);
	waitKey(0);

	if (stage != decompositeTimes)
		Decomposite(LL);

	Recnstruct(LL, HL, LH, HH);
}

void Recnstruct(Mat LL, Mat HL, Mat LH, Mat HH) {
	Mat L = Mat::zeros(LL.rows * 2, LL.cols, CV_32F);
	Mat H = Mat::zeros(HH.rows * 2, HH.cols, CV_32F);

	for (int y = 0; y < LL.rows; y++) {
		for (int x = 0; x < LL.cols; x++) {
			float a = LL.at<float>(y, x);
			float b = LH.at<float>(y, x);
			L.at<float>(y * 2, x) = a + b;
			L.at<float>(y * 2 + 1, x) = a - b;

			float c = HL.at<float>(y, x);
			float d = HH.at<float>(y, x);
			H.at<float>(y * 2, x) = c + d;
			H.at<float>(y * 2 + 1, x) = c - d;
		}
	}

	Mat result = Mat::zeros(LL.rows * 2, LL.cols * 2, CV_32F);
	
	for (int y = 0; y < L.rows; y++) {
		for (int x = 0; x < L.cols; x++) {
			float a = L.at<float>(y, x);
			float b = H.at<float>(y, x);
			result.at<float>(y, x * 2) = a + b;
			result.at<float>(y, x * 2 + 1) = a - b;
		}
	}
	
	stage--;
	string windowName = "Recnstruction_stage " + to_string(stage);
	Mat savedImg;
	result.convertTo(savedImg, CV_8U, 255);
	imwrite("Images/" + windowName + ".png", savedImg);

	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, result);
	waitKey(0);
}