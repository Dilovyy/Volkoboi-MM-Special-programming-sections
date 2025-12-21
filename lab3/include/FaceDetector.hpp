#pragma once
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <atomic>

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();

    void submitFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();

private:
    void worker();
    bool hasNewFrame = false;

    cv::dnn::Net net;

    std::thread th;
    std::mutex mtx;
    std::atomic<bool> running{true};

    cv::Mat frame;
    std::vector<cv::Rect> faces;
};

