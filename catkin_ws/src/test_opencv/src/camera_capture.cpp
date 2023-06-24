#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

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
        cv::Mat hist_equalized_image;
        cv::cvtColor(frame, hist_equalized_image, cv::COLOR_BGR2YCrCb);
        std::vector<cv::Mat> vec_channels;
        cv::split(hist_equalized_image, vec_channels);
        cv::equalizeHist(vec_channels[0], vec_channels[0]);
        cv::merge(vec_channels, hist_equalized_image);
        cv::cvtColor(hist_equalized_image, hist_equalized_image, cv::COLOR_YCrCb2BGR);
        cv::imshow("Equlized live", hist_equalized_image);
        cv::imshow("Live", frame);
        if (cv::waitKey(1) >= 0) break;
    }
    return 0;
}
