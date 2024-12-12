#pragma once

#include <string>
#include <mutex>
#include <iostream>
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
        INFO,
        WARNING,
        ERROR
    };

    template <typename... Args>
    static void log(Level level, std::format_string<Args...> fmt, Args&&... args);

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

    static constexpr const char *levelToString(const Level level);
    static std::string getCurrentTime();
};

#include "Logger.ipp"
