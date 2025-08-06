#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include <chrono>
#include <iomanip>

static class Logger {
public:
    static enum Level { DEBUG, INFO, WARN, ERROR };

    static void init(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (logFile.is_open()) {
            logFile.close();
        }
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
	}

    static void log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!logFile.is_open()) return;

        std::string allMessage = "[" + currentDateTime() + "] " + "[" + levelToString(level) + "] " + message;

		logFile << allMessage << std::endl;
        std::cout << allMessage << std::endl;
    }

    static void debug(const std::string& msg) { log(DEBUG, msg); }
    static void info(const std::string& msg) { log(INFO, msg); }
    static void warn(const std::string& msg) { log(WARN, msg); }
    static void error(const std::string& msg) { log(ERROR, msg); }

private:
    static std::ofstream logFile;
    static std::mutex mutex_;

    static std::string levelToString(Level level) {
        switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }

    static std::string currentDateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm;

#ifdef _WIN32
        localtime_s(&local_tm, &now_time_t); // для Windows
#else
        localtime_r(&now_time_t, &local_tm); // для Linux/Unix
#endif

        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};