/**
 * Auth :   liubo
 * Date :   2023-08-18 00:00:00
 * Comment: 测试模板
 */

#include <iostream>
#include <sstream>
#include <tuple>

#include "AutoRegTestFunc.h"
#include "log.h"


namespace test_template
{
#ifdef CPP20
    template <typename T, T value>
    constexpr T ConstValue = value;
    constexpr auto const ConstInt10 = ConstValue<int, 10>;
    constexpr auto const ConstInt20 = ConstValue<int, 20>;
    constexpr auto const ConstFloat1 = ConstValue<float, 1.1>;
    constexpr auto const ConstFloat2 = ConstValue<float, 2.1>;
    constexpr auto const ConstString1 = ConstValue<const char*, "123">;
    constexpr auto const ConstString2 = ConstValue<const char*, "456">;
#else    
    template <typename T, T value>
    constexpr T ConstValue = value;
    constexpr auto const ConstInt10 = ConstValue<int, 10>;
    constexpr auto const ConstInt20 = ConstValue<int, 20>;
    constexpr auto const ConstFloat1 = 1.1f;
    constexpr auto const ConstFloat2 = 2.1f;
    constexpr auto const ConstString1 = "123";
    constexpr auto const ConstString2 = "456";
#endif


    // 非类型的模板参数
    template<typename T1, T1 t, typename T2 = decltype(t)>
    T2 Get()
    {
        return t;
    }

    // 答应
    void print()
    {
        std::cout << std::endl;
    }
    template<typename T1, typename ...T>
    void print(T1&& t, T&&...ts)
    {
        std::cout << t << "\t";
        print(ts...);
    }
    
    template<typename ...T>
    void printDouble(T&& ...ts)
    {
        print(ts+ts ...);
    }

    template<typename FN, typename ...T>
    void Show()
    {
        std::tuple<int, float, std::string> values = std::make_tuple (1, 2, "3");
        int a = std::get<0>(values);
        std::cout << a << std::endl;
    }
    template<typename ...T1>
    void DumpTuple(std::tuple<T1...>&& t)
    {
        std::cout << std::get<0>(t) << std::endl;
    }
    
    static void Test1()
    {
        hellocpp::TestPrint();
        hellocpp::Println(ConstInt10, ConstInt20, ConstFloat1, ConstFloat2, ConstString1, ConstString2);

        int a = Get<int, 1>();
        hellocpp::Println(a);
        
        printDouble(1.1f, 2.2f);

        
        std::tuple<int, float, std::string> vv = std::make_tuple(1, 2.1f, "3");
        // DumpTuple<int, float, std::string>(vv);
        
    }

    static AutoRegTestFunc autoTest1(Test1);
}
