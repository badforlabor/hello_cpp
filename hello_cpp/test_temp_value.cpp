/**
 * Auth :   liubo
 * Date :   2025-03-25 03:02
 * Comment: 测试临时变量的作用域
 */



#include <iostream>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <type_traits>

using namespace std;
#include "log.h"

#include "AutoRegTestFunc.h"

namespace test_temp_value
{
    static int GetValue()
    {
        int a = 10;
        return a;
    }
    static void ShowValue(const int& a)
    {
        std::cout << a << std::endl;
    }
    static void Test3()
    {
        ShowValue(GetValue());
    }
    static void Test4()
    {
        const int& a = GetValue(); 
        ShowValue(a);
    }
    static int GetValue2()
    {
        return 10;
    }
    static void Test5()
    {
        const int& a = GetValue2(); 
        ShowValue(a);
    }
    
    static AutoRegTestFunc autoTestSFinae3(Test3);
    static AutoRegTestFunc autoTestSFinae4(Test4);
    static AutoRegTestFunc autoTestSFinae5(Test5);
    
}




