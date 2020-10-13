#include <opencv2/opencv.hpp>

#include <iostream>

using namespace cv;

int main() {
	// Connecting to a capturing device
	VideoCapture cap(1); // Create capturing object and connect to capturing device
	if (!cap.isOpened()) { // Test if connection was successful
		std::cout << "Could not connect to capturing device!\n";
		return 0;
	}

	// Matrix frame for image information
	Mat frame;

	// Pass image from capturing device to frame
	cap >> frame;

	// Run while valid frames are passed
	while (!frame.empty()) {
		cap >> frame; // Pass image from capturing device to frame
		imshow("Input", frame); // Open/update window that shows input frame

		// Convert to grayscale
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		// Intensity-level slicing
		Mat processed;
		frame.copyTo(processed);
		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {
				uchar pointValue = frame.at<uchar>(i, j);
				if (pointValue > 160 || pointValue < 90) {
					processed.at<uchar>(i, j) = 0;
				}
				else {
					processed.at<uchar>(i, j) = 255;
				}
			}
		}

		imshow("Processed input", processed); // Open/update window that shows processed frame

		// Close on ESC press (also introduces a 10ms delay)
		if (waitKey(10) == 27)
			break;
	}

	return 0;
}