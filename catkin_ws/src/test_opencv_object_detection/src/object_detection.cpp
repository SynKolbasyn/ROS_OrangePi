#include <opencv2/opencv.hpp>
#include <iostream>

std::vector<cv::Rect> detectObjects(const cv::Mat& image, cv::CascadeClassifier& cascade) {
	std::vector<cv::Rect> objects;
	cascade.detectMultiScale(image, objects, 1.1, 3);
	return objects;
}

int main() {
	cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(0, cv::CAP_ANY);
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    cv::CascadeClassifier cascade;
	cascade.load("/home/orangepi/Downloads/opencv-4.x/data/haarcascades/haarcascade_frontalface_default.xml");
    std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;
    while (true) {
        cap.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        std::vector<cv::Rect> objects = detectObjects(frame, cascade);	
		std::cout << "Number of objects detected: " << objects.size() << std::endl;
		for (cv::Rect rect : objects) {
			cv::rectangle(frame, rect, cv::Scalar(255,0,0), 5, 8, 0);
		}
        cv::imshow("Live", frame);
        if (cv::waitKey(1) >= 0) break;
    }
    return 0;
}
