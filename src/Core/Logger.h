#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "Assert.h"


#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

namespace Frost
{
    class Logger
    {
    public:
        enum class Level { Info, Warn, Error };

        struct Entry
        {
            Level       level;
            std::string message;
            std::string timestamp;
        };

        static Logger& Get()
        {
            static Logger instance;
            return instance;
        }

        void Init(bool consoleOutput = true, bool retain = true)
        {
            FROST_ASSERT_ALWAYS(!m_Initialized, "Logger already initialized");
            m_ConsoleOutput = consoleOutput;
            m_Retain = retain;
            m_Initialized = true;
        }

        template <typename... Args>
        void Log(const std::string& msg, Args&&... args)
        {
            Print(Level::Info, Format(msg, std::forward<Args>(args)...));
        }

        template <typename... Args>
        void Warn(const std::string& msg, Args&&... args)
        {
            Print(Level::Warn, Format(msg, std::forward<Args>(args)...));
        }

        template <typename... Args>
        void Error(const std::string& msg, Args&&... args)
        {
            Print(Level::Error, Format(msg, std::forward<Args>(args)...));
        }

        const std::vector<Entry>& GetEntries() const { return m_Entries; }
        void                      ClearEntries() { m_Entries.clear(); }

    private:
        Logger() = default;
        ~Logger() = default;
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void Print(Level level, const std::string& msg)
        {
            FROST_ASSERT_ALWAYS(m_Initialized, "Logger not initialized");

            std::string full = Prefix(level) + GetTimestamp() + " - " + msg;

            if (m_ConsoleOutput)
            {
                SetColor(level);
                std::cout << full << "\n";
                ResetColor();
            }

            if (m_Retain)
                m_Entries.push_back({ level, msg, GetTimestamp() });
        }

        std::string Prefix(Level level)
        {
            switch (level)
            {
            case Level::Info:  return "[INFO]  ";
            case Level::Warn:  return "[WARN]  ";
            case Level::Error: return "[ERROR] ";
            default:           return "[?]     ";
            }
        }

        void SetColor(Level level)
        {
#ifdef _WIN32
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            switch (level)
            {
            case Level::Info:  SetConsoleTextAttribute(h, 2); break;
            case Level::Warn:  SetConsoleTextAttribute(h, 6); break;
            case Level::Error: SetConsoleTextAttribute(h, 4); break;
            }
#else
            switch (level)
            {
            case Level::Info:  std::cout << "\033[32m"; break;
            case Level::Warn:  std::cout << "\033[33m"; break;
            case Level::Error: std::cout << "\033[31m"; break;
            }
#endif
        }

        void ResetColor()
        {
#ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
            std::cout << "\033[0m";
#endif
        }

        std::string GetTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::ostringstream oss;
            oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }

        template <typename... Args>
        std::string Format(const std::string& fmt, Args&&... args)
        {
            char buffer[2048];
            std::snprintf(buffer, sizeof(buffer),
                fmt.c_str(), std::forward<Args>(args)...);
            return std::string(buffer);
        }

        bool               m_Initialized = false;
        bool               m_ConsoleOutput = true;
        bool               m_Retain = true;
        std::vector<Entry> m_Entries;
    };

} // namespace Frost

// 
// macros
// 
#define FROST_INIT_LOG(console, retain) Frost::Logger::Get().Init(console, retain)
#define FROST_LOG(msg, ...)             Frost::Logger::Get().Log(msg,   ##__VA_ARGS__)
#define FROST_WARN(msg, ...)            Frost::Logger::Get().Warn(msg,  ##__VA_ARGS__)
#define FROST_ERROR(msg, ...)           Frost::Logger::Get().Error(msg, ##__VA_ARGS__)