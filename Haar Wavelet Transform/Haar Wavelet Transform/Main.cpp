#include <iostream>
#include <string>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace std;
using namespace cv;

void Decomposition(Mat source, int &stage);
void composite(Mat LL, Mat HL, Mat LH, Mat HH);

int main() {
	int stage = 0;
	Mat source = imread("Images/Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	source.convertTo(source, CV_32F, 1 / 255.0);
	imshow("Source", source);
	waitKey(1);
	Decomposition(source, stage);
}

void Decomposition(Mat source,int &stage) {
	Mat L = Mat::zeros(source.rows, source.cols / 2, CV_32F);
	Mat H = Mat::zeros(source.rows, source.cols / 2, CV_32F);

	for (int y = 0; y < source.rows; y++) {
		for (int x = 0; x <= source.cols - 2; x+=2) {
			float a = source.at<float>(y, x);
			float b = source.at<float>(y, x + 1);
			L.at<float>(y, x / 2) = (a + b) / 2.0f;
			H.at<float>(y, x / 2) = (a - b);
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
	string windowName = "stage " + to_string(stage);
	namedWindow(windowName, WINDOW_NORMAL);
	imshow(windowName, result);
	waitKey(1);

	int choice;
	cout << "Choose to decomposite or composite(1: Decomposite / 2: Composite): ";
	cin >> choice;
	cout << endl;
	if (choice == 1)
		Decomposition(LL, stage);
}

void composite(Mat LL, Mat HL, Mat LH, Mat HH) {

}