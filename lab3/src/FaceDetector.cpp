#include "FaceDetector.hpp"
#include "ConfigManager.hpp"
#include "Logger.hpp"

//FaceDetector::FaceDetector(const std::string& proto, const std::string& model) { net = cv::dnn::readNetFromCaffe(proto, model);th = std::thread(&FaceDetector::worker, this);}
FaceDetector::FaceDetector() {
    auto& cfg = ConfigManager::getInstance();
    try {
        net = cv::dnn::readNetFromCaffe(cfg.modelProto, cfg.modelWeights);
        Logger::getInstance().info("FaceDetector: Network loaded successfully.");} catch (const std::exception& e) {
        Logger::getInstance().error("FaceDetector: Failed to load network: " + std::string(e.what()));
    }

    running = true;
    th = std::thread(&FaceDetector::worker, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    if (th.joinable()) th.join();
    Logger::getInstance().info("FaceDetector: Thread stopped.");
}

void FaceDetector::submitFrame(const cv::Mat& f) {
    std::lock_guard<std::mutex> lock(mtx);
    frame = f.clone();
    hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return faces;
}

void FaceDetector::worker() {
    while (running) {
        cv::Mat local;
        bool hasFrame = false;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (hasNewFrame && !frame.empty()) {
                local = frame.clone();
                hasNewFrame = false;
                hasFrame = true;
            }
        }

        if (!hasFrame) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }


        cv::Mat blob = cv::dnn::blobFromImage(
            local, 1.0, cv::Size(300, 300),
            cv::Scalar(104.0, 177.0, 123.0)
        );
        net.setInput(blob);
        cv::Mat detections = net.forward();



        std::vector<cv::Rect> found;
        cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

        for (int i = 0; i < detectionMat.rows; i++) {
            float confidence = detectionMat.at<float>(i, 2);

            if (confidence > 0.5f) {
                int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * local.cols);
                int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * local.rows);
                int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * local.cols);
                int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * local.rows);

                x1 = std::max(0, std::min(x1, local.cols));
                y1 = std::max(0, std::min(y1, local.rows));
                x2 = std::max(0, std::min(x2, local.cols));
                y2 = std::max(0, std::min(y2, local.rows));

                if (x2 > x1 && y2 > y1) {
                    found.emplace_back(cv::Point(x1, y1), cv::Point(x2, y2));
                }
            }
        }


        {
            std::lock_guard<std::mutex> lock(mtx);
            faces = found;
        }
       // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


