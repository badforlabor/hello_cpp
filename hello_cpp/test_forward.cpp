/**
 * Auth :   liubo
 * Date :   2023-08-17 00:00:00
 * Comment: 测试std::forward
 * 详细，看这个，解释的更清楚：  https://www.bilibili.com/video/BV1a1SPYaEBY
 * 引用折叠规则：
 *          T& &   -> T&    左值引用 的引用  折叠成 左值引用
 *          T& &&  -> T&    左值引用 的右值引用  折叠成 左值引用
 *          T&& &  -> T&    右值引用 的引用  折叠成 左值引用
 *          T&& && -> T&&   右值引用 的右值引用  折叠成 右值引用
 *          很有规律，就是带左值引用的，就被折叠成左值引用。不带左值引用的，即全部是右值引用的，就会折叠成右值引用！
 */

#include <iostream>
#include <string.h>
#include "AutoRegTestFunc.h"

namespace std_source_code
{
    template<typename T>
    T&& forward(typename std::remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    // 比如，如果是这个：forward<std::string&>，那么转成源码后，得到的返回值是std::string& &&，那么最终得到的就是std::string&（被折叠了）！ 
}

namespace test_forward
{
    void f(const std::string& s)
    {        
        std::cout << "左值：" << s << std::endl;
    }
    void f(std::string&& s)
    {        
        std::cout << "右值：" << s << std::endl;
    }
    
    void g(const std::string& s)
    {        
        f(s);
    }
    void g(std::string&& s)
    {
        // !!! 看这里！ 问，f(s)调用的是左值还是右值引用？
        // 答案是，右值引用。因为s是个临时变量，能取地址（即能&s)，所以是左值！
        f(s);
#if false
        void* p = &s;
        std::cout << "s是能够取地址的！所以是左值！" << p << std::endl;
#endif
    }

    
    void g2(const std::string& s)
    {        
        f(s);
    }
    void g2(std::string&& s)
    {
        // 必须调用forawrd，否则，不会调用f的右值引用！
        f(std::forward<std::string&&>(s));
    }

    static void TestForward2()
    {
        {
            g(std::string("hello1")); // 右值引用！因为无法取地址，是即将消亡的值！所有匿名变量，都是右值引用！
            std::string s2("hello2");
            g(s2); // 左值！因为能取地址！！
        }

        // 这个才是正确写法！
        {
            g2(std::string("hello1"));
            std::string s2("hello2");
            g2(s2); // 左值！因为能取地址！！            
        }
    }


    
    struct A
    {
        int value;

        explicit A(int value) : value(value)
        {
            std::cout << "construct 1" << std::endl;
        }

        explicit A(const A& a) : value(a.value)
        {
            std::cout << "construct 2" << std::endl;
        }

        explicit A() : value(0)
        {
            std::cout << "construct 3" << std::endl;
        }

        A& operator =(const A& a) = default;

        ~A()
        {
            std::cout << "deconstruct" << std::endl;
        }

        void Show() const
        {
            std::cout << "A::Show:" << value << std::endl;
        }
    };

    static void Func1_1(A& a)
    {
        a.value = 2;
        a.Show();
    }

    static void Func1(A&& a)
    {
        a.Show();
        Func1_1(a);

        a.value = 3;
        A b;
        b = a;
        b.Show();
    }

    static void Func1(A& a)
    {
        a.Show();

        a.value = 3;
        A b;
        b = a;
        b.Show();
    }

    static void Func2(A& a)
    {
        a.Show();
    }

    // static void Func3(A& && a)
    // {
    //     a.Show();
    // }
    static void Show(int&& a)
    {
        a = 1;
        std::cout << "右值:" << a << std::endl;
    }

    static void Show(int& a)
    {
        std::cout << "左值:" << a << std::endl;
        Show(std::move(a)); // 将“左值引用”转化成“右值引用”
    }

    static void TestForward()
    {
        std::cout << "TestForward..." << std::endl;
        A a(1);
        A b(a);
        A c;
        c = b;

        a.Show();
        b.Show();
        c.Show();

        Func1(std::move(a));
        Func2(a);
        // Func3(a);

        int aa = 0;
        Show(aa);
        Show(0);
        Show(std::forward<int>(aa)); // 将“左值”转化成“右值引用”
        Show(std::forward<int>(0)); // 将“右值”转化成“右值引用”
        Show(std::forward<int&>(aa));
        Show(std::forward<int&&>(aa));
    }


    static AutoRegTestFunc autoTestForward(TestForward);
    static AutoRegTestFunc autoTestForward2(TestForward2);
}
