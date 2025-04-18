/**
 * Auth :   liubo
 * Date :   2025-05-17 00:00:00
 * Comment: 测试 inline
 */


#pragma once

#include <string>


template<typename T>
struct TypeName 
{
    static std::string value()
    {
        return "undefine";
    }
};

// inline，可以加，也可以不加。
template<>
struct TypeName<int>
{
    static inline std::string value()
    {
        return "Int";
    }
};


template<>
struct TypeName<float>
{
    static std::string value()
    {
        return "Float";
    }
};


