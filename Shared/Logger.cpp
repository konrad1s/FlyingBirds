#include "Logger.h"

void Logger::setLogLevel(Level level)
{
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::setOutputFile(const std::string& filename)
{
    std::lock_guard<std::mutex> lock(logMutex);

    if (fileStream.is_open())
    {
        fileStream.close();
    }

    fileStream.open(filename, std::ios::out | std::ios::app);
    if (!fileStream)
    {
        std::cerr << "[Logger] Failed to open log file: " << filename << '\n';
        logToFile = false;
    }
    else
    {
        logToFile = true;
    }
}

void Logger::disableFileLogging()
{
    std::lock_guard<std::mutex> lock(logMutex);

    if (fileStream.is_open())
    {
        fileStream.close();
    }

    logToFile = false;
}
