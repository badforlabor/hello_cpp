/**
 * Auth :   liubo
 * Date :   2024-10-17 02:41 
 * Comment: 练习Tuple的一些写法
 */


#include <assert.h>
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "AutoRegTestFunc.h"


namespace test_tuple
{
    class FNestedString
    {
    public:
        void Add()
        {
            
        }
        
        template<typename Type, typename ...Types>
        void Add(const Type& Str, const Types& ...Other)
        {
            NestedString.push_back(Str);
            Add(Other...);
        }
        
        template<typename Type, typename ...Types>
        void Add2(int n, const Type& Str, const Types& ...Other)
        {
            NestedString.push_back(Str);
            Add(Other...);
            Add(std::to_string(n));
        }
        template<typename Type, typename ...Types>
        void Add2(const Type& Str, const Types& ...Other, const std::string& n)
        {
            NestedString.push_back(Str);
            Add(Other...);
            Add(n);
        }


        static std::string Concat1(const int& A, const int& B)
        {
            return std::to_string(A+B);
        }   
        static std::string Concat1(const std::string& A, const std::string& B)
        {
            return A + "/" + B;
        }         
        template<typename Type, typename Type2>
        static std::string Concat(const Type& A, const Type2& B)
        {
            return Concat1(A, B);
        }
        template<typename Type, typename ...Types>
        static std::string Concat(const Type& A, const Types& ...B)
        {
            return Concat1(A, Concat(B...));
        }
        
        std::string ToString()
        {
            std::stringstream ss;
            for (auto Str : NestedString)
            {
                if(!ss.str().empty())
                {
                    ss << ",";                    
                }
                ss << Str;
            }
            return ss.str();
        }
        
        static void Test1()
        {
            std::string Ret = FNestedString::Concat("1", "2", "3");             
            std::cout << Ret << std::endl;
            // std::string Ret2 = FNestedString::Concat(1, 2, 3);
            // std::cout << Ret2 << std::endl;
        }
        
        static void Test2()
        {
            FNestedString a;
            a.Add("1", "2", "3");
            a.Add2(5, "4", "4-1");
            a.Add2("6", "7");

#define KG_TT(...) a.Add(__VA_ARGS__)
#define KG_TT2(n, ...) a.Add2(n, __VA_ARGS__)
            KG_TT("a", "b", "c");
            KG_TT2(5, "d", "e");
            
            std::cout << a.ToString() << std::endl;
        }
        
    private:
        std::vector<std::string> NestedString;
    };

    // 参数展开表达式
    class FNestedArgs
    {
    public:
        static void PrintArgs(const char* format, ...)
        {
            va_list args; // 声明一个 va_list 类型的变量
            va_start(args, format); // 初始化 args，使其指向第一个可变参数

            // 处理格式字符串
            while (*format != '\0') {
                if (*format == 'd') { // 整数
                    int i = va_arg(args, int); // 获取下一个参数
                    std::cout << i;
                } else if (*format == 'f') { // 浮点数
                    double d = va_arg(args, double);
                    std::cout << d;
                } else if (*format == 'c') { // 字符
                    int c = va_arg(args, int); // char 在 va_arg 中被提升为 int
                    std::cout << static_cast<char>(c);
                } else if (*format == 's') { // 字符串
                    const char* s = va_arg(args, const char*);
                    std::cout << s;
                }
                ++format; // 移动到下一个格式字符
            }

            va_end(args); // 清理工作
        }
        // 啥也不干，只是为了验证"ToString(Args)..."语法
        template<class T>
        static T ToString(T&& v)
        {
            return v;
        }
        template<typename ...T>
        static void TPrintArgs(const char* format, T...Args)
        {
            // "ToString(Args)..."意思是会递归调用ToString(Args)
            PrintArgs(format, ToString(Args)...);
            std::cout << std::endl;
        }
        
        static void Test3()
        {
            TPrintArgs("%d%s", 1, "23");
        }
    };
    
    class FNestedArgs2
    {
    public:
        
        // 基础情况：当元组为空时，什么也不做
        template <typename Func, typename Tuple, std::size_t... Is>
        static constexpr decltype(auto) apply_impl(Func&& func, Tuple&& tuple, std::index_sequence<Is...>) {
            return std::forward<Func>(func)(std::get<Is>(std::forward<Tuple>(tuple))...);
        }

        // 主函数：用于展开元组并调用函数
        template <typename Func, typename Tuple>
        static constexpr decltype(auto) apply(Func&& func, Tuple&& tuple) {
            return apply_impl(std::forward<Func>(func), std::forward<Tuple>(tuple), 
                              std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
        }
        
        static void printArgs(int a, int b, int c, int d, int e) {
            std::cout << a << " " << b << " " << c << " " << d << " " << e << std::endl;
        }

        // 辅助函数，用于将 vector 转换为 tuple
        template<typename T>
        static auto vectorToTuple(const std::vector<T>& vec) {
            return apply([](auto&&... args) { return std::make_tuple(args...); }, vec);
        }
        
        static void Test4()
        {
            std::vector<int> vec = {1, 2, 3, 4, 5};

            // 通过 std::apply 将 vector 的元素作为参数传递
            // apply(printArgs, vectorToTuple(vec));
        }
    };

    

    static AutoRegTestFunc autoTest1(FNestedString::Test1);
    static AutoRegTestFunc autoTest2(FNestedString::Test2);
    static AutoRegTestFunc autoTest3(FNestedArgs::Test3);
    static AutoRegTestFunc autoTest4(FNestedArgs2::Test4);
}
