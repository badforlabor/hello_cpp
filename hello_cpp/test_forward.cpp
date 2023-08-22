/**
 * Auth :   liubo
 * Date :   2023-08-17 00:00:00
 * Comment: 测试std::forward
 */

#include <iostream>
#include <string.h>
#include "AutoRegTestFunc.h"

namespace test_forward
{
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
}
