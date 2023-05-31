#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
    cv::Mat frame;
    cv::VideoCapture cap;
    int deviceID = 0;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;
    while (true) {
        cap.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        cv::imshow("Live", frame);
        if (cv::waitKey(1) >= 0) break;
    }
    return 0;
}
