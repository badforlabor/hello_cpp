// Copyright KuaiShou Games, Inc. All Rights Reserved.
// Author: liubo11@kuaishou.com
/**
 * Auth :   liubo
 * Date :   2024-12-11 09:15 
 * Comment: 理解SFINAE的意义：
 *          1. 模板编程时，未成功命中的不会报错，而是尝试找下一个，如果都找不到，才报错！
 *          2. 编译时，推到的结果，即便理论上会引起编译错误，但是没命中，也不算错误，也忽略掉。（简单理解就是，没命中的，不编译，所以不会有编译错误）
 */ 

#include <iostream>
#include <assert.h>
#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_sfinae
{   
    // Condition是false的时候，命中这里
    template<bool Condition, typename T>
    struct enable_if {};

    // 偏特化： Condition是true的时候，命中这里
    template<typename T>
    struct enable_if<true, T>
    {
        using type = T;
    };

    // 假设T的size是大于8的，那么命中的结果是"typename enable_if<sizeof(T) <= sizeof(void*), T>::type 返回的是空"，此时按理是编译错误，但认为是“不命中”，即SFINAE
    //          不命中，所以会继续找同名函数，直到命中，如果都没命中，才报错！
    template<typename T>
    int TestSFINAE(typename enable_if<sizeof(T) <= sizeof(void*), T>::type t)
    {
        return 1;
    }


    template<typename T>
    int TestSFINAE(typename enable_if<(sizeof(T) > sizeof(void*)), T>::type const &t)
    {
        return 2;
    }

    // 简化表达式
    template<typename T>
    struct IsLessThan
    {
        constexpr static bool Value = sizeof(T) <= sizeof(void*);
    };
    template<typename T>
    struct Not
    {
        constexpr static bool Value = !T::Value;
    };
    
    template<typename T>
    int TestSFINAE2(typename enable_if<IsLessThan<T>::Value, T>::type t)
    {
        return 1;
    }
    
    template<typename T>
    int TestSFINAE2(typename enable_if<!IsLessThan<T>::Value, T>::type t)
    {
        return 2;
    }
    
    template<typename T>
    int TestSFINAE2_2(typename enable_if<Not<IsLessThan<T>>::Value, T>::type t)
    {
        return 2;
    }

    // 再次简化
    template<bool Condition, typename T>
    using enable_if_t = typename enable_if<Condition, T>::type;

    template<typename T>
    constexpr bool IsLessThan_t = IsLessThan<T>::Value;

    template<typename T>
    int TestSFINAE3(enable_if_t<IsLessThan<T>::Value, T> t)
    {
        return 1;
    }
    
    template<typename T>
    int TestSFINAE3(enable_if_t<!IsLessThan<T>::Value, T> t)
    {
        return 2;
    }
    
    template<typename T>
    int TestSFINAE4(enable_if_t<IsLessThan_t<T>, T> t)
    {
        return 1;
    }
    
    template<typename T>
    int TestSFINAE4(enable_if_t<!IsLessThan_t<T>, T> t)
    {
        return 2;
    }
    
    struct TestA
    {
        int A[999] = {0};
        float B = 0;
        std::string C;

        void ShowMe()
        {
            int TempD = 0;
            auto Func = [&, TempD]()
            {
                std::cout << A  << TempD << std::endl;
            };
            Func();
        }
    };

    static void Test1()
    {
        {
            int A = 0;
            assert(TestSFINAE<int>(A) == 1);        
        }
        {
            TestA A;
            assert(TestSFINAE<TestA>(A) == 2);
        }
        
        {
            int A = 0;
            assert(TestSFINAE2<int>(A) == 1);        
        }
        {
            TestA A;
            assert(TestSFINAE2<TestA>(A) == 2);
        }
        
        {
            int A = 0;
            assert(TestSFINAE3<int>(A) == 1);        
        }
        {
            TestA A;
            assert(TestSFINAE3<TestA>(A) == 2);
        }    
        
        {
            int A = 0;
            assert(TestSFINAE4<int>(A) == 1);        
        }
        {
            TestA A;
            assert(TestSFINAE4<TestA>(A) == 2);
        }      
    }
    
    static AutoRegTestFunc autoTestForward3(Test1);
}
