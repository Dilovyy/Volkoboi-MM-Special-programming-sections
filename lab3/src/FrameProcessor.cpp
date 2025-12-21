#include "FrameProcessor.hpp"

FrameProcessor::FrameProcessor() {
    pict = cv::imread("pict/pict.png");
}



cv::Mat FrameProcessor::process(const cv::Mat& frame,
                                KeyProcessor::Mode mode,
                                double zoomLevel, int sliderValue) {
    cv::Mat result = frame.clone();

    switch (mode) {
    case KeyProcessor::Mode::FACE: break;
    case KeyProcessor::Mode::ZOOMARROWS:{
        int newW = frame.cols / zoomLevel;
        int newH = frame.rows / zoomLevel;
        int x = (frame.cols - newW) / 2;
        int y = (frame.rows - newH) / 2;

        cv::Rect roi(x, y, newW, newH);
        roi &= cv::Rect(0, 0, frame.cols, frame.rows); 

       cv::Mat cropped = frame(roi);
       cv::resize(cropped, result, frame.size()); }
        break;

    case KeyProcessor::Mode::DRAWLINE:
        for (size_t i = 0; i < lineStart.size(); i++) {
            cv::line(result, lineStart[i], lineEnd[i], cv::Scalar(255, 255, 0), 2);
        }
        if (drawing) {
            cv::line(result, p1, p2, cv::Scalar(0, 255, 0), 2);
        }
        break;

    case KeyProcessor::Mode::PICTS:
        if (!pict.empty()) {
            int x = 10, y = 10;
            int w = std::min(pict.cols, result.cols - x);
            int h = std::min(pict.rows, result.rows - y);
            if (w > 0 && h > 0) {
                cv::Rect roi(x, y, w, h);
                pict(cv::Rect(0, 0, w, h)).copyTo(result(roi));
            }
        }
        break;

    case KeyProcessor::Mode::INVERT:
        cv::bitwise_not(frame, result);
        break;

    case KeyProcessor::Mode::BLUR:{
        int blurSize = (sliderValue / 10) * 2 + 1;
        blurSize = std::max(1, std::min(blurSize, 99));
        cv::GaussianBlur(result, result, cv::Size(blurSize, blurSize), 0);}
        break;

    case KeyProcessor::Mode::CANNY:{
        cv::Canny(frame, result, 100, 200);}
        break;

    case KeyProcessor::Mode::NORMAL:
    default:{
        int brightness = sliderValue - 50;
        if (brightness != 0) {
           result.convertTo(result, -1, 1, brightness);
        }}
        break;
    }
    return result;
}

void FrameProcessor::onMouse(int event, int x, int y, int flags) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        p1 = cv::Point(x, y);
        p2 = p1;
        drawing = true;
    }
    else if (event == cv::EVENT_MOUSEMOVE && drawing) {
        p2 = cv::Point(x, y);
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        p2 = cv::Point(x, y);
        drawing = false;

        lineStart.push_back(p1);
        lineEnd.push_back(p2);
    }
}
