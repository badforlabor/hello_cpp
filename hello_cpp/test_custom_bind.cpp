
#include "custom_bind.h"

#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_custom_bind
{

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
    
    static void TestForward()
    {
        // return;
        
        ClsShow c;
        auto func = wstd1::bind(&ClsShow::Show, &c, std::placeholders::_1, std::placeholders::_2);
        func.call("123", 4);

        
        typedef wstd1::function<void(std::string, int)> ShowFunc1;
        ShowFunc1 func2 = wstd1::bind(&ClsShow::Show, &c, std::placeholders::_1, std::placeholders::_2);
        func2("123", 4);

        // func2 = wstd1::bind(&ClsShow::Show, &c, std::placeholders::_1, 1);
        // func2("123", 4);
        //
        // auto func3 = wstd1::bind(&ClsShow::Show, &c, "0123", std::placeholders::_2);
        // func.call("123", 4);
    }
    
    static AutoRegTestFunc autoTestForward(TestForward);
    
}