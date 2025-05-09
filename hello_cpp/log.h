/**
 * Auth :   liubo
 * Date :   2023-08-18 00:00:00
 * Comment: 日志工具
 */
#pragma once

#define NOMINMAX        // 禁用 Windows 的 min/max 宏
#define WIN32_LEAN_AND_MEAN  // 减少 Windows 头文件包含

#include <iostream>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/fmt/bundled/printf.h"
#include "spdlog/fmt/xchar.h"
#include <iostream>

namespace hellocpp
{
    // 别用这个了，改成用LogHelper::sprintf吧！！！
    
    // 声明
    template <typename... Args>
    void Println(std::stringstream& out, Args&&...);

    // 全特化一个最小的
    template <>
    inline void Println(std::stringstream& out)
    {
        if (out.gcount() > 0)
        {
            out << ",";
        }
    }

    // 定义
    template <typename Args1, typename... Args>
    void Println(std::stringstream& out, Args1&& arg1, Args&&... args)
    {
        if (out.str().size() > 0)
        {
            out << ", ";
        }
        out << arg1;
        Println(out, args...);
    }

    template <typename... Args>
    void Println(Args&&... args)
    {
        std::stringstream out;
        Println(out, args...);
        std::cout << out.str() << std::endl;
    }

    struct FA
    {
        int A;
        float B;
        std::string C;

        FA() = default;
        
        FA(const FA&) = delete;
        FA& operator=(const FA&) = delete;

        friend std::stringstream& operator << (std::stringstream& ss, const FA& a)
        {
            ss << "{A=" << a.A << ", B=" << a.B << ", C=" << a.C << "}";
            return ss;
        }
    };

    inline void TestPrint()
    {
        FA fa;
        fa.A = 1; fa.B = 2.3f; fa.C = "C";
        Println(1, 2.0f, "3", fa);
    }
}


class LogHelper
{
public:
    inline static std::shared_ptr<spdlog::logger> GetLogger()
    {
        static std::shared_ptr<spdlog::logger> logger;

        if (logger != nullptr)
        {
            return logger;
        }

        const char* const LogPath = "logs/hello-cpp-log.txt";

        try
        {
            //// 创建一个基本的文件日志记录器
            //auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
            //// 设置日志级别为调试
            //logger->set_level(spdlog::level::debug);

            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::trace);

            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LogPath, true);
            file_sink->set_level(spdlog::level::debug);

            auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();

            std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink, msvc_sink};

            logger = std::make_shared<spdlog::logger>("basic_logger", sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::trace);

            // 每秒flush
            spdlog::flush_every(std::chrono::seconds(1));
            // 遇到warn的时候，立即flush
            logger->flush_on(spdlog::level::trace);

            // 记录不同级别的日志
            logger->debug("Init logger. This is a debug message");
            logger->info("Init logger. This is an info message");
            logger->warn("Init logger. This is a warning message");
            logger->error("Init logger. This is an error message");

            logger->debug("Init logger. hello, fmt. {:d}, {:.4f}, 0x{:X}", 1, 3.14159f, 0xFF123FF);
            logger->debug(fmt::sprintf("Init logger. hello, fmt. %d, %.4f, 0x%X", 1, 3.14159f, 0xFF123FF));
            logger->flush();
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cout << "Log initialization failed: " << ex.what() << std::endl;
        }

        return logger;
    }
    static std::string wstring_to_utf8(const std::wstring& wstr) {
        if (wstr.empty()) return "";
        
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        std::string str(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
        return str;
    }
    
    template <typename... T>
    static void Fetal(const char* fmt, const T&... args)
    {
        GetLogger()->error(fmt::sprintf(fmt, args...));
        
        assert(0);
    }

    template <typename... T>
    static void Err(const char* fmt, const T&... args)
    {
        GetLogger()->error(fmt::sprintf(fmt, args...));
    }

    template <typename... T>
    static void Warn(const char* fmt, const T&... args)
    {
        GetLogger()->warn(fmt::sprintf(fmt, args...));
    }

    template <typename... T>
    static void Info(const char* fmt, const T&... args)
    {
        GetLogger()->info(fmt::sprintf(fmt, args...));
    }

    template <typename... T>
    static void Err(const std::wstring& fmt, const T&... args)
    {
        std::wstring str = fmt::sprintf(fmt, args...);
        GetLogger()->error(wstring_to_utf8(str));
    }
    template <typename... T>
    static void Warn(const std::wstring& fmt, const T&... args)
    {
        std::wstring str = fmt::sprintf(fmt, args...);
        GetLogger()->warn(wstring_to_utf8(str));
    }
    template <typename... T>
    static void Info(const std::wstring& fmt, const T&... args)
    {
        std::wstring str = fmt::sprintf(fmt, args...);
        GetLogger()->info(wstring_to_utf8(str));
    }

    
    // 使用printf的形式，拼接字符串！！！
    template<typename ...T>
    inline static std::string sprintf(const std::string& format, const T& ... args)
    {
        return fmt::sprintf(format, args...);
    }
    
    template<typename ...T>
    inline static std::wstring sprintf(const std::wstring& format, const T& ... args)
    {
        return fmt::sprintf(format, args...);
    }
};
