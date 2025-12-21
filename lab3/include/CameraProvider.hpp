#pragma once
#include <opencv2/opencv.hpp>

class CameraProvider {
public:
    CameraProvider(int index = 0);
    bool isOpened() const;
    cv::Mat getFrame();

private:
    cv::VideoCapture cap;
};
