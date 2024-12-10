// https://www.bilibili.com/video/BV1xzBdYME17
#include "assert.h"
#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_decltype
{

    // 判定是值类型： std::is_arithmetic<decltype(a)>::value
    // 判定是引用类型： std::is_reference<decltype(b)>::value
    
    // 是否是const int&
    template<typename T>
    constexpr bool isConstIntRef() {
        return std::is_same<const int&, T>::value;
    }
    
    // 是纯右值、左值、还是右值引用
    template<typename T>
    struct value_type
    {
        static constexpr int value = 1; 
    };
    template<typename T>
    struct value_type<T&>
    {
        static constexpr int value = 2; 
    };
    template<typename T>
    struct value_type<T&&>
    {
        static constexpr int value = 3; 
    };

    struct S1
    {
        int A = 0;
    };
    
    static void TestDeclType()
    {
        {
            int a = 1;
            decltype(a) b = a; // 相当于： int b = a;
            static_assert(std::is_arithmetic<decltype(a)>::value);
        }
        
        {
            int a = 1;
            int& b = a;
            decltype(b) c = a; // 相当于： int& c = a;
            static_assert(std::is_reference<decltype(b)>::value);
        }
        
        {
            decltype(S1::A) a = 1; // 相当于 int a = 1;
            S1 s1;
            decltype(s1.A) b = 1; // 相当于 int b = 1;
        }

        // 与auto的区别
        {
            const int& a = 1;
            const int& b = a;
            decltype(b) c = a; // 相当于是const int& c = a;
            auto d = b; // 相当于是int d = b, auto只能根据初始化表达式推断，无法推导出cv限定符，而decltype可以根据类型推断
            // cv限定符，指的是 const, volatile     
            static_assert(isConstIntRef<decltype(b)>());    // 是const int&
            static_assert(std::is_reference<decltype(b)>::value);    // 是引用类型
            static_assert(std::is_arithmetic<decltype(d)>::value); // 是值类型
        }

        {
            // decltype(auto)，会根据初始值表达式进行推导
            // 相当于是：  decltype(初始值表达式) 变量名字 = 初始值表达式
            std::string s1 = "hello";
            decltype(auto) s2 = s1 + ",world"; // 相当于是 std::string s2 = s1+",world";
            decltype(auto) s3 = s1.append(",world"); // 相当于是 std::string& s3 = s1.append(",world");
            assert(s2 == "hello,world");
            assert(s3 == "hello,world");
            assert(s1 == "hello,world");            
        }
    }

    std::string f1()
    {
        return std::string("f1");
    }
    std::string& f2()
    {
        static std::string ret = std::string("f2");
        return ret;
    }
    std::string&& f3(std::string& s)
    {
        return std::move(s);
    }

    
    // 表达式
    static void TestDeclType2()
    {
        /* 规则是：
         *      delctype(表达式）的推导：
         *         如果是左值(lvalue)，那么返回“类型引用”
         *         如果是纯右值(prvalue)，那么返回“类型”
         *         如果是xvalue，那么返回右值引用
         *
         * 
         */
        
        {
            int a = 1;
            // 特别注意，(a)是个表达式，且返回的是左值，所以decltype((a))返回的是引用
            // 为啥(a)是左值，很简单，能这样写： (a)=1，所以是左值咯；而且还能这么写int*p = &(a)
            decltype((a)) b = a; // 相当于是int& b = a
            static_assert(std::is_reference<decltype((a))>::value);
        }

        {
            // 右值引用，类型引用，类型
            int&& a = 0;
            int b = 0;
            int& c = b;
            assert((std::is_same<int&&, decltype(a)>::value));
            static_assert(std::is_same<int&&, decltype(a)>::value);
            static_assert(std::is_same<int, decltype(b)>::value);
            static_assert(std::is_same<int&, decltype(c)>::value);
        }

        // 这个更有意思！
        {
            // 如果传递的是函数，并不会真正的执行！只是推导而已！
            decltype(f1()) s1; // 值
            decltype(f2()) s2 = s1; // 引用！
            decltype(f3(s1)) s3 = "hello"; // 右值引用！
            
            static_assert(std::is_same<std::string, decltype(s1)>::value);
            static_assert(std::is_same<std::string&, decltype(s2)>::value);
            static_assert(std::is_same<std::string&&, decltype(s3)>::value);
        }

        // 这个也要分清！
        {
            std::string s1 = "hello";
            decltype(s1 + ",world") s2 = s1; // 相当于是 std::string s2 = s1
            decltype(s1.append(",world")) s3 = s1; // 相当于是 std::string& s3 = s1
            assert(s3 == "hello"); // decltype(s1.append(",world"))中的append并不会真正执行，只是用来推导的，所以不会影响s1/s3的结果
            assert(s1 == "hello");
            
            static_assert(std::is_same<std::string, decltype(s1)>::value);
            static_assert(std::is_same<std::string, decltype(s1 + ",world")>::value);
            static_assert(std::is_same<std::string&, decltype(s1.append(",world"))>::value);

            static_assert(value_type<decltype(s1)>::value == 1);
            static_assert(value_type<decltype(s1.append(",world"))>::value == 2);
        }        
    }

    /*
     * 更高级的应用
     */
    // c++14的写法
    template<typename T, typename U>
    decltype(auto) add(T&& t, U&& u)
    {
        return std::forward<T>(t) + std::forward<U>(u);
    }

    // c++11的写法, ->表示推导返回类型！ 直接问chatgpt即可！
    template<typename T, typename U>
    auto add_c11(T&& t, U&& u) -> decltype(std::forward<T>(t) + std::forward<U>(u)) 
    {
        return std::forward<T>(t) + std::forward<U>(u);
    }

    // 返回的是引用类型的， 相当于是int&
    decltype(auto) f2(int i)
    {
        // (i)，是个表达式，相当于左值
        return (i);
    }
    
    // 返回的是值类型的， 相当于是int
    decltype(auto) f4(int i)
    {
        // (i+1)，是个右值！
        return (i+1);
    }
    
    static void TestDeclType3()
    {
        {
            // 匿名变量，一定是右值，所以返回值，也是右值
            static_assert(std::is_same<std::string, decltype(add(std::string("hello"), std::string(",world")))>::value);
        }
        {
            std::string s1("hello");
            std::string s2("world");
            static_assert(std::is_same<std::string, decltype(add(s1, s2))>::value);
            // static_assert(std::is_reference<decltype(add(s1, s2))>::value);
            // static_assert(std::is_arithmetic<decltype(add(s1, s2))>::value);
        }
        {
            int a = 1;
            static_assert(std::is_same<int&, decltype(f2(a))>::value);
            static_assert(std::is_reference<decltype(f2(a))>::value);
            
            static_assert(std::is_same<int, decltype(f4(a))>::value);
            static_assert(std::is_arithmetic<decltype(f4(a))>::value);
        }
    }
    
    static AutoRegTestFunc autoTestForward(TestDeclType);
    static AutoRegTestFunc autoTestForward2(TestDeclType2);
    static AutoRegTestFunc autoTestForward3(TestDeclType3);
}
