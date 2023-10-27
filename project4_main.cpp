/*

Project made by Atharva Pandkar
This file contains the task for prcv assingment 4
This file has task 1 to task 3

*/




#include <cstdio>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/saturate.hpp>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace cv;
using namespace std;
// change the file name to main fo using it
int mainasd(int argc, char* argv[]) {


	Mat Image;
	Mat grayImg;

	VideoCapture* capdev;
	char fil[256];
	int stop = 0;
	int Imageid = 0;
	char buffer[256];
	std::vector<int> pars;

	pars.push_back(10);
	// The identifier added to the file names for later

	if (argc < 2) {
		printf("Usage: %s need more parameters\n", argv[0]);
		exit(-1);
	}

	// open the video device
	capdev = new VideoCapture(0);
	// capdev = new VideoCapture(1);//uncoment to use mobile camera

	if (!capdev->isOpened()) {
		printf("Unable to open video device\n");
		return(-1);
	}
	// store user given file identifier
	strcpy(fil, argv[1]);

	Size refS((int)capdev->get(CAP_PROP_FRAME_WIDTH),
		(int)capdev->get(CAP_PROP_FRAME_HEIGHT));

	printf("Expected size: %d %d\n", refS.width, refS.height);

	namedWindow("Video", 1);

	// initialize point set and corner set for camera calibration
	std::vector<Point2f> corner_set;
	std::vector<std::vector<Point2f>> corner_list;
	std::vector<Point3f> point_set;
	std::vector<std::vector<Point3f>> point_list;
	Size patternSize = Size(9, 6);

	for (int x = 0; x > -6; x--) {
		for (int y = 0; y < 9; y++) {
			point_set.push_back(Point3f(y, x, 0));

		}
	}

	for (; !stop;) {
		// storing the Images of the video capture device into a mat object and loop over to create a video
		*capdev >> Image;

		if (Image.empty()) {
			printf("Image is empty\n");
			break;
		}
		cvtColor(Image, grayImg, COLOR_BGR2GRAY);//converting Img to Gray scale img.
		//Task1
		//Detect and Extract Chessboard Corners

		//The function checks if the given frame has a chessboard in it and locates the corners.
		bool patternFound = findChessboardCorners(grayImg, patternSize, corner_set);

		if (patternFound) {
			//The function iterates to find the sub-pixel accurate location of corners or radial saddle points
			cornerSubPix(grayImg, corner_set, Size(11, 11), Size(-1, -1),
				TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		}
		Mat ImageCopy = Image.clone();//clone the image

		/*Draw corners : The function draws individual chessboard corners detected either as red circles
		 if the board was not found, or as colored corners connected with lines if the board was found.*/ 
		drawChessboardCorners(Image, patternSize, Mat(corner_set), patternFound);
		imshow("Video", Image);
		int key = waitKey(10);

		switch (key) {
		case 'q':
			stop = 1;
			break;

			//Capturing the frame: if user presses 1
		case '1':
			sprintf(buffer, "..\\data\\%s.%03d.png", fil, Imageid++);
			if (imwrite(buffer, ImageCopy, pars)) {
				printf("frame_capture: %s\n", buffer);
			}
			break;


			//Task 2
			//Select Calibration Images

		// if the user types '2', then store the vector of corners (camera matrix and distortion coeff) 
		case '2':
		{
			if (corner_set.size() != 54)
				break;
			// Pushing all the corners and point set into a vector
			corner_list.push_back(corner_set);
			point_list.push_back(point_set);
			sprintf(buffer, "images\\Calibrate.%s.%03d.png", fil, Imageid++);
			imwrite(buffer, Image, pars);//Writing the calbiration image
			printf("Cal_frame_capture: %s\n", buffer);
			std::vector<double> distortion_coeff;
			std::vector<Mat> rvecs; // Rotation Vector
			std::vector<Mat> tvecs; //Translation Vector

			//Input 5 corners to create a matrix
			if (corner_list.size() >= 5) {
				float data[3][3] = { {1,0,float(Image.cols) / 2},{0,1,float(Image.rows) / 2},{0,0,1} };
				Mat camera_matrix = Mat(3, 3, CV_32FC1, data);
				Size imageSize = Size(Image.cols, Image.rows);

				//Task3
				//Calibrate the Camera

				cout << "camera_matrix  before : " << camera_matrix << "\n" << "distortion_coeff  before : ";//getting camera matrix and distrortin coeff
				for (int i = 0; i < distortion_coeff.size(); i++) {
					cout << distortion_coeff.at(i) << ", ";
				}
				/*calibrateCamera function :Finds the camera intrinsic and extrinsic parameters from several views of a calibration pattern.
				 returns:The overall RMS re-projection error.*/
				double error = calibrateCamera(point_list, corner_list, imageSize, camera_matrix, distortion_coeff, rvecs, tvecs, CV_CALIB_FIX_ASPECT_RATIO);
				cout << "camera_matrix: " << camera_matrix << "\n" << "distortion_coeff: ";//getting camera matrix and distrortin coeff
				for (int i = 0; i < distortion_coeff.size(); i++) {
					cout << distortion_coeff.at(i) << ", ";
				}
				cout << "\nerror: " << error << "\n";
			}
			break;

		}
		case '3':
		{
			vector<double> distortion_coeff;
			vector<Mat> rvecs;// Rotation Vector
			vector<Mat> tvecs;//Translation Vector				
			if (corner_list.size() >= 5) { //at least 5 corners should be availabel
				float data[3][3] = { {1,0,float(Image.cols) / 2},{0,1,float(Image.rows) / 2},{0,0,1} };
				Mat camera_matrix = Mat(3, 3, CV_32FC1, data);
				Size imageSize = Size(Image.cols, Image.rows);
				double error = calibrateCamera(point_list, corner_list, imageSize, camera_matrix, distortion_coeff, rvecs, tvecs, CV_CALIB_FIX_ASPECT_RATIO);
				cout << "camera_matrix: " << camera_matrix << "\n" << "distortion_coeff: ";

				for (int i = 0; i < distortion_coeff.size(); i++) {
					cout << distortion_coeff.at(i) << ", ";
				}

				cout << "\nerror: " << error << "\n";
				cv::FileStorage fs("intrinsics.xml", cv::FileStorage::WRITE); // writing the camera matrix and distortion_coefficients in xml format.
				fs << "camera_matrix" << camera_matrix;
				fs << "distortion_coefficients" << distortion_coeff;
				fs.release();
				
			}
			break;
		}
		}
	}
		printf("Terminating\n");// teminating the Video Capture.
		delete capdev;
		return(0);
	}
	
