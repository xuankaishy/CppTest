#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <string>
#include <ctime>
#include <thread>
#include <iomanip>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    void setLogLevel(LogLevel level)
    {
        m_log_level = level;
    }

    void setLogFile(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(m_log_file_mutex);
        if (m_file_stream.is_open())
        {
            m_file_stream.close();
        }
        m_file_stream.open(filename, std::ios::app);
    }

    void log(LogLevel level, const std::string& message) 
    {
        if (level < m_log_level)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_log_file_mutex);
        std::stringstream ss;
        ss << getCurrentTime() << " [" << getLevelName(level) << "] "
            << "[Thread " << std::this_thread::get_id() << "] "
            << message << std::endl;

        if (m_file_stream.is_open())
        {
            m_file_stream << ss.str();
        }
        else
        {
            std::cout << ss.str();
        }
    }

    static std::string getCurrentTime() {
        std::time_t currentTime = std::time(nullptr);  // 获取当前时间
        struct tm localTime;

        // 使用 localtime_s 将 time_t 转换为本地时间的 tm 结构
        if (localtime_s(&localTime, &currentTime) == 0) {
            // 创建字符串流
            std::ostringstream oss;
            // 将格式化时间写入字符串流
            oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
            return oss.str();  // 返回格式化后的字符串
        }
        else {
            return "Error converting time";  // 返回错误信息
        }
    }

private:
    Logger() : m_log_level(LogLevel::INFO) {}

    std::string getLevelName(LogLevel level) {
        switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
        }
    }

    LogLevel m_log_level;
    std::ofstream m_file_stream;
    std::mutex m_log_file_mutex;
};

// 宏来简化日志调用
#define LOG_DEBUG(message) Logger::getInstance().log(LogLevel::DEBUG, message)
#define LOG_INFO(message) Logger::getInstance().log(LogLevel::INFO, message)
#define LOG_WARN(message) Logger::getInstance().log(LogLevel::WARN, message)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::ERROR, message)

// 使用Logger的示例
//int main() {
//    Logger::getInstance().setLogFile("log.txt");
//    Logger::getInstance().setLogLevel(m_log_level::DEBUG);
//
//    LOG_INFO("This is an info log.");
//    LOG_DEBUG("This is a debug log.");
//    LOG_WARN("This is a warning.");
//    LOG_ERROR("This is an error.");
//
//    return 0;
//}


