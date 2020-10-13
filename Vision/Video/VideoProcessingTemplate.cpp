#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
	// Connecting to a capturing device
	VideoCapture cap; // Object for a capturing device
	if (!cap.open(1)) { // Connect and test if connecting was successful
		std::cout << "Could not connect to capturing device!\n";
		return 0;
	}

	/* Alternative connection code
	VideoCapture cap(1); // Create capturing object and connect to capturing device
	if (!cap.isOpened()) { // Test if connection was successful
		std::cout << "Could not connect to capturing device!\n";
		return 0;
	}
	*/

	// Matrix frame for image information
	Mat frame;

	// Pass image from capturing device to frame
	cap >> frame;

	// Run while valid frames are passed
	while (!frame.empty()) {
		cap >> frame; // Pass image from capturing device to frame

		// PUT YOUR PROCESSING HERE

		imshow("Processed input", frame); // Open/update window that shows processed frame

		// Close on ESC press (also introduces a 10ms delay)
		if (waitKey(10) == 27)
			break;
	}

	return 0;
}