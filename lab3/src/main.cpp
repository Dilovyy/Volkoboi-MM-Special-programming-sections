#include <opencv2/opencv.hpp>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp"

FrameProcessor* g_fp = nullptr;

void mouseCallback(int event, int x, int y, int flags, void*) {
    if (g_fp)
        g_fp->onMouse(event, x, y, flags);
}


int main() {
    ConfigManager::getInstance().load("settings.json");
    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
    Logger::getInstance().info("Application started");

    Logger::getInstance().info("Camera ID: " + std::to_string(ConfigManager::getInstance().cameraId));
    CameraProvider camera(ConfigManager::getInstance().cameraId);

   // if (!camera.isOpened()) {
     //   Logger::getInstance().info("Couldnt open the camera, using default");
        //camera.open(0);
   // }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display;

    FaceDetector faceDetector;
//("models/deploy.prototxt","models/res10_300x300_ssd_iter_140000.caffemodel");

    g_fp = &frameProcessor;

    //const std::string windowName = "Lab 3 - VolkoboiMM";
    //cv::namedWindow(windowName);
    //cv::setMouseCallback(windowName, mouseCallback);

    display.setMouseCallback(mouseCallback, nullptr);
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    double fps = 0.0;


    while (true) {
        int key = cv::waitKey(1);
        if (key == 27) break; // ESC
 //       if(key !=-1){
   //     std::stringstream ss;
     //   ss << "Key pressed: " << key << " Mode changed to: " << (int)keyProcessor.getMode();
       // Logger::getInstance().debug(ss.str());}

        keyProcessor.processKey(key);
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        cv::Mat processed = frameProcessor.process(frame, keyProcessor.getMode(), keyProcessor.getZoom(), display.getSliderValue());

        if (keyProcessor.getMode() == KeyProcessor::Mode::FACE) {
            faceDetector.submitFrame(frame);
            auto faces = faceDetector.getFaces();

            for (const auto& face : faces) {
                cv::rectangle(processed, face, cv::Scalar(0, 255, 0), 2);
        }}

        // Розрахунок FPS
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        
        if (elapsed.count() >= 1.0) {
            fps = frameCount / elapsed.count();
            frameCount = 0;
            startTime = currentTime;
        }
        
        // Додавання FPS на екран (нижній лівий кут)
        std::string fpsInfo = cv::format("FPS: %.1f", fps);
        cv::putText(processed, fpsInfo, 
                   cv::Point(10, processed.rows - 10),
                   cv::FONT_HERSHEY_SIMPLEX, 0.7, 
                   cv::Scalar(255, 255, 255), 2);
        
        // Додавання режиму на екран (нижній лівий кут, вище FPS)
        std::string modeText;
        switch (keyProcessor.getMode()) {
            case KeyProcessor::Mode::NORMAL: modeText = "Mode: Normal"; break;
            case KeyProcessor::Mode::INVERT: modeText = "Mode: Invert"; break;
            case KeyProcessor::Mode::BLUR: modeText = "Mode: Blur"; break;
            case KeyProcessor::Mode::CANNY: modeText = "Mode: Canny"; break;
            case KeyProcessor::Mode::ZOOMARROWS: modeText = "Mode: Zoom"; break;
            case KeyProcessor::Mode::DRAWLINE: modeText = "Mode: Draw"; break;
            case KeyProcessor::Mode::PICTS: modeText = "Mode: Picture"; break;
            case KeyProcessor::Mode::FACE: modeText = "Mode: Face Detection"; break;
        }
        
        cv::putText(processed, modeText, 
                   cv::Point(10, processed.rows - 40),
                   cv::FONT_HERSHEY_SIMPLEX, 0.7, 
                   cv::Scalar(255, 255, 255), 2);

        display.show(processed);
    }

    cv::destroyAllWindows();
    Logger::getInstance().info("Application ended");
    return 0;
}
