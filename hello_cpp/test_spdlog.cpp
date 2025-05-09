/**
* Auth :   liubo
 * Date :   2025-04-18 00:00:00
 * Comment: 日志工具
 */

#define NOMINMAX        // 禁用 Windows 的 min/max 宏
#define WIN32_LEAN_AND_MEAN  // 减少 Windows 头文件包含

#include "log.h"
#include "AutoRegTestFunc.h"

namespace test_spdlog
{
    static void Test1()
    {
        {
            std::string s1 = fmt::format("str={}, float={}, int={}", "123", 1.0f, 2);
            LogHelper::GetLogger()->info(s1);
            
            std::wstring s2 = fmt::format(L"str={}, float={}, int={}", L"123", 1.0f, 2);       // 如果用widechar，就必须都是widechar，否则会产生编译错误！！
            LogHelper::Info(s2);
        }
        {
            std::string s1 = fmt::sprintf("str=%s, float=%.2f, int=%d", "123", 1.0f, 2);
            LogHelper::GetLogger()->info(s1);
            
            std::wstring s2 = fmt::sprintf(L"str=%s, float=%.2f, int=%d", L"123", 1.0f, 2);
            LogHelper::Info(s2);
        }
    }
    static AutoRegTestFunc autoTestParametarPack(Test1);
}

