#include "Display.hpp"

Display::Display(const std::string& windowName) : winName(windowName) {
    cv::namedWindow(winName);
    cv::createTrackbar("Intensity", winName, &sliderValue, 100, onTrackbar, this);
}

void Display::show(const cv::Mat& frame) {
    cv::imshow(winName, frame);
}

void Display::setMouseCallback(cv::MouseCallback callback, void* userdata) {
    cv::setMouseCallback(winName, callback, userdata);
}

void Display::onTrackbar(int value, void* userdata) {
    Display* display = static_cast<Display*>(userdata);
    if (display) {
        cv::setWindowProperty(display->winName, cv::WND_PROP_TOPMOST, 1);
        cv::setWindowProperty(display->winName, cv::WND_PROP_TOPMOST, 0);
    }
}
