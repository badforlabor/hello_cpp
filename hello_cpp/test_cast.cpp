
#include "Event.h"

#include <iostream>
#include "AutoRegTestFunc.h"

namespace test_cast
{
    class Base
    {
    public:
        virtual ~Base() {}
        virtual void ShowMe()
        {
            std::cout << "Base" << std::endl;
        }
    };
    class A : public Base
    {
    public:        
        virtual void ShowMe() override
        {
            std::cout << "A" << std::endl;
        }
        Base* Ptr = nullptr;
    };
    class B : public Base
    {
    public:        
        virtual void ShowMe() override
        {
            std::cout << "B" << std::endl;
        }
        int Value = 1;
    };
    
    static void TestCast()
    {
        Base* ptr = new B;
        A* a = static_cast<A*>(ptr);
        a->ShowMe();
        std::cout << a->Ptr << std::endl;
    }
    static AutoRegTestFunc autoTestForward(TestCast);
}
