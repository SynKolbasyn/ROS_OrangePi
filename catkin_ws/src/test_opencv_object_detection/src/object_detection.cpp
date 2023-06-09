#include <opencv2/opencv.hpp>
#include <iostream>/*
#include <fstream>

const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;
 
// Text parameters.
const float FONT_SCALE = 0.7;
const int FONT_FACE = cv::FONT_HERSHEY_SIMPLEX;
const int THICKNESS = 1;
 
// Colors.
cv::Scalar BLACK = cv::Scalar(0,0,0);
cv::Scalar BLUE = cv::Scalar(255, 178, 50);
cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
cv::Scalar RED = cv::Scalar(0,0,255);

void draw_label(cv::Mat& input_image, std::string label, int left, int top) {
    // Display the label at the top of the bounding box.
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
    top = std::max(top, label_size.height);
    // Top left corner.
    cv::Point tlc = cv::Point(left, top);
    // Bottom right corner.
    cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);
    // Draw white rectangle.
    cv::rectangle(input_image, tlc, brc, BLACK, cv::FILLED);
    // Put the label on the black rectangle.
    cv::putText(input_image, label, cv::Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}

std::vector<cv::Mat> pre_process(cv::Mat &input_image, cv::dnn::Net &net) {
    // Convert to blob.
    cv::Mat blob;
    cv::dnn::blobFromImage(input_image, blob, 1./255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);
 
    net.setInput(blob);
 
    // Forward propagate.
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());
 
    return outputs;
}

cv::Mat post_process(cv::Mat &input_image, std::vector<cv::Mat> &outputs, const std::vector<std::string> &class_name) {
    // Initialize vectors to hold respective outputs while unwrapping     detections.
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    // Resizing factor.
    float x_factor = input_image.cols / INPUT_WIDTH;
    float y_factor = input_image.rows / INPUT_HEIGHT;
    float *data = (float *)outputs[0].data;
    const int dimensions = 85;
    // 25200 for default size 640.
    const int rows = 25200;
    // Iterate through 25200 detections.
    for (int i = 0; i < rows; ++i) {
        float confidence = data[4];
        // Discard bad detections and continue.
        if (confidence >= CONFIDENCE_THRESHOLD) {
            float * classes_scores = data + 5;
            // Create a 1x85 Mat and store class scores of 80 classes.
            cv::Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
            // Perform minMaxLoc and acquire the index of best class  score.
            cv::Point class_id;
            double max_class_score;
            minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            // Continue if the class score is above the threshold.
            if (max_class_score > SCORE_THRESHOLD) {
                // Store class ID and confidence in the pre-defined respective vectors.
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);
                // Center.
                float cx = data[0];
                float cy = data[1];
                // Box dimension.
                float w = data[2];
                float h = data[3];
                // Bounding box coordinates.
                int left = int((cx - 0.5 * w) * x_factor);
                int top = int((cy - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                // Store good detections in the boxes vector.
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        // Jump to the next row.
        data += 85;
    }
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    for (int i = 0; i < indices.size(); i++) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        int left = box.x;
        int top = box.y;
        int width = box.width;
        int height = box.height;
        // Draw bounding box.
        cv::rectangle(input_image, cv::Point(left, top), cv::Point(left + width, top + height), BLUE, 3*THICKNESS);
        // Get the label for the class name and its confidence.
        std::string label = cv::format("%.2f", confidences[idx]);
        label = class_name[class_ids[idx]] + ":" + label;
        // Draw class labels.
        draw_label(input_image, label, left, top);
    }
    return input_image;
}
*/
std::vector<cv::Rect> detectObjects(const cv::Mat& image, cv::CascadeClassifier& cascade) {
	std::vector<cv::Rect> objects;
	cascade.detectMultiScale(image, objects, 1.1, 3);
	return objects;
}

int main() {
	std::cout << "OpenCV version: " << CV_VERSION << std::endl;
	/*
	std::vector<std::string> class_list;
    std::ifstream ifs("/home/orangepi/Downloads/classes.txt");
    std::string line;
    while (getline(ifs, line)) {
        class_list.push_back(line);
	}*/
	cv::Mat frame;
	cv::namedWindow("Live");
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cv::CascadeClassifier cascade;
	cascade.load("/home/orangepi/Downloads/opencv-4.x/data/haarcascades/haarcascade_frontalface_default.xml");/*
	cv::dnn::Net net = cv::dnn::readNet("/home/orangepi/Downloads/YOLOv5/yolov5n.onnx");
    std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;*/
    while (true) {
        cap.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // resize(frame, frame, cv::Size(720, 480), cv::INTER_LINEAR);
        std::vector<cv::Rect> objects = detectObjects(frame, cascade);	
		std::cout << "Number of objects detected: " << objects.size() << std::endl;
		for (cv::Rect rect : objects) {
			cv::rectangle(frame, rect, cv::Scalar(255,0,0), 5, 8, 0);
		}/*
		std::vector<cv::Mat> detections;     // Process the image.
		detections = pre_process(frame, net);
		cv::Mat img = post_process(frame, detections, class_list);
		// Put efficiency information.
		// The function getPerfProfile returns the overall time for     inference(t) and the timings for each of the layers(in layersTimes).
		std::vector<double> layersTimes;
		double freq = cv::getTickFrequency() / 1000;
		double t = net.getPerfProfile(layersTimes) / freq;
		std::string label = cv::format("Inference time : %.2f ms", t);
		putText(img, label, cv::Point(20, 40), FONT_FACE, FONT_SCALE, RED);*/
        cv::imshow("Live", frame);
        if (cv::waitKey(1) >= 0) break;
    }
    cap.release();
    return 0;
}
