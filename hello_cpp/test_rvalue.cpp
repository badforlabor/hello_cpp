// Copyright KuaiShou Games, Inc. All Rights Reserved.
// Author: liubo11@kuaishou.com
/**
 * Auth :   liubo
 * Date :   2025-01-23 03:13 
 * Comment: 右值引用！！！
 */

#include <iostream>
#include <string.h>
#include "AutoRegTestFunc.h"

#pragma optimize("", off)
#pragma clang optimize off

namespace test_rvalue
{
    class FRValue
    {
    public:
        FRValue()
        {
            Make(ptr, "default");
            std::cout << "FRValue构造函数" << std::endl;            
        }
        
        FRValue(const char* str)            
        {
            Make(ptr, str);
            std::cout << "FRValue构造函数, str" << std::endl;            
        }
        static void Make(char*& ptr, const char* str)
        {
            int Size = strlen(str);
            ptr = new char[strlen(str) + 2];
            strcpy(ptr, str);
            ptr[Size+0] = 'a';
            ptr[Size+1] = '\0';            
        }

#if false
        FRValue(FRValue&& R) noexcept
        {
            Make(ptr, R.ptr);        
            std::cout << "FRValue移动构造函数" << std::endl;            
        }
#else
        FRValue(FRValue&& R) noexcept   // 如果没有定义移动构造函数，会执行拷贝构造函数！！！
        : ptr(R.ptr)
        {
            R.ptr = nullptr;
            std::cout << "FRValue移动构造函数" << std::endl;            
        }
#endif
        
        FRValue(const FRValue& R)
        {
            Make(ptr, R.ptr);            
            std::cout << "FRValue拷贝构造函数" << std::endl;
        }

        ~FRValue()
        {
            // std::cout << "析构！！" << std::endl;
            printf("析构:%p\n", ptr);
            if(ptr != nullptr)
            {
                delete []ptr;
            }
        }

        void ShowMe() const
        {
            // std::cout << (ptr != nullptr) ? ptr : "" << std::endl;
            printf("FRValue:%p,%s\n", ptr, ptr ? ptr : "");
        }
        
    private:
        char* ptr = nullptr;
    };

    // 返回一个匿名变量，即xvalue！
    FRValue GetValue2()
    {
        FRValue Ret("TestRValue");
        return Ret;
    }
    
    static void TestRValue()
    {
        {
            // https://www.cnblogs.com/kekec/p/11303391.html
            std::cout << "test1...." << std::endl;  
            FRValue Test2 = (GetValue2()); // GetValue2()返回临时变量，所以会调用：移动拷贝构造，但是有可能触发RVO/NRVO，导致被优化掉！
            
            Test2.ShowMe();      
        }  

        {
            std::cout << "test2...." << std::endl;  
            FRValue Test2 = std::move(GetValue2()); // 调用移动构造函数！
            
            Test2.ShowMe();      
        }  

        {
            std::cout << "test3...." << std::endl;
            FRValue Test2; // 默认构造
            FRValue Test3 = Test2; // 拷贝构造函数
            FRValue Test4 = std::move(Test2); // 移动构造函数，执行完毕后Test2.ptr会变成nullptr

            Test3.ShowMe();
            Test4.ShowMe();
        }

        {
            std::cout << "test4...." << std::endl;
            FRValue Test2 = GetValue2(); // GetValue2()返回临时变量，所以会调用：移动拷贝构造
            FRValue Test3 = Test2; // 拷贝构造函数

            Test3.ShowMe();
        }
    }

    static AutoRegTestFunc autoTestForward(TestRValue);
}
