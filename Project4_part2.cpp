/*

Project made by Atharva Pandkar
This file contains the task for prcv assingment 4
This file has task 4 to task 6

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


/*
Task4: Calculate Current Position of the Camera
Task5: Project Outside Corners or 3D Axes
Task6: Create a Virtual Object          */


using namespace cv;
using namespace std;

Mat cam_matrix = Mat::zeros(3, 3, CV_32FC1);
vector<float> distortLine;

int circ(Mat frame,Mat dst,Mat rvec,Mat tvec,Mat camera_matrix,Mat distortion_coeffs) {
	std::vector<cv::Point3f> object_points;
	std::vector<cv::Point2f> image_points;
	cv::Size pattern_size(9, 6);
	for (int i = 0; i < pattern_size.height; i++) {
		for (int j = 0; j < pattern_size.width; j++) {
			object_points.push_back(cv::Point3f(j, i, 0));
		}
	}
	cv::projectPoints(object_points, rvec, tvec, camera_matrix, distortion_coeffs, image_points);
	cv::circle(dst, image_points[1], 5, cv::Scalar(0, 0, 255), -1); // Red circle at top-left corner
	cv::circle(dst, image_points[2], 5, cv::Scalar(0, 255, 0), -1); // Green circle at top-right corner
	cv::circle(dst, image_points[3], 5, cv::Scalar(255, 0, 0), -1); // Blue circle at bottom-right corner
	cv::circle(dst, image_points[4], 5, cv::Scalar(255, 255, 0), -1); // Cyan circle at bottom-left corner
	return 0;
}
int cube(cv::Mat frame, cv::Mat dst, Mat rvec, Mat tvec, cv::Mat camera_matrix, cv::Mat distortion_coeffs, std::vector<cv::Point2f> image_points) {
	vector<Point3f> cubePoints;
	cubePoints.push_back(Point3f(0, 0, 0));   // bottom face
	cubePoints.push_back(Point3f(0, 1, 0));
	cubePoints.push_back(Point3f(1, 1, 0));
	cubePoints.push_back(Point3f(1, 0, 0));
	cubePoints.push_back(Point3f(0, 0, -1));  // top face
	cubePoints.push_back(Point3f(0, 1, -1));
	cubePoints.push_back(Point3f(1, 1, -1));
	cubePoints.push_back(Point3f(1, 0, -1));

	// Define the edges of the cube
	vector<Vec2i> cubeEdges;
	cubeEdges.push_back(Vec2i(0, 1));
	cubeEdges.push_back(Vec2i(1, 2));
	cubeEdges.push_back(Vec2i(2, 3));
	cubeEdges.push_back(Vec2i(3, 0));
	cubeEdges.push_back(Vec2i(4, 5));
	cubeEdges.push_back(Vec2i(5, 6));
	cubeEdges.push_back(Vec2i(6, 7));
	cubeEdges.push_back(Vec2i(7, 4));
	cubeEdges.push_back(Vec2i(0, 4));
	cubeEdges.push_back(Vec2i(1, 5));
	cubeEdges.push_back(Vec2i(2, 6));
	cubeEdges.push_back(Vec2i(3, 7));

	// Define colors for the edges
	Scalar lineColors[12] = { Scalar(255, 0, 0), Scalar(255, 0, 0), Scalar(255, 0, 0), Scalar(255, 0, 0),
							 Scalar(0, 255, 0), Scalar(0, 255, 0), Scalar(0, 255, 0), Scalar(0, 255, 0),
							 Scalar(0, 0, 255), Scalar(0, 0, 255), Scalar(0, 0, 255), Scalar(0, 0, 255) };

	// Draw the 3D cube on the image
	vector<Point2f> projectedCubeEdges;
	projectPoints(cubePoints, rvec, tvec, camera_matrix, distortion_coeffs, projectedCubeEdges);
	for (int i = 0; i < 12; i++) {
		Point2f p1 = projectedCubeEdges[cubeEdges[i][0]];
		Point2f p2 = projectedCubeEdges[cubeEdges[i][1]];
		line(dst, p1, p2, lineColors[i], 2);
	}
	//cv::imshow("Chessboard asd", frame);
	return 0;
}

int mainasda(int argc, char** argv) {

	// Read camera calibration parameters from file
	cv::FileStorage fs("intrinsics.xml", cv::FileStorage::READ);
	cv::Mat camera_matrix, distortion_coeffs;
	fs["camera_matrix"] >> camera_matrix;
	fs["distortion_coeffs"] >> distortion_coeffs;
	cout << camera_matrix;
	cout << distortion_coeffs;
	fs.release();
	int key = 1;
	// Start video loop
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Failed to open camera" << std::endl;
		return -1;
	}
	cv::Mat frame;
	cv::Mat rvec, tvec;
	while (cap.read(frame)) {
		// Detect chessboard
		cv::Size pattern_size(9, 6);
		std::vector<cv::Point2f> corners;
		bool found = cv::findChessboardCorners(frame, pattern_size, corners);
		if (found) {
			// Refine corner positions
			cv::Mat gray;
			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
			cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
				cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));

			// Get 3D object points
			std::vector<cv::Point3f> object_points;
			for (int i = 0; i < pattern_size.height; i++) {
				for (int j = 0; j < pattern_size.width; j++) {
					object_points.push_back(cv::Point3f(j, i, 0));
				}
			}
			//Task 4
			// Calculate Current Position of the Camera
			// Get rotation and translation vectors
			
			cv::solvePnP(object_points, corners, camera_matrix, distortion_coeffs, rvec, tvec);

			// Print rotation and translation data
			std::cout << "Rotation vector: " << rvec << std::endl;
			std::cout << "Translation vector: " << tvec << std::endl;

			// Draw chessboard corners
			cv::drawChessboardCorners(frame, pattern_size, corners, found);
			cv::imshow("main board", frame);
			// basic done 
			cv::Mat des;
			cv::Mat des1;
			std::vector<cv::Point2f> image_points;
			frame.copyTo(des);
			frame.copyTo(des1);
				
			// Task 5
			// Project Outside Corners or 3D Axes
			circ(frame, des, rvec, tvec, camera_matrix, distortion_coeffs);
			// Task 6
			// Create a Virtual Object
				cube(frame, des1, rvec, tvec, camera_matrix, distortion_coeffs, image_points);
				cv::imshow("Chessboard points", des);
				cv::imshow("Chessboard cube", des1);
			
			

			
			
		// Check for quit key
		if (cv::waitKey(1) == 'q') {
			break;
		}
		
		
		
		}
	}

	return 0;
}
