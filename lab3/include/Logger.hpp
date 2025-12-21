#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4
};

class Logger {
public:
    // Видаляємо методи копіювання, щоб не можна було створити копію логера
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    // Статичний метод доступу
    static Logger& getInstance() {
        static Logger instance; // Створюється один раз при першому виклику
        return instance;
    }

    void setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        currentLevel = level;
    }
    LogLevel getLogLevel() const {
        return currentLevel;
    }


    void flush() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.flush();
        }
    }


    void info(const std::string& msg)  { log(LogLevel::INFO, msg); }
    void warn(const std::string& msg)  { log(LogLevel::WARN, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }
    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }


private:
    Logger() {
        auto now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );

        std::tm tm{};
        localtime_r(&now, &tm);

        std::stringstream ss;
        ss << "log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".log";

        logFile.open(ss.str(), std::ios::app);

        if (!logFile.is_open()) {
            std::cerr << "Could not create log file\n";
        }
    }

    ~Logger() {
        if (logFile.is_open())
            logFile.close();
    }

    void log(LogLevel level, const std::string& message) {
        if (level < currentLevel) return;

        std::lock_guard<std::mutex> lock(logMutex);

        std::stringstream formatted;
        formatted << "[" << getTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message;

        std::cout << formatted.str() << std::endl;

        if (logFile.is_open())
            logFile << formatted.str() << std::endl;
    }

    std::string getTimestamp() {
        auto now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );

        std::tm tm{};
        localtime_r(&now, &tm);

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::TRACE: return "TRACE";
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    std::mutex logMutex;
    std::ofstream logFile;
    LogLevel currentLevel = LogLevel::INFO;
};
