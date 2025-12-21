#pragma once
#include <string>
#include <fstream>
#include "json.hpp"
#include "Logger.hpp"

using json = nlohmann::json;

class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    int cameraId = 0;
    int camWidth = 640;
    int camHeight = 480;

    std::string modelProto = "deploy.prototxt";
    std::string modelWeights = "res10_300x300_ssd_iter_140000.caffemodel";
    float confidenceThreshold = 0.5f;

    std::string logLevel = "INFO";

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            Logger::getInstance().warn("Settings file not found! Using default");
            return;
        }

        try {
            json j;
            file >> j;

            if (j.contains("camera")) {
                cameraId = j["camera"].value("id", cameraId);
                camWidth = j["camera"].value("width", camWidth);
                camHeight = j["camera"].value("height", camHeight);
            }

            if (j.contains("model")) {
                modelProto = j["model"].value("proto", modelProto);
                modelWeights = j["model"].value("weights", modelWeights);
                confidenceThreshold = j["model"].value("threshold", confidenceThreshold);
            }

            if (j.contains("logging")) {
                logLevel = j["logging"].value("level", logLevel);
            }

            Logger::getInstance().info("Configuration loaded successfully from " + filename);
        } catch (const json::exception& e) {
            Logger::getInstance().error("JSON Parse Error: " + std::string(e.what()) + ". Using defaults.");
        }
    }

private:
    ConfigManager() {}
};
