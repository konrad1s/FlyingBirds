template <typename... Args>
void Logger::log(Level level, std::format_string<Args...> fmt, Args&&... args)
{
    if (static_cast<int>(level) < static_cast<int>(currentLevel))
    {
        return;
    }

    try
    {
        std::string message  = std::format(fmt, std::forward<Args>(args)...);
        std::string time     = getCurrentTime();
        const char* levelStr = levelToString(level);
        std::string finalMsg = std::format("[{}] [{}] {}\n", levelStr, time, message);

        std::lock_guard<std::mutex> lock(logMutex);

        if (logToFile && fileStream.is_open())
        {
            fileStream << finalMsg;
            fileStream.flush();
        }

        if (level == Level::ERROR)
            std::cerr << finalMsg;
        else
            std::cout << finalMsg;
    }
    catch (const std::format_error &e)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cerr << "[Logger] Formatting failed: " << e.what() << "\n";
    }
    catch (const std::exception &e)
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cerr << "[Logger] Unexpected error: " << e.what() << "\n";
    }
}

template <typename... Args>
void Logger::info(std::format_string<Args...> fmt, Args &&...args)
{
    log(Level::INFO, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::debug(std::format_string<Args...> fmt, Args &&...args)
{
    log(Level::DEBUG, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::warning(std::format_string<Args...> fmt, Args &&...args)
{
    log(Level::WARNING, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::error(std::format_string<Args...> fmt, Args &&...args)
{
    log(Level::ERROR, fmt, std::forward<Args>(args)...);
}

constexpr const char *Logger::levelToString(const Level level)
{
    switch (level)
    {
    case Level::INFO:
        return "INFO";
    case Level::DEBUG:
        return "DEBUG";
    case Level::WARNING:
        return "WARNING";
    case Level::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

inline std::string Logger::getCurrentTime()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto nowTime = system_clock::to_time_t(now);

    std::tm tm;

#ifdef _WIN32
    if (localtime_s(&tm, &nowTime) != 0)
    {
        throw std::runtime_error("Failed to get local time using localtime_s.");
    }
#else
    if (localtime_r(&nowTime, &tm) == nullptr)
    {
        throw std::runtime_error("Failed to get local time using localtime_r.");
    }
#endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}
