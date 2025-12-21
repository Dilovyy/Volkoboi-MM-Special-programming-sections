#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Display {
public:
    Display(const std::string& windowName = "Lab 3 - VolkoboiMM");
    void show(const cv::Mat& frame);
    void setMouseCallback(cv::MouseCallback callback, void* userdata);
    int getSliderValue() const { return sliderValue; }
private:
    std::string winName;
    int sliderValue = 50;
    static void onTrackbar(int value, void* userdata);
};
