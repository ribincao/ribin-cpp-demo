#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <mutex>
#include <execinfo.h>

#define MAX_STACK_TRACE_SIZE 100



class Logger {
public:
    enum LogMode {
        CONSOLE,
        FILE
    };
    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_FATAL
    };
    Logger(LogMode mode) {
     if (mode == LogMode::FILE) {
            const std::string& logFileName = "./server.log";
            m_logFile.open(logFileName.c_str());
        }

        m_logMode = mode;
    }

    virtual ~Logger() {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    void Debug(const std::string& message) {
        log(LogLevel::LOG_DEBUG, message);
    }

    void Info(const std::string& message) {
        log(LogLevel::LOG_INFO, message);
    }

    void Warn(const std::string& message) {
        log(LogLevel::LOG_WARNING, message);
    }

    void Error(const std::string& message) {
        log(LogLevel::LOG_ERROR, message);
    }

    void Fatal(const std::string& message) {
       log(LogLevel::LOG_FATAL, message);
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);

        std::string f_message = format_log(level, message);
        if(f_message == "") {
            return;
        }

        if(m_logMode == LogMode::FILE) {
            m_logFile << f_message << std::endl;
        } else {
            std::cout << f_message << std::endl;
        }
    }

    std::string format_log(LogLevel level, const std::string& message) {
        // 获取当前时间
        time_t rawTime;
        struct tm* timeInfo;
        char timeBuffer[80];
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        strftime(timeBuffer, 80, "%Y-%m-%d %H:%M:%S", timeInfo);
        
        void* trace[MAX_STACK_TRACE_SIZE];
        int size = backtrace(trace, MAX_STACK_TRACE_SIZE);
        char** symbols = backtrace_symbols(trace, size);

        std::string s;
        // 输出日志
        switch (level) {
            case LogLevel::LOG_DEBUG:
                s += "[DEBUG][" + std::string(timeBuffer) + "]" + std::string(message);
                break;
            case LogLevel::LOG_INFO:
                s += "[INFO][" + std::string(timeBuffer) + "]" + std::string(message);
                break;
            case LogLevel::LOG_WARNING:
                s += "[WARNING][" + std::string(timeBuffer) + "]" + std::string(message);
                break;
            case LogLevel::LOG_ERROR:
                s += "[ERROR][" + std::string(timeBuffer) + "]" + std::string(message);
                break;
            case LogLevel::LOG_FATAL:
                s += "[FATAL][" + std::string(timeBuffer) + "]" + std::string(message);
                break;
        default:
            break;
        }
        return s;
    }

private:
    std::ofstream m_logFile;
    LogMode m_logMode;
    std::mutex mutex_; // 互斥锁，保证线程安全
};


