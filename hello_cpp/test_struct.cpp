
#include "Event.h"

#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_struct
{
    struct StructA
    {
        StructA(float vb) : Vb(vb)
        {
        }
        
        int Va = 1;
        float Vb;
    };
    
    static void TestStruct()
    {
#if false
        StructA a; // 编译失败，提示没有构造函数
#else
        StructA a(2.1f);
#endif        
        std::cout<< a.Va << std::endl;
    }
    static AutoRegTestFunc autoTestForward(TestStruct);
}
