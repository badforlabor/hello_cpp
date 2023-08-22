/**
 * Auth :   liubo
 * Date :   2023-08-17 00:00:00
 * Comment: 测试function和bind函数
 */

#include <functional>
#include <iostream>
#include <string.h>
#include "AutoRegTestFunc.h"

namespace test_event
{    
    typedef std::function<void(std::string, int)> ShowFunc;

    static void Show(std::string a, int b)
    {
        std::cout << "a=" << a << ", b=" << b << std::endl;
    }
    class ClsShow
    {
    public:
        int ShowValue = 1;
        void Show(std::string a, int b)
        {
            std::cout << "ClsShow::Show, ShowValue=" << ShowValue << ", a=" << a << ", b=" << b << std::endl;
        }

        void Show2(std::string a, int b, float c)
        {
            std::cout << "ClsShow::Show2, ShowValue=" << ShowValue << ", a=" << a << ", b=" << b << ", c=" << c << std::endl;            
        }
    };
    
    static void TestFunction()
    {
        ShowFunc func = Show;
        func("123", 4);

        ClsShow c;
        func = std::bind(&ClsShow::Show, &c, std::placeholders::_1, std::placeholders::_2);
        func("123", 4);

        // function与目标参数不一样的时候，用bind也能绑定成功！
        func = std::bind(&ClsShow::Show2, &c, std::placeholders::_1, std::placeholders::_2, 3.14f);
        func("123", 4);
        
        func = std::bind(&ClsShow::Show2, &c, std::string("000"), std::placeholders::_2, 3.14f);
        func("123", 4);

        func = [](std::string a, int b)
        {
            std::cout << "lambda a=" << a << ", b=" << b << std::endl;    
        };
        func("123", 4);

        int lambdaValue1 = 0;
        
        // lambda，传引用
        func = [&lambdaValue1](std::string a, int b)
        {
            std::cout << "lambda a=" << a << ", b=" << b << ", lambdaValue1=" << lambdaValue1 << std::endl;    
        };
        lambdaValue1 = 1;
        func("123", 4);

        // lambda，传值
        func = [lambdaValue1](std::string a, int b)
        {
            std::cout << "lambda a=" << a << ", b=" << b << ", lambdaValue1=" << lambdaValue1 << std::endl;    
        };
        lambdaValue1 = 2;
        func("123", 4);


        ShowFunc a1 = std::bind(&ClsShow::Show, &c, std::placeholders::_1, std::placeholders::_2);
        ShowFunc a2 = std::bind(&ClsShow::Show, &c, std::placeholders::_1, std::placeholders::_2);
        
    }


    static AutoRegTestFunc autoTestFunction(TestFunction);
}
