/**
 * Auth :   liubo
 * Date :   2023-08-18 00:00:00
 * Comment: 日志工具
 */
#pragma once

#include <iostream>
#include <sstream>

namespace hellocpp
{
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
