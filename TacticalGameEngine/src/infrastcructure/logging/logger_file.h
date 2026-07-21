#pragma once

#include "../../core/interfaces/ILogger.h"

#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Engine::Infrastructure::Logging
{
    class LoggerFile : public Core::Interfaces::ILogger
    {
    private:
        std::ofstream log_file_;

        std::string getCurrentTimestamp() const
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }

        void writeLog(const std::string &level, const std::string &message)
        {
            if (log_file_.is_open())
            {
                log_file_ << "[" << getCurrentTimestamp() << "] [" << level << "] " << message << std::endl;
            }
        }

    public:
        explicit LoggerFile(const std::string &filename = "logs/app.log")
        {
            std::filesystem::path log_path(filename);
            if (log_path.has_parent_path())
            {
                std::filesystem::create_directories(log_path.parent_path());
            }
            log_file_.open(filename, std::ios::app);
        }

        ~LoggerFile() override
        {
            if (log_file_.is_open())
            {
                log_file_.close();
            }
        }

        LoggerFile(const LoggerFile &) = delete;
        LoggerFile &operator=(const LoggerFile &) = delete;
        LoggerFile(LoggerFile &&other) noexcept = default;
        LoggerFile &operator=(LoggerFile &&other) noexcept = default;

        void info(const std::string &message) override
        {
            writeLog("INFO", message);
        }

        void warn(const std::string &message) override
        {
            writeLog("WARN", message);
        }

        void errror(const std::string &message) override
        {
            writeLog("ERROR", message);
        }
    };
}