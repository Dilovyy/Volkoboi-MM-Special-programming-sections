#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    FrameProcessor();

    cv::Mat process(const cv::Mat& frame,
                    KeyProcessor::Mode mode, double zoomLevel, int sliderValue);

    void onMouse(int event, int x, int y, int flags);

private:
    bool drawing = false;
    cv::Point p1, p2;
    std::vector<cv::Point> lineStart;
    std::vector<cv::Point> lineEnd;
    cv::Mat pict;
};

