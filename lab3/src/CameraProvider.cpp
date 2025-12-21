#include "CameraProvider.hpp"
#include <iostream>
#include "Logger.hpp"
#include "ConfigManager.hpp"

CameraProvider::CameraProvider(int index) {

    auto& cfg = ConfigManager::getInstance();

    cap.open(index, cv::CAP_V4L2);

    if (!cap.isOpened()) {
       // std::cerr << "Cannot open camera\n";
        Logger::getInstance().error("CameraProvider: Cannot open camera with index " + std::to_string(index));
        cap.open(0, cv::CAP_V4L2);
      //  return -1;
    }

    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, cfg.camWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, cfg.camHeight);
    cap.set(cv::CAP_PROP_FPS, 30);
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap.read(frame);
    return frame;
}


