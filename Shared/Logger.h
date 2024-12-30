#pragma once

#include <string>
#include <mutex>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <format>

class Logger
{
public:
    enum class Level
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    static void setLogLevel(Level level);
    static void setOutputFile(const std::string& filename);
    static void disableFileLogging();

    template <typename... Args>
    static void log(Level level, std::format_string<Args...> fmt, Args&&... args);

    template <typename... Args>
    static void debug(std::format_string<Args...> fmt, Args&&... args);

    template <typename... Args>
    static void info(std::format_string<Args...> fmt, Args&&... args);

    template <typename... Args>
    static void warning(std::format_string<Args...> fmt, Args&&... args);

    template <typename... Args>
    static void error(std::format_string<Args...> fmt, Args&&... args);

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static inline std::mutex logMutex;
    static inline std::ofstream fileStream;
    static inline bool logToFile = false;
    static inline Level currentLevel = Level::INFO;

    static constexpr const char *levelToString(const Level level);
    static inline std::string getCurrentTime();
};

#include "Logger.ipp"
